#include "ShaderModule.h"
#include "SBase.h"
#include "ShaderManager.h"
#include "ShaderParser.h"

#include "ShaderFormat.h"
#include "ShaderFormat_GL_2_1.h"
#include "ShaderFormat_GL_4_1.h"

#include "Range.h"
#include "Director.h"
#include <stack>

namespace sge {
    
    std::unordered_map<std::string, ShaderModule*> ShaderModule::_cache;
    ShaderParser* ShaderModule::_shaderParser= new ShaderParser();

    ShaderModule::ShaderModule(){
    
        shaderParts[0].type=SHADER_PART_TYPE;
        shaderParts[1].type=SHADER_PART_TYPE;
        shaderParts[2].type=SHADER_PART_TYPE;
        
        registerShaderState(&_state);

    }
    
    ShaderModule::~ShaderModule(){}
    
    void ShaderModule::registerShaderState(ShaderState* shaderState){
    
        if(std::find(_registerShaderStates.begin(), _registerShaderStates.end(), shaderState)==_registerShaderStates.end()){
        
            _registerShaderStates.push_back(shaderState);
            shaderState->addObserver(this);
            
            // init uploead
            std::unordered_map<std::string, Shader*>::iterator it;
            
            for (it= _subShaders.begin(); it!=_subShaders.end(); ++it) {
                
                shaderState->uploadToShader(it->second);
                
            }
            
        
        }
    
    }
    
    void ShaderModule::shaderStateChanged(ShaderState* state){
    
        std::unordered_map<std::string, Shader*>::iterator it;

        for (it= _subShaders.begin(); it!=_subShaders.end(); ++it) {
            
            it->second->_dirtyShaderStates.insert(state);
            //state->uploadToShader(it->second);
            
        }
    
    
    }

    
    ShaderModule* ShaderModule::get(const std::string& file){
    
        if(_cache.find(file)!=_cache.end()) return _cache[file];
        
        ShaderModule* module= new ShaderModule();
        module->_filePath=file;
        _cache.insert(std::make_pair(file, module));

        _shaderParser->setLocationTarget(LOCATION_TARGET_APP_BUNDLE);
        _shaderParser->parseShaderIntoModule(file, module);
        
        return module;
    
    }
    
    void ShaderModule::addDefine(const std::string& define){
        
        _shaderDefines.insert(define);
        
    }
    
    void ShaderModule::removeDefine(const std::string& define){
        
        _shaderDefines.erase(define);
        
    }
    
    void ShaderModule::replaceDefine(const std::string& defineToBeReplaced, const std::string& define){
        
        _shaderDefines.erase(defineToBeReplaced);
        _shaderDefines.insert(define);
        
    }
    
    
    ShaderState* ShaderModule::getGlobalShaderState(){
    
        return &_state;
    
    }
    
    
    std::string ShaderModule::getSourceForShaderPart(ShaderPart* part, std::string& outIncludes){
    
        std::string source="";
        std::string includeSource="";

        std::vector<ItHelper> shaderPartStack;
        shaderPartStack.push_back(ItHelper(part));
        
        std::vector<ShaderPart*> includeParts;
        
        while (shaderPartStack.size()>0) {
            
            if(shaderPartStack.back().iterator==0){
                
            if(shaderPartStack.back().shaderPart->type==SHADER_PART_CONTENT){
                
                source.append(shaderPartStack.back().shaderPart->content);
                shaderPartStack.pop_back();
                continue;
                
            }
            
            if(shaderPartStack.back().shaderPart->type==SHADER_PART_INCLUDE && shaderPartStack.size()>1){
                
                includeSource.append(getSourceForShaderPart(shaderPartStack.back().shaderPart,outIncludes));
                includeParts.push_back(shaderPartStack.back().shaderPart);
                shaderPartStack.pop_back();
                continue;
                
            }
            
            if(shaderPartStack.back().shaderPart->type==SHADER_PART_AREA && shaderPartStack.back().iterator==0 && shaderPartStack.back().shaderPart->name=="main"){
                
                source.append("\nvoid main(){\n\n");
                
            }
            
                
            }
            
            
            if(shaderPartStack.back().iterator>=shaderPartStack.back().shaderPart->subparts.size()){
                
                if(shaderPartStack.back().shaderPart->type==SHADER_PART_AREA  && shaderPartStack.back().shaderPart->name=="main"){
                    
                    source.append("\n\n}\n\n");
                    
                }
                
                shaderPartStack.pop_back();
                
            }else{
                
                shaderPartStack.back().iterator++;
                shaderPartStack.push_back(ItHelper(&shaderPartStack.back().shaderPart->subparts[shaderPartStack.back().iterator-1]));
                
            }
            
        }
        
        outIncludes.append(includeSource);
        return source;
        
    }
    
