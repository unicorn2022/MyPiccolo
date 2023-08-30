#pragma once

#include "runtime/core/math/math.h"
#include "runtime/core/math/quaternion.h"
#include "runtime/core/meta/reflection/reflection.h"

#include <cassert>

namespace Piccolo {
    // Vector3需要被反射
    REFLECTION_TYPE(Vector3)
    CLASS(Vector3, Fields)
    {
        REFLECTION_BODY(Vector3);

    public:
        float x {0.f}, y {0.f}, z {0.f};

    public:
        /* 构造函数 */
        Vector3() = default;
        Vector3(float x_, float y_, float z_) :
            x {x_}, y {y_}, z {z_} {}
        explicit Vector3(const float coords[3]) :
            x {coords[0]}, y {coords[1]}, z {coords[2]} {}

        /* 取值运算符 */
        float operator[](size_t i) const {
            assert(i < 3);
            return *(&x + i);
        }
        float& operator[](size_t i) {
            assert(i < 3);
            return *(&x + i);
        }

        /* 指针 */
        float*       ptr() { return &x; }
        const float* ptr() const { return &x; }

        /* 比较运算符 */
        bool operator==(const Vector3& rhs) const { return (x == rhs.x && y == rhs.y && z == rhs.z); }
        bool operator!=(const Vector3& rhs) const { return x != rhs.x || y != rhs.y || z != rhs.z; }

        /* 四则运算 */
        const Vector3& operator+() const { return *this; }
        Vector3        operator+(const Vector3& rhs) const { return Vector3(x + rhs.x, y + rhs.y, z + rhs.z); }
        Vector3        operator-() const { return Vector3(-x, -y, -z); }
        Vector3        operator-(const Vector3& rhs) const { return Vector3(x - rhs.x, y - rhs.y, z - rhs.z); }
        Vector3        operator*(float scalar) const { return Vector3(x * scalar, y * scalar, z * scalar); }
        Vector3        operator*(const Vector3& rhs) const { return Vector3(x * rhs.x, y * rhs.y, z * rhs.z); }

        Vector3 operator/(float scalar) const {
            assert(scalar != 0.0);
            return Vector3(x / scalar, y / scalar, z / scalar);
        }
        Vector3 operator/(const Vector3& rhs) const {
            assert((rhs.x != 0 && rhs.y != 0 && rhs.z != 0));
            return Vector3(x / rhs.x, y / rhs.y, z / rhs.z);
        }

        /* 与其他类型的运算 */
        friend Vector3 operator+(const Vector3& lhs, float rhs) { return Vector3(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs); }
        friend Vector3 operator+(float lhs, const Vector3& rhs) { return Vector3(lhs + rhs.x, lhs + rhs.y, lhs + rhs.z); }
        friend Vector3 operator-(const Vector3& lhs, float rhs) { return Vector3(lhs.x - rhs, lhs.y - rhs, lhs.z - rhs); }
        friend Vector3 operator-(float lhs, const Vector3& rhs) { return Vector3(lhs - rhs.x, lhs - rhs.y, lhs - rhs.z); }
        friend Vector3 operator*(float scalar, const Vector3& rhs) { return Vector3(scalar * rhs.x, scalar * rhs.y, scalar * rhs.z); }
        friend Vector3 operator/(float scalar, const Vector3& rhs) {
            assert(rhs.x != 0 && rhs.y != 0 && rhs.z != 0);
            return Vector3(scalar / rhs.x, scalar / rhs.y, scalar / rhs.z);
        }

        /* 需要更新自身的四则运算 */
        Vector3& operator+=(const Vector3& rhs) {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;
            return *this;
        }
        Vector3& operator+=(float scalar) {
            x += scalar;
            y += scalar;
            z += scalar;
            return *this;
        }
        Vector3& operator-=(const Vector3& rhs) {
            x -= rhs.x;
            y -= rhs.y;
            z -= rhs.z;
            return *this;
        }
        Vector3& operator-=(float scalar) {
            x -= scalar;
            y -= scalar;
            z -= scalar;
            return *this;
        }
        Vector3& operator*=(float scalar) {
            x *= scalar;
            y *= scalar;
            z *= scalar;
            return *this;
        }
        Vector3& operator*=(const Vector3& rhs) {
            x *= rhs.x;
            y *= rhs.y;
            z *= rhs.z;
            return *this;
        }
        Vector3& operator/=(float scalar) {
            assert(scalar != 0.0);
            x /= scalar;
            y /= scalar;
            z /= scalar;
            return *this;
        }
        Vector3& operator/=(const Vector3& rhs) {
            assert(rhs.x != 0 && rhs.y != 0 && rhs.z != 0);
            x /= rhs.x;
            y /= rhs.y;
            z /= rhs.z;
            return *this;
        }

        // 长度
        float length() const { return std::hypot(x, y, z); }
        // 长度的平方
        float squaredLength() const { return x * x + y * y + z * z; }

        // 距离
        float distance(const Vector3& rhs) const { return (*this - rhs).length(); }
        // 距离的平方
        float squaredDistance(const Vector3& rhs) const { return (*this - rhs).squaredLength(); }

        // 点乘
        float dotProduct(const Vector3& vec) const { return x * vec.x + y * vec.y + z * vec.z; }

        // 归一化
        void normalise() {
            float length = std::hypot(x, y, z);
            if (length == 0.f) return;

            float inv_lengh = 1.0f / length;
            x *= inv_lengh;
            y *= inv_lengh;
            z *= inv_lengh;
        }

        // 叉乘
        Vector3 crossProduct(const Vector3& rhs) const {
            return Vector3(y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x);
        }

        // 使得向量的每个分量 <= 另一个向量的对应分量
        void makeFloor(const Vector3& cmp) {
            if (cmp.x < x) x = cmp.x;
            if (cmp.y < y) y = cmp.y;
            if (cmp.z < z) z = cmp.z;
        }
        // 使得向量的每个分量 >= 另一个向量的对应分量
        void makeCeil(const Vector3& cmp) {
            if (cmp.x > x) x = cmp.x;
            if (cmp.y > y) y = cmp.y;
            if (cmp.z > z) z = cmp.z;
        }

        // 计算两个向量之间的夹角
        Radian angleBetween(const Vector3& dest) const {
            float len_product = length() * dest.length();

            // Divide by zero check
            if (len_product < 1e-6f) len_product = 1e-6f;

            float f = dotProduct(dest) / len_product;

            f = Math::clamp(f, (float)-1.0, (float)1.0);
            return Math::acos(f);
        }

        // 获取当前向量旋转到 dest 的最短四元数
        // 如果 dest 更接近于当前向量的反方向, 则使用 fallback_axis 作为旋转轴旋转180度
        Quaternion getRotationTo(const Vector3& dest, const Vector3& fallback_axis = Vector3::ZERO) const {
            Quaternion q;

            // 先将两个向量归一化
            Vector3 v0 = *this;
            Vector3 v1 = dest;
            v0.normalise();
            v1.normalise();

            float d = v0.dotProduct(v1);
            // 点积>=1, 说明两个向量同方向, 返回单位四元数
            if (d >= 1.0f) return Quaternion::IDENTITY;

            // 点积<-1, 说明两个向量方向相反
            if (d < (1e-6f - 1.0f)) {
                if (fallback_axis != Vector3::ZERO) {
                    // 关于 fall back axis 旋转180度
                    q.fromAngleAxis(Radian(Math_PI), fallback_axis);
                } else {
                    // 生成一个旋转轴, 与当前向量垂直
                    Vector3 axis = Vector3::UNIT_X.crossProduct(*this);
                    if (axis.isZeroLength()) axis = Vector3::UNIT_Y.crossProduct(*this);
                    axis.normalise();
                    // 关于 fall back axis 旋转180度
                    q.fromAngleAxis(Radian(Math_PI), axis);
                }
            }
            // 点积在(-1, 1)之间
            // c = v0 × v1, d = v0·v1
            // q = [d + sqrt(c·c + d²), c]
            else {
                float s    = Math::sqrt((1 + d) * 2);
                float invs = 1 / s;

                Vector3 c = v0.crossProduct(v1);

                q.x = c.x * invs;
                q.y = c.y * invs;
                q.z = c.z * invs;
                q.w = s * 0.5f;
                q.normalise();
            }
            return q;
        }

        // 判断向量长度是否为零
        bool isZeroLength(void) const {
            float sqlen = (x * x) + (y * y) + (z * z);
            return (sqlen < (1e-06 * 1e-06));
        }

        // 判断向量是否为零向量
        bool isZero() const { return x == 0.f && y == 0.f && z == 0.f; }

        // 生成该向量归一化后的向量
        Vector3 normalisedCopy(void) const {
            Vector3 ret = *this;
            ret.normalise();
            return ret;
        }

        // 计算该向量在给定法线 normal 的平面上的反射向量
        // 假设该向量指向平面外部, 如果不是, 需要取反
        Vector3 reflect(const Vector3& normal) const {
            return Vector3(*this - (2 * this->dotProduct(normal) * normal));
        }

        // 投影到法线为 normal 的平面上
        Vector3 project(const Vector3& normal) const {
            return Vector3(*this - (this->dotProduct(normal) * normal));
        }

        // 深度拷贝
        Vector3 absoluteCopy() const { return Vector3(fabsf(x), fabsf(y), fabsf(z)); }

        // 向量线性插值
        static Vector3 lerp(const Vector3& lhs, const Vector3& rhs, float alpha) {
            return lhs + alpha * (rhs - lhs);
        }

        // 将向量约束在[min, max]之间
        static Vector3 clamp(const Vector3& v, const Vector3& min, const Vector3& max) {
            return Vector3(Math::clamp(v.x, min.x, max.x), Math::clamp(v.y, min.y, max.y), Math::clamp(v.z, min.z, max.z));
        }

        // 获取最大的元素
        static float getMaxElement(const Vector3& v) { return Math::getMaxElement(v.x, v.y, v.z); }

        // 判断该向量是否无效
        bool isNaN() const { return Math::isNan(x) || Math::isNan(y) || Math::isNan(z); }

        // 特殊向量
        static const Vector3 ZERO;
        static const Vector3 UNIT_X;
        static const Vector3 UNIT_Y;
        static const Vector3 UNIT_Z;
        static const Vector3 NEGATIVE_UNIT_X;
        static const Vector3 NEGATIVE_UNIT_Y;
        static const Vector3 NEGATIVE_UNIT_Z;
        static const Vector3 UNIT_SCALE;
    };
} // namespace Piccolo
