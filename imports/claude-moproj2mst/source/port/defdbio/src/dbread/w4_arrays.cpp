// dbread/w4_arrays.cpp: ReadArrays (1401d2be0) and the per-object readers it calls that belong to
// helper W4 of packet p-dbread (node BC arrays 140170410, element integers 140174130, edge / face
// arrays 140176df0, BC function tables 140177ba0 / 140178250). Field maps:
// docs/re/dbmap/objarrays.md sections 2-6, object.md (BCCFNC, SYMSUF).
//
// Index arithmetic is 32-bit as in the binary: start - 1 + offset (* width) is an int, then
// sign-extended; memcpy sizes are (int64)(count [* width]) << 2 (or << 3) with the product in int.
#include <defdbio/dbread/w4.hpp>
#include <defdbio/dbread/w1.hpp>
#include <defdbio/dbread/w2.hpp>

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <defdbio/gendb/gate_ports.hpp>
#include <defdbio/gendb/gendb.hpp>
#include <defdbio/gendb/pre_steps.hpp>
#include <defmodel/bc/boundary_topology.hpp>
#include <defmodel/bc/edge_face.hpp>
#include <defmodel/bc/node_arrays.hpp>
#include <defmodel/geo_kw/mesh_model.hpp>
#include <defmodel/layout/cls_14017a6c0.hpp>
#include <defmodel/layout/cls_1401364a0.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/dynary.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/linked/axis_point_list.hpp>
#include <defmodel/llobj/dynary_table.hpp>
#include <defmodel/mesh2d/material_ops.hpp>
#include <defmodel/sdctl/step_timer.hpp>
#include <defmodel/step/db_step.hpp>
#include <defmodel/step/name_arrays.hpp>
#include <defrt/stdlib/legacy_printf.hpp>
#include <defrt/stdlib/refcount.hpp>

#include "dbread/w4_access.hpp"

