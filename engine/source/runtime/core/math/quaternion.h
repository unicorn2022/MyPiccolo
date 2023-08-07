#pragma once

#include "runtime/core/math/math.h"
#include "runtime/core/meta/reflection/reflection.h"

#include <cassert>

namespace MyPiccolo {
    class Matrix3x3;
    class Vector3;

    REFLECTION_TYPE(Quaternion)
    CLASS(Quaternion, Fields)
    {
        REFLECTABLE_CLASS(Quaternion)

    public:
        float w {1.f}, x {0.f}, y {0.f}, z {0.f};

    public:
        Quaternion() = default;
        Quaternion(float w_, float x_, float y_, float z_) :
            w {w_}, x {x_}, y {y_}, z {z_} {}
        // 旋转矩阵 => 四元数
        explicit Quaternion(const Matrix3x3& rot) { this->fromRotationMatrix(rot); }
        // 绕轴旋转 => 四元数
        Quaternion(const Radian& angle, const Vector3& axis) { this->fromAngleAxis(angle, axis); }
        // 3个正交的轴 => 四元数
        Quaternion(const Vector3& xaxis, const Vector3& yaxis, const Vector3& zaxis) { this->fromAxes(xaxis, yaxis, zaxis); }

        /* 指针相关 */
        float*       ptr() { return &w; }
        const float* ptr() const { return &w; }

        /* 旋转矩阵 <=> 四元数 */
        void fromRotationMatrix(const Matrix3x3& rotation);
        void toRotationMatrix(Matrix3x3 & rotation) const;
        void toRotationMatrix(Matrix4x4 & rotation) const;

        /* 绕轴旋转 <=> 四元数 */
        void              fromAngleAxis(const Radian& angle, const Vector3& axis);
        void              toAngleAxis(Radian & angle, Vector3 & axis) const;
        static Quaternion getQuaternionFromAngleAxis(const Radian& angle, const Vector3& axis);

        /* 方向 <=> 四元数*/
        void              fromDirection(const Vector3& direction, const Vector3& up_direction);
        static Quaternion getQuaternionFromDirection(const Vector3& direction, const Vector3& up_direction);

        /* 3个正交的轴 <=> 四元数 */
        void    fromAxes(const Vector3& x_axis, const Vector3& y_axis, const Vector3& z_axis);
        void    toAxes(Vector3 & x_axis, Vector3 & y_axis, Vector3 & z_axis) const;
        Vector3 xAxis() const;
        Vector3 yAxis() const;
        Vector3 zAxis() const;

        /* 四则运算 */
        Quaternion operator-() const { return Quaternion(-w, -x, -y, -z); }
        Quaternion operator+(const Quaternion& rhs) const {
            return Quaternion(w + rhs.w, x + rhs.x, y + rhs.y, z + rhs.z);
        }
        Quaternion operator-(const Quaternion& rhs) const {
            return Quaternion(w - rhs.w, x - rhs.x, y - rhs.y, z - rhs.z);
        }
        Quaternion mul(const Quaternion& rhs) const { return (*this) * rhs; }
        Quaternion operator*(float scalar) const { return Quaternion(w * scalar, x * scalar, y * scalar, z * scalar); }
        Quaternion operator*(const Quaternion& rhs) const;
        Vector3    operator*(const Vector3& rhs) const;
        Quaternion operator/(float scalar) const {
            assert(scalar != 0.0f);
            return Quaternion(w / scalar, x / scalar, y / scalar, z / scalar);
        }
        friend Quaternion operator*(float scalar, const Quaternion& rhs) {
            return Quaternion(scalar * rhs.w, scalar * rhs.x, scalar * rhs.y, scalar * rhs.z);
        }

        /* 比较运算 */
        bool operator==(const Quaternion& rhs) const {
            return (rhs.x == x) && (rhs.y == y) && (rhs.z == z) && (rhs.w == w);
        }
        bool operator!=(const Quaternion& rhs) const {
            return (rhs.x != x) || (rhs.y != y) || (rhs.z != z) || (rhs.w != w);
        }

        /* 工具函数 */
        // 判断是否有效
        bool isNaN() const { return Math::isNan(x) || Math::isNan(y) || Math::isNan(z) || Math::isNan(w); }

        float getX() const { return x; }
        float getY() const { return y; }
        float getZ() const { return z; }
        float getW() const { return w; }

        // 点乘
        float dot(const Quaternion& rkQ) const { return w * rkQ.w + x * rkQ.x + y * rkQ.y + z * rkQ.z; }
        // 长度
        float length() const { return std::sqrt(w * w + x * x + y * y + z * z); }
        // 归一化
        void normalise(void) {
            float factor = 1.0f / this->length();
            *this        = *this * factor;
        }
        // 求逆
        Quaternion inverse() const {
            float norm = w * w + x * x + y * y + z * z;
            if (norm > 0.0) {
                // 归一化后求共轭
                float inv_norm = 1.0f / norm;
                return Quaternion(w * inv_norm, -x * inv_norm, -y * inv_norm, -z * inv_norm);
            } else {
                return ZERO;
            }
        }
        // 共轭
        Quaternion conjugate() const { return Quaternion(w, -x, -y, -z); }
        /**
         * @brief 计算绕X轴旋转角度
         * @param reproject_axis 
         */
        Radian getRoll(bool reproject_axis = true) const;
        /**
         * @brief 计算绕Y轴旋转角度
         * @param reproject_axis 
         */
        Radian getPitch(bool reproject_axis = true) const;
        /**
         * @brief 计算绕Z轴旋转角度
         * @param reproject_axis 
         */
        Radian getYaw(bool reproject_axis = true) const;
        /**
         * @brief sLerp 插值算法: 通过arccos, 计算两个方向的夹角, 通过θ插值.
         *        sLerp可以保证插值的角速度恒定, 并且扭矩是最小的(除非 shortest_path = false).
         *        但是sLerp是不可交换的, 也就是说 sLerp(0.75f,A,B) != sLerp(0.25f,B,A);
         */
        static Quaternion sLerp(float t, const Quaternion& kp, const Quaternion& kq, bool shortest_path = false);
        /**
         * @brief nLerp 插值算法: q1和q2的线性插值, 然后进行归一化.
         *        nLerp比sLerp快, 且nLerp是可以交换的, 并且也是扭矩最小的(除非 shortest_path = false).
         *        但是nLerp不能保证角速度恒定
         */
        static Quaternion nLerp(float t, const Quaternion& kp, const Quaternion& kq, bool shortest_path = false);
    };

    // 特殊四元数
    static const Quaternion ZERO;
    static const Quaternion IDENTITY;

    static const float k_epsilon;
} // namespace MyPiccolo