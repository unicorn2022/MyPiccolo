#pragma once

#include "runtime/function/render/interface/rhi.h"
#include "runtime/function/render/interface/vulkan/vulkan_rhi_resource.h"

#include <vk_mem_alloc.h>
#include <vulkan/vulkan.h>

#include <functional>
#include <map>
#include <vector>

namespace Piccolo {

    class VulkanRHI final : public RHI {
    public:
        // 初始化
        virtual void initialize(RHIInitInfo init_info) override final;
        // 准备 context
        virtual void prepareContext() override final;

        /* 申请 & 创建 */
        // allocate 命令缓冲
        bool allocateCommandBuffers(const RHICommandBufferAllocateInfo* pAllocateInfo, RHICommandBuffer*& pCommandBuffers) override;
        // allocate 描述符集
        bool allocateDescriptorSets(const RHIDescriptorSetAllocateInfo* pAllocateInfo, RHIDescriptorSet*& pDescriptorSets) override;
        // create 交换链
        void createSwapchain() override;
        // recreate 交换链
        void recreateSwapchain() override;
        // create 交换链图像视图
        void createSwapchainImageViews() override;
        // create 深度图像&视图
        void createFramebufferImageAndView() override;
        // get or create 默认采样器
        RHISampler* getOrCreateDefaultSampler(RHIDefaultSamplerType type) override;
        // get or create Mipmap采样器
        RHISampler* getOrCreateMipmapSampler(uint32_t width, uint32_t height) override;
        // create 着色器模块
        RHIShader* createShaderModule(const std::vector<unsigned char>& shader_code) override;
        // create 缓冲区
        void createBuffer(RHIDeviceSize size, RHIBufferUsageFlags usage, RHIMemoryPropertyFlags properties, RHIBuffer*& buffer, RHIDeviceMemory*& buffer_memory) override;
        // create 缓冲区并初始化
        void createBufferAndInitialize(RHIBufferUsageFlags usage, RHIMemoryPropertyFlags properties, RHIBuffer*& buffer, RHIDeviceMemory*& buffer_memory, RHIDeviceSize size, void* data = nullptr, int datasize = 0) override;
        // create 缓冲区内存分配器 (VMA = Vulkan Memory Allocator)
        bool createBufferVMA(VmaAllocator                   allocator,
                             const RHIBufferCreateInfo*     pBufferCreateInfo,
                             const VmaAllocationCreateInfo* pAllocationCreateInfo,
                             RHIBuffer*&                    pBuffer,
                             VmaAllocation*                 pAllocation,
                             VmaAllocationInfo*             pAllocationInfo) override;
        // create 缓冲区对齐内存分配器 (VMA = Vulkan Memory Allocator)
        bool createBufferWithAlignmentVMA(
            VmaAllocator                   allocator,
            const RHIBufferCreateInfo*     pBufferCreateInfo,
            const VmaAllocationCreateInfo* pAllocationCreateInfo,
            RHIDeviceSize                  minAlignment,
            RHIBuffer*&                    pBuffer,
            VmaAllocation*                 pAllocation,
            VmaAllocationInfo*             pAllocationInfo) override;
        // copy 缓冲区内容
        void copyBuffer(RHIBuffer* srcBuffer, RHIBuffer* dstBuffer, RHIDeviceSize srcOffset, RHIDeviceSize dstOffset, RHIDeviceSize size) override;
        // create 图像
        void createImage(uint32_t image_width, uint32_t image_height, RHIFormat format, RHIImageTiling image_tiling, RHIImageUsageFlags image_usage_flags, RHIMemoryPropertyFlags memory_property_flags, RHIImage*& image, RHIDeviceMemory*& memory, RHIImageCreateFlags image_create_flags, uint32_t array_layers, uint32_t miplevels) override;
        // create 图像视图
        void createImageView(RHIImage* image, RHIFormat format, RHIImageAspectFlags image_aspect_flags, RHIImageViewType view_type, uint32_t layout_count, uint32_t miplevels, RHIImageView*& image_view) override;
        // create 全局图像
        void createGlobalImage(RHIImage*& image, RHIImageView*& image_view, VmaAllocation& image_allocation, uint32_t texture_image_width, uint32_t texture_image_height, void* texture_image_pixels, RHIFormat texture_image_format, uint32_t miplevels = 0) override;
        // create 立方贴图
        void createCubeMap(RHIImage*& image, RHIImageView*& image_view, VmaAllocation& image_allocation, uint32_t texture_image_width, uint32_t texture_image_height, std::array<void*, 6> texture_image_pixels, RHIFormat texture_image_format, uint32_t miplevels) override;
        // create 命令池
        bool createCommandPool(const RHICommandPoolCreateInfo* pCreateInfo, RHICommandPool*& pCommandPool) override;
        // create 描述符池
        bool createDescriptorPool(const RHIDescriptorPoolCreateInfo* pCreateInfo, RHIDescriptorPool*& pDescriptorPool) override;
        // create 描述符集布局
        bool createDescriptorSetLayout(const RHIDescriptorSetLayoutCreateInfo* pCreateInfo, RHIDescriptorSetLayout*& pSetLayout) override;
        // create Fence: CPU 和 GPU 之间的同步
        bool createFence(const RHIFenceCreateInfo* pCreateInfo, RHIFence*& pFence) override;
        // create 帧缓冲
        bool createFramebuffer(const RHIFramebufferCreateInfo* pCreateInfo, RHIFramebuffer*& pFramebuffer) override;
        // create 图形渲染管线
        bool createGraphicsPipelines(RHIPipelineCache* pipelineCache, uint32_t createInfoCount, const RHIGraphicsPipelineCreateInfo* pCreateInfos, RHIPipeline*& pPipelines) override;
        // create 计算管线
        bool createComputePipelines(RHIPipelineCache* pipelineCache, uint32_t createInfoCount, const RHIComputePipelineCreateInfo* pCreateInfos, RHIPipeline*& pPipelines) override;
        // create 管线布局
        bool createPipelineLayout(const RHIPipelineLayoutCreateInfo* pCreateInfo, RHIPipelineLayout*& pPipelineLayout) override;
        // create 渲染流程
        bool createRenderPass(const RHIRenderPassCreateInfo* pCreateInfo, RHIRenderPass*& pRenderPass) override;
        // create 采样器
        bool createSampler(const RHISamplerCreateInfo* pCreateInfo, RHISampler*& pSampler) override;
        // create Semaphore: GPU 的 Command Queue 之间的同步                                                                                                                                                                                                                   = 0;
        bool createSemaphore(const RHISemaphoreCreateInfo* pCreateInfo, RHISemaphore*& pSemaphore) override;

