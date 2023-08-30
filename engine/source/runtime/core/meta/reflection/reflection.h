/**
 * @file reflection.h
 * @brief 反射在客户端开发中的使用
 */

#pragma once
#include "runtime/core/meta/json.h"

#include <functional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace Piccolo {

/* 属性描述宏 */
// 定义__REFLECTION_PARSER__时, 使用META(), CLASS(), STRUCT()宏表示需要被反射的 字段/类/结构体
// __attribute__((annotate(...)))：可以让clang生成AST的时候, 将标记添加进去
// #__VA_ARGS__：用于支持在宏定义中定义可变数量参数, 用于接收 ... 传递的多个参数
#if defined(__REFLECTION_PARSER__)
#define META(...) __attribute__((annotate(#__VA_ARGS__)))
#define CLASS(class_name, ...) class __attribute__((annotate(#__VA_ARGS__))) class_name
#define STRUCT(struct_name, ...) struct __attribute__((annotate(#__VA_ARGS__))) struct_name
// 没有定义__REFLECTION_PARSER__时, META(), CLASS(), STRUCT()宏不起作用
#else
#define META(...)
#define CLASS(class_name, ...) class class_name
#define STRUCT(struct_name, ...) struct struct_name
#endif

/* 当前类的友元类 */
// ##：是预处理拼接标记, 在宏定义展开的时候, 将##左边的内容, 与##右边的内容拼接到一起
#define REFLECTION_BODY(class_name)                                                   \
    friend class Reflection::TypeFieldReflectionOparator::Type##class_name##Operator; \
    friend class PSerializer;
/* 当前类的反射类: 注册到 Reflection::TypeFieldReflectionOparato 中 */
#define REFLECTION_TYPE(class_name)             \
    namespace Reflection {                      \
        namespace TypeFieldReflectionOparator { \
            class Type##class_name##Operator;   \
        }                                       \
    };

/* 注册相关宏 */
// 注册字段名
#define REGISTER_FIELD_TO_MAP(name, value) TypeMetaRegisterinterface::registerToFieldMap(name, value);
// 注册类名
#define REGISTER_BASE_CLASS_TO_MAP(name, value) TypeMetaRegisterinterface::registerToClassMap(name, value);
// 注册数组
#define REGISTER_ARRAY_TO_MAP(name, value) TypeMetaRegisterinterface::registerToArrayMap(name, value);
// 取消注册
#define UNREGISTER_ALL TypeMetaRegisterinterface::unregisterAll();

/* 反射指针 */
// new
#define PICCOLO_REFLECTION_NEW(name, ...) Reflection::ReflectionPtr(#name, new name(__VA_ARGS__));
// delete
#define PICCOLO_REFLECTION_DELETE(value) \
    if (value) {                           \
        delete value.operator->();         \
        value.getPtrReference() = nullptr; \
    }
// deep copy
#define PICCOLO_REFLECTION_DEEP_COPY(type, dst_ptr, src_ptr) *static_cast<type*>(dst_ptr) = *static_cast<type*>(src_ptr.getPtr());

