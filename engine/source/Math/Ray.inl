#include "Ray.h"
#include "SMath.h"

namespace sge
{

inline Ray& Ray::operator*=(const Mat4& matrix)
{
    transform(matrix);
    return *this;
}

    inline const Ray operator*(const Mat4& matrix, const Ray& ray)
{
    Ray r(ray);
    r.transform(matrix);
    return r;
}

}
