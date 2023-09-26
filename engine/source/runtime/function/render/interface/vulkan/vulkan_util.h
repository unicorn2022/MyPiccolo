#pragma once

#include "runtime/function/render/interface/rhi.h"

#include <vk_mem_alloc.h>
#include <vulkan/vulkan.h>

#include <array>
#include <unordered_map>
#include <vector>

namespace Piccolo {
    /* Vulkan 工具类 */
    class VulkanUtil {
    public:
        // 查询memory类型
        static uint32_t findMemoryType(VkPhysicalDevice physical_device, uint32_t type_filter, VkMemoryPropertyFlags properties_flag);
        // create 着色器模块
        static VkShaderModule createShaderModule(VkDevice device, const std::vector<unsigned char>& shader_code);
        // create 缓冲区
        static void createBuffer(VkPhysicalDevice physical_device, VkDevice device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& buffer_memory);
        // create 缓冲区并初始化
        static void createBufferAndInitialize(VkDevice device, VkPhysicalDevice physicalDevice, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkBuffer* buffer, VkDeviceMemory* memory, VkDeviceSize size, void* data = nullptr, int datasize = 0);
        // copy 缓冲区
        static void copyBuffer(RHI* rhi, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize srcOffset, VkDeviceSize dstOffset, VkDeviceSize size);
        // create 图像
        static void createImage(VkPhysicalDevice physical_device, VkDevice device, uint32_t image_width, uint32_t image_height, VkFormat format, VkImageTiling image_tiling, VkImageUsageFlags image_usage_flags, VkMemoryPropertyFlags memory_property_flags, VkImage& image, VkDeviceMemory& memory, VkImageCreateFlags image_create_flags, uint32_t array_layers, uint32_t miplevels);
        // create 图像视图
        static VkImageView createImageView(VkDevice device, VkImage& image, VkFormat format, VkImageAspectFlags image_aspect_flags, VkImageViewType view_type, uint32_t layout_count, uint32_t miplevels);
        // create 全局图像
        static void createGlobalImage(RHI* rhi, VkImage& image, VkImageView& image_view, VmaAllocation& image_allocation, uint32_t texture_image_width, uint32_t texture_image_height, void* texture_image_pixels, RHIFormat texture_image_format, uint32_t miplevels = 0);
        // create 立方体图像
        static void createCubeMap(RHI* rhi, VkImage& image, VkImageView& image_view, VmaAllocation& image_allocation, uint32_t texture_image_width, uint32_t texture_image_height, std::array<void*, 6> texture_image_pixels, RHIFormat texture_image_format, uint32_t miplevels);
        // generate 纹理MipMaps
        static void generateTextureMipMaps(RHI* rhi, VkImage image, VkFormat image_format, uint32_t texture_width, uint32_t texture_height, uint32_t layers, uint32_t miplevels);
        // trasition 图像布局
        static void transitionImageLayout(RHI* rhi, VkImage image, VkImageLayout old_layout, VkImageLayout new_layout, uint32_t layer_count, uint32_t miplevels, VkImageAspectFlags aspect_mask_bits);
        // copy 缓冲区到图像
        static void copyBufferToImage(RHI* rhi, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layer_count);
        // generate Mipmapped 图像
        static void genMipmappedImage(RHI* rhi, VkImage image, uint32_t width, uint32_t height, uint32_t mip_levels);

        // get or create Mipmapped采样器
        static VkSampler getOrCreateMipmapSampler(VkPhysicalDevice physical_device, VkDevice device, uint32_t width, uint32_t height);
        // destroy Mipmapped采样器
        static void destroyMipmappedSampler(VkDevice device);
        // get or create Nearest采样器
        static VkSampler getOrCreateNearestSampler(VkPhysicalDevice physical_device, VkDevice device);
        // get or create Linear采样器
        static VkSampler getOrCreateLinearSampler(VkPhysicalDevice physical_device, VkDevice device);
        // destroy Nearest采样器
        static void destroyNearestSampler(VkDevice device);
        // destroy Linear采样器
        static void destroyLinearSampler(VkDevice device);

    private:
        // mipmap => sampler
        static std::unordered_map<uint32_t, VkSampler> m_mipmap_sampler_map;
        // Nearest采样器
        static VkSampler m_nearest_sampler;
        // Linear采样器
        static VkSampler m_linear_sampler;
    };
} // namespace Piccolo