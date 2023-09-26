/**
 * @file rhi_struct.h
 * @brief RHI硬件接口类型定义
 */
#pragma once

#include "runtime/function/render/render_type.h"
#include <optional>

namespace Piccolo {
    /////////////////////////////////////////////////
    // 删除指针
#define RHI_DELETE_PTR(x) \
    delete (x);           \
    (x) = nullptr;

    //////////////////// 类型 ////////////////////////
    class RHIInstance {};            // 实例
    class RHIPhysicalDevice {};      // 物理设备
    class RHIDevice {};              // 逻辑设备
    class RHIDeviceMemory {};        // 设备内存
    class RHIQueue {};               // 指令队列
    class RHIImage {};               // image
    class RHIImageView {};           // image视图
    class RHIRenderPass {};          // 渲染流程
    class RHIPipeline {};            // 渲染管线
    class RHIPipelineCache {};       // 渲染管线缓冲
    class RHIPipelineLayout {};      // 渲染管线布局
    class RHIBuffer {};              // 缓冲区
    class RHIBufferView {};          // 缓冲区视图
    class RHIFramebuffer {};         // 帧缓冲
    class RHICommandPool {};         // 命令池
    class RHICommandBuffer {};       // 命令缓冲区
    class RHIFence {};               // Fence: CPU 和 GPU 之间的同步
    class RHISemaphore {};           // Semaphore: GPU 的 Command Queue 之间的同步
    class RHIEvent {};               // Event: CPU 和 GPU 之间发送信号进行同步
    class RHIShader {};              // 着色器
    class RHISampler {};             // 采样器
    class RHIDescriptorPool {};      // 描述符池
    class RHIDescriptorSet {};       // 描述符集
    class RHIDescriptorSetLayout {}; // 描述符集布局

    //////////////////// 结构体声明 //////////////////////////
    struct RHIMemoryBarrier;                        // memory barrier
    struct RHIBufferMemoryBarrier;                  // buffer memory barrier
    struct RHIImageMemoryBarrier;                   // image memory barrier
    struct RHICopyDescriptorSet;                    // 描述符集 拷贝
    struct RHIDescriptorImageInfo;                  // image描述符 信息
    struct RHIDescriptorBufferInfo;                 // buffer描述符 信息
    struct RHIOffset2D;                             // 2D偏移
    struct RHISpecializationMapEntry;               // 专用化映射条目
    struct RHIImageSubresourceRange;                // image子资源范围
    struct RHIExtent2D;                             // 2D矩形
    struct RHIExtent3D;                             // 3D立方体
    struct RHIApplicationInfo;                      // 应用 信息
    struct RHIAttachmentDescription;                // 附件 描述
    struct RHIBufferCopy;                           // buffer 拷贝
    struct RHIBufferCreateInfo;                     // buffer 创建信息
    struct RHIBufferImageCopy;                      // buffer image拷贝
    struct RHICommandBufferAllocateInfo;            // 指令缓冲 申请信息
    struct RHICommandBufferBeginInfo;               // 指令缓冲 开始信息
    struct RHICommandBufferInheritanceInfo;         // 指令缓冲 继承信息
    struct RHICommandPoolCreateInfo;                // 指令缓冲池 创建信息
    struct RHIDescriptorPoolSize;                   // 描述符池 大小
    struct RHIDescriptorPoolCreateInfo;             // 描述符池 创建信息
    struct RHIDescriptorSetAllocateInfo;            // 描述符集 申请信息
    struct RHIDescriptorSetLayoutBinding;           // 描述符集 binding布局
    struct RHIDescriptorSetLayoutCreateInfo;        // 描述符集 布局创建信息
    struct RHIDeviceCreateInfo;                     // 逻辑设备 创建信息
    struct RHIDeviceQueueCreateInfo;                // 逻辑设备队列 创建信息
    struct RHIExtensionProperties;                  // 扩展 特性
    struct RHIFenceCreateInfo;                      // Fence 创建信息
    struct RHIFormatProperties;                     // 格式 特性
    struct RHIFramebufferCreateInfo;                // 帧缓冲 创建信息
    struct RHIGraphicsPipelineCreateInfo;           // 图形渲染管线 创建信息
    struct RHIComputePipelineCreateInfo;            // 计算管线 创建信息
    struct RHIImageBlit;                            // image blit
    struct RHIImageCreateInfo;                      // image 创建信息
    struct RHIImageFormatProperties;                // image 格式特性
    struct RHIImageViewCreateInfo;                  // image_view 创建信息
    struct RHIInstanceCreateInfo;                   // instance 创建信息
    struct RHILayerProperties;                      // layer 特性
    struct RHIMemoryAllocateInfo;                   // memory 申请信息
    struct RHIMemoryHeap;                           // memory 堆
    struct RHIMemoryRequirements;                   // memory 需求
    struct RHIMemoryType;                           // memory 类型
    struct RHIPhysicalDeviceFeatures;               // 物理设备 特性
    struct RHIPhysicalDeviceLimits;                 // 物理设备 限制
    struct RHIPhysicalDeviceMemoryProperties;       // 物理设备 内存特性
    struct RHIPhysicalDeviceProperties;             // 物理设备 特性
    struct RHIPhysicalDeviceSparseProperties;       // 物理设备 稀疏特性
    struct RHIPipelineColorBlendStateCreateInfo;    // 渲染管线 颜色混合 创建信息
    struct RHIPipelineDepthStencilStateCreateInfo;  // 渲染管线 深度&模板检测 创建信息
    struct RHIPipelineDynamicStateCreateInfo;       // 渲染管线 动态状态 创建信息
    struct RHIPipelineInputAssemblyStateCreateInfo; // 渲染管线 顶点聚集 创建信息
    struct RHIPipelineLayoutCreateInfo;             // 渲染管线 布局 创建信息
    struct RHIPipelineMultisampleStateCreateInfo;   // 渲染管线 多重采样 创建信息
    struct RHIPipelineRasterizationStateCreateInfo; // 渲染管线 光栅化 创建信息
    struct RHIPipelineShaderStageCreateInfo;        // 渲染管线 着色器阶段 创建信息
    struct RHIPipelineTessellationStateCreateInfo;  // 渲染管线 曲面细分 创建信息
    struct RHIPipelineVertexInputStateCreateInfo;   // 渲染管线 顶点输入 创建信息
    struct RHIPipelineViewportStateCreateInfo;      // 渲染管线 视口 创建信息
    struct RHIPushConstantRange;                    // push_constant 范围
    struct RHIQueueFamilyProperties;                // queue_family 特性
    struct RHIRenderPassCreateInfo;                 // 渲染流程 创建信息
    struct RHISamplerCreateInfo;                    // 采样器 创建信息
    struct RHISemaphoreCreateInfo;                  // Semaphore 创建信息
    struct RHIShaderModuleCreateInfo;               // 着色器模块 创建信息
    struct RHISubmitInfo;                           // 提交 信息
    struct RHISubpassDependency;                    // 子流程 依赖
    struct RHISubpassDescription;                   // 子流程 描述
    struct RHIWriteDescriptorSet;                   // 描述符集 写入
    struct RHIOffset3D;                             // 3D偏移
    struct RHIAttachmentReference;                  // 附件 引用
    struct RHIComponentMapping;                     // 组件映射
    struct RHIImageSubresourceLayers;               // image子资源层
    struct RHIPipelineColorBlendAttachmentState;    // 渲染管线 颜色混合 附件状态
    struct RHIRect2D;                               // 2D矩形
    struct RHISpecializationInfo;                   // 专用化信息
    struct RHIStencilOpState;                       // 模板操作状态
    struct RHIVertexInputAttributeDescription;      // 顶点输入 属性描述
    struct RHIVertexInputBindingDescription;        // 顶点输入 绑定描述
    struct RHIViewport;                             // 视口
    struct RHIRenderPassBeginInfo;                  // 渲染流程 开始信息
    union RHIClearValue;                            // 清除值
    union RHIClearColorValue;                       // 清除颜色值
    struct RHIClearDepthStencilValue;               // 清除深度模板值

