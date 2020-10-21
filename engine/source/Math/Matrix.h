#ifndef Matrix_h
#define Matrix_h

// derived from the gameplay engine
// https://github.com/gameplay3d/GamePlay

#include "SBase.h"

namespace sge{

    class Plane;
    class Quaternion;
    
class Mat4{

    public:
       
    float m[16];
        
    Mat4();
    Mat4(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24,
         float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44);
    Mat4(const float* m);
    Mat4(const Mat4& copy);
    ~Mat4();
    
    
    float& operator[] (const int index);

    
    std::vector<float> getMat3Array();
    
    // Statics
    static const Mat4& identity();
    static const Mat4& zero();
    static void createLookAt(const Vec3& eyePosition, const Vec3& targetPosition, const Vec3& up, Mat4* dst);
        
    static void createPerspective(float fieldOfView, float aspectRatio, float zNearPlane, float zFarPlane, Mat4* dst);
    static void createOrthographic(float width, float height, float zNearPlane, float zFarPlane, Mat4* dst);
    static void createOrthographicOffCenter(float left, float right, float bottom, float top,
                                            float zNearPlane, float zFarPlane, Mat4* dst);
            
    
    static void createReflection(const Plane& plane, Mat4* dst);
    static void createScale(const Vec3& scale, Mat4* dst);
    static void createRotation(const Quaternion& quat, Mat4* dst);
    static void createRotation(const Vec3& axis, float angle, Mat4* dst);
    static void createRotationX(float angle, Mat4* dst);
    static void createRotationY(float angle, Mat4* dst);
    static void createRotationZ(float angle, Mat4* dst);
    static void createFromEuler(float yaw, float pitch, float roll, Mat4* dst);
    static void createTranslation(const Vec3& translation, Mat4* dst);
        
    static void multiply(const Mat4& m1, const Mat4& m2, Mat4* dst);
    static void multiply(const Mat4& m, float scalar, Mat4* dst);
        
    static void subtract(const Mat4& m1, const Mat4& m2, Mat4* dst);
    static void add(const Mat4& m1, const Mat4& m2, Mat4* dst);

    // Addition
    void add(float scalar);
    void add(float scalar, Mat4* dst);
    void add(const Mat4& m);
    
    void multiply(float scalar);
    void multiply(float scalar, Mat4* dst) const;
    void multiply(const Mat4& m);
    
    void subtract(const Mat4& m);

    bool inverse();
    bool inverse(Mat4* dst) const;
    
    void transpose();
    void transpose(Mat4* dst) const;
    
    void negate();
    void negate(Mat4* dst) const;

    bool isIdentity() const;
    
    bool decompose(Vec3* scale, Quaternion* rotation, Vec3* translation) const;
    float determinant() const;
    
    
    // Setter
    void set(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24,
             float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44);
    
    void set(const float* m);
    void set(const Mat4& m);
    
    void setIdentity();
    void setZero();

    
    // Getter
    void getScale(Vec3* scale) const;
    bool getRotation(Quaternion* rotation) const;
    void getTranslation(Vec3* translation) const;

    void getUpVector(Vec3* dst) const;
    void getDownVector(Vec3* dst) const;
    void getLeftVector(Vec3* dst) const;
    void getRightVector(Vec3* dst) const;
    void getForwardVector(Vec3* dst) const;
    void getBackVector(Vec3* dst) const;
            
    // Rotation
    void rotate(const Quaternion& q);
    void rotate(const Quaternion& q, Mat4* dst) const;
    void rotate(const Vec3& axis, float angle);
    void rotate(const Vec3& axis, float angle, Mat4* dst) const;
    void rotateX(float angle);
    void rotateX(float angle, Mat4* dst) const;
    void rotateY(float angle);
    void rotateY(float angle, Mat4* dst) const;
    void rotateZ(float angle);
    void rotateZ(float angle, Mat4* dst) const;
    
    // Scale
    void scale(float value);
    void scale(float value, Mat4* dst) const;
    void scale(float xScale, float yScale, float zScale);
    void scale(float xScale, float yScale, float zScale, Mat4* dst) const;
    void scale(const Vec3& s);
    void scale(const Vec3& s, Mat4* dst) const;
    
    // Translate
    void translate(float x, float y, float z);
    void translate(float x, float y, float z, Mat4* dst) const;
    void translate(const Vec3& t);
    void translate(const Vec3& t, Mat4* dst) const;
    
    // Tranform
    void transformPoint(Vec3* point) const;
    void transformPoint(const Vec3& point, Vec3* dst) const;
    void transformVector(Vec3* vector) const;
    void transformVector(const Vec3& vector, Vec3* dst) const;
    
    void transformVector(float x, float y, float z, float w, Vec3* dst) const;
    void transformVector(Vec4* vector) const;
    void transformVector(const Vec4& vector, Vec4* dst) const;

    // Inlines
    inline const Mat4 operator+(const Mat4& m) const;
    inline Mat4& operator+=(const Mat4& m);
    inline const Mat4 operator-(const Mat4& m) const;
    inline Mat4& operator-=(const Mat4& m);
    inline const Mat4 operator-() const;
    inline const Mat4 operator*(const Mat4& m) const;
    inline Mat4& operator*=(const Mat4& m);
            
};
    
    
    inline Vec3& operator*=(Vec3& v, const Mat4& m);
    inline const Vec3 operator*(const Mat4& m, const Vec3& v);
    inline Vec4& operator*=(Vec4& v, const Mat4& m);
    inline const Vec4 operator*(const Mat4& m, const Vec4& v);
    
    inline const Mat4 Mat4::operator+(const Mat4& m) const
    {
        Mat4 result(*this);
        result.add(m);
        return result;
    }
    
    inline Mat4& Mat4::operator+=(const Mat4& m)
    {
        add(m);
        return *this;
    }
    
    inline const Mat4 Mat4::operator-(const Mat4& m) const
    {
        Mat4 result(*this);
        result.subtract(m);
        return result;
    }
    
    inline Mat4& Mat4::operator-=(const Mat4& m)
    {
        subtract(m);
        return *this;
    }
    
    inline const Mat4 Mat4::operator-() const
    {
        Mat4 m(*this);
        m.negate();
        return m;
    }
    
    inline const Mat4 Mat4::operator*(const Mat4& m) const
    {
        Mat4 result(*this);
        result.multiply(m);
        return result;
    }
    
    inline Mat4& Mat4::operator*=(const Mat4& m)
    {
        multiply(m);
        return *this;
    }
    
    inline Vec3& operator*=(Vec3& v, const Mat4& m)
    {
        m.transformVector(&v);
        return v;
    }
    
    inline const Vec3 operator*(const Mat4& m, const Vec3& v)
    {
        Vec3 x;
        m.transformVector(v, &x);
        return x;
    }
    
    inline Vec4& operator*=(Vec4& v, const Mat4& m)
    {
        m.transformVector(&v);
        return v;
    }
    
    inline const Vec4 operator*(const Mat4& m, const Vec4& v)
    {
        Vec4 x;
        m.transformVector(v, &x);
        return x;
    }
}

#endif 
