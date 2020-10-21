#insert "sge/shader/View.glsl"

// -----------------------------------------------------
## fragment
// -----------------------------------------------------

area main override{

input vec2 m_uv;

uniform sampler2D u_sge_mainTex;
uniform vec2 inverseSize;

const float R_fxaaSpanMax=8.0;
const float R_fxaaReduceMin=1.0/128.0;
const float R_fxaaReduceMul=1.0/8.0;
    
    vec4 tex=texture2D(u_sge_mainTex,m_uv);
    vec3 col= tex.xyz;
    
    vec3 luma= vec3(0.299,0.587,0.114);
    
    float ltl= dot( texture2D( u_sge_mainTex,m_uv + vec2(-inverseSize.x, -inverseSize.y ) ).xyz, luma );
    float ltr= dot( texture2D( u_sge_mainTex,m_uv + vec2(inverseSize.x, -inverseSize.y ) ).xyz, luma );
    float lbl= dot( texture2D( u_sge_mainTex,m_uv + vec2(-inverseSize.x, inverseSize.y ) ).xyz, luma );
    float lbr= dot( texture2D( u_sge_mainTex,m_uv + vec2(inverseSize.x, inverseSize.y ) ).xyz, luma );
    float lm=  dot( texture2D( u_sge_mainTex,m_uv ).xyz, luma );
    
    float b=(ltl+ltr+lbl+lbr+lm)*0.2;
    
    vec2 dir;
    dir.x= -((ltl+ltr)-(lbl+lbr));
    dir.y= ((ltl+lbl)-(ltr+lbr));
    
    float dirReduce = max((ltl + ltr + lbl + lbr) * (R_fxaaReduceMul * 0.25), R_fxaaReduceMin);
    float inverseDirAdjustment = 1.0/(min(abs(dir.x), abs(dir.y)) + dirReduce);
    
    dir = min(vec2(R_fxaaSpanMax, R_fxaaSpanMax),
              max(vec2(-R_fxaaSpanMax, -R_fxaaSpanMax), dir * inverseDirAdjustment)) * inverseSize;
    
    vec3 result1 = (1.0/2.0) * (
                                texture2D(u_sge_mainTex, m_uv.xy + (dir * vec2(1.0/3.0 - 0.5))).xyz +
                                texture2D(u_sge_mainTex, m_uv.xy + (dir * vec2(2.0/3.0 - 0.5))).xyz);
    
    vec3 result2 = result1 * (1.0/2.0) + (1.0/4.0) * (
                                                      texture2D(u_sge_mainTex, m_uv.xy + (dir * vec2(0.0/3.0 - 0.5))).xyz +
                                                      texture2D(u_sge_mainTex, m_uv.xy + (dir * vec2(3.0/3.0 - 0.5))).xyz);
    
    float lumaMin = min(lm, min(min(ltl, ltr), min(lbl, lbr)));
    float lumaMax = max(lm, max(max(ltl, ltr), max(lbl, lbr)));
    float lumaResult2 = dot(luma, result2);
    
    if(lumaResult2 < lumaMin || lumaResult2 > lumaMax)
        output.color = vec4(result1, 1.0);
    else
        output.color = vec4(result2, 1.0);

}






