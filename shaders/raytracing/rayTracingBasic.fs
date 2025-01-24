#version 460 core

struct RayTracingMaterial{
    vec4 color;
    vec3 emissionColor;
    float emissionStrength;
    int isInvisibleLightSource;
};

struct Sphere{
    vec3 position;
    float radius;
    RayTracingMaterial material;
};

struct Triangle{
    vec3 posA, posB, posC;
    vec3 normalA, normalB, normalC;
};

struct MeshInfo{
    int firstTriangleIndex;
    int numTriangles;
    RayTracingMaterial material;
    vec3 boundsMin;
    vec3 boundsMax;
    bool isPortal;
    vec3 portalPoint1;
    vec3 portalPoint2;
    mat4 portalRotationMatrix;
};

struct Ray{
    vec3 origin;
    vec3 dir;
};

struct HitInfo{
    bool didHit;
    float dist;
    vec3 hitPoint;
    vec3 normal;
    RayTracingMaterial material;
};

in vec2 TexCoords;

out vec4 FragColor;

uniform float uTime;
uniform vec4 uMouse;
uniform vec2 uResolution;
uniform vec3 uCameraPos;
uniform vec3 uCameraFocusPoint;
uniform uint uFramesRendered;

uniform mat4 uCameraTransformationMatrix;

uniform MeshInfo uMeshInfo[10];
uniform int uMeshCount;

uniform Sphere uSpheres[10];
uniform int uSphereCount;

uniform vec3 lightPositions[10];

uniform int uResetBuffer;
uniform sampler2D uPrevFrame;

layout(std430, binding = 3) readonly buffer vertex_buffer
{
    Triangle bTriangles[];
};

#define RAYS_PER_PIXEL 10
#define MAX_BOUNCE 15
#define MAX_PORTAL_DEPTH 5

vec3 GroundColour = vec3(0.35,0.3,0.35);
vec3 SkyColourHorizon = vec3(1,1,1);
vec3 SkyColourZenith = vec3(0.08,0.36,0.72);
float SunFocus = 50;
float SunIntensity = 20;

//--scene data---------------------------------------------------------------------

HitInfo RaySphere(Ray ray, vec3 sphereCenter, float sphereRadius){
    HitInfo hitInfo;
    hitInfo.didHit = false;
    vec3 offsetRayOrigin = ray.origin - sphereCenter;

    float a = dot(ray.dir, ray.dir);
    float b = 2 * dot(offsetRayOrigin, ray.dir);
    float c = dot(offsetRayOrigin, offsetRayOrigin) - sphereRadius * sphereRadius;

    float discriminant = b * b - 4 * a * c;

    //if discriminant is < 0, then we didnt hit the circle
    if (discriminant >= 0){
        float dist = (-b - sqrt(discriminant)) / (2 * a);
        dist = max(0,dist);
        hitInfo.didHit = (dist > 0);
        hitInfo.dist = dist;
        hitInfo.hitPoint = ray.origin + ray.dir * dist;
        hitInfo.normal = normalize(hitInfo.hitPoint - sphereCenter);
    }

    return hitInfo;
}

HitInfo RayTriangle(Ray ray, Triangle tri){
    vec3 edgeAB = tri.posB - tri.posA;
    vec3 edgeAC = tri.posC - tri.posA;
    vec3 normalVector = cross(edgeAB, edgeAC);
    vec3 ao = ray.origin - tri.posA;
    vec3 dao = cross(ao, ray.dir);

    float determinant = -dot(ray.dir, normalVector);
    float invDet = 1 / determinant;

    // Calculate dst to triangle & barycentric coordinates of intersection point
    float dst = dot(ao, normalVector) * invDet;
    float u = dot(edgeAC, dao) * invDet;
    float v = -dot(edgeAB, dao) * invDet;
    float w = 1 - u - v;

    // Initialize hit info
    HitInfo hitInfo;
    hitInfo.didHit = determinant >= 1E-6 && dst >= 0 && u >= 0 && v >= 0 && w >= 0;
    hitInfo.hitPoint = ray.origin + ray.dir * dst;
    hitInfo.normal = normalize(tri.normalA * w + tri.normalB * u + tri.normalC * v);
    hitInfo.dist = dst;
    return hitInfo;
}

bool rayBoundingBox(Ray ray, vec3 boxMin, vec3 boxMax){
    vec3 invDir = 1 / ray.dir;
    vec3 tMin = (boxMin - ray.origin) * invDir;
    vec3 tMax = (boxMax - ray.origin) * invDir;
    vec3 t1 = min(tMin, tMax);
    vec3 t2 = max(tMin, tMax);
    float tNear = max(max(t1.x, t1.y), t1.z);
    float tFar = min(min(t2.x, t2.y), t2.z);
    return tNear <= tFar;
}