    //////////////////// 结构体定义 ////////////////////////
    struct RHIMemoryBarrier {
        RHIStructureType sType;
        const void*      pNext;
        RHIAccessFlags   srcAccessMask;
        RHIAccessFlags   dstAccessMask;
    };

    struct RHICopyDescriptorSet {
        RHIStructureType  sType;
        const void*       pNext;
        RHIDescriptorSet* srcSet;
        uint32_t          srcBinding;
        uint32_t          srcArrayElement;
        RHIDescriptorSet* dstSet;
        uint32_t          dstBinding;
        uint32_t          dstArrayElement;
        uint32_t          descriptorCount;
    };

    struct RHIDescriptorImageInfo {
        RHISampler*    sampler;
        RHIImageView*  imageView;
        RHIImageLayout imageLayout;
    };

    struct RHIDescriptorBufferInfo {
        RHIBuffer*    buffer;
        RHIDeviceSize offset;
        RHIDeviceSize range;
    };

    struct RHIOffset2D {
        int32_t x;
        int32_t y;
    };

    struct RHISpecializationMapEntry {
        uint32_t constantID;
        uint32_t offset;
        size_t   size;
    };

    struct RHIBufferMemoryBarrier {
        RHIStructureType sType;
        const void*      pNext;
        RHIAccessFlags   srcAccessMask;
        RHIAccessFlags   dstAccessMask;
        uint32_t         srcQueueFamilyIndex;
        uint32_t         dstQueueFamilyIndex;
        RHIBuffer*       buffer;
        RHIDeviceSize    offset;
        RHIDeviceSize    size;
    };

    struct RHIImageSubresourceRange {
        RHIImageAspectFlags aspectMask;
        uint32_t            baseMipLevel;
        uint32_t            levelCount;
        uint32_t            baseArrayLayer;
        uint32_t            layerCount;
    };

    struct RHIImageMemoryBarrier {
        RHIStructureType         sType;
        const void*              pNext;
        RHIAccessFlags           srcAccessMask;
        RHIAccessFlags           dstAccessMask;
        RHIImageLayout           oldLayout;
        RHIImageLayout           newLayout;
        uint32_t                 srcQueueFamilyIndex;
        uint32_t                 dstQueueFamilyIndex;
        RHIImage*                image;
        RHIImageSubresourceRange subresourceRange;
    };

    struct RHIExtent2D {
        uint32_t width;
        uint32_t height;
    };

    struct RHIExtent3D {
        uint32_t width;
        uint32_t height;
        uint32_t depth;
    };

    struct RHIApplicationInfo {
        RHIStructureType sType;
        const void*      pNext;
        const char*      pApplicationName;
        uint32_t         applicationVersion;
        const char*      pEngineName;
        uint32_t         engineVersion;
        uint32_t         apiVersion;
    };

