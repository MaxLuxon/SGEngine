#include "sge/shader/def/Lighting.glsl"

area lighting{

    input vec4 m_position;

    uniform vec3 lightDirection;
    uniform vec3 u_sge_camWorldPos;
    uniform vec3 sunLight;
    uniform vec3 ambientLightColor[6];
    
    vec3 viewDir            = normalize(u_sge_camWorldPos-m_position.xyz);
    vec3 viewReflectedDir   = -reflect(viewDir,normal);
    vec3 ambientLight=vec3(0);
    vec3 ambientLightReflect=vec3(0);

    float nValue=(normal.y*0.5+0.5)*5;
    float nValueR=(viewReflectedDir.y*0.5+0.5)*5;

    int index= int(nValue-0.01);
    int indexR= int(nValueR-0.01);

    ambientLight+=mix(ambientLightColor[index],ambientLightColor[index+1],nValue-index);
    ambientLightReflect+=mix(ambientLightColor[indexR],ambientLightColor[indexR+1],nValueR-indexR);
    ambientLightReflect= mix(ambientLightReflect, albedo*ambientLightReflect, material.y);


    // Material
    float roughness=material.x;
    const float fresnelIO=0.8;
    
    float diffuse = orenNayarDiffuse(lightDirection,viewDir,normal, roughness, 1.0)/PI;
    float specular = cookTorranceSpecular(lightDirection,viewDir,normal, roughness, fresnelIO);

    vec3 specColor= mix(sunLight, albedo*sunLight,material.y);
    light += mix(sunLight * diffuse * albedo, specColor*specular*0.5, 1-roughness)*visibilty;
    
    // Subsurface
#if SGE_SUBSURFACE_SCATTERING
    
    light +=  albedo.rgb*sunLight*(pow(dot(lightDirection,normal)*0.5+0.5,u_sge_material.subsurface.w))*u_sge_material.subsurface.rgb*visibilty;
    
#endif
    
    
    float mfresnel= fresnel(viewDir,normal)*u_sge_material.fresnelFactor;

#if SGE_REFLECTION_CUBE_MAP
    
    uniform samplerCube u_cubeMap;
    vec3 reflection=pow(texture(u_cubeMap,-reflect(viewDir,normal)).rgb,vec3(2.2))*(0.01+ mfresnel);
    reflection= mix(reflection,albedo*reflection,material.y)*(1-roughness);
    light+=reflection*24*max(normal.y*0.7+0.3,0.0)*ambientOcclusion;
    
#else
    
    // Ambient lights
    vec3 indirectLight= vec3(0);
    indirectLight=vec3(ambientLight*albedo.rgb)*roughness;
    indirectLight+=ambientLightReflect*mfresnel*0.2;
    
    light+=ambientOcclusion*indirectLight;
    
#endif
    
    
    
    

}
