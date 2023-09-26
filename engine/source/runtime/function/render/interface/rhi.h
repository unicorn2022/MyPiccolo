#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vk_mem_alloc.h>

#include <functional>
#include <memory>
#include <vector>

#include "rhi_struct.h"

namespace Piccolo {
    class WindowSystem;

    // RHI 初始化信息
    struct RHIInitInfo {
        std::shared_ptr<WindowSystem> window_system;
    };

    // RHI 接口
    class RHI {
    public:
        virtual ~RHI() = 0;

        // 初始化
        virtual void initialize(RHIInitInfo initialize_info) = 0;
        // 准备 context
        virtual void prepareContext() = 0;

        // 是否支持点光源阴影
        virtual bool isPointLightShadowEnabled() = 0;

        /* 申请 & 创建 */
        // allocate 命令缓冲
        virtual bool allocateCommandBuffers(const RHICommandBufferAllocateInfo* pAllocateInfo, RHICommandBuffer*& pCommandBuffers) = 0;
        // allocate 描述符集
        virtual bool allocateDescriptorSets(const RHIDescriptorSetAllocateInfo* pAllocateInfo, RHIDescriptorSet*& pDescriptorSets) = 0;
        // create 交换链
        virtual void createSwapchain() = 0;
        // recreate 交换链
        virtual void recreateSwapchain() = 0;
        // create 交换链图像视图
        virtual void createSwapchainImageViews() = 0;
        // create 深度图像&视图
        virtual void createFramebufferImageAndView() = 0;
        // get or create 默认采样器
        virtual RHISampler* getOrCreateDefaultSampler(RHIDefaultSamplerType type) = 0;
        // get or create Mipmap采样器
        virtual RHISampler* getOrCreateMipmapSampler(uint32_t width, uint32_t height) = 0;
        // create 着色器模块
        virtual RHIShader* createShaderModule(const std::vector<unsigned char>& shader_code) = 0;
        // create 缓冲区
        virtual void createBuffer(RHIDeviceSize size, RHIBufferUsageFlags usage, RHIMemoryPropertyFlags properties, RHIBuffer*& buffer, RHIDeviceMemory*& buffer_memory) = 0;
        // create 缓冲区并初始化
        virtual void createBufferAndInitialize(RHIBufferUsageFlags usage, RHIMemoryPropertyFlags properties, RHIBuffer*& buffer, RHIDeviceMemory*& buffer_memory, RHIDeviceSize size, void* data = nullptr, int datasize = 0) = 0;
        // create 缓冲区内存分配器 (VMA = Vulkan Memory Allocator)
        virtual bool createBufferVMA(VmaAllocator                   allocator,
                                     const RHIBufferCreateInfo*     pBufferCreateInfo,
                                     const VmaAllocationCreateInfo* pAllocationCreateInfo,
                                     RHIBuffer*&                    pBuffer,
                                     VmaAllocation*                 pAllocation,
                                     VmaAllocationInfo*             pAllocationInfo) = 0;
        // create 缓冲区对齐内存分配器 (VMA = Vulkan Memory Allocator)
        virtual bool createBufferWithAlignmentVMA(
            VmaAllocator                   allocator,
            const RHIBufferCreateInfo*     pBufferCreateInfo,
            const VmaAllocationCreateInfo* pAllocationCreateInfo,
            RHIDeviceSize                  minAlignment,
            RHIBuffer*&                    pBuffer,
            VmaAllocation*                 pAllocation,
            VmaAllocationInfo*             pAllocationInfo) = 0;
        // copy 缓冲区内容
        virtual void copyBuffer(RHIBuffer* srcBuffer, RHIBuffer* dstBuffer, RHIDeviceSize srcOffset, RHIDeviceSize dstOffset, RHIDeviceSize size) = 0;
        // create 图像
        virtual void createImage(uint32_t image_width, uint32_t image_height, RHIFormat format, RHIImageTiling image_tiling, RHIImageUsageFlags image_usage_flags, RHIMemoryPropertyFlags memory_property_flags, RHIImage*& image, RHIDeviceMemory*& memory, RHIImageCreateFlags image_create_flags, uint32_t array_layers, uint32_t miplevels) = 0;
        // create 图像视图
        virtual void createImageView(RHIImage* image, RHIFormat format, RHIImageAspectFlags image_aspect_flags, RHIImageViewType view_type, uint32_t layout_count, uint32_t miplevels, RHIImageView*& image_view) = 0;
        // create 全局图像
        virtual void createGlobalImage(RHIImage*& image, RHIImageView*& image_view, VmaAllocation& image_allocation, uint32_t texture_image_width, uint32_t texture_image_height, void* texture_image_pixels, RHIFormat texture_image_format, uint32_t miplevels = 0) = 0;
        // create 立方贴图
        virtual void createCubeMap(RHIImage*& image, RHIImageView*& image_view, VmaAllocation& image_allocation, uint32_t texture_image_width, uint32_t texture_image_height, std::array<void*, 6> texture_image_pixels, RHIFormat texture_image_format, uint32_t miplevels) = 0;
        // create 命令池
        virtual void createCommandPool() = 0;
        // create 命令池
        virtual bool createCommandPool(const RHICommandPoolCreateInfo* pCreateInfo, RHICommandPool*& pCommandPool) = 0;
        // create 描述符池
        virtual bool createDescriptorPool(const RHIDescriptorPoolCreateInfo* pCreateInfo, RHIDescriptorPool*& pDescriptorPool) = 0;
        // create 描述符集布局
        virtual bool createDescriptorSetLayout(const RHIDescriptorSetLayoutCreateInfo* pCreateInfo, RHIDescriptorSetLayout*& pSetLayout) = 0;
        // create Fence: CPU 和 GPU 之间的同步
        virtual bool createFence(const RHIFenceCreateInfo* pCreateInfo, RHIFence*& pFence) = 0;
        // create 帧缓冲
        virtual bool createFramebuffer(const RHIFramebufferCreateInfo* pCreateInfo, RHIFramebuffer*& pFramebuffer) = 0;
        // create 图形渲染管线
        virtual bool createGraphicsPipelines(RHIPipelineCache* pipelineCache, uint32_t createInfoCount, const RHIGraphicsPipelineCreateInfo* pCreateInfos, RHIPipeline*& pPipelines) = 0;
        // create 计算管线
        virtual bool createComputePipelines(RHIPipelineCache* pipelineCache, uint32_t createInfoCount, const RHIComputePipelineCreateInfo* pCreateInfos, RHIPipeline*& pPipelines) = 0;
        // create 管线布局
        virtual bool createPipelineLayout(const RHIPipelineLayoutCreateInfo* pCreateInfo, RHIPipelineLayout*& pPipelineLayout) = 0;
        // create 渲染流程
        virtual bool createRenderPass(const RHIRenderPassCreateInfo* pCreateInfo, RHIRenderPass*& pRenderPass) = 0;
        // create 采样器
        virtual bool createSampler(const RHISamplerCreateInfo* pCreateInfo, RHISampler*& pSampler) = 0;
        // create Semaphore: GPU 的 Command Queue 之间的同步                                                                                                                                                                                                                   = 0;
        virtual bool createSemaphore(const RHISemaphoreCreateInfo* pCreateInfo, RHISemaphore*& pSemaphore) = 0;

