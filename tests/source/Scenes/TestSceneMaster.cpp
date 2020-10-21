#include "TestSceneMaster.h"
#include "glInclude.h"
#include "PlayerMovementComponent.h"

using namespace sge;
using namespace std;


TestSceneMaster::TestSceneMaster(){
    
    // Input register
    UpdateReceiver* newReceiver= new UpdateReceiver(CALLBACK_SELECTOR(TestSceneMaster::update, this), 1/60. , 0);
    
    KeyboardReceiver* newKeyboard= new KeyboardReceiver(0);
    newKeyboard->releaseCallback=CALLBACK_SELECTOR(TestSceneMaster::didReleaseKey, this);
    newKeyboard->pressCallback=  CALLBACK_SELECTOR(TestSceneMaster::didPressKey, this);
    
    TouchReceiver* newTouch= new TouchReceiver(0);
    newTouch->touchBeganCallback=CALLBACK_SELECTOR(TestSceneMaster::touchBegan, this);
    newTouch->touchEndedCallback=CALLBACK_SELECTOR(TestSceneMaster::touchEnded, this);
    newTouch->touchMovedCallback=CALLBACK_SELECTOR(TestSceneMaster::touchMoved, this);
    newTouch->mouseWheelCallback=CALLBACK_SELECTOR(TestSceneMaster::mouseWheel, this);
    
    Director::getInstance()->getKeyboardDispenser()->addReceiver(newKeyboard);
    Director::getInstance()->getUpdateDispenser()->addReceiver(newReceiver);
    Director::getInstance()->getTouchDispenser()->addReceiver(newTouch);
    
    Director::getInstance()->setMouseTrapped(true);
    
    // Init
    Vec2 winS= Director::getInstance()->getWindowSize();
    
    _time=0;
    
    // Shader
    _lensShader= ShaderModule::get("sge/shader/post/Lens.glsl");
    _lensShader->addDefine("SGE_TONE_MAPPING");
    _lensShader->addDefine("COLOR_GRADING");
    _lensShader->getGlobalShaderState()->setUniformTexture("u_colorRamp", Texture::get("assets/textures/colorRamp.png"), 2);
    Texture::get("assets/textures/colorRamp.png")->setTextureWrap(TextureWrap::CLAMP_TO_EDGE);
    
    _fxaaShader= ShaderModule::get("sge/shader/post/FXAA.glsl");
    _fxaaShader->getGlobalShaderState()->setUniform2f("inverseSize", Vec2( 1.0/winS.x, 1.0/winS.y));

    // Buffer
    for (int i=0; i<2; i++) {
        
        _buffer[i]= FrameBuffer::create(winS.x, winS.y, TextureFormat::RGBA16F, TextureFormat::DEPTH, TextureFormat::NONE);
        _buffer[i]->clear(true, true, true);
        _buffer[i]->setClearColor(Color(50,50,50, 1));
        
    }
    
    _deferredBuffer= new DeferredBuffer("", winS);

    // Lighting
    Vec3 sun=Color("e9dbb9").getVec3();
    _globalStateEnviroment.setUniform3f("sunLight", pow(sun,2.2)*30);
    _globalStateEnviroment.setUniform3f("u_fogColor", pow(Color("c6a684").getVec3(),2.2));
    
    
    Director::getInstance()->modelDefaultMaterial->getShaderModule()->registerShaderState(&_globalStateEnviroment);
    ShaderModule::get("sge/shader/Foliage.glsl")->registerShaderState(&_globalStateEnviroment);
    
    _shadowMap= ShadowMap::create(Vec2(2048, 2048), TextureFormat::DEPTH);
    Director::getInstance()->modelDefaultMaterial->getShaderModule()->registerShaderState(_shadowMap->getShaderState());
    ShaderModule::get("sge/shader/Foliage.glsl")->registerShaderState(_shadowMap->getShaderState());


    // Cameras
    _camera= Camera::createPerspective(45, winS.x/winS.y, 0.1, 500);
    _camera->setPosition(Vec3(4, 4, 4));
    _camera->lookAt(Vec3(0, 0, 0));

    _shadowCam= Camera::createOrthographic(40, 40, 1, -10, 50);
    _shadowCam->setUp(Vec3(0, 1, 0));
    _shadowCam->setPosition(Vec3(7, 7, 7));
    _shadowCam->lookAt(Vec3(0, 0, 0));
    _shadowCam->_middle=true;
    
    // Models
    _terrain= Model::create("assets/models/testterrain.obj");
    _terrain->transform().setScale(10);
    _terrain->getRenderObject()->setMaterial(Material::get("assets/materials/terrain.mat"));
    _terrain->getRenderObject()->getMaterial()->getShaderState()->setUniform2f("u_sge_uv_scale", Vec2(50, 50));
    _worldNode->addChild(_terrain);
    
    _cubeMap= TextureCubeMap::create({"assets/textures/cube/posx.jpg","assets/textures/cube/negx.jpg","assets/textures/cube/posy.jpg","assets/textures/cube/negy.jpg","assets/textures/cube/posz.jpg","assets/textures/cube/negz.jpg"});

    
    _grassMaterial= Material::get("assets/models/grass/material.mat");
    _grassMaterial->setCullFaceSide(CullFace::NONE);
    
    for (int i=0; i<250; i++) {
        
        _grass[i]= Model::create("assets/models/grass/model/LOD_0.dae");
        _grass[i]->transform().setTranslation((rand()%2000-1000)/30., 0, (rand()%2000-1000)/30.);
        _grass[i]->transform().setScale((rand()%50)/100.0+1.5);
        _grass[i]->transform().setRotation(Vec3(0, rand()%360, 0));
        _grass[i]->getRenderObject()->setFrustumCulling(FrustumCulling::NONE);
        _grass[i]->getRenderObject()->setMaterial(_grassMaterial);
        _grass[i]->getRenderObject()->setRenderType(RENDER_TYPE_BATCH);
        _worldNode->addChild(_grass[i]);
        
        Vec3 normal;

        _grass[i]->transform().setTranslationY(getHeightOfTerrain(Vec2(_grass[i]->transform().getTranslation().x,_grass[i]->transform().getTranslation().z), normal));
    }

    
    Material* mat=Material::get("assets/models/foliage/plant.mat");
    mat->setCullFaceSide(CullFace::NONE);
    
    for (int i=0; i<300; i++) {
        
        int ran=rand()%20;
        if(ran>5 && ran<12){
            
            _farn[i]= Model::create("assets/models/foliage/fern.dae");
            
        }else if(ran<14){
            
            _farn[i]= Model::create("assets/models/foliage/lotus.dae");
            
            
        }else{
            
            _farn[i]= Model::create("assets/models/foliage/strauch.dae");
            
            
        }
        
        
        _farn[i]->transform().setTranslation((rand()%2000-1000)/40., 0, (rand()%2000-1000)/40.);
        _farn[i]->transform().setScale((rand()%50)/50.0+1.5);
        _farn[i]->transform().setRotation(Vec3(rand()%20-10, rand()%360, rand()%20-10));
        _farn[i]->getRenderObject()->setFrustumCulling(FrustumCulling::NONE);
        _farn[i]->getRenderObject()->setMaterial(mat);
        _farn[i]->getRenderObject()->setRenderType(RENDER_TYPE_BATCH);
        _worldNode->addChild(_farn[i]);
        
        Vec3 normal;
        _farn[i]->transform().setTranslationY(getHeightOfTerrain(Vec2(_farn[i]->transform().getTranslation().x,_farn[i]->transform().getTranslation().z),normal));
        
        normal+=Vec3((rand()%10-5)/50.0,0.3,(rand()%10-5)/50.0);
        normal.normalize();
        
        _farn[i]->transform().setRotation(normal, rand()%360);
        
    }
    
    for (int i=0; i<25; i++) {
        
            _testModels[i]= Model::create("assets/models/foliage/trop.dae");
            _testModels[i]->getRenderObject()->setMaterial(mat);
            _testModels[i]->transform().setRotation(Vec3(rand()%10-5, rand()%360, rand()%10-5));
            _testModels[i]->transform().setTranslation((rand()%200-100)*0.4, 0, (rand()%200-100)*0.4);
            _testModels[i]->transform().setScale(0.3+(rand()%80)/4.0);
            _testModels[i]->getRenderObject()->setRenderType(RENDER_TYPE_BATCH);
            this->addChild(_testModels[i]);
            
            Vec3 normal;
            _testModels[i]->transform().setTranslationY(getHeightOfTerrain(Vec2(_testModels[i]->transform().getTranslation().x,_testModels[i]->transform().getTranslation().z),normal));
            
            
            
        
    }

    _skyBox= Model::create("sge/models/skyDome.obj");
    _skyBox->getRenderObject()->getMaterial()->setShaderModule(ShaderModule::get("sge/shader/SkyBox.glsl"));
    _skyBox->getRenderObject()->getMaterial()->getShaderState()->setUniformTexture("u_cubeMap", _cubeMap, 4);
    _skyBox->getRenderObject()->setFrustumCulling(FrustumCulling::NONE);
    _skyBox->getRenderObject()->setRenderLayer(Director::getInstance()->getRenderer()->getRenderLayer(RENDER_LAYER_SKY));
    _worldNode->addChild(_skyBox);
    
    
    // Rigged character
    _modelRigged= ModelRigged::create("assets/models/char.dae");
    _modelRigged->playAnimation("main", 1, true);
    _worldNode->addChild(_modelRigged);
    
    ArmatureAnimation* idle= ArmatureAnimation::create("assets/models/charIdle.dae");
    _modelRigged->getArmature()->addAnimation(idle, "idle");
    _modelRigged->playAnimation("idle", 1, true);

    PlayerMovementComponent* pmc= new PlayerMovementComponent();
    _modelRigged->addComponent(pmc);
    pmc->setMoveDir(Vec3(0,0,1));
    pmc->setVelocity(0.6);
    pmc->setMoveCamera(_camera);
    
    CameraControlComponent* cameraControl= new CameraControlComponent();
    _camera->addComponent(cameraControl);
    cameraControl->setFollowEntity(_modelRigged);
    cameraControl->setVelocity(0.5);
    cameraControl->setCameraDistance(10);
    cameraControl->setCameraOffset(Vec2(2, 3));

    // UI
    _cursor= View::create(5, 5, "assets/ui/cursor.png");
    _cursor->setPosition(winS.x*0.5, winS.y*0.5);
    _cursor->setZOrder(9);
    this->addChild(_cursor);
    

    
    _debugView= IGMV_DebugInfo::create(200, 200);
    _debugView->setPosition(50, winS.y-50);
    this->addChild(_debugView);


    // Effects
    _bloomEffect= new Bloom(winS, 4);
    _ssao= new SSAO(winS.x, winS.y);

    
    ResourceManager* resm= ResourceManager::getInstance();
    resm->printResourcesForType("mesh");
    
    
    _deferredLightingShader=ShaderModule::get("sge/shader/DeferredLighting.glsl");
    _deferredLightingShader->getGlobalShaderState()->setUniformTexture("sge_albedo_tex", _deferredBuffer->getColorTexture(), 0);
    _deferredLightingShader->getGlobalShaderState()->setUniformTexture("sge_normal_tex", _deferredBuffer->getNormalTexture(), 1);
    _deferredLightingShader->getGlobalShaderState()->setUniformTexture("sge_position_tex", _deferredBuffer->getPositionTexture(), 2);
    _deferredLightingShader->getGlobalShaderState()->setUniformTexture("sge_material_tex", _deferredBuffer->getGlossTexture(), 3);
    _deferredLightingShader->getGlobalShaderState()->setUniformTexture("sge_ssao_tex", _ssao->getTexture(), 4);
    _deferredLightingShader->registerShaderState(&_globalStateEnviroment);
    _deferredLightingShader->registerShaderState(_shadowMap->getShaderState());
    
    _frustumCuller= new CameraFilterFrustum();


    FileUtils::FileWatcher* shaderWatcher= new FileUtils::FileWatcher();
    shaderWatcher->targetPath="/Users/SirniciMac/Projects/SGEngine/sge/shader/";
    shaderWatcher->callback=CALLBACK_SELECTOR(TestSceneMaster::shaderChanged, this);
    
    Director::getInstance()->getFileUtils()->addFileWatcher(shaderWatcher);
    
    _selectedModel=NULL;
    
}

