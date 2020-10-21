#include "Application.h"

#include <time.h>
#include <iostream>
#include <memory.h>

#include "glInclude.h"

#include "Resolutions.h"
#include "Director.h"

using namespace std;

namespace sge {
    
    
    double Application::_lastUpdate=0;
    double Application::_lastDraw=0;
    
    void Director::setMouseTrapped(bool trapped){}
    
    int Application::run(ApplicationController* controller, const std::string& configFile){
    
        controller->willLaunch();

        
        controller->didFinishLaunching();

        return 0;
    }
    
    
    void Application::init(){
        
        
    }
    
    void Application::update(){
        
//        float fpsLimit=1./ Director::getInstance()->getFpsLimit();
//        
//        if(SDL_GetTicks()-_lastDraw>fpsLimit*100*0.5){
//            
//            _lastDraw=SDL_GetTicks();
//            
//            glClear(GL_COLOR_BUFFER_BIT);
//            
//            Director::getInstance()->draw();
//            SDL_GL_SwapWindow(sdl_window);
//        }
//        
//        Director::getInstance()->update((SDL_GetTicks()-_lastUpdate)/100.);
//        _lastUpdate=SDL_GetTicks();
//    
        
    }
    


}


//int sdl_event_filter(void* userdata, SDL_Event* event){
//    
//    
//    Vec2 winS= Director::getInstance()->getWindowSize();
//    
//    //The mouse offsets
//    int x = 0, y = 0;
//    
//    //If the mouse moved
//    if( event->type == SDL_MOUSEMOTION )
//    {
//        //Get the mouse offsets
//        x = event->motion.x;
//        y = event->motion.y;
//        
//        Director::getInstance()->mouseMoved(Vec2(x, winS.y-y), true);
//        return 0;
//        
//    }
//    
//    if( event->type == SDL_MOUSEBUTTONUP )
//    {
//        //Get the mouse offsets
//        x = event->motion.x;
//        y = event->motion.y;
//        Director::getInstance()->mouseUp(Vec2(x, winS.y-y),true);
//        return 0;
//        
//    }
//    
//    if( event->type == SDL_WINDOWEVENT_RESIZED )
//    {
//        
//        Director::getInstance()->setWindowSize(Vec2(event->window.data1, event->window.data2));
//        
//        return 0;
//        
//    }
//    
//    
//    if( event->type == SDL_MOUSEBUTTONDOWN)
//    {
//        //Get the mouse offsets
//        x = event->motion.x;
//        y = event->motion.y;
//        Director::getInstance()->mouseDown(Vec2(x, winS.y-y),true);
//        return 0;
//        
//    }
//    
//    switch (event->type) {
//            
//        case SDL_KEYDOWN:
//            Director::getInstance()->keyDown(KeyEvent(int(event->key.keysym.sym)));
//            return 0;
//        case SDL_KEYUP:
//            Director::getInstance()->keyUp(KeyEvent(int(event->key.keysym.sym)));
//            return 0;
//        default:
//            break;
//    }
//    
//    
//    return 1;
//}
//

