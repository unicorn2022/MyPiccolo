#pragma once

#include "runtime/core/math/math.h"
#include "runtime/core/math/quaternion.h"
#include "runtime/core/math/vector3.h"

#include <cstring>

// NOTE.  使用右手坐标系
// Coordinate axis rotation matrices are of the form
//   RX =    1       0       0
//           0     cos(t) -sin(t)
//           0     sin(t)  cos(t)
// where t > 0 indicates a counterclockwise rotation in the yz-plane
//   RY =  cos(t)    0     sin(t)
//           0       1       0
//        -sin(t)    0     cos(t)
// where t > 0 indicates a counterclockwise rotation in the zx-plane
//   RZ =  cos(t) -sin(t)    0
//         sin(t)  cos(t)    0
//           0       0       1
// where t > 0 indicates a counterclockwise rotation in the xy-plane.

namespace MyPiccolo {
    // 一个3×3的矩阵, 可以表示关于轴的旋转, 使用右手坐标系
    class Matrix3x3 {
    public:
        float m_mat[3][3];

    public:
        // 默认构造函数, 不初始化矩阵
        Matrix3x3() { operator=(IDENTITY); }
        /* 构造函数 */
        explicit Matrix3x3(float arr[3][3]) {
            memcpy(m_mat[0], arr[0], 3 * sizeof(float));
            memcpy(m_mat[1], arr[1], 3 * sizeof(float));
            memcpy(m_mat[2], arr[2], 3 * sizeof(float));
        }
        Matrix3x3(float (&float_array)[9]) {
            m_mat[0][0] = float_array[0];
            m_mat[0][1] = float_array[1];
            m_mat[0][2] = float_array[2];
            m_mat[1][0] = float_array[3];
            m_mat[1][1] = float_array[4];
            m_mat[1][2] = float_array[5];
            m_mat[2][0] = float_array[6];
            m_mat[2][1] = float_array[7];
            m_mat[2][2] = float_array[8];
        }
        Matrix3x3(float entry00, float entry01, float entry02, float entry10, float entry11, float entry12, float entry20, float entry21, float entry22) {
            m_mat[0][0] = entry00;
            m_mat[0][1] = entry01;
            m_mat[0][2] = entry02;
            m_mat[1][0] = entry10;
            m_mat[1][1] = entry11;
            m_mat[1][2] = entry12;
            m_mat[2][0] = entry20;
            m_mat[2][1] = entry21;
            m_mat[2][2] = entry22;
        }
        Matrix3x3(const Vector3& row0, const Vector3& row1, const Vector3& row2) {
            m_mat[0][0] = row0.x;
            m_mat[0][1] = row0.y;
            m_mat[0][2] = row0.z;
            m_mat[1][0] = row1.x;
            m_mat[1][1] = row1.y;
            m_mat[1][2] = row1.z;
            m_mat[2][0] = row2.x;
            m_mat[2][1] = row2.y;
            m_mat[2][2] = row2.z;
        }
        Matrix3x3(const Quaternion& q) {
            float yy = q.y * q.y;
            float zz = q.z * q.z;
            float xy = q.x * q.y;
            float zw = q.z * q.w;
            float xz = q.x * q.z;
            float yw = q.y * q.w;
            float xx = q.x * q.x;
            float yz = q.y * q.z;
            float xw = q.x * q.w;

            m_mat[0][0] = 1 - 2 * yy - 2 * zz;
            m_mat[0][1] = 2 * xy + 2 * zw;
            m_mat[0][2] = 2 * xz - 2 * yw;

            m_mat[1][0] = 2 * xy - 2 * zw;
            m_mat[1][1] = 1 - 2 * xx - 2 * zz;
            m_mat[1][2] = 2 * yz + 2 * xw;

            m_mat[2][0] = 2 * xz + 2 * yw;
            m_mat[2][1] = 2 * yz - 2 * xw;
            m_mat[2][2] = 1 - 2 * xx - 2 * yy;
        }

        /* 与 float 数组的转换 */
        void fromData(float (&float_array)[9]) {
            m_mat[0][0] = float_array[0];
            m_mat[0][1] = float_array[1];
            m_mat[0][2] = float_array[2];
            m_mat[1][0] = float_array[3];
            m_mat[1][1] = float_array[4];
            m_mat[1][2] = float_array[5];
            m_mat[2][0] = float_array[6];
            m_mat[2][1] = float_array[7];
            m_mat[2][2] = float_array[8];
        }
        void toData(float (&float_array)[9]) const {
            float_array[0] = m_mat[0][0];
            float_array[1] = m_mat[0][1];
            float_array[2] = m_mat[0][2];
            float_array[3] = m_mat[1][0];
            float_array[4] = m_mat[1][1];
            float_array[5] = m_mat[1][2];
            float_array[6] = m_mat[2][0];
            float_array[7] = m_mat[2][1];
            float_array[8] = m_mat[2][2];
        }

