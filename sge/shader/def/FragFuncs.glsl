void twoSidedNormal(inout vec3 normal){
    
    normal= mix(-normal,normal, int(gl_FrontFacing));
    
}

float linearizeDepth (float depth, float nearPlane, float farPlane) {

    return (2.0*nearPlane) / (farPlane + nearPlane - depth * (farPlane - nearPlane));
    
}