        /* 命令缓冲 & 命令写入 */
        // (函数指针)wait Fence
        virtual bool waitForFencesPFN(uint32_t fenceCount, RHIFence* const* pFence, RHIBool32 waitAll, uint64_t timeout) = 0;
        // (函数指针)reset Fence
        virtual bool resetFencesPFN(uint32_t fenceCount, RHIFence* const* pFences) = 0;
        // (函数指针)reset 命令池
        virtual bool resetCommandPoolPFN(RHICommandPool* commandPool, RHICommandPoolResetFlags flags) = 0;
        // (函数指针)begin 命令缓冲的记录
        virtual bool beginCommandBufferPFN(RHICommandBuffer* commandBuffer, const RHICommandBufferBeginInfo* pBeginInfo) = 0;
        // (函数指针)end 命令缓冲的记录
        virtual bool endCommandBufferPFN(RHICommandBuffer* commandBuffer) = 0;
        // (函数指针)command 开始渲染流程
        virtual void cmdBeginRenderPassPFN(RHICommandBuffer* commandBuffer, const RHIRenderPassBeginInfo* pRenderPassBegin, RHISubpassContents contents) = 0;
        // (函数指针)command 下一个子流程
        virtual void cmdNextSubpassPFN(RHICommandBuffer* commandBuffer, RHISubpassContents contents) = 0;
        // (函数指针)command 结束渲染流程
        virtual void cmdEndRenderPassPFN(RHICommandBuffer* commandBuffer) = 0;
        // (函数指针)command 绑定渲染管线
        virtual void cmdBindPipelinePFN(RHICommandBuffer* commandBuffer, RHIPipelineBindPoint pipelineBindPoint, RHIPipeline* pipeline) = 0;
        // (函数指针)command 设置视口
        virtual void cmdSetViewportPFN(RHICommandBuffer* commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const RHIViewport* pViewports) = 0;
        // (函数指针)command 设置裁剪
        virtual void cmdSetScissorPFN(RHICommandBuffer* commandBuffer, uint32_t firstScissor, uint32_t scissorCount, const RHIRect2D* pScissors) = 0;
        // (函数指针)command 绑定顶点缓冲
        virtual void cmdBindVertexBuffersPFN(
            RHICommandBuffer*    commandBuffer,
            uint32_t             firstBinding,
            uint32_t             bindingCount,
            RHIBuffer* const*    pBuffers,
            const RHIDeviceSize* pOffsets) = 0;
        // (函数指针)command 绑定索引缓冲
        virtual void cmdBindIndexBufferPFN(RHICommandBuffer* commandBuffer, RHIBuffer* buffer, RHIDeviceSize offset, RHIIndexType indexType) = 0;
        // (函数指针)command 绑定描述符集
        virtual void cmdBindDescriptorSetsPFN(
            RHICommandBuffer*              commandBuffer,
            RHIPipelineBindPoint           pipelineBindPoint,
            RHIPipelineLayout*             layout,
            uint32_t                       firstSet,
            uint32_t                       descriptorSetCount,
            const RHIDescriptorSet* const* pDescriptorSets,
            uint32_t                       dynamicOffsetCount,
            const uint32_t*                pDynamicOffsets) = 0;
        // (函数指针)command 绘制索引
        virtual void cmdDrawIndexedPFN(RHICommandBuffer* commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) = 0;
        // (函数指针)command 清除附件
        virtual void cmdClearAttachmentsPFN(RHICommandBuffer* commandBuffer, uint32_t attachmentCount, const RHIClearAttachment* pAttachments, uint32_t rectCount, const RHIClearRect* pRects) = 0;