    struct RHIAttachmentDescription {
        RHIAttachmentDescriptionFlags flags;
        RHIFormat                     format;
        RHISampleCountFlagBits        samples;
        RHIAttachmentLoadOp           loadOp;
        RHIAttachmentStoreOp          storeOp;
        RHIAttachmentLoadOp           stencilLoadOp;
        RHIAttachmentStoreOp          stencilStoreOp;
        RHIImageLayout                initialLayout;
        RHIImageLayout                finalLayout;
    };

    struct RHIBufferCopy {
        RHIDeviceSize srcOffset;
        RHIDeviceSize dstOffset;
        RHIDeviceSize size;
    };

    struct RHIBufferCreateInfo {
        RHIStructureType     sType;
        const void*          pNext;
        RHIBufferCreateFlags flags;
        RHIDeviceSize        size;
        RHIBufferUsageFlags  usage;
        RHISharingMode       sharingMode;
        uint32_t             queueFamilyIndexCount;
        const uint32_t*      pQueueFamilyIndices;
    };

    struct RHIOffset3D {
        int32_t x;
        int32_t y;
        int32_t z;
    };

    struct RHIImageSubresourceLayers {
        RHIImageAspectFlags aspectMask;
        uint32_t            mipLevel;
        uint32_t            baseArrayLayer;
        uint32_t            layerCount;
    };

    struct RHIBufferImageCopy {
        RHIDeviceSize             bufferOffset;
        uint32_t                  bufferRowLength;
        uint32_t                  bufferImageHeight;
        RHIImageSubresourceLayers imageSubresource;
        RHIOffset3D               imageOffset;
        RHIExtent3D               imageExtent;
    };

    struct RHICommandBufferAllocateInfo {
        RHIStructureType      sType;
        const void*           pNext;
        RHICommandPool*       commandPool;
        RHICommandBufferLevel level;
        uint32_t              commandBufferCount;
    };

    struct RHICommandBufferBeginInfo {
        RHIStructureType                       sType;
        const void*                            pNext;
        RHICommandBufferUsageFlags             flags;
        const RHICommandBufferInheritanceInfo* pInheritanceInfo;
    };

    struct RHICommandBufferInheritanceInfo {
        RHIStructureType               sType;
        const void*                    pNext;
        RHIRenderPass*                 renderPass;
        uint32_t                       subpass;
        RHIFramebuffer*                framebuffer;
        RHIBool32                      occlusionQueryEnable;
        RHIQueryControlFlags           queryFlags;
        RHIQueryPipelineStatisticFlags pipelineStatistics;
    };

    struct RHICommandPoolCreateInfo {
        RHIStructureType          sType;
        const void*               pNext;
        RHICommandPoolCreateFlags flags;
        uint32_t                  queueFamilyIndex;
    };

    struct RHIDescriptorPoolSize {
        RHIDescriptorType type;
        uint32_t          descriptorCount;
    };

    struct RHIDescriptorPoolCreateInfo {
        RHIStructureType             sType;
        const void*                  pNext;
        RHIDescriptorPoolCreateFlags flags;
        uint32_t                     maxSets;
        uint32_t                     poolSizeCount;
        const RHIDescriptorPoolSize* pPoolSizes;
    };

    struct RHIDescriptorSetAllocateInfo {
        RHIStructureType                     sType;
        const void*                          pNext;
        RHIDescriptorPool*                   descriptorPool;
        uint32_t                             descriptorSetCount;
        const RHIDescriptorSetLayout* const* pSetLayouts;
    };

    struct RHIDescriptorSetLayoutBinding {
        uint32_t            binding;
        RHIDescriptorType   descriptorType;
        uint32_t            descriptorCount;
        RHIShaderStageFlags stageFlags;
        RHISampler* const*  pImmutableSamplers = nullptr;
    };

    struct RHIDescriptorSetLayoutCreateInfo {
        RHIStructureType                     sType;
        const void*                          pNext;
        RHIDescriptorSetLayoutCreateFlags    flags;
        uint32_t                             bindingCount;
        const RHIDescriptorSetLayoutBinding* pBindings;
    };

    struct RHIDeviceCreateInfo {
        RHIStructureType                  sType;
        const void*                       pNext;
        RHIDeviceCreateFlags              flags;
        uint32_t                          queueCreateInfoCount;
        const RHIDeviceQueueCreateInfo**  pQueueCreateInfos;
        uint32_t                          enabledLayerCount;
        const char* const*                ppEnabledLayerNames;
        uint32_t                          enabledExtensionCount;
        const char* const*                ppEnabledExtensionNames;
        const RHIPhysicalDeviceFeatures** pEnabledFeatures;
    };

    struct RHIDeviceQueueCreateInfo {
        RHIStructureType          sType;
        const void*               pNext;
        RHIDeviceQueueCreateFlags flags;
        uint32_t                  queueFamilyIndex;
        uint32_t                  queueCount;
        const float*              pQueuePriorities;
    };

    struct RHIExtensionProperties {
        char     extensionName[RHI_MAX_EXTENSION_NAME_SIZE];
        uint32_t specVersion;
    };

    struct RHIFenceCreateInfo {
        RHIStructureType    sType;
        const void*         pNext;
        RHIFenceCreateFlags flags;
    };