float TestSceneMaster::getHeightOfTerrain(sge::Vec2 pos, sge::Vec3& normal){

    // traverse triangle list and find the picked triangle
    Mesh* mesh=_terrain->getRenderObject()->getMesh();
    sge::Mat4 tranform= _terrain->getModelMatrix();

    size_t polycount = mesh->indices.size()/3.0;
    
    Ray ray= Ray(Vec3(pos.x, 1000, pos.y), Vec3(0, -1, 0));
    
    float result = 1.0e6f;
    float distance = 0.0f;
    Vec3 v0, v1, v2;

    for(size_t i=0; i<polycount; i++){
        
        // get a single triangle from the mesh
        v0=mesh->positions[mesh->indices[i*3]];
        v1=mesh->positions[mesh->indices[i*3+1]];
        v2=mesh->positions[mesh->indices[i*3+2]];
        
        // transform triangle to world space
        tranform.transformPoint(&v0);
        tranform.transformPoint(&v1);
        tranform.transformPoint(&v2);

        // test to see if the ray intersects with this triangle
        if( ray.calcTriangleIntersection(v0, v1, v2, &distance) ) {
            
            // set our result to this if its closer than any intersection we've had so far
            if( distance < result ) {
                result = distance;
                // assuming this is the closest triangle, we'll set our normal
                // while we've got all the points handy
                normal = ( v1 - v0 ).cross( v2 - v0 ).normalized();
            }
        }
    }
    
    // did we have a hit?
    if( distance > 0 ) {
        Vec3 pickedPoint = ray.getPointForLength(result);
        return pickedPoint.y;
    }
  
    
    return 0;
}