        /* 命令缓冲 & 命令写入 */
        // (函数指针)wait Fence
        bool waitForFencesPFN(uint32_t fenceCount, RHIFence* const* pFence, RHIBool32 waitAll, uint64_t timeout) override;
        // (函数指针)reset Fence
        bool resetFencesPFN(uint32_t fenceCount, RHIFence* const* pFences) override;
        // (函数指针)reset 命令池
        bool resetCommandPoolPFN(RHICommandPool* commandPool, RHICommandPoolResetFlags flags) override;
        // (函数指针)begin 命令缓冲的记录
        bool beginCommandBufferPFN(RHICommandBuffer* commandBuffer, const RHICommandBufferBeginInfo* pBeginInfo) override;
        // (函数指针)end 命令缓冲的记录
        bool endCommandBufferPFN(RHICommandBuffer* commandBuffer) override;
        // (函数指针)command 开始渲染流程
        void cmdBeginRenderPassPFN(RHICommandBuffer* commandBuffer, const RHIRenderPassBeginInfo* pRenderPassBegin, RHISubpassContents contents) override;
        // (函数指针)command 下一个子流程
        void cmdNextSubpassPFN(RHICommandBuffer* commandBuffer, RHISubpassContents contents) override;
        // (函数指针)command 结束渲染流程
        void cmdEndRenderPassPFN(RHICommandBuffer* commandBuffer) override;
        // (函数指针)command 绑定渲染管线
        void cmdBindPipelinePFN(RHICommandBuffer* commandBuffer, RHIPipelineBindPoint pipelineBindPoint, RHIPipeline* pipeline) override;
        // (函数指针)command 设置视口
        void cmdSetViewportPFN(RHICommandBuffer* commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const RHIViewport* pViewports) override;
        // (函数指针)command 设置裁剪
        void cmdSetScissorPFN(RHICommandBuffer* commandBuffer, uint32_t firstScissor, uint32_t scissorCount, const RHIRect2D* pScissors) override;
        // (函数指针)command 绑定顶点缓冲
        void cmdBindVertexBuffersPFN(
            RHICommandBuffer*    commandBuffer,
            uint32_t             firstBinding,
            uint32_t             bindingCount,
            RHIBuffer* const*    pBuffers,
            const RHIDeviceSize* pOffsets) override;
        // (函数指针)command 绑定索引缓冲
        void cmdBindIndexBufferPFN(RHICommandBuffer* commandBuffer, RHIBuffer* buffer, RHIDeviceSize offset, RHIIndexType indexType) override;
        // (函数指针)command 绑定描述符集
        void cmdBindDescriptorSetsPFN(
            RHICommandBuffer*              commandBuffer,
            RHIPipelineBindPoint           pipelineBindPoint,
            RHIPipelineLayout*             layout,
            uint32_t                       firstSet,
            uint32_t                       descriptorSetCount,
            const RHIDescriptorSet* const* pDescriptorSets,
            uint32_t                       dynamicOffsetCount,
            const uint32_t*                pDynamicOffsets) override;
        // (函数指针)command 绘制索引
        void cmdDrawIndexedPFN(RHICommandBuffer* commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) override;
        // (函数指针)command 清除附件
        void cmdClearAttachmentsPFN(RHICommandBuffer* commandBuffer, uint32_t attachmentCount, const RHIClearAttachment* pAttachments, uint32_t rectCount, const RHIClearRect* pRects) override;

