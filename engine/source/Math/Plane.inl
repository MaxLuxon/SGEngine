#include "Plane.h"

namespace sge
{

inline Plane& Plane::operator*=(const Mat4& matrix)
{
    transform(matrix);
    return *this;
}

inline const Plane operator*(const Mat4& matrix, const Plane& plane)
{
    Plane p(plane);
    p.transform(matrix);
    return p;
}

}
