#include "AppDelegate.h"

#include "TestSceneMaster.h"

using namespace sge;

void AppDelegate::willLaunch(){
    
#ifndef MOBILE_TARGET
    
    Director::getInstance()->setWindowSize(Vec2(200, 200));

#endif
    
    Director::getInstance()->setFpsLimit(60);
    
}

void AppDelegate::didFinishLaunching(){
    
    GPUInfo::print();

    Director::getInstance()->replaceScene(new TestSceneMaster());
    
}

void AppDelegate::entersBackground(){
    
    
}

void AppDelegate::enterForeground(){
    
    
    
}
