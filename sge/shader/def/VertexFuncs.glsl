mat3 getTBNMatrix(in vec3 normal, in vec3 tangent){

    vec3 binormal = cross(normal, tangent);
    return  transpose(mat3(tangent,binormal,normal));
    
}

mat3 getTBNMatrixWithRotation(in vec3 normal, in vec3 tangent, in mat3 normalMatrix){
    
    vec3 binormal = cross(normal, tangent);
    return  transpose(normalMatrix*mat3(tangent,binormal,normal));
    
}