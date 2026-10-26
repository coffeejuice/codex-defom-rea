// dbread/w1_objects.cpp: ReadObjInfo (FUN_1401d2230) and the per-object readers it calls for the
// object header (14016caf0) and the boundary records / curves / surfaces (140174e40)
// (packet p-dbread, helper W1). Decompiles: deform-rea-work/out/dbread/w1/w1_d0.c (annotated:
// w1_ann.c), listings w1_listing.txt. The GENDB mirror is gendb3d/driver.cpp, gendb/p4_06/obj2d.cpp.
#define _CRT_SECURE_NO_WARNINGS
#include <defdbio/dbread/w1.hpp>

#include <cstring>

#include <defdbio/gendb/gate_ports.hpp>
#include <defmodel/bc/boundary_topology.hpp>
#include <defmodel/dynary_init.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/geo_kw/mesh_model.hpp>
#include <defmodel/geo_kw/surface_model.hpp>
#include <defmodel/layout/cls_14017a6c0.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/obj_geo2d.hpp>
#include <defmodel/layout/obj_geo3d.hpp>
#include <defmodel/llobj/cls_14017e740.hpp>
#include <defmodel/llobj_ctor/llobj_ctor.hpp>
#include <defmodel/step/db_step.hpp>
#include <defrt/ftnstr/ftnstr.hpp>
#include <defrt/stdlib/legacy_printf.hpp>
#include <defrt/stdlib/refcount.hpp>

#include "dbread/w1_access.hpp"

