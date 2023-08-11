#pragma once

#include "runtime/core/meta/reflection/reflection.h"

#include <string>
#include <vector>

namespace MyPiccolo {
    REFLECTION_TYPE(WorldRes)
    CLASS(WorldRes, Fields)
    {
        REFLECTION_BODY(WorldRes);

    public:
        // 世界名称
        std::string m_name;
        // 世界中所有关卡的url
        std::vector<std::string> m_level_urls;
        // 默认关卡的url
        std::string m_default_level_url;
    };
} // namespace MyPiccolo