    std::string ShaderModule::getVertexSource(const std::vector<std::string>& defines, ShaderState* initUniforms){
        
        ShaderPart shaderPartxVertexCopy=shaderParts[0];
        
        _shaderParser->filterShaderPart(&shaderPartxVertexCopy, defines);
    
        std::string includeSource;

        std::string vertexSource=getSourceForShaderPart(&shaderPartxVertexCopy,includeSource);
        
        ShaderVarContainer varContainer;
        
        _shaderParser->parseIOUfromSource(vertexSource, &varContainer);
        _shaderParser->replaceVariables(vertexSource, "output.position", "gl_Position");
        
        std::string header;
        
        for (int i=0; i<varContainer.inputs.size(); i++) {
            
            ShaderVar* var=&varContainer.inputs[i];
            header.append("in "+var->type+" "+var->name+";\n");
            
        }
        
        header.append("\n");
        
        for (int i=0; i<varContainer.outputs.size(); i++) {
            
            ShaderVar* var=&varContainer.outputs[i];
            header.append("out "+var->type+" "+var->name+";\n");
            
        }
        
        header.append("\n");
        
        for (int i=0; i<varContainer.uniforms.size(); i++) {
            
            ShaderVar* var=&varContainer.uniforms[i];
            header.append("uniform "+var->type+" "+var->name+";\n");
            
            if(var->initVar.length()!=0){
                
                initUniforms->setUniformWithStrings(var->name, var->type, var->initVar);
                
            }
            
        }
        
        header.append("\n\n");
    
        return "#version 330\n"+includeSource+header+vertexSource;
    
    }
    
    std::string ShaderModule::getFragSource(const std::vector<std::string>& defines, ShaderState* initUniforms){
    
    
        ShaderPart shaderPartFragCopy=shaderParts[2];
        
        std::string includeSource;
        _shaderParser->filterShaderPart(&shaderPartFragCopy, defines);
        std::string fragSource=getSourceForShaderPart(&shaderPartFragCopy, includeSource);
        
        
        ShaderVarContainer varContainer;
        int outs=0;
        for (int i=0; i<10; i++) {
            
            std::stringstream ss; ss <<"["<<i<<"]";
            std::stringstream ss1; ss1<<i;
            
            if(SString::containsString(fragSource, "output"+ss.str())) outs++;
            _shaderParser->replaceVariables(fragSource, "output"+ss.str(), "sge_output_"+ss1.str());

        }
        
        _shaderParser->parseIOUfromSource(fragSource, &varContainer);
        _shaderParser->replaceVariables(fragSource, "output.color", "fragColor");
        _shaderParser->replaceVariables(fragSource, "texture2D", "texture");
        _shaderParser->replaceVariables(includeSource, "texture2D", "texture");

        std::string header;
        
        for (int i=0; i<varContainer.inputs.size(); i++) {
            
            ShaderVar* var=&varContainer.inputs[i];
            header.append("in "+var->type+" "+var->name+";\n");
            
        }
        
        header.append("\n");
        
    
        if(outs>0){
        
            for (int i=0; i<outs; i++) {
           
                std::stringstream ss; ss <<"layout(location = "<< i <<") out vec4 sge_output_"<<i<<";\n";
                header.append(ss.str());

            }
        
        }else{
        
            header.append("out vec4 fragColor;\n");

        
        }

        header.append("\n");
        
        for (int i=0; i<varContainer.uniforms.size(); i++) {
            
            ShaderVar* var=&varContainer.uniforms[i];
            header.append("uniform "+var->type+" "+var->name+";\n");
            
            if(var->initVar.length()!=0){
            
                initUniforms->setUniformWithStrings(var->name, var->type, var->initVar);
            
            }
            
        }
        
        header.append("\n\n");
        
        return "#version 330\n"+includeSource+header+fragSource;
        
    }
    
