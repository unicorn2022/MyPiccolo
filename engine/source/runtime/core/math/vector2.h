#pragma once

#include "runtime/core/math/math.h"
#include "runtime/core/meta/reflection/reflection.h"

#include <cassert>
#include <cmath>

namespace Piccolo {
    // Vector2需要被反射
    REFLECTION_TYPE(Vector2)
    CLASS(Vector2, Fields)
    {
        REFLECTION_BODY(Vector2);

    public:
        float x {0.f}, y {0.f};

    public:
        /* 构造函数 */
        Vector2() = default;
        Vector2(float x_, float y_) :
            x(x_), y(y_) {}
        explicit Vector2(float scaler) :
            x(scaler), y(scaler) {}
        explicit Vector2(const float v[2]) :
            x(v[0]), y(v[1]) {}
        explicit Vector2(float* const r) :
            x(r[0]), y(r[1]) {}

        /* 指针 */
        float*       ptr() { return &x; }
        const float* ptr() const { return &x; }

        /* 取值运算符 */
        float operator[](size_t i) const {
            assert(i < 2);
            return (i == 0 ? x : y);
        }
        float& operator[](size_t i) {
            assert(i < 2);
            return (i == 0 ? x : y);
        }

        /* 比较运算符 */
        bool operator==(const Vector2& rhs) const { return (x == rhs.x && y == rhs.y); }
        bool operator!=(const Vector2& rhs) const { return (x != rhs.x || y != rhs.y); }

        /* 四则运算 */
        const Vector2& operator+() const { return *this; }
        Vector2        operator+(const Vector2& rhs) const { return Vector2(x + rhs.x, y + rhs.y); }
        Vector2        operator-() const { return Vector2(-x, -y); }
        Vector2        operator-(const Vector2& rhs) const { return Vector2(x - rhs.x, y - rhs.y); }
        Vector2        operator*(float scalar) const { return Vector2(x * scalar, y * scalar); }
        Vector2        operator*(const Vector2& rhs) const { return Vector2(x * rhs.x, y * rhs.y); }

        Vector2 operator/(const Vector2& rhs) const {
            assert((rhs.x != 0 && rhs.y != 0));
            return Vector2(x / rhs.x, y / rhs.y);
        }
        Vector2 operator/(float scale) const {
            assert(scale != 0.0);
            float inv = 1.0f / scale;
            return Vector2(x * inv, y * inv);
        }

        /* 与其他类型的运算 */
        friend Vector2 operator+(const Vector2& lhs, float rhs) { return Vector2(lhs.x + rhs, lhs.y + rhs); }
        friend Vector2 operator+(float lhs, const Vector2& rhs) { return Vector2(lhs + rhs.x, lhs + rhs.y); }
        friend Vector2 operator-(const Vector2& lhs, float rhs) { return Vector2(lhs.x - rhs, lhs.y - rhs); }
        friend Vector2 operator-(float lhs, const Vector2& rhs) { return Vector2(lhs - rhs.x, lhs - rhs.y); }
        friend Vector2 operator*(float scalar, const Vector2& rhs) { return Vector2(scalar * rhs.x, scalar * rhs.y); }
        friend Vector2 operator/(float fScalar, const Vector2& rhs) {
            assert(rhs.x != 0 && rhs.y != 0);
            return Vector2(fScalar / rhs.x, fScalar / rhs.y);
        }

        /* 需要更新自身的四则运算 */
        Vector2& operator+=(const Vector2& rhs) {
            x += rhs.x;
            y += rhs.y;
            return *this;
        }
        Vector2& operator+=(float scalar) {
            x += scalar;
            y += scalar;
            return *this;
        }
        Vector2& operator-=(const Vector2& rhs) {
            x -= rhs.x;
            y -= rhs.y;
            return *this;
        }
        Vector2& operator-=(float scalar) {
            x -= scalar;
            y -= scalar;
            return *this;
        }
        Vector2& operator*=(float scalar) {
            x *= scalar;
            y *= scalar;
            return *this;
        }
        Vector2& operator*=(const Vector2& rhs) {
            x *= rhs.x;
            y *= rhs.y;
            return *this;
        }
        Vector2& operator/=(float scalar) {
            assert(scalar != 0.0);
            float inv = 1.0f / scalar;
            x *= inv;
            y *= inv;
            return *this;
        }
        Vector2& operator/=(const Vector2& rhs) {
            x /= rhs.x;
            y /= rhs.y;
            return *this;
        }

        // 长度
        float length() const { return std::hypot(x, y); }
        // 长度的平方
        float squaredLength() const { return x * x + y * y; }

        // 距离
        float distance(const Vector2& rhs) const { return (*this - rhs).length(); }
        // 距离的平方
        float squaredDistance(const Vector2& rhs) const { return (*this - rhs).squaredLength(); }

        // 点乘
        float dotProduct(const Vector2& vec) const { return x * vec.x + y * vec.y; }

        // 归一化, 返回向量原来的长度
        float normalise() {
            float length = std::hypot(x, y);

            if (length > 0.0f) {
                float inv_length = 1.0f / length;
                x *= inv_length;
                y *= inv_length;
            }

            return length;
        }

        float getX() const { return x; }
        float getY() const { return y; }

        void setX(float value) { x = value; }
        void setY(float value) { y = value; }

        // 两个向量的中点
        Vector2 midPoint(const Vector2& vec) const { return Vector2((x + vec.x) * 0.5f, (y + vec.y) * 0.5f); }

        // 两个向量的每个分量 < 另一个向量的对应分量
        bool operator<(const Vector2& rhs) const { return x < rhs.x && y < rhs.y; }
        // 两个向量的每个分量 > 另一个向量的对应分量
        bool operator>(const Vector2& rhs) const { return x > rhs.x && y > rhs.y; }

        // 使得向量的每个分量 <= 另一个向量的对应分量
        void makeFloor(const Vector2& cmp) {
            if (cmp.x < x) x = cmp.x;
            if (cmp.y < y) y = cmp.y;
        }
        // 使得向量的每个分量 >= 另一个向量的对应分量
        void makeCeil(const Vector2& cmp) {
            if (cmp.x > x) x = cmp.x;
            if (cmp.y > y) y = cmp.y;
        }

        // 生成一个垂直于该向量的向量
        Vector2 perpendicular(void) const { return Vector2(-y, x); }

        // 2D 叉乘, 返回一个浮点数, 该浮点数是两个向量组成的三角形的面积的两倍
        float crossProduct(const Vector2& rhs) const { return x * rhs.y - y * rhs.x; }

        // 判断向量长度是否为零
        bool isZeroLength(void) const {
            float sqlen = (x * x) + (y * y);
            return (sqlen < (Float_EPSILON * Float_EPSILON));
        }

        // 生成该向量归一化后的向量
        Vector2 normalisedCopy(void) const {
            Vector2 ret = *this;
            ret.normalise();
            return ret;
        }

        // 计算该向量在给定法线 normal 的平面上的反射向量
        // 假设该向量指向平面外部, 如果不是, 需要取反
        Vector2 reflect(const Vector2& normal) const {
            return Vector2(*this - (2 * this->dotProduct(normal) * normal));
        }

        // 判断该向量是否无效
        bool isNaN() const { return Math::isNan(x) || Math::isNan(y); }

        // 向量线性插值
        static Vector2 lerp(const Vector2& lhs, const Vector2& rhs, float alpha) { return lhs + alpha * (rhs - lhs); }

        // 特殊向量
        static const Vector2 ZERO;
        static const Vector2 UNIT_X;
        static const Vector2 UNIT_Y;
        static const Vector2 NEGATIVE_UNIT_X;
        static const Vector2 NEGATIVE_UNIT_Y;
        static const Vector2 UNIT_SCALE;
    };

} // namespace Piccolo
