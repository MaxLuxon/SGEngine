// -----------------------------------------------------
## vertex
// -----------------------------------------------------

area main{
    

    input vec3 a_position (loc=1);

    vec3 vertexPos=a_position;

#ifndef posOnly
    
    #include "sge/shader/def/Material.glsl"

    // Colors
    uniform Material u_sge_material;
    input vec3 a_color;
    vec3 m_color= pow(a_color*u_sge_material.color,vec3(2.2));
    output vec3 m_color;
        
#endif
    
    vec4 outputPos;
    #insert "sge/shader/StandardTransform.glsl" replace(m_position=m_position);

    uniform vec2 u_sge_uv_scale(1,1);
    input vec2 a_uv;
    output vec2 m_uv=a_uv*u_sge_uv_scale;
    output vec2 m_uvOriginal=a_uv;


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
    
    float visibilty=1;
    float ambientOcclusion=1;
    uniform vec3 u_sge_camWorldPos;
    
    input vec4 m_position;
    
    #ifndef posOnly
    
    input vec2 m_uv;
    input vec2 m_uvOriginal;
    
    vec3 normal;
    uniform Material u_sge_material;
    vec4 material=vec4(u_sge_material.roughness,u_sge_material.metalness,0,0);
    float distanceZ= distance(u_sge_camWorldPos, m_position.rgb);

    uniform sampler2D u_blend_map;
    uniform sampler2D u_atex_0;
    uniform sampler2D u_ntex_0;
    uniform sampler2D u_atex_1;
    uniform sampler2D u_ntex_1;
    uniform sampler2D u_atex_2;
    uniform sampler2D u_ntex_2;

    vec4 blendValues= texture(u_blend_map, m_uvOriginal);

    
// NORMAL //////////////////////////////////////
#ifdef SGE_NORMAL_MAPPING
    
        input vec3 m_normal;
        input mat3 m_tbn_matrix;
        input vec2 m_uv;
   
        vec3 normalTex=texture(u_ntex_0,m_uv).rgb*blendValues[0];
        normalTex= mix(normalTex,texture(u_ntex_1,m_uv).rgb,blendValues[1]);
        normalTex= mix(normalTex,texture(u_ntex_2,m_uv).rgb,blendValues[2]);
    
        normal= normalize((normalTex*2.0-1.0)*m_tbn_matrix);
    
#else
        input vec3 m_normal;
        normal=normalize(m_normal);
    
#endif
    
    
// COLOR ////////////////////////////////////
    
    input vec3 m_color;
    vec3 color=texture(u_atex_0,m_uv).rgb*blendValues[0];
    color= mix(color,texture(u_atex_1,m_uv).rgb,blendValues[1]);
    color= mix(color,texture(u_atex_2,m_uv).rgb,blendValues[2]);
    color= pow(color, vec3(2.2));

    #if DEFERRED_RENDERING
    
    #elseif FORWARD_RENDERING
    
        // SHADOW //////////////////////////////////////
        #if SGE_RECEIVE_SHADOW
    
        #include "sge/shader/def/CascadeShadow.glsl"
    
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
    
        //color= simpleFog(color, vec3(0.7,0.5,0.3)*15, distanceZ);
        uniform vec3 lightDirection;
        uniform vec3 u_fogColor;
        uniform vec3 sunLight;
    
        vec3 viewDir2            = normalize(u_sge_camWorldPos-m_position.xyz);
    
        color=fog(color, distanceZ, viewDir2, m_position.xyz, lightDirection, u_fogColor,sunLight);

    #else
    
        // No Color informations needed
        // e.g. for pure depth rendering
    
    #endif
    
    
    
// DEBUG / OUTPUT ///////////////////////////////////
    
    #if DEFERRED_RENDERING
    
        input vec3 m_color;
        input vec3 m_normal;
        input vec4 m_position;
    
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