HitInfo CalculateRayCollision(Ray ray){
    HitInfo closest;
    closest.dist = 99999;
    closest.didHit = false;

    for (int i=0; i< uSphereCount; i++){
        Sphere sphere = uSpheres[i];
        HitInfo hitInfo = RaySphere(ray, sphere.position, sphere.radius);

        if (hitInfo.didHit && hitInfo.dist < closest.dist){
            closest = hitInfo;
            closest.material = sphere.material;
        }
    }

    return closest;
}

float randomValue(inout uint state){
    state = state * 747796405 + 2891336453;
    uint result = ((state >> ((state >>28) + 4)) ^ state) * 2778033737;
    result = (result >> 22) ^ result;
    return result / 4294967295.0;
}

float randomValueNormalDistribution(inout uint state){
    float theta = 2 * 3.1415926 * randomValue(state);
    float rho = sqrt( -2 * log(randomValue(state)));
    return rho * cos(theta);
}

vec3 randomDirection(inout uint state){
    //float x = randomValueNormalDistribution(state);
    //float y = randomValueNormalDistribution(state);
    //float z = randomValueNormalDistribution(state);

    float x = randomValue(state);
    float y = randomValue(state);
    float z = randomValue(state);

    return normalize(vec3(x,y,z) * 2 - 1);
}

vec3 getEnvironmentLight(Ray ray){

    float skyGradientT = pow(smoothstep(0.0, 0.4, ray.dir.y), 0.35);
    vec3 skyGradient = mix(SkyColourHorizon, SkyColourZenith, skyGradientT);

    float groundToSkyT = smoothstep(-0.01, 0.0, ray.dir.y);
    vec3 composite = mix(GroundColour, skyGradient, groundToSkyT);// + sun * sunMask;
    return composite;
}

vec3 trace(Ray ray, inout uint state){

    vec3 incomingLight = vec3(0,0,0);
    vec3 rayColor = vec3(1,1,1);
    int portalDepth = 0;

    for (int i=0; i<MAX_BOUNCE; i++){

        if (i == MAX_BOUNCE - 1){
            // TODO fix light positions
            // TODO in basic sphere test, white sphere gets light at the bottom but it shouldn't
            ray.dir = normalize(lightPositions[0] - ray.origin);
        }

        HitInfo hitInfo = CalculateRayCollision(ray);
        if (hitInfo.didHit){

            ray.origin = hitInfo.hitPoint;
            if (hitInfo.material.isInvisibleLightSource == 1 && i == 0){
                ray.origin = hitInfo.hitPoint + ray.dir * 0.001;
                continue;
            }

            ray.dir = normalize(hitInfo.normal + randomDirection(state));

            vec3 emittedLight = hitInfo.material.emissionColor * hitInfo.material.emissionStrength;
            incomingLight += emittedLight * rayColor;

            rayColor*= hitInfo.material.color.xyz;

            float p = max(rayColor.r, max(rayColor.g, rayColor.b));
            if (randomValue(state) >= p) {
                break;
            }
            rayColor *= 1.0f / p;
        }
        else{
            //incomingLight += getEnvironmentLight(ray) * rayColor;
            break;
        }
    }

    return incomingLight;
}

void main(){

    float iTime = uTime;
    vec2 iResolution = uResolution;
    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = gl_FragCoord.xy/iResolution.xy;

    uv = uv * 2.0 - 1.0;//transform from [0,1] to [-1,1] //this is FOV
    uv.x *= iResolution.x / iResolution.y; //aspect fix

    vec3 cameraDir = uCameraFocusPoint;
    vec3 rayDir = normalize((vec4(uv,1,1) * uCameraTransformationMatrix).xyz);

    Ray ray;
    ray.origin = uCameraPos;
    ray.dir = rayDir;

    uint pixelIndex = uint(gl_FragCoord.y * uResolution.x + gl_FragCoord.x);

    uint state = pixelIndex + uFramesRendered * 719393;

    vec3 incomingLight = vec3(0,0,0);
    for (int i=0; i<RAYS_PER_PIXEL; i++){
        incomingLight += trace(ray, state);
    }

    incomingLight /= RAYS_PER_PIXEL;

    vec2 texPos = ( gl_FragCoord.xy / uResolution );
    vec3 oldColor = texture2D(uPrevFrame, texPos).xyz;

    float weight = 1.0 / (uFramesRendered + 1);

    if (uResetBuffer == 1){
        FragColor = vec4(incomingLight,1.0);
    }
    else{
        FragColor = vec4(oldColor.xyz * (1 - weight) + incomingLight * weight, 1);
    }
}