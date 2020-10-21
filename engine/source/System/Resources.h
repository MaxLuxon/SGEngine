#ifndef Resources_h
#define Resources_h

const unsigned long MEGABYTE = 1024 * 1024;

#include "SBase.h"
class Resource{

public:
    
    Resource(std::string type);
    ~Resource();

    unsigned long sizeInByte;
    std::string type;
    std::string name;
    
    bool readyToUse;

};


class ResourceManager{
    
    typedef std::set<Resource*> ResourceArray;

    std::unordered_map<std::string, ResourceArray> ressourceTypeMap;
    
    
public:
    
    static ResourceManager* getInstance();

    void addResource(Resource* resource);
    void removeResource(Resource* resource);
    
    unsigned long getTotalBytesOfResourceType(const std::string& type);
    void printResourcesForType(const std::string& type);

};


#endif /* Ressources_hpp */
