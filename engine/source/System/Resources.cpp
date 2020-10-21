#include "Resources.h"
#include <iomanip>

Resource::Resource(std::string type){

    this->type=type;
    sizeInByte=0;
    ResourceManager::getInstance()->addResource(this);

}

Resource::~Resource(){

    ResourceManager::getInstance()->removeResource(this);

}

ResourceManager* ResourceManager::getInstance(){

    static ResourceManager* _instance=NULL;
    if(!_instance) _instance= new ResourceManager();
    
    return _instance;
    
}

void ResourceManager::addResource(Resource* resource){

    ressourceTypeMap[resource->type].insert(resource);

}

void ResourceManager::removeResource(Resource* resource){

    ressourceTypeMap[resource->type].erase(resource);

}

void ResourceManager::printResourcesForType(const std::string& type){

    double sizeInMBTotal=0;
    
    std::cout<< "\n/////////////// Resources for type: " << type<< std::endl<< std::endl;
    if(ressourceTypeMap.find(type)!=ressourceTypeMap.end()){
        
        std::set<Resource*>::iterator it;
        std::set<Resource*>* set=&ressourceTypeMap[type];
        
        for (it=set->begin(); it!=set->end(); ++it) {
            
            double sizeInMB= (*it)->sizeInByte/double(MEGABYTE);
            sizeInMBTotal+=sizeInMB;
            std::stringstream ss;
            ss<< std::setprecision(2) << sizeInMB;
            
            std::string st=ss.str();
            int l=(int) st.length();
            if(l>6) l=6;
            
            for (int s=0; s<6-l; s++) {
                ss<<" ";
            }
            
            std::cout<< ss.str() << " mb"<< " : " << (*it)->name << std::endl;
            
        }
        
        
    }
    

    std::cout<< "\n/////////////// Total: " << sizeInMBTotal << " mb" << std::endl;


}

unsigned long ResourceManager::getTotalBytesOfResourceType(const std::string& type){

    unsigned long bytes=0;
    if(ressourceTypeMap.find(type)!=ressourceTypeMap.end()){
    
        std::set<Resource*>::iterator it;
        std::set<Resource*>* set=&ressourceTypeMap[type];
    
        for (it=set->begin(); it!=set->end(); ++it) {
            
            bytes+=(*it)->sizeInByte;
            
        }
        
    
    }
    
    return bytes;

}
