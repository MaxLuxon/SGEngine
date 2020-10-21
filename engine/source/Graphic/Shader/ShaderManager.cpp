#include "ShaderManager.h"
#include "Director.h"

namespace sge {
    
    ShaderManager* ShaderManager::getInstance(){
    
        static ShaderManager* _instance=NULL;
        if(!_instance){
            
            _instance= new ShaderManager();
            _instance->_shaderCount=0;
        
        }
        
        return _instance;
    
    }

    Shader* ShaderManager::findChachedShader(const std::string& nameID){
        
        if(_shaderMap.find(nameID)!=_shaderMap.end()) return _shaderMap[nameID];
        return NULL;
        
    }
    
    Shader* ShaderManager::createNewShader(const std::string& vertexSource, const std::string& fragmentSource, const std::string& nameID){
        
        _shaderCount++;
        
        Shader* shader= new Shader(vertexSource, fragmentSource);
        shader->_nameID=nameID;
        shader->_internID=(int) _shaderCount;
        _shaderMap.insert(std::make_pair(nameID, shader));
        return shader;
        
    }
    
    void ShaderManager::bindShader(Shader* shader){
    
        if(_activeShader==shader) return;
        _activeShader=shader;
        Director::getInstance()->getGraphicDevice()->bindShader(shader);
        
        checkRenderError();
    }
    
    Shader* ShaderManager::getActiveShader(){
    
        return _activeShader;
    
    
    }
    
   
    
}
