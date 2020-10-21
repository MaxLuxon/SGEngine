#insert "sge/shader/View.glsl"

// -----------------------------------------------------
## fragment
// -----------------------------------------------------

area main override{
    
#include "sge/shader/def/FragFuncs.glsl"
    
    input vec2 m_uv;
    
    uniform sampler2D normalTex;
    uniform sampler2D positionTex;
    uniform sampler2D noiseTex;
    uniform sampler2D depthTex;
    
    uniform int uKernelSize;
    uniform vec3 uKernelOffsets[128];
    uniform float uRadius=1;

    
    uniform vec2 u_resolution;
    uniform mat4 u_ViewMat;
    
    
    vec2 noiseScale = vec2(1920.0/4.0, 1080.0/4.0); // screen = 800x600
    
    vec3 fragPos = texture(positionTex, m_uv).xyz;
    
    float baseDepth=linearizeDepth(texture(depthTex, m_uv.xy).r,0.1,500); // linearize
    vec2 texel=vec2(1.)/u_resolution;
    
    vec3 normal = normalize(mat3(u_ViewMat)* texture(normalTex, m_uv).xyz);
    vec3 randomVec = texture(noiseTex, m_uv.xy * noiseScale).xyz;
    
    vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN = mat3(tangent, bitangent, normal);
    
    vec3 radius= texel.xyy*80;
    
    float occlusion = 0.0;
    for(int i = 0; i < uKernelSize; ++i)
    {
        // get sample position
        vec3 thesample = TBN * uKernelOffsets[i]; // From tangent to view-space
        thesample = m_uv.xyy + thesample * radius;
        
        float sampleDepth = linearizeDepth(texture(depthTex, thesample.xy).r,0.1,500); // linearize
        bool rangeCheck=distance(sampleDepth,baseDepth)<0.005;
        occlusion += (sampleDepth <= baseDepth ? 1.0 : 0.0)*float(rangeCheck);
        
    }
    
    occlusion = 1 - (occlusion / uKernelSize)*1.5;
    
    output.color=  vec4(vec3(occlusion) , 1);
    
}







