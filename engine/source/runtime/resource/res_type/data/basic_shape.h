#pragma once

#include "runtime/core/math/vector3.h"
#include "runtime/core/meta/reflection/reflection.h"

namespace MyPiccolo {
    REFLECTION_TYPE(Geometry)
    CLASS(Geometry, Fields)
    {
        REFLECTION_BODY(Geometry);

    public:
        virtual ~Geometry() {}
    };

    REFLECTION_TYPE(Box)
    CLASS(Box
          : public Geometry, Fields)
    {
        REFLECTION_BODY(Box);

    public:
        ~Box() override {}

        Vector3 m_half_extents {0.5f, 0.5f, 0.5f}; // 长方体半高
    };

    REFLECTION_TYPE(Sphere)
    CLASS(Sphere
          : public Geometry, Fields)
    {
        REFLECTION_BODY(Sphere);

    public:
        ~Sphere() override {}

        float m_radius {0.5f}; // 球体半径
    };

    REFLECTION_TYPE(Capsule)
    CLASS(Capsule
          : public Geometry, Fields)
    {
        REFLECTION_BODY(Capsule);

    public:
        ~Capsule() override {}

        float m_radius {0.3f};      // 胶囊体半径
        float m_half_height {0.7f}; // 胶囊体半高
    };
} // namespace MyPiccolo