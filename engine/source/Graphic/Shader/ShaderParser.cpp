#include "ShaderParser.h"
#include "ShaderModule.h"
#include "SBase.h"
#include "Director.h"

#include <stack>

namespace sge {
    
    ShaderModule::ShaderVar ShaderParser::parserShaderVarFromLine(const std::string& line){
    
        ShaderModule::ShaderVar var;
        std::string text=line;
        
        if(SString::containsString(text, "u_time")){
            
        }
        
        // Global
        if(SString::containsString(text, "global")){
        
            var.global=true;
            SString::replaceAll(text, "global", "");
        
        }
        
        // Function
        if(SString::containsString(text, "input")){
            
            var.function="input";
            SString::replaceAll(text, "input", "");
            
        }else if(SString::containsString(text, "output")){
            
            var.function="output";
            SString::replaceAll(text, "output", "");
            
        }else if(SString::containsString(text, "uniform")){
            
            var.function="uniform";
            SString::replaceAll(text, "uniform", "");
            
        }
        
        SString::removeWhiteSpaceAtFront(text);
        
        unsigned long posW= text.find(" ");
        if(posW!=std::string::npos){
        
            var.type=text.substr(0,posW);
            text.erase(0,posW+1);
        
        }
        
        SString::replaceAll(text, ";", "");
        SString::replaceAll(text, "\n", "");
        
        //init var
        unsigned long posBO= text.find("(");
        unsigned long posBC= text.find(")");
        
        if(posBO!=std::string::npos && posBC!=std::string::npos){
        
            var.initVar=text.substr(posBO+1,posBC-posBO-1);
            text.erase(posBO,posBC);
        
        }
        
        SString::replaceAll(text, " ", "");
        //Name
        
        unsigned long equalPos= text.find("=");
        if(equalPos!=std::string::npos){
            
            text.erase(equalPos,text.length());
            
        }
        
        var.name=text;
        
       

        return var;
        
    }
    
    std::vector<std::pair<std::string, std::string>> ShaderParser::getReplacmentsFromLine(const std::string& line){
    
        std::vector<std::pair<std::string, std::string>> array;
        
        std::string text=line;
        unsigned long pos=text.find("\"");
        if(pos!=std::string::npos) text.erase(0, pos+1);
        pos=text.find("\"");
        if(pos!=std::string::npos) text.erase(0, pos+1);
        pos=text.find("(");
        if(pos!=std::string::npos) text.erase(0, pos+1);
        
        SString::replaceAll(text, "\n", "");
        SString::replaceAll(text, ";", "");
        SString::replaceAll(text, ")", ",");
        
        unsigned long commaPos= text.find(",");
        while (commaPos!=std::string::npos) {
            
            std::string pair=text.substr(0,commaPos);
            text.erase(0,commaPos+1);
            commaPos= text.find(",");

            unsigned long equalPos= pair.find("=");
            std::string firstPart= pair.substr(0, equalPos);
            pair.erase(0,equalPos+1);

            array.push_back(std::make_pair(firstPart, pair));
            
        }

        return array;
        
    }
    
    void ShaderParser::replaceVariables(std::string& source, const std::string& from, const std::string& to){
    
        static std::string div=";= \"\n*+-:&!=%|(),.-[]";
    
        std::vector<size_t> positions; // holds all the positions that sub occurs within str
        std::vector<size_t> positionsToReplace; // holds all the positions that sub occurs within str

        size_t pos = source.find(from, 0);
        while(pos != std::string::npos)
        {
            positions.push_back(pos);
            pos = source.find(from,pos+1);
        }
        
        for (int i=0; i<positions.size(); i++) {
            
            bool isVariableLeft=false;
            bool isVariableRight=false;
            
            int pos=(int)positions[i];
            int posEnd=pos+(int) from.length();

            if(pos-1<0){
            
                isVariableLeft=true;
            
            }else{
            
                if(source.find(source[pos-1])!=std::string::npos){
                
                    isVariableLeft=true;
                    
                }
            
            }
            
            if(posEnd>=source.length()){
                
                isVariableRight=true;
                
            }else{
                
                if(source.find(source[posEnd])!=std::string::npos){
                    
                    isVariableRight=true;
                    
                }
                
            }
            
            
            if(isVariableLeft && isVariableRight){
            
                positionsToReplace.push_back(positions[i]);
            
            }
            
        }
        
        std::string tempReplaceString;
        
        for (int i=0; i<from.length(); i++) {
            tempReplaceString.append("@");
        }
        
        
        for (int i=0; i<positionsToReplace.size(); i++) {
        
            source.replace(positionsToReplace[i], tempReplaceString.length(), tempReplaceString);
            
        }
    
        SString::replaceAll(source, tempReplaceString, to);
    
    }
    
