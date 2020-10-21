#insert "sge/shader/View.glsl"

// -----------------------------------------------------
## fragment
// -----------------------------------------------------

area main override{
    
    #include "sge/shader/def/Lighting.glsl"
    #include "sge/shader/def/ToneMapping.glsl"

    uniform sampler2D sge_albedo_tex;
    uniform sampler2D sge_normal_tex;
    uniform sampler2D sge_position_tex;
    uniform sampler2D sge_material_tex;
    uniform sampler2D sge_ssao_tex;

    input vec2 m_uv;
    
    vec4 albedo= texture(sge_albedo_tex, m_uv);
    if(albedo.a==0) discard;
    
    vec3 normal= texture(sge_normal_tex, m_uv).xyz;
    vec3 position= texture(sge_position_tex, m_uv).xyz;
    vec4 material= texture(sge_material_tex, m_uv);
    
    uniform vec3 lightDirection;
    uniform vec3 u_eye_pos;
    uniform vec3 sunLight;
    
    // Shadows
    #include "sge/shader/def/CascadeShadow.glsl"
    
    uniform sampler2D u_shadowMap[2];
    uniform vec4 u_splits;
    uniform vec3 u_sge_camWorldPos;
    uniform mat4 u_shadowBiasMVP[2];
    
    float visibilty=1;
    float distanceZ= distance(u_eye_pos, position.rgb);

    vec4 shadowCoord[2];
    shadowCoord[0] =  u_shadowBiasMVP[0] * vec4(position.xyz,1.0);
    shadowCoord[1] =  u_shadowBiasMVP[1] * vec4(position.xyz,1.0);
    visibilty= cascadeShadow2Splits(u_shadowMap,shadowCoord,distanceZ,u_splits);

    
    // Ligthing
    vec3 color= vec3(0);
    vec3 viewDir = normalize(u_eye_pos-position.xyz);
    
    
    float ndotl= dot(normal, lightDirection);
    float diffuse  = clamp(ndotl,0,1);
    float specular = cookTorranceSpecular(lightDirection,viewDir,normal, material.x, 1.2);
    
    vec3 specColor= mix(sunLight, albedo.xyz*sunLight,material.y);
    color += mix(sunLight * diffuse * albedo.xyz*0.7, specColor, (1-material.x)*specular*0.5)*visibilty;
    
    // Subsurface
    color += visibilty*material.z*sunLight*albedo.xyz*clamp(-ndotl,0,1.0);
    
    // Ambient / Indirect light
    uniform vec3 u_groundColor(0.62,0.30,0.21);
    float ambientOcclussion=texture(sge_ssao_tex, m_uv).r;
    
    float mfresnel= pow(fresnel(viewDir,normal),5.0);

    float ind = clamp( dot( normal, normalize(lightDirection*vec3(-1.0,0.0,-1.0)) ), 0.0, 1.0 );
    float sky = clamp( 0.5 + 0.5*normal.y, 0.0, 1.0 );
    float ground =  clamp( 0.5 + 0.5*-normal.y, 0.0, 1.0 );
    
    vec3 ambientLight=sky*vec3(0.16,0.20,0.28)*10;
    ambientLight+= ind*vec3(0.40,0.28,0.20)*sunLight*0.1;
    ambientLight+= ground*u_groundColor*sunLight*0.4;
    
    vec3 reflectedView= reflect(viewDir,normal);
    float skyReflect = pow(clamp( -reflectedView.y, 0.0, 1.0 ),5.0);
    color+= vec3(mfresnel*0.3)*(1-material.x);
    color+=skyReflect*vec3(0.16,0.30,0.58)*5*mfresnel*(1-material.x);

    color += ambientLight*albedo.xyz*ambientOcclussion*0.20;
  
    // Atmosperic scattering
    #include "sge/shader/def/Fog.glsl"
    
    uniform vec3 u_fogColor;
    
    color=fog(color, distanceZ, viewDir, position.xyz, lightDirection, u_fogColor,sunLight*0.3);
    
    color = gammaCorrection(color, 2.2);

    
    output.color=vec4(color,1.0);
    
    
}
