#include <stdio.h>
#include "GraphicDeviceDefines.h"

namespace sge {
    
    //BlendFuncs
    BlendFunc BlendFunc::ADD= BlendFunc(Blend::ONE, Blend::ONE);
    BlendFunc BlendFunc::NORMAL= BlendFunc(Blend::SRC_ALPHA, Blend::ONE_MINUS_SRC_ALPHA);
    BlendFunc BlendFunc::MULTIPLY= BlendFunc(Blend::DST_COLOR, Blend::ONE_MINUS_SRC_ALPHA);
    BlendFunc BlendFunc::SCREEN= BlendFunc(Blend::ONE, Blend::ONE_MINUS_SRC_COLOR);
    
    BlendFunc::BlendFunc(){
        
        src=Blend::SRC_ALPHA;
        dst=Blend::ONE_MINUS_SRC_ALPHA;
    }
    
    BlendFunc::BlendFunc(int src, int dst){
        
        this->src=src;
        this->dst=dst;
    }

    
    SInt TextureFormat::getByteOfType(SInt type){
    
        switch (type) {
            case NONE:
                return 0;
                break;
            case RGB:
                return 3;
                break;
            case RGBA:
                return 4;
                break;
            case DEPTH:
                return 1;
                break;
            case RED:
                return 1;
                break;
                
                // Not Sure
            case STENCIL:
                return 1;
                break;
            case RGBA16F:
                return 8;
                break;
                
            case RGBA32F:
                return 16;
                break;
               
            case RGB16F:
                return 6;
                break;
                
            case DEPTH16:
                return 2;
                break;
              
            case DEPTH32:
                return 4;
                break;
                
            case DEPTH24_STENCIL8:
                return 4;
                break;
                
                // Not sure
            case DEPTH_STENCIL:
                return 2;
                break;
                
            default:
                break;
        }
    
        return 0;
    }
    
    const SInt TextureWrap::CLAMP;
    const SInt TextureWrap::REPEAT;
    const SInt TextureWrap::REPEAT_MIRRORED;
    const SInt TextureWrap::CLAMP_TO_EDGE;
    
    const SInt TextureType::NORMAL2D;
    const SInt TextureType::ARRAY;
    const SInt TextureType::CUBE_MAP;
    const SInt TextureType::CUBE_MAP_POSITIVE_X;
    const SInt TextureType::CUBE_MAP_NEGATIVE_X;
    const SInt TextureType::CUBE_MAP_POSITIVE_Y;
    const SInt TextureType::CUBE_MAP_NEGATIVE_Y;
    const SInt TextureType::CUBE_MAP_POSITIVE_Z;
    const SInt TextureType::CUBE_MAP_NEGATIVE_Z;
    
    const SInt TextureFormat::RGB;
    const SInt TextureFormat::RGBA;
    const SInt TextureFormat::DEPTH;
    const SInt TextureFormat::RED;
    const SInt TextureFormat::STENCIL;
    const SInt TextureFormat::RGBA16F;
    const SInt TextureFormat::RGB16F;
    const SInt TextureFormat::DEPTH16;
    const SInt TextureFormat::NONE;
    const SInt TextureFormat::DEPTH32;
    const SInt TextureFormat::DEPTH24_STENCIL8;
    const SInt TextureFormat::DEPTH_STENCIL;

    const SInt TextureFilter::LINEAR;
    const SInt TextureFilter::NEAREST;
    const SInt TextureFilter::NEAREST_MIPMAP_NEAREST;
    const SInt TextureFilter::LINEAR_MIPMAP_NEAREST;
    const SInt TextureFilter::NEAREST_MIPMAP_LINEAR;
    const SInt TextureFilter::LINEAR_MIPMAP_LINEAR ;
    
    const SInt DataType::UNSIGNED_BYTE;
    const SInt DataType::BYTE;
    const SInt DataType::UNSIGNED_INT;
    const SInt DataType::UNSIGNED_SHORT;
    const SInt DataType::INT;
    const SInt DataType::SHORT;
    const SInt DataType::FLOAT;
    const SInt DataType::DOUBLE;
    const SInt DataType::UNSIGNED_INT_8_8_8_8_REV;
    const SInt DataType::UNSIGNED_INT_24_8;

    const SInt Blend::ZERO;
    const SInt Blend::ONE;
    const SInt Blend::SRC_COLOR;
    const SInt Blend::ONE_MINUS_SRC_COLOR;
    const SInt Blend::DST_COLOR;
    const SInt Blend::ONE_MINUS_DST_COLOR;
    const SInt Blend::SRC_ALPHA;
    const SInt Blend::ONE_MINUS_SRC_ALPHA;
    const SInt Blend::DST_ALPHA;
    const SInt Blend::ONE_MINUS_DST_ALPHA;
    const SInt Blend::CONSTANT_ALPHA;
    const SInt Blend::ONE_MINUS_CONSTANT_ALPHA;
    const SInt Blend::SRC_ALPHA_SATURATE;
    
    const SInt DepthFunc::NEVER;
    const SInt DepthFunc::LESS;
    const SInt DepthFunc::EQUAL;
    const SInt DepthFunc::LEQUAL;
    const SInt DepthFunc::GREATER;
    const SInt DepthFunc::NOTEQUAL;
    const SInt DepthFunc::GEQUAL;
    const SInt DepthFunc::ALWAYS;
    
    const SInt BufferDrawType::STATIC;
    const SInt BufferDrawType::DYNAMIC;
    const SInt BufferDrawType::STREAM;
    
    const SInt BufferType::ARRAY;
    const SInt BufferType::ELEMENT_ARRAY;
    
    const SInt CullFace::NONE;
    const SInt CullFace::BACK;
    const SInt CullFace::FRONT;
    const SInt CullFace::FRONT_AND_BACK;
    
    const SInt DrawMode::TRIANGLES;
    const SInt DrawMode::LINES;
    const SInt DrawMode::POINTS;
    const SInt DrawMode::TRIANGLE_STRIP;

    const SInt StencilFunc::NEVER;
    const SInt StencilFunc::ALWAYS;
    const SInt StencilFunc::LESS;
    const SInt StencilFunc::LEQUAL;
    const SInt StencilFunc::EQUAL;
    const SInt StencilFunc::GREATER;
    const SInt StencilFunc::GEQUAL;
    const SInt StencilFunc::NOTEQUAL;
    
    const SInt StencilOperation::KEEP;
    const SInt StencilOperation::ZERO;
    const SInt StencilOperation::REPLACE;
    const SInt StencilOperation::INCR;
    const SInt StencilOperation::DECR;
    const SInt StencilOperation::INVERT;
    const SInt StencilOperation::INCR_WRAP;
    const SInt StencilOperation::DECR_WRAP;
    
    const SUint ShaderType::VERTEX;
    const SUint ShaderType::FRAGMENT;
    const SUint ShaderType::GEOMETRY;
    
    const SUint FrustumCulling::NONE;
    const SUint FrustumCulling::BOX;
    const SUint FrustumCulling::SPHERE;
        
    
}

