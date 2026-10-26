// defmodel/llobj/llobj_parts.hpp: typed views of the LLOBJ_DATA sub-objects that the layout
// header keeps as byte slots (their classes are p1-04's, in this folder).
#pragma once

#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/llobj/cls_14017ef20.hpp>
#include <defmodel/llobj/cls_140182880.hpp>
#include <defmodel/llobj/cls_140185570.hpp>
#include <defmodel/llobj/cls_140185c60.hpp>
#include <defmodel/llobj/cls_1401860a0.hpp>
#include <defmodel/llobj/cls_14018b220.hpp>
#include <defmodel/llobj/cls_1401889a0.hpp>
#include <defmodel/llobj/cls_140189440.hpp>
#include <defmodel/llobj/cls_1401956d0.hpp>
#include <defmodel/llobj/cls_140197a50.hpp>
#include <defmodel/llobj/cls_14017e740.hpp>

namespace def::model::llobj::parts {

static_assert(sizeof(Cls_14017ef20) <= sizeof(LLOBJ_DATA::s8a0));
inline Cls_14017ef20& part_8a0(LLOBJ_DATA& o) { return *reinterpret_cast<Cls_14017ef20*>(o.s8a0); }
static_assert(sizeof(Cls_140182880) <= sizeof(LLOBJ_DATA::s9e0));
inline Cls_140182880& part_9e0(LLOBJ_DATA& o) { return *reinterpret_cast<Cls_140182880*>(o.s9e0); }
static_assert(sizeof(Cls_140185570) <= sizeof(LLOBJ_DATA::sae8));
inline Cls_140185570& part_ae8(LLOBJ_DATA& o) { return *reinterpret_cast<Cls_140185570*>(o.sae8); }
static_assert(sizeof(Cls_140185c60) <= sizeof(LLOBJ_DATA::sb58));
inline Cls_140185c60& part_b58(LLOBJ_DATA& o) { return *reinterpret_cast<Cls_140185c60*>(o.sb58); }
static_assert(sizeof(Cls_1401860a0) <= sizeof(LLOBJ_DATA::s1ea0));
inline Cls_1401860a0& part_1ea0(LLOBJ_DATA& o) { return *reinterpret_cast<Cls_1401860a0*>(o.s1ea0); }
static_assert(sizeof(Cls_14018b220) <= sizeof(LLOBJ_DATA::s2160));
inline Cls_14018b220& part_2160(LLOBJ_DATA& o) { return *reinterpret_cast<Cls_14018b220*>(o.s2160); }
static_assert(sizeof(Cls_1401889a0) <= sizeof(LLOBJ_DATA::s2be8));
inline Cls_1401889a0& part_2be8(LLOBJ_DATA& o) { return *reinterpret_cast<Cls_1401889a0*>(o.s2be8); }
static_assert(sizeof(Cls_140189440) <= sizeof(LLOBJ_DATA::s2d10));
inline Cls_140189440& part_2d10(LLOBJ_DATA& o) { return *reinterpret_cast<Cls_140189440*>(o.s2d10); }
static_assert(sizeof(Cls_1401956d0) <= sizeof(LLOBJ_DATA::s2e68));
inline Cls_1401956d0& part_2e68(LLOBJ_DATA& o) { return *reinterpret_cast<Cls_1401956d0*>(o.s2e68); }
static_assert(sizeof(Cls_140197a50) <= sizeof(LLOBJ_DATA::s3920));
inline Cls_140197a50& part_3920(LLOBJ_DATA& o) { return *reinterpret_cast<Cls_140197a50*>(o.s3920); }
static_assert(sizeof(Cls_14017e740) == sizeof(LLOBJ_DATA::sbe0[0]));
inline Cls_14017e740& part_be0(LLOBJ_DATA& o, int i) { return *reinterpret_cast<Cls_14017e740*>(o.sbe0[i]); }

} // namespace def::model::llobj::parts
