#pragma once

#include "runtime/core/math/random.h"

#include <algorithm>
#include <cmath>
#include <limits>

// 比较浮点数是否相等
#define CMP(x, y) (fabsf(x - y) < FLT_EPSILON * fmaxf(1.0f, fmaxf(fabsf(x), fabsf(y))))

namespace MyPiccolo {
    static const float Math_POS_INFINITY = std::numeric_limits<float>::infinity();  // 无穷大
    static const float Math_NEG_INFINITY = -std::numeric_limits<float>::infinity(); // 负无穷大
    static const float Math_PI           = 3.14159265358979323846264338327950288f;  // pi
    static const float Math_ONE_OVER_PI  = 1.0f / Math_PI;                          // 1/pi
    static const float Math_TWO_PI       = 2.0f * Math_PI;                          // 2*pi
    static const float Math_HALF_PI      = 0.5f * Math_PI;                          // pi/2
    static const float Math_fDeg2Rad     = Math_PI / 180.0f;                        // 角度转弧度
    static const float Math_fRad2Deg     = 180.0f / Math_PI;                        // 弧度转角度
    static const float Math_LOG2         = log(2.0f);                               // log2
    static const float Math_EPSILON      = 1e-6f;                                   // 精度

    static const float Float_EPSILON  = FLT_EPSILON; // float精度
    static const float Double_EPSILON = DBL_EPSILON; // double精度

    class Radian; // 弧度
    class Angle;  // 角度
    class Degree; // 度数

    class Vector2;    // 二维向量
    class Vector3;    // 三维向量
    class Vector4;    // 四维向量
    class Matrix3x3;  // 3x3矩阵
    class Matrix4x4;  // 4x4矩阵
    class Quaternion; // 四元数

    /* 弧度 */
    class Radian {
        float m_rad;

    public:
        explicit Radian(float r = 0) :
            m_rad(r) {}
        explicit Radian(const Degree& d);

        /* 赋值相关 */
        Radian& operator=(float f) {
            m_rad = f;
            return *this;
        }
        Radian& operator=(const Degree& d);

        /* 不同角度制之间的转换 */
        float valueRadians() const { return m_rad; }
        float valueDegrees() const; // see bottom of this file
        float valueAngleUnits() const;

        void setValue(float f) { m_rad = f; }

        /* 加法相关 */
        const Radian& operator+() const { return *this; }
        Radian        operator+(const Radian& r) const { return Radian(m_rad + r.m_rad); }
        Radian        operator+(const Degree& d) const;
        Radian&       operator+=(const Radian& r) {
            m_rad += r.m_rad;
            return *this;
        }
        Radian& operator+=(const Degree& d);
        /* 减法相关 */
        Radian  operator-() const { return Radian(-m_rad); }
        Radian  operator-(const Radian& r) const { return Radian(m_rad - r.m_rad); }
        Radian  operator-(const Degree& d) const;
        Radian& operator-=(const Radian& r) {
            m_rad -= r.m_rad;
            return *this;
        }
        Radian& operator-=(const Degree& d);
        /* 乘法相关 */
        Radian  operator*(float f) const { return Radian(m_rad * f); }
        Radian  operator*(const Radian& f) const { return Radian(m_rad * f.m_rad); }
        Radian& operator*=(float f) {
            m_rad *= f;
            return *this;
        }
        /* 除法相关 */
        Radian  operator/(float f) const { return Radian(m_rad / f); }
        Radian& operator/=(float f) {
            m_rad /= f;
            return *this;
        }
        /* 比较相关 */
        bool operator<(const Radian& r) const { return m_rad < r.m_rad; }
        bool operator<=(const Radian& r) const { return m_rad <= r.m_rad; }
        bool operator==(const Radian& r) const { return m_rad == r.m_rad; }
        bool operator!=(const Radian& r) const { return m_rad != r.m_rad; }
        bool operator>=(const Radian& r) const { return m_rad >= r.m_rad; }
        bool operator>(const Radian& r) const { return m_rad > r.m_rad; }
    };

    /* 角度 */
    class Degree {
        float m_deg; // if you get an error here - make sure to define/typedef 'float' first

    public:
        explicit Degree(float d = 0) :
            m_deg(d) {}
        explicit Degree(const Radian& r) :
            m_deg(r.valueDegrees()) {}

