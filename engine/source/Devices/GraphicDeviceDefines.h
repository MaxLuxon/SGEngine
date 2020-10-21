#ifndef GraphicDeviceDefines_h
#define GraphicDeviceDefines_h

#include "SBase.h"

namespace sge {
    
    class TextureWrap{
    
    public:
        static const SInt CLAMP = 0;
        static const SInt REPEAT = 1;
        static const SInt REPEAT_MIRRORED = 2;
        static const SInt CLAMP_TO_EDGE= 3;
        
    };
    
    class TextureType{
   
        
    public:
        static const SInt NORMAL2D = 0;
        static const SInt ARRAY = 1;
        static const SInt CUBE_MAP = 2;
        static const SInt CUBE_MAP_POSITIVE_X = 3;
        static const SInt CUBE_MAP_NEGATIVE_X= 4;
        static const SInt CUBE_MAP_POSITIVE_Y = 5;
        static const SInt CUBE_MAP_NEGATIVE_Y = 6;
        static const SInt CUBE_MAP_POSITIVE_Z = 7;
        static const SInt CUBE_MAP_NEGATIVE_Z = 8;

    };
    
    class TextureFormat{
        
    public:
        static const SInt NONE = 0;
        static const SInt RGB = 1;
        static const SInt RGBA = 2;
        static const SInt DEPTH = 3;
        static const SInt RED = 4;
        static const SInt STENCIL = 5;
        static const SInt RGBA16F= 6;
        static const SInt RGBA32F= 7;
        static const SInt RGB16F= 8;
        static const SInt DEPTH16= 9;
        static const SInt DEPTH32= 10;
        static const SInt DEPTH24_STENCIL8=11;
        static const SInt DEPTH_STENCIL=12;
        
        static SInt getByteOfType(SInt type);

        
    };
    
    
    class TextureFilter{
        
    public:
        static const SInt LINEAR = 0;
        static const SInt NEAREST = 1;
        static const SInt NEAREST_MIPMAP_NEAREST = 2;
        static const SInt LINEAR_MIPMAP_NEAREST = 3;
        static const SInt NEAREST_MIPMAP_LINEAR = 4;
        static const SInt LINEAR_MIPMAP_LINEAR = 5;
        
    };
    
    class DataType{
        
    public:
        
        static const SInt UNSIGNED_BYTE = 0;
        static const SInt BYTE = 1;
        static const SInt UNSIGNED_INT = 2;
        static const SInt UNSIGNED_SHORT = 3;
        static const SInt INT = 4;
        static const SInt SHORT = 5;
        static const SInt FLOAT = 6;
        static const SInt DOUBLE = 7;
        static const SInt UNSIGNED_INT_8_8_8_8_REV = 8;
        static const SInt UNSIGNED_INT_24_8 = 9;


        
        
    };
    
    // Blend defines
    class Blend{
        
    public:
        
        static const SInt ZERO = 0;
        static const SInt ONE = 1;
        static const SInt SRC_COLOR = 2;
        static const SInt ONE_MINUS_SRC_COLOR = 3;
        static const SInt DST_COLOR = 4;
        static const SInt ONE_MINUS_DST_COLOR = 5;
        static const SInt SRC_ALPHA = 6;
        static const SInt ONE_MINUS_SRC_ALPHA = 7;
        static const SInt DST_ALPHA = 8;
        static const SInt ONE_MINUS_DST_ALPHA = 9;
        static const SInt CONSTANT_ALPHA = 10;
        static const SInt ONE_MINUS_CONSTANT_ALPHA = 11;
        static const SInt SRC_ALPHA_SATURATE = 12;
        
    };
    
    // Depth Defines
    class DepthFunc{
        
    public:
        
        static const SInt NEVER = 0;
        static const SInt LESS = 1;
        static const SInt EQUAL = 2;
        static const SInt LEQUAL = 3;
        static const SInt GREATER = 4;
        static const SInt NOTEQUAL = 5;
        static const SInt GEQUAL = 6;
        static const SInt ALWAYS = 7;
        
    };
    
    class BufferDrawType{
        
    public:
        
        static const SInt STATIC = 0;
        static const SInt DYNAMIC = 1;
        static const SInt STREAM = 2;
   
    };
    
    class BufferType{
        
    public:
        
        static const SInt ARRAY = 0;
        static const SInt ELEMENT_ARRAY = 1;
        
    };
    
    // Cull Face Defines
    class CullFace{
        
    public:
        
        static const SInt NONE = 0;
        static const SInt BACK = 1;
        static const SInt FRONT = 2;
        static const SInt FRONT_AND_BACK = 3;
        
    };
    
    // Cull Face Defines
    class DrawMode{
        
    public:
        
        static const SInt TRIANGLES = 0;
        static const SInt LINES = 1;
        static const SInt POINTS = 2;
        static const SInt TRIANGLE_STRIP = 3;

    };
    
    
    // Stencil Defines
    class StencilFunc{
        
    public:
        
        static const SInt NEVER = 0;
        static const SInt ALWAYS = 1;
        static const SInt LESS = 2;
        static const SInt LEQUAL = 3;
        static const SInt EQUAL = 4;
        static const SInt GREATER = 5;
        static const SInt GEQUAL = 6;
        static const SInt NOTEQUAL = 7;
    };
    
    // Stencil operation Defines
    class StencilOperation{
        
    public:
        
        static const SInt KEEP = 0;
        static const SInt ZERO = 1;
        static const SInt REPLACE = 2;
        static const SInt INCR = 3;
        static const SInt DECR = 4;
        static const SInt INVERT = 5;
        static const SInt INCR_WRAP = 6;
        static const SInt DECR_WRAP = 7;
    };
    
    class ShaderType{
        
    public:
        
        static const SUint VERTEX = 0;
        static const SUint FRAGMENT = 1;
        static const SUint GEOMETRY = 2;
        
    };
    
    class BlendFunc{
        
    public:
       
        int src;
        int dst;
        BlendFunc();
        BlendFunc(int src, int dst);
        
        static BlendFunc ADD;
        static BlendFunc NORMAL;
        static BlendFunc MULTIPLY;
        static BlendFunc SCREEN;

    };
    
    
    class FrustumCulling{
        
    public:
        
        static const SUint NONE = 0;
        static const SUint BOX = 1;
        static const SUint SPHERE = 2;
        
    };
    

    
}

#endif