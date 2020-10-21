#ifndef __SSTests__SceneGame__
#define __SSTests__SceneGame__

#include "SGEngine.h"
#include "IGMV_DebugInfo.h"

class TestSceneMaster: public sge::Scene{
    
private:
    
    // Camera
    sge::Camera* _camera;
    sge::Camera* _shadowCam;

    sge::Model* _terrain;
    sge::Model* _grass[250];
    sge::Model* _farn[300];
    sge::Model* _testModels[25];

  
    sge::ModelRigged* _modelRigged;
    float _time;
    sge::DeferredBuffer* _deferredBuffer;
    sge::FrameBuffer* _buffer[2];

    sge::View* _cursor;

    sge::ShaderModule*  _lensShader;
    sge::ShaderModule*  _fxaaShader;
    sge:: ShaderModule* _deferredLightingShader;

    sge::ShadowMap* _shadowMap;
    sge::TextureCubeMap* _cubeMap;
    
    sge::Bloom* _bloomEffect;
    
    sge::Model* _skyBox;
    sge::Material* _grassMaterial;
    
    sge::CameraFilterFrustum* _frustumCuller;
    IGMV_DebugInfo* _debugView;

    sge::SSAO* _ssao;
    sge::ShaderState _globalStateEnviroment;
    
    sge::Model* _selectedModel;
    
public:
    
    float getHeightOfTerrain(sge::Vec2 pos, sge::Vec3& normal);
    
    void shaderChanged(sge::FileUtils::FileWatcher::Event* event);
    
    TestSceneMaster();
    ~TestSceneMaster();

    void draw();
    void enter();
    void exit();

    void update(float delta);
    
    void didReleaseKey(KeyEvent* event);
    void didPressKey(KeyEvent* event);
    
    void mouseWheel(sge::TouchEvent* event);

    void touchBegan(sge::TouchEvent* event);
    void touchMoved(sge::TouchEvent* event);
    void touchEnded(sge::TouchEvent* event);
    
};

#endif
