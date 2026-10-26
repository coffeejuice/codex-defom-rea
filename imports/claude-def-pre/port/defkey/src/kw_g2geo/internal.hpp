// kw-g2geo internals: raw field access at recovered offsets, the dimension tests, and the four
// keyword families (curve, surface, mesh, misc) the Packet API entry points route to.
//
// LLOBJ_DATA keeps most of its members opaque (defmodel/layout/llobj_data.hpp), so the readers
// and writers address them by offset, as the decompile does: at<std::int32_t>(obj, 0x880) is
// *(int*)(obj + 0x880). Offsets are the ones in docs/re/keywords/<KW>.md.
#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdio>

#include <defkey/kw_g2geo.hpp>
#include <defkey/line_io.hpp>
#include <defmodel/keyword_registry.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>

namespace def::key::g2geo {

using model::DB_STEP_DATA;
using model::LLOBJ_DATA;

template <class T, class C>
inline T& at(C& base, std::size_t off)
{
    return *reinterpret_cast<T*>(reinterpret_cast<std::byte*>(&base) + off);
}
template <class T, class C>
inline const T& at(const C& base, std::size_t off)
{
    return *reinterpret_cast<const T*>(reinterpret_cast<const std::byte*>(&base) + off);
}

// FUN_1400015e0 / FUN_1400015f0: model+0x1c == 2 / == 3.
inline bool is_2d(const DB_STEP_DATA& m) { return at<std::int32_t>(m, 0x1c) == 2; }
inline bool is_3d(const DB_STEP_DATA& m) { return at<std::int32_t>(m, 0x1c) == 3; }
// the object's model (obj+0x3a98, FUN_140158f50)
inline DB_STEP_DATA& owner_of(LLOBJ_DATA& obj) { return *at<DB_STEP_DATA*>(obj, 0x3a98); }

// model+0x1744: the flags FUN_1401c5810 stores for the KEY writer.
inline std::uint32_t writer_flags(const DB_STEP_DATA& m) { return at<std::uint32_t>(m, 0x1744); }

// FUN_1402f7720(id): registry name, printed with %-6s.
inline const char* kw(int id) { return model::keyword_name(id); }

// The common prologue of a registry write handler (1402b4f80 ...): n = ref[0];
// obj = FUN_14017aaf0(model+0x1380, n), not checked for null; n = obj+0x3f24 when > 0.
struct Target {
    LLOBJ_DATA* obj;
    std::int32_t n;
};
Target target(DB_STEP_DATA& m, const KeyRef& ref);

// Keyword ids (registry, docs/re/keyword_registry.tsv).
enum : int {
    kDIEGEO = 158, kRZ = 159, kDRZ = 160, kURZ = 165, kFRZ = 166, kPRZ = 167, kELMCON = 173,
    kMTLGRP = 174, kSYMSUF = 262, kDRMESH = 263, kWPAXIS = 264, kTPLOGY = 269, kCSPMTX = 270,
    kCRDSYS = 282, kGEO23 = 283, kGEOSEC = 284, kMSHSEC = 285, kFLDGEO = 289, kFLWNET = 290,
    kPOLYTP = 299, kDIEPNT = 300, kDIEEDG = 301,
};

// ---- families --------------------------------------------------------------------------------
// Each family: handles(id) for its parser cases, read_case (0/2/3 like FUN_14038b8a0), and the
// writers it owns. The entry points in dispatch.cpp route by id.

namespace curve {  // DIEGEO curve/type-3 forms, TPLOGY, CSPMTX (curve.cpp)
bool handles(int id);
int read_case(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj, int id);
// DIEGEO 3D: the parser case reads line 0 and routes the surface form to surface::read_surface.
int write_curves(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj, std::uint32_t n);    // 1402aaa70
int write_tplogy(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj, std::uint32_t n);    // 1402a9f90
int write_cspmtx(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj, std::uint32_t n);    // 1402aaf40
int write_curve_group(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj, std::uint32_t n);  // 1402ab160
}  // namespace curve

namespace surface {  // DIEGEO surface form, POLYTP, DIEPNT, DIEEDG, FLDGEO, FLWNET (surface.cpp)
bool handles(int id);
int read_case(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj, int id);
// DIEGEO 3D surface form after line 0 (a = first int, nnode, nelem): 0 or 2.
int read_surface(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj, std::int32_t a, std::int32_t nnode,
                 std::int32_t nelem);
int write_geometry_dispatch(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj, std::uint32_t n);  // 1402a26c0
int write_surface(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj, std::uint32_t n);            // 1402a27a0
// 1402a2b70(model, f, name, geo3d, N): FLDGEO (obj+0x3328) / FLWNET (obj+0x3458) surface meshes
int write_surface_mesh(DB_STEP_DATA& m, std::FILE* f, const char* name, std::byte* geo3d, std::uint32_t n);
}  // namespace surface

namespace mesh {  // RZ, ELMCON, MTLGRP, DRZ, URZ, FRZ, PRZ, DRMESH (mesh.cpp)
bool handles(int id);
int read_case(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj, int id);
bool handles_write(int id);
int write_keyword(int id, std::FILE* f, DB_STEP_DATA& m, const KeyRef& ref);
int write_nodes(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj, std::uint32_t n);     // 1402a2db0
int write_elements(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj, std::uint32_t n);  // 1402a30c0
}  // namespace mesh

namespace misc {  // SYMSUF, WPAXIS, CRDSYS, GEO23, GEOSEC, MSHSEC (misc.cpp)
bool handles(int id);
int read_case(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj, int id);
int write_symsuf(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj, std::uint32_t n);  // 1402a3880
int write_wpaxis(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj, std::uint32_t n);  // 1402a39e0
}  // namespace misc

}  // namespace def::key::g2geo
