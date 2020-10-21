#include "ShaderFormat_GL_2_1.h"
#include "ShaderModule.h"

namespace sge {

    ShaderFormat_GL_2_1::ShaderFormat_GL_2_1(){

        _prefix="#version 120\n";
        
    }

    ShaderFormat_GL_2_1::~ShaderFormat_GL_2_1(){}

    std::string ShaderFormat_GL_2_1::getVertexSource(ShaderModule* shaderModule){

//        std::string source=_prefix;
//        ShaderModule* sm=shaderModule;
//        
//        for (int i=0; i<sm->_shaderParts[SHADERTYPE_VERTEX].structs.size(); i++) {
//            
//            source.append(sm->_shaderParts[SHADERTYPE_VERTEX].structs[i].code + "\n\n\n");
//            
//        }
//        
//        if(sm->_shaderParts[SHADERTYPE_VERTEX].structs.size()>0) source.append("\n");
//        
//        for (int i=0; i<sm->_shaderParts[SHADERTYPE_VERTEX].attributes.size(); i++) {
//            
//            ShaderModule::VariablePair vp=sm->_shaderParts[SHADERTYPE_VERTEX].attributes[i];
//            source.append("attribute "+ vp.varType+ " " +vp.varName + ";\n");
//            
//        }
//        
//        if(sm->_shaderParts[SHADERTYPE_VERTEX].attributes.size()>0) source.append("\n");
//        
//        for (int i=0; i<sm->_shaderParts[SHADERTYPE_VERTEX].variying.size(); i++) {
//            
//            ShaderModule::VariablePair vp=sm->_shaderParts[SHADERTYPE_VERTEX].variying[i];
//            
//            if(vp.varType.find("flat")!=-1 || vp.varName.find("flat")!=-1){
//                
//                SString::replaceAll(vp.varType, "flat", "");
//                SString::replaceAll(vp.varName, "flat", "");
//                
//                ShaderModule::VariablePair vpnew= sm->getVariablePair(vp.varName);
//                
//                source.append("flat varying "+ vpnew.varType+ " " +vpnew.varName + ";\n");
//                
//            }else{
//                
//                source.append("varying "+ vp.varType+ " " +vp.varName + ";\n");
//                
//            }
//        }
//        
//        if(sm->_shaderParts[SHADERTYPE_VERTEX].variying.size()>0) source.append("\n");
//        
//        for (int i=0; i<sm->_shaderParts[SHADERTYPE_VERTEX].uniform.size(); i++) {
//            
//           ShaderModule:: VariablePair vp=sm->_shaderParts[SHADERTYPE_VERTEX].uniform[i];
//            source.append("uniform "+ vp.varType+ " " +vp.varName + ";\n");
//            
//        }
//        
//        for (int i=0; i<sm->_shaderParts[SHADERTYPE_VERTEX].precodeFragment.size(); i++) {
//            
//            source.append(sm->_shaderParts[SHADERTYPE_VERTEX].precodeFragment[i].code + "\n");
//            
//        }
//        
//        if(sm->_shaderParts[SHADERTYPE_VERTEX].precodeFragment.size()>0) source.append("\n");
//        
//        
//        
//        for (int i=0; i<sm->_shaderParts[SHADERTYPE_VERTEX].functions.size(); i++) {
//            
//            source.append(sm->_shaderParts[SHADERTYPE_VERTEX].functions[i].code + "\n\n\n");
//            
//        }
//        
//        if(sm->_shaderParts[SHADERTYPE_VERTEX].functions.size()>0) source.append("\n\n\n");
//    
//        source.append("\nvoid main(){\n\n");
//        
//        for (int i=0; i<sm->_shaderParts[SHADERTYPE_VERTEX].moduleVars.size(); i++) {
//            
//            ShaderModule::VariablePair vp=sm->_shaderParts[SHADERTYPE_VERTEX].moduleVars[i];
//            source.append(vp.varType+ " " +vp.varName + ";\n");
//            
//        }
//        
//        if(sm->_shaderParts[SHADERTYPE_VERTEX].moduleVars.size()>0) source.append("\n");
//        
//        for (int i=0; i<sm->_shaderParts[SHADERTYPE_VERTEX].codeFragment.size(); i++) {
//            
//            ShaderModule::CodeFragmentPair vp=sm->_shaderParts[SHADERTYPE_VERTEX].codeFragment[i];
//            source.append(vp.code+"\n");
//            
//        }
//        
//        source.append("\n}");

        return "";

    }