    struct RHIFormatProperties {
        RHIFormatFeatureFlags linearTilingFeatures;
        RHIFormatFeatureFlags optimalTilingFeatures;
        RHIFormatFeatureFlags bufferFeatures;
    };

    struct RHIFramebufferCreateInfo {
        RHIStructureType          sType;
        const void*               pNext;
        RHIFramebufferCreateFlags flags;
        RHIRenderPass*            renderPass;
        uint32_t                  attachmentCount;
        RHIImageView* const*      pAttachments;
        uint32_t                  width;
        uint32_t                  height;
        uint32_t                  layers;
    };

    struct RHIGraphicsPipelineCreateInfo {
        RHIStructureType                               sType;
        const void*                                    pNext;
        RHIPipelineCreateFlags                         flags;
        uint32_t                                       stageCount;
        const RHIPipelineShaderStageCreateInfo*        pStages;
        const RHIPipelineVertexInputStateCreateInfo*   pVertexInputState;
        const RHIPipelineInputAssemblyStateCreateInfo* pInputAssemblyState;
        const RHIPipelineTessellationStateCreateInfo*  pTessellationState;
        const RHIPipelineViewportStateCreateInfo*      pViewportState;
        const RHIPipelineRasterizationStateCreateInfo* pRasterizationState;
        const RHIPipelineMultisampleStateCreateInfo*   pMultisampleState;
        const RHIPipelineDepthStencilStateCreateInfo*  pDepthStencilState;
        const RHIPipelineColorBlendStateCreateInfo*    pColorBlendState;
        const RHIPipelineDynamicStateCreateInfo*       pDynamicState;
        RHIPipelineLayout*                             layout;
        RHIRenderPass*                                 renderPass;
        uint32_t                                       subpass;
        RHIPipeline*                                   basePipelineHandle;
        int32_t                                        basePipelineIndex;
    };

    struct RHIComputePipelineCreateInfo {
        RHIStructureType                  sType;
        const void*                       pNext;
        RHIPipelineCreateFlags            flags;
        RHIPipelineShaderStageCreateInfo* pStages;
        RHIPipelineLayout*                layout;
        RHIPipeline*                      basePipelineHandle;
        int32_t                           basePipelineIndex;
    };

    struct RHIImageBlit {
        RHIImageSubresourceLayers srcSubresource;
        RHIOffset3D               srcOffsets[2];
        RHIImageSubresourceLayers dstSubresource;
        RHIOffset3D               dstOffsets[2];
    };

    struct RHIImageCreateInfo {
        RHIStructureType       sType;
        const void*            pNext;
        RHIImageCreateFlags    flags;
        RHIImageType           imageType;
        RHIFormat              format;
        RHIExtent3D            extent;
        uint32_t               mipLevels;
        uint32_t               arrayLayers;
        RHISampleCountFlagBits samples;
        RHIImageTiling         tiling;
        RHIImageUsageFlags     usage;
        RHISharingMode         sharingMode;
        uint32_t               queueFamilyIndexCount;
        const uint32_t*        pQueueFamilyIndices;
        RHIImageLayout         initialLayout;
    };

    struct RHIImageFormatProperties {
        RHIExtent3D         maxExtent;
        uint32_t            maxMipLevels;
        uint32_t            maxArrayLayers;
        RHISampleCountFlags sampleCounts;
        RHIDeviceSize       maxResourceSize;
    };

    struct RHIComponentMapping {
        RHIComponentSwizzle r;
        RHIComponentSwizzle g;
        RHIComponentSwizzle b;
        RHIComponentSwizzle a;
    };

    struct RHIImageViewCreateInfo {
        RHIStructureType         sType;
        const void*              pNext;
        RHIImageViewCreateFlags  flags;
        RHIImage*                image;
        RHIImageViewType         viewType;
        RHIFormat                format;
        RHIComponentMapping      components;
        RHIImageSubresourceRange subresourceRange;
    };

    struct RHIInstanceCreateInfo {
        RHIStructureType           sType;
        const void*                pNext;
        RHIInstanceCreateFlags     flags;
        const RHIApplicationInfo** pApplicationInfo;
        uint32_t                   enabledLayerCount;
        const char* const*         ppEnabledLayerNames;
        uint32_t                   enabledExtensionCount;
        const char* const*         ppEnabledExtensionNames;
    };

    struct RHILayerProperties {
        char     layerName[RHI_MAX_EXTENSION_NAME_SIZE];
        uint32_t specVersion;
        uint32_t implementationVersion;
        char     description[RHI_MAX_DESCRIPTION_SIZE];
    };

    struct RHIMemoryAllocateInfo {
        RHIStructureType sType;
        const void*      pNext;
        RHIDeviceSize    allocationSize;
        uint32_t         memoryTypeIndex;
    };

    struct RHIMemoryHeap {
        RHIDeviceSize      size;
        RHIMemoryHeapFlags flags;
    };

    struct RHIMemoryRequirements {
        RHIDeviceSize size;
        RHIDeviceSize alignment;
        uint32_t      memoryTypeBits;
    };

    struct RHIMemoryType {
        RHIMemoryPropertyFlags propertyFlags;
        uint32_t               heapIndex;
    };