        // begin 命令缓冲的记录
        bool beginCommandBuffer(RHICommandBuffer* commandBuffer, const RHICommandBufferBeginInfo* pBeginInfo) override;
        // command 将图像拷贝到缓冲区
        void cmdCopyImageToBuffer(RHICommandBuffer* commandBuffer, RHIImage* srcImage, RHIImageLayout srcImageLayout, RHIBuffer* dstBuffer, uint32_t regionCount, const RHIBufferImageCopy* pRegions) override;
        // command 将图像拷贝到图像
        void cmdCopyImageToImage(RHICommandBuffer* commandBuffer, RHIImage* srcImage, RHIImageAspectFlagBits srcFlag, RHIImage* dstImage, RHIImageAspectFlagBits dstFlag, uint32_t width, uint32_t height) override;
        // command 拷贝缓冲区
        void cmdCopyBuffer(RHICommandBuffer* commandBuffer, RHIBuffer* srcBuffer, RHIBuffer* dstBuffer, uint32_t regionCount, RHIBufferCopy* pRegions) override;
        // command 绘制
        void cmdDraw(RHICommandBuffer* commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) override;
        // command 调度计算着色器的运行
        void cmdDispatch(RHICommandBuffer* commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) override;
        // command 间接调度计算着色器的运行
        void cmdDispatchIndirect(RHICommandBuffer* commandBuffer, RHIBuffer* buffer, RHIDeviceSize offset) override;
        // command 管线屏障
        void cmdPipelineBarrier(RHICommandBuffer* commandBuffer, RHIPipelineStageFlags srcStageMask, RHIPipelineStageFlags dstStageMask, RHIDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const RHIMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const RHIBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const RHIImageMemoryBarrier* pImageMemoryBarriers) override;
        // end 命令缓冲的记录
        bool endCommandBuffer(RHICommandBuffer* commandBuffer) override;
        // update 描述符集
        void updateDescriptorSets(uint32_t descriptorWriteCount, const RHIWriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount, const RHICopyDescriptorSet* pDescriptorCopies) override;
        // queue 提交命令
        bool queueSubmit(RHIQueue* queue, uint32_t submitCount, const RHISubmitInfo* pSubmits, RHIFence* fence) override;
        // queue 等待所有命令提交完成
        bool queueWaitIdle(RHIQueue* queue) override;
        // reset 命令池
        void resetCommandPool() override;
        // wati Fence
        void waitForFences() override;
        // wati Fence
        bool waitForFences(uint32_t fenceCount, const RHIFence* const* pFences, RHIBool32 waitAll, uint64_t timeout);

