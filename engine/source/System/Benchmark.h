#ifndef Benchmark_h
#define Benchmark_h

#include "SBase.h"
#include <unordered_map>

#include "Timer.h"

namespace sge{

class Benchmark{

    
private:
    
    std::unordered_map<int, Timer> _timerMap;
    std::unordered_map<std::string, unsigned int> _countMap;
    std::unordered_map<std::string, unsigned int> _countMapTemp;

    
public:
    
    static Benchmark* getInstance();
    
    Timer* getTimer(int timerID);

    void resetTimer(int timerID);
    void runTimer(int timerID);
    void stopTimer(int timerID);
    
    
    void resetCounter();
    void addToCounter(const std::string& counter, unsigned int count);

    int getCounter(const std::string& counter);

};
    
}

#endif /* Benchmark_hpp */
