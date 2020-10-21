#insert "sge/shader/View.glsl"

// -----------------------------------------------------
## fragment
// -----------------------------------------------------

area main override{

    input vec2 m_uv;
    uniform sampler2D u_sge_mainTex;

    float d=texture(u_sge_mainTex, m_uv).r;
    
    output.color =  vec4(vec3(d),1.0);

}