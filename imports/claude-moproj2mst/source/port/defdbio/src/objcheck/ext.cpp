// defdbio/src/objcheck/ext.cpp: default bodies of the p4-03 thin interface (see ext.hpp).
// "stub for <addr>" bodies restate the decompile of a trivial function another packet owns; the
// integration step points the wrapper at the owner's port instead.
#include <defdbio/gendb/gate_ports.hpp>
#include <defdbio/gendb/obj_counts.hpp>
#include <defmodel/statevar_kw/statevar_kw.hpp>
#include <defmodel/node_access.hpp>
#include <defmodel/bc/boundary_topology.hpp>
#include <defmodel/bc/node_arrays.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/geo_kw/mesh_model.hpp>
#include "ext.hpp"

#include <bit>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <initializer_list>

#include <defmodel/layout/cls_14017a6c0.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/material.hpp>
#include <defmodel/layout/material_list.hpp>
#include <defmodel/layout/obj_geo3d.hpp>
#include <defmodel/mesh2d/material_ops.hpp>
#include <defmodel/step/db_step.hpp>
#include <defmodel/dynary_init.hpp>
#include <defrt/port.hpp>

#include "raw.hpp"

namespace def::dbio::objcheck::ext {

namespace {

using raw::f64;
using raw::i32;
using raw::ptr;
using raw::u8;

inline std::uint64_t u(const void* p) { return reinterpret_cast<std::uintptr_t>(p); }
inline std::uint64_t ui(std::int64_t v) { return static_cast<std::uint64_t>(v); }

// Runs the hook when installed; true when it handled the call.
bool hooked(std::uint64_t addr, std::initializer_list<std::uint64_t> args, std::uint64_t* ret)
{
    if (g_hook == nullptr) return false;
    std::uint64_t a[8] = {};
    int n = 0;
    for (std::uint64_t v : args) a[n++] = v;
    return g_hook(addr, a, n, ret);
}

template <class T>
T as(std::uint64_t r)
{
    if constexpr (std::is_same_v<T, double>) return std::bit_cast<double>(r);
    else if constexpr (std::is_same_v<T, bool>) return (r & 0xff) != 0;
    else if constexpr (std::is_pointer_v<T>) return reinterpret_cast<T>(static_cast<std::uintptr_t>(r));
    else return static_cast<T>(r);
}

} // namespace

#define EXT_HOOK(T, addr, ...)                                                                     \
    do {                                                                                           \
        std::uint64_t r_ = 0;                                                                      \
        if (hooked(addr, {__VA_ARGS__}, &r_)) return as<T>(r_);                                    \
    } while (0)
#define EXT_HOOK_VOID(addr, ...)                                                                   \
    do {                                                                                           \
        std::uint64_t r_ = 0;                                                                      \
        if (hooked(addr, {__VA_ARGS__}, &r_)) return;                                              \
    } while (0)

// ---- error records (bare `ret` in the binary) ---------------------------------------------------
// stub for 14001cd60
void fun_14001cd60(const std::int32_t* code, const char* key, const void* a, const void* b)
{
    EXT_HOOK_VOID(0x14001cd60, u(code), u(key), u(a), u(b));
}
// stub for 14001cd70
void fun_14001cd70(const std::int32_t* code, const char* key, const void* a, const void* b)
{
    EXT_HOOK_VOID(0x14001cd70, u(code), u(key), u(a), u(b));
}
// stub for 14001cd80
void fun_14001cd80(const std::int32_t* code, const char* key, const void* a, const void* b)
{
    EXT_HOOK_VOID(0x14001cd80, u(code), u(key), u(a), u(b));
}
// stub for 14001cd90
void fun_14001cd90(const std::int32_t* code, const char* key, const void* a, const void* b)
{
    EXT_HOOK_VOID(0x14001cd90, u(code), u(key), u(a), u(b));
}

// ---- simulation control getters -------------------------------------------------------------------
// stub for 1401f3c10
std::int32_t fun_1401f3c10(const void* sim) { EXT_HOOK(std::int32_t, 0x1401f3c10, u(sim)); return i32(sim, 0x104); }
// stub for 1401f3c30
std::int32_t fun_1401f3c30(const void* sim) { EXT_HOOK(std::int32_t, 0x1401f3c30, u(sim)); return i32(sim, 0x108); }
// stub for 1401f3c50
std::int32_t fun_1401f3c50(const void* sim) { EXT_HOOK(std::int32_t, 0x1401f3c50, u(sim)); return i32(sim, 0x10c); }
// stub for 1401f3c70
std::int32_t fun_1401f3c70(const void* sim) { EXT_HOOK(std::int32_t, 0x1401f3c70, u(sim)); return i32(sim, 0x110); }
// stub for 1401f3cb0
std::int32_t fun_1401f3cb0(const void* sim) { EXT_HOOK(std::int32_t, 0x1401f3cb0, u(sim)); return i32(sim, 0x118); }
// stub for 1401ee580
std::int32_t fun_1401ee580(const void* sim) { EXT_HOOK(std::int32_t, 0x1401ee580, u(sim)); return i32(sim, 0x240); }

// ---- LLOBJ_DATA ---------------------------------------------------------------------------------
bool fun_140160d20(void* obj)
{
    EXT_HOOK(bool, 0x140160d20, u(obj));
    return model::geo_kw::curve_count(*static_cast<model::LLOBJ_DATA*>(obj)) > 0; // kw-g2geo (140160d20)
}
// stub for 140160d90
bool fun_140160d90(const void* obj) { EXT_HOOK(bool, 0x140160d90, u(obj)); return u8(obj, 0x3990) != 0; }
// stub for 140160f10
bool fun_140160f10(const void* obj) { EXT_HOOK(bool, 0x140160f10, u(obj)); return i32(obj, 0xc) != 7; }
std::int32_t fun_140165170(void* obj)
{
    EXT_HOOK(std::int32_t, 0x140165170, u(obj));
    return model::bc::edge_face_slot_count(*static_cast<model::LLOBJ_DATA*>(obj)); // 140165170
}
// stub for 140167ae0 (1401f3d90 on the owner's simulation control: c10 == 0, c30 == 1, c50 == 0,
// c70 == 0, cb0 == 0; a message when the object has no owner)
bool fun_140167ae0(void* obj)
{
    EXT_HOOK(bool, 0x140167ae0, u(obj));
    void* step = ptr(obj, 0x3a98);
    if (step == nullptr) {
        std::fprintf(stdout, "Error in LLOBJ_DATA::onlyDeformationModeOn()\n");
        return false;
    }
    const void* sim = static_cast<char*>(step) + 0x750;
    return fun_1401f3c10(sim) == 0 && fun_1401f3c30(sim) == 1 && fun_1401f3c50(sim) == 0 &&
           fun_1401f3c70(sim) == 0 && fun_1401f3cb0(sim) == 0;
}
// stub for 140198620
void* fun_140198620(void* obj, const std::int32_t* i)
{
    EXT_HOOK(void*, 0x140198620, u(obj), u(i));
    return ptr(obj, 0x3c00 + static_cast<std::int64_t>(*i) * 8);
}
void fun_1401fb270(void* obj, std::int32_t* a, std::int32_t* material)
{
    EXT_HOOK_VOID(0x1401fb270, u(obj), u(a), u(material));
    model::geo_kw::material_groups(*static_cast<model::LLOBJ_DATA*>(obj), a, material); // 1401fb270
}

// stub for 14017aaf0
model::LLOBJ_DATA* fun_14017aaf0(void* list, std::int32_t id)
{
    EXT_HOOK(model::LLOBJ_DATA*, 0x14017aaf0, u(list), ui(id));
    void* p = ptr(list, 0);
    while (p != nullptr && i32(p, 8) != id) p = ptr(p, 0x3940);
    return static_cast<model::LLOBJ_DATA*>(p);
}
// stub for 1401d6ea0
model::LLOBJ_DATA* fun_1401d6ea0(void* step, std::int32_t id)
{
    EXT_HOOK(model::LLOBJ_DATA*, 0x1401d6ea0, u(step), ui(id));
    return fun_14017aaf0(static_cast<char*>(step) + 0x1380, id);
}
model::LLOBJ_DATA* fun_1401d6ed0(void* step, std::int32_t n)
{
    EXT_HOOK(model::LLOBJ_DATA*, 0x1401d6ed0, u(step), ui(n));
    return static_cast<model::LLOBJ_DATA*>(model::step_object_at(*static_cast<model::DB_STEP_DATA*>(step), n));
}
model::Material* fun_1401d7060(void* step, std::int32_t id)
{
    EXT_HOOK(model::Material*, 0x1401d7060, u(step), ui(id));
    return model::step_material_by_id(*static_cast<model::DB_STEP_DATA*>(step), id);
}
// stub for 1401d7090 (-> 14029c260)
model::Material* fun_1401d7090(void* step, std::int32_t n)
{
    EXT_HOOK(model::Material*, 0x1401d7090, u(step), ui(n));
    return fun_14029c260(static_cast<char*>(step) + 0x1328, n);
}
// stub for 14019a280
std::int32_t fun_14019a280(const void* list) { EXT_HOOK(std::int32_t, 0x14019a280, u(list)); return i32(list, 0x10); }
model::LLOBJ_DATA* fun_14017aa80(void* list, std::int32_t n)
{
    EXT_HOOK(model::LLOBJ_DATA*, 0x14017aa80, u(list), ui(n));
    return static_cast<model::Cls_14017a6c0*>(list)->at(n);
}

// 14017f020: the p67-3 port
void* fun_14017f020(void* blk)
{
    EXT_HOOK(void*, 0x14017f020, u(blk));
    return model::fun_14017f020(static_cast<model::Cls_14017ef20*>(blk)); // p67-3
}
void fun_14017ffe0(void* blk, std::int32_t n, void* widths, std::int32_t mode)
{
    EXT_HOOK_VOID(0x14017ffe0, u(blk), ui(n), u(widths), ui(mode));
    model::bc::node_set_alloc(blk, n, static_cast<const std::int32_t*>(widths), mode); // 14017ffe0
}
// 140182980: the kw-g2sv port
void* fun_140182980(void* blk)
{
    EXT_HOOK(void*, 0x140182980, u(blk));
    return model::svkw::set9e0_dims(blk); // kw-g2sv
}
void fun_1401836c0(void* blk, std::int32_t n, void* widths, std::int32_t w, std::int32_t id)
{
    EXT_HOOK_VOID(0x1401836c0, u(blk), ui(n), u(widths), ui(w), ui(id));
    model::bc::element_set_alloc(blk, n, static_cast<const std::int32_t*>(widths), w, id); // 1401836c0
}
void fun_1401d48d0(void* owner, const std::int32_t* id)
{
    EXT_HOOK_VOID(0x1401d48d0, u(owner), u(id));
    model::fun_1401d48d0(static_cast<model::DB_STEP_DATA*>(owner), id); // p67-3
}

// stub for 140184520 (same pattern as 140184050 with array 0x35 at +0x98)
double* fun_140184520(void* blk)
{
    EXT_HOOK(double*, 0x140184520, u(blk));
    void* widths = fun_140182980(blk);
    fun_1401836c0(blk, i32(blk, 0xe8), widths, i32(blk, 0xec), 0x35);
    if (ptr(blk, 0xf0) != nullptr) {
        std::int32_t k = 0x35;
        fun_1401d48d0(ptr(blk, 0xf0), &k);
    }
    return static_cast<double*>(ptr(blk, 0x98));
}

// stub for 140186da0 (entries of kind 6 in the list at blk + 0x200)
std::int32_t fun_140186da0(void* blk)
{
    EXT_HOOK(std::int32_t, 0x140186da0, u(blk));
    std::int32_t n = fun_14019a530(blk);
    std::int32_t k = 0;
    if (n < 1) return 0;
    for (std::int32_t i = 1; i <= n; ++i) {
        void* e = fun_140259b60(static_cast<char*>(blk) + 0x200, i);
        if (i32(e, 4) == 6) k = k + 1;
    }
    return k;
}
// stub for 14019a530 (14019a550(blk + 0x200): count at +0x10)
std::int32_t fun_14019a530(void* blk)
{
    EXT_HOOK(std::int32_t, 0x14019a530, u(blk));
    return i32(blk, 0x200 + 0x10);
}
// stub for 140259b60
void* fun_140259b60(void* list, std::int32_t n)
{
    EXT_HOOK(void*, 0x140259b60, u(list), ui(n));
    std::int32_t k = 0;
    void* p = ptr(list, 0);
    while (p != nullptr && (k = k + 1, k < n)) p = ptr(p, 0xe0);
    return p;
}
// stub for 140189580 (14019a590(blk + 0x40): int at +0x14)
std::int32_t fun_140189580(void* blk)
{
    EXT_HOOK(std::int32_t, 0x140189580, u(blk));
    return i32(blk, 0x40 + 0x14);
}
// stub for 140195b60 (14019a590(blk + 0x78): int at +0x14)
std::int32_t fun_140195b60(void* blk)
{
    EXT_HOOK(std::int32_t, 0x140195b60, u(blk));
    return i32(blk, 0x78 + 0x14);
}
// stub for 140195b90 (14019a590(blk + 0xa8): int at +0x14)
std::int32_t fun_140195b90(void* blk)
{
    EXT_HOOK(std::int32_t, 0x140195b90, u(blk));
    return i32(blk, 0xa8 + 0x14);
}
// stub for 140136a10 (max(count 14013c1b0 at +0x10, every number at +0x54; links +0x60))
std::int32_t fun_140136a10(void* list)
{
    EXT_HOOK(std::int32_t, 0x140136a10, u(list));
    std::int32_t m = i32(list, 0x10);
    for (void* p = ptr(list, 0); p != nullptr; p = ptr(p, 0x60)) m = i32(p, 0x54) < m ? m : i32(p, 0x54);
    return m;
}
// stub for 1401368c0
void* fun_1401368c0(void* list, std::int32_t id)
{
    EXT_HOOK(void*, 0x1401368c0, u(list), ui(id));
    void* p = ptr(list, 0);
    while (p != nullptr && i32(p, 0x54) != id) p = ptr(p, 0x60);
    return p;
}
// stub for 1402580f0
void* fun_1402580f0(void* list, std::int32_t id)
{
    EXT_HOOK(void*, 0x1402580f0, u(list), ui(id));
    void* p = ptr(list, 0);
    while (p != nullptr && i32(p, 0) != id) p = ptr(p, 0x38);
    return p;
}

bool fun_14012f610(const void* t)
{
    EXT_HOOK(bool, 0x14012f610, u(t));
    return model::dynary_counts_positive(static_cast<const model::DYNARY*>(t));
}
std::int32_t fun_14012ff20(const void* t)
{
    EXT_HOOK(std::int32_t, 0x14012ff20, u(t));
    return model::dynary_value_count(static_cast<const model::DYNARY*>(t));
}

// stub for 140199c50
std::int32_t fun_140199c50(const void* list) { EXT_HOOK(std::int32_t, 0x140199c50, u(list)); return i32(list, 0x10); }
model::Material* fun_14029c260(void* list, std::int32_t n)
{
    EXT_HOOK(model::Material*, 0x14029c260, u(list), ui(n));
    return model::material_list_nth(static_cast<const model::MaterialList*>(list), n);
}
// stub for 1401b0620
bool fun_1401b0620(const void* mat) { EXT_HOOK(bool, 0x1401b0620, u(mat)); return i32(mat, 4) != 0; }
bool fun_14028d0a0(const void* mat)
{
    EXT_HOOK(bool, 0x14028d0a0, u(mat));
    return model::material_is_mixture(static_cast<const model::Material*>(mat));
}
std::int32_t fun_14029bb50(void* list, const char* flag)
{
    EXT_HOOK(std::int32_t, 0x14029bb50, u(list), u(flag));
    // kw-g2sv port; it takes the model, whose material list is at +0x1328
    auto* m = reinterpret_cast<model::DB_STEP_DATA*>(static_cast<char*>(list) - 0x1328);
    return model::svkw::material_grain_model(*m, reinterpret_cast<const bool*>(flag));
}

std::int32_t fun_1400034d0(void* geo)
{
    EXT_HOOK(std::int32_t, 0x1400034d0, u(geo));
    return static_cast<model::OBJ_GEO3D*>(geo)->node_count();
}
std::int32_t fun_140003500(void* geo)
{
    EXT_HOOK(std::int32_t, 0x140003500, u(geo));
    return static_cast<model::OBJ_GEO3D*>(geo)->elem_count();
}
// stub for 140005510 (140005100(geo + 0x18, 0))
std::int32_t* fun_140005510(void* geo)
{
    EXT_HOOK(std::int32_t*, 0x140005510, u(geo));
    return &static_cast<model::OBJ_GEO3D*>(geo)->elems.mutable_at(0);
}
// stub for 140006410 (140005e70(geo + 0x10, 0))
double* fun_140006410(void* geo)
{
    EXT_HOOK(double*, 0x140006410, u(geo));
    return &static_cast<model::OBJ_GEO3D*>(geo)->coords.mutable_at(0);
}
std::int32_t fun_1402fcfe0(void* geo, const char* flag)
{
    EXT_HOOK(std::int32_t, 0x1402fcfe0, u(geo), u(flag));
    return gendb::geo3d_surface_patch_count(static_cast<model::OBJ_GEO3D*>(geo), flag); // Phase 4 gate port
}

// stub for 1400015d0
std::int32_t fun_1400015d0(const void* list) { EXT_HOOK(std::int32_t, 0x1400015d0, u(list)); return i32(list, 0x10); }
// stub for 1401fac90
void* fun_1401fac90(void* list, std::int32_t n)
{
    EXT_HOOK(void*, 0x1401fac90, u(list), ui(n));
    std::int32_t k = 0;
    void* p = ptr(list, 0);
    while (p != nullptr && (k = k + 1, k < n)) p = ptr(p, 0x390);
    return p;
}

// 140239a30: the owner's port (defdbio/gendb/obj_counts.hpp vec3_norm)
double fun_140239a30(const double* v)
{
    EXT_HOOK(double, 0x140239a30, u(v));
    return def::dbio::gendb::vec3_norm(v);
}
// stub for 140239b40
void fun_140239b40(double* out, const double* v)
{
    EXT_HOOK_VOID(0x140239b40, u(out), u(v));
    double r[3] = {v[0], v[1], v[2]};
    double len = fun_140239a30(v);
    // COMISD DAT_14051f260, fabs(len); JBE -> zero, so a NaN length also gives the zero vector
    // (gendb::vec3_normalized, the p4-02a port of 140239b40); corrected at the P6-7 gate
    if (!(1e-12 < std::fabs(len))) { // DAT_14051f260
        r[2] = 0.0;
        r[1] = 0.0;
        r[0] = 0.0;
    } else {
        r[0] = r[0] / len;
        r[1] = r[1] / len;
        r[2] = r[2] / len;
    }
    out[0] = r[0];
    out[1] = r[1];
    out[2] = r[2];
}
// stub for 14023b720 (length 14023b610: 0 when the sum of squares is 0)
double fun_14023b720(double* v)
{
    EXT_HOOK(double, 0x14023b720, u(v));
    double s = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
    double len = s == 0.0 ? 0.0 : std::sqrt(s);
    if (len != 0.0) {
        v[0] = v[0] / len;
        v[1] = v[1] / len;
        v[2] = v[2] / len;
    }
    return len;
}
// stub for 14023b3a0
double fun_14023b3a0(const double* a, const double* b)
{
    EXT_HOOK(double, 0x14023b3a0, u(a), u(b));
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}
// stub for 1403091c0 (length 140309230)
double fun_1403091c0(double* v)
{
    EXT_HOOK(double, 0x1403091c0, u(v));
    double s = v[0] * v[0] + v[1] * v[1];
    double len = s == 0.0 ? 0.0 : std::sqrt(s);
    if (len != 0.0) {
        v[0] = v[0] / len;
        v[1] = v[1] / len;
    }
    return len;
}
// stub for 1403090e0
double fun_1403090e0(const double* a, const double* b)
{
    EXT_HOOK(double, 0x1403090e0, u(a), u(b));
    return a[0] * b[0] + a[1] * b[1];
}

} // namespace def::dbio::objcheck::ext
