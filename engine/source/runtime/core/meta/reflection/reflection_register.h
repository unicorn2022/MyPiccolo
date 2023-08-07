/**
 * @file reflection_register.h
 * @brief 反射注册器
 */

#pragma once
namespace MyPiccolo {
    namespace Reflection {
        class TypeMetaRegister {
        public:
            static void Register();
            static void Unregister();
        };
    } // namespace Reflection
} // namespace MyPiccolo