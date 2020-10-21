#insert "sge/shader/View.glsl"

// -----------------------------------------------------
## fragment
// -----------------------------------------------------

area main override{
    
    input vec2 m_uv;
    uniform sampler2D u_sge_mainTex;

    gl_FragDepth = texture(u_sge_mainTex, v_uv).r;
    
  //  output.color =  vec4(color.rgb , u_tint_color.a);
    
}