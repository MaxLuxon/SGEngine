// -----------------------------------------------------
## vertex
// -----------------------------------------------------

area main{

    input vec3 a_position (loc:0);
    input vec2 a_uv       (loc:1);
    input vec3 a_color    (loc:2);

    output vec2 m_uv     =  a_uv;
    output vec3 m_color  =  a_color;
    
    #if precomputed
    
        output.position = vec4( a_position , 1.0 );
    
    #else
    
        uniform mat4 u_sge_mvpMatrix;
        output.position =    u_sge_mvpMatrix * vec4( a_position , 1.0 );
    
    #endif
    
}

// -----------------------------------------------------
## fragment
// -----------------------------------------------------

area main{
    
    uniform sampler2D u_sge_mainTex;

    input vec2 m_uv;
    input vec3 m_color;

    float a=texture(u_sge_mainTex, m_uv).r;
    if(a==0.0) discard;
    
    output.color = vec4(m_color, a);

}
