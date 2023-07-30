#pragma once

#include <filesystem>
#include <vector>

namespace MyPiccolo{

/**
 * @brief 文件服务
 */
class FileSystem{
public:
    // 从目录中获取所有文件的路径
    std::vector<std::filesystem::path> getFiles(const std::filesystem::path& directory);
};


}