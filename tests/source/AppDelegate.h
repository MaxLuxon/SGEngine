#ifndef __SSTests__AppDelegate__
#define __SSTests__AppDelegate__

#include <stdio.h>
#include "SGEngine.h"

class AppDelegate: public sge::ApplicationController{
    
public:
    
    void willLaunch();
    void didFinishLaunching();
    void entersBackground();
    void enterForeground();
    
};


#endif
