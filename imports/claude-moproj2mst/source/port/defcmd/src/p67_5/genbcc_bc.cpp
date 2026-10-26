// GENBCC model-level helpers (packet p67-5): default boundary conditions of a 2D object.
// Spec: docs/re/commands/GENBCC.md. Decompiles: deform-rea-work/out/p67/genbcc.c,
// out/p67/gencmds/genbcc2.c; listings (floating point, call arguments):
// deform-rea-work/out/p675/genbcc/fl.txt.
#include "genbcc_bc.hpp"
#include <defcmd/contact_cmds.hpp>
#include <defcmd/vec3.hpp>
#include <defmodel/node_access.hpp>

#include <cmath>
#include <cstddef>
#include <cstdio>
#include <cstdlib>

#include <defdbio/objcheck/objcheck.hpp>
#include <defkey/kw_g2obj.hpp>
#include <defmodel/bc/boundary_topology.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/layout/cls_140269250.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/statevar/geo2d_kernels.hpp>
#include <defrt/port.hpp>
#include <defrt/rdata.hpp>
#include <defrt/stdlib/legacy_printf.hpp>
#include <defrt/rdata.hpp>

namespace def::cmd::genbcc {

namespace {

template <class T, class C>
T& at(C& base, std::size_t off)
{
    return *reinterpret_cast<T*>(reinterpret_cast<unsigned char*>(&base) + off);
}
template <class T, class C>
const T& at(const C& base, std::size_t off)
{
    return *reinterpret_cast<const T*>(reinterpret_cast<const unsigned char*>(&base) + off);
}

constexpr std::size_t kMesh = 0x87c;
constexpr std::size_t kNodes = 0x880;
constexpr std::size_t kElems = 0x884;
constexpr std::size_t kNbe = 0x88c;
constexpr std::size_t kNodeSet = 0x8a0;
constexpr std::size_t kCoords = 0x8a8;
constexpr std::size_t kUrz = 0x8b8;
constexpr std::size_t kFrz = 0x8c0;
constexpr std::size_t kBccdef = 0x990;
constexpr std::size_t kBccdfn = 0x998;
constexpr std::size_t kBcctmp = 0x9a0;
constexpr std::size_t kBcctfn = 0x9a8;
constexpr std::size_t kConn = 0x9e8;
constexpr std::size_t kEccdef = 0xae8;
constexpr std::size_t kEcdefn = 0xaf0;
constexpr std::size_t kEcctmp = 0xb00;
constexpr std::size_t kEctmfn = 0xb08;
constexpr std::size_t kNLoops = 0xbd8;
constexpr std::size_t kLoops = 0xbe0;
constexpr std::size_t kLocTmp = 0x2f40;
constexpr std::size_t kModel = 0x3a98;  // obj+15000

// rdata 14051f1b8  1.0E-6 (FRZ "is set" threshold)
constexpr double kFrzEps = def::rt::rdata::d_14051f1b8;
// rdata 14052c598  1.0E29 (start of the minimum search)
constexpr double kBig = def::rt::rdata::d_14052c598;
// @data 140524ce0  0.05 (axis tolerance factor)
constexpr double kAxisFactor = 0.05;
// rdata 14051bed8  0.0
constexpr double kZero = def::rt::rdata::d_14051bed8;

DB_STEP_DATA* owner(const LLOBJ_DATA& obj) { return at<DB_STEP_DATA*>(obj, kModel); }

// 1400015e0 / 1400015f0 on the object's model: dimension word (+0x1c) == 2 / == 3
bool model_2d(const LLOBJ_DATA& obj) { return at<std::int32_t>(*owner(obj), 0x1c) == 2; }
bool model_3d(const LLOBJ_DATA& obj) { return at<std::int32_t>(*owner(obj), 0x1c) == 3; }

// 32-bit index arithmetic of the original (IMUL / ADD on dwords, then MOVSXD)
std::int64_t ix(std::int32_t row, std::int32_t w, std::int32_t j)
{
    const auto p = static_cast<std::uint32_t>(row) * static_cast<std::uint32_t>(w) + static_cast<std::uint32_t>(j);
    return static_cast<std::int32_t>(p);
}

// 140158ca0: obj+0x39c0 (140158ce0) == 3
bool object_is_3d(LLOBJ_DATA& obj) { return model::geo_kw::object_dim(obj) == 3; }

} // namespace

// @src 1401ac6c0  find object *no (1401d6ea0) and regenerate its default BCs
bool regenerate_object(DB_STEP_DATA& m, const std::int32_t* no)
{
    LLOBJ_DATA* obj = key::g2obj::find_object(m, *no);
    if (obj == nullptr) return false;
    return regenerate_default_bc(*obj);
}

// @src 1402066e0  3D object: 140206730 (returns 0, nothing changed); else the 2D body
// @src 140158ca0  obj+0x39c0 == 3 (inlined: object_is_3d)
// @src 140206730  return 0
bool regenerate_default_bc(LLOBJ_DATA& obj)
{
    if (object_is_3d(obj)) return false;
    return regenerate_default_bc_2d(obj);
}

// @src 1402069a0  2D body: clear the BCs, axis search or all edges, set BCCDEF / ECCTMP
bool regenerate_default_bc_2d(LLOBJ_DATA& obj)
{
    std::int32_t n_axis = 0;   // local_28
    std::int32_t n_edges = 0;  // local_38
    // The original allocates NBE ints for each list (140256dc0). The axis list can take NBE + 1
    // entries (both nodes of edge 1 and the second node of every later edge); the original then
    // writes one int past its block. The port allocates one more so that case stays in bounds.
    const std::int32_t nbe = at<std::int32_t>(obj, kNbe);
    std::int32_t* axis_nodes = model::bc::calloc_ints(nbe == 0 ? 0 : nbe + 1);  // local_58
    std::int32_t* edges = model::bc::calloc_ints(nbe);                          // local_48
    DB_STEP_DATA& m = *owner(obj);
    std::int32_t w_def = at<std::int32_t>(m, 0x1640);  // local_3c: BCCDEF width (dims[0x12])
    std::int32_t w_dfn = at<std::int32_t>(m, 0x1644);  // local_60: BCCDFN width (dims[0x13])
    std::int32_t w_urz = at<std::int32_t>(m, 0x1600);  // local_64: URZ width (dims[2])
    std::int32_t w_frz = at<std::int32_t>(m, 0x1604);  // local_40: FRZ width (dims[3])
    reset_thermal_bc(obj);
    reset_deformation_bc(obj, &w_def, &w_dfn, &w_urz, &w_frz);
    if (object_geotyp(obj) == 1 || object_geotyp(obj) == 3) {
        axis_search(obj, &n_axis, axis_nodes, &n_edges, edges);
    } else {
        all_edges(obj, &n_edges, edges);
    }
    if (0 < n_axis) {
        // 140206b65..140206c3a: dof 0, code 1, function 0, value 0.0, widths (BCCDEF, BCCDFN),
        // mode 0, then -1, 0, 0.0 (used by modes 1 and 2 only)
        double* urz = model::fun_1401809c0(reinterpret_cast<model::Cls_14017ef20*>(&at<unsigned char>(obj, kNodeSet)));  // owner p67-3
        set_node_codes(obj, axis_nodes, n_axis, at<std::int32_t*>(obj, kBccdef), at<std::int32_t*>(obj, kBccdfn),
                       urz, 0, 1, 0, 0.0, &w_def, &w_dfn, 0, -1, 0, 0.0);
    }
    if (0 < n_edges) {
        // 140206c4d..140206ce4: kind 2, ECCTMP, no function / value array, code 2, function 0,
        // value 0.0, mode 0, then 0, 0.0
        set_edge_codes(obj, 2, edges, n_edges, at<std::int32_t*>(obj, kEcctmp), nullptr, nullptr, 2, 0, 0.0, 0, 0,
                       0.0);
    }
    if (axis_nodes != nullptr) std::free(axis_nodes);
    if (edges != nullptr) std::free(edges);
    return true;
}

// @src 140204d20  clear the thermal BCs and the LOCTMP windows
void reset_thermal_bc(LLOBJ_DATA& obj)
{
    std::int32_t* bcctmp = at<std::int32_t*>(obj, kBcctmp);
    std::int32_t* bcctfn = at<std::int32_t*>(obj, kBcctfn);
    for (std::int32_t i = 0; i < at<std::int32_t>(obj, kNodes); ++i) {
        if (0 < bcctmp[i]) bcctfn[i] = 0;
        bcctmp[i] = 0;
    }
    std::int32_t* ecctmp = at<std::int32_t*>(obj, kEcctmp);
    std::int32_t* ectmfn = at<std::int32_t*>(obj, kEctmfn);
    if (ecctmp != nullptr && ectmfn != nullptr) {
        // only the first NBE slots; a multi-loop object has NBE - 2 + 2 * loops + 1 slots
        for (std::int32_t j = 0; j < at<std::int32_t>(obj, kNbe); ++j) {
            if (0 < ecctmp[j]) ectmfn[j] = 0;
            ecctmp[j] = 0;
        }
    }
    at<model::Cls_140269250>(obj, kLocTmp).clear();  // 1402699c0
}

// @src 140204180  clear the deformation BCs (2D: components 0 and 1, negative codes kept; 3D:
// 3 components at stride 3, every code cleared)
void reset_deformation_bc(LLOBJ_DATA& obj, const std::int32_t* w_def, const std::int32_t* w_dfn,
                          const std::int32_t* w_urz, const std::int32_t* w_frz)
{
    std::int32_t* code = at<std::int32_t*>(obj, kBccdef);
    std::int32_t* fn = at<std::int32_t*>(obj, kBccdfn);
    double* urz = at<double*>(obj, kUrz);
    double* frz = at<double*>(obj, kFrz);
    for (std::int32_t i = 0; i < at<std::int32_t>(obj, kNodes); ++i) {
        // the dimension test is made per node in the original
        if (!model_2d(obj)) {
            for (std::int32_t c = 0; c < 3; ++c) {
                const std::int64_t k = ix(i, 3, c);
                if (code[k] == 0) {
                    if (kFrzEps < std::fabs(frz[k])) fn[k] = 0;  // 140484558 = fabs
                } else {
                    fn[k] = 0;
                    if (code[k] == 1) urz[k] = 0.0;
                }
                code[k] = 0;
            }
        } else {
            for (std::int32_t c = 0; c < 2; ++c) {
                if (code[ix(i, *w_def, c)] == 0) {
                    if (kFrzEps < std::fabs(frz[ix(i, *w_frz, c)])) fn[ix(i, *w_dfn, c)] = 0;
                } else {
                    fn[ix(i, *w_dfn, c)] = 0;
                    if (code[ix(i, *w_def, c)] == 1) urz[ix(i, *w_urz, c)] = 0.0;
                }
                if (-1 < code[ix(i, *w_def, c)]) code[ix(i, *w_def, c)] = 0;
            }
        }
    }
    std::int32_t* eccdef = at<std::int32_t*>(obj, kEccdef);
    std::int32_t* ecdefn = at<std::int32_t*>(obj, kEcdefn);
    for (std::int32_t j = 0; j < at<std::int32_t>(obj, kNbe); ++j) {  // no null test in the original
        eccdef[j] = 0;
        ecdefn[j] = 0;
    }
}

// @src 140212420  GEOTYP of the owning model, 0 without one
// @src 1401d6d90  m+0x850
std::int32_t object_geotyp(const LLOBJ_DATA& obj)
{
    if (owner(obj) == nullptr) return 0;
    return at<std::int32_t>(*owner(obj), 0x850);
}

// @src 140206f60  edges 1..NBE
void all_edges(LLOBJ_DATA& obj, std::int32_t* count, std::int32_t* list)
{
    for (std::int32_t l = 1; l <= at<std::int32_t>(obj, kNbe); ++l) {
        list[*count] = l;
        *count = *count + 1;
    }
}

// @src 140206d50  axis nodes and off-axis edges (listing 140206d50..140206f4d)
bool axis_search(LLOBJ_DATA& obj, std::int32_t* n_axis, std::int32_t* axis_nodes, std::int32_t* n_edges,
                 std::int32_t* edges)
{
    std::int32_t n1 = 0;     // RSP+0x3c
    std::int32_t n2 = 0;     // RSP+0x20
    bool prev_on = false;    // RSP+0x51
    bool cur_on = false;     // RSP+0x50
    const double hmin = min_edge_length(obj);
    const double tol = kAxisFactor * hmin;  // 140206db2: MOVSD 0.05; MULSD hmin
    for (std::int32_t l = 1; l <= at<std::int32_t>(obj, kNbe); ++l) {
        const std::int32_t slot = edge_number_slot(obj, &l);
        model::bc::edge_slot_nodes(obj, slot, &n1, &n2);  // 140200e20
        const double* p1 = node_coords(obj, n1);
        const double* p2 = node_coords(obj, n2);
        if (p1 == nullptr || p2 == nullptr) continue;
        prev_on = cur_on;
        cur_on = false;
        // COMISD tol, fabs(x); JBE skips: taken when tol > fabs(x)
        if (l == 1 && tol > std::fabs(p1[0])) {
            prev_on = true;
            axis_nodes[*n_axis] = n1;
            *n_axis = *n_axis + 1;
        }
        if (tol > std::fabs(p2[0])) {
            cur_on = true;
            axis_nodes[*n_axis] = n2;
            *n_axis = *n_axis + 1;
        }
        if (!prev_on || !cur_on) {
            edges[*n_edges] = l;
            *n_edges = *n_edges + 1;
        }
    }
    return true;
}

// 140202a20: ported in defcmd/contact_cmds.hpp (min_edge_length)
double min_edge_length(LLOBJ_DATA& obj)
{
    return def::cmd::min_edge_length(&obj);
}

// 1403046a0: ported in defcmd/contact_cmds.hpp (min_edge_length_2d)
double min_edge_length_2d(LLOBJ_DATA& obj)
{
    return def::cmd::min_edge_length_2d(&obj);
}

// 1403087e0 / 1403087a0: ported in defcmd/vec3.hpp (v3::len of a V2)
double vec_length_2d(const double* v) { return v3::len(v3::make2(v)); }

// 1403054e0: ported in defdbio/objcheck/objcheck.hpp (obj_edge_index): 140305520 on the
// object's boundary loops
std::int32_t edge_number_slot(const LLOBJ_DATA& obj, const std::int32_t* edge)
{
    return dbio::objcheck::obj_edge_index(const_cast<LLOBJ_DATA*>(&obj), edge);
}

// 140305520: ported in defdbio/objcheck/objcheck.hpp (boundary_edge_index): edge number -> slot
std::int32_t edge_to_slot(const std::int32_t* edge, const std::int32_t* nloops, const void* loops)
{
    return dbio::objcheck::boundary_edge_index(edge, nloops, loops);
}

// @src 140163490  node coordinate pointer
// 140160bf0 (inlined; owner gendb::has_mesh, defdbio/gendb/gendb.hpp): obj+0x87c != 0
double* node_coords(LLOBJ_DATA& obj, std::int32_t n)
{
    if (at<std::int32_t>(obj, kMesh) == 0) return nullptr;
    if (n < 1 || at<std::int32_t>(obj, kNodes) < n) return nullptr;
    if (owner(obj) == nullptr) return nullptr;
    const std::int32_t w = at<std::int32_t>(*owner(obj), 0x15f8);
    double* xy = model::fun_1401808d0(reinterpret_cast<model::Cls_14017ef20*>(&at<unsigned char>(obj, kNodeSet)));  // owner p67-3
    return xy + ix(n - 1, w, 0);
}

// @src 140206fd0  node codes by mode (0: direct; 1: 140207f50; 2: 140207f50, then direct)
void set_node_codes(LLOBJ_DATA& obj, const std::int32_t* nodes, std::int32_t n, std::int32_t* code,
                    std::int32_t* fn, double* val, std::int32_t dof, std::int32_t c, std::int32_t f,
                    double v, const std::int32_t* w_code, const std::int32_t* w_fn, std::int32_t mode,
                    std::int32_t a14, std::int32_t a15, double a16)
{
    (void)a14;
    (void)a15;
    (void)a16;
    if (mode == 0) {
        set_node_codes_direct(obj, nodes, n, code, fn, val, dof, c, f, v, w_code, w_fn);
    } else if (mode == 1 || mode == 2) {
        // 140207f50 (GENBCC passes mode 0; not ported)
        DEF_NOT_PORTED(0x140207f50);
    }
}

// @src 140207250  set code c / function f / value v at component dof of every listed node,
// moving a code that must be kept (3, 4, negative; 3D also 5..8, 11, 12) to a free component
// first. `dof` is a parameter the original overwrites, so a search result carries over to the
// following nodes.
void set_node_codes_direct(LLOBJ_DATA& obj, const std::int32_t* nodes, std::int32_t n, std::int32_t* code,
                           std::int32_t* fn, double* val, std::int32_t dof, std::int32_t c, std::int32_t f,
                           double v, const std::int32_t* w_code, const std::int32_t* w_fn)
{
    const std::int32_t mn = *w_code < *w_fn ? *w_code : *w_fn;
    for (std::int32_t k = 0; k < n; ++k) {
        const std::int32_t row = nodes[k] + -1;
        auto C = [&](std::int32_t j) -> std::int32_t& { return code[ix(row, *w_code, j)]; };
        auto F = [&](std::int32_t j) -> std::int32_t& { return fn[ix(row, *w_fn, j)]; };
        bool is3d = model_3d(obj);
        if ((((c == 0) || (c == 1)) && (f < 0)) || (is3d && c == 6)) {
            bool found = false;
            for (std::int32_t j = 0; j < mn; ++j) {
                if (C(j) == c && F(j) == f) {
                    found = true;
                    break;
                }
            }
            if (found) continue;  // the pair is already there
        }
        // LAB_1402073ed
        if ((c == 0 || c == 1) && -1 < f) {
            bool keep = C(dof) == 3 || C(dof) < 0;
            if (model_3d(obj)) {
                keep = keep || C(dof) == 5;
                keep = keep || C(dof) == 6;
                keep = keep || C(dof) == 7;
                keep = keep || C(dof) == 8;
                keep = keep || C(dof) == 0xb;
                keep = keep || C(dof) == 0xc;
            }
            keep = keep || C(dof) == 4;
            keep = keep || F(dof) < 0;
            if (keep) {
                for (std::int32_t j = 0; j < *w_code; ++j) {
                    if (C(j) == 0 && -1 < F(j) && j != dof) {
                        C(j) = C(dof);
                        if (F(dof) < 0) {
                            F(j) = F(dof);
                        } else if (model_3d(obj) && (C(dof) == 6 || C(dof) == 7 || C(dof) == 8)) {
                            F(j) = F(dof);
                        }
                        break;
                    }
                }
            }
        }
        bool search = c == 3 || c == 4 || f < 0 || c < 0;
        if (model_3d(obj)) {
            search = search || c == 5;
            search = search || c == 6 || c == 7 || c == 8 || c == 0xb || c == 0xc;
        }
        if (search) {
            std::int32_t j = *w_code;
            bool busy = true;
            std::int32_t last_neg = -1;
            while ((j = j + -1, -1 < j && busy)) {
                if (!model_3d(obj)) {
                    // the function array is indexed with the code width here
                    busy = !(C(j) == 0 && fn[ix(row, *w_code, j)] == 0 && val[ix(row, *w_code, j)] == kZero);
                } else {
                    busy = !(C(j) == 0 || (C(j) == c && C(j) != 6));
                }
                if (!busy) break;
                if (model_3d(obj) && C(j) < 0) last_neg = j;
            }
            if (j < 0 && busy) j = 0;
            if (model_3d(obj) && busy) {
                if (last_neg < 0) {
                    if (C(j) == 1 && c == 6) continue;  // LAB_1402072b1: node skipped
                } else if (c == 6 || c == 1) {
                    j = last_neg;
                }
            }
            dof = j;
        }
        C(dof) = c;
        F(dof) = f;
        if (val != nullptr) val[ix(row, *w_code, dof)] = v;  // value indexed with the code width
    }
}

// @src 1402084c0  edge codes by mode (0: direct; 1: 140208920; 2: 140208920, then direct)
void set_edge_codes(LLOBJ_DATA& obj, std::int32_t kind, const std::int32_t* edges, std::int32_t n,
                    std::int32_t* code, std::int32_t* fn, double* val, std::int32_t c, std::int32_t f,
                    double v, std::int32_t mode, std::int32_t a12, double a13)
{
    (void)a12;
    (void)a13;
    if (mode == 0) {
        set_edge_codes_direct(obj, kind, edges, n, code, fn, val, c, f, v);
    } else if (mode == 1 || mode == 2) {
        // 140208920 (GENBCC passes mode 0; not ported)
        DEF_NOT_PORTED(0x140208920);
    }
}

// @src 1402086c0  edge codes: 2D through the edge numbers (140305d70 / 140305e90), 3D directly
// by face index - 1
void set_edge_codes_direct(LLOBJ_DATA& obj, std::int32_t kind, const std::int32_t* edges, std::int32_t n,
                           std::int32_t* code, std::int32_t* fn, double* val, std::int32_t c, std::int32_t f,
                           double v)
{
    std::int32_t cnt = n;  // local_res20
    const bool with_value = ((kind == 1 && c == 2) || (kind == 2 && c == 3) || (kind == 3 && c == 3) ||
                             (kind == 4 && c == 3));
    if (!model_3d(obj)) {
        store_edge_ints(obj, code, edges, &cnt, &c);
        if (val != nullptr) {
            const double zero = 0.0;
            store_edge_doubles(obj, val, edges, &cnt, &zero);
        }
        if (fn != nullptr) store_edge_ints(obj, fn, edges, &cnt, &f);
        if (with_value && val != nullptr) store_edge_doubles(obj, val, edges, &cnt, &v);
    } else if (edges != nullptr && code != nullptr) {
        for (std::int32_t i = 0; i < cnt; ++i) {
            const std::int32_t e = edges[i] + -1;
            code[e] = c;
            if (((kind != 2) || (c != 2)) && ((kind != 3) || (c != 2)) && fn != nullptr) fn[e] = f;
            if (with_value && val != nullptr) val[e] = v;
        }
    }
}

namespace {
// Common part of 140305d70 / 140305e90. Returns the slot to store into, or -1 when the original
// stores nothing (edge number out of range) or stores out of bounds (slot outside 0..NS).
std::int32_t checked_slot(LLOBJ_DATA& obj, std::int32_t edge, std::int32_t ns)
{
    std::int32_t l = edge;  // local_14
    if (l < 1 || at<std::int32_t>(obj, kNbe) < l) {
        rt::crt::fprintf(stderr, "EdgeNo(%d) is out of range[1 -- %d]\n", l, at<std::int32_t>(obj, kNbe));
        return -1;
    }
    const std::int32_t slot = edge_number_slot(obj, &l);  // local_10
    if (slot < 0 || ns < slot) {
        rt::crt::fprintf(stderr, "ECC array index(%d) is out of range[0 -- %d]\n", slot, ns);
        // The original stores arr[slot] anyway (a write before or after the array, heap
        // corruption). With a consistent boundary table (NBE = sum of loop node counts - 1, which
        // 140165170 recomputes) every edge number 1..NBE has a slot in 0..NS, so this branch is
        // not reached; the port skips the write.
        return -1;
    }
    return slot;
}
} // namespace

// @src 140305d70  int store per edge number
std::int32_t store_edge_ints(LLOBJ_DATA& obj, std::int32_t* arr, const std::int32_t* edges,
                             const std::int32_t* n, const std::int32_t* v)
{
    if (edges == nullptr) return -1;
    const std::int32_t ns = model::bc::edge_face_slot_count(obj);  // 140165170
    for (std::int32_t i = 0; i < *n; ++i) {
        const std::int32_t slot = checked_slot(obj, edges[i], ns);
        if (slot >= 0) arr[slot] = *v;
    }
    return 0;
}

// @src 140305e90  double store per edge number
std::int32_t store_edge_doubles(LLOBJ_DATA& obj, double* arr, const std::int32_t* edges,
                                const std::int32_t* n, const double* v)
{
    if (arr == nullptr) return -1;
    const std::int32_t ns = model::bc::edge_face_slot_count(obj);  // 140165170
    for (std::int32_t i = 0; i < *n; ++i) {
        const std::int32_t slot = checked_slot(obj, edges[i], ns);
        if (slot >= 0) arr[slot] = *v;
    }
    return 0;
}

} // namespace def::cmd::genbcc
