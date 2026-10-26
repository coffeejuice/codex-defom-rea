// gendb3d/accessors.cpp: GENDB running counters of DB_WRITE_VAR (getters and setters; packet p4-05).
#include <defdbio/gendb3d/obj3d.hpp>

#include <defmodel/layout/db_step_data.hpp>

namespace def::dbio::g3d {

// @src 140199da0  return +0x6c
std::int32_t numnp(const DB_STEP_DATA* m) { return m->u06c; }
// @src 140199db0  return +0x70
std::int32_t numel(const DB_STEP_DATA* m) { return m->u070; }
// @src 14019a350  return +0x80
std::int32_t n_boundaries(const DB_STEP_DATA* m) { return m->u074[(0x80 - 0x74) / 4]; }
// @src 14019a200  return +0x98
std::int32_t n_objects_meshed(const DB_STEP_DATA* m) { return m->u074[(0x98 - 0x74) / 4]; }
// @src 14019a2f0  return +0xcc
std::int32_t fold_flag(const DB_STEP_DATA* m) { return m->u0cc; }
// @src 140199d60  return +0xd0
std::int32_t fold_node_offset(const DB_STEP_DATA* m) { return m->u0d0; }
// @src 140199d80  return +0xd4
std::int32_t fold_face_offset(const DB_STEP_DATA* m) { return m->u0d4; }
// @src 14019a220  return +0x110
std::int32_t record_counter(const DB_STEP_DATA* m) { return m->u110; }
// @src 140199ce0  return +0x114
std::int32_t node_offset(const DB_STEP_DATA* m) { return m->u114; }
// @src 140199d20  return +0x118
std::int32_t elem_offset(const DB_STEP_DATA* m) { return m->u118; }
// @src 14019a0c0  return +0x11c
std::int32_t bnd_node_offset(const DB_STEP_DATA* m) { return m->u11c; }
// @src 14019a100  return +0x124
std::int32_t geo_point_offset(const DB_STEP_DATA* m) { return m->u124; }
// @src 14019a140  return +0x128
std::int32_t geo_facet_offset(const DB_STEP_DATA* m) { return m->u128; }
// @src 14019a180  return +0x12c
std::int32_t geo_facet_id_offset(const DB_STEP_DATA* m) { return m->u12c; }
// @src 14019a1c0  return +0x130
std::int32_t face_offset(const DB_STEP_DATA* m) { return m->u130; }
// @src 14019a5a0  return +0x134
std::int32_t window_row(const DB_STEP_DATA* m) { return m->u134; }
// @src 14019a5e0  return +0x138
std::int32_t window_pos(const DB_STEP_DATA* m) { return m->u138; }

// @src 14019a310  +0xd0 = *v
void set_fold_node_offset(DB_STEP_DATA* m, const std::int32_t* v) { m->u0d0 = *v; }
// @src 14019a330  +0xd4 = *v
void set_fold_face_offset(DB_STEP_DATA* m, const std::int32_t* v) { m->u0d4 = *v; }
// @src 14019a2d0  +0x110 = *v
void set_record_counter(DB_STEP_DATA* m, const std::int32_t* v) { m->u110 = *v; }
// @src 140199d00  +0x114 = *v
void set_node_offset(DB_STEP_DATA* m, const std::int32_t* v) { m->u114 = *v; }
// @src 140199d40  +0x118 = *v
void set_elem_offset(DB_STEP_DATA* m, const std::int32_t* v) { m->u118 = *v; }
// @src 14019a0e0  +0x11c = *v
void set_bnd_node_offset(DB_STEP_DATA* m, const std::int32_t* v) { m->u11c = *v; }
// @src 14019a120  +0x124 = *v
void set_geo_point_offset(DB_STEP_DATA* m, const std::int32_t* v) { m->u124 = *v; }
// @src 14019a160  +0x128 = *v
void set_geo_facet_offset(DB_STEP_DATA* m, const std::int32_t* v) { m->u128 = *v; }
// @src 14019a1a0  +0x12c = *v
void set_geo_facet_id_offset(DB_STEP_DATA* m, const std::int32_t* v) { m->u12c = *v; }
// @src 14019a1e0  +0x130 = *v
void set_face_offset(DB_STEP_DATA* m, const std::int32_t* v) { m->u130 = *v; }

} // namespace def::dbio::g3d
