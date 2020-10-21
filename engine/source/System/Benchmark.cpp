#include "Benchmark.h"

#include "glInclude.h"
namespace sge{

    void Benchmark::resetCounter(){
    
        _countMap=_countMapTemp;
        
        typedef std::unordered_map<std::string, unsigned int>::iterator it_type;
        for(it_type iterator = _countMapTemp.begin(); iterator != _countMapTemp.end(); iterator++) {
           
            iterator->second = 0;
           
        }
    
    }
    
    void Benchmark::addToCounter(const std::string& counter, unsigned int count){
    
    
        _countMapTemp[counter]+= count;
    
    }
    
    int Benchmark::getCounter(const std::string& counter){
    
        return _countMap[counter];
    
    }
    

    Benchmark* Benchmark::getInstance(){
    
        static Benchmark* _instance= new Benchmark();
        return _instance;
    
    }
    
    Timer* Benchmark::getTimer(int timerID){
    
        return &_timerMap[timerID];
    
    }
    
    void Benchmark::resetTimer(int timerID){

        _timerMap[timerID].reset();
    
    }
    
    void Benchmark::runTimer(int timerID){
    
        glFinish();

        _timerMap[timerID].run();
    
    }
    
    void Benchmark::stopTimer(int timerID){
    
        glFinish();

        _timerMap[timerID].pause();
    }

}
