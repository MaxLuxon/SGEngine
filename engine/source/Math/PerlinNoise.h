#ifndef EXOGROUNDS_PERLINNOISE_H
#define EXOGROUNDS_PERLINNOISE_H

#include <stdio.h>
#include <vector>

namespace sge {
    
    class PerlinNoise{
        
    protected:
        
        std::vector<int> p;
    
    public:

        PerlinNoise();
        PerlinNoise(unsigned int seed);
        double noise(double x, double y, double z);
        
    private:
        
        double fade(double t);
        double lerp(double t, double a, double b);
        double grad(int hash, double x, double y, double z);
        
    };
    
} // Namespace End

#endif