        /* 查询 */
        // get 物理设备属性
        void getPhysicalDeviceProperties(RHIPhysicalDeviceProperties* pProperties) override;
        // get 当前命令缓冲区
        RHICommandBuffer* getCurrentCommandBuffer() const override;
        // get 命令缓冲区链表
        RHICommandBuffer* const* getCommandBufferList() const override;
        // get 命令池
        RHICommandPool* getCommandPool() const override;
        // get 描述符池
        RHIDescriptorPool* getDescriptorPool() const override;
        // get Fence链表
        RHIFence* const* getFenceList() const override;
        // get 队列族索引
        QueueFamilyIndices getQueueFamilyIndices() const override;
        // get 图像队列
        RHIQueue* getGraphicsQueue() const override;
        // get 计算队列
        RHIQueue* getComputeQueue() const override;
        // get 交换链信息
        RHISwapChainDesc getSwapchainInfo() override;
        // get 深度图像信息
        RHIDepthImageDesc getDepthImageInfo() const override;
        // get 一次flight的最大帧数
        uint8_t getMaxFramesInFlight() const override;
        // get 当前帧索引
        uint8_t getCurrentFrameIndex() const override;
        // set 当前帧索引
        void setCurrentFrameIndex(uint8_t index) override;

        /* 单次命令写入 */
        // begin 单次命令记录
        RHICommandBuffer* beginSingleTimeCommands() override;
        // end 单次命令记录
        void endSingleTimeCommands(RHICommandBuffer* command_buffer) override;
        // prepare 在渲染流程之前的准备
        bool prepareBeforePass(std::function<void()> passUpdateAfterRecreateSwapchain) override;
        // submit 渲染指令
        void submitRendering(std::function<void()> passUpdateAfterRecreateSwapchain) override;
        // push event
        void pushEvent(RHICommandBuffer* commond_buffer, const char* name, const float* color) override;
        // pop event
        void popEvent(RHICommandBuffer* commond_buffer) override;

        /* 销毁 */
        virtual ~VulkanRHI() override final;
        // clear
        void clear() override;
        // clear 交换链
        void clearSwapchain() override;
        // destroy 默认采样器
        void destroyDefaultSampler(RHIDefaultSamplerType type) override;
        // destroy Mipmap采样器
        void destroyMipmappedSampler() override;
        // destroy 着色器模块
        void destroyShaderModule(RHIShader* shader) override;
        // destroy Semaphore
        void destroySemaphore(RHISemaphore* semaphore) override;
        // destory 采样器
        void destroySampler(RHISampler* sampler) override;
        // destroy Vulkan实例
        void destroyInstance(RHIInstance* instance) override;
        // destroy 图像视图
        void destroyImageView(RHIImageView* imageView) override;
        // destroy 图像
        void destroyImage(RHIImage* image) override;
        // destroy 帧缓冲
        void destroyFramebuffer(RHIFramebuffer* framebuffer) override;
        // destroy Fence
        void destroyFence(RHIFence* fence) override;
        // destroy 逻辑设备
        void destroyDevice() override;
        // destroy 命令池
        void destroyCommandPool(RHICommandPool* commandPool) override;
        // destroy 缓冲区
        void destroyBuffer(RHIBuffer*& buffer) override;
        // free 命令缓冲区
        void freeCommandBuffers(RHICommandPool* commandPool, uint32_t commandBufferCount, RHICommandBuffer* pCommandBuffers) override;

        /* 内存 */
        // free 内存
        void freeMemory(RHIDeviceMemory*& memory) override;
        // 内存映射
        bool mapMemory(RHIDeviceMemory* memory, RHIDeviceSize offset, RHIDeviceSize size, RHIMemoryMapFlags flags, void** ppData) override;
        // 内存取消映射
        void unmapMemory(RHIDeviceMemory* memory) override;
        // invalidate 映射的内存范围
        void invalidateMappedMemoryRanges(void* pNext, RHIDeviceMemory* memory, RHIDeviceSize offset, RHIDeviceSize size) override;
        // flush 映射的内存范围
        void flushMappedMemoryRanges(void* pNext, RHIDeviceMemory* memory, RHIDeviceSize offset, RHIDeviceSize size) override;

        /* 信号量 */
        // get 纹理拷贝信号量
        RHISemaphore*& getTextureCopySemaphore(uint32_t index) override;

    public:
        // flight最大帧数
        static uint8_t const k_max_frames_in_flight {3};

        // 图形渲染队列
        RHIQueue* m_graphics_queue {nullptr};
        // 计算队列
        RHIQueue* m_compute_queue {nullptr};

        // 交换链图像格式
        RHIFormat m_swapchain_image_format {RHI_FORMAT_UNDEFINED};
        // 交换链图像视图
        std::vector<RHIImageView*> m_swapchain_imageviews;
        // 交换链尺寸
        RHIExtent2D m_swapchain_extent;
        // 视图
        RHIViewport m_viewport;
        // 裁剪
        RHIRect2D m_scissor;

