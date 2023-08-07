# MyPiccolo
本项目基于GAMES104的Piccolo引擎进行开发，首先进行引擎的复现，之后会逐步增加一些渲染方面的新功能

# 编译方式

```shell
cmake -S . -B build
cmake --build build --config Release
```

或者直接运行`build_windows.bat`

# 实现的功能

## 一、平台层：`runtime/platform`

基于`<filesystem>`，实现对文件路径的处理

- `/path`：相对路径、文件分段、文件扩展名、文件真实名
- `/file_service`：从目录中获取所有文件的路径

## 二、核心层：`runtime/core`

实现引擎必备的核心功能，供上层调用

- `/base`：

  - `/hash.h`：对哈希函数的封装
  - `/macro.h`：常用宏定义，如日志、引擎睡眠、获取名称、ASSERT

- `/color`：颜色类的定义

- `/log`：基于`spdlog`，实现对不同级别日志的封装

- `/math`：数学库

  - `/math.h`：对两种角度与一个通用类的封装，实现了
    - 弧度`Radian`、角度`Angle`、度数`Degree`三种类的实现与转换
    - 四则运算、三角函数的封装
  - `/random.h`：对随机数生成器的封装，实现了
    - 平均分布、伯努利分布、正态分布
    - 对数组的每个元素生成一个随机数
  - `/vector2~4.h`：对向量的封装，实现了
    - 四则运算
    - 点乘、叉乘、长度、归一化、中点、clamp
    - 线性插值、向平面投影、两向量之间的夹角、旋转到dest的最短四元数
  - `/matrix3.h`：对矩阵的封装，实现了
    - 四则运算、与数组的转换
    - 转置、求逆、行列式
    - QDU分解、旋转矩阵<=>绕轴旋转角度、缩放矩阵
  - `/matrix4.h`：齐次坐标矩阵，实现了
    - 
  - `/quaternion.h`：四元数，实现了
    - 四则运算
    - 与旋转矩阵、绕轴旋转、局部坐标系的转换
    - 点乘、长度、归一化、求逆、共轭、转化成欧拉角
    - 插值计算：sLerp、nLerp

- `/meta`：

  - `/reflection`：反射在客户端开发中的使用

    - 属性描述宏：`META()`、`CLASS()`、`STRUCT()`

    - TypeMeta注册接口：`TypeMetaRegisterinterface`

    - 类型元数据：`TypeMeta`

    - 字段访问器：`FieldAccessor`

    - 数组访问器：`ArrayAccessor`

    - 反射类实例：`ReflectionInstance`

    - 反射类指针：`ReflectionPtr`

    - 不同类型的属性拥有的相关函数

      ```c++
      typedef std::tuple<SetFunction, GetFunction, GetNameFunction, GetNameFunction, GetNameFunction, GetBoolFunction>     FieldFunctionTuple;
      typedef std::tuple<GetBaseClassReflectionInstanceListFunc, ConstructorWithPJson, WritePJsonByName, WritePJsonByName> ClassFunctionTuple;
      typedef std::tuple<SetArrayFunction, GetArrayFunction, GetSizeFunction, GetNameFunction, GetNameFunction>            ArrayFunctionTuple;
      ```

  - `/serializer`：序列化&反序列化

    - 定义了模板类的接口，需要依据具体类进行偏特化，如下修改

      ```c++
      // 实例写入PJson: char
      template<>
      PJson PSerializer::write(const char& instance);
      // PJson读取实例: char
      template<>
      char& PSerializer::read(const PJson& json_context, char& instance);
      ```

    - 实现了基础类型的偏特化：char、int、unsigned int、float、double、bool、string

- 

