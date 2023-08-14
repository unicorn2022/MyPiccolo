#pragma once

#include "runtime/core/math/math.h"
#include "runtime/core/math/transform.h"

#include "runtime/resource/res_type/data/skeleton_data.h"

namespace MyPiccolo {
    /* 单个节点 */
    class Node {
    public:
        enum class TransformSpace {
            LOCAL, // 局部空间
            AREN,  // 父骨骼空间
            OBJECT // 模型空间
        };
#ifdef _DEBUG
    public:
#else
    protected:
#endif
        Node* m_parent {nullptr}; // 父骨骼节点

        std::string m_name; // 骨骼名称

        /* 父骨骼空间的变换 */
        Quaternion m_orientation {Quaternion::IDENTITY}; // 父骨骼空间的旋转
        Vector3    m_position {Vector3::ZERO};           // 父骨骼空间的位置
        Vector3    m_scale {Vector3::UNIT_SCALE};        // 父骨骼空间的缩放

        /* 模型空间的变换 */
        Quaternion m_derived_orientation {Quaternion::IDENTITY}; // 模型空间的旋转
        Vector3    m_derived_position {Vector3::ZERO};           // 模型空间的位置
        Vector3    m_derived_scale {Vector3::UNIT_SCALE};        // 模型空间的缩放

        /* 初始姿势时的变换 */
        Quaternion m_initial_orientation {Quaternion::IDENTITY}; // 初始姿势时的旋转
        Vector3    m_initial_position {Vector3::ZERO};           // 初始姿势时的位置
        Vector3    m_initial_scale {Vector3::UNIT_SCALE};        // 初始姿势时的缩放

        /* Tpos时变换矩阵的逆 */
        Matrix4x4 m_inverse_Tpose;

        /* 是否被修改过 */
        bool m_is_dirty {true};

    protected:
        // 设置父节点, 只能在内部调用
        virtual void setParent(Node* parent);

    public:
        Node(const std::string name);
        virtual ~Node();
        void clear();

        const std::string& getName(void) const;
        virtual Node*      getParent(void) const;

        virtual const Quaternion& getOrientation() const;
        virtual void              setOrientation(const Quaternion& q);
        virtual void              resetOrientation(void);

        virtual const Vector3& getPosition(void) const;
        virtual void           setPosition(const Vector3& pos);

        virtual const Vector3& getScale(void) const;
        virtual void           setScale(const Vector3& inScale);
        virtual void           scale(const Vector3& inScale);

        /**
         * @brief 更新模型空间的变换
         * @remark 局部空间 => 模型空间, 是从当前节点开始, 逐步应用父节点的变换, 直到根骨骼
        */
        virtual void updateDerivedTransform(void);

        /**
         * @brief 沿当前骨骼朝向平移骨骼节点
         * @param d 平移量
         * @param relativeTo 在哪个变换空间内, 默认为父骨骼空间
        */
        virtual void translate(const Vector3& d, TransformSpace relativeTo = TransformSpace::AREN);

        /**
         * @brief 旋转骨骼节点
         * @param q 旋转四元数
         * @param relativeTo 在哪个变换空间内, 默认为局部空间
        */
        virtual void rotate(const Quaternion& q, TransformSpace relativeTo = TransformSpace::LOCAL);

        virtual const Quaternion& _getDerivedOrientation(void) const;
        virtual const Vector3&    _getDerivedPosition(void) const;
        virtual const Vector3&    _getDerivedScale(void) const;
        virtual const Matrix4x4&  _getInverseTpose(void) const;

        virtual bool isDirty() const;
        virtual void setDirty();
        virtual void update();

        // 设置当前姿势为初始姿势
        virtual void setAsInitialPose(void);
        // 重置为初始姿势
        virtual void resetToInitialPose(void);

        virtual const Quaternion& getInitialOrientation(void) const;
        virtual const Vector3&    getInitialPosition(void) const;
        virtual const Vector3&    getInitialScale(void) const;
    };

    /* 单个骨骼节点 */
    class Bone : public Node {
        friend class LoDSkeleton;

    protected:
        std::shared_ptr<RawBone> m_definition {};

    public:
        Bone();
        void initialize(std::shared_ptr<RawBone> definition, Bone* parent_bone);

        size_t getID(void) const;
    };
} // namespace MyPiccolo