    void ShaderParser::parseIOUfromSource(std::string& string, ShaderModule::ShaderVarContainer* varContainer){
    
        std::string newSource;

        std::istringstream f(string);
        std::string line;

        while (std::getline(f, line)) {
        
            if(SString::containsString(line, "uniform ")){
                
                ShaderModule::ShaderVar var= parserShaderVarFromLine(line);
                varContainer->addShaderVar(var);
                
                continue;
                
            }
            
            if(SString::containsString(line, "input ")){
            
                ShaderModule::ShaderVar var= parserShaderVarFromLine(line);
                varContainer->addShaderVar(var);

                continue;
                    
            }
            
            if(SString::containsString(line, "output ")){
                
                if(!SString::containsString(line, "=")){
                    
                    ShaderModule::ShaderVar var= parserShaderVarFromLine(line);
                    varContainer->addShaderVar(var);
                    
                    continue;
                    
                }else{
                    
                    ShaderModule::ShaderVar var= parserShaderVarFromLine(line);
                    varContainer->addShaderVar(var);
                    
                    SString::replaceFirst(line,"output ","");
                    
                    int pos=(int) line.find(" ");
                    if(pos!=std::string::npos){
                        
                        line.erase(0,pos+1);
                        
                    }
                    
                    newSource.append(line+"\n");
                    continue;
                    
                }
                
            }

        
            newSource.append(line+"\n");

        }
        
        // Check input/ output which still have a type prefix
        for (int i=0; i<varContainer->inputs.size(); i++) {

            std::string toRemove="\n"+varContainer->inputs[i].type+" "+varContainer->inputs[i].name;
            SString::replaceAll(newSource, toRemove, "\n"+varContainer->inputs[i].name);
        
        }
        
        for (int i=0; i<varContainer->outputs.size(); i++) {
            
            std::string toRemove="\n"+varContainer->outputs[i].type+" "+varContainer->outputs[i].name;
            SString::replaceAll(newSource, toRemove, "\n"+varContainer->outputs[i].name);
            
        }

        
        string=newSource;
    
    }
    
