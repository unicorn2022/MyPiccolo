# [Piccolo引擎运行时架构](https://github.com/BoomingTech/Piccolo/wiki/Piccolo%E5%BC%95%E6%93%8E%E8%BF%90%E8%A1%8C%E6%97%B6%E6%9E%B6%E6%9E%84)

# 平台层 `platform`

提供操作系统/平台相关的底层功能。

目前包括：

- 文件系统 `file_service`
- 路径 `path`

# 核心层 `core`

提供软件系统常用模块。

目前包括：

- 基础库 `base` （宏、哈希）
- 色彩 `color`
- 数学库 `math`
- 元数据系统`meta`
  - 反射 `reflection`
  - 序列化/反序列化 `serializer`
- 日志系统 `log`

# 资源层 `resource`

提供资产加载、保存功能，资产的结构化数据定义和相关路径配置等。

目前包括：

- 资产系统 `asset_manager`
- 配置系统 `config_manager`
- 结构化数据定义`res_type`
  - 全局数据`global`
    - 全局粒子设置 `global_particle`
    - 全局渲染配置 `global_rendering`
  - 通用数据`common`
    - 世界 `world`
    - 关卡 `level`
    - 对象 `object`
  - 组件数据`components`
    - 动画 `animation`
    - 相机 `camera`
    - 粒子发射器 `emitter`
    - 网格 `mesh`
    - 运动 `motor`
    - 刚体 `rigid_body`
  - 其他数据`data`
    - 动画片段 `animation_clip`
    - 动画骨骼节点 `animation_skeleton_node_map`
    - 基本形状 `basic_shape`
    - 动画混合状态 `blend_state`
    - 相机配置 `camera_config`
    - 材质 `material`
    - 网格数据 `mesh_data`
    - 骨骼 `skeleton_data`
    - 骨骼掩膜 `skeleton_mask`

# 功能层 `function`

提供引擎功能模块。分为框架和子系统两部分。

## 框架 `framework`

运行时功能核心框架。核心框架采用世界 `world` -关卡 `level` -GO `object` -组件 `component` 的层级架构。

世界管理器 `world_manager` 负责管理世界的加载、卸载、保存，和tick下属当前关卡。 关卡 `level` 负责加载、卸载、保存关卡。同时关卡也管理下属GO的tick、创建和删除。 游戏对象 `object` 负责加载、保存GO。同时GO也管理下属组件。

组件全都继承自 `component.h` 中的 `Component` 类，目前组件包括：

- 动画 `animation`
- 相机 `camera`
- 网格 `mesh`
- 运动 `motor`
- 粒子 `particle`
- 刚体 `rigidbody`
- 变换 `transform`

## 子系统

`function` 文件夹中 `framework` 文件夹之外所有部分。在具体GO组件的功能之外，运行时功能层其他子系统。

目前包括：

- 动画 `animation`
- 角色 `character`
- 控制器 `controller`
- 全局上下文 `global`
- 输入 `input`
- 粒子 `particle`
- 物理 `physics`
- 渲染 `render`
- UI `ui`