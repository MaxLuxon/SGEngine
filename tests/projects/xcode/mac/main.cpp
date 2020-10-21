#include "AppDelegate.h"
#include "SGEngine.h"

int main(int argc, char *argv[]){
    
    AppDelegate appDelegate;
    return sge::Application::run(&appDelegate, "assets/config/main.ini");
    
}
