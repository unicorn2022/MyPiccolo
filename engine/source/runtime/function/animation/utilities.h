#pragma once

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

namespace MyPiccolo {
    class Bone;
    class RawBone;
    class SkeletonData;

    // 计算 value 在 vec 中的下标
    template<typename T>
    size_t index(const std::vector<T>& vec, const T& value) {
        return std::distance(vec.begin(), std::find(vec.begin(), vec.end(), value));
    }

    // 将 addition 中的内容添加到 base 后面
    template<typename T>
    void append_vector(std::vector<T>& base, const std::vector<T>& addition) {
        base.insert(base.end(), addition.begin(), addition.end());
    }

    /**
     * @brief 通过下标查找骨骼
     * @param bones 骨骼数组
     * @param key 下标
     * @param size 数组大小
     * @param is_flat 是否为扁平数组
    */
    Bone* find_by_index(Bone* bones, int key, int size, bool is_flat = false);
    /**
     * @brief 通过下标查找骨骼
     * @param bones 骨骼数组
     * @param key 下标
     * @param size 数组大小
     * @param is_flat 是否为扁平数组
    */
    std::shared_ptr<RawBone> find_by_index(std::vector<std::shared_ptr<RawBone>>& bones, int key, bool is_flat = false);
    /**
     * @brief 通过骨骼名查找下标
     * @param skeleton 骨骼数据
     * @param name 骨骼名
    */
    int find_index_by_name(const SkeletonData& skeleton, const std::string& name);
} // namespace MyPiccolo