/**
 * @file meta_example.cpp
 * @brief 反射的使用示例
 */
#pragma once

#include "runtime/core/meta/reflection/reflection.h"

namespace Piccolo {
    /* Fields：整个类的所有成员变量均需要反射 */
    REFLECTION_TYPE(BaseTest)
    CLASS(BaseTest, Fields)
    {
        REFLECTION_BODY(BaseTest);

    public:
        int               m_int;
        std::vector<int*> m_int_vector;
    };

    /* WhiteListFields：只有标明META(Enable)的属性需要反射 */
    REFLECTION_TYPE(Test1)
    CLASS(Test1 : public BaseTest, WhiteListFields)
    {
        REFLECTION_BODY(Test1);

    public:
        META(Enable)
        char m_char;
    };

    /* Fields：整个类的所有成员变量均需要反射 */
    REFLECTION_TYPE(Test2)
    CLASS(Test2 : public BaseTest, , Fields)
    {
        REFLECTION_BODY(Test2);

    public:
        std::vector<Reflection::ReflectionPtr<BaseTest>> m_test_base_array;
    };
} // namespace Piccolo
