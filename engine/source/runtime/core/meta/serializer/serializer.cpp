#include "serializer.h"
#include <assert.h>
namespace MyPiccolo {
    /* 序列化&反序列化：对 PJson 进行偏特化 */
    // 实例写入PJson: char
    template<>
    PJson PSerializer::write(const char& instance) {
        return PJson(instance);
    }
    // PJson读取实例: char
    template<>
    char& PSerializer::read(const PJson& json_context, char& instance) {
        assert(json_context.is_number());
        return instance = static_cast<char>(json_context.number_value());
    }

    // 实例写入PJson: int
    template<>
    PJson PSerializer::write(const int& instance) {
        return PJson(instance);
    }
    // PJson读取实例: int
    template<>
    int& PSerializer::read(const PJson& json_context, int& instance) {
        assert(json_context.is_number());
        return instance = static_cast<int>(json_context.number_value());
    }

    // 实例写入PJson: unsigned int
    template<>
    PJson PSerializer::write(const unsigned int& instance) {
        return PJson(static_cast<int>(instance));
    }
    // PJson读取实例: unsigned int
    template<>
    unsigned int& PSerializer::read(const PJson& json_context, unsigned int& instance) {
        assert(json_context.is_number());
        return instance = static_cast<unsigned int>(json_context.number_value());
    }

    // 实例写入PJson: float
    template<>
    PJson PSerializer::write(const float& instance) {
        return PJson(instance);
    }
    // PJson读取实例: float
    template<>
    float& PSerializer::read(const PJson& json_context, float& instance) {
        assert(json_context.is_number());
        return instance = static_cast<float>(json_context.number_value());
    }

    // 实例写入PJson: double
    template<>
    PJson PSerializer::write(const double& instance) {
        return PJson(instance);
    }
    // PJson读取实例: double
    template<>
    double& PSerializer::read(const PJson& json_context, double& instance) {
        assert(json_context.is_number());
        return instance = json_context.number_value();
    }

    // 实例写入PJson: bool
    template<>
    PJson PSerializer::write(const bool& instance) {
        return PJson(instance);
    }
    // PJson读取实例: bool
    template<>
    bool& PSerializer::read(const PJson& json_context, bool& instance) {
        assert(json_context.is_bool());
        return instance = json_context.bool_value();
    }

    // 实例写入PJson: string
    template<>
    PJson PSerializer::write(const std::string& instance) {
        return PJson(instance);
    }
    // PJson读取实例: string
    template<>
    std::string& PSerializer::read(const PJson& json_context, std::string& instance) {
        assert(json_context.is_string());
        return instance = json_context.string_value();
    }
} // namespace MyPiccolo