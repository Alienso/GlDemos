#version 430 core

struct RayTracingMaterial{
    vec4 color;
    vec3 emissionColor;
    float emissionStrength;
};

struct Sphere{
    vec3 position;
    float radius;
    RayTracingMaterial material;
};

in vec2 TexCoords;

out vec4 FragColor;

uniform float uTime;
uniform vec4 uMouse;
uniform vec2 uResolution;

uniform int uSphereCount;
uniform uint uFramesRendered;
uniform Sphere uSpheres[6];

uniform int uResetBuffer;
uniform sampler2D uPrevFrame;

#define RAYS_PER_PIXEL 10
#define MAX_BOUNCE 15

vec3 GroundColour = vec3(0.35,0.3,0.35);
vec3 SkyColourHorizon = vec3(1,1,1);
vec3 SkyColourZenith = vec3(0.08,0.36,0.72);
float SunFocus = 50;
float SunIntensity = 20;

//--scene data---------------------------------------------------------------------

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
        if (dist > 0){
            hitInfo.didHit = true;
            hitInfo.dist = dist;
            hitInfo.hitPoint = ray.origin + ray.dir * dist;
            hitInfo.normal = normalize(hitInfo.hitPoint - sphereCenter);
        }
    }

    return hitInfo;
}

HitInfo CalculateRayCollision(Ray ray){
    HitInfo closest;
    //TODO init
    closest.dist = 99999;

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
    float x = randomValueNormalDistribution(state);
    float y = randomValueNormalDistribution(state);
    float z = randomValueNormalDistribution(state);

    return normalize(vec3(x,y,z));
}

vec3 randomHemisphereDirection(vec3 normal, inout uint state){
    vec3 dir = randomDirection(state);
    return dir * sign(dot(normal, dir));
}

vec3 getEnvironmentLight(Ray ray){

    float skyGradientT = pow(smoothstep(0.0, 0.4, ray.dir.y), 0.35);
    vec3 skyGradient = mix(SkyColourHorizon, SkyColourZenith, skyGradientT);
    //float sun = pow(max(0, dot(ray.dir, normalize(uSpheres[2].position))), SunFocus) * SunIntensity;

    float groundToSkyT = smoothstep(-0.01, 0.0, ray.dir.y);
    //float sunMask = float(groundToSkyT >= 1);
    vec3 composite = mix(GroundColour, skyGradient, groundToSkyT);// + sun * sunMask;
    return composite;
}

vec3 trace(Ray ray, inout uint state){

    vec3 incomingLight = vec3(0,0,0);
    vec3 rayColor = vec3(1,1,1);

    for (int i=0; i<MAX_BOUNCE; i++){
        HitInfo hitInfo = CalculateRayCollision(ray);
        if (hitInfo.didHit){
            ray.origin = hitInfo.hitPoint;
            vec3 diffuseDir = normalize(hitInfo.normal + randomDirection(state));
            vec3 specularDir = reflect(ray.dir, hitInfo.normal);
            ray.dir = mix(diffuseDir, specularDir, 0.0);
            //ray.dir = randomHemisphereDirection(hitInfo.normal, state);

            vec3 emittedLight = hitInfo.material.emissionColor * hitInfo.material.emissionStrength;
            incomingLight += emittedLight * rayColor;
            rayColor*= hitInfo.material.color.xyz;
        }
        else{
            incomingLight += getEnvironmentLight(ray) * rayColor;
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

    uv = uv * 2.0 - 1.0;//transform from [0,1] to [-1,1]
    uv.x *= iResolution.x / iResolution.y; //aspect fix

    vec3 cameraPos = vec3(0,6.0,-25.0);//camera pos animation
    vec3 cameraFocusPoint = vec3(0,0,0);//camera look target point animation
    vec3 cameraDir = normalize(cameraFocusPoint - cameraPos);
    vec3 rayDir = normalize(cameraDir + vec3(uv,0));

    Ray ray;
    ray.origin = cameraPos;
    ray.dir = rayDir;

    //vec3 spherePos = vec3(0,0,-1);
    //HitInfo hitInfo = RaySphere(ray, spherePos, 2);
    //FragColor = vec4(hitInfo.didHit,hitInfo.didHit,hitInfo.didHit,1);

    //HitInfo hitInfo = CalculateRayCollision(ray);
    //FragColor = hitInfo.material.color;

    uint pixelIndex = uint(gl_FragCoord.y * uResolution.x + gl_FragCoord.x);
    //float value = pixelIndex / (uResolution.x * uResolution.y);
    //vec3 value = randomDirection(pixelIndex);
    //FragColor = vec4(value,1);

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

    //HitInfo hitInfo = RaySphere(ray, vec3(0,0,0), 2);
    //FragColor = vec4(hitInfo.didHit,hitInfo.didHit,hitInfo.didHit,1);
    //FragColor = vec4(incomingLight,1);


    //if (uSpheres[2].position.x == 0 && uSpheres[2].position.y == -1002 && uSpheres[2].position.z == 0){
    //if (uSpheres[2].material.emissionStrength == 1){
    //    FragColor = vec4(1.0,0.0,0.0,1.0);
    //}
    //else{
    //    FragColor = vec4(0.0,1.0,0.0,1.0);
    //}
}