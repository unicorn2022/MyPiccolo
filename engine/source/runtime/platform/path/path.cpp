#include "path.h"

using namespace std;

namespace MyPiccolo{

// 获取相对路径
const std::filesystem::path Path::getRelativePath(const std::filesystem::path& directory, const std::filesystem::path& file_path){
    return file_path.lexically_relative(directory);
}

// 获取文件分段
const std::vector<std::string> Path::getPathSegments(const std::filesystem::path& file_path){
    vector<string> segments;
    for(auto it = file_path.begin(); it != file_path.end(); ++it){
        segments.emplace_back(it->generic_string());
    }
    return segments;
}

// 获取文件扩展名(最多三个)
const std::tuple<std::string, std::string, std::string> Path::getFileExtensions(const std::filesystem::path& file_path){
    return make_tuple(
        file_path.extension().generic_string(),
        file_path.stem().extension().generic_string(),
        file_path.stem().stem().extension().generic_string()
    );
}

// 获取文件真实名
const std::string Path::getFilePureName(const std::string file_full_name){
    string file_pure_name = file_full_name;
    auto pos = file_full_name.find_first_of('.');
    if(pos != string::npos){
        file_pure_name = file_full_name.substr(0, pos);
    }
    return file_pure_name;
}

}