    struct RHIPhysicalDeviceFeatures {
        RHIBool32 robustBufferAccess;
        RHIBool32 fullDrawIndexUint32;
        RHIBool32 imageCubeArray;
        RHIBool32 independentBlend;
        RHIBool32 geometryShader;
        RHIBool32 tessellationShader;
        RHIBool32 sampleRateShading;
        RHIBool32 dualSrcBlend;
        RHIBool32 logicOp;
        RHIBool32 multiDrawIndirect;
        RHIBool32 drawIndirectFirstInstance;
        RHIBool32 depthClamp;
        RHIBool32 depthBiasClamp;
        RHIBool32 fillModeNonSolid;
        RHIBool32 depthBounds;
        RHIBool32 wideLines;
        RHIBool32 largePoints;
        RHIBool32 alphaToOne;
        RHIBool32 multiViewport;
        RHIBool32 samplerAnisotropy;
        RHIBool32 textureCompressionETC2;
        RHIBool32 textureCompressionASTC_LDR;
        RHIBool32 textureCompressionBC;
        RHIBool32 occlusionQueryPrecise;
        RHIBool32 pipelineStatisticsQuery;
        RHIBool32 vertexPipelineStoresAndAtomics;
        RHIBool32 fragmentStoresAndAtomics;
        RHIBool32 shaderTessellationAndGeometryPointSize;
        RHIBool32 shaderImageGatherExtended;
        RHIBool32 shaderStorageImageExtendedFormats;
        RHIBool32 shaderStorageImageMultisample;
        RHIBool32 shaderStorageImageReadWithoutFormat;
        RHIBool32 shaderStorageImageWriteWithoutFormat;
        RHIBool32 shaderUniformBufferArrayDynamicIndexing;
        RHIBool32 shaderSampledImageArrayDynamicIndexing;
        RHIBool32 shaderStorageBufferArrayDynamicIndexing;
        RHIBool32 shaderStorageImageArrayDynamicIndexing;
        RHIBool32 shaderClipDistance;
        RHIBool32 shaderCullDistance;
        RHIBool32 shaderFloat64;
        RHIBool32 shaderInt64;
        RHIBool32 shaderInt16;
        RHIBool32 shaderResourceResidency;
        RHIBool32 shaderResourceMinLod;
        RHIBool32 sparseBinding;
        RHIBool32 sparseResidencyBuffer;
        RHIBool32 sparseResidencyImage2D;
        RHIBool32 sparseResidencyImage3D;
        RHIBool32 sparseResidency2Samples;
        RHIBool32 sparseResidency4Samples;
        RHIBool32 sparseResidency8Samples;
        RHIBool32 sparseResidency16Samples;
        RHIBool32 sparseResidencyAliased;
        RHIBool32 variableMultisampleRate;
        RHIBool32 inheritedQueries;
    };

