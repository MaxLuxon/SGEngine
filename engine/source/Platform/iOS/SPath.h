#include "SBase.h"

#ifndef SGENGINE_SPATH_H
#define SGENGINE_SPATH_H

class SPath{

public:
    static std::string getRessourcePath();
    static std::string getDocumentsPath();
    static std::string getLibraryPath();
    
    static void setPathsToWatch(const std::vector<std::string>& paths);

};


#endif