        /* 取值运算符 */
        float* operator[](size_t row_index) const { return (float*)m_mat[row_index]; }
        // 取一列元素
        Vector3 getColumn(size_t col_index) const {
            assert(0 <= col_index && col_index < 3);
            return Vector3(m_mat[0][col_index], m_mat[1][col_index], m_mat[2][col_index]);
        }
        // 设置一列元素
        void setColumn(size_t iCol, const Vector3& vec);
        // 从坐标轴设置矩阵, matrix = [x_axis, y_axis, z_axis]
        void fromAxes(const Vector3& x_axis, const Vector3& y_axis, const Vector3& z_axis);

        /* 比较运算符 */
        bool operator==(const Matrix3x3& rhs) const {
            for (size_t row_index = 0; row_index < 3; row_index++) {
                for (size_t col_index = 0; col_index < 3; col_index++) {
                    if (m_mat[row_index][col_index] != rhs.m_mat[row_index][col_index])
                        return false;
                }
            }
            return true;
        }
        bool operator!=(const Matrix3x3& rhs) const { return !operator==(rhs); }

        /* 四则运算 */
        Matrix3x3 operator+(const Matrix3x3& rhs) const {
            Matrix3x3 sum;
            for (size_t row_index = 0; row_index < 3; row_index++) {
                for (size_t col_index = 0; col_index < 3; col_index++) {
                    sum.m_mat[row_index][col_index] = m_mat[row_index][col_index] + rhs.m_mat[row_index][col_index];
                }
            }
            return sum;
        }
        Matrix3x3 operator-(const Matrix3x3& rhs) const {
            Matrix3x3 diff;
            for (size_t row_index = 0; row_index < 3; row_index++) {
                for (size_t col_index = 0; col_index < 3; col_index++) {
                    diff.m_mat[row_index][col_index] = m_mat[row_index][col_index] - rhs.m_mat[row_index][col_index];
                }
            }
            return diff;
        }
        Matrix3x3 operator*(const Matrix3x3& rhs) const {
            Matrix3x3 prod;
            for (size_t row_index = 0; row_index < 3; row_index++) {
                for (size_t col_index = 0; col_index < 3; col_index++) {
                    prod.m_mat[row_index][col_index] = m_mat[row_index][0] * rhs.m_mat[0][col_index] +
                                                       m_mat[row_index][1] * rhs.m_mat[1][col_index] +
                                                       m_mat[row_index][2] * rhs.m_mat[2][col_index];
                }
            }
            return prod;
        }
        // 矩阵 * 向量 [3x3 * 3x1 = 3x1]
        Vector3 operator*(const Vector3& rhs) const {
            Vector3 prod;
            for (size_t row_index = 0; row_index < 3; row_index++) {
                prod[row_index] =
                    m_mat[row_index][0] * rhs.x + m_mat[row_index][1] * rhs.y + m_mat[row_index][2] * rhs.z;
            }
            return prod;
        }
        // 向量 * 矩阵 [1x3 * 3x3 = 1x3]
        friend Vector3 operator*(const Vector3& point, const Matrix3x3& rhs) {
            Vector3 prod;
            for (size_t row_index = 0; row_index < 3; row_index++) {
                prod[row_index] = point.x * rhs.m_mat[0][row_index] + point.y * rhs.m_mat[1][row_index] +
                                  point.z * rhs.m_mat[2][row_index];
            }
            return prod;
        }
        Matrix3x3 operator-() const {
            Matrix3x3 neg;
            for (size_t row_index = 0; row_index < 3; row_index++) {
                for (size_t col_index = 0; col_index < 3; col_index++)
                    neg[row_index][col_index] = -m_mat[row_index][col_index];
            }
            return neg;
        }
        // 矩阵 * 标量
        Matrix3x3 operator*(float scalar) const {
            Matrix3x3 prod;
            for (size_t row_index = 0; row_index < 3; row_index++) {
                for (size_t col_index = 0; col_index < 3; col_index++)
                    prod[row_index][col_index] = scalar * m_mat[row_index][col_index];
            }
            return prod;
        }
        // 标量 * 矩阵
        friend Matrix3x3 operator*(float scalar, const Matrix3x3& rhs) {
            Matrix3x3 prod;
            for (size_t row_index = 0; row_index < 3; row_index++) {
                for (size_t col_index = 0; col_index < 3; col_index++)
                    prod[row_index][col_index] = scalar * rhs.m_mat[row_index][col_index];
            }
            return prod;
        }