void TestSceneMaster::draw(){
    
    Renderer* renderer= Director::getInstance()->getRenderer();
    Vec2 winS= Director::getInstance()->getWindowSize();
    
    _debugView->clear();
    _debugView->addInfoInt("FPS", int(Director::getInstance()->getCurrentFps()));
    _debugView->addInfoInt("DrawCalls",  Benchmark::getInstance()->getCounter("drawCalls"));
    _debugView->addInfoFloat("DrawTime", Benchmark::getInstance()->getTimer(2)->getTimeInMs());
    _debugView->addInfoInt("Tex Switches", Benchmark::getInstance()->getCounter("texSwitches"));
    _debugView->addInfoInt("TriCount", Benchmark::getInstance()->getCounter("triCount"));
    
    
    _frustumCuller->checkVisibiltyForRootNodeAndCamera(_worldNode, _camera);
    _frustumCuller->checkVisibiltyForRootNodeAndCamera(_worldNode, _shadowMap->getCameraAtIndex(0));
    
    _worldNode->getRenderObject()->multiplicateHiddenForCameraWithCamera(_shadowMap->getCameraAtIndex(0),_camera);
    
     Director::getInstance()->getScreenBuffer()->clear(0, 1, 0);
    
   
   //   Benchmark::getInstance()->resetTimer(2);
  //  Benchmark::getInstance()->runTimer(2);
    
    _shadowMap->clear();
    _shadowMap->renderLayerIntoLevel(renderer->getRenderLayer(RENDER_LAYER_SCENE), _shadowCam, _camera, 0);
    _shadowMap->finish();
    

    // Forward
    _buffer[0]->clear(0, true, 0);
    //renderer->renderLayer(renderer->getRenderLayer(RENDER_LAYER_SCENE), _buffer[0], _camera);
    renderer->renderLayer(renderer->getRenderLayer(RENDER_LAYER_SKY), _buffer[0], _camera);
    
    
    // Deferred
    _deferredBuffer->clear();
    _deferredBuffer->renderInLayer(renderer->getRenderLayer(RENDER_LAYER_SCENE), _camera);
    _deferredLightingShader->getGlobalShaderState()->setUniformTexture("sge_albedo_tex", _deferredBuffer->getColorTexture(), 0);
    
    
    _ssao->render(_deferredBuffer, _camera);

    Director::getInstance()->getCompositor()->compose(NULL, _buffer[0], BlendFunc::NORMAL, _deferredLightingShader);
    
    Director::getInstance()->getCompositor()->compose(_buffer[0]->getRenderTarget(0)->getTexture(), _buffer[1], BlendFunc::NORMAL, _fxaaShader);
    
    // Bloom
    float exposure=1.2;
    _bloomEffect->setThreshold(2.0/exposure);
    _bloomEffect->renderInTexture(_buffer[0]->getRenderTarget(0)->getTexture());
    Director::getInstance()->getCompositor()->compose(_bloomEffect->getTexture(),  _buffer[1], BlendFunc::ADD, NULL);
    
    
    // Sollte eigl nicht nötig sein
      _lensShader->getGlobalShaderState()->setUniformTexture("u_colorRamp", Texture::get("assets/textures/colorRamp.png"), 2);
    Director::getInstance()->getCompositor()->compose(_buffer[1]->getRenderTarget(0)->getTexture(), Director::getInstance()->getScreenBuffer(), BlendFunc::NORMAL, _lensShader, 1);

    
    renderer->renderLayer(renderer->getRenderLayer(RENDER_LAYER_UI), Director::getInstance()->getScreenBuffer(), Director::Camera2d);
    

   // Benchmark::getInstance()->stopTimer(2);

}

