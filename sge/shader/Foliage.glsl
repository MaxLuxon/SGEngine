// -----------------------------------------------------
## vertex
// -----------------------------------------------------

area main{
    
#ifndef posOnly
    
#include "sge/shader/def/Material.glsl"
    
    // Colors
    uniform Material u_sge_material;
    vec3 m_color= pow(u_sge_material.color,vec3(2.2));
    output vec3 m_color;
    
#endif
    
    input vec3 a_color;
    output vec3 m_color=a_color;

    input vec3 a_position (loc=1);
    
    uniform float u_sge_time;
    uniform vec3 u_player_pos;
    uniform mat4 u_sge_modelMatrix;
    
    float amount=a_position.y*0.01*a_color.r;
        
    vec3 pos= (u_sge_modelMatrix* vec4(a_position,1)).xyz;
    vec3 pDir= (pos-u_player_pos);
    float pdist= length(pDir*vec3(1,0,1));
    pDir= normalize(pDir)*vec3(1,0,1);
        
    pdist= 1-min(pdist,1.0);
        
        
    vec3 m_transform=vec3(cos(u_sge_time+pos.x*50),cos(u_sge_time+pos.y*50)*0.3,cos(u_sge_time+pos.z*50))*amount;
    
    m_transform=clamp(m_transform,vec3(-0.2),vec3(0.2));
    
   // m_transform+=pDir*pdist*a_position.y;
    //m_vertex= m_vertex+dir*instance_normal;
    //   v_color= vec3(1)-vec3(cos(m_vertex.z*10),sin(m_vertex.x*10),0)*0.05;
    
    vec3 vertexPos=a_position+m_transform;
    
    vec4 outputPos;
#insert "sge/shader/StandardTransform.glsl" replace(m_position=m_position);
    
    input vec2 a_uv;
    output vec2 m_uv=a_uv;
    
    
#if SGE_RECEIVE_SHADOW
    
    output vec4 shadowCoord[2];
    uniform mat4 u_shadowBiasMVP[2];
    shadowCoord[0] =  u_shadowBiasMVP[0] * vec4(m_position.xyz,1.0);
    shadowCoord[1] =  u_shadowBiasMVP[1] * vec4(m_position.xyz,1.0);
    
#endif
    
    output.position= outputPos;
    
}

// -----------------------------------------------------
## fragment
// -----------------------------------------------------

area main{
    
#include "sge/shader/def/Material.glsl"
#insert "sge/shader/AlphaCut.glsl"
    
    
#ifndef posOnly
    
    input vec4 m_position;
    uniform vec3 u_sge_camWorldPos;
    vec3 normal;
    uniform Material u_sge_material;
    vec4 material=vec4(u_sge_material.roughness,u_sge_material.metalness,0,0);
    float distanceZ= distance(u_sge_camWorldPos, m_position.rgb);
    
    float visibilty=0;
    float ambientOcclusion=1;
    
    input vec3 m_color;
    ambientOcclusion=m_color.g;
    
    // MATERIAL ////////////////////////////////////
#if SGE_MATERIAL_TEXTURE
    
    input vec2 m_uv;
    uniform sampler2D u_sge_matTex;
    
    material.x=texture(u_sge_matTex,m_uv).x;
    
#endif
    
    // NORMAL //////////////////////////////////////
#ifdef SGE_NORMAL_MAPPING
    
    input vec3 m_normal;
    input mat3 m_tbn_matrix;
    input vec2 m_uv;
    
    uniform sampler2D u_sge_normalTex;
    
    normal= normalize((texture(u_sge_normalTex,m_uv).rgb*2.0-1.0)*m_tbn_matrix);
    
#else
    
    input vec3 m_normal;
    normal=normalize(m_normal);
    
#endif
    
    
#if SGE_TWO_SIDED
    
    normal= mix(-normal,normal,vec3(gl_FrontFacing));
    
#endif
    
    // COLOR ////////////////////////////////////
    
    vec3 color=vec3(1);
    
#if SGE_ALBEDO_TEXTURE
    
    input vec2 m_uv;
    uniform sampler2D u_sge_mainTex;
    color=pow(texture(u_sge_mainTex,m_uv).rgb,vec3(2.2));
    
#endif
    
#if OUTPUT_WHITE
    
    color=vec3(1);
    
#endif
    
    
#if  DEFERRED_RENDERING
    
 
    
#elseif FORWARD_RENDERING
    
    
    // SHADOW //////////////////////////////////////
    #if SGE_RECEIVE_SHADOW
    
    #include "sge/shader/def/CascadeShadow.glsl"
    
    // Shadow
    uniform sampler2D u_shadowMap[2];
    uniform vec4 u_splits;
    uniform vec3 u_sge_camWorldPos;
    
    input vec4 m_position;
    input vec4 shadowCoord[2];
    
    visibilty= cascadeShadow2Splits(u_shadowMap,shadowCoord,distanceZ,u_splits);
    
    #endif
    
    vec3 light=vec3(0);
    #insert "sge/shader/Lighting.glsl" replace(albedo=color);
    color=light;
    
    #include "sge/shader/def/Fog.glsl"
    
    input vec4 m_position;
    uniform vec3 u_sge_camWorldPos;
    
    //color= simpleFog(color, vec3(0.7,0.5,0.2)*15, distanceZ);
    uniform vec3 lightDirection;
    uniform vec3 u_fogColor;
    uniform vec3 sunLight;
    
    vec3 viewDir2  = normalize(u_sge_camWorldPos-m_position.xyz);
    
    color=fog(color, distanceZ, viewDir2, m_position.xyz, lightDirection,u_fogColor, sunLight);

    
#else
    
    // No Color informations needed
    // e.g. for pure depth rendering
    
#endif
    
    
    // DEBUG / OUTPUT ///////////////////////////////////
    
#if DEFERRED_RENDERING
    
    input vec3 m_color;
    input vec3 m_normal;
    input vec4 m_position;
    
    material.z=u_sge_material.subsurface.w;
    
#if SGE_SUBSURFACE_SCATTERING_MAP
    uniform sampler2D u_sge_ss_tex;
    material.z=texture(u_sge_ss_tex,m_uv).r*u_sge_material.subsurface.w;
    
#endif
    
    
    output[0]=vec4(color,1.0);
    output[1]=vec4(normal,1.0);
    output[2]=m_position;
    output[3]=material;
    
#elseif OUTPUT_POSITION
    
    input vec4 m_position;
    output.color=vec4(m_position.rgb*0.1,1);
    
#else
    
    output.color=vec4(color,1);
    
#endif
    
    
    
#endif
    
    
}
