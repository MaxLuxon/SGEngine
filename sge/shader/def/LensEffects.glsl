float vignette(vec2 uv, float gradientLength){

    float len = length(uv - vec2(0.5));
    
    return  smoothstep(0.3, 0.6, len);
  

}

vec3 chromaticAberration(vec2 in_uv, sampler2D in_texture){

    float dist= 0.5+(1.0-distance(in_uv, vec2(0.5, 0.5))*2.0)*0.5;
    
    // lens distortion coefficient (between
    float k = -0.15*0.1;
    
    // cubic distortion value
    float kcube = 0.15*0.1;
    
    float r2 = (in_uv.x-0.5) * (in_uv.x-0.5) + (in_uv.y-0.5) * (in_uv.y-0.5);
    float f = 0.0;
    
    //only compute the cubic distortion if necessary
    if( kcube == 0.0){
        f = 1.0 + r2 * k;
    }else{
        f = 1.0 + r2 * (k + kcube * sqrt(r2));
    };
    
    // get the right pixel for the current position
    float x = f*(in_uv.x-0.5)+0.5;
    float y = f*(in_uv.y-0.5)+0.5;
    
    vec2 uv=vec2(x,y);
    vec4 inputDistord;
    
    float aberration=0.002*pow((1.0-dist),1.4);
    
    inputDistord.r = texture2D(in_texture, (uv)*1.0).r;
    inputDistord.g = texture2D(in_texture, (uv)*1.0-(aberration)).g;
    inputDistord.b = texture2D(in_texture, (uv)*1.0-(aberration*2.0)).b;
    
    return inputDistord.rgb;

}

float sunBleeding(vec2 in_uv, vec3 sunDir, vec3 viewDir){

    return 0;
}
