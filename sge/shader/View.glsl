// -----------------------------------------------------
## vertex
// -----------------------------------------------------

area main{

    input vec3 a_position(loc=0);
    input vec2 a_uv      (loc=1);

    uniform mat4 u_sge_mvpMatrix;
    uniform vec4 normalTexcords;
    
    output vec2 m_uv = a_uv;
    output vec2 m_uvNormalized = (a_uv-normalTexcords.xy)/vec2(normalTexcords.z, normalTexcords.w);
    
    output.position = u_sge_mvpMatrix * vec4( a_position , 1.0 );
    
}

// -----------------------------------------------------
## fragment
// -----------------------------------------------------

area main{

    input vec2 m_uv;
    input vec2 m_uvNormalized;

    vec4 outColor=vec4(1);
    uniform vec4 u_tint_color(1,1,1,1);

    #ifdef SGE_TEXTURE

        uniform sampler2D u_sge_mainTex;
        outColor=texture(u_sge_mainTex, m_uv);

    #endif

    uniform vec4 u_overlay_color;
    outColor = mix(outColor, u_overlay_color, u_overlay_color.a);

    #ifdef SGE_STENCIL_MASK

        uniform sampler2D maskTexture;
        vec4 masktex=texture(maskTexture,m_uv);
        outColor.a=masktex.a;

    #endif

    output.color =	outColor*u_tint_color;

}