    Shader* ShaderModule::getShaderForDefines(std::vector<std::string> defines){
        
        std::string key="";
        for (int i=0; i<defines.size(); i++) {
            key.append(defines[i]+"@");
        }
        
        if(_subShaders.find(key)!=_subShaders.end()){
        
            return _subShaders[key];
            
        }
        
        defines.insert(defines.end(), _shaderDefines.begin(), _shaderDefines.end());
        
        ShaderState initUniforms;
        std::string vertex= getVertexSource(defines, &initUniforms);
        std::string frag= getFragSource(defines, &initUniforms);
        
        Shader* shader= new Shader(vertex,frag);
        shader->initUniforms=initUniforms;
        
        initUniforms.uploadToShader(shader);
        
        _subShaders.insert(std::make_pair(key, shader));
        shader->_parentModule=this;
        
        for (int i=0; i<_registerShaderStates.size(); i++) {
            
            _registerShaderStates[i]->uploadToShader(shader);
            
        }
        
        shader->shaderDefines=defines;
        
        return shader;
    
    }
    
    void ShaderModule::ShaderVarContainer::addShaderVar(const ShaderVar& var){
    
        // Add only if not already in
        // If already in check if information can be appended
        
        // Inputs
        if(var.function=="input"){
        
            bool found=false;
            for (int i=0; i<inputs.size(); i++) {
                
                if(inputs[i].name==var.name){
                
                    found=true;
                    
                    if(inputs[i].type.length()==0) inputs[i].type=var.type;
                    if(inputs[i].global==0) inputs[i].global=var.global;
                    if(inputs[i].initVar.length()==0) inputs[i].initVar=var.initVar;

                }
                
            }
            
            if(!found){
            
                inputs.push_back(var);
            
            }
        
        }
    
        // Outputs
        if(var.function=="output"){
            
            bool found=false;
            for (int i=0; i<outputs.size(); i++) {
                
                if(outputs[i].name==var.name){
                    
                    found=true;
                    
                    if(outputs[i].type.length()==0) outputs[i].type=var.type;
                    if(outputs[i].global==0) outputs[i].global=var.global;
                    if(outputs[i].initVar.length()==0) outputs[i].initVar=var.initVar;
                    
                }
                
            }
            
            if(!found){
                
                outputs.push_back(var);
                
            }
            
        }
    
        // uniforms
        if(var.function=="uniform"){
            
            bool found=false;
            for (int i=0; i<uniforms.size(); i++) {
                
                if(uniforms[i].name==var.name){
                    
                    found=true;
                    
                    if(uniforms[i].type.length()==0) uniforms[i].type=var.type;
                    if(uniforms[i].global==0) uniforms[i].global=var.global;
                    if(uniforms[i].initVar.length()==0) uniforms[i].initVar=var.initVar;
                    
                }
                
            }
            
            if(!found){
                
                uniforms.push_back(var);
                
            }
            
        }
        
        
        
        
    }
    
    
    void ShaderModule::refresh(){
    
        _shaderParser->setLocationTarget(LOCATION_TARGET_PROJECT);
        _shaderParser->parseShaderIntoModule(_filePath, this);
      
        std::unordered_map<std::string, Shader*>::iterator it;
      
        for (it=_subShaders.begin(); it!=_subShaders.end(); ++it) {
            
            ShaderState initUniforms;
            std::string vertex  = getVertexSource(it->second->shaderDefines, &initUniforms);
            std::string frag    = getFragSource  (it->second->shaderDefines, &initUniforms);
            
            it->second->recompile(vertex, frag);
            it->second->initUniforms=initUniforms;
            initUniforms.uploadToShader(it->second);
            
            for (int i=0; i<_registerShaderStates.size(); i++) {
                
                _registerShaderStates[i]->uploadToShader(it->second);
                
            }
        
        }
    
        
    }


    
}












