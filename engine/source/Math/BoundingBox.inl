#include "BoundingBox.h"

namespace sge
{

inline BoundingBox& BoundingBox::operator*=(const Mat4& matrix)
{
    transform(matrix);
    return *this;
}

inline const BoundingBox operator*(const Mat4& matrix, const BoundingBox& box)
{
    BoundingBox b(box);
    b.transform(matrix);
    return b;
}

}
