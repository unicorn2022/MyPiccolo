#include "runtime/function/animation/utilities.h"

#include "runtime/function/animation/node.h"

namespace Piccolo {
    /**
     * @brief 通过下标查找骨骼
     * @param bones 骨骼数组
     * @param key 下标
     * @param size 数组大小
     * @param is_flat 是否为扁平数组
    */
    Bone* find_by_index(Bone* bones, int key, int size, bool is_flat = false) {
        if (key == std::numeric_limits<int>::max()) return nullptr;

        if (is_flat) {
            if (key >= size)
                return nullptr;
            else
                return &bones[key];
        } else {
            for (int i = 0; i < size; i++)
                if (bones[i].getID() == key)
                    return &bones[i];
        }

        return nullptr;
    }
    /**
     * @brief 通过下标查找骨骼
     * @param bones 骨骼数组
     * @param key 下标
     * @param size 数组大小
     * @param is_flat 是否为扁平数组
    */
    std::shared_ptr<RawBone> find_by_index(std::vector<std::shared_ptr<RawBone>>& bones, int key, bool is_flat = false) {
        if (key == std::numeric_limits<int>::max()) return nullptr;

        if (is_flat)
            return bones[key];
        else {
            const auto it = std::find_if(bones.begin(), bones.end(), [&](const auto& bone) { return bone->index == key; });
            if (it != bones.end()) return *it;
        }

        return nullptr;
    }
    /**
     * @brief 通过骨骼名查找下标
     * @param skeleton 骨骼数据
     * @param name 骨骼名
    */
    int find_index_by_name(const SkeletonData& skeleton, const std::string& name) {
        const auto it = std::find_if(skeleton.bones_map.begin(), skeleton.bones_map.end(), [&](const auto& bone) { return bone.name == name; });
        if (it != skeleton.bones_map.end())
            return it->index;
        else
            return std::numeric_limits<int>::max();
    }

} // namespace Piccolo