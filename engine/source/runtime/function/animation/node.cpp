#include "runtime/function/animation/node.h"

#include "runtime/core/math/math.h"

namespace Piccolo {
    //---------------------------------------------------------------------
    void Node::setParent(Node* parent) {
        m_parent = parent;
        setDirty();
    }

    Node::Node(const std::string name) { m_name = name; }
    Node::~Node() {}
    void Node::clear() {}

    const std::string& Node::getName(void) const {
        return m_name;
    }
    Node* Node::getParent(void) const {
        return m_parent;
    }

    const Quaternion& Node::getOrientation() const {
        return m_orientation;
    }
    void Node::setOrientation(const Quaternion& q) {
        if (q.isNaN()) {
            // LOG_ERROR(__FUNCTION__, "Invalid orientation supplied as parameter");
            m_orientation = Quaternion::IDENTITY;
        } else {
            m_orientation = q;
            m_orientation.normalise();
        }
        setDirty();
    }
    void Node::resetOrientation(void) {
        m_orientation = Quaternion::IDENTITY;
        setDirty();
    }

    const Vector3& Node::getPosition(void) const {
        return m_position;
    }
    void Node::setPosition(const Vector3& pos) {
        if (pos.isNaN()) {
            // LOG_ERROR
        }
        m_position = pos;
        setDirty();
    }

    const Vector3& Node::getScale(void) const {
        return m_scale;
    }
    void Node::setScale(const Vector3& inScale) {
        if (inScale.isNaN()) {
            // LOG_ERROR
        }
        m_scale = inScale;
        setDirty();
    }
    void Node::scale(const Vector3& inScale) {
        m_scale *= inScale;
        setDirty();
    }

    /**
     * @brief 更新模型空间的变换
     * @remark 局部空间 => 模型空间, 是从当前节点开始, 逐步应用父节点的变换, 直到根骨骼
    */
    void Node::updateDerivedTransform(void) {
        if (m_parent) {
            // 旋转: 先应用自己的旋转, 再应用父骨骼的旋转
            const Quaternion& parent_orientation = m_parent->_getDerivedOrientation();
            m_derived_orientation                = parent_orientation * m_orientation;
            m_derived_orientation.normalise();
            // 缩放: 先应用自己的缩放, 再应用父骨骼的缩放
            const Vector3& parent_scale = m_parent->_getDerivedScale();
            m_derived_scale             = parent_scale * m_scale;
            // 位移: 先将自己的位移转换到模型空间, 然后再应用父骨骼的位移
            m_derived_position = parent_orientation * (parent_scale * m_position); // 基于父骨骼的旋转和缩放, 计算模型空间中的位移
            m_derived_position += m_parent->_getDerivedPosition();                 // 添加上父骨骼的位移
        } else {
            m_derived_orientation = m_orientation;
            m_derived_position    = m_position;
            m_derived_scale       = m_scale;
        }
    }

    /**
     * @brief 沿当前骨骼朝向平移骨骼节点
     * @param d 平移量
     * @param relativeTo 在哪个变换空间内, 默认为父骨骼空间
     * @remark 局部空间 => 模型空间, 是从当前节点开始, 逐步应用父节点的变换, 直到根骨骼
    */
    void Node::translate(const Vector3& d, TransformSpace relativeTo) {
        switch (relativeTo) {
            // 局部空间的平移: 应用当前骨骼朝向, 将平移转化到父骨骼空间
            case TransformSpace::LOCAL:
                m_position += m_orientation * d;
                break;
            // 全局坐标的平移: 应用父骨骼的旋转和缩放, 将平移转化到父骨骼空间
            case TransformSpace::OBJECT:
                if (m_parent) {
                    m_position += (m_parent->_getDerivedOrientation().inverse() * d) / m_parent->_getDerivedScale();
                } else {
                    m_position += d;
                }
                break;
            // 父骨骼空间的平移: 直接平移
            case TransformSpace::AREN:
                m_position += d;
                break;
        }
        setDirty();
    }

    /**
     * @brief 旋转骨骼节点
     * @param q 旋转四元数
     * @param relativeTo 在哪个变换空间内, 默认为局部空间
     * @remark 局部空间 => 模型空间, 是从当前节点开始, 逐步应用父节点的变换, 直到根骨骼
    */
    void Node::rotate(const Quaternion& q, TransformSpace relativeTo) {
        Quaternion qnorm = q;
        qnorm.normalise();

        switch (relativeTo) {
            // 父骨骼空间的旋转: 先进行应用父骨骼空间原有旋转，然后再旋转父骨骼空间的旋转增量qnorm
            case TransformSpace::AREN:
                m_orientation = qnorm * m_orientation;
                break;
            // 全局坐标的旋转: 先将全局旋转增量qnorm转换到局部空间, 然后再应用局部空间的旋转, 最后再转换到全局空间
            case TransformSpace::OBJECT:
                m_orientation = m_orientation * _getDerivedOrientation().inverse() * qnorm * _getDerivedOrientation();
                break;
            // 局部空间的旋转: 先应用局部空间的旋转, 然后再应用父骨骼空间的原有旋转
            case TransformSpace::LOCAL:
                m_orientation = m_orientation * qnorm;
                break;
        }

        setDirty();
    }

    const Quaternion& Node::_getDerivedOrientation(void) const { return m_derived_orientation; }
    const Vector3&    Node::_getDerivedPosition(void) const { return m_derived_position; }
    const Vector3&    Node::_getDerivedScale(void) const { return m_derived_scale; }
    const Matrix4x4&  Node::_getInverseTpose(void) const { return m_inverse_Tpose; }

    bool Node::isDirty() const { return m_is_dirty; }
    void Node::setDirty() { m_is_dirty = true; }
    void Node::update() {
        updateDerivedTransform();
        m_is_dirty = false;
    }

    // 设置当前姿势为初始姿势
    void Node::setAsInitialPose(void) {
        m_initial_orientation = m_orientation;
        m_initial_position    = m_position;
        m_initial_scale       = m_scale;
    }
    // 重置为初始姿势
    void Node::resetToInitialPose(void) {
        m_orientation = m_initial_orientation;
        m_position    = m_initial_position;
        m_scale       = m_initial_scale;
        setDirty();
    }

    const Quaternion& Node::getInitialOrientation(void) const { return m_initial_orientation; }
    const Vector3&    Node::getInitialPosition(void) const { return m_initial_position; }
    const Vector3&    Node::getInitialScale(void) const { return m_initial_scale; }

    //---------------------------------------------------------------------
    Bone::Bone() :
        Node(std::string()) {}

    void Bone::initialize(std::shared_ptr<RawBone> definition, Bone* parent_bone) {
        m_definition = definition;
        m_parent     = parent_bone;

        if (definition) {
            m_name = definition->name;
            setOrientation(definition->binding_pose.m_rotation);
            setPosition(definition->binding_pose.m_position);
            setScale(definition->binding_pose.m_scale);
            m_inverse_Tpose = definition->tpose_matrix;
            setAsInitialPose();
        }
    }

    size_t Bone::getID(void) const {
        if (m_definition) return m_definition->index;

        return std::numeric_limits<size_t>().max();
    }
} // namespace Piccolo