namespace def::dbio::dbread {

using model::DB_STEP_DATA;
using model::DYNARY;
using model::LLOBJ_DATA;
using namespace w4d;

namespace {

inline std::int32_t* ip(void* obj, std::size_t off) { return f<std::int32_t*>(obj, off); }
inline double* dp(void* obj, std::size_t off) { return f<double*>(obj, off); }

// -v with the wraparound of NEG
inline std::int32_t neg(std::int32_t v) { return static_cast<std::int32_t>(0u - static_cast<std::uint32_t>(v)); }

inline void copy4(void* dst, const void* src, std::int32_t count)
{
    std::memcpy(dst, src, static_cast<std::size_t>(static_cast<std::int64_t>(count) << 2));
}
inline void copy8(void* dst, const void* src, std::int32_t count)
{
    std::memcpy(dst, src, static_cast<std::size_t>(static_cast<std::int64_t>(count) << 3));
}

// operator new (thunk 1404797d8) of count * 4 bytes: MOVSXD, MUL by 4, CMOVO -1 on overflow; the
// buffers are released with free()
inline std::int32_t* new_ints(std::int32_t count)
{
    const std::uint64_t n = static_cast<std::uint64_t>(static_cast<std::int64_t>(count));
    const std::uint64_t bytes = n >= (1ull << 62) ? ~0ull : n * 4u;
    return static_cast<std::int32_t*>(std::malloc(bytes));
}

// MAR / A of the model, loaded afresh at every use as the binary does (+0x1b88 / +0x1b78)
inline std::int32_t* mar_of(DB_STEP_DATA* m) { return static_cast<std::int32_t*>(m->u1b88); }
inline double* a_of(DB_STEP_DATA* m) { return static_cast<double*>(m->u1b78); }

// 140158e90 + 1401c7d40 on the object's owner
inline void select_tables_of(LLOBJ_DATA* obj)
{
    std::int32_t dim = model::llobj::int_at_1c(obj->owner);
    model::select_index_tables(&dim);
}

} // namespace

// @src 140257490  140257510(node, p), then 140257590(node, p + 3)
void axis_point_set(model::linked::Cls_140257340* node, const double* p)
{
    gendb::point_node_set_point(node, p);
    gendb::point_node_set_direction(node, p + 3);
}

// @src 140170410  only with MAR and obj+0x87c != 0; arrays allocated (140162ff0) when +0x8a8 is 0;
//                 each array is read when the stored width equals W[id] (or is 0; NBCD also 3)
void read_node_bc_info(LLOBJ_DATA* obj, const std::int32_t* mar, const std::int32_t* mar_tab,
                       const std::int32_t* w, std::int32_t numnp)
{
    select_tables_of(obj);
    if (mar == nullptr || fi(obj, 0x87c) == 0) {
        return;
    }
    if (f<void*>(obj, 0x8a8) == nullptr) {
        model::bc::alloc_node_arrays(*obj, w);
    }
    const std::int32_t nn = fi(obj, 0x880);
    const std::int32_t off = fi(obj, 0x3984);
    if (w[0x48 / 4] == mar_tab[mix(0x1406e8278)] || mar_tab[mix(0x1406e8278)] == 0) {
        copy4(ip(obj, 0x990), mar + (mar_tab[mix(0x1406e8218)] + -1 + (off + -1) * w[0x48 / 4]), nn * w[0x48 / 4]);
    }
    if (w[0x4c / 4] == mar_tab[mix(0x1406e827c)] || mar_tab[mix(0x1406e827c)] == 0) {
        copy4(ip(obj, 0x998), mar + (mar_tab[mix(0x1406e821c)] + -1 + (off + -1) * w[0x4c / 4]), nn * w[0x4c / 4]);
    }
    if (w[0x50 / 4] == mar_tab[mix(0x1406e8280)] || mar_tab[mix(0x1406e8280)] == 0 ||
        mar_tab[mix(0x1406e8280)] == 3) {
        copy4(ip(obj, 0x9a0), mar + (mar_tab[mix(0x1406e8220)] + -2 + off), nn);
    }
    if (w[0x54 / 4] == mar_tab[mix(0x1406e8284)] || mar_tab[mix(0x1406e8284)] == 0) {
        copy4(ip(obj, 0x9a8), mar + (mar_tab[mix(0x1406e8224)] + -2 + off), nn);
    }
    if (mar_tab[mix(0x1406e8280)] == 3) {
        copy4(ip(obj, 0x9b8), mar + (mar_tab[mix(0x1406e8220)] + numnp + -2 + off), nn);
    }
    if (mar_tab[mix(0x1406e8280)] == 3) {
        copy4(ip(obj, 0x9b0), mar + (mar_tab[mix(0x1406e8220)] + numnp * 2 + -2 + off), nn);
    }
}

// @src 140174130  ReadElemIntegerInfoFromDB (timer DAT_1406e2e20): connectivity rows minus the
//                 object's node offset (2D: 4 per element from IIELM; 3D: nodes per element from
//                 the IIOBJ word 13 start), material groups when W[0x3d] matches
void read_elem_integer_info(LLOBJ_DATA* obj, const std::int32_t* mar, const std::int32_t* mar_tab,
                            const std::int32_t* w, const std::int32_t* iiobj, const std::int32_t* which)
{
    select_tables_of(obj);
    if (fi(obj, 0x87c) == 0) {
        return;
    }
    model::StepTimer& t = model::g_timer_read_elem_integer_info;
    t.start("Allocate memory");
    if (f<void*>(obj, 0x9e8) == nullptr) {
        model::bc::alloc_element_arrays(*obj, w);
    }
    std::int32_t start = 0;                 // local_20
    const std::int32_t* rows = nullptr;     // local_18 (set on every path the callers take: *which == -1)
    t.checkpoint("Assign Connectity", nullptr);
    if (!rt::dimension_is_2(obj->owner)) {
        if (rt::dimension_is_3(obj->owner)) {
            if (*which == -1 || *which == mix(0x1406e820c)) {
                if (*which == -1) {
                    start = mar_tab[mix(0x1406e820c)] + -1;
                }
                rows = mar + (start + -1 + iiobj[0x34 / 4]);
            }
            for (std::int32_t k = 0; k < fi(obj, 0x884) * fi(obj, 0x894); k = k + 1) {
                ip(obj, 0x9e8)[k] = (rows[k] - fi(obj, 0x3984)) + 1;
            }
        }
    } else {
        const std::int32_t s = mar_tab[mix(0x1406e820c)];
        const std::int32_t eoff = fi(obj, 0x3988);
        for (std::int32_t k = 0; k < fi(obj, 0x884) * 4; k = k + 1) {
            ip(obj, 0x9e8)[k] = (mar[(s + -5 + eoff * 4) + k] - fi(obj, 0x3984)) + 1;
        }
    }
    t.checkpoint("Assign material group", nullptr);
    if (*which == -1 || *which == mix(0x1406e8210)) {
        if (*which == -1) {
            start = mar_tab[mix(0x1406e8210)] + -1;
        }
        if (w[0xf4 / 4] == mar_tab[mix(0x1406e8270)] || mar_tab[mix(0x1406e8270)] == 0) {
            copy4(ip(obj, 0x9f8), mar + (start + -1 + fi(obj, 0x3988)), fi(obj, 0x884));
            model::geo_kw::update_material_group(*obj); // 140202960
        }
    }
    const char report = 1;
    t.stop(&report);
}

// @src 140176df0  edge / face arrays (140165170 slots, reallocated by 1401631e0) when the stored
//                 version is at least 4000 (3D) / 7000 (2D); the +0xb30.. group in 2D always and
//                 in 3D from 11000
void read_edge_face_info(LLOBJ_DATA* obj, const std::int32_t* mar, const double* a, const std::int32_t* mar_tab,
                         const std::int32_t* a_tab, std::int32_t version)
{
    select_tables_of(obj);
    // 140176e3f: obj + 0xbe0 != 0 (the edge / face sub-object's address)
    if (fi(obj, 0x87c) == 0 || at(obj, 0xbe0) == nullptr) {
        return;
    }
    const std::int32_t ns = model::bc::edge_face_slot_count(*obj);
    model::bc::realloc_edge_face_arrays(*obj);
    if (!rt::dimension_is_2(obj->owner)) {
        if (version < 4000) {
            return;
        }
    } else if (version < 7000) {
        return;
    }
    const std::int32_t off = fi(obj, 0x398c);
    auto mi = [&](std::uint64_t flag, std::uint64_t slot, std::size_t member) {
        if (mar_tab[mix(flag)] == 1) {
            copy4(ip(obj, member), mar + (mar_tab[mix(slot)] + -2 + off), ns);
        }
    };
    auto ad = [&](std::uint64_t flag, std::uint64_t slot, std::size_t member, bool zero_ok) {
        if (a_tab[aix(flag)] == 1 || (zero_ok && a_tab[aix(flag)] == 0)) {
            copy8(dp(obj, member), a + (a_tab[aix(slot)] + -2 + off), ns);
        }
    };
    mi(0x1406e8294, 0x1406e8234, 0xae8);
    mi(0x1406e82a0, 0x1406e8240, 0xaf0);
    ad(0x1406e8478, 0x1406e838c, 0xaf8, true);
    mi(0x1406e8298, 0x1406e8238, 0xb00);
    mi(0x1406e82a4, 0x1406e8244, 0xb08);
    ad(0x1406e847c, 0x1406e8390, 0xb10, true);
    mi(0x1406e829c, 0x1406e823c, 0xb18);
    mi(0x1406e82a8, 0x1406e8248, 0xb20);
    ad(0x1406e8480, 0x1406e8394, 0xb28, true);
    if (rt::dimension_is_2(obj->owner)) {
        mi(0x1406e82c4, 0x1406e8250, 0xb30);
        mi(0x1406e82c8, 0x1406e8254, 0xb38);
        ad(0x1406e846c, 0x1406e8380, 0xb40, false);
    }
    if (10999 < version && rt::dimension_is_3(obj->owner)) {
        mi(0x1406e82c4, 0x1406e8250, 0xb30);
        mi(0x1406e82c8, 0x1406e8254, 0xb38);
        ad(0x1406e846c, 0x1406e8380, 0xb40, false);
    }
}

namespace {

// part of @src 140177ba0 / 140178250: node BC function numbers (+0x998 rows of W[0x13], +0x9a8 rows of
// W[0x15]) in 1..nfn are marked (7 when the NBCD code +0x990 is 6, else 1; 2) and replaced by the
// second IIFNC word of their row
void map_node_functions(LLOBJ_DATA* obj, const std::int32_t* iifnc, std::int32_t nfn, const std::int32_t* w,
                        std::int32_t* mark)
{
    for (std::int32_t k = 0; k < fi(obj, 0x880) * w[0x4c / 4]; k = k + 1) {
        const std::int32_t v = ip(obj, 0x998)[k];
        if (0 < v && v <= nfn) {
            if (ip(obj, 0x990)[k] == 6) {
                mark[v + -1] = 7;
            } else {
                mark[v + -1] = 1;
            }
            ip(obj, 0x998)[k] = iifnc[v * 2 + -1];
        }
    }
    for (std::int32_t k = 0; k < fi(obj, 0x880) * w[0x54 / 4]; k = k + 1) {
        const std::int32_t v = ip(obj, 0x9a8)[k];
        if (0 < v && v <= nfn) {
            mark[v + -1] = 2;
            ip(obj, 0x9a8)[k] = iifnc[v * 2 + -1];
        }
    }
}

} // namespace

// @src 140177ba0  marks 1..7 per function (nodes, then edge / face slots), then the BCCFNC table
//                 (obj+0x2e68, 140136910 clear) gets every marked function but 7 (140136670,
//                 +4 = 1, +0x10 = 1, 1401309b0), SYMSUF points (obj+0x2e80, 3D only) every 7
void read_bc_functions(LLOBJ_DATA* obj, const std::int32_t* iifnc, double* llfnc, std::int32_t nfn,
                       const std::int32_t* w)
{
    auto* mark = static_cast<std::int32_t*>(std::calloc(static_cast<std::size_t>(static_cast<std::int64_t>(nfn)), 4));
    for (std::int32_t k = 0; k < nfn; k = k + 1) {
        mark[k] = 0;
    }
    map_node_functions(obj, iifnc, nfn, w, mark);
    const std::int32_t ns = model::bc::edge_face_slot_count(*obj); // 140165170
    for (std::int32_t k = 0; k < ns; k = k + 1) {
        std::int32_t v = ip(obj, 0xaf0)[k];
        if (ip(obj, 0xae8)[k] != 0 && ip(obj, 0xae8)[k] != 9 && 0 < v && v <= nfn) {
            mark[v + -1] = 3;
            ip(obj, 0xaf0)[k] = iifnc[v * 2 + -1];
        }
        if (ip(obj, 0xb00)[k] != 0 && ip(obj, 0xb00)[k] != 4) {
            v = ip(obj, 0xb08)[k];
            if (0 < v && v <= nfn) {
                mark[v + -1] = 4;
                ip(obj, 0xb08)[k] = iifnc[v * 2 + -1];
            }
        }
        if (ip(obj, 0xb18)[k] != 0 && ip(obj, 0xb18)[k] != 4) {
            v = ip(obj, 0xb20)[k];
            if (0 < v && v <= nfn) {
                mark[v + -1] = 5;
                ip(obj, 0xb20)[k] = iifnc[v * 2 + -1];
            }
        }
        if (ip(obj, 0xb30)[k] != 0 && ip(obj, 0xb30)[k] != 4) {
            v = ip(obj, 0xb38)[k];
            if (0 < v && v <= nfn) {
                mark[v + -1] = 6;
                ip(obj, 0xb38)[k] = iifnc[v * 2 + -1];
            }
        }
    }
    auto* table = reinterpret_cast<model::Cls_1401364a0*>(at(obj, 0x2e68));
    auto* points = reinterpret_cast<model::linked::Cls_140257340List*>(at(obj, 0x2e80));
    table->clear(); // 140136910
    if (rt::dimension_is_3(obj->owner)) {
        points->clear(); // 1402581b0
    }
    for (std::int32_t k = 0; k < nfn; k = k + 1) {
        if (0 < mark[k]) {
            if (mark[k] == 7) {
                if (rt::dimension_is_3(obj->owner)) {
                    model::linked::Cls_140257340* p = points->add(0); // 140257e90
                    p->id = iifnc[k * 2 + 1];
                    axis_point_set(p, llfnc + (iifnc[k * 2] + -1));
                }
            } else {
                DYNARY* d = table->add(); // 140136670
                fi(d, 4) = 1;
                fi(d, 0x10) = 1;
                char flag = 0;
                read_func_from_db_array(d, const_cast<std::int32_t*>(iifnc), llfnc, k + 1, &flag); // 1401309b0
            }
        }
    }
    if (mark != nullptr) {
        std::free(mark);
    }
}

// @src 140178250  the node part of 140177ba0 only (the marks are not used)
void read_node_bc_functions(LLOBJ_DATA* obj, const std::int32_t* iifnc, double* llfnc, std::int32_t nfn,
                            const std::int32_t* w)
{
    (void)llfnc;
    auto* mark = static_cast<std::int32_t*>(std::calloc(static_cast<std::size_t>(static_cast<std::int64_t>(nfn)), 4));
    for (std::int32_t k = 0; k < nfn; k = k + 1) {
        mark[k] = 0;
    }
    map_node_functions(obj, iifnc, nfn, w, mark);
    if (mark != nullptr) {
        std::free(mark);
    }
}

// @src 1401d2be0  ReadArrays (timer DAT_1406edac0): per object (kind != 7) node info (14016dfb0),
//                 node BC arrays, element info (140170ed0), element integers, edge / face arrays
//                 and BC functions; then boundary numbers -> object numbers (record -> object),
//                 material positions -> material numbers, primary die, NBCD / BC references and
//                 MTLGRP renumbered. Branches below 1401d2b70 == 0 are unreachable in this build
//                 (1401d2b70 and 1401d2bb0 always return 1); they are kept as in the binary.
void read_arrays(DB_STEP_DATA* m)
{
    std::int32_t dim = model::llobj::int_at_1c(m); // 140158e90
    model::select_index_tables(&dim);              // 1401c7d40
    model::StepTimer& t = model::g_timer_read_obj_state_variable;
    std::int32_t* mar = static_cast<std::int32_t*>(m->u1b88);
    std::int32_t* iiobj_base = mar + (m->u1b90[mix(0x1406e8208)] + -1);
    t.start("start");
    for (std::int32_t i = 0; i < m->u1378; i = i + 1) {
        auto* obj = static_cast<LLOBJ_DATA*>(model::step_object_at(*m, i + 1)); // 1401d6ed0
        std::int32_t* iiobj = iiobj_base + m->u1b90[mix(0x1406e8268)] * i;
        if (obj == nullptr || fi(obj, 0xc) == 7) {
            continue;
        }
        t.checkpoint("ReadNodeInfoFromDB", nullptr);
        if (!model::names_up_to_date(m)) {
            // 14016dfb0 per A slot, with the address of the slot's index global as the region
            obj_read_node_arrays(obj, mar_of(m), a_of(m) + (m->u1dc0[aix(0x1406e82f0)] + -1), m->u1b90, m->u1dc0, m->u15f8,
                                 m->u13e8, aix_ptr(0x1406e82f0), 0);
            obj_read_node_arrays(obj, mar_of(m), a_of(m) + (m->u1dc0[aix(0x1406e82fc)] + -1), m->u1b90, m->u1dc0, m->u15f8,
                                 m->u13e8, aix_ptr(0x1406e82fc), 0);
        } else if (rt::dimension_is_3(m)) {
            std::int32_t which = -1;
            obj_read_node_arrays(obj, mar_of(m), a_of(m), m->u1b90, m->u1dc0, m->u15f8, m->u13e8, &which, 0); // 14016dfb0
        } else if (rt::dimension_is_2(m)) {
            std::int32_t which = -1;
            obj_read_node_arrays(obj, mar_of(m), a_of(m), m->u1b90, m->u1dc0, m->u15f8, m->u13e8, &which, // 14016dfb0
                                 m->u1b6c);
        }
        if (gendb::step_fun_1401d2bb0(m)) {
            read_node_bc_info(obj, mar_of(m), m->u1b90, m->u15f8, m->u13e8);
        }
        t.checkpoint("ReadElemInfoFromDB", nullptr);
        if (model::names_up_to_date(m)) {
            std::int32_t which = -1;
            obj_read_element_arrays(obj, mar_of(m), a_of(m), m->u1b90, m->u1dc0, m->u15f8, iiobj, &which); // 140170ed0
        }
        if (gendb::step_fun_1401d2bb0(m)) {
            const std::int32_t which = -1;
            read_elem_integer_info(obj, static_cast<std::int32_t*>(m->u1b88), m->u1b90, m->u15f8, iiobj, &which);
        }
        if (!model::names_up_to_date(m)) {
            t.checkpoint("ReadNodeBdryCndFuncFromDB", nullptr);
            read_node_bc_functions(obj, static_cast<std::int32_t*>(m->u2220), static_cast<double*>(m->u2228),
                                   m->u13fc, m->u15f8);
        } else {
            t.checkpoint("ReadEdgeFaceInfoFromDB", nullptr);
            read_edge_face_info(obj, static_cast<std::int32_t*>(m->u1b88), static_cast<double*>(m->u1b78),
                                m->u1b90, m->u1dc0, gendb::stored_version(*m)); // 1401cc180
            t.checkpoint("ReadBdryCndFuncFromDB", nullptr);
            read_bc_functions(obj, static_cast<std::int32_t*>(m->u2220), static_cast<double*>(m->u2228), m->u13fc,
                              m->u15f8);
        }
    }
    if (!gendb::step_fun_1401d2bb0(m)) {
        return;
    }
    t.checkpoint("Boundary Conversion", nullptr);
    std::int32_t* bnd_obj = new_ints(m->u13f0); // boundary record -> object number
    for (std::int32_t i = 0; i < m->u1378; i = i + 1) {
        LLOBJ_DATA* obj = m->objects.at(i + 1); // 14017aa80
        if (obj == nullptr || fi(obj, 0xc) == 7) {
            continue;
        }
        for (std::int32_t k = 0; k < fi(obj, 0x888); k = k + 1) {
            bnd_obj[fi(obj, 0x3588) + -1 + k] = fi(obj, 8);
        }
    }
    t.checkpoint("Material no conversion", nullptr);
    std::int32_t* mat_no = new_ints(m->n_materials); // material position -> material number
    for (std::int32_t i = 0; i < m->n_materials; i = i + 1) {
        mat_no[i] = fi(model::material_list_nth(&m->materials, i + 1), 0); // 14029c260
    }
    t.checkpoint("PDie conversion", nullptr);
    std::int32_t& pdie = fi(m, 0x8b8);
    if (pdie < 1 || m->u13f0 < pdie) {
        if (pdie != 0) {
            rt::crt::printf("ERROR: primary boundry No %d invalid!\n", pdie);
            pdie = 1;
        }
    } else {
        pdie = bnd_obj[pdie + -1];
    }
    t.checkpoint("NBCD conversion", nullptr);
    for (std::int32_t i = 0; i < m->u1378; i = i + 1) {
        LLOBJ_DATA* obj = m->objects.at(i + 1);
        if (fi(obj, 0xc) == 7) {
            continue;
        }
        std::int32_t* nbcd = ip(obj, 0x990);
        if (rt::dimension_is_3(m)) {
            // a node whose three codes are equal and negative keeps code - 200 in the first
            for (std::int32_t k = 0; k < fi(obj, 0x880); k = k + 1) {
                if (nbcd[k * 3] < 0 && nbcd[k * 3] == nbcd[k * 3 + 1] && nbcd[k * 3] == nbcd[k * 3 + 2]) {
                    nbcd[k * 3] = nbcd[k * 3] + -200;
                    nbcd[k * 3 + 2] = 0;
                    nbcd[k * 3 + 1] = 0;
                }
            }
        }
        for (std::int32_t k = 0; k < fi(obj, 0x880) * fi(m, 0x1640); k = k + 1) {
            const std::int32_t v = nbcd[k];
            if (v < 0) {
                bool ok = true;
                if (rt::dimension_is_3(m)) {
                    if (v < -200 && -300 < v) {
                        ok = neg(v) + -200 <= m->u13f0;
                        if (ok) {
                            nbcd[k] = -200 - bnd_obj[neg(v) + -0xc9];
                        }
                    } else if (m->u13f0 < neg(v)) {
                        ok = false;
                    } else {
                        nbcd[k] = neg(bnd_obj[neg(v) + -1]);
                    }
                } else if (rt::dimension_is_2(m)) {
                    if (m->u13f0 < neg(v)) {
                        ok = false;
                    } else {
                        nbcd[k] = neg(bnd_obj[neg(v) + -1]);
                    }
                }
                if (!ok) {
                    nbcd[k] = 0;
                    const std::int32_t node = k / fi(m, 0x1640) + 1;
                    // FUN_14047903c() + 0x30: the CRT's stdout
                    rt::crt::fprintf(stdout, "Object %d Node %d has wrong NBCD Code(%d). Initialized.\n", fi(obj, 8),
                                     node, v);
                }
            }
        }
        for (std::int32_t k = 0; k < fi(obj, 0x880); k = k + 1) {
            if (ip(obj, 0x9a0)[k] < 0) {
                ip(obj, 0x9a0)[k] = neg(bnd_obj[neg(ip(obj, 0x9a0)[k]) + -1]);
            }
        }
        for (std::int32_t k = 0; k < fi(obj, 0x884); k = k + 1) {
            if (ip(obj, 0x9f8)[k] < 1 || m->n_materials < ip(obj, 0x9f8)[k]) {
                rt::crt::printf("ERROR in ReadObjStateVariableFromDB, converting MTLGRP!\n");
            } else {
                ip(obj, 0x9f8)[k] = mat_no[ip(obj, 0x9f8)[k] + -1];
            }
        }
        model::geo_kw::update_material_group(*obj); // 140202960
        if (fi(obj, 0x21c8) == 5 && 0 < fi(obj, 0x25a0) && fi(obj, 0x25a0) <= m->u13f0) {
            fi(obj, 0x25a0) = bnd_obj[fi(obj, 0x25a0) + -1];
        }
    }
    t.checkpoint("free temp memory", nullptr);
    std::free(bnd_obj);
    std::free(mat_no);
    const char report = 0;
    t.stop(&report);
}

} // namespace def::dbio::dbread
