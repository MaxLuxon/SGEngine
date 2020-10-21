vec3 simpleFog(in vec3 color, in vec3 fogColor, float distanceZ){

    vec3 outColor= mix(color, fogColor, min(1-exp(-max(distanceZ-7.5,0.0)*0.01),1.0));
    return outColor;
    
}

vec3 fog(in vec3 color, float distanceZ, in vec3 viewDir, in vec3 position, in vec3 sunDir,in vec3 skyColor, in vec3 sunColor){
    
    const float c=0.1;
    
    // Fog Color
    float sunAmount =  pow( max( dot( viewDir, -sunDir ), 0.0 ) ,8.0);
    vec3  fogColor  = mix( skyColor*sunColor*0.06,  sunColor*0.1, 1-sunAmount);
    
    // Fog amounts
    float fogAmountHeight = 1.0/(abs(position.y)*c+1.0);
    
    vec3 be= vec3(1.085, 1.172,1.349)*0.007*fogAmountHeight;
    vec3 bs= vec3(0.785, 1.172,1.249)*0.004*fogAmountHeight;
    
    vec3 extinction = vec3( exp(-distanceZ*be.x), exp(-distanceZ*be.y), exp(-distanceZ*be.z) );
    
    vec3 scattering =  vec3( exp(-distanceZ*bs.x), exp(-distanceZ*bs.y), exp(-distanceZ*bs.z) );
    scattering=vec3(1)-scattering;
    
    vec3 endColor = color.rgb*(extinction);
    endColor+=scattering*fogColor;
    
    return endColor;
    
    
}
