// Internal helpers of packet p9-03b (CNVT3D sweep half b).
#pragma once

#include <cstddef>
#include <cstdint>

#include <defcmd/contact_cmds.hpp>
#include <defcmd/meshgen/meshgen_base.hpp>
#include <defdbio/gendb3d/obj3d.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>

namespace def::cmd::cnvt3d_sweep::detail {

using model::DB_STEP_DATA;
using model::LLOBJ_DATA;

template <class T, class B>
inline T& at(B* base, std::ptrdiff_t off)
{
    return *reinterpret_cast<T*>(reinterpret_cast<unsigned char*>(const_cast<std::remove_const_t<B>*>(base)) + off);
}

// converter fields (CNVT3D.md 3)
inline LLOBJ_DATA*& cvt_obj(MeshGenBase* c) { return c->u008; }          // +0x08
inline LLOBJ_DATA* cvt_bound(MeshGenBase* c) { return c->u010; }         // +0x10
inline DB_STEP_DATA* cvt_m2(MeshGenBase* c) { return c->u3f48; }         // +0x3f48
inline DB_STEP_DATA* cvt_m3(MeshGenBase* c) { return at<DB_STEP_DATA*>(c, 0x3f50); }
inline double cvt_c(MeshGenBase* c) { return at<double>(c, 0x4770); }
inline const char* cvt_stem(MeshGenBase* c) { return c->u4360; }         // +0x4360

// object fields
inline std::int32_t& obj_id(LLOBJ_DATA* o) { return at<std::int32_t>(o, 8); }
inline std::int32_t& obj_nn(LLOBJ_DATA* o) { return at<std::int32_t>(o, 0x880); }
inline std::int32_t& obj_ne(LLOBJ_DATA* o) { return at<std::int32_t>(o, 0x884); }
inline void* node_set(LLOBJ_DATA* o) { return reinterpret_cast<unsigned char*>(o) + 0x8a0; }
inline void* elem_set(LLOBJ_DATA* o) { return reinterpret_cast<unsigned char*>(o) + 0x9e0; }

// 1401808d0: node coordinates (allocated on demand), def::model::fun_1401808d0 (p67-3)
double* coords(LLOBJ_DATA* o);
// 140183cb0: element connectivity es+8 after the region sync; owner defcmd contact/contact_cmds.cpp element_set_conn (p67)
inline std::int32_t* conn(LLOBJ_DATA* o) { return element_set_conn(elem_set(o)); }
// 140183d00: material groups es+0x18 after the region sync; owner defdbio gendb3d/node_elem.cpp
inline std::int32_t* mtlgrp(LLOBJ_DATA* o) { return dbio::g3d::element_material_groups(elem_set(o)); }
// 140183c20: integration points of element-set component j (Cls_140182880::fun_140183c20)
std::int32_t ipoints(LLOBJ_DATA* o, std::int32_t j);

// The accessor pattern of the node / element state-variable sets: allocate variable `id` for
// the set's rows (14017f020 + 14017ffe0 / 140182980 + 1401836c0), mark it in the owner model
// (1401d48d0) when `mark`, return the slot.
double* node_sv(void* set, std::int32_t id, std::ptrdiff_t slot, bool mark = true);
double* elem_sv(void* set, std::int32_t id, std::ptrdiff_t slot, bool mark = true);

// SV id table DAT_1405caa80 (46 ints)
extern const std::int32_t kSvIds[0x2e];

} // namespace def::cmd::cnvt3d_sweep::detail
