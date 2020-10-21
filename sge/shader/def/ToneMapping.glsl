/*
 
 This shader experiments the effect of different tone mapping operators.
 This is still a work in progress.
 
 More info:
 http://slideshare.net/ozlael/hable-john-uncharted2-hdr-lighting
 http://filmicgames.com/archives/75
 http://filmicgames.com/archives/183
 http://filmicgames.com/archives/190
 http://imdoingitwrong.wordpress.com/2010/08/19/why-reinhard-desaturates-my-blacks-3/
 http://mynameismjp.wordpress.com/2010/04/30/a-closer-look-at-tone-mapping/
 http://renderwonk.com/publications/s2010-color-course/
 
 --
 Zavie
 
 */

vec3 colorGrading(vec3 in_color, sampler2D colorRamp){
    
    return vec3(texture2D(colorRamp, vec2(in_color.r,0.5)).r,
                texture2D(colorRamp, vec2(in_color.g,0.5)).g,
                texture2D(colorRamp, vec2(in_color.b,0.5)).b);
    
}

vec3 gammaCorrection(vec3 color, float gamma){

    return pow(color, vec3(1.0/gamma));

}

vec3 linearToneMapping(vec3 color){
    
    float exposure = 1.;
    color = clamp(exposure * color, 0., 1.);
    return color;

}

vec3 simpleReinhardToneMapping(vec3 color){
    
    float exposure = 1.5;
    color *= exposure/(1. + color / exposure);
    return color;
}

vec3 lumaBasedReinhardToneMapping(vec3 color){
    
    float luma = dot(color, vec3(0.2126, 0.7152, 0.0722));
    float toneMappedLuma = luma / (1. + luma);
    color *= toneMappedLuma / luma;
    return color;
}

vec3 whitePreservingLumaBasedReinhardToneMapping(vec3 color){
    
    float white = 2.;
    float luma = dot(color, vec3(0.2126, 0.7152, 0.0722));
    float toneMappedLuma = luma * (1. + luma / (white*white)) / (1. + luma);
    color *= toneMappedLuma / luma;
    return color;
}

vec3 RomBinDaHouseToneMapping(vec3 color){
    
    color = exp( -1.0 / ( 2.72*color + 0.15 ) );
    return color;
}

vec3 filmicToneMapping(vec3 color){
    
    color = max(vec3(0.), color - vec3(0.004));
    color = (color * (6.2 * color + .5)) / (color * (6.2 * color + 1.7) + 0.06);
    return color;
}

vec3 Uncharted2ToneMapping(vec3 color){
    
    float A = 0.15;
    float B = 0.50;
    float C = 0.10;
    float D = 0.20;
    float E = 0.02;
    float F = 0.30;
    float W = 11.2;
    float exposure = 2.;
    color *= exposure;
    color = ((color * (A * color + C * B) + D * E) / (color * (A * color + B) + D * F)) - E / F;
    float white = ((W * (A * W + C * B) + D * E) / (W * (A * W + B) + D * F)) - E / F;
    color /= white;
    return color;
    
}
