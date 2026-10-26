// p4-02b: the GENDB counts frame 1401bc230 and its DB_STEP_DATA-level counters.
// Spec: docs/re/dbmap/stepdriver.md section 5.4. Messages go to the CRT's stdout
// (FUN_14047903c() + 0x30 is _iob[1]) through def::rt::crt::fprintf.
#include <defdbio/gendb/counts_frame.hpp>

#include <cstdio>
#include <cstring>

#include <defdbio/gendb/counts_ext.hpp>
#include <defmodel/layout/obj_geo3d.hpp>
#include <defmodel/step/db_step.hpp>
#include <defrt/stdlib/legacy_printf.hpp>
#include <defrt/stdlib/refcount.hpp>

namespace def::dbio::gendb {

using model::field;
namespace crt = def::rt::crt;

namespace {

// object number (+8) and kind (+0xc) of an LLOBJ_DATA; kind 7 objects are skipped everywhere
std::int32_t& obj_number(LLOBJ_DATA* o) { return field<std::int32_t>(o, 0x8); }
std::int32_t obj_kind(LLOBJ_DATA* o) { return field<std::int32_t>(o, 0xc); }
// node count of the mesh (+0x8a0, 140182840; count at +0x12c) and element count of the
// connectivity (+0x9e0, 1401854f0; count at +0xe8). The calls feed NUMNP (+0x6c) and NUMEL (+0x70)
// respectively, so 0x8a0 holds the nodes and 0x9e0 the elements (the layout comments of
// Cls_14017ef20 / Cls_140182880 have the two names the other way round).
std::int32_t obj_nodes(LLOBJ_DATA* o) { return field<std::int32_t>(o, 0x8a0 + 0x12c); }
std::int32_t obj_elems(LLOBJ_DATA* o) { return field<std::int32_t>(o, 0x9e0 + 0xe8); }
// nodes per element (140185530, +0x9e0 + 0xec)
std::int32_t obj_elem_width(LLOBJ_DATA* o) { return field<std::int32_t>(o, 0x9e0 + 0xec); }

} // namespace

// @src 14001cd60  empty diagnostic hook (TRANS)
void diag_14001cd60(const std::int32_t*, const char*, const void*, const void*) {}
// @src 14001cd70  empty diagnostic hook (MSTMTR)
void diag_14001cd70(const std::int32_t*, const char*, const void*, const void*) {}
// @src 14001cd80  empty diagnostic hook (PHASTF)
void diag_14001cd80(const std::int32_t*, const char*, const void*, const void*) {}
// @src 14001cd90  empty diagnostic hook (OBJNA, MTLGRP, CRPTIM, DIEGEO, PHASTF, OBJNAM)
void diag_14001cd90(const std::int32_t*, const char*, const void*, const void*) {}

// @src 14019a5c0  DB_WRITE_VAR+0x134 = *v
void set_u134(DB_STEP_DATA* m, const std::int32_t* v) { m->u134 = *v; }
// @src 14019a600  DB_WRITE_VAR+0x138 = *v
void set_u138(DB_STEP_DATA* m, const std::int32_t* v) { m->u138 = *v; }
// @src 1401f4610  DB_WRITE_VAR+0x7c (NOBJ)
std::int32_t object_count_field(DB_STEP_DATA* m) { return m->u074[2]; }
// @src 1401fb1e0  DB_WRITE_VAR+0xe4 = *v
void set_u0e4(DB_STEP_DATA* m, const std::int32_t* v) { m->u0e4 = *v; }

// @src 1401bd300  omitted: licence. The original asks the licence manager (1400b8370) per module
// and stores the answers at DB_WRITE_VAR+0x20 (microstructure), +0x24 (diffusion), +0x28 and +0x2c
// (grain / heating). The port starts no licence check and runs with every module available.
void module_licence_flags(DB_STEP_DATA* m)
{
    m->u020 = 1;
    m->u024 = 1;
    m->u028 = 1;
    m->u02c = 1;
}

// @src 1401caf40  largest object number among the objects (by position) that are not kind 7
std::int32_t max_object_number(DB_STEP_DATA* m)
{
    std::int32_t mx = 0;
    for (std::int32_t i = 0; i < m->u1378; i = i + 1) {
        auto* o = static_cast<LLOBJ_DATA*>(model::step_object_at(*m, i + 1));
        if (o != nullptr && obj_kind(o) != 7 && mx < obj_number(o)) {
            mx = obj_number(o);
        }
    }
    return mx;
}

// @src 1401bc230  counts frame
void counts_frame(DB_STEP_DATA* m, std::int32_t* err)
{
    std::int32_t code = 0; // local_64
    std::int32_t a = 0;    // local_68
    std::int32_t b = 0;    // local_60
    Cls_1401e7b10& s = m->sim_ctrl;

    model::db_write_var_init(*m);
    std::memcpy(&m->u0e8, &s.geoerr[0], 8); // m+0xe8 = m+0xdb0
    std::memcpy(&m->u0f0, &s.geoerr[1], 8); // m+0xf0 = m+0xdb8
    material_list_clear_flags(&m->materials);
    std::int32_t one = 1;
    set_u134(m, &one);
    std::int32_t one2 = 1;
    set_u138(m, &one2);

    if (s.trans[0] == 0 && s.trans[1] == 0 && s.trans[2] == 0 && s.trans[3] == 0) {
        crt::fprintf(stdout, "ERROR  : No mode has been activated\n");
        crt::fprintf(stdout, "         TRANS(1) .. TRANS(4) = 0\n");
        code = 1;
        a = 0;
        b = 0;
        diag_14001cd60(&code, "TRANS", &a, &b);
        *err = 1;
        return;
    }

    module_licence_flags(m);
    if (0 < s.trans[2] && m->u020 == 0) {
        crt::fprintf(stdout, "The microstructure module has not been licensed\n");
        crt::fprintf(stdout, "so it is being deactivated\n");
        s.trans[2] = 0;
        code = 2;
        a = 0;
        b = 0;
        diag_14001cd60(&code, "TRANS", &a, &b);
    }
    if (0 < s.trans[3] && m->u024 == 0) {
        crt::fprintf(stdout, "The diffusion module has not been licensed\n");
        crt::fprintf(stdout, "so it is being deactivated\n");
        s.trans[3] = 0;
        code = 3;
        a = 0;
        b = 0;
        diag_14001cd60(&code, "TRANS", &a, &b);
    }
    if (0 < s.trans[4] && m->u028 == 0) {
        crt::fprintf(stdout, "The grain module has not been licensed\n");
        crt::fprintf(stdout, "so it is being deactivated\n");
        s.trans[4] = 0;
        code = 0x25;
        a = 0;
        b = 0;
        diag_14001cd60(&code, "TRANS", &a, &b);
    }
    if (0 < s.htmthd && m->u028 == 0) {
        crt::fprintf(stdout, "The heating module has not been licensed\n");
        crt::fprintf(stdout, "so it is being deactivated\n");
        s.htmthd = 0;
        code = 4;
        a = 0;
        b = 0;
        diag_14001cd60(&code, "TRANS", &a, &b);
    }
    m->u13c = sim_ctrl_smode(&s);

    if (max_object_number(m) < 0x65) {
        if (!rt::dimension_is_3(m)) {
            object_counts_2d(m, &m->u074[2], &m->u06c, &m->u070, &m->u074[3], &m->u074[4],
                             &m->u074[0], &m->u074[17], &m->u104, &m->u108, err);
        } else {
            object_counts_3d(m, &m->u074[2], &m->u06c, &m->u070, &m->u074[11], &m->u074[3],
                             &m->u074[7], &m->u074[8], &m->u074[9], &m->u074[10], &m->u074[0],
                             &m->u074[12], &m->u074[13], &m->u074[14], &m->u074[15],
                             &m->u074[16], err);
        }
        material_count(m, &m->u074[1], err);
        relation_count(m, &m->u074[18], err);
        material_group_count(m, &m->u0c8, err);
        element_name_width(m, &m->u0d8, err);
        node_name_width(m, &m->u0dc, err);
        sim_ctrl_function_counts(m, &m->u074[5], &m->u074[6], err);
        step_material_function_counts(m, &m->u074[5], &m->u074[6], err);
        phase_pair_function_counts(m, &m->u074[5], &m->u074[6], err);
        gendb_ext().fun_1401beb30(m, &m->u074[5], &m->u074[6], &m->u074[19], &m->u074[20], err);
        inter_object_function_counts(m, &m->u074[5], &m->u074[6], &m->u074[19], &m->u074[20], err);
    } else {
        crt::fprintf(stdout, "ERROR  : The number of objects exceeds its limit\n");
        crt::fprintf(stdout, "         100 is the maximum allowed number\n");
        code = 5;
        a = 0;
        b = 0;
        diag_14001cd90(&code, "OBJNAM", &a, &b);
        *err = 1;
    }
}

// @src 1401bdaf0  2D object counts. Arguments (DB_WRITE_VAR offsets from 1401bc230): nobj 0x7c,
// numnp 0x6c, numel 0x70, nbcrv 0x80, nbseg 0x84, ntot 0x74, nout 0xb8, ndof 0x104, ndof2 0x108.
void object_counts_2d(DB_STEP_DATA* m, std::int32_t* nobj, std::int32_t* numnp,
                      std::int32_t* numel, std::int32_t* nbcrv, std::int32_t* nbseg,
                      std::int32_t* ntot, std::int32_t* nout, std::int32_t* ndof,
                      std::int32_t* ndof2, std::int32_t* err)
{
    GendbExt& x = gendb_ext();
    Cls_1401e7b10& s = m->sim_ctrl;
    *nobj = max_object_number(m);
    if (*nobj <= 0) {
        return;
    }
    *err = 0;
    s.u680 = 0.0; // m+0xdd0: largest coordinate range
    std::int32_t ncurv = 0; // [rsp+0x20]
    for (std::int32_t k = 1; k <= m->u1378; k = k + 1) {
        LLOBJ_DATA* o = x.fun_14017aaf0(&m->objects, k);
        if (o == nullptr || obj_kind(o) == 7) {
            continue;
        }
        ncurv = x.fun_140198310(o);
        if (!object_skip_mesh(o)) {
            if (obj_nodes(o) < 4 || obj_elems(o) < 1) {
                crt::fprintf(stdout, "ERROR: Object %4d should have valid mesh\n", obj_number(o));
                std::int32_t c = 0x48, z = 0;
                diag_14001cd90(&c, "", &obj_number(o), &z); // str 1405264e7 (empty)
                *err = 1;
            } else {
                *numnp = obj_nodes(o) + *numnp;
                *numel = obj_elems(o) + *numel;
                std::int32_t nmc = field<std::int32_t>(o, 0xbd8); // curves of the mesh boundary
                ncurv = 0;
                *nbcrv = *nbcrv + nmc;
                if (obj_kind(o) == 0 && x.fun_140198310(o) != field<std::int32_t>(o, 0xbd8)) {
                    std::int32_t g = x.fun_140198310(o);
                    crt::fprintf(stdout, "WARNING: The number of curves on the geometry %3d\n", g);
                    crt::fprintf(stdout, "         does not equal the number of curves in \n");
                    crt::fprintf(stdout, "         the mesh %3d\n", field<std::int32_t>(o, 0xbd8));
                    std::int32_t c = 2;
                    std::int32_t gv = x.fun_140198310(o);
                    diag_14001cd90(&c, "DIEGEO", &gv, &field<std::int32_t>(o, 0xbd8));
                }
                for (std::int32_t i = 0; i < field<std::int32_t>(o, 0xbd8); i = i + 1) {
                    *nbseg = *nbseg + field<std::int32_t>(o, 0xbe0 + static_cast<std::size_t>(i) * 0x30);
                }
                double mn[2] = {0.0, 0.0};
                double mx[2] = {0.0, 0.0};
                if (rt::dimension_is_2(m)) {
                    object_mesh_range(o, mn, mx);
                }
                // 1401bde59: COMISD cur, diff; JBE takes the difference (also when unordered)
                double d0 = mx[0] - mn[0];
                s.u680 = (s.u680 > d0) ? s.u680 : mx[0] - mn[0];
                double d1 = mx[1] - mn[1];
                s.u680 = (s.u680 > d1) ? s.u680 : mx[1] - mn[1];
                std::uint32_t flags = 0;
                if (s.trans[1] == 1) {
                    if (obj_kind(o) == 1 || obj_kind(o) == 2) {
                        flags = 1;
                    }
                    if (obj_kind(o) == 3) {
                        flags = flags + 0x10;
                    }
                    if (obj_kind(o) == 4) {
                        flags = flags + 0x11;
                    }
                    if (obj_kind(o) == 6) {
                        flags = flags + 0x11;
                    }
                }
                if (s.trans[0] == 1) {
                    flags = flags + 0x100;
                }
                if (field<void*>(o, 0x9f8) != nullptr) {
                    std::int32_t bad = 0;
                    if (!object_mark_groups(o, flags, &bad)) {
                        crt::fprintf(stdout,
                                     "ERROR: Invalid Material Group Number %6d for Object # %4d\n",
                                     bad, obj_number(o));
                        std::int32_t c = 1;
                        diag_14001cd90(&c, "MTLGRP", &bad, &obj_number(o));
                        *err = 1;
                    }
                }
            }
            if (0 < field<std::int32_t>(o, 8000) && m->u020 == 0) {
                crt::fprintf(stdout, "   Creep can be active only as part of the\n");
                crt::fprintf(stdout, "   heat treat system. The microstructure\n");
                crt::fprintf(stdout, "   module has not been licensed so creep\n");
                crt::fprintf(stdout, "   is being deactivated\n");
                field<std::int32_t>(o, 8000) = 0;
                std::int32_t c = 3, z1 = 0, z2 = 0;
                diag_14001cd90(&c, "CRPTIM", &z1, &z2);
            }
        }
        std::int32_t nc = x.fun_140198310(o);
        bool any_type1 = false;
        for (std::int32_t i = 0; i < nc; i = i + 1) {
            OBJ_GEO2D* g = x.fun_1401982c0(o, &i);
            if (g != nullptr && obj_kind(o) == 0 && 0 < g->n_points && g->is_extension() == 0) {
                if (g->curve_type == 1) {
                    std::int32_t ne = geo2d_entity_count(g);
                    *nout = *nout + ne;
                    any_type1 = true;
                } else {
                    *nout = *nout + g->n_points;
                }
            }
        }
        if (rt::dimension_is_2(m) && obj_kind(o) == 0 && object_geo2d_usable(o)) {
            double mn[2] = {0.0, 0.0};
            double mx[2] = {0.0, 0.0};
            object_geo2d_range(o, mn, mx);
            double d0 = mx[0] - mn[0];
            s.u680 = (s.u680 > d0) ? s.u680 : mx[0] - mn[0];
            double d1 = mx[1] - mn[1];
            s.u680 = (s.u680 > d1) ? s.u680 : mx[1] - mn[1];
            if (any_type1) {
                *nout = nc + -1 + *nout;
            }
            *nbcrv = *nbcrv + ncurv;
        }
    }
    if (s.geotyp == 3) {
        *ndof = 3;
        *ndof2 = 6;
    } else {
        *ndof = 2;
        *ndof2 = 4;
    }
    *ntot = *numnp + *nbcrv;
    std::int32_t c = 0x19, z = 0;
    diag_14001cd90(&c, "OBJNA", nobj, &z);
}

// @src 1401bd3d0  3D object counts. Arguments (DB_WRITE_VAR offsets from 1401bc230): nobj 0x7c,
// numnp 0x6c, numel 0x70, nconn 0xa0, nmesh 0x80, nbnd 0x90, nbfac 0x94, ngelm 0x98, ngnod 0x9c,
// ntot 0x74, ndnp 0xa4, ndel 0xa8, nrnp 0xac, nrfac 0xb0, nconn8 0xb4.
void object_counts_3d(DB_STEP_DATA* m, std::int32_t* nobj, std::int32_t* numnp,
                      std::int32_t* numel, std::int32_t* nconn, std::int32_t* nmesh,
                      std::int32_t* nbnd, std::int32_t* nbfac, std::int32_t* ngelm,
                      std::int32_t* ngnod, std::int32_t* ntot, std::int32_t* ndnp,
                      std::int32_t* ndel, std::int32_t* nrnp, std::int32_t* nrfac,
                      std::int32_t* nconn8, std::int32_t* err)
{
    GendbExt& x = gendb_ext();
    Cls_1401e7b10& s = m->sim_ctrl;
    std::int32_t seen = 0; // iVar5
    *nobj = max_object_number(m);
    if (*nobj <= 0) {
        return;
    }
    for (std::int32_t k = 1; k <= m->u1378; k = k + 1) {
        LLOBJ_DATA* o = x.fun_14017aaf0(&m->objects, k);
        if (o == nullptr || obj_kind(o) == 7) {
            continue;
        }
        seen = 1;
        if (!object_skip_mesh(o)) {
            if (obj_nodes(o) < 4 || obj_elems(o) < 1) {
                crt::fprintf(stdout, "ERROR: Object %4d should have valid mesh\n", obj_number(o));
                std::int32_t c = 0x48, z = 0;
                diag_14001cd90(&c, "", &obj_number(o), &z); // str 1405264e6 (empty)
                *err = 1;
            } else {
                *numnp = obj_nodes(o) + *numnp;
                *numel = obj_elems(o) + *numel;
                *nconn = obj_elems(o) * obj_elem_width(o) + *nconn;
                if (obj_elem_width(o) == 8) {
                    *nconn8 = *nconn8 + obj_elems(o) * 8;
                } else {
                    *nconn8 = obj_elems(o) + *nconn8;
                }
                if (0 < obj_kind(o)) {
                    *ndnp = obj_nodes(o) + *ndnp;
                    *ndel = obj_elems(o) + *ndel;
                }
                // local_40 = 4 (3 for width 4) is computed but not used
                auto* bnd = reinterpret_cast<Cls_14017e740*>(&o->sbe0[0][0]);
                boundary_node_list(bnd, &o->u87c[1]); // obj+0x880
                *nbfac = *nbfac + field<std::int32_t>(o, 0xbf0);
                *nbnd = *nbnd + field<std::int32_t>(o, 0xbe0);
                if (obj_kind(o) != 0) {
                    *nrnp = obj_nodes(o) + *nrnp;
                    *nrfac = *nrfac + field<std::int32_t>(o, 0xbf0);
                    m->u0d0 = object_geo3d_nodes(o) + m->u0d0;
                    m->u0d4 = object_geo3d_elems(o) + m->u0d4;
                }
                std::uint32_t flags = 0;
                if (s.trans[1] == 1) {
                    if (obj_kind(o) == 1 || obj_kind(o) == 2) {
                        flags = 1;
                    }
                    if (obj_kind(o) == 3) {
                        flags = flags + 0x10;
                    }
                    if (obj_kind(o) == 4) {
                        flags = flags + 0x11;
                    }
                }
                if (s.trans[0] == 1) {
                    flags = flags + 0x100;
                }
                if (field<void*>(o, 0x9f8) != nullptr) {
                    std::int32_t bad = 0;
                    if (!object_mark_groups(o, flags, &bad)) {
                        crt::fprintf(stdout,
                                     "ERROR: Invalid Material Group Number %6d for Object # %4d\n",
                                     bad, obj_number(o));
                        std::int32_t c = 1;
                        diag_14001cd90(&c, "MTLGRP", &bad, &obj_number(o));
                        *err = 1;
                    }
                }
            }
            if (0 < field<std::int32_t>(o, 8000) && m->u020 == 0) {
                crt::fprintf(stdout, "   Creep can be active only as part of the\n");
                crt::fprintf(stdout, "   heat treat system. The microstructure\n");
                crt::fprintf(stdout, "   module has not been licensed so creep\n");
                crt::fprintf(stdout, "   is being deactivated\n");
                field<std::int32_t>(o, 8000) = 0;
                std::int32_t c = 3, z1 = 0, z2 = 0;
                diag_14001cd90(&c, "CRPTIM", &z1, &z2);
            }
        }
        *nmesh = *nmesh + seen;
        // 140198620(o, &0): the first geometry slot u3c00[0]
        auto* g = static_cast<model::OBJ_GEO_BASE*>(o->u3c00[0]);
        if (g != nullptr && obj_kind(o) == 0) {
            auto* g3 = static_cast<model::OBJ_GEO3D*>(g);
            if (1 < g3->node_count() && g->is_extension() == 100) {
                *ngnod = g3->node_count() + *ngnod;
                *ngelm = g3->elem_count() + *ngelm;
            }
        }
    }
    *ntot = *numnp + *nmesh;
    std::int32_t c = 0x19, z = 0;
    diag_14001cd90(&c, "OBJNA", nobj, &z);
    if (0 < m->u0d0 && 0 < m->u0d4) {
        std::int32_t one = 1;
        model::db_write_var_set_u0cc(*m, &one);
    }
}

// @src 1401be510  number of materials written (MSTMTR mixtures renumbered)
void material_count(DB_STEP_DATA* m, std::int32_t* nmat, std::int32_t* err)
{
    std::int32_t l[4];
    l[2] = 0;
    l[1] = 0;
    l[3] = 0;
    l[0] = material_list_mark_mixtures(&m->materials);
    if (0 < m->sim_ctrl.trans[2] && l[0] == 0) {
        crt::fprintf(stdout, "ERROR  : One mixture material must be specified\n");
        crt::fprintf(stdout, "         for transformation to be active\n");
        l[2] = 1;
        l[1] = 0;
        l[3] = 0;
        diag_14001cd70(&l[2], "MSTMTR", &l[1], &l[3]);
        *err = 1;
    }
    *nmat = material_list_renumber(&m->materials);
    if (m->u009 != 0) {
        l[2] = 3;
        l[3] = 0;
        diag_14001cd70(&l[2], "MSTMTR", &l[0], &l[3]);
        l[2] = 4;
        l[3] = 0;
        diag_14001cd70(&l[2], "MSTMTR", nmat, &l[3]);
    }
}

// @src 1401be660  number of inter-material relations (after pruning the invalid ones)
void relation_count(DB_STEP_DATA* m, std::int32_t* nrel, std::int32_t*)
{
    std::int32_t c = 0, z = 0;
    phase_pair_list_prune(&m->phase_pairs);
    *nrel = phase_pair_list_active(&m->phase_pairs);
    if (m->u009 != 0) {
        c = 2;
        z = 0;
        diag_14001cd80(&c, "PHASTF", nrel, &z);
    }
}

// @src 1401be700  largest material-group count of the meshed objects
void material_group_count(DB_STEP_DATA* m, std::int32_t* ngrp, std::int32_t* err)
{
    GendbExt& x = gendb_ext();
    for (std::int32_t k = 1; k <= m->u1378; k = k + 1) {
        LLOBJ_DATA* o = x.fun_14017aaf0(&m->objects, k);
        if (o == nullptr || obj_kind(o) == 7 || obj_nodes(o) < 4 || obj_elems(o) < 1 ||
            field<void*>(o, 0x9f8) == nullptr) {
            continue;
        }
        bool single = true;
        std::int32_t matno = 0;
        object_group_list(o, &single, &matno);
        if (m->sim_ctrl.trans[2] != 0 && 0 < field<std::int32_t>(o, 0x3590) && single) {
            crt::fprintf(stdout, "ERROR  : Invalid Material Group Number %6d for\n", matno);
            crt::fprintf(stdout, " Object # %4d\n", obj_number(o));
            crt::fprintf(stdout, "         A phase cannot be specified as the material\n");
            crt::fprintf(stdout, "         for an object, it must be a mixture material.\n");
            std::int32_t c = 0x45;
            diag_14001cd90(&c, "OBJNA", &matno, &obj_number(o));
            *err = 1;
        }
        std::int32_t v = field<std::int32_t>(o, 0x3590);
        *ngrp = (v < *ngrp) ? *ngrp : v;
    }
    if (m->u009 != 0 && 0 < *ngrp) {
        std::int32_t c = 0x18, z = 0;
        diag_14001cd90(&c, "PHASTF", ngrp, &z);
    }
}

// @src 1401be960  element-name width: max(2, number of UENAME strings), then resize the array
void element_name_width(DB_STEP_DATA* m, std::int32_t* w, std::int32_t*)
{
    *w = (m->u33a8 < 3) ? 2 : m->u33a8;
    gendb_ext().fun_1401d2090(m, *w);
}

// @src 1401be9d0  node-name width: max(2, number of UNNAME strings), then resize the array
void node_name_width(DB_STEP_DATA* m, std::int32_t* w, std::int32_t*)
{
    *w = (m->u3398 < 3) ? 2 : m->u3398;
    gendb_ext().fun_1401d21c0(m, *w);
}

// @src 1401bea40  -> 1401ee650 on the simulation control
void sim_ctrl_function_counts(DB_STEP_DATA* m, std::int32_t* n, std::int32_t* len, std::int32_t*)
{
    sim_ctrl_counts(&m->sim_ctrl, n, len);
}

// @src 1401bea90  -> 14029b8e0 on the material list
void step_material_function_counts(DB_STEP_DATA* m, std::int32_t* n, std::int32_t* len,
                                   std::int32_t* err)
{
    material_list_function_counts(&m->materials, n, len, err);
}

// @src 1401beae0  -> 14033a370 on the phase-pair list
void phase_pair_function_counts(DB_STEP_DATA* m, std::int32_t* n, std::int32_t* len,
                                std::int32_t* err)
{
    phase_pair_list_function_counts(&m->phase_pairs, n, len, err);
}

// @src 1401beb90  -> 1401fa170 on the inter-object list (err is passed but not used)
void inter_object_function_counts(DB_STEP_DATA* m, std::int32_t* n, std::int32_t* len,
                                  std::int32_t* nwin, std::int32_t* lwin, std::int32_t*)
{
    inter_object_list_counts(&m->inter_obj, n, len, nwin, lwin);
}

} // namespace def::dbio::gendb