void TestSceneMaster::update(float delta){
    
    _time+=delta*0.001;
        
    _shadowCam->setPosition(Vec3(sin(_time)*7, 4, cos(_time)*7));
    _shadowCam->lookAt(Vec3(0, 0, 0));
    
    _globalStateEnviroment.setUniform3f("u_player_pos", _modelRigged->transform().getTranslation());
    _globalStateEnviroment.setUniform3f("u_eye_pos", _camera->transform().getTranslation());
    _globalStateEnviroment.setUniform3f("lightDirection", _shadowCam->getDirection()*Vec3(-1, -1, -1));


    Vec3 normal;
    _modelRigged->transform().setTranslationY(getHeightOfTerrain(Vec2(_modelRigged->transform().getTranslation().x,_modelRigged->transform().getTranslation().z), normal));
    
    
    if(_selectedModel){
    
      Bone* bone=  _modelRigged->getArmature()->getBoneNamed("Lower Arm.R");
        
        Vec3 pos(-2.37424,0.14948,1.22448);
        bone->globalTransform.transformPoint(&pos);
        
    
        _selectedModel->transform().setTranslation(pos);
  
    }
    
    
}

void TestSceneMaster::touchBegan(sge::TouchEvent* event){}
void TestSceneMaster::touchMoved(sge::TouchEvent* event){}

