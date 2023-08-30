#pragma once

#include "runtime/core/meta/reflection/reflection.h"

#include <string>
#include <vector>

namespace Piccolo {

    REFLECTION_TYPE(BoneBlendMask)
    CLASS(BoneBlendMask, Fields)
    {
        REFLECTION_BODY(BoneBlendMask);

    public:
        std::string      skeleton_file_path; // 骨骼文件路径
        std::vector<int> enabled;            // 是否启用某个骨骼
    };
} // namespace Piccolo