        /* 工具函数 */
        // 矩阵转置
        Matrix3x3 transpose() const {
            Matrix3x3 transpose_v;
            for (size_t row_index = 0; row_index < 3; row_index++) {
                for (size_t col_index = 0; col_index < 3; col_index++)
                    transpose_v[row_index][col_index] = m_mat[col_index][row_index];
            }
            return transpose_v;
        }
        // 矩阵求逆
        bool inverse(Matrix3x3& inv_mat, float fTolerance = 1e-06) const {
            // 使用代数余子式求逆矩阵，比高斯消元法快8倍
            float det = determinant();
            if (std::fabs(det) <= fTolerance) return false;

            inv_mat[0][0] = m_mat[1][1] * m_mat[2][2] - m_mat[1][2] * m_mat[2][1];
            inv_mat[0][1] = m_mat[0][2] * m_mat[2][1] - m_mat[0][1] * m_mat[2][2];
            inv_mat[0][2] = m_mat[0][1] * m_mat[1][2] - m_mat[0][2] * m_mat[1][1];
            inv_mat[1][0] = m_mat[1][2] * m_mat[2][0] - m_mat[1][0] * m_mat[2][2];
            inv_mat[1][1] = m_mat[0][0] * m_mat[2][2] - m_mat[0][2] * m_mat[2][0];
            inv_mat[1][2] = m_mat[0][2] * m_mat[1][0] - m_mat[0][0] * m_mat[1][2];
            inv_mat[2][0] = m_mat[1][0] * m_mat[2][1] - m_mat[1][1] * m_mat[2][0];
            inv_mat[2][1] = m_mat[0][1] * m_mat[2][0] - m_mat[0][0] * m_mat[2][1];
            inv_mat[2][2] = m_mat[0][0] * m_mat[1][1] - m_mat[0][1] * m_mat[1][0];

            float inv_det = 1.0f / det;
            for (size_t row_index = 0; row_index < 3; row_index++) {
                for (size_t col_index = 0; col_index < 3; col_index++)
                    inv_mat[row_index][col_index] *= inv_det;
            }

            return true;
        }
        // 矩阵求逆
        Matrix3x3 inverse(float tolerance = 1e-06) const {
            Matrix3x3 inv = ZERO;
            inverse(inv, tolerance);
            return inv;
        }
        // 矩阵行列式
        float determinant() const {
            float cofactor00 = m_mat[1][1] * m_mat[2][2] - m_mat[1][2] * m_mat[2][1];
            float cofactor10 = m_mat[1][2] * m_mat[2][0] - m_mat[1][0] * m_mat[2][2];
            float cofactor20 = m_mat[1][0] * m_mat[2][1] - m_mat[1][1] * m_mat[2][0];

            float det = m_mat[0][0] * cofactor00 + m_mat[0][1] * cofactor10 + m_mat[0][2] * cofactor20;

            return det;
        }

        // QDU分解： M = QR = QDU, Q为旋转矩阵, D为缩放矩阵, U为切变矩阵
        void calculateQDUDecomposition(Matrix3x3& out_Q, Vector3& out_D, Vector3& out_U) const;

        // 旋转矩阵 => 关于某个轴axis旋转radian弧度, 矩阵必须是正交的
        void toAngleAxis(Vector3& axis, Radian& angle) const;
        // 旋转矩阵 => 关于某个轴axis旋转angle角度, 矩阵必须是正交的
        void toAngleAxis(Vector3& axis, Degree& angle) const {
            Radian r;
            toAngleAxis(axis, r);
            angle = r;
        }
        // 关于某个轴axis旋转radian弧度 => 旋转矩阵
        void fromAngleAxis(const Vector3& axis, const Radian& radian);

        // 缩放矩阵
        static Matrix3x3 scale(const Vector3& scale) {
            Matrix3x3 mat = ZERO;

            mat.m_mat[0][0] = scale.x;
            mat.m_mat[1][1] = scale.y;
            mat.m_mat[2][2] = scale.z;

            return mat;
        }

        // 特殊矩阵
        static const Matrix3x3 ZERO;
        static const Matrix3x3 IDENTITY;
    };
} // namespace MyPiccolo
