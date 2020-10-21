#include "BoundingSphere.h"

namespace sge
{

inline BoundingSphere& BoundingSphere::operator*=(const Mat4& matrix)
{
    transform(matrix);
    return *this;
}

inline const BoundingSphere operator*(const Mat4& matrix, const BoundingSphere& sphere)
{
    BoundingSphere s(sphere);
    s.transform(matrix);
    return s;
}

}
