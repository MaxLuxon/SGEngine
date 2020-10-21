#include "Application.h"

#include <time.h>
#include <iostream>
#include <memory.h>

#include <OpenGL/gl.h>
#include <SDL.h>

#include "Resolutions.h"
#include "Director.h"
#include "IniParser.h"

#include "ShaderFormat_GL_2_1.h"
#include "ShaderFormat_GL_4_1.h"

#include "ShaderModule.h"

using namespace std;

namespace sge {
    
    double Application::_lastUpdate=0;
    double Application::_lastDraw=0;
    
    SDL_Window* sdl_window=NULL;
    
    void Director::setMouseTrapped(bool trapped){
        
        _isMouseTrapped=trapped;
        
        if(trapped){
        
            SDL_SetWindowGrab(sdl_window, SDL_TRUE);
            SDL_SetRelativeMouseMode(SDL_TRUE);
        
        }else{
        
            SDL_SetWindowGrab(sdl_window, SDL_FALSE);
            SDL_SetRelativeMouseMode(SDL_FALSE);
            
            Vec2 winS= Director::getInstance()->getWindowSize();

            SDL_WarpMouseInWindow(sdl_window, winS.x/2.0, winS.y/2.0);

        }
        
    }
    
    int sdl_event_filter(void* userdata, SDL_Event* event){
        
        
        Vec2 winS= Director::getInstance()->getWindowSize();
        
        //The mouse offsets
        float x = 0, y = 0;
        
        //If the mouse moved
        if( event->type == SDL_MOUSEMOTION )
        {
            
            if(Director::getInstance()->isMouseTrapped()){
            
                //Get the mouse offsets
                x = event->motion.xrel;
                y = event->motion.yrel;
                
                Director::getInstance()->mouseMovedRelative(Vec2(x, -y), true);
            
                return 0;
            }
            
            //Get the mouse offsets
            x = event->motion.x;
            y = event->motion.y;
            
            
            Director::getInstance()->mouseMoved(Vec2(x, winS.y-y), true);
            
         
            
            return 0;
            
        }
        
        if( event->type == SDL_MOUSEBUTTONUP )
        {
            //Get the mouse offsets
            x = event->motion.x;
            y = event->motion.y;
            Director::getInstance()->mouseUp(Vec2(x, winS.y-y),true);
            return 0;
            
        }
        
        if( event->type == SDL_WINDOWEVENT_RESIZED ){
            
            Director::getInstance()->setWindowSize(Vec2(event->window.data1, event->window.data2));
            return 0;
            
        }
        
        
        if( event->type == SDL_MOUSEBUTTONDOWN){
            
            //Get the mouse offsets
            x = event->motion.x;
            y = event->motion.y;
            Director::getInstance()->mouseDown(Vec2(x, winS.y-y),true);
            return 0;
            
        }
        
        if( event->type == SDL_MOUSEWHEEL){
           
            Director::getInstance()->mouseScrolled(Vec2(event->wheel.x, event->wheel.y));
            return 0;
            
        }
        
        switch (event->type) {
                
            case SDL_KEYDOWN:
                Director::getInstance()->keyDown(KeyEvent(int(event->key.keysym.sym)));
                return 0;
            case SDL_KEYUP:
                Director::getInstance()->keyUp(KeyEvent(int(event->key.keysym.sym)));
                return 0;
            default:
                break;
        }
        
        
        return 1;
    }



    int Application::run(ApplicationController* controller, const std::string& configFile){
    
        IniReader* configParser= new IniReader();
        std::string fullPath=Director::getInstance()->getFileUtils()->getRessourcePath(configFile);
        configParser->parseFile(fullPath);
        
        controller->willLaunch();

        /* initialize SDL */
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            printf("Could not initialize SDL\n");
            return 1;
        }
        
        
        IniReader::Section* graphicsSection= configParser->getSection("graphics");
        Director::getInstance()->setWindowSize(Vec2(graphicsSection->getValueAsInt("resolutionX"), graphicsSection->getValueAsInt("resolutionY")));
        
        
        IniReader::Section* resourceSection= configParser->getSection("resourceLocations");
        Director::getInstance()->resourceLocations=resourceSection->entries;
        
//        int fpsLimit= graphicsSection->getValueAsInt("fpslimit");
        bool fullscreen= graphicsSection->getValueAsInt("fullscreen");
        std::string renderer= graphicsSection->getValue("renderer");