    void ShaderParser::parseFileIntoShaderPart(const std::string& file, ShaderModule::ShaderPart* part, ShaderModule* module){
    
        std::string source= getSourceFromFile(file);

        int currentShaderType=-1;
        
        std::istringstream f(source);
        std::string line;
        
        std::stack<ShaderModule::ShaderPart*> _shaderPartStack;
        std::stack<int> _openAreasBracketCount;
        
        if(part) _shaderPartStack.push(part);
        
        int openBracketCount=0;
        
        while (std::getline(f, line)) {
            
            SString::removeWhiteSpaceAtFront(line);
            
            // Shader type ///
            if (SString::containsString(line, "##") && part==NULL) {
                
                if(currentShaderType!=0 && SString::containsString(line, "vertex")){
                    
                    currentShaderType=0;
                    _shaderPartStack={};
                    _shaderPartStack.push(&module->shaderParts[currentShaderType]);
                    
                }else if(currentShaderType!=2 && SString::containsString(line, "fragment")){
                    
                    currentShaderType=2;
                    _shaderPartStack={};
                    _shaderPartStack.push(&module->shaderParts[currentShaderType]);
                    
                }
                
                continue;
                
            }
            
            // Defines ///
            if (SString::containsString(line, "#")) {
                
                if(SString::containsString(line, "#if") || SString::containsString(line, "#ifdef") || SString::containsString(line, "#ifndef")){
                    
                    _shaderPartStack.top()->subparts.push_back(ShaderModule::ShaderPart(SHADER_PART_DEFINE));
                    _shaderPartStack.top()->subparts.back().name=getDefineFromLine(line);
                    
                    
                    if(SString::containsString(line, "#if")) _shaderPartStack.top()->subparts.back().subtype=SHADER_PART_DEFINE_IF;
                    if(SString::containsString(line, "#ifdef")) _shaderPartStack.top()->subparts.back().subtype=SHADER_PART_DEFINE_IFDEF;
                    if(SString::containsString(line, "#ifndef")){
                        
                        _shaderPartStack.top()->subparts.back().subtype=SHADER_PART_DEFINE_IFNDEF;
                    
                    }

                    _shaderPartStack.push(&_shaderPartStack.top()->subparts.back());
                    continue;
                    
                }else if(SString::containsString(line, "#elseif")){
                    
                    _shaderPartStack.pop();
                    _shaderPartStack.top()->subparts.push_back(ShaderModule::ShaderPart(SHADER_PART_DEFINE));
                    _shaderPartStack.top()->subparts.back().name=getDefineFromLine(line);
                    _shaderPartStack.top()->subparts.back().subtype=SHADER_PART_DEFINE_ELSEIF;
                    
                    _shaderPartStack.push(&_shaderPartStack.top()->subparts.back());
                    continue;
                    
                }else if(SString::containsString(line, "#else")){
                    
                    _shaderPartStack.pop();
                    _shaderPartStack.top()->subparts.push_back(ShaderModule::ShaderPart(SHADER_PART_DEFINE));
                    _shaderPartStack.top()->subparts.back().name=getDefineFromLine(line);
                    _shaderPartStack.top()->subparts.back().subtype=SHADER_PART_DEFINE_ELSE;

                    _shaderPartStack.push(&_shaderPartStack.top()->subparts.back());
                    continue;
                    
                }else if(SString::containsString(line, "#endif")){
                    
                    _shaderPartStack.pop();
                    continue;
                    
                }else if(SString::containsString(line, "#include")){
                    
                    _shaderPartStack.top()->subparts.push_back(ShaderModule::ShaderPart(SHADER_PART_INCLUDE));
                    std::string file2= getFilePathFromLine(line);
                    parseFileIntoShaderPart(file2, &_shaderPartStack.top()->subparts.back(), module);
                    
                    continue;
                    
                }
                
                
            }
            
            if(line.find("area ")==0){
                
                _shaderPartStack.top()->subparts.push_back(ShaderModule::ShaderPart(SHADER_PART_AREA));
                
                if(line.find(" override")!=std::string::npos){
                
                    _shaderPartStack.top()->subparts.back().shouldOverride=true;
                    
                }
                
                std::string text=line;
                SString::replaceAll(text, " override", "");
                SString::replaceAll(text, "{", "");
                SString::replaceAll(text, "area", "");
                SString::replaceAll(text, " ", "");

                _shaderPartStack.top()->subparts.back().name=text;

                
                _shaderPartStack.push(&_shaderPartStack.top()->subparts.back());
                
                
                _openAreasBracketCount.push(openBracketCount);
                continue;
                
            }
            
            openBracketCount+=std::count(line.begin(), line.end(), '{');
            int closeBrackets=(int)std::count(line.begin(), line.end(), '}');
            openBracketCount-=closeBrackets;
            
            for (int b=0; b<closeBrackets && _openAreasBracketCount.size()>0; b++) {
                
                if(_openAreasBracketCount.top()>openBracketCount){
                    
                    _shaderPartStack.pop();
                    _openAreasBracketCount.pop();
                    SString::replaceFirst(line, "}", "");
                    
                }
                
            }
            
            
            if(_shaderPartStack.top()->subparts.size()==0){
                
                _shaderPartStack.top()->subparts.push_back(ShaderModule::ShaderPart(SHADER_PART_CONTENT));
                _shaderPartStack.top()->subparts.back().content.append(line+"\n");
                
            }else if( _shaderPartStack.top()->subparts.back().type==SHADER_PART_CONTENT){
                
                _shaderPartStack.top()->subparts.back().content.append(line+"\n");
                
            }else{
                
                _shaderPartStack.top()->subparts.push_back(ShaderModule::ShaderPart(SHADER_PART_CONTENT));
                _shaderPartStack.top()->subparts.back().content.append(line+"\n");
                
            }
            
            
            
        }

    
    
    }
    