        /* 赋值相关 */
        Degree& operator=(float f) {
            m_deg = f;
            return *this;
        }
        Degree& operator=(const Degree& d) {
            m_deg = d.m_deg;
            return *this;
        }
        Degree& operator=(const Radian& r) {
            m_deg = r.valueDegrees();
            return *this;
        }

        /* 不同角度制之间的转换 */
        float valueDegrees() const { return m_deg; }
        float valueRadians() const; // see bottom of this file
        float valueAngleUnits() const;

        /* 加法相关 */
        const Degree& operator+() const { return *this; }
        Degree        operator+(const Degree& d) const { return Degree(m_deg + d.m_deg); }
        Degree        operator+(const Radian& r) const { return Degree(m_deg + r.valueDegrees()); }
        Degree&       operator+=(const Degree& d) {
            m_deg += d.m_deg;
            return *this;
        }
        Degree& operator+=(const Radian& r) {
            m_deg += r.valueDegrees();
            return *this;
        }
        /* 减法相关 */
        Degree  operator-() const { return Degree(-m_deg); }
        Degree  operator-(const Degree& d) const { return Degree(m_deg - d.m_deg); }
        Degree  operator-(const Radian& r) const { return Degree(m_deg - r.valueDegrees()); }
        Degree& operator-=(const Degree& d) {
            m_deg -= d.m_deg;
            return *this;
        }
        Degree& operator-=(const Radian& r) {
            m_deg -= r.valueDegrees();
            return *this;
        }
        /* 乘法相关 */
        Degree  operator*(float f) const { return Degree(m_deg * f); }
        Degree  operator*(const Degree& f) const { return Degree(m_deg * f.m_deg); }
        Degree& operator*=(float f) {
            m_deg *= f;
            return *this;
        }
        /* 除法相关 */
        Degree  operator/(float f) const { return Degree(m_deg / f); }
        Degree& operator/=(float f) {
            m_deg /= f;
            return *this;
        }
        /* 比较相关 */
        bool operator<(const Degree& d) const { return m_deg < d.m_deg; }
        bool operator<=(const Degree& d) const { return m_deg <= d.m_deg; }
        bool operator==(const Degree& d) const { return m_deg == d.m_deg; }
        bool operator!=(const Degree& d) const { return m_deg != d.m_deg; }
        bool operator>=(const Degree& d) const { return m_deg >= d.m_deg; }
        bool operator>(const Degree& d) const { return m_deg > d.m_deg; }
    };

    /* 度数, 会自动转化为弧度&角度 */
    class Angle {
        float m_angle;

    public:
        explicit Angle(float angle) :
            m_angle(angle) {}
        Angle() { m_angle = 0; }

        explicit operator Radian() const;
        explicit operator Degree() const;
    };

    /* 数学库 */
    class Math {
    private:
        enum class AngleUnit {
            AU_DEGREE,
            AU_RADIAN
        };
        static AngleUnit k_AngleUnit; // 角度单位

    public:
        Math();

        /* 浮点运算相关 */
        static float abs(float value) { return std::fabs(value); }
        static bool  isNan(float f) { return std::isnan(f); }
        static float sqr(float value) { return value * value; }
        static float sqrt(float fValue) { return std::sqrt(fValue); }
        static float invSqrt(float value) { return 1.f / sqrt(value); }
        static bool  realEqual(float a, float b, float tolerance = std::numeric_limits<float>::epsilon());
        static float clamp(float v, float min, float max) { return std::clamp(v, min, max); }
        static float getMaxElement(float x, float y, float z) { return std::max({x, y, z}); }

        /* 不同角度单位之间的转换 */
        static float degreesToRadians(float degrees);
        static float radiansToDegrees(float radians);
        static float angleUnitsToRadians(float units);
        static float radiansToAngleUnits(float radians);
        static float angleUnitsToDegrees(float units);
        static float degreesToAngleUnits(float degrees);

        /* 三角函数相关 */
        static float  sin(const Radian& rad) { return std::sin(rad.valueRadians()); }
        static float  sin(float value) { return std::sin(value); }
        static float  cos(const Radian& rad) { return std::cos(rad.valueRadians()); }
        static float  cos(float value) { return std::cos(value); }
        static float  tan(const Radian& rad) { return std::tan(rad.valueRadians()); }
        static float  tan(float value) { return std::tan(value); }
        static Radian acos(float value);
        static Radian asin(float value);
        static Radian atan(float value) { return Radian(std::atan(value)); }
        static Radian atan2(float y_v, float x_v) { return Radian(std::atan2(y_v, x_v)); }

