#include "file_service.h"

using namespace std;

namespace MyPiccolo{

std::vector<std::filesystem::path> FileSystem::getFiles(const std::filesystem::path& directory){
    
    vector<filesystem::path> files;
    
    // 递归读取当前路径下的所有文件&文件夹
    for(auto const& directory_entry : filesystem::recursive_directory_iterator{directory}){
        if (directory_entry.is_regular_file()) {
            files.push_back(directory_entry);
        }
    }
    return files;
}

} // namespace MyPiccolo