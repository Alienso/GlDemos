#version 430 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform vec2 uResolution;
uniform uint uBlurStr;

void main()
{
    vec3 accumulate = vec3(0,0,0);

    int blurStr = int(uBlurStr);
    int counter = 0;

    for(int i = -blurStr; i<=blurStr; i++){
        for (int j = -blurStr; j<=blurStr; j++){
            if (i*i + j*j <= blurStr * blurStr){
                vec2 offset = vec2(i,j) / uResolution + TexCoords;
                //if (offset.x < -1 || offset.y < -1){
                //    accumulate = texture2D(screenTexture, TexCoords).rgb * ((blurStr + 1) * (blurStr + 1));
                //    break;
                //}
                accumulate = accumulate + texture2D(screenTexture, offset).rgb;
                counter = counter + 1;
            }
        }
    }

    //accumulate = accumulate / ((2 * blurStr + 1) * (2 * blurStr + 1));
    accumulate = accumulate / counter;
    FragColor = vec4(accumulate, 1.0);
}