        // 取最大值
        template<class T>
        static constexpr T max(const T A, const T B) {
            return std::max(A, B);
        }
        // 取最小值
        template<class T>
        static constexpr T min(const T A, const T B) {
            return std::min(A, B);
        }
        // 取最大值
        template<class T>
        static constexpr T max3(const T A, const T B, const T C) {
            return std::max({A, B, C});
        }
        // 取最小值
        template<class T>
        static constexpr T min3(const T A, const T B, const T C) {
            return std::min({A, B, C});
        }

        /**
         * @brief 计算 view 矩阵
         * @param position 位置
         * @param orientation 朝向
         * @param reflect_matrix 反射矩阵
         */
        static Matrix4x4
        makeViewMatrix(const Vector3& position, const Quaternion& orientation, const Matrix4x4* reflect_matrix = nullptr);
        /**
         * @brief 计算 LookAt 矩阵
         * @param eye_position 眼睛位置
         * @param target_position 目标位置
         * @param up_dir 向上方向
         */
        static Matrix4x4
        makeLookAtMatrix(const Vector3& eye_position, const Vector3& target_position, const Vector3& up_dir);
        /**
         * @brief 计算 透视投影 矩阵
         * @param fovy 视角
         * @param aspect 长宽比
         * @param znear 近平面距离
         * @param zfar 原平面距离
         */
        static Matrix4x4 makePerspectiveMatrix(Radian fovy, float aspect, float znear, float zfar);
        /**
         * @brief 计算 正交投影 矩阵
         * @param left  左边界
         * @param right 右边界
         * @param bottom 下边界
         * @param top 上边界
         * @param znear 近平面
         * @param zfar 原平面
         */
        static Matrix4x4
        makeOrthographicProjectionMatrix(float left, float right, float bottom, float top, float znear, float zfar);
        /**
         * @brief 计算 正交投影 矩阵
         * @param left  左边界
         * @param right 右边界
         * @param bottom 下边界
         * @param top 上边界
         * @param znear 近平面
         * @param zfar 原平面
         */
        static Matrix4x4
        makeOrthographicProjectionMatrix01(float left, float right, float bottom, float top, float znear, float zfar);
    };

    /* Radian类的实现 */
    inline Radian::Radian(const Degree& d) :
        m_rad(d.valueRadians()) {}
    inline Radian& Radian::operator=(const Degree& d) {
        m_rad = d.valueRadians();
        return *this;
    }
    inline Radian Radian::operator+(const Degree& d) const { return Radian(m_rad + d.valueRadians()); }
    inline Radian& Radian::operator+=(const Degree& d) {
        m_rad += d.valueRadians();
        return *this;
    }
    inline Radian Radian::operator-(const Degree& d) const { return Radian(m_rad - d.valueRadians()); }
    inline Radian& Radian::operator-=(const Degree& d) {
        m_rad -= d.valueRadians();
        return *this;
    }
    inline float  Radian::valueDegrees() const { return Math::radiansToDegrees(m_rad); }
    inline float  Radian::valueAngleUnits() const { return Math::radiansToAngleUnits(m_rad); }
    inline Radian operator*(float a, const Radian& b) { return Radian(a * b.valueRadians()); }
    inline Radian operator/(float a, const Radian& b) { return Radian(a / b.valueRadians()); }

    /* Degree类的实现 */
    inline float  Degree::valueRadians() const { return Math::degreesToRadians(m_deg); }
    inline float  Degree::valueAngleUnits() const { return Math::degreesToAngleUnits(m_deg); }
    inline Degree operator*(float a, const Degree& b) { return Degree(a * b.valueDegrees()); }
    inline Degree operator/(float a, const Degree& b) { return Degree(a / b.valueDegrees()); }

    /* Angle类的实现 */
    inline Angle::operator Radian() const { return Radian(Math::angleUnitsToRadians(m_angle)); }
    inline Angle::operator Degree() const { return Degree(Math::angleUnitsToDegrees(m_angle)); }

} // namespace MyPiccolo