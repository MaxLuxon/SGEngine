// -----------------------------------------------------
## vertex
// -----------------------------------------------------

area main{

    input vec4 a_vertex;
    input vec4 a_color;
    
    uniform mat4 u_projectionMatrix;
    uniform mat4 u_viewMatrix;
    uniform mat4 u_modelMatrix;
    
    mat4 m=u_modelMatrix;
    m[3]+=vec4(a_vertex.xyz,1.0);
    
    mat4 ModelView =  u_viewMatrix*m;
    
    float rotation= a_vertex.y+a_vertex.x;
    
    v_position   =    a_normal;
    v_color    =    clamp(a_color,0,1);
    v_uv       =     mat2(cos(rotation), sin(rotation), -sin(rotation), cos(rotation))*(a_normal.xy)+vec2(0.5);
    
    output.position=   u_projectionMatrix * (ModelView * vec4(0.0, 0.0, 0.0, 1.0)
                                         + vec4(a_normal.x*a_vertex.w, a_normal.y*a_vertex.w*((sin(a_vertex.y*5+a_vertex.z)+1.5)*0.2+0.6), 0.0, 0.0));
    ;
    
}
    
// -----------------------------------------------------
## fragment
// -----------------------------------------------------
    
area main{
        
    input vec2 v_uv;
    
    vec4 outColor=vec4(1);
        
    #if TEXTURE
        
        uniform vec4 u_tint_color;
        uniform sampler2D texture0;
        outColor=texture2D(texture0, v_uv)*u_tint_color;
        
    #endif
        
    uniform vec4 u_overlay_color;
    outColor = mix(outColor, u_overlay_color, u_overlay_color.a);
        
    #if STENCIL_MASK
        
        uniform sampler2D textureMask;
        vec4 masktex=texture2D(textureMask,v_uvNormalized);
        outColor.a=masktex.a;
        
    #endif
        
    output.color =	outColor;
        
}