        // begin 命令缓冲的记录
        virtual bool beginCommandBuffer(RHICommandBuffer* commandBuffer, const RHICommandBufferBeginInfo* pBeginInfo) = 0;
        // command 将图像拷贝到缓冲区
        virtual void cmdCopyImageToBuffer(RHICommandBuffer* commandBuffer, RHIImage* srcImage, RHIImageLayout srcImageLayout, RHIBuffer* dstBuffer, uint32_t regionCount, const RHIBufferImageCopy* pRegions) = 0;
        // command 将图像拷贝到图像
        virtual void cmdCopyImageToImage(RHICommandBuffer* commandBuffer, RHIImage* srcImage, RHIImageAspectFlagBits srcFlag, RHIImage* dstImage, RHIImageAspectFlagBits dstFlag, uint32_t width, uint32_t height) = 0;
        // command 拷贝缓冲区
        virtual void cmdCopyBuffer(RHICommandBuffer* commandBuffer, RHIBuffer* srcBuffer, RHIBuffer* dstBuffer, uint32_t regionCount, RHIBufferCopy* pRegions) = 0;
        // command 绘制
        virtual void cmdDraw(RHICommandBuffer* commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) = 0;
        // command 调度计算着色器的运行
        virtual void cmdDispatch(RHICommandBuffer* commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) = 0;
        // command 间接调度计算着色器的运行
        virtual void cmdDispatchIndirect(RHICommandBuffer* commandBuffer, RHIBuffer* buffer, RHIDeviceSize offset) = 0;
        // command 管线屏障
        virtual void cmdPipelineBarrier(RHICommandBuffer* commandBuffer, RHIPipelineStageFlags srcStageMask, RHIPipelineStageFlags dstStageMask, RHIDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const RHIMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const RHIBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const RHIImageMemoryBarrier* pImageMemoryBarriers) = 0;
        // end 命令缓冲的记录
        virtual bool endCommandBuffer(RHICommandBuffer* commandBuffer) = 0;
        // update 描述符集
        virtual void updateDescriptorSets(uint32_t descriptorWriteCount, const RHIWriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount, const RHICopyDescriptorSet* pDescriptorCopies) = 0;
        // queue 提交命令
        virtual bool queueSubmit(RHIQueue* queue, uint32_t submitCount, const RHISubmitInfo* pSubmits, RHIFence* fence) = 0;
        // queue 等待所有命令提交完成
        virtual bool queueWaitIdle(RHIQueue* queue) = 0;
        // reset 命令池
        virtual void resetCommandPool() = 0;
        // wati Fence
        virtual void waitForFences() = 0;