    struct RHIPhysicalDeviceLimits {
        uint32_t            maxImageDimension1D;
        uint32_t            maxImageDimension2D;
        uint32_t            maxImageDimension3D;
        uint32_t            maxImageDimensionCube;
        uint32_t            maxImageArrayLayers;
        uint32_t            maxTexelBufferElements;
        uint32_t            maxUniformBufferRange;
        uint32_t            maxStorageBufferRange;
        uint32_t            maxPushConstantsSize;
        uint32_t            maxMemoryAllocationCount;
        uint32_t            maxSamplerAllocationCount;
        RHIDeviceSize       bufferImageGranularity;
        RHIDeviceSize       sparseAddressSpaceSize;
        uint32_t            maxBoundDescriptorSets;
        uint32_t            maxPerStageDescriptorSamplers;
        uint32_t            maxPerStageDescriptorUniformBuffers;
        uint32_t            maxPerStageDescriptorStorageBuffers;
        uint32_t            maxPerStageDescriptorSampledImages;
        uint32_t            maxPerStageDescriptorStorageImages;
        uint32_t            maxPerStageDescriptorInputAttachments;
        uint32_t            maxPerStageResources;
        uint32_t            maxDescriptorSetSamplers;
        uint32_t            maxDescriptorSetUniformBuffers;
        uint32_t            maxDescriptorSetUniformBuffersDynamic;
        uint32_t            maxDescriptorSetStorageBuffers;
        uint32_t            maxDescriptorSetStorageBuffersDynamic;
        uint32_t            maxDescriptorSetSampledImages;
        uint32_t            maxDescriptorSetStorageImages;
        uint32_t            maxDescriptorSetInputAttachments;
        uint32_t            maxVertexInputAttributes;
        uint32_t            maxVertexInputBindings;
        uint32_t            maxVertexInputAttributeOffset;
        uint32_t            maxVertexInputBindingStride;
        uint32_t            maxVertexOutputComponents;
        uint32_t            maxTessellationGenerationLevel;
        uint32_t            maxTessellationPatchSize;
        uint32_t            maxTessellationControlPerVertexInputComponents;
        uint32_t            maxTessellationControlPerVertexOutputComponents;
        uint32_t            maxTessellationControlPerPatchOutputComponents;
        uint32_t            maxTessellationControlTotalOutputComponents;
        uint32_t            maxTessellationEvaluationInputComponents;
        uint32_t            maxTessellationEvaluationOutputComponents;
        uint32_t            maxGeometryShaderInvocations;
        uint32_t            maxGeometryInputComponents;
        uint32_t            maxGeometryOutputComponents;
        uint32_t            maxGeometryOutputVertices;
        uint32_t            maxGeometryTotalOutputComponents;
        uint32_t            maxFragmentInputComponents;
        uint32_t            maxFragmentOutputAttachments;
        uint32_t            maxFragmentDualSrcAttachments;
        uint32_t            maxFragmentCombinedOutputResources;
        uint32_t            maxComputeSharedMemorySize;
        uint32_t            maxComputeWorkGroupCount[3];
        uint32_t            maxComputeWorkGroupInvocations;
        uint32_t            maxComputeWorkGroupSize[3];
        uint32_t            subPixelPrecisionBits;
        uint32_t            subTexelPrecisionBits;
        uint32_t            mipmapPrecisionBits;
        uint32_t            maxDrawIndexedIndexValue;
        uint32_t            maxDrawIndirectCount;
        float               maxSamplerLodBias;
        float               maxSamplerAnisotropy;
        uint32_t            maxViewports;
        uint32_t            maxViewportDimensions[2];
        float               viewportBoundsRange[2];
        uint32_t            viewportSubPixelBits;
        size_t              minMemoryMapAlignment;
        RHIDeviceSize       minTexelBufferOffsetAlignment;
        RHIDeviceSize       minUniformBufferOffsetAlignment;
        RHIDeviceSize       minStorageBufferOffsetAlignment;
        int32_t             minTexelOffset;
        uint32_t            maxTexelOffset;
        int32_t             minTexelGatherOffset;
        uint32_t            maxTexelGatherOffset;
        float               minInterpolationOffset;
        float               maxInterpolationOffset;
        uint32_t            subPixelInterpolationOffsetBits;
        uint32_t            maxFramebufferWidth;
        uint32_t            maxFramebufferHeight;
        uint32_t            maxFramebufferLayers;
        RHISampleCountFlags framebufferColorSampleCounts;
        RHISampleCountFlags framebufferDepthSampleCounts;
        RHISampleCountFlags framebufferStencilSampleCounts;
        RHISampleCountFlags framebufferNoAttachmentsSampleCounts;
        uint32_t            maxColorAttachments;
        RHISampleCountFlags sampledImageColorSampleCounts;
        RHISampleCountFlags sampledImageIntegerSampleCounts;
        RHISampleCountFlags sampledImageDepthSampleCounts;
        RHISampleCountFlags sampledImageStencilSampleCounts;
        RHISampleCountFlags storageImageSampleCounts;
        uint32_t            maxSampleMaskWords;
        RHIBool32           timestampComputeAndGraphics;
        float               timestampPeriod;
        uint32_t            maxClipDistances;
        uint32_t            maxCullDistances;
        uint32_t            maxCombinedClipAndCullDistances;
        uint32_t            discreteQueuePriorities;
        float               pointSizeRange[2];
        float               lineWidthRange[2];
        float               pointSizeGranularity;
        float               lineWidthGranularity;
        RHIBool32           strictLines;
        RHIBool32           standardSampleLocations;
        RHIDeviceSize       optimalBufferCopyOffsetAlignment;
        RHIDeviceSize       optimalBufferCopyRowPitchAlignment;
        RHIDeviceSize       nonCoherentAtomSize;
    };

    struct RHIPhysicalDeviceMemoryProperties {
        uint32_t      memoryTypeCount;
        RHIMemoryType memoryTypes[RHI_MAX_MEMORY_TYPES];
        uint32_t      memoryHeapCount;
        RHIMemoryHeap memoryHeaps[RHI_MAX_MEMORY_HEAPS];
    };

    struct RHIPhysicalDeviceSparseProperties {
        RHIBool32 residencyStandard2DBlockShape;
        RHIBool32 residencyStandard2DMultisampleBlockShape;
        RHIBool32 residencyStandard3DBlockShape;
        RHIBool32 residencyAlignedMipSize;
        RHIBool32 residencyNonResidentStrict;
    };

    struct RHIPhysicalDeviceProperties {
        uint32_t                          apiVersion;
        uint32_t                          driverVersion;
        uint32_t                          vendorID;
        uint32_t                          deviceID;
        RHIPhysicalDeviceType             deviceType;
        char                              deviceName[RHI_MAX_PHYSICAL_DEVICE_NAME_SIZE];
        uint8_t                           pipelineCacheUUID[RHI_UUID_SIZE];
        RHIPhysicalDeviceLimits           limits;
        RHIPhysicalDeviceSparseProperties sparseProperties;
    };

    struct RHIPipelineColorBlendStateCreateInfo {
        RHIStructureType                            sType;
        const void*                                 pNext;
        RHIPipelineColorBlendStateCreateFlags       flags;
        RHIBool32                                   logicOpEnable;
        RHILogicOp                                  logicOp;
        uint32_t                                    attachmentCount;
        const RHIPipelineColorBlendAttachmentState* pAttachments;
        float                                       blendConstants[4];
    };

    struct RHIStencilOpState {
        RHIStencilOp failOp;
        RHIStencilOp passOp;
        RHIStencilOp depthFailOp;
        RHICompareOp compareOp;
        uint32_t     compareMask;
        uint32_t     writeMask;
        uint32_t     reference;
    };

    struct RHIPipelineDepthStencilStateCreateInfo {
        RHIStructureType                        sType;
        const void*                             pNext;
        RHIPipelineDepthStencilStateCreateFlags flags;
        RHIBool32                               depthTestEnable;
        RHIBool32                               depthWriteEnable;
        RHICompareOp                            depthCompareOp;
        RHIBool32                               depthBoundsTestEnable;
        RHIBool32                               stencilTestEnable;
        RHIStencilOpState                       front;
        RHIStencilOpState                       back;
        float                                   minDepthBounds;
        float                                   maxDepthBounds;
    };