        // 深度图像格式
        RHIFormat m_depth_image_format {RHI_FORMAT_UNDEFINED};
        // 深度图像视图
        RHIImageView* m_depth_image_view = new VulkanImageView();

        // Fence(每帧一个)
        RHIFence* m_rhi_is_frame_in_flight_fences[k_max_frames_in_flight];

        // 描述符池
        RHIDescriptorPool* m_descriptor_pool = new VulkanDescriptorPool();

        // 指令池
        RHICommandPool* m_rhi_command_pool;

        // 指令缓冲(每帧一个)
        RHICommandBuffer* m_command_buffers[k_max_frames_in_flight];
        // 当前帧指令缓冲
        RHICommandBuffer* m_current_command_buffer = new VulkanCommandBuffer();

        // 队列族索引
        QueueFamilyIndices m_queue_indices;

        // GLFW窗口
        GLFWwindow* m_window {nullptr};
        // vulkan实例
        VkInstance m_instance {nullptr};
        // vulkan表面
        VkSurfaceKHR m_surface {nullptr};
        // 物理设备
        VkPhysicalDevice m_physical_device {nullptr};
        // 逻辑设备
        VkDevice m_device {nullptr};
        // 显示队列
        VkQueue m_present_queue {nullptr};

        // 交换链
        VkSwapchainKHR m_swapchain {nullptr};
        // 交换链中的图像
        std::vector<VkImage> m_swapchain_images;

        // 深度图像
        RHIImage* m_depth_image = new VulkanImage();
        // 深度图像内存
        VkDeviceMemory m_depth_image_memory {nullptr};

        // 交换链帧缓冲
        std::vector<VkFramebuffer> m_swapchain_framebuffers;

        // VMA内存分配器
        VmaAllocator m_assets_allocator;

        /* 函数指针 */
        // command 开始调试
        PFN_vkCmdBeginDebugUtilsLabelEXT _vkCmdBeginDebugUtilsLabelEXT;
        // command 结束调试
        PFN_vkCmdEndDebugUtilsLabelEXT _vkCmdEndDebugUtilsLabelEXT;
        // wait fence
        PFN_vkWaitForFences _vkWaitForFences;
        // reset fence
        PFN_vkResetFences _vkResetFences;
        // reset 命令池
        PFN_vkResetCommandPool _vkResetCommandPool;
        // begin 命令缓冲
        PFN_vkBeginCommandBuffer _vkBeginCommandBuffer;
        // end 命令缓冲
        PFN_vkEndCommandBuffer _vkEndCommandBuffer;
        // command 开始渲染流程
        PFN_vkCmdBeginRenderPass _vkCmdBeginRenderPass;
        // command 下一个子流程
        PFN_vkCmdNextSubpass _vkCmdNextSubpass;
        // command 结束渲染流程
        PFN_vkCmdEndRenderPass _vkCmdEndRenderPass;
        // command 绑定渲染管线
        PFN_vkCmdBindPipeline _vkCmdBindPipeline;
        // command 设置视口
        PFN_vkCmdSetViewport _vkCmdSetViewport;
        // command 设置裁剪
        PFN_vkCmdSetScissor _vkCmdSetScissor;
        // command 绑定顶点缓冲
        PFN_vkCmdBindVertexBuffers _vkCmdBindVertexBuffers;
        // command 绑定索引缓冲
        PFN_vkCmdBindIndexBuffer _vkCmdBindIndexBuffer;
        // command 绑定描述符集
        PFN_vkCmdBindDescriptorSets _vkCmdBindDescriptorSets;
        // command 绘制索引
        PFN_vkCmdDrawIndexed _vkCmdDrawIndexed;
        // command 清除附件
        PFN_vkCmdClearAttachments _vkCmdClearAttachments;

        // 全局描述符池
        VkDescriptorPool m_vk_descriptor_pool;