        /* 查询 */
        // get 物理设备属性
        virtual void getPhysicalDeviceProperties(RHIPhysicalDeviceProperties* pProperties) = 0;
        // get 当前命令缓冲区
        virtual RHICommandBuffer* getCurrentCommandBuffer() const = 0;
        // get 命令缓冲区链表
        virtual RHICommandBuffer* const* getCommandBufferList() const = 0;
        // get 命令池
        virtual RHICommandPool* getCommandPool() const = 0;
        // get 描述符池
        virtual RHIDescriptorPool* getDescriptorPool() const = 0;
        // get Fence链表
        virtual RHIFence* const* getFenceList() const = 0;
        // get 队列族索引
        virtual QueueFamilyIndices getQueueFamilyIndices() const = 0;
        // get 图像队列
        virtual RHIQueue* getGraphicsQueue() const = 0;
        // get 计算队列
        virtual RHIQueue* getComputeQueue() const = 0;
        // get 交换链信息
        virtual RHISwapChainDesc getSwapchainInfo() = 0;
        // get 深度图像信息
        virtual RHIDepthImageDesc getDepthImageInfo() const = 0;
        // get 一次flight的最大帧数
        virtual uint8_t getMaxFramesInFlight() const = 0;
        // get 当前帧索引
        virtual uint8_t getCurrentFrameIndex() const = 0;
        // set 当前帧索引
        virtual void setCurrentFrameIndex(uint8_t index) = 0;

        /* 单次命令写入 */
        // begin 单次命令记录
        virtual RHICommandBuffer* beginSingleTimeCommands() = 0;
        // end 单次命令记录
        virtual void endSingleTimeCommands(RHICommandBuffer* command_buffer) = 0;
        // prepare 在渲染流程之前的准备
        virtual bool prepareBeforePass(std::function<void()> passUpdateAfterRecreateSwapchain) = 0;
        // submit 渲染指令
        virtual void submitRendering(std::function<void()> passUpdateAfterRecreateSwapchain) = 0;
        // push event
        virtual void pushEvent(RHICommandBuffer* commond_buffer, const char* name, const float* color) = 0;
        // pop event
        virtual void popEvent(RHICommandBuffer* commond_buffer) = 0;

        /* 销毁 */
        // clear
        virtual void clear() = 0;
        // clear 交换链
        virtual void clearSwapchain() = 0;
        // destroy 默认采样器
        virtual void destroyDefaultSampler(RHIDefaultSamplerType type) = 0;
        // destroy Mipmap采样器
        virtual void destroyMipmappedSampler() = 0;
        // destroy 着色器模块
        virtual void destroyShaderModule(RHIShader* shader) = 0;
        // destroy Semaphore
        virtual void destroySemaphore(RHISemaphore* semaphore) = 0;
        // destory 采样器
        virtual void destroySampler(RHISampler* sampler) = 0;
        // destroy Vulkan实例
        virtual void destroyInstance(RHIInstance* instance) = 0;
        // destroy 图像视图
        virtual void destroyImageView(RHIImageView* imageView) = 0;
        // destroy 图像
        virtual void destroyImage(RHIImage* image) = 0;
        // destroy 帧缓冲
        virtual void destroyFramebuffer(RHIFramebuffer* framebuffer) = 0;
        // destroy Fence
        virtual void destroyFence(RHIFence* fence) = 0;
        // destroy 逻辑设备
        virtual void destroyDevice() = 0;
        // destroy 命令池
        virtual void destroyCommandPool(RHICommandPool* commandPool) = 0;
        // destroy 缓冲区
        virtual void destroyBuffer(RHIBuffer*& buffer) = 0;
        // free 命令缓冲区
        virtual void freeCommandBuffers(RHICommandPool* commandPool, uint32_t commandBufferCount, RHICommandBuffer* pCommandBuffers) = 0;

        /* 内存 */
        // free 内存
        virtual void freeMemory(RHIDeviceMemory*& memory) = 0;
        // 内存映射
        virtual bool mapMemory(RHIDeviceMemory* memory, RHIDeviceSize offset, RHIDeviceSize size, RHIMemoryMapFlags flags, void** ppData) = 0;
        // 内存取消映射
        virtual void unmapMemory(RHIDeviceMemory* memory) = 0;
        // invalidate 映射的内存范围
        virtual void invalidateMappedMemoryRanges(void* pNext, RHIDeviceMemory* memory, RHIDeviceSize offset, RHIDeviceSize size) = 0;
        // flush 映射的内存范围
        virtual void flushMappedMemoryRanges(void* pNext, RHIDeviceMemory* memory, RHIDeviceSize offset, RHIDeviceSize size) = 0;

        /* 信号量 */
        // get 纹理拷贝信号量
        virtual RHISemaphore*& getTextureCopySemaphore(uint32_t index) = 0;

    private:
    };

    inline RHI::~RHI() = default;

} // namespace Piccolo