    void ShaderParser::filterShaderPart(ShaderModule::ShaderPart* part, const std::vector<std::string>& defines){
    
        std::vector<ShaderModule::ItHelper> shaderPartStack;
        shaderPartStack.push_back(ShaderModule::ItHelper(part));
        
        while (shaderPartStack.size()>0) {
            
            if(shaderPartStack.back().iterator==0){
                
                if(shaderPartStack.back().shaderPart->type==SHADER_PART_DEFINE){
                    
                    // Reset lastIfResult
                    if(shaderPartStack.back().shaderPart->subtype!=SHADER_PART_DEFINE_ELSE &&
                       shaderPartStack.back().shaderPart->subtype!=SHADER_PART_DEFINE_ELSEIF){
                        
                        if(shaderPartStack.size()>1){
                            
                            shaderPartStack[shaderPartStack.size()-2].lastIfResult=0;
                            
                        }
                        
                    }
                    
                    // Get last if result
                    bool lastResult=shaderPartStack[shaderPartStack.size()-2].lastIfResult;
                    
                    bool insert=false;
                    for (int i=0; i<defines.size(); i++) {
                        
                        std::string name=shaderPartStack.back().shaderPart->name;
                        
                        if(name==defines[i]){
                            
                            insert=true;
                        }
                        
                    }
                    
                    if(shaderPartStack.back().shaderPart->subtype==SHADER_PART_DEFINE_ELSEIF){
                        
                        if(insert && lastResult==0){
                            
                            
                        }else{
                            
                            insert=false;
                            
                        }
                        
                        
                    }else if(shaderPartStack.back().shaderPart->subtype==SHADER_PART_DEFINE_ELSE){
                        
                        if(lastResult==0) insert=true;
                        else insert=false;
                        
                    }else if(shaderPartStack.back().shaderPart->subtype==SHADER_PART_DEFINE_IFNDEF){
                        
                        insert=!insert;
                        
                    }
                    
                    
                    if(!insert){
                        
                        shaderPartStack.back().shaderPart->subparts.clear();
                        shaderPartStack.pop_back();
                        
                    }else{
                        
                        if(shaderPartStack.size()>1){
                            
                            shaderPartStack[shaderPartStack.size()-2].lastIfResult=1;
                            
                        }
                        
                    }
                    
                }else{
                    
                    if(shaderPartStack.size()>1){
                        
                        shaderPartStack[shaderPartStack.size()-2].lastIfResult=0;
                        
                    }
                    
                    
                }
                
            }
            
            if(shaderPartStack.back().iterator>=shaderPartStack.back().shaderPart->subparts.size()){
                
                shaderPartStack.pop_back();
                
            }else{
                
                shaderPartStack.back().iterator++;
                shaderPartStack.push_back(ShaderModule::ItHelper(&shaderPartStack.back().shaderPart->subparts[shaderPartStack.back().iterator-1]));
                
            }
            
        }
        
        
        handleAreasForPart(part);
    
    }
    
    
    void ShaderParser::handleAreasForPart(ShaderModule::ShaderPart *part){
    
        std::vector<ShaderModule::ShaderPart*> areaShaderPart;
        
        std::vector<ShaderModule::ItHelper> shaderPartStack;
        shaderPartStack.push_back(ShaderModule::ItHelper(part));
        
        while (shaderPartStack.size()>0) {
            
            if(shaderPartStack.back().iterator==0){
                
                if(shaderPartStack.back().shaderPart->type==SHADER_PART_AREA){
                    
                    areaShaderPart.push_back(shaderPartStack.back().shaderPart);
                    
                }
                
                
            }
            

            if(shaderPartStack.back().iterator>=shaderPartStack.back().shaderPart->subparts.size()){
                
                shaderPartStack.pop_back();
                
            }else{
                
                shaderPartStack.back().iterator++;
                shaderPartStack.push_back(ShaderModule::ItHelper(&shaderPartStack.back().shaderPart->subparts[shaderPartStack.back().iterator-1]));
                
            }
            
        }
        
        for (int i=0; i<areaShaderPart.size(); i++) {
            
            ShaderModule::ShaderPart* area= areaShaderPart[i];
            
            if(area->shouldOverride){
                
                for (int a=i-1; a>=0; a--) {
                    
                    ShaderModule::ShaderPart* area2= areaShaderPart[a];
                    
                    if(area->name==area2->name && !area2->shouldOverride){
                    
                        area2->subparts.clear();
                        area2->subparts=area->subparts;
                        
                    }

                    
                    
                }
            
                area->type=SHADER_PART_PLACEHOLDER;
                area->subparts.clear();
            
            }
            
            
        }
        
        
    
    }
    
