#pragma once

#include "runtime/function/render/interface/rhi.h"

#include <optional>
#include <vulkan/vulkan.h>

namespace Piccolo {
    /* 缓冲区 */
    class VulkanBuffer : public RHIBuffer {
    public:
        void setResource(VkBuffer res) {
            m_resource = res;
        }
        VkBuffer getResource() const {
            return m_resource;
        }

    private:
        VkBuffer m_resource;
    };

    /* 缓冲区视图 */
    class VulkanBufferView : public RHIBufferView {
    public:
        void setResource(VkBufferView res) {
            m_resource = res;
        }
        VkBufferView getResource() const {
            return m_resource;
        }

    private:
        VkBufferView m_resource;
    };

    /* 命令缓冲区 */
    class VulkanCommandBuffer : public RHICommandBuffer {
    public:
        void setResource(VkCommandBuffer res) {
            m_resource = res;
        }
        const VkCommandBuffer getResource() const {
            return m_resource;
        }

    private:
        VkCommandBuffer m_resource;
    };

    /* 命令池 */
    class VulkanCommandPool : public RHICommandPool {
    public:
        void setResource(VkCommandPool res) {
            m_resource = res;
        }
        VkCommandPool getResource() const {
            return m_resource;
        }

    private:
        VkCommandPool m_resource;
    };

    /* 描述符池 */
    class VulkanDescriptorPool : public RHIDescriptorPool {
    public:
        void setResource(VkDescriptorPool res) {
            m_resource = res;
        }
        VkDescriptorPool getResource() const {
            return m_resource;
        }

    private:
        VkDescriptorPool m_resource;
    };

    /* 描述符集 */
    class VulkanDescriptorSet : public RHIDescriptorSet {
    public:
        void setResource(VkDescriptorSet res) {
            m_resource = res;
        }
        VkDescriptorSet getResource() const {
            return m_resource;
        }

    private:
        VkDescriptorSet m_resource;
    };

    /* 描述符集布局 */
    class VulkanDescriptorSetLayout : public RHIDescriptorSetLayout {
    public:
        void setResource(VkDescriptorSetLayout res) {
            m_resource = res;
        }
        VkDescriptorSetLayout getResource() const {
            return m_resource;
        }

    private:
        VkDescriptorSetLayout m_resource;
    };

    /* 逻辑设备 */
    class VulkanDevice : public RHIDevice {
    public:
        void setResource(VkDevice res) {
            m_resource = res;
        }
        VkDevice getResource() const {
            return m_resource;
        }

    private:
        VkDevice m_resource;
    };

    /* 逻辑设备内存 */
    class VulkanDeviceMemory : public RHIDeviceMemory {
    public:
        void setResource(VkDeviceMemory res) {
            m_resource = res;
        }
        VkDeviceMemory getResource() const {
            return m_resource;
        }

    private:
        VkDeviceMemory m_resource;
    };

    /* Event: CPU 和 GPU 之间发送信号进行同步 */
    class VulkanEvent : public RHIEvent {
    public:
        void setResource(VkEvent res) {
            m_resource = res;
        }
        VkEvent getResource() const {
            return m_resource;
        }

    private:
        VkEvent m_resource;
    };

    /* Fence: CPU 和 GPU 之间的同步 */
    class VulkanFence : public RHIFence {
    public:
        void setResource(VkFence res) {
            m_resource = res;
        }
        VkFence getResource() const {
            return m_resource;
        }

    private:
        VkFence m_resource;
    };

    /* 帧缓冲 */
    class VulkanFramebuffer : public RHIFramebuffer {
    public:
        void setResource(VkFramebuffer res) {
            m_resource = res;
        }
        VkFramebuffer getResource() const {
            return m_resource;
        }

    private:
        VkFramebuffer m_resource;
    };

    /* 图像 */
    class VulkanImage : public RHIImage {
    public:
        void setResource(VkImage res) {
            m_resource = res;
        }
        VkImage& getResource() {
            return m_resource;
        }

    private:
        VkImage m_resource;
    };

    /* 图像视图 */
    class VulkanImageView : public RHIImageView {
    public:
        void setResource(VkImageView res) {
            m_resource = res;
        }
        VkImageView getResource() const {
            return m_resource;
        }

    private:
        VkImageView m_resource;
    };

    /* vulkan实例 */
    class VulkanInstance : public RHIInstance {
    public:
        void setResource(VkInstance res) {
            m_resource = res;
        }
        VkInstance getResource() const {
            return m_resource;
        }

    private:
        VkInstance m_resource;
    };

    /* 指令队列 */
    class VulkanQueue : public RHIQueue {
    public:
        void setResource(VkQueue res) {
            m_resource = res;
        }
        VkQueue getResource() const {
            return m_resource;
        }

    private:
        VkQueue m_resource;
    };

    /* 物理设备 */
    class VulkanPhysicalDevice : public RHIPhysicalDevice {
    public:
        void setResource(VkPhysicalDevice res) {
            m_resource = res;
        }
        VkPhysicalDevice getResource() const {
            return m_resource;
        }

    private:
        VkPhysicalDevice m_resource;
    };

    /* 渲染管线 */
    class VulkanPipeline : public RHIPipeline {
    public:
        void setResource(VkPipeline res) {
            m_resource = res;
        }
        VkPipeline getResource() const {
            return m_resource;
        }

    private:
        VkPipeline m_resource;
    };

    /* 渲染管线缓存 */
    class VulkanPipelineCache : public RHIPipelineCache {
    public:
        void setResource(VkPipelineCache res) {
            m_resource = res;
        }
        VkPipelineCache getResource() const {
            return m_resource;
        }

    private:
        VkPipelineCache m_resource;
    };

    /* 渲染管线布局 */
    class VulkanPipelineLayout : public RHIPipelineLayout {
    public:
        void setResource(VkPipelineLayout res) {
            m_resource = res;
        }
        VkPipelineLayout getResource() const {
            return m_resource;
        }

    private:
        VkPipelineLayout m_resource;
    };

    /* 渲染流程 */
    class VulkanRenderPass : public RHIRenderPass {
    public:
        void setResource(VkRenderPass res) {
            m_resource = res;
        }
        VkRenderPass getResource() const {
            return m_resource;
        }

    private:
        VkRenderPass m_resource;
    };

    /* 采样器 */
    class VulkanSampler : public RHISampler {
    public:
        void setResource(VkSampler res) {
            m_resource = res;
        }
        VkSampler getResource() const {
            return m_resource;
        }

    private:
        VkSampler m_resource;
    };

    /* Semaphore: GPU 的 Command Queue 之间的同步 */
    class VulkanSemaphore : public RHISemaphore {
    public:
        void setResource(VkSemaphore res) {
            m_resource = res;
        }
        VkSemaphore& getResource() {
            return m_resource;
        }

    private:
        VkSemaphore m_resource;
    };

    /* 着色器 */
    class VulkanShader : public RHIShader {
    public:
        void setResource(VkShaderModule res) {
            m_resource = res;
        }
        VkShaderModule getResource() const {
            return m_resource;
        }

    private:
        VkShaderModule m_resource;
    };
} // namespace Piccolo