    struct RHIPipelineDynamicStateCreateInfo {
        RHIStructureType                   sType;
        const void*                        pNext;
        RHIPipelineDynamicStateCreateFlags flags;
        uint32_t                           dynamicStateCount;
        const RHIDynamicState*             pDynamicStates;
    };

    struct RHIPipelineInputAssemblyStateCreateInfo {
        RHIStructureType                         sType;
        const void*                              pNext;
        RHIPipelineInputAssemblyStateCreateFlags flags;
        RHIPrimitiveTopology                     topology;
        RHIBool32                                primitiveRestartEnable;
    };

    struct RHIPipelineLayoutCreateInfo {
        RHIStructureType               sType;
        const void*                    pNext;
        RHIPipelineLayoutCreateFlags   flags;
        uint32_t                       setLayoutCount;
        RHIDescriptorSetLayout* const* pSetLayouts;
        uint32_t                       pushConstantRangeCount;
        const RHIPushConstantRange*    pPushConstantRanges;
    };

    struct RHIPipelineMultisampleStateCreateInfo {
        RHIStructureType                       sType;
        const void*                            pNext;
        RHIPipelineMultisampleStateCreateFlags flags;
        RHISampleCountFlagBits                 rasterizationSamples;
        RHIBool32                              sampleShadingEnable;
        float                                  minSampleShading;
        const RHISampleMask**                  pSampleMask;
        RHIBool32                              alphaToCoverageEnable;
        RHIBool32                              alphaToOneEnable;
    };

    struct RHIPipelineRasterizationStateCreateInfo {
        RHIStructureType                         sType;
        const void*                              pNext;
        RHIPipelineRasterizationStateCreateFlags flags;
        RHIBool32                                depthClampEnable;
        RHIBool32                                rasterizerDiscardEnable;
        RHIPolygonMode                           polygonMode;
        RHICullModeFlags                         cullMode;
        RHIFrontFace                             frontFace;
        RHIBool32                                depthBiasEnable;
        float                                    depthBiasConstantFactor;
        float                                    depthBiasClamp;
        float                                    depthBiasSlopeFactor;
        float                                    lineWidth;
    };

    struct RHIPipelineShaderStageCreateInfo {
        RHIStructureType                  sType;
        const void*                       pNext;
        RHIPipelineShaderStageCreateFlags flags;
        RHIShaderStageFlagBits            stage;
        RHIShader*                        module;
        const char*                       pName;
        const RHISpecializationInfo*      pSpecializationInfo;
    };

    struct RHIPipelineTessellationStateCreateInfo {
        RHIStructureType                        sType;
        const void*                             pNext;
        RHIPipelineTessellationStateCreateFlags flags;
        uint32_t                                patchControlPoints;
    };

    struct RHIPipelineVertexInputStateCreateInfo {
        RHIStructureType                          sType;
        const void*                               pNext;
        RHIPipelineVertexInputStateCreateFlags    flags;
        uint32_t                                  vertexBindingDescriptionCount;
        const RHIVertexInputBindingDescription*   pVertexBindingDescriptions;
        uint32_t                                  vertexAttributeDescriptionCount;
        const RHIVertexInputAttributeDescription* pVertexAttributeDescriptions;
    };

    struct RHIPipelineViewportStateCreateInfo {
        RHIStructureType                    sType;
        const void*                         pNext;
        RHIPipelineViewportStateCreateFlags flags;
        uint32_t                            viewportCount;
        const RHIViewport*                  pViewports;
        uint32_t                            scissorCount;
        const RHIRect2D*                    pScissors;
    };

    struct RHIPushConstantRange {
        RHIShaderStageFlags stageFlags;
        uint32_t            offset;
        uint32_t            size;
    };

    struct RHIQueueFamilyProperties {
        RHIQueueFlags queueFlags;
        uint32_t      queueCount;
        uint32_t      timestampValidBits;
        RHIExtent3D   minImageTransferGranularity;
    };

    struct RHIRenderPassCreateInfo {
        RHIStructureType                sType;
        const void*                     pNext;
        RHIRenderPassCreateFlags        flags;
        uint32_t                        attachmentCount;
        const RHIAttachmentDescription* pAttachments;
        uint32_t                        subpassCount;
        const RHISubpassDescription*    pSubpasses;
        uint32_t                        dependencyCount;
        const RHISubpassDependency*     pDependencies;
    };

    struct RHISamplerCreateInfo {
        RHIStructureType      sType;
        const void*           pNext;
        RHISamplerCreateFlags flags;
        RHIFilter             magFilter;
        RHIFilter             minFilter;
        RHISamplerMipmapMode  mipmapMode;
        RHISamplerAddressMode addressModeU;
        RHISamplerAddressMode addressModeV;
        RHISamplerAddressMode addressModeW;
        float                 mipLodBias;
        RHIBool32             anisotropyEnable;
        float                 maxAnisotropy;
        RHIBool32             compareEnable;
        RHICompareOp          compareOp;
        float                 minLod;
        float                 maxLod;
        RHIBorderColor        borderColor;
        RHIBool32             unnormalizedCoordinates;
    };

    struct RHISemaphoreCreateInfo {
        RHIStructureType        sType;
        const void*             pNext;
        RHISemaphoreCreateFlags flags;
    };