        // 当前帧编号
        uint8_t m_current_frame_index {0};
        // 命令池
        VkCommandPool m_command_pools[k_max_frames_in_flight];
        // 命令缓冲
        // semaphore: 等待图像可用
        VkCommandBuffer m_vk_command_buffers[k_max_frames_in_flight];
        VkSemaphore     m_image_available_for_render_semaphores[k_max_frames_in_flight];
        // semaphore: 等待图像显示完成
        VkSemaphore m_image_finished_for_presentation_semaphores[k_max_frames_in_flight];
        // semaphore: 等待图像可以完成
        RHISemaphore* m_image_available_for_texturescopy_semaphores[k_max_frames_in_flight];
        // fence: 等待帧完成
        VkFence m_is_frame_in_flight_fences[k_max_frames_in_flight];

        // TODO: set
        // 当前命令缓冲
        VkCommandBuffer m_vk_current_command_buffer;

        // 当前交换链图像索引
        uint32_t m_current_swapchain_image_index;

    private:
        // 启用的vulkan层
        const std::vector<char const*> m_validation_layers {"VK_LAYER_KHRONOS_validation"};
        // vulkan api版本号
        uint32_t m_vulkan_api_version {VK_API_VERSION_1_0};

        // 启用的vulkan扩展
        std::vector<char const*> m_device_extensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

        // default sampler cache
        // Linear采样器
        RHISampler* m_linear_sampler = nullptr;
        // Nearest采样器
        RHISampler* m_nearest_sampler = nullptr;
        // Mipmap采样器
        std::map<uint32_t, RHISampler*> m_mipmap_sampler_map;

    private:
        // 创建vulkan实例
        void createInstance();
        // 初始化调试信息
        void initializeDebugMessenger();
        // 创建窗口
        void createWindowSurface();
        // 初始化物理设备
        void initializePhysicalDevice();
        // 创建逻辑设备
        void createLogicalDevice();
        // 创建命令池
        void createCommandPool() override;

        // 创建命令缓冲
        void createCommandBuffers();
        // 创建描述符池
        void createDescriptorPool();
        // 创建同步信号量
        void createSyncPrimitives();
        // 创建资产分配器
        void createAssetAllocator();

    public:
        // 是否允许点光源阴影
        bool isPointLightShadowEnabled() override;

    private:
        // 是否启用验证层
        bool m_enable_validation_Layers {true};
        // 是否启用调试工具
        bool m_enable_debug_utils_label {true};
        // 是否启用点光源阴影
        bool m_enable_point_light_shadow {true};

        // used in descriptor pool creation
        // 最大定点混合mesh数目
        uint32_t m_max_vertex_blending_mesh_count {256};
        // 最大材质数目
        uint32_t m_max_material_count {256};

        // 检查是否支持验证层
        bool checkValidationLayerSupport();
        // 获取需要的扩展
        std::vector<const char*> getRequiredExtensions();
        // 填充debug messenger创建信息
        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

        // 调试工具: 信息管理
        VkDebugUtilsMessengerEXT m_debug_messenger = nullptr;
        // 创建调试工具: 信息管理扩展
        VkResult createDebugUtilsMessengerEXT(VkInstance                                instance,
                                              const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                              const VkAllocationCallbacks*              pAllocator,
                                              VkDebugUtilsMessengerEXT*                 pDebugMessenger);
        // 销毁调试工具: 信息管理扩展
        void destroyDebugUtilsMessengerEXT(VkInstance                   instance,
                                           VkDebugUtilsMessengerEXT     debugMessenger,
                                           const VkAllocationCallbacks* pAllocator);

        // 查询当前物理设备的指令队列族
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice physical_device);
        // 检查当前物理设备是否支持需要的扩展
        bool checkDeviceExtensionSupport(VkPhysicalDevice physical_device);
        // 检查当前物理设备是否支持逻辑设备
        bool isDeviceSuitable(VkPhysicalDevice physical_device);
        // 检查交换链支持
        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice physical_device);

        // 查询深度图像格式
        VkFormat findDepthFormat();
        // 查询支持的格式
        VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates,
                                     VkImageTiling                tiling,
                                     VkFormatFeatureFlags         features);

        // 选择交换链表面格式
        VkSurfaceFormatKHR chooseSwapchainSurfaceFormatFromDetails(const std::vector<VkSurfaceFormatKHR>& available_surface_formats);
        // 选择交换链显示格式
        VkPresentModeKHR chooseSwapchainPresentModeFromDetails(const std::vector<VkPresentModeKHR>& available_present_modes);
        // 选择交换链图像分辨率
        VkExtent2D chooseSwapchainExtentFromDetails(const VkSurfaceCapabilitiesKHR& capabilities);
    };
} // namespace Piccolo
