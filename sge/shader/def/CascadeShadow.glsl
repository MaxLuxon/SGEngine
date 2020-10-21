float cascadeShadow2Splits(in sampler2D shadowMap[2],in vec4 shadowCoords[2] , in float distanceZ, in vec4 splits){

    const float bias = 0.0015;

    /// Shadow
    float visibilty=1.0;
    
    vec2 poissonDisk[16];
    poissonDisk[0]= vec2( -0.94201624,  -0.39906216 );
    poissonDisk[1]= vec2(  0.94558609,  -0.76890725 );
    poissonDisk[2]= vec2( -0.094184101, -0.92938870  );
    poissonDisk[3]= vec2(  0.34495938,   0.29387760  );
    poissonDisk[4]= vec2( -0.91588581,   0.45771432);
    poissonDisk[5]= vec2( -0.81544232,  -0.87912464);
    poissonDisk[6]= vec2( -0.38277543,   0.27676845 );
    poissonDisk[7]= vec2(  0.97484398,   0.75648379);
    poissonDisk[8]= vec2(  0.44323325,  -0.97511554  );
    poissonDisk[9]= vec2(  0.53742981,  -0.47373420 );
    poissonDisk[10]= vec2(-0.26496911,  -0.41893023 );
    poissonDisk[11]= vec2( 0.79197514,   0.19090188 );
    poissonDisk[12]= vec2(-0.24188840,   0.99706507);
    poissonDisk[13]= vec2(-0.81409955,   0.91437590 );
    poissonDisk[14]= vec2( 0.19984126,   0.78641367 );
    poissonDisk[15]= vec2( 0.14383161,  -0.14100790 );
    
    
    for (int i=0; i<1; i++) {
        
        float splitLength=distance(splits[i+1],splits[i]);
        float splitMiddle=splits[i]+splitLength*0.5;
        float splitDistanceFromMiddle= distance(distanceZ,splitMiddle)/splitLength*2.0;
        
      //  float amount= max(sign(1.0-splitDistanceFromMiddle),0.0);
        float amount= 1-clamp(pow(distance(shadowCoords[i].xy, vec2(0.5))*2,10),0,1);
        
        for (int p=0;p<16;p++){
            
            
               visibilty-=1./16.*max(sign((shadowCoords[i].z-bias)-texture2D( shadowMap[i], shadowCoords[i].xy+poissonDisk[p]/2048.0 ).r),0.0)*amount;
                
            
        }
        
    }
    
    return clamp(visibilty,0,1);
    
}
