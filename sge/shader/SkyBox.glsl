// -----------------------------------------------------
## vertex
// -----------------------------------------------------

area main{

    input vec3 a_position;
    
    uniform mat4 u_sge_mvpMatrix;
    
    mat4 transform=u_sge_mvpMatrix;
    transform[3]= vec4(0,0,0,0);
        
    vec4 pos = transform * vec4(a_position, 1.0);
    output vec3 v_3Duv = a_position.xyz;
    pos.z=pos.w*0.999999;
    output.position = pos;
    
}

// -----------------------------------------------------
## fragment
// -----------------------------------------------------

area main{

    input vec3 v_3Duv;
    
    uniform samplerCube u_cubeMap;
    output.color = texture(u_cubeMap, v_3Duv)*2;
    
}
