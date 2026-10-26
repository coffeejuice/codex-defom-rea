// defcmd/cnvt3d.hpp: packet p9-01, the CNVT3D driver (FUN_1401467f0) and the functions it owns:
// status/log files, the copy of the simulation data into the new 3D model, the 2D -> 3D object
// property copy with its axis mapping, the 3D surface patch helpers, the mesh-window conversion.
// FIXGEO and the outline file I/O are in defmodel/geoprim/fixgeo.hpp, the COW list front /
// pop_front (140159a40 / 140159970) in defmodel/sdctl/cow.hpp. The command entry for the
// dispatcher is defcmd/cmd_p9_01.hpp.
//
// Specs: docs/re/commands/CNVT3D.md, CNVT3D_SWEEP.md (context), common.md.
// Status: docs/re/status/p9-01.tsv.
//
// Calls into packets that are not on this branch go through Cnvt3dExt (one pointer per callee):
// each defaults to the owner's function when its header is in the build (__has_include), else to
// a DEF_NOT_PORTED stub; tests replace them. See ext() below for the list and owners.
#pragma once

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>

#include <defcmd/meshgen/meshgen_base.hpp>

namespace def::model {
class DB_STEP_DATA;
class LLOBJ_DATA;
class LLWINDEF_DATA;
class OBJ_GEO3D;
class Cls_1401e7b10;
namespace linked {
class Cls_140258580;
class Cls_140258580List;
} // namespace linked
} // namespace def::model