/* Meta定义 */
#define TypeMetaDef(class_name, ptr) Piccolo::Reflection::ReflectionInstance(Piccolo::Reflection::TypeMeta::newMetaFromName(#class_name), (class_name*)ptr)
#define TypeMetaDefPtr(class_name, ptr) \
    new Piccolo::Reflection::ReflectionInstance(Piccolo::Reflection::TypeMeta::newMetaFromName(#class_name), (class_name*)ptr)

    // is_safely_castable<T,U>表示: 是否能够安全的将T类型转换为U类型
    template<typename T, typename U, typename = void>
    struct is_safely_castable : std::false_type {};
    template<typename T, typename U>
    struct is_safely_castable<T, U, std::void_t<decltype(static_cast<U>(std::declval<T>()))>> : std::true_type {};

    // 反射相关工具类的声明
    namespace Reflection {
        class TypeMeta;
        class FieldAccessor;
        class ArrayAccessor;
        class ReflectionInstance;
    } // namespace Reflection

    // 字段相关函数
    typedef std::function<void(void*, void*)> SetFunction;
    typedef std::function<void*(void*)>       GetFunction;
    typedef std::function<const char*()>      GetNameFunction;
    // 类相关函数
    typedef std::function<void*(const PJson&)>                          ConstructorWithPJson;
    typedef std::function<PJson(void*)>                                 WritePJsonByName;
    typedef std::function<int(Reflection::ReflectionInstance*&, void*)> GetBaseClassReflectionInstanceListFunc;
    // 数组相关函数
    typedef std::function<void(int, void*, void*)> SetArrayFunction;
    typedef std::function<void*(int, void*)>       GetArrayFunction;
    typedef std::function<int(void*)>              GetSizeFunction;
    typedef std::function<bool()>                  GetBoolFunction;

    // tuple定义
    typedef std::tuple<SetFunction, GetFunction, GetNameFunction, GetNameFunction, GetNameFunction, GetBoolFunction>     FieldFunctionTuple;
    typedef std::tuple<GetBaseClassReflectionInstanceListFunc, ConstructorWithPJson, WritePJsonByName, WritePJsonByName> ClassFunctionTuple;
    typedef std::tuple<SetArrayFunction, GetArrayFunction, GetSizeFunction, GetNameFunction, GetNameFunction>            ArrayFunctionTuple;

    namespace Reflection {
        /* TypeMeta注册接口 static */
        class TypeMetaRegisterinterface {
        public:
            static void registerToFieldMap(const char* name, FieldFunctionTuple* value);
            static void registerToClassMap(const char* name, ClassFunctionTuple* value);
            static void registerToArrayMap(const char* name, ArrayFunctionTuple* value);

            static void unregisterAll();
        };

        /* 类型元数据 */
        class TypeMeta {
            friend class FieldAccessor;
            friend class ArrayAccessor;
            friend class TypeMetaRegisterinterface;

        public:
            TypeMeta();

            // 根据name新建TypeMeta
            static TypeMeta newMetaFromName(std::string type_name);

            // 根据name新建ArrayAccessor
            static bool newArrayAccessorFromName(std::string array_type_name, ArrayAccessor& accessor);
            // 根据name和json新建ReflectionInstance
            static ReflectionInstance newFromNameAndPJson(std::string type_name, const PJson& json_context);
            // 根据name和instance写入json
            static PJson writeByName(std::string type_name, void* instance);

            // 获取类型名
            std::string getTypeName();

            // 获取字段列表, 将其输出到 out_list 中, 并返回字段数量
            int getFieldsList(FieldAccessor*& out_list);

            // 获取反射实例列表, 将其输出到 out_list 中, 并返回实例数量
            int getBaseClassReflectionInstanceList(ReflectionInstance*& out_list, void* instance);

            // 根据 name 获取 FieldAccessor
            FieldAccessor getFieldByName(const char* name);

            bool isValid() { return m_is_valid; }

            // 赋值运算符
            TypeMeta& operator=(const TypeMeta& dest);

        private:
            TypeMeta(std::string type_name);

        private:
            std::vector<FieldAccessor, std::allocator<FieldAccessor>> m_fields; // 字段列表

            std::string m_type_name; // 类型名

            bool m_is_valid; // 是否有效
        };

        /* 字段访问器 */
        class FieldAccessor {
            friend class TypeMeta;

        public:
            FieldAccessor();

            void* get(void* instance);
            void  set(void* instance, void* value);

            // 获取拥有该字段的类型元数据
            TypeMeta getOwnerTypeMeta();

            // 获取类型元数据, 返回false表示该类不是反射类
            bool getTypeMeta(TypeMeta& field_type);
            // 获取字段名
            const char* getFieldName() const;
            // 获取字段类型名
            const char* getFieldTypeName();
            // 判断是否为数组
            bool isArrayType();

            FieldAccessor& operator=(const FieldAccessor& dest);

        private:
            FieldAccessor(FieldFunctionTuple* functions);

        private:
            FieldFunctionTuple* m_functions;       // 字段的get,set等函数
            const char*         m_field_name;      // 字段名
            const char*         m_field_type_name; // 字段类型名
        };

        /* 数组访问器 */
        class ArrayAccessor {
            friend class TypeMeta;

        public:
            ArrayAccessor();
            const char* getArrayTypeName();
            const char* getElementTypeName();

            // 设置instance中第index个元素的值
            void set(int index, void* instance, void* element_value);
            // 获取instance中第index个元素的值
            void* get(int index, void* instance);
            // 获取instance中数组的大小
            int getSize(void* instance);

            ArrayAccessor& operator=(ArrayAccessor& dest);

        private:
            ArrayAccessor(ArrayFunctionTuple* array_func);

        private:
            ArrayFunctionTuple* m_func;              // 数组的get,set等函数
            const char*         m_array_type_name;   // 数组类型名
            const char*         m_element_type_name; // 元素类型名
        };

        /* 反射类实例 */
        class ReflectionInstance {
        public:
            ReflectionInstance(TypeMeta meta, void* instance) :
                m_meta(meta), m_instance(instance) {}
            ReflectionInstance() :
                m_meta(), m_instance(nullptr) {}

            ReflectionInstance& operator=(ReflectionInstance& dest);

            ReflectionInstance& operator=(ReflectionInstance&& dest);

        public:
            TypeMeta m_meta;     // 类型元数据
            void*    m_instance; // 数据实例
        };

        /* 反射类指针 */
        template<typename T>
        class ReflectionPtr {
            template<typename U>
            friend class ReflectionPtr;

        public:
            /* 构造函数 */
            ReflectionPtr(std::string type_name, T* instance) :
                m_type_name(type_name), m_instance(instance) {}
            ReflectionPtr() :
                m_type_name(), m_instance(nullptr) {}
            ReflectionPtr(const ReflectionPtr& dest) :
                m_type_name(dest.m_type_name), m_instance(dest.m_instance) {}

            /* 赋值 */
            template<typename U>
            ReflectionPtr<T>& operator=(const ReflectionPtr<U>& dest) {
                if (this == static_cast<void*>(&dest)) return *this;
                m_type_name = dest.m_type_name;
                m_instance  = static_cast<T*>(dest.m_instance);
                return *this;
            }
            template<typename U>
            ReflectionPtr<T>& operator=(ReflectionPtr<U>&& dest) {
                if (this == static_cast<void*>(&dest)) return *this;
                m_type_name = dest.m_type_name;
                m_instance  = static_cast<T*>(dest.m_instance);
                return *this;
            }
            ReflectionPtr<T>& operator=(const ReflectionPtr<T>& dest) {
                if (this == &dest) return *this;
                m_type_name = dest.m_type_name;
                m_instance  = dest.m_instance;
                return *this;
            }
            ReflectionPtr<T>& operator=(ReflectionPtr<T>&& dest) {
                if (this == &dest) return *this;
                m_type_name = dest.m_type_name;
                m_instance  = dest.m_instance;
                return *this;
            }

            std::string getTypeName() const { return m_type_name; }

            void setTypeName(std::string name) { m_name = type_name; }

            bool operator==(const T* ptr) const { return (m_instance == ptr); }

            bool operator!=(const T* ptr) const { return (m_instance != ptr); }

            bool operator==(const ReflectionPtr<T>& rhs_ptr) const { return (m_instance == rhs_ptr.m_instance); }

            bool operator!=(const ReflectionPtr<T>& rhs_ptr) const { return (m_instance != rhs_ptr.m_instance); }

            /* 强制类型转换 */
            template<typename T1>
            explicit operator T1*() {
                return static_cast<T1*>(m_instance);
            }
            template<typename T1>
            operator ReflectionPtr<T1>() {
                return ReflectionPtr<T1>(m_type_name, (T1*)(m_instance));
            }
            template<typename T1>
            explicit operator const T1*() const {
                return static_cast<T1*>(m_instance);
            }
            template<typename T1>
            operator const ReflectionPtr<T1>() const {
                return ReflectionPtr<T1>(m_type_name, (T1*)(m_instance));
            }

            /* 指针访问 */
            T*       operator->() { return m_instance; }
            T*       operator->() const { return m_instance; }
            T&       operator*() { return *(m_instance); }
            T*       getPtr() { return m_instance; }
            T*       getPtr() const { return m_instance; }
            const T& operator*() const { return *(static_cast<const T*>(m_instance)); }
            T*&      getPtrReference() { return m_instance; }

            /* 判断是否为空 */
            operator bool() const { return (m_instance != nullptr); }

        private:
            std::string m_type_name {""};     // 类型名
            typedef T   m_type;               // 类型
            T*          m_instance {nullptr}; // 数据实例
        };
    } // namespace Reflection

} // namespace Piccolo