    std::string ShaderFormat_GL_2_1::getFragmentSource(ShaderModule* shaderModule){

//        std::string source=_prefix;
//        ShaderModule* sm=shaderModule;
//        
//        for (int i=0; i<sm->_shaderParts[SHADERTYPE_FRAGMENT].structs.size(); i++) {
//            
//            source.append(sm->_shaderParts[SHADERTYPE_FRAGMENT].structs[i].code + "\n\n\n");
//            
//        }
//        
//        if(sm->_shaderParts[SHADERTYPE_FRAGMENT].structs.size()>0) source.append("\n");
//        
//        
//        for (int i=0; i<sm->_shaderParts[SHADERTYPE_FRAGMENT].codeDefines.size(); i++) {
//            
//            source.append(sm->_shaderParts[SHADERTYPE_FRAGMENT].codeDefines[i]);
//            
//        }
//        
//        for (int i=0; i<sm->_shaderParts[SHADERTYPE_FRAGMENT].variying.size(); i++) {
//            
//            ShaderModule::VariablePair vp=sm->_shaderParts[SHADERTYPE_FRAGMENT].variying[i];
//            source.append("varying "+ vp.varType+ " " +vp.varName + ";\n");
//            
//        }
//        
//        if(sm->_shaderParts[SHADERTYPE_FRAGMENT].variying.size()>0) source.append("\n");
//        
//        for (int i=0; i<sm->_shaderParts[SHADERTYPE_FRAGMENT].uniform.size(); i++) {
//            
//            ShaderModule::VariablePair vp=sm->_shaderParts[SHADERTYPE_FRAGMENT].uniform[i];
//            source.append("uniform "+ vp.varType+ " " +vp.varName + ";\n");
//            
//        }
//        
//        if(sm->_shaderParts[SHADERTYPE_FRAGMENT].uniform.size()>0) source.append("\n");
//        
//        for (int i=0; i<sm->_shaderParts[SHADERTYPE_FRAGMENT].precodeFragment.size(); i++) {
//            
//            source.append(sm->_shaderParts[SHADERTYPE_FRAGMENT].precodeFragment[i].code + "\n");
//            
//        }
//        
//        if(sm->_shaderParts[SHADERTYPE_FRAGMENT].precodeFragment.size()>0) source.append("\n");
//        
//        
//        for (int i=0; i<sm->_shaderParts[SHADERTYPE_FRAGMENT].functions.size(); i++) {
//            
//            source.append(sm->_shaderParts[SHADERTYPE_FRAGMENT].functions[i].code + "\n\n\n");
//            
//        }
//        
//        if(sm->_shaderParts[SHADERTYPE_FRAGMENT].functions.size()>0) source.append("\n\n\n");
//        
//        
//        source.append("void main(){\n\n");
//        
//        for (int i=0; i<sm->_shaderParts[SHADERTYPE_FRAGMENT].moduleVars.size(); i++) {
//            
//            ShaderModule::VariablePair vp=sm->_shaderParts[SHADERTYPE_FRAGMENT].moduleVars[i];
//            source.append(vp.varType+ " " +vp.varName + ";\n");
//            
//        }
//        
//        if(sm->_shaderParts[SHADERTYPE_FRAGMENT].moduleVars.size()>0) source.append("\n");
//        
//        for (int i=0; i<sm->_shaderParts[SHADERTYPE_FRAGMENT].codeFragment.size(); i++) {
//            
//            ShaderModule::CodeFragmentPair vp=sm->_shaderParts[SHADERTYPE_FRAGMENT].codeFragment[i];
//            source.append(vp.code+"\n");
//            
//        }
        
        //source.append("\n\n}");
        return "";

    }

}