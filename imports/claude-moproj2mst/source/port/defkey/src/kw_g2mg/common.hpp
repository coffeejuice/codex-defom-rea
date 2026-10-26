// kw_g2mg/common.hpp: object field access by offset and the calls into other packets.
#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdio>

#include <defkey/kw_g2mg.hpp>
#include <defkey/node_elem_var.hpp> // kw-g2bc: read_value_records 1404199a0
#include <defmodel/layout/cls_140269250.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/dyn_ary_linked_list.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/op_stop_linked_list.hpp>


namespace def::key::g2mg {

using model::DB_STEP_DATA;
using model::LLOBJ_DATA;

// The object fields this packet uses are inside sub-objects the LLOBJ_DATA layout leaves opaque;
// they are addressed by offset, as the decompile does.
template <class T>
T& at(LLOBJ_DATA& o, std::size_t off)
{
    return *reinterpret_cast<T*>(reinterpret_cast<std::byte*>(&o) + off);
}
template <class T>
const T& at(const LLOBJ_DATA& o, std::size_t off)
{
    return *reinterpret_cast<const T*>(reinterpret_cast<const std::byte*>(&o) + off);
}

inline std::int32_t obj_number(const LLOBJ_DATA& o) { return at<std::int32_t>(o, 0x8); }
inline std::int32_t obj_type(const LLOBJ_DATA& o) { return at<std::int32_t>(o, 0xc); }
// +0x3f24: the renumbered object number the registry handlers print when > 0
inline std::int32_t obj_out_no(const LLOBJ_DATA& o) { return at<std::int32_t>(o, 0x3f24); }
// +0x2d50: the MGUSER window list (first Cls_140269250 of the Cls_140189440 at +0x2d10)
inline model::Cls_140269250& mesh_windows(LLOBJ_DATA& o) { return at<model::Cls_140269250>(o, 0x2d50); }
// +0x20b8: the OPSTOP list (Cls_1401860a0 at +0x1ea0, +0x218)
inline model::OpStopLinkedList& opstop_list(LLOBJ_DATA& o) { return at<model::OpStopLinkedList>(o, 0x20b8); }
inline model::DynAryLinkedList& dynary_list(LLOBJ_DATA& o) { return o.uba8; }

// 1400015e0 (p1-01, inlined as in p1-03's sim_ctrl.cpp): the model is 2D / 3D
inline bool is_2d(const DB_STEP_DATA& m) { return m.dimension == 2; }
inline bool is_3d(const DB_STEP_DATA& m) { return m.dimension == 3; }

} // namespace def::key::g2mg