namespace def::cmd::cnvt3d {

using model::DB_STEP_DATA;
using model::LLOBJ_DATA;

// ---- converter fields the generic MeshGenBase layout keeps as padding -------------------------
// +0x3f50 the new 3D model, +0x3f58 the source model's DB object (m+0x3390), +0x4770 the GEO23
// "c" value (a double; MeshGenBase declares the slot as a pointer).
inline DB_STEP_DATA*& model3d(MeshGenBase& c) { return *reinterpret_cast<DB_STEP_DATA**>(c.pad_3f50); }
inline void*& db_object(MeshGenBase& c) { return *reinterpret_cast<void**>(c.pad_3f50 + 8); }
inline double geo23_c(const MeshGenBase& c)
{
    double v;
    std::memcpy(&v, &c.u4770, 8);
    return v;
}
inline void set_geo23_c(MeshGenBase& c, double v) { std::memcpy(&c.u4770, &v, 8); }

// @data 1406e2d84 / 1406e2d88  current object (1-based) and object count of the conversion, for
// the "[%d/%d] " prefix of 14013c340 (p9-03)
inline std::int32_t g_obj_index = 0;
inline std::int32_t g_obj_count = 0;

// ---- status and log files (CNVT3D.md sections 4.2, 7) -----------------------------------------
// @src 14013c6b0  append msg + "\n" to CONV_STATUS.DAT ("a"; nothing else when that fails), then
// to <stem minus a trailing ".DB">.LOG ("a") and, when that opened, printf("%s\n", msg)
void log_message(const char* msg, const char* stem);
// @src 14013c320  remove("CONV_STATUS.DAT")
void delete_status_file();
// @src 14013c970  mode 0: remove DEF_CNV23.DAT; mode 1: open it "w" and, when sim_no > 0, write
// "\tSimNo\t" sim_no "\n"
void cnv23_file(std::int32_t mode, std::int32_t sim_no);
// @src 14013d760  log_message("$BGN$", " ")
void log_begin();
// @src 14013d790  log_message("$END$", " ")
void log_end();

// The small FILE wrapper of 14013c6b0 / 14013c970 / 14013d7e0: {name, FILE*}.
struct FileWrap {
    const char* name;  // 0x0 (1401b1880)
    std::FILE* f;      // 0x8
};
// @src 1401b1970  fwrite(p, n, 1, f) when the file is open
void file_write(FileWrap* w, const void* p, std::uint32_t n);

// ---- driver ---------------------------------------------------------------------------------
// @src 1401467f0  CNVT3D converter: builds a 3D model from objects ids[0..n) of `m` and writes it
// as a KEY file to m+0x140. db_name: m+0x1748 (log stem; "DEFORM" when null or empty).
void convert(MeshGenBase& cvt, const char* db_name, DB_STEP_DATA* m, const std::int32_t* ids,
             std::uint32_t n);
// @src 140148750  copy the 2D object's properties into its 3D object (14021d4c0); with a 2D
// mesh-generation kind (+0x2d94) != 0 also convert the mesh windows (140147f90)
void copy_object_props(MeshGenBase& cvt, LLOBJ_DATA* obj);
// @src 140147f90  mesh windows of obj (2D) -> 3D windows of o3 (a GEOPRIM revolve / extrude of
// each window outline in a scratch model); o3 null: the 3D object of obj's id in the new model
void convert_mesh_windows(MeshGenBase& cvt, LLOBJ_DATA* obj, LLOBJ_DATA* o3);
// @src 140159000  identity (the ACTOUT name m+0x140)
inline const char* key_name(const char* p) { return p; }

// ---- new model ------------------------------------------------------------------------------
// @src 1401a91d0  copy simulation control, materials, phase pairs, inter-object data of src
void copy_model_data(DB_STEP_DATA* dst, DB_STEP_DATA* src);
// @src 1401f36f0  simulation-control copy (flag: 1401e91d0's axis mode), then the counters
// +0xe4 and +0xf0 = src's + 1
void sim_ctrl_copy(model::Cls_1401e7b10* dst, model::Cls_1401e7b10* src, char flag);
// @src 1401e91d0  sim control 0x170..0x298: flag 0 swaps the 2nd/3rd components of the three
// vectors at 0x180/0x198/0x1b0 and the axis codes 2 <-> 3 at 0x290
void sim_ctrl_copy_axes(model::Cls_1401e7b10* dst, const model::Cls_1401e7b10* src, char flag);
// @src 1401e89c0  3D models only: 0x1c8, DSMAX, 0x1d0/0x1d8, 0x268..0x280, 0x260 = 0, 0x2a0
// (16 bytes), 0x2c0 = 1.0, the DYNARY at 0x1e8
void sim_ctrl_copy_3d_a(model::Cls_1401e7b10* dst, const model::Cls_1401e7b10* src);
// @src 1401e9c50  3D models only: 0x2e0 / 0x2e4 / 0x318 end as 2 (the remapping is overwritten),
// 0x2d0 = 0.005, 0x2d8 = 0.05, 0x310..0x324 copied
void sim_ctrl_copy_3d_b(model::Cls_1401e7b10* dst, const model::Cls_1401e7b10* src);
// @src 1401eb220  3D models only: 0x640..0x660 block, ten 0x51-byte strings at 0x688, 0x9b4
void sim_ctrl_copy_3d_c(model::Cls_1401e7b10* dst, const model::Cls_1401e7b10* src);
// @src 1401fa150  inter-object copy: empty
inline void inter_object_copy(void*, const void*) {}
// @src 14029b6c0  -> 14029b630 (p8-08) material list copy
void material_list_copy(void* dst, void* src);
// @src 140339dc0  -> 140339d30 (p8-08) phase-pair list copy
void phase_pair_copy(void* dst, void* src);

// ---- object property copy -------------------------------------------------------------------
// @src 14021d4c0  name (strncpy 0x40), type, GEO23 block (0x128), movement (14021ef80 with the
// 3D object's WPAXIS kind), +0x1ea0 (14021f160, 0), TRGVOL (14021d630), mesh windows (*windows)
void copy_object(LLOBJ_DATA* o3, LLOBJ_DATA* o2, const char* windows);
// @src 14021d630  TRGVOL from the mesh / geometry volume; 0 for object kind 2, else 1
std::int32_t update_target_volume(LLOBJ_DATA* o);
// @src 14021ef80  movement (Cls_14018b220) copy with the 2D -> 3D direction mapping (*axis 2/3)
void movement_copy_axis(std::byte* dst, const std::byte* src, const std::int32_t* axis);
// @src 14021f160  +0x1ea0 data: 0x18, 0x28, the vector at 0x30 (flag 0: 2nd/3rd swapped)
void props_copy_axis(std::byte* dst, const std::byte* src, char flag);
// @src 14021f760  mesh-generation windows (140189b70 with 0/0), then +0x7c, +0x78
void meshwin_copy(std::byte* dst, const std::byte* src);
// @src 140189110  GEO23 mode > 0
inline bool geo23_active(const std::int32_t* g23) { return 0 < *g23; }
// @src 140266860  window shape (+0x210) = 3
void window_set_shape3(model::LLWINDEF_DATA* w);

// ---- WPAXIS records -------------------------------------------------------------------------
// @src 14025a270  first record of kind (+4) 4; none and *create: append one (140259950) of kind 4
model::linked::Cls_140258580* wpaxis_find_kind4(model::linked::Cls_140258580List* l, const char* create);
// @src 14025a3b0  the kind-4 record (made when missing) gets axis *axis (140259270)
void wpaxis_set_axis(model::linked::Cls_140258580List* l, const std::int32_t* axis);
// @src 140259270  record vectors +0x80 / +0x68 for axis 1, 2 or 3
void wpaxis_record_set_axis(model::linked::Cls_140258580* r, const std::int32_t* axis);
// @src 14025a360  the kind-4 record's axis (140259170), 0 without one
std::int32_t wpaxis_axis(model::linked::Cls_140258580List* l);
// @src 140259170  3 when the vector at +0x80 . (0,0,1) == 1, 2 for (0,1,0), 1 for (1,0,0), else 0
std::int32_t wpaxis_record_axis(const model::linked::Cls_140258580* r);
// 140258ed0 / 140258f50 / 140258fd0 / 140259060 / 1402590f0 (P2, unowned; ported here):
// memcpy 3 doubles to +0x8 / +0x20 / +0x68 (normalised, 14023b720) / +0x80 (normalised) / +0x98
void wpaxis_set_08(model::linked::Cls_140258580* r, const double* v);
void wpaxis_set_20(model::linked::Cls_140258580* r, const double* v);
void wpaxis_set_68(model::linked::Cls_140258580* r, const double* v);
void wpaxis_set_80(model::linked::Cls_140258580* r, const double* v);
void wpaxis_set_98(model::linked::Cls_140258580* r, const double* v);

// ---- 3D surface patches (surface 0 of an object, obj+0x3c00[0]) -----------------------------
// @src 140198d30  partial revolve (ang < 360): faces lying in the start or end cut plane get a
// negative 4th node, the patch normals are rebuilt (1402059a0) and the patches parallel to
// neither cut plane are dropped (then 140205e40); ang >= 360: only 1402059a0
void mark_cut_faces(LLOBJ_DATA* o, double start, double ang);
// @src 1402059a0  rebuild the surface patch list (+0x28 point list of surface 0) from the faces
// with a non-positive 4th node; copy_axis: start from a copy of obj+0x2e80 (P2, unowned; ported
// here as a callee of 140198d30 / 140205e40)
void rebuild_patches(LLOBJ_DATA* o, char copy_axis);
// @src 140205e40  assign the existing patches to the faces (id + 600, 0 when none matches);
// without patches 1402059a0
void assign_patches(LLOBJ_DATA* o, char copy_axis);
// @src 14023c770  fabs(n . (p - q)) (P2, unowned)
double plane_distance(const double* p, const double* n, const double* q);
// @src 14023e050  unit normal of the triangle (a, b, c) about b (unchanged length when 0) (P2)
void triangle_normal(const double* a, const double* b, const double* c, double* n);

// ---- calls into other packets ---------------------------------------------------------------
struct Cnvt3dExt {
    // 140147b70 (p9-03): generate geometry (5) / tet mesh (2) / brick mesh (4)
    void (*generate)(MeshGenBase& cvt, std::int32_t id, std::int32_t nsec, std::int32_t m,
                     double start, double ang, double c, std::int32_t kind);
    // 14013e880 (p8-08): delete the exchange files (dispatcher case, before the converter)
    void (*exchange_cleanup)(MeshGenBase& cvt, bool flag);
    // 1401b1520 (p8-08): s cut to n characters when 0 < n < length
    void (*str_left)(std::string& out, const std::string& s, std::int32_t n);
    // 1402062b0 (p8-07): some face of surface 0 has a non-zero patch id
    bool (*has_patch_ids)(LLOBJ_DATA* o);
    // 140141580 (p8-07): NBCD.OUT -> node BCs of cvt+8
    std::int32_t (*read_nbcd)(MeshGenBase& cvt);
    // 1401e8040 / 1401e84f0 / 1401ea900 (p8-08): simulation-control copy parts
    void (*sim_ctrl_copy_a)(model::Cls_1401e7b10* dst, model::Cls_1401e7b10* src);
    void (*sim_ctrl_copy_b)(model::Cls_1401e7b10* dst, model::Cls_1401e7b10* src);
    void (*sim_ctrl_copy_c)(model::Cls_1401e7b10* dst, model::Cls_1401e7b10* src);
    // 14029b630 / 140339d30 (p8-08): material list / phase-pair list copy
    void (*material_list_copy)(void* dst, void* src);
    void (*phase_pair_copy)(void* dst, void* src);
    // 1401c5810 (p5-01 write_key_file): write the model as a KEY file; 0 ok
    std::int32_t (*write_key)(DB_STEP_DATA* m, const char* name, const std::uint32_t* flags);
    // 1402ba500 (p9-04 builder): build the primitive into o3 / model3
    bool (*build_primitive)(void* prim, LLOBJ_DATA* o3, const char* write_key, DB_STEP_DATA* model3);
    // 1402f9b60 via the window's _OBJ_GEO3D vtable +0x20 (copy_from): window+0x220 = surface
    // (kw-g3 keeps that member unconstructed; stub until it is)
    void (*window_geo3d_assign)(model::LLWINDEF_DATA* w, model::OBJ_GEO3D* s);
};
Cnvt3dExt& ext();
// every pointer back to its default
void reset_ext();

// Point defmodel's FIXGEO launch (def::model::geoprim::g_fixgeo_launch) at the launcher 140224100.
void bind_fixgeo_launch();

} // namespace def::cmd::cnvt3d
