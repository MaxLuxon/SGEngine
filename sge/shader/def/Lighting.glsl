#define PI 3.1415

struct Light {
    
    int shadowMapIndex;

    vec3 position;
    vec3 direction;
    
    float constant;
    float linear;
    float quadratic;
    
    vec3 color;
    
};


// http://www.standardabweichung.de/code/javascript/webgl-glsl-fresnel-schlick-approximation

float fresnel(vec3 direction, vec3 normal) {

    vec3 halfDirection = normalize( normal + direction );
    
    float cosine = dot( halfDirection, direction );
    float product = max( cosine, 0.0 );
    float factor =  1-pow( product, 5.0 ) ;
    
    return factor;
    
}

float beckmannDistribution(in float NdotH, in float roughness){
    
    float a = roughness * roughness;
    float a2 = a * a;
    float d = NdotH * NdotH * (a2 - 1.f) + 1.f;
    return a2 / (PI * d * d + 1e-7f);
    
    
//    float mSquared=roughness*roughness;
//    float r1 = 1.0 / ( 4.0 * mSquared * pow(NdotH, 4.0));
//    float r2 = (NdotH * NdotH - 1.0) / (mSquared * NdotH * NdotH);
//    
//    return r1 * exp(r2);;
}

float cookTorranceSpecular(in vec3 lightDirection,in vec3 viewDirection,in vec3 surfaceNormal,in float roughness,in float fresnel){
    
    float VdotN = max(dot(viewDirection, surfaceNormal), 0.0);
    float LdotN = max(dot(lightDirection, surfaceNormal), 0.0);
    
    //Half angle vector
    vec3 H = normalize(lightDirection + viewDirection);
    
    //Geometric term
    float NdotH = max(dot(surfaceNormal, H), 0.0);
    float VdotH = max(dot(viewDirection, H), 0.001);
    
    float G = min(1, min((2*NdotH*VdotN/VdotH), (2*NdotH*LdotN/VdotH)));
    
    float alpha = acos(NdotH);
    float D = exp(-(alpha*alpha)/(roughness*roughness));

    //Fresnel term
    float F = pow(1.0 - VdotH, 5.0);
    F *= (1.0 - fresnel);
    F += fresnel;
    
    return (F*D*G)* (PI/4);
    
}

float orenNayarDiffuse(in vec3 l,in vec3 v,in vec3 n,in float roughness,in float albedo ){
    
    float LdotV = dot( l, v );
    float NdotL = dot( n, l );
    float NdotV = dot( n, v );
    
    float s = LdotV - NdotL * NdotV;
    float t = mix( 1.0, max( NdotL, NdotV ), step( 0.0, s ) );
    
    float sigma2 = roughness * roughness;
    float A = 1.0 + sigma2 * ( albedo / ( sigma2 + 0.13 ) + 0.5 / ( sigma2 + 0.33 ) );
    float B = 0.45 * sigma2 / ( sigma2 + 0.09 );
    
    return albedo * max( 0.0, NdotL ) * ( A + B * s / t ) / PI;
}