    struct RHIShaderModuleCreateInfo {
        RHIStructureType           sType;
        const void*                pNext;
        RHIShaderModuleCreateFlags flags;
        size_t                     codeSize;
        const uint32_t*            pCode;
    };

    struct RHISubmitInfo {
        RHIStructureType             sType;
        const void*                  pNext;
        uint32_t                     waitSemaphoreCount;
        RHISemaphore**               pWaitSemaphores;
        const RHIPipelineStageFlags* pWaitDstStageMask;
        uint32_t                     commandBufferCount;
        RHICommandBuffer* const*     pCommandBuffers;
        uint32_t                     signalSemaphoreCount;
        const RHISemaphore**         pSignalSemaphores;
    };

    struct RHISubpassDependency {
        uint32_t              srcSubpass;
        uint32_t              dstSubpass;
        RHIPipelineStageFlags srcStageMask;
        RHIPipelineStageFlags dstStageMask;
        RHIAccessFlags        srcAccessMask;
        RHIAccessFlags        dstAccessMask;
        RHIDependencyFlags    dependencyFlags;
    };

    struct RHISubpassDescription {
        RHISubpassDescriptionFlags    flags;
        RHIPipelineBindPoint          pipelineBindPoint;
        uint32_t                      inputAttachmentCount;
        const RHIAttachmentReference* pInputAttachments;
        uint32_t                      colorAttachmentCount;
        const RHIAttachmentReference* pColorAttachments;
        const RHIAttachmentReference* pResolveAttachments;
        const RHIAttachmentReference* pDepthStencilAttachment;
        uint32_t                      preserveAttachmentCount;
        const uint32_t*               pPreserveAttachments;
    };

    struct RHIWriteDescriptorSet {
        RHIStructureType         sType;
        const void*              pNext;
        RHIDescriptorSet*        dstSet;
        uint32_t                 dstBinding;
        uint32_t                 dstArrayElement;
        uint32_t                 descriptorCount;
        RHIDescriptorType        descriptorType;
        RHIDescriptorImageInfo*  pImageInfo       = nullptr;
        RHIDescriptorBufferInfo* pBufferInfo      = nullptr;
        RHIBufferView*           pTexelBufferView = nullptr;
    };

    struct RHIAttachmentReference {
        uint32_t       attachment;
        RHIImageLayout layout;
    };

    struct RHIPipelineColorBlendAttachmentState {
        RHIBool32              blendEnable;
        RHIBlendFactor         srcColorBlendFactor;
        RHIBlendFactor         dstColorBlendFactor;
        RHIBlendOp             colorBlendOp;
        RHIBlendFactor         srcAlphaBlendFactor;
        RHIBlendFactor         dstAlphaBlendFactor;
        RHIBlendOp             alphaBlendOp;
        RHIColorComponentFlags colorWriteMask;
    };

    struct RHIRect2D {
        RHIOffset2D offset;
        RHIExtent2D extent;
    };

    struct RHISpecializationInfo {
        uint32_t                          mapEntryCount;
        const RHISpecializationMapEntry** pMapEntries;
        size_t                            dataSize;
        const void*                       pData;
    };

    struct RHIVertexInputAttributeDescription {
        uint32_t  location;
        uint32_t  binding;
        RHIFormat format;
        uint32_t  offset;
    };

    struct RHIVertexInputBindingDescription {
        uint32_t           binding;
        uint32_t           stride;
        RHIVertexInputRate inputRate;
    };

    struct RHIViewport {
        float x;
        float y;
        float width;
        float height;
        float minDepth;
        float maxDepth;
    };

    struct RHIRenderPassBeginInfo {
        RHIStructureType     sType;
        const void*          pNext;
        RHIRenderPass*       renderPass;
        RHIFramebuffer*      framebuffer;
        RHIRect2D            renderArea;
        uint32_t             clearValueCount;
        const RHIClearValue* pClearValues;
    };

    struct RHIClearDepthStencilValue {
        float    depth;
        uint32_t stencil;
    };

    union RHIClearColorValue {
        float    float32[4];
        int32_t  int32[4];
        uint32_t uint32[4];
    };

    union RHIClearValue {
        RHIClearColorValue        color;
        RHIClearDepthStencilValue depthStencil;
    };

    struct RHIClearRect {
        RHIRect2D rect;
        uint32_t  baseArrayLayer;
        uint32_t  layerCount;
    };

    struct RHIClearAttachment {
        RHIImageAspectFlags aspectMask;
        uint32_t            colorAttachment;
        RHIClearValue       clearValue;
    };

    struct RHISwapChainDesc {
        RHIExtent2D                extent;
        RHIFormat                  image_format;
        RHIViewport*               viewport;
        RHIRect2D*                 scissor;
        std::vector<RHIImageView*> imageViews;
    };

    struct RHIDepthImageDesc {
        RHIImage*     depth_image      = VK_NULL_HANDLE;
        RHIImageView* depth_image_view = VK_NULL_HANDLE;
        RHIFormat     depth_image_format;
    };

    struct QueueFamilyIndices {
        std::optional<uint32_t> graphics_family;
        std::optional<uint32_t> present_family;
        std::optional<uint32_t> m_compute_family;

        bool isComplete() {
            return graphics_family.has_value() && present_family.has_value() && m_compute_family.has_value();
        }
    };

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR        capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR>   presentModes;
    };
} // namespace Piccolo