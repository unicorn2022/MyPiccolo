#pragma once

#include "runtime/core/meta/reflection/reflection.h"

#include <string>
#include <vector>

namespace Piccolo {

    /* 骨骼节点映射 */
    REFLECTION_TYPE(AnimSkelMap)
    CLASS(AnimSkelMap, Fields)
    {
        REFLECTION_BODY(AnimSkelMap);

    public:
        std::vector<int> convert; // index => 骨骼节点编号
    };
} // namespace Piccolo