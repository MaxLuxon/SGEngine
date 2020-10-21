#insert "sge/shader/View.glsl"

// -----------------------------------------------------
## fragment
// -----------------------------------------------------


area main override{
   
    input vec2 m_uv;
    
    uniform float u_threshold;
    uniform sampler2D u_sge_mainTex;
    
    uniform vec2 dir;
    uniform vec2 resolution;
    
    uniform float radius;
    uniform float sampleNum;
    
    vec4 col = vec4(0);
    vec2 unit = 1.0 / resolution.xy;
    
    float r = radius;
    float sampleStep = r / sampleNum;
    
    float count = 0.0;
    
    for(float y = -r; y < r; y += sampleStep){
        
        float weight = (r - abs(y));
        vec2 tc=m_uv + vec2(y * unit.x * dir.x ,  y * unit.y*dir.y);
        tc.x=clamp(tc.x,0.0,1.0);
        tc.y=clamp(tc.y,0.0,1.0);
        
        col += texture(u_sge_mainTex, tc) * weight;
        count += weight;
    }
    

    output.color=  col / count;
    
}




