#include <stdio.h>
#include "Mesh.h"

namespace sge {
    
    void Mesh::tintMesh(const Color& color, Mesh* mesh){

        for (int i=0; i<mesh->colors.size(); i++) {
            
            mesh->colors[i]=Vec3(color.r, color.g, color.b);
            
        }
        
    
        mesh->update();
    
    }
    
    Mesh* Mesh::cube=NULL;
    Mesh* Mesh::plane=NULL;
    Mesh* Mesh::sphere=NULL;

    Mesh* Mesh::getStaticMesh(std::string name){
        
        if(name=="cube"){
            
            if(!cube){
                
                cube= makeCube();
                cube->update();
            }
            
            
            return cube;
            
        }
        
        
        if(name=="plane"){
            
            if(!plane){
                
                plane= makePlane();
                plane->update();
            }
            
            return plane;
            
        }
        
        if(name=="sphere"){
            
            if(!sphere){
                
                sphere= makeSphere();
                sphere->update();
            }
            
            return sphere;
            
        }

        
        return NULL;
    }
    
    
Mesh* Mesh::makeCube(){
    
    Mesh*m= new Mesh( MESH_NORMALS | MESH_TEXCOORDS | MESH_COLORS | MESH_TANGENTS);
    
    m->positions.push_back(Vec3(0, 0, 0));
    m->positions.push_back(Vec3(1, 0, 0));
    m->positions.push_back(Vec3(1, 0, 1));
    m->positions.push_back(Vec3(0, 0, 1));
    m->positions.push_back(Vec3(0, 1, 0));
    m->positions.push_back(Vec3(1, 1, 0));
    m->positions.push_back(Vec3(1, 1, 1));
    m->positions.push_back(Vec3(0, 1, 1));
    
    for (int i=0; i<m->positions.size(); i++) {
      
        m->colors.push_back(Vec3(1, 1, 1));
        m->texCoords.push_back(Vec2(0, 0));
        m->normals.push_back(Vec3(0, 1, 0));
        m->tangents.push_back(Vec3(1, 0, 0));

    }
    
    m->indices={0,1, 1,2, 2,3, 3,0, 0,4, 1,5, 2,6, 3,7, 4,5, 5,6, 6,7, 7,4};
    m->contentBox.max.set(1, 1, 1);
    
    
    m->update();
    
    return m;
}
    
Mesh* Mesh::makeSphere(){
        
    Mesh*m= new Mesh( MESH_NORMALS | MESH_TEXCOORDS | MESH_COLORS);
    
    int rings= 10;
    int sectors=10;
    float radius= 0.5;
    
    float const R = 1./(float)(rings-1);
    float const S = 1./(float)(sectors-1);
    int r, s;
    
    m->positions.resize(rings * sectors);
    m->normals.resize(rings * sectors);
    m->texCoords.resize(rings * sectors);
    m->colors.resize(rings * sectors);

    int i=0;
    for(r = 0; r < rings; r++){
        for(s = 0; s < sectors; s++) {
       
        float const y = sin( -M_PI_2 + M_PI * r * R );
        float const x = cos(2*M_PI * s * S) * sin( M_PI * r * R );
        float const z = sin(2*M_PI * s * S) * sin( M_PI * r * R );
        
        m->positions[i]= Vec3(x * radius, y * radius, z * radius)+Vec3(0.5,0.5,0.5);
        m->texCoords[i].set(s*S, r*R);
        m->normals[i].set(x, y, z);
        m->colors[i].set(1, 1, 1);
        
        i++;
        
        }
    }
    
    m->indices.resize(rings * sectors * 4);
    int count=0;
    
    for(r = 0; r < rings-1; r++){
        for(s = 0; s < sectors-1; s++) {
        
            m->indices[count+0]= r * sectors + s;
            m->indices[count+1]= r * sectors + (s+1);
            m->indices[count+2]= r * sectors + s;
            m->indices[count+3]= (r+1) * sectors + s;
            
            count+=4;
        }
    }
    
    
       // m->indices={0,1, 1,2, 2,3, 3,0, 0,4, 1,5, 2,6, 3,7, 4,5, 5,6, 6,7, 7,4};
        m->contentBox.max.set(1, 1, 1);
        //m->contentBox.min.set(-0.5, -0.5, -0.5);

        m->update();
        
        return m;
}

Mesh* Mesh::makePlane(){
    
    Mesh*m= new Mesh( MESH_NORMALS | MESH_TEXCOORDS | MESH_COLORS | MESH_TANGENTS );
    
    m->positions.push_back(Vec3(0, 0, 0));
    m->positions.push_back(Vec3(1, 0, 0));
    m->positions.push_back(Vec3(1, 0, 1));
    m->positions.push_back(Vec3(0, 0, 1));
    
    m->texCoords.push_back(Vec2(0, 0));
    m->texCoords.push_back(Vec2(1, 0));
    m->texCoords.push_back(Vec2(1, 1));
    m->texCoords.push_back(Vec2(0, 1));

    for (int i=0; i<4; i++) {
        m->colors.push_back(Vec3(1, 1, 1));
        m->normals.push_back(Vec3(0, 1, 0));
        m->tangents.push_back(Vec3(1, 0, 0));

    }
    
    m->indices={0,2,1,0,3,2};
    m->update();
    
    return m;
}

}