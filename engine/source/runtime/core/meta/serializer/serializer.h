/**
 * @file serializer.h
 * @brief 序列化&反序列化
 */

#pragma once
#include "runtime/core/meta/json.h"
#include "runtime/core/meta/reflection/reflection.h"

#include <cassert>

namespace Piccolo {
    template<typename...>
    inline constexpr bool always_false = false;

    /* 序列化&反序列化：该模板类只有接口, 需要对不同的类型进行偏特化 */
    class PSerializer {
    public:
        // 将 instance反射指针 中的内容写入PJson
        template<typename T>
        static PJson write(const Reflection::ReflectionPtr<T>& instance) {
            std::string type_name    = instance.getTypeName();
            T*          instacne_ptr = static_cast<T*>(instance.operator->());
            return PJson::object {
                {"$typeName", PJson {type_name}},                                        // 类型名称
                {"$context", Reflection::TypeMeta::writeByName(type_name, instance_ptr)} // 调用 Reflection::TypeMeta::writeByName 写入内容
            };
        }
        // 读取PJson中的内容, 生成 instance反射指针
        template<typename T>
        static T*& read(const PJson& json_context, Reflection::ReflectionPtr<T>& instance) {
            std::string type_name = json_context["$typeName"].string_value();
            instance.setTypeName(type_name);
            return readPointer(json_context["$context"], instance.getPtrReference());
        }

    public:
        // 将 instance指针 中的内容写入PJson
        template<typename T>
        static PJson writePointer(T* instance) {
            return PJson::object {
                {"$typeName", PJson {"*"}},                 // 指针类型
                {"$context", PSerializer::write(*instance)} // 调用 PSerializer::write(实例) 写入内容
            };
        }
        // 读取PJson中的内容, 生成 instance指针
        template<typename T>
        static T*& readPointer(const PJson& json_context, T*& instance) {
            assert(instance == nullptr);
            std::string type_name = json_context["$typeName"].string_value();
            assert(!type_name.empty());

            if (type_name[0] == '*') {
                // 如果是指针类型, 则调用 read(实例)
                instance = new T;
                read(json_context["$context"], *instance);
            } else {
                // 如果是实例类型, 则调用 Reflection::TypeMeta::newFromNameAndPJson(), 创建一个指针
                instance = static_cast<T*>(Reflection::TypeMeta::newFromNameAndPJson(type_name, json_context["$context"].m_instance));
            }
            return instance;
        }

    public:
        // 将 instance实例 中的内容写入PJson
        template<typename T>
        static PJson write(const T& instance) {
            if constexpr (std::is_pointer<T>::value) {
                return writePointer((T)instance);
            } else {
                static_assert(always_false<T>, "PSerializer::write<T> has not been implemented yet!");
                return PJson();
            }
        }
        // 读取PJson中的内容, 生成 instance实例
        template<typename T>
        static T& read(const PJson& json_context, T& instance) {
            if constexpr (std::is_pointer<T>::value) {
                return readPointer(json_context, instance);
            } else {
                static_assert(always_false<T>, "PSerializer::read<T> has not been implemented yet!");
                return instance;
            }
        }
    };

    /* 序列化&反序列化：对 PJson 进行偏特化 */
    // 实例写入PJson: char
    template<>
    PJson PSerializer::write(const char& instance);
    // PJson读取实例: char
    template<>
    char& PSerializer::read(const PJson& json_context, char& instance);

    // 实例写入PJson: int
    template<>
    PJson PSerializer::write(const int& instance);
    // PJson读取实例: int
    template<>
    int& PSerializer::read(const PJson& json_context, int& instance);

    // 实例写入PJson: unsigned int
    template<>
    PJson PSerializer::write(const unsigned int& instance);
    // PJson读取实例: unsigned int
    template<>
    unsigned int& PSerializer::read(const PJson& json_context, unsigned int& instance);

    // 实例写入PJson: float
    template<>
    PJson PSerializer::write(const float& instance);
    // PJson读取实例: float
    template<>
    float& PSerializer::read(const PJson& json_context, float& instance);

    // 实例写入PJson: double
    template<>
    PJson PSerializer::write(const double& instance);
    // PJson读取实例: double
    template<>
    double& PSerializer::read(const PJson& json_context, double& instance);

    // 实例写入PJson: bool
    template<>
    PJson PSerializer::write(const bool& instance);
    // PJson读取实例: bool
    template<>
    bool& PSerializer::read(const PJson& json_context, bool& instance);

    // 实例写入PJson: string
    template<>
    PJson PSerializer::write(const std::string& instance);
    // PJson读取实例: string
    template<>
    std::string& PSerializer::read(const PJson& json_context, std::string& instance);

} // namespace Piccolo