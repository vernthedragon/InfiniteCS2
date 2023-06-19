#include "Utl.h"

void Transform::ToMatrix(Mat3x4& matrix) const noexcept {
    matrix.Matrix[0][0] = 1.0 - 2.0 * rot.y * rot.y - 2.0 * rot.z * rot.z;
    matrix.Matrix[1][0] = 2.0 * rot.x * rot.y + 2.0 * rot.w * rot.z;
    matrix.Matrix[2][0] = 2.0 * rot.x * rot.z - 2.0 * rot.w * rot.y;

    matrix.Matrix[0][1] = 2.0f * rot.x * rot.y - 2.0f * rot.w * rot.z;
    matrix.Matrix[1][1] = 1.0f - 2.0f * rot.x * rot.x - 2.0f * rot.z * rot.z;
    matrix.Matrix[2][1] = 2.0f * rot.y * rot.z + 2.0f * rot.w * rot.x;

    matrix.Matrix[0][2] = 2.0f * rot.x * rot.z + 2.0f * rot.w * rot.y;
    matrix.Matrix[1][2] = 2.0f * rot.y * rot.z - 2.0f * rot.w * rot.x;
    matrix.Matrix[2][2] = 1.0f - 2.0f * rot.x * rot.x - 2.0f * rot.y * rot.y;

    matrix.Matrix[0][3] = pos.x;
    matrix.Matrix[1][3] = pos.y;
    matrix.Matrix[2][3] = pos.z;
}