void TestSceneMaster::touchEnded(sge::TouchEvent* event){

    Vec2 winS= Director::getInstance()->getWindowSize();
    

    Ray ray;
    _camera->pickRay(Rectangle(0, 0, winS.x, winS.y), winS.x*0.5, winS.y*0.5, &ray);
    

    Model* nearestModel=NULL;
    float nearestDistance=100000;
    
    
    if(_selectedModel==NULL){
    
        for (int i=0; i<300; i++) {
            
            float t=_farn[i]->getBoundingBox().intersects(ray);
            if(t!=Ray::INTERSECTS_NONE && t<nearestDistance){
                
                nearestModel=_farn[i];
                nearestDistance=t;
                
            }
            
            
        }

        if(nearestModel){
        
            _selectedModel=nearestModel;
       
            _selectedModel->removeFromParent();
            _selectedModel->transform().setTranslation(0, 0, 1);
            _modelRigged->addChild(_selectedModel);
            
        }
    
        return;
        
    }
    
    
    for (int i=0; i<25; i++) {
        
        float t=_testModels[i]->getBoundingBox().intersects(ray);
        if(t!=Ray::INTERSECTS_NONE && t<nearestDistance){
        
            nearestModel=_testModels[i];
            nearestDistance=t;
        
        }
        
        
    }

    
    if(nearestModel){
    
        Vec3 normal;
        float polyT= nearestModel->getRenderObject()->getMeshRayIntersection(&ray, normal);
        
        if(polyT!=0.0){
            
            _selectedModel->transform().setRotationFromDirection(normal);
            _selectedModel->transform().setTranslation(ray.getPointForLength(polyT));
            
            _selectedModel->removeFromParent();
            _worldNode->addChild(_selectedModel);

            
            _selectedModel=NULL;

            return;
            
        }

    }else{
        
        Vec3 normal;
        float polyT= _terrain->getRenderObject()->getMeshRayIntersection(&ray, normal);

        if(polyT<50){
        
            _selectedModel->transform().setRotationFromDirection(normal);
            _selectedModel->transform().setTranslation(ray.getPointForLength(polyT));
        
            _selectedModel->removeFromParent();
            _worldNode->addChild(_selectedModel);
            _selectedModel=NULL;

        }
        
    
    }
    

}

void TestSceneMaster::mouseWheel(sge::TouchEvent* event){}


void TestSceneMaster::shaderChanged(sge::FileUtils::FileWatcher::Event* event){

    std::string pathChanged= event->changedPath;
    SString::replaceAll(pathChanged, "/Users/SirniciMac/Projects/SGEngine/", "");
    
    ShaderModule::get(pathChanged)->refresh();

}


void TestSceneMaster::didReleaseKey(KeyEvent* event){
    
    if(event->KeyCode==KeyEvent::ESCAPE){
        
        Director::getInstance()->setMouseTrapped(!Director::getInstance()->isMouseTrapped());
       
        CameraControlComponent*ccc= (CameraControlComponent*) _camera->getComponentByName("FlyoverCameraComponent");
        ccc->setLockAngles(!Director::getInstance()->isMouseTrapped());
        
    }
    
    if(event->KeyCode==KeyEvent::W){
        
        _modelRigged->playAnimation("idle", 1, 1);
    }
    
}

void TestSceneMaster::didPressKey(KeyEvent* event){
    
    if(event->KeyCode==KeyEvent::W){
        
        _modelRigged->playAnimation("main", 1.5, 1);
    }
  
}

void TestSceneMaster::enter(){
    
    
}

void TestSceneMaster::exit(){
    
    
}

TestSceneMaster::~TestSceneMaster(){
    
    
}