    void ShaderParser::setLocationTarget(SUint target){
    
        _locationTarget=target;
    
    }
    
    std::string ShaderParser::getSourceFromFile(const std::string& file){
    
        std::string fullPath;
        
        if(_locationTarget==LOCATION_TARGET_APP_BUNDLE){
        
            fullPath= Director::getInstance()->getFileUtils()->getRessourcePath(file);
        
        }else{
        

            std::string package= Director::getInstance()->getFileUtils()->getFirstFolderForPath(file);
            SString::replaceAll(package, "/", "");
            std::string ressourcePath= Director::getInstance()->resourceLocations[package];
            
            fullPath= ressourcePath+"/"+file;
        
        }
        

        std::string source;
        std::ifstream infile(fullPath);
        
        if(infile.is_open()){
            
            std::string line;
            while (std::getline(infile, line)){
                
                removeCommentsFromLine(line);
                clearWhiteSpace(line);
                
                if(SString::containsString(line, "#insert")){
                
                    std::string source= getFilePathFromLine(line);
                    std::string tempLine=line;
                
                    line=getSourceFromFile(source);
                    
                    if(SString::containsString(tempLine, "replace")){
                        
                        std::vector<std::pair<std::string, std::string>> array= getReplacmentsFromLine(tempLine);
                        
                        for (int i=0; i<array.size(); i++) {
                            
                            replaceVariables(line, array[i].first, array[i].second);

                        }
                       
                    }
                    
                }
                
                if(line.length()!=0 && line!="\n") source.append(line+"\n");
            }
            
        }else{
            
            std::cout << "Cannot open file:" << file << std::endl;
            assert(0);
            
        }
        
        clearWhiteSpace(source);
        
        return source;
    
    }
    
 

    void ShaderParser::parseShaderIntoModule(const std::string& file, ShaderModule* module){
    
        // Reset shadermodule
        module->shaderParts[0]= ShaderModule::ShaderPart();
        module->shaderParts[1]= ShaderModule::ShaderPart();
        module->shaderParts[2]= ShaderModule::ShaderPart();

        parseFileIntoShaderPart(file, NULL, module);
        
    }
    
    void ShaderParser::clearWhiteSpace(std::string& string){
        
        SString::replaceAll(string, "\r", "\n");
        SString::replaceAll(string, "\t", "");
        SString::replaceAll(string, "\n\n", "\n");
        SString::replaceAll(string, "\n\n", "\n");
        SString::replaceAll(string, "\n{", "{");
        SString::replaceAll(string, "  ", " ");
        SString::replaceAll(string, "  ", " ");
        SString::replaceAll(string, "\n\n", "\n");
        SString::replaceAll(string, "\n\n", "\n");
        SString::replaceAll(string, "\n{", "{");
        SString::replaceAll(string, "  ", " ");
        SString::replaceAll(string, ") {", "){");
        SString::replaceAll(string, "if (", "if(");
        SString::replaceAll(string, "while (", "while(");
        SString::replaceAll(string, "for (", "for(");
        SString::replaceAll(string, "do (", "do(");
        SString::replaceAll(string, "# ", "#");
        SString::replaceAll(string, ";\n", ";");
        SString::replaceAll(string, ";", ";\n");

    }
    
    void ShaderParser::removeCommentsFromLine(std::string& string){
        
        if(string.length()==0) return;
        
        // Remove Comments
        for(int i=0; i<string.length()-1; i++){
            
            if(string[i]=='/' && string[i+1]=='/'){
                
                string.erase(i, string.length());
                break;
            }
            
        }
        
    }
    
    std::string ShaderParser::getDefineFromLine(const std::string& text){
        
        std::string s=text;
        
        SString::replaceAll(s, "#ifndef", "");
        SString::replaceAll(s, "#ifdef", "");
        SString::replaceAll(s, "#elseif", "");
        SString::replaceAll(s, "#if", "");
        SString::replaceAll(s, "#else", "");
        
        SString::replaceAll(s, " ", "");
        
        return s;
        
    }
    
    std::string ShaderParser::getFilePathFromLine(const std::string& text){
        
        std::string source=text;
        
        int pos1=(int) source.find("\"");
        
        source.erase(0, pos1+1);
        pos1=(int) source.find("\"");
        
        std::string header= source.substr(0, pos1);
        
        return header;
    }

    

}
