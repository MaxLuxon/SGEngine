#include "DebugDraw.h"
#include "Director.h"

namespace sge {

void DebugDraw::startUp(){
    
    _renderer= Director::getInstance()->getRenderer();
    
    _debugNode=Node::create();
    
    _frustumMesh= new Mesh(MESH_COLORS);
    _cubeMesh= new Mesh(MESH_COLORS);
    _sphereMesh= new Mesh(MESH_COLORS);
    
    for (int i=0; i<8; i++) {
        
        _frustumMesh->positions.push_back(Vec3(0, i, 0));
        
        if(i<4){
            _frustumMesh->colors.push_back(Vec3(1, 0, 0));
        }else{
            
            _frustumMesh->colors.push_back(Vec3(0, 1, 0));
            
        }
        
    }
    
    // Front
    _frustumMesh->indices.push_back(0);
    _frustumMesh->indices.push_back(1);
    _frustumMesh->indices.push_back(2);
    _frustumMesh->indices.push_back(0);
    _frustumMesh->indices.push_back(2);
    _frustumMesh->indices.push_back(3);
    
    //Far
    _frustumMesh->indices.push_back(4);
    _frustumMesh->indices.push_back(5);
    _frustumMesh->indices.push_back(6);
    _frustumMesh->indices.push_back(4);
    _frustumMesh->indices.push_back(6);
    _frustumMesh->indices.push_back(7);
    
    // top
    _frustumMesh->indices.push_back(0);
    _frustumMesh->indices.push_back(3);
    _frustumMesh->indices.push_back(4);
    
    _frustumMesh->indices.push_back(4);
    _frustumMesh->indices.push_back(7);
    _frustumMesh->indices.push_back(0);
    
    // Side
    _frustumMesh->indices.push_back(2);
    _frustumMesh->indices.push_back(5);
    _frustumMesh->indices.push_back(4);
    
    _frustumMesh->indices.push_back(7);
    _frustumMesh->indices.push_back(6);
    _frustumMesh->indices.push_back(1);
    
//    ShaderModule* module= ShaderModule::createFromFiles({"support/shader/vert/ModelVertex.glsl", "support/shader/frag/Debug.glsl"});
//    
//    _debugState.shaderState=module->getShader()->getState();
//    _debugState.drawMode=DrawMode::TRIANGLES;
    
}


void DebugDraw::renderDebugCube( std::vector<Vec3> points, FrameBuffer* buffer, Camera* camera){
    
    for (int i=0; i<8 && i<points.size(); i++) {
        
        _frustumMesh->positions[i]=points[i];
        
    }
    
    _frustumMesh->update();
    _debugState.camera=camera;
    
    _debugState.camera=camera;
    _debugState.view=camera->getViewMatrix();
    _debugState.projection=camera->getProjectionMatrix();
    _debugState.model=_debugNode->getModelMatrix();
    
    buffer->bind();
    
    _frustumMesh->getVAO()->bind();
    
    _renderer->renderMeshWithRenderState(_debugNode, _frustumMesh, &_debugState);
    _frustumMesh->getVAO()->unbind();
    
    buffer->unbind();
    
}


void DebugDraw::renderDebugFrustum(Frustum* frustum, FrameBuffer* buffer, Camera* camera){
    
    Vec3 corners[8];
    frustum->getCorners(&corners[0]);
    
    for (int i=0; i<8; i++) {
        
        _frustumMesh->positions[i]=corners[i];
        
    }
    
    _frustumMesh->update();
    _debugState.camera=camera;
    
    _debugState.camera=camera;
    _debugState.view=camera->getViewMatrix();
    _debugState.projection=camera->getProjectionMatrix();
    _debugState.model=_debugNode->getModelMatrix();
    
    buffer->bind();
    
    _frustumMesh->getVAO()->bind();
    
    _renderer->renderMeshWithRenderState(_debugNode, _frustumMesh, &_debugState);
    _frustumMesh->getVAO()->unbind();
    
    buffer->unbind();
    
}
    
}
