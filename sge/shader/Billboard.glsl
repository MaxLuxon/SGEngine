// -----------------------------------------------------
## vertex
// -----------------------------------------------------

area main{

    input vec3 a_vertex (loc=0);
    input vec3 a_uv     (loc=1);

    mat4 modelView;
    
    #ifdef precomuted
    
        uniform mat4 u_sge_matrix_model;
        mat4 modelView = u_sge_matrix_model;

    #else
    

    #endif
    
    output vec2 m_uv = a_uv;

    // First colunm.
    modelView[0][0] = 1.0;
    modelView[0][1] = 0.0;
    modelView[0][2] = 0.0;
    
    #ifdef spherical_billboard
    
        // Second colunm.
        modelView[1][0] = 0.0;
        modelView[1][1] = 1.0;
        modelView[1][2] = 0.0;
    
    #endif

    // Thrid colunm.
    modelView[2][0] = 0.0;
    modelView[2][1] = 0.0;
    modelView[2][2] = 1.0;

    vec4 P = modelView * a_vertex;
    output.position = projectview * P;

}

// -----------------------------------------------------
## fragment
// -----------------------------------------------------

area main{

    #insert "support/AlphaCut.glsl"
    
    #ifdef  DEFERRED_RENDERING
    
        #insert "sge/shader/Deferred.glsl"
    
    #elseif FORWARD_RENDERING
    
        #insert "sge/shader/Lighting.glsl"
    
    #else
    
        // No Color informations needed
        // e.g. for pure depth rendering
    
    #endif

}
