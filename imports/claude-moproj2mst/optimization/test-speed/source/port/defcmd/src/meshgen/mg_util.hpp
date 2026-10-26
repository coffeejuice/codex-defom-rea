// Private helpers of the p8-07 mesher sources (not a public header).
#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <share.h>

#include <defcmd/meshgen/meshgen_base.hpp>
#include <defdbio/gendb3d/obj3d.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/llobj/cls_140182880.hpp>
#include <defmodel/llobj/cls_14017ef20.hpp>
#include <defmodel/node_access.hpp>

namespace def::cmd::mg {

using model::DB_STEP_DATA;
using model::LLOBJ_DATA;

template <class T>
inline T& at(void* p, std::ptrdiff_t off)
{
    return *reinterpret_cast<T*>(static_cast<char*>(p) + off);
}
template <class T>
inline const T& at(const void* p, std::ptrdiff_t off)
{
    return *reinterpret_cast<const T*>(static_cast<const char*>(p) + off);
}

// 14047b934: fopen = _fsopen(name, mode, _SH_DENYNO)
inline std::FILE* crt_fopen(const char* name, const char* mode) { return _fsopen(name, mode, _SH_DENYNO); }

inline model::Cls_14017ef20* nodes(LLOBJ_DATA* o) { return reinterpret_cast<model::Cls_14017ef20*>(o->s8a0); }
inline model::Cls_140182880* elems(LLOBJ_DATA* o) { return reinterpret_cast<model::Cls_140182880*>(o->s9e0); }

inline std::int32_t& obj_number(LLOBJ_DATA* o) { return at<std::int32_t>(o, 8); }
inline std::int32_t& obj_type(LLOBJ_DATA* o) { return at<std::int32_t>(o, 0xc); }
inline std::int32_t& has_mesh(LLOBJ_DATA* o) { return o->u87c[0]; }   // +0x87c
inline std::int32_t& node_count(LLOBJ_DATA* o) { return o->u87c[1]; } // +0x880
inline std::int32_t& elem_count(LLOBJ_DATA* o) { return o->u87c[2]; } // +0x884

// 1401808d0 node coordinates (defmodel node_access)
inline double* node_coords(LLOBJ_DATA* o) { return model::fun_1401808d0(nodes(o)); }

// 140180c40: node state variable id 8 (temperature, slot +0x40): the accessor pattern of
// defmodel/node_access.hpp with key 8 (also ported as p407::node_var_08 behind p4-07's ext layer).
inline double* node_temperature(LLOBJ_DATA* o)
{
    model::Cls_14017ef20* s = nodes(o);
    std::int32_t* dims = model::fun_14017f020(s);
    model::fun_14017ffe0(s, s->n, dims, 8);
    if (s->u130 != nullptr) {
        const std::int32_t which = 8;
        model::fun_1401d48d0(static_cast<DB_STEP_DATA*>(s->u130), &which);
    }
    return static_cast<double*>(s->u010[6]);
}

// 140183d00: element material group array (+0x18) after the owner's region sync 1401d5820(.., 2);
// owner defdbio gendb3d/node_elem.cpp
inline std::int32_t* elem_groups(LLOBJ_DATA* o) { return dbio::g3d::element_material_groups(elems(o)); }

} // namespace def::cmd::mg
