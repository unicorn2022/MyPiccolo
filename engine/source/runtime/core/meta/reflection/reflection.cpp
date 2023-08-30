#include "reflection.h"
#include <cstring>
#include <map>

namespace Piccolo {
    namespace Reflection {
        const char* k_unknown_type = "UnknownType";
        const char* k_unknown      = "Unknown";

        static std::multimap<std::string, FieldFunctionTuple*> m_field_map; // 字段映射
        static std::map<std::string, ClassFunctionTuple*>      m_class_map; // 类映射
        static std::map<std::string, ArrayFunctionTuple*>      m_array_map; // 数组映射

        /* TypeMeta注册接口 static */
        void TypeMetaRegisterinterface::registerToFieldMap(const char* name, FieldFunctionTuple* value) { m_field_map.insert(std::make_pair(name, value)); }
        void TypeMetaRegisterinterface::registerToClassMap(const char* name, ClassFunctionTuple* value) {
            // 已经存在的类不再注册
            if (m_class_map.find(name) == m_class_map.end()) {
                m_class_map.insert(std::make_pair(name, value));
            } else {
                delete value;
            }
        }
        void TypeMetaRegisterinterface::registerToArrayMap(const char* name, ArrayFunctionTuple* value) {
            // 已经存在的数组不再注册
            if (m_array_map.find(name) == m_array_map.end()) {
                m_array_map.insert(std::make_pair(name, value));
            } else {
                delete value;
            }
        }
        void TypeMetaRegisterinterface::unregisterAll() {
            // 清空字段映射
            for (const auto& itr : m_field_map) delete itr.second;
            m_field_map.clear();
            // 清空类映射
            for (const auto& itr : m_class_map) delete itr.second;
            m_class_map.clear();
            // 清空数组映射
            for (const auto& itr : m_array_map) delete itr.second;
            m_array_map.clear();
        }

        /* 类型元数据 */
        TypeMeta::TypeMeta() : m_type_name(k_unknown_type), m_is_valid(false) { m_fields.clear(); }
        TypeMeta::TypeMeta(std::string type_name) : m_type_name(type_name) {
            m_is_valid = false;
            m_fields.clear();

            // 遍历当前type_name对应的所有字段, 创建对应的 FieldAccessor
            auto fileds_iter = m_field_map.equal_range(type_name);
            while (fileds_iter.first != fileds_iter.second) {
                // 根据 FieldFunctionTuple 创建 FieldAccessor
                FieldAccessor f_field(fileds_iter.first->second);
                // 将 FieldAccessor 添加到 m_fields
                m_fields.emplace_back(f_field);
                m_is_valid = true;

                ++fileds_iter.first;
            }
        }
        // 根据name新建TypeMeta
        TypeMeta TypeMeta::newMetaFromName(std::string type_name) {
            TypeMeta f_type(type_name);
            return f_type;
        }
        // 根据name新建ArrayAccessor
        bool TypeMeta::newArrayAccessorFromName(std::string array_type_name, ArrayAccessor& accessor) {
            // 找到 name 对应的 ArrayFunctionTuple
            auto iter = m_array_map.find(array_type_name);

            // 根据 ArrayFunctionTuple 创建 ArrayAccessor
            if (iter != m_array_map.end()) {
                ArrayAccessor new_accessor(iter->second);
                accessor = new_accessor;
                return true;
            }

            return false;
        }
        // 根据name和json新建ReflectionInstance
        ReflectionInstance TypeMeta::newFromNameAndPJson(std::string type_name, const PJson& json_context) {
            // 找到 name 对应的 ClassFunctionTuple
            auto iter = m_class_map.find(type_name);

            // 根据 ClassFunctionTuple 的 ConstructorWithPJson 函数, 创建 ReflectionInstance
            if (iter != m_class_map.end()) {
                return ReflectionInstance(TypeMeta(type_name), (std::get<1>(*iter->second)(json_context)));
            }

            return ReflectionInstance();
        }
        // 根据name和instance写入json
        PJson TypeMeta::writeByName(std::string type_name, void* instance) {
            // 找到 name 对应的 ClassFunctionTuple
            auto iter = m_class_map.find(type_name);

            // 根据 ClassFunctionTuple 的 WritePJsonByName 函数, 创建 PJson
            if (iter != m_class_map.end()) {
                return std::get<2>(*iter->second)(instance);
            }
            return PJson();
        }
        // 获取类型名
        std::string TypeMeta::getTypeName() { return m_type_name; }
        // 获取字段列表, 将其输出到 out_list 中, 并返回字段数量
        int TypeMeta::getFieldsList(FieldAccessor*& out_list) {
            int count = m_fields.size();
            out_list  = new FieldAccessor[count];
            for (int i = 0; i < count; ++i) {
                out_list[i] = m_fields[i];
            }
            return count;
        }
        // 获取反射实例列表, 将其输出到 out_list 中, 并返回实例数量
        int TypeMeta::getBaseClassReflectionInstanceList(ReflectionInstance*& out_list, void* instance) {
            // 找到 name 对应的 ClassFunctionTuple
            auto iter = m_class_map.find(m_type_name);

            // 调用 ClassFunctionTuple 的 GetBaseClassReflectionInstanceList 函数
            if (iter != m_class_map.end()) {
                return (std::get<0>(*iter->second))(out_list, instance);
            }

            return 0;
        }
        // 根据 name 获取 FieldAccessor
        FieldAccessor TypeMeta::getFieldByName(const char* name) {
            // 遍历 m_fields, 找到 name 对应的 FieldAccessor
            const auto it = std::find_if(m_fields.begin(), m_fields.end(), [&](const auto& i) { return std::strcmp(i.getFieldName(), name) == 0; });
            if (it != m_fields.end()) return *it;
            return FieldAccessor(nullptr);
        }
        // 赋值运算符
        TypeMeta& TypeMeta::operator=(const TypeMeta& dest) {
            if (this == &dest) return *this;

            m_fields.clear();
            m_fields = dest.m_fields;

            m_type_name = dest.m_type_name;
            m_is_valid  = dest.m_is_valid;

            return *this;
        }

