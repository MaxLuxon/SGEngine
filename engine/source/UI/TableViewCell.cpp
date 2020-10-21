/////////////////////////////////////////////////////
//      Sirnic's Game Engine © Max Gittel          //
/////////////////////////////////////////////////////

#include "TableViewCell.h"

namespace sge {
    
    TableViewCell* TableViewCell::create(Vec2 size){
    
        TableViewCell* tvc= new TableViewCell();
        tvc->initWithSizeColorTexture(size.x, size.y, Color::white, NULL);
        return tvc;
    
    }
    
}