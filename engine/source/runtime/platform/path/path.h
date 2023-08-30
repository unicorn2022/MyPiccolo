#pragma once

#include <filesystem>
#include <string>
#include <tuple>
#include <vector>

namespace Piccolo{

/**
 * @brief 路径工具类
 */
class Path{
public:
    // 获取相对路径
    static const std::filesystem::path getRelativePath(const std::filesystem::path& directory, const std::filesystem::path& file_path);

    // 获取文件分段
    static const std::vector<std::string> getPathSegments(const std::filesystem::path& file_path);

    // 获取文件扩展名(最多三个)
    static const std::tuple<std::string, std::string, std::string> getFileExtensions(const std::filesystem::path& file_path);

    // 获取文件真实名
    static const std::string getFilePureName(const std::string file_full_name);
};

}