        /* 字段访问器 */
        FieldAccessor::FieldAccessor() {
            m_field_type_name = k_unknown_type;
            m_field_name      = k_unknown;
            m_functions       = nullptr;
        }
        FieldAccessor::FieldAccessor(FieldFunctionTuple* functions) : m_functions(functions) {
            m_field_type_name = k_unknown_type;
            m_field_name      = k_unknown;
            if (m_functions == nullptr) return;

            // 调用 FieldFunctionTuple 的 GetFieldName 和 GetFieldType 函数
            m_field_name      = (std::get<3>(*m_functions))();
            m_field_type_name = (std::get<4>(*m_functions))();
        }
        void* FieldAccessor::get(void* instance) {
            // todo: should check validation
            // 调用 FieldFunctionTuple 的 GetFunction
            return static_cast<void*>((std::get<1>(*m_functions))(instance));
        }
        void FieldAccessor::set(void* instance, void* value) {
            // todo: should check validation
            // 调用 FieldFunctionTuple 的 SetFunction
            (std::get<0>(*m_functions))(instance, value);
        }
        // 获取拥有该字段的类型元数据
        TypeMeta FieldAccessor::getOwnerTypeMeta() {
            // todo: should check validation
            // 调用 FieldFunctionTuple 的 GetOwnerTypeMeta 函数
            TypeMeta f_type((std::get<2>(*m_functions))());
            return f_type;
        }
        // 获取类型元数据, 返回false表示该类不是反射类
        bool FieldAccessor::getTypeMeta(TypeMeta& field_type) {
            TypeMeta f_type(m_field_type_name);
            field_type = f_type;
            return f_type.m_is_valid;
        }
        const char* FieldAccessor::getFieldName() const { return m_field_name; }
        const char* FieldAccessor::getFieldTypeName() { return m_field_type_name; }
        // 判断是否为数组类型
        bool FieldAccessor::isArrayType() {
            // todo: should check validation
            // 调用 FieldFunctionTuple 的 IsArrayType 函数
            return (std::get<5>(*m_functions))();
        }
        // 赋值运算符
        FieldAccessor& FieldAccessor::operator=(const FieldAccessor& dest) {
            if (this == &dest) return *this;
            m_functions       = dest.m_functions;
            m_field_name      = dest.m_field_name;
            m_field_type_name = dest.m_field_type_name;
            return *this;
        }

        /* 数组访问器 */
        ArrayAccessor::ArrayAccessor() : m_func(nullptr), m_array_type_name("UnKnownType"), m_element_type_name("UnKnownType") {}
        ArrayAccessor::ArrayAccessor(ArrayFunctionTuple* array_func) : m_func(array_func) {
            m_array_type_name   = k_unknown_type;
            m_element_type_name = k_unknown_type;
            if (m_func == nullptr) return;

            m_array_type_name   = std::get<3>(*m_func)();
            m_element_type_name = std::get<4>(*m_func)();
        }
        const char* ArrayAccessor::getArrayTypeName() { return m_array_type_name; }
        const char* ArrayAccessor::getElementTypeName() { return m_element_type_name; }
        // 设置instance中第index个元素的值
        void ArrayAccessor::set(int index, void* instance, void* element_value) {
            // todo: should check validation
            size_t count = getSize(instance);
            // todo: should check validation(index < count)
            std::get<0> (*m_func)(index, instance, element_value);
        }
        // 获取instance中第index个元素的值
        void* ArrayAccessor::get(int index, void* instance) {
            // todo: should check validation
            size_t count = getSize(instance);
            // todo: should check validation(index < count)
            return std::get<1>(*m_func)(index, instance);
        }
        // 获取instance中数组的大小
        int ArrayAccessor::getSize(void* instance) {
            // todo: should check validation
            return std::get<2>(*m_func)(instance);
        }
        // 赋值运算符
        ArrayAccessor& ArrayAccessor::operator=(ArrayAccessor& dest) {
            if (this == &dest) return *this;

            m_func              = dest.m_func;
            m_array_type_name   = dest.m_array_type_name;
            m_element_type_name = dest.m_element_type_name;
            return *this;
        }

        /* 反射类实例 */
        ReflectionInstance& ReflectionInstance::operator=(ReflectionInstance& dest) {
            if (this == &dest) return *this;

            m_instance = dest.m_instance;
            m_meta     = dest.m_meta;
            return *this;
        }

        ReflectionInstance& ReflectionInstance::operator=(ReflectionInstance&& dest) {
            if (this == &dest) return *this;

            m_instance = dest.m_instance;
            m_meta     = dest.m_meta;
            return *this;
        }
    } // namespace Reflection

} // namespace Piccolo