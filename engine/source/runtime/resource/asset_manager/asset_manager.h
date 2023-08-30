#pragma once

#include "runtime/core/base/macro.h"
#include "runtime/core/meta/serializer/serializer.h"

#include <filesystem>
#include <fstream>
#include <functional>
#include <sstream>
#include <string>

#include "_generated/serializer/all_serializer.h"

namespace Piccolo {
    class AssetManager {
    public:
        /**
         * @brief 从json文件中加载资产
         * @param asset_url 存储路径
         * @param out_asset 资产
         * @return 是否加载成功
         */
        template<typename AssetType>
        bool loadAsset(const std::string& asset_url, AssetType& out_asset) const {
            // json文件 => 字符流
            std::filesystem::path asset_path = getFullPath(asset_url);
            std::ifstream         asset_json_file(asset_path);
            if (!asset_json_file) {
                LOG_ERROR("open file: {} failed!", asset_path.generic_string());
                return false;
            }

            std::stringstream buffer;
            buffer << asset_json_file.rdbuf();
            std::string asset_json_text(buffer.str());

            // 字符流 => asset对象
            std::string error;
            auto&&      asset_json = PJson::parse(asset_json_text, error);
            if (!error.empty()) {
                LOG_ERROR("parse json file {} failed!", asset_url);
                return false;
            }

            PSerializer::read(asset_json, out_asset);
            return true;
        }

        /**
         * @brief 将out_asset保存为json文件
         * @param out_asset 资产
         * @param asset_url 存储路径
         * @return 是否保存成功
         */
        template<typename AssetType>
        bool saveAsset(const AssetType& out_asset, const std::string& asset_url) const {
            std::ofstream asset_json_file(getFullPath(asset_url));
            if (!assert_json_file) {
                LOG_ERROR("open file: {} failed!", asset_url);
                return false;
            }

            // asset对象 => 字符流
            auto&&        asset_json      = PSerilizer::write(out_asset);
            std::string&& asset_json_text = asset_json.dump();

            // 写入文件
            asset_json_file << asset_json_text;
            asset_json_file.flush();

            return true;
        }
        std::filesystem::path getFullPath(const std::string& relative_path) const;
    };
} // namespace Piccolo