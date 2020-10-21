#include "sge/shader/def/VertexFuncs.glsl"

// Base for all Standart Vertex shaders
// For Dynamic Objects/ Static or Batched Objects/ Rigged and Instanced Objects

#if SGE_PRECOMPUTED

    uniform mat4 u_sge_projectionViewMatrix;

    outputPos=u_sge_projectionViewMatrix * vec4(vertexPos, 1.0);
    output vec4 m_position= vec4(vertexPos, 1.0);

    #ifndef posOnly

    // Normals
    input vec3 a_normal;
    m_normal =  a_normal;
    output vec3 m_normal;

    #endif

#elseif SGE_RIGGED

    uniform mat4 u_sge_mvpMatrix;
    uniform mat4 u_sge_modelMatrix;

    input vec4 a_boneIDs;
    input vec4 a_boneWeights;

    uniform mat4 u_bones[50];

    mat4 BoneTransform=  u_bones[int(a_boneIDs[0])] * a_boneWeights[0];
    BoneTransform     += u_bones[int(a_boneIDs[1])] * a_boneWeights[1];
    BoneTransform     += u_bones[int(a_boneIDs[2])] * a_boneWeights[2];
    BoneTransform     += u_bones[int(a_boneIDs[3])] * a_boneWeights[3];


    outputPos=u_sge_mvpMatrix * BoneTransform* vec4(vertexPos, 1.0);

    output vec4 m_position= u_sge_modelMatrix* BoneTransform* vec4(vertexPos, 1.0);

    #ifndef posOnly

    // Normals
    input vec3 a_normal;
    uniform mat3 u_sge_normalMatrix;

    mat3 normbone= inverse(mat3(BoneTransform));
    normbone= transpose(normbone);

    m_normal =  normbone*u_sge_normalMatrix * a_normal;
    output vec3 m_normal;

    #endif

#elseif SGE_INSTANCED

    #extension GL_ARB_draw_instanced : enable

    input mat4 instance_model;
    input mat3 instance_normal;

    uniform mat4 u_sge_view_projection_matrix;
    uniform vec3 u_camWorldPos;

    const vec2 fadeOfDistance= vec2(4,3);
    
    m_transform= vec3(0);
    m_vertex = a_vertex;
    m_normalMatrix = instance_normal;
    m_normal = normalize( a_normal);

    v_position   = vec4( instance_model   *   vec4(m_vertex,1.0)).xyz+m_transform;
    
    float dista= 1-clamp((distance(u_camWorldPos,v_position.xyz)-fadeOfDistance.x)/fadeOfDistance.y,0,1);
    
    v_position   = vec4( instance_model   *   vec4(m_vertex*vec3(1,dista,1),1.0)).xyz+m_transform;
    
    v_normal=m_normal;
    
    outputPos = u_projectionViewMatrix*(instance_model * vec4( m_vertex*vec3(1,dista,1) , 1.0)+vec4(m_transform,0.0));
    
    #ifndef posOnly
    
        // Normals
        input vec3 a_normal;
        vec3 m_normal = normalize((BoneTransform*  vec4(a_normal, 0.0)).xyz);
        output vec3 m_normal;
    
    #endif
    
    
#else

    uniform mat4 u_sge_mvpMatrix;
    uniform mat4 u_sge_modelMatrix;

    outputPos=u_sge_mvpMatrix * vec4(vertexPos, 1.0);

    output vec4 m_position=u_sge_modelMatrix* vec4(vertexPos, 1.0);

    #ifndef posOnly

    // Normals
    input vec3 a_normal;
    uniform mat3 u_sge_normalMatrix;
    m_normal =  u_sge_normalMatrix * a_normal;
    output vec3 m_normal;

    #endif


#endif


#if outputTangent
output m_tangent;
#endif

// Normal Mapping
#ifdef SGE_NORMAL_MAPPING
input vec3 a_normal;
input vec3 a_tangent;

uniform mat3 u_sge_normalMatrix;

output mat3 m_tbn_matrix = getTBNMatrixWithRotation(a_normal, a_tangent, u_sge_normalMatrix);
#endif