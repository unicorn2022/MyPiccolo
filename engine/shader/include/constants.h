#define m_max_point_light_count 15                  // 最大点光源数量
#define m_max_point_light_geom_vertices 90          // 最大点光源几何体顶点数量 90 = 2 * 3 * 15
#define m_mesh_per_drawcall_max_instance_count 64   // 每个绘制调用的最大实例数量
#define m_mesh_vertex_blending_max_joint_count 1024 // 每个顶点最大关节数量

#define CHAOS_LAYOUT_MAJOR row_major // 主要布局为行主序
layout(CHAOS_LAYOUT_MAJOR) buffer;   // 缓冲
layout(CHAOS_LAYOUT_MAJOR) uniform;  // 常量