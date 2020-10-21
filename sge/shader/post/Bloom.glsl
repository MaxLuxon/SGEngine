#insert "sge/shader/View.glsl"

// -----------------------------------------------------
## fragment
// -----------------------------------------------------

area main override{
    
#include "sge/shader/def/ToneMapping.glsl"

    input vec2 m_uv;

    uniform float u_threshold;
    uniform sampler2D u_sge_mainTex;
    
    vec4 tex=texture2D(u_sge_mainTex,m_uv);
    
    vec3 outcolor=vec3(0,0,0);
   
    outcolor= tex.rgb;
    outcolor = min(vec3(256 * 256, 256 * 256, 256 * 256), outcolor.rgb);
    
    // todo: make this adjustable (e.g. LUT)
    float TotalLuminance = (0.2126*outcolor.r + 0.7152*outcolor.g + 0.0722*outcolor.b) * 1;
    
    float BloomLuminance = TotalLuminance - u_threshold;
    // mask 0..1
    float BloomAmount = clamp(BloomLuminance / 2.0f,0.0,1.0);
    
    vec3 bloom=outcolor*BloomAmount;
    output.color=  vec4(whitePreservingLumaBasedReinhardToneMapping(bloom.rgb) , 1);
    
}