        Vec2 res=Director::getInstance()->getWindowSize();
        
        SDL_DisplayMode displayMode;
        SDL_GetDesktopDisplayMode(0, &displayMode);
        
        if(renderer=="OPEN_GL_4_1"){
        
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
            SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
            
            //ShaderModule::setShaderFormat(new ShaderFormat_GL_4_1());

        }else{
        
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

          //  ShaderModule::setShaderFormat(new ShaderFormat_GL_2_1());

        }
        
        
        SDL_GL_SetSwapInterval(0);
        
        Vec2 winS= Director::getInstance()->getWindowSize();
        
        if(!fullscreen){
        
            sdl_window= SDL_CreateWindow(NULL, 0, 0,winS.x, winS.y, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
        
        
        }else{
        
            sdl_window= SDL_CreateWindow(NULL, 0, 0,winS.x, winS.y, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN);

        }
        
        if(!sdl_window){
            printf("Could not initialize Window\n");
            return 1;
        }
        
        
        SDL_GLContext gl=SDL_GL_CreateContext(sdl_window);
        
        if( gl == NULL ){
            
            printf( "OpenGL context could not be created! SDL Error: %s\n", SDL_GetError() );
        
        }else{
            
            SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
            SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
            SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
            SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
            SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
            
        }
        
        Director::getInstance()->init();

        cout << "OpenGL version " << glGetString(GL_VERSION) << endl;
        cout << "GLSL version " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
        
        controller->didFinishLaunching();

        SDL_AddEventWatch(sdl_event_filter, NULL);
   
        SDL_Event event;
        bool done=false;
        
        while (!done) {
            
            SDL_PumpEvents();
            while (SDL_PollEvent(&event)) {
                
                switch (event.type) {
                        
                    case SDL_QUIT:
                        done=true;
                        break;
                        
                    case SDL_APP_DIDENTERFOREGROUND:
                        
                        break;
                        
                    case SDL_APP_DIDENTERBACKGROUND:
                        
                        break;
                        
                    case SDL_APP_WILLENTERBACKGROUND:
                        
                        break;
                        
                    case SDL_APP_WILLENTERFOREGROUND:
                        
                        break;
                        
                    case SDL_APP_TERMINATING:
                        
                        break;
                        
                    case SDL_APP_LOWMEMORY:
                        
                        break;
                        
                        
                    default:
                        break;
                }
                
                
            }
            
            update();
            SDL_Delay( 5 );
            
            
        }
        
        /* shutdown SDL */
        SDL_GL_DeleteContext(gl);
        SDL_DestroyWindow(sdl_window);
        SDL_Quit();
        
        return 0;
    }
    
    
    void Application::init(){
        
        
    }
    
    void Application::update(){
        
        float fpsLimit=100./ 60.0;
        
        if(SDL_GetTicks()-_lastDraw>fpsLimit){
            
            _lastDraw=SDL_GetTicks();
            
            Director::getInstance()->timer.reset();
            Director::getInstance()->timer.run();
            Director::getInstance()->draw();
        
    
            SDL_GL_SwapWindow(sdl_window);
            Director::getInstance()->timer.pause();

            if(Director::getInstance()->_frameCount%60==0) Director::getInstance()->_timeCount= Director::getInstance()->timer.getTimeInMs();

            
        }

        
        Director::getInstance()->update((SDL_GetTicks()-_lastUpdate)/100.);
        _lastUpdate=SDL_GetTicks();
        
    }
    


}