namespace def::dbio::dbread {

using model::g_a_index;
using model::g_mar_index;
using w1::at;
using w1::dyn;

// @src 1401d2230  ReadObjInfo
void read_obj_info(DB_STEP_DATA* m)
{
    std::int32_t dim = at<std::int32_t>(m, 0x1c); // 140158e90
    model::select_index_tables(&dim);              // 1401c7d40
    const std::int32_t iiobj = m->u1b90[g_mar_index[3]];
    auto* const mar = static_cast<std::int32_t*>(m->u1b88);
    const std::int32_t llobj = m->u1dc0[g_a_index[5]];
    auto* const a = static_cast<double*>(m->u1b78);
    if (rt::dimension_is_3(m)) {
        at<std::int32_t>(m, 0x1658) = at<std::int32_t>(m, 0x205c);
    }
    auto* const iifnc = static_cast<std::int32_t*>(m->u2220);
    auto* const llfnc = static_cast<double*>(m->u2228);
    for (std::int32_t k = 0; k < m->u1378; k = k + 1) {
        LLOBJ_DATA* obj = model::llobj_ctor::list_add(&m->objects); // 14017a880
        at<std::int32_t>(obj, 8) = k + 1;
        std::int32_t* ii = mar + (iiobj - 1) + m->u1b90[g_mar_index[27]] * k;
        double* ll = a + (llobj - 1) + m->u1dc0[g_a_index[64]] * k;
        read_object_header(obj, ii, ll, iifnc, llfnc);
        if (at<std::int32_t>(obj, 0xc) != 7) {
            read_obj_block_1ea0(w1::ptr(obj, 0x1ea0), ii, ll, iifnc, llfnc);
            const std::int32_t version = m->step_version; // 1401cc180
            read_obj_block_2e68(w1::ptr(obj, 0x2e68), ii, ll, iifnc, llfnc, static_cast<double*>(m->u2230),
                                m->u1dc0[g_a_index[68]], static_cast<double*>(m->u2238), version);
            read_obj_block_2d10(w1::ptr(obj, 0x2d10), ii, ll, iifnc, llfnc, static_cast<double*>(m->u2230),
                                m->u1dc0[g_a_index[68]], static_cast<double*>(m->u2238));
            read_obj_boundaries(obj, static_cast<std::int32_t*>(m->u1b88), static_cast<double*>(m->u1b78),
                                m->u1b90, m->u1dc0, iifnc, static_cast<double*>(m->u2228));
        }
    }
}

// @src 14016caf0
void read_object_header(LLOBJ_DATA* obj, std::int32_t* ii, double* ll, std::int32_t* iifnc, double* llfnc)
{
    auto I = [obj](std::ptrdiff_t off) -> std::int32_t& { return at<std::int32_t>(obj, off); };
    if (ii[0] == 0) {
        I(0xc) = 7;
        return;
    }
    if (ii[0] == 1) {
        I(0x87c) = 1;
    } else if (ii[0] == 2) {
        I(0x87c) = 0;
    }
    const std::int32_t kind = ii[1];
    if (kind == 0) {
        I(0xc) = 0;
    } else if (kind == 1) {
        I(0xc) = 1;
    } else if (kind == 2) {
        I(0xc) = 3;
    } else if (kind == 3) {
        I(0xc) = 4;
    } else if (kind == 4) {
        I(0xc) = 6;
    }
    if (ii[2] == 1) {
        I(0xc) = 2;
    }
    I(0x3984) = ii[4];
    I(0x880) = (ii[5] - I(0x3984)) + 1;
    if (I(0x880) < 1) {
        I(0x880) = 0;
    }
    I(0x3988) = ii[6];
    I(0x884) = (ii[7] - I(0x3988)) + 1;
    if (I(0x884) < 1) {
        I(0x884) = 0;
    }
    if (I(0x87c) == 0) {
        I(0x884) = 0;
        I(0x880) = 0;
    }
    I(0x3588) = ii[8];
    I(0x358c) = ii[9];
    I(0x888) = (I(0x358c) - I(0x3588)) + 1;
    if (I(0x888) < 0x65) {
        if (I(0x87c) != 0) {
            model::bc::set_boundary_count(*obj, I(0x888)); // 140163300
        }
        model::DB_STEP_DATA* owner = obj->owner;
        if (!rt::dimension_is_2(owner)) {
            if (rt::dimension_is_3(owner)) {
                if (ii[0xc] == 4) {
                    model::geo_kw::set_element_type(*obj, 2); // 140165500
                } else if (ii[0xc] == 8) {
                    model::geo_kw::set_element_type(*obj, 4);
                } else {
                    model::geo_kw::set_element_type(*obj, 2);
                }
            }
        } else if (ii[10] == 0) {
            I(0x890) = 0;
        } else if (ii[10] == 1) {
            I(0x890) = 1;
        }
        std::strncpy(obj->name, reinterpret_cast<const char*>(ll) + 0x20, 0x40);
        text_byteswap(obj->name, obj->owner->u748, 8, 0); // 140324020
        rt::trim_fortran(obj->name, 0x40);                // 1403239f0
        I(0x3590) = static_cast<std::int32_t>(ll[0x30]);
        if (I(0x3590) != 0) {
            const double fn = ll[0x31];
            model::dynary_init(&obj->u3598, 0x7c); // 14012d2a0
            I(0x35d8) = I(0x3590);                  // u3598.ncomp
            char with_shape = 0;
            read_func_from_db_array(&obj->u3598, iifnc, llfnc, static_cast<std::int32_t>(fn), &with_shape);
        }
    } else {
        rt::crt::printf("ERROR: NoBdry = %d greater than maximum!\n", I(0x888));
    }
}

// @src 140198230
void set_curve_count(LLOBJ_DATA* obj, std::int32_t n)
{
    model::GEO2D_AND_MESH2D* g = model::geo_kw::geometry(*obj, 0); // 140197f40
    geo2d_set_edge_count(g, &n);                                   // 14027fe20
}

// @src 140163340
void set_surface_or_curve_count(LLOBJ_DATA* obj, std::int32_t n)
{
    if (!rt::dimension_is_3(obj->owner)) {
        if (rt::dimension_is_2(obj->owner)) {
            set_curve_count(obj, n);
        }
        return;
    }
    std::int32_t k = n;
    if (0 < n) {
        for (; k < model::geo_kw::surface_count(*obj); k = k + 1) { // 1401985a0
            auto* s = static_cast<model::OBJ_GEO3D*>(obj->u3c00[k]);
            s->clear(); // 1402f9340
            s->reset(); // vtable slot 2
        }
    }
    std::int32_t c = n;
    if (100 < n) {
        c = 100;
    }
    at<std::int32_t>(obj, 0x3994) = c;
}

// @src 140174e40
void read_obj_boundaries(LLOBJ_DATA* obj, std::int32_t* mar, double* a, std::int32_t* mar_tab, std::int32_t* a_tab,
                         std::int32_t* iifnc, double* llfnc)
{
    namespace gk = model::geo_kw;
    auto I = [obj](std::ptrdiff_t off) -> std::int32_t& { return at<std::int32_t>(obj, off); };
    model::DB_STEP_DATA* const owner = obj->owner;
    std::int32_t dim = at<std::int32_t>(owner, 0x1c); // 140158e90
    model::select_index_tables(&dim);
    double* const llbnd = a + (a_tab[g_a_index[6]] - 1);
    if (!(0 < I(0x3588))) {
        return;
    }
    double* rec = llbnd + a_tab[g_a_index[65]] * (I(0x3588) - 1);
    std::memcpy(w1::ptr(obj, 0x3008), rec, 800);
    if (!rt::dimension_is_3(owner)) {
        if (rt::dimension_is_2(owner) && (rec[0] == 1.0 || rec[0] == 2.0)) {
            model::GEO2D_AND_MESH2D* g = gk::geometry(*obj, 0); // 140197f40
            geo2d_set_edge_count(g, &I(0x888));                 // 14027fe20
        }
    } else {
        if (rec[0] == 1.0 || rec[0] == 2.0) {
            gk::set_surface_flag(*obj, true); // 1401985e0
        }
        if (gk::has_surface_flag(*obj)) {
            set_surface_or_curve_count(obj, I(0x888)); // 140163340
        }
    }
    std::int32_t* blk = nullptr; // local_e0: the on-demand MAR group (unused in this build)
    for (std::int32_t i = 0; i < I(0x888); i = i + 1) {
        rec = llbnd + a_tab[g_a_index[65]] * (i - 1 + I(0x3588));
        const std::ptrdiff_t b = 0xbe0 + static_cast<std::ptrdiff_t>(i) * 0x30; // boundary record i
        if (!rt::dimension_is_2(owner)) {
            if (!rt::dimension_is_3(owner)) {
                continue;
            }
            if (I(0x87c) == 1) {
                I(b) = static_cast<std::int32_t>(rec[1]);
                I(b + 0x10) = static_cast<std::int32_t>(rec[3]);
                model::bc::alloc_boundary_arrays(at<model::Cls_14017e740>(obj, b)); // 14017e8a0
                const std::int32_t first = static_cast<std::int32_t>(rec[4]);
                if (i == 0) {
                    I(0x398c) = first;
                }
                if (obj->owner == nullptr) {
                    rt::crt::fprintf(w1::out_stream(), "m_pDbStepData has not been set.\n");
                }
                std::int32_t* src;
                if (!gendb::step_fun_1401d2bb0(obj->owner)) {
                    std::int32_t grp = 4;
                    blk = static_cast<std::int32_t*>(gendb::step_read_mar_on_demand(obj->owner, &grp)); // 1401d5820
                    src = blk + (first * 4 - 4);
                } else {
                    src = mar + (mar_tab[g_mar_index[12]] - 5 + first * 4);
                }
                auto* faces = at<std::int32_t*>(obj, b + 0x18);
                for (std::int32_t f = 0; f < I(b + 0x10) * 4; f = f + 1) {
                    faces[f] = (src[f] - I(0x3984)) + 1;
                }
                if (!gendb::step_fun_1401d2bb0(obj->owner)) {
                    std::int32_t grp = 5;
                    blk = static_cast<std::int32_t*>(gendb::step_read_mar_on_demand(obj->owner, &grp));
                    src = blk + (first - 1);
                } else {
                    src = mar + (mar_tab[g_mar_index[13]] - 2 + first);
                }
                auto* elems = at<std::int32_t*>(obj, b + 0x20);
                for (std::int32_t f = 0; f < I(b + 0x10); f = f + 1) {
                    elems[f] = (src[f] - I(0x3988)) + 1;
                }
            }
            if (gk::has_surface_flag(*obj) == true) {
                model::OBJ_GEO3D* s = gk::surface(*obj, i); // 140198620
                std::int32_t nelem = static_cast<std::int32_t>(rec[7]);
                std::int32_t nnode = static_cast<std::int32_t>(rec[5]);
                gk::geo3d_size(s, &nnode, &nelem); // 1402fcd90
                std::int32_t pos = static_cast<std::int32_t>(rec[6]);
                const double* xyz = a + (a_tab[g_a_index[8]] - 2 + pos);
                const std::int32_t nn = s->node_count();           // 1400034d0
                double* coords = gk::geo3d_coords(s);              // 140006410
                const std::int32_t ne = s->elem_count();           // 140003500
                std::int32_t* elems = gk::geo3d_elems(s);          // 140005510
                std::int32_t* ids = gk::geo3d_elem_ids(s);         // 140199fa0
                for (std::int32_t f = 0; f < nn * 3; f = f + 1) {
                    std::memcpy(&coords[f], &xyz[f], 8);
                }
                pos = static_cast<std::int32_t>(rec[8]);
                std::int32_t* src;
                if (!gendb::step_fun_1401d2bb0(obj->owner)) {
                    std::int32_t grp = 3;
                    blk = static_cast<std::int32_t*>(gendb::step_read_mar_on_demand(obj->owner, &grp));
                    src = blk + (pos - 1);
                } else {
                    src = mar + (mar_tab[13] - 2 + pos); // the table slot itself, not through g_mar_index
                }
                for (std::int32_t f = 0; f < ne * 4; f = f + 1) {
                    elems[f] = src[f];
                }
                pos = static_cast<std::int32_t>(rec[0x61]); // +0x308
                if (pos == 0) {
                    geo3d_zero_elem_ids(s); // 1402f9af0
                } else {
                    if (!gendb::step_fun_1401d2bb0(obj->owner)) {
                        std::int32_t grp = 3;
                        blk = static_cast<std::int32_t*>(gendb::step_read_mar_on_demand(obj->owner, &grp));
                        src = blk + (pos - 1);
                    } else {
                        src = mar + (mar_tab[13] - 2 + pos);
                    }
                    for (std::int32_t f = 0; f < ne; f = f + 1) {
                        ids[f] = src[f];
                    }
                }
                const char rebuild = 1;
                const std::int32_t npatch = gendb::geo3d_surface_patch_count(s, &rebuild); // 1402fcfe0
                if (npatch != static_cast<std::int32_t>(rec[0x66])) { // +0x330
                    rt::crt::fprintf(w1::out_stream(),
                                     "Info : during bearing surface loading, erroneous entity has been found.\n");
                    const double db = rec[0x66];
                    rt::crt::fprintf(w1::out_stream(), "       Count : %d <--> DB : %d\n", npatch,
                                     static_cast<std::int32_t>(db));
                }
                const std::int32_t p0 = static_cast<std::int32_t>(rec[0x62]); // +0x310
                const std::int32_t p1 = static_cast<std::int32_t>(rec[0x63]); // +0x318
                if (0 < p0) {
                    for (std::int32_t k = p0; k <= p1; k = k + 1) {
                        std::int32_t id = (k - p0) + 1;
                        std::int32_t type = 0;
                        gk::DiePointSet* ps = gk::geo3d_point_set_for(s, &id, &type); // 1402fd020
                        const std::int32_t off = iifnc[k * 2 - 2] - 1;
                        die_point_set_from_values(ps, llfnc + off); // 1402f8850
                    }
                }
                const std::int32_t e0 = static_cast<std::int32_t>(rec[0x64]); // +0x320
                const std::int32_t e1 = static_cast<std::int32_t>(rec[0x65]); // +0x328
                if (0 < e0) {
                    for (std::int32_t k = e0; k <= e1; k = k + 1) {
                        std::int32_t id = (k - e0) + 1;
                        std::int32_t type = 0;
                        gk::DieEdgeSet* es = gk::geo3d_edge_set_for(s, &id, &type); // 1402fd490
                        const std::int32_t off = iifnc[k * 2 - 2] - 1;
                        die_edge_set_from_values(es, llfnc + off); // 1402f8de0
                    }
                }
            }
        } else {
            if (I(0x87c) == 1) {
                I(b) = static_cast<std::int32_t>(rec[2]);
                model::bc::alloc_boundary_arrays(at<model::Cls_14017e740>(obj, b)); // 14017e8a0
                const std::int32_t first = static_cast<std::int32_t>(rec[3]);
                if (i == 0) {
                    I(0x398c) = first;
                }
                const std::int32_t* src = mar + (mar_tab[g_mar_index[12]] - 2 + first);
                auto* nodes = at<std::int32_t*>(obj, b + 8);
                for (std::int32_t f = 0; f < I(b); f = f + 1) {
                    nodes[f] = (src[f] - I(0x3984)) + 1;
                }
            }
            if (gk::geometry_has_edges(*obj) == true) { // 140160d20
                gk::curve(*obj, i)->curve_type = 2;    // 1401982c0
                const double npt = rec[4];
                gk::curve(*obj, i)->n_points = static_cast<std::int32_t>(npt);
                gk::curve(*obj, i)->clear_data(); // vtable slot 3: allocate the point array
                const std::int32_t pos = static_cast<std::int32_t>(rec[5]);
                const double* src = a + (a_tab[g_a_index[8]] - 2 + pos);
                const std::int32_t rows = gk::curve(*obj, i)->n_points;
                const std::int32_t dof = gk::curve(*obj, i)->dof(); // 1402760e0
                const std::int32_t cnt = rows * dof;
                std::memcpy(gk::curve(*obj, i)->points, src, static_cast<std::size_t>(static_cast<std::int64_t>(cnt) << 3));
                const bool dropped = curve_drop_bad_segments(gk::curve(*obj, i)); // 14027e700
                if (dropped) {
                    rt::crt::fprintf(w1::out_stream(),
                                     "Info : during geometry loading, erroneous entity has been removed.\n");
                }
                if (static_cast<std::int32_t>(rec[6]) == 1) {
                    gk::curve(*obj, i)->set_curve_type(1); // 1402763d0
                }
            }
        }
    }
    (void)blk;
    rec = a + (a_tab[g_a_index[6]] - 1 + a_tab[g_a_index[65]] * (I(0x3588) - 1));
    read_movement(w1::ptr(obj, 0x2160), rec, iifnc, llfnc, obj->owner->u748); // 14018d3c0
    if (rt::dimension_is_3(obj->owner)) {
        w1::copy8(obj, 0x2058, rec, 0x58);
        w1::copy8(obj, 0x2060, rec, 0x178);
        w1::copy8(obj, 0x2068, rec, 0x180);
        w1::copy8(obj, 0x2070, rec, 0x188);
        w1::copy8(obj, 0x2078, rec, 0x160);
        w1::copy8(obj, 0x2080, rec, 0x168);
        w1::copy8(obj, 0x2088, rec, 0x170);
    }
}

} // namespace def::dbio::dbread
