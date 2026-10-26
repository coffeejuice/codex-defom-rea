// State-variable writer FUN_1402a4a80 (_DB_STEP_DATA::KWD_writeObjStateVariables), its
// edge/face, custom-variable and phase-gather writers, and the registry write handlers that wrap
// it (packet kw-g2sv). Spec: docs/re/keywords/group2-statevar-writers.md, the keyword specs.
#include <defkey/kw_g2sv.hpp>

#include <cstdio>
#include <cstdlib>

#include <defkey/key_writer.hpp>
#include <defkey/kw_g2obj.hpp>
#include <defkey/table_io.hpp>
#include <defmodel/bc/boundary_topology.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/state_var_base.hpp>
#include <defmodel/statevar_kw/statevar_kw.hpp>
#include <defrt/stdlib/legacy_printf.hpp>

#if __has_include(<defkey/kw_g2geo.hpp>)
#include <defkey/kw_g2geo.hpp>
#define DEF_KW_G2GEO 1
#endif

#include "sv_internal.hpp"

namespace def::key::g2sv {

using namespace detail;
using def::model::LLOBJ_DATA;

namespace {

const char* kw(int id) { return def::key::keyword_label(id); } // 1402f7720

// @src 1402f6600  int value per boundary edge (2D) / face (3D)
int write_edge_face_int(std::FILE* f, const char* name, std::uint32_t no, LLOBJ_DATA* obj,
                        const std::int32_t* v, std::int32_t m, std::int32_t d)
{
    if (f == nullptr) return 2;
    if (obj == nullptr || v == nullptr) return 0;
    const std::int32_t ns = def::model::bc::edge_face_slot_count(*obj); // 140165170
    std::int32_t count = 0;
    bool uniform = true;
    if (1 < m) return 1;
    DB_STEP_DATA& om = *obj->owner;
    for (std::int32_t e = 0; e < ns; e = e + 1) {
        std::int32_t n1 = 0, n2 = 0;
        bool valid = !is_2d(om);
        if (!valid) {
            def::model::bc::edge_slot_nodes(*obj, e, &n1, &n2); // 140200e20
            valid = 0 < n1 && n1 <= nodes(*obj) && 0 < n2 && n2 <= nodes(*obj);
        }
        if (valid) {
            if (v[e] != *v) uniform = false;
            if (v[e] != d) count = count + 1;
        }
    }
    if (uniform) {
        def::rt::crt::fprintf(f, "%-6s %7i %7i %7i\n", name, no, 0, *v);
    } else {
        def::rt::crt::fprintf(f, "%-6s %7i %7i %7i\n", name, no, count, d);
        std::uint32_t s = 0;
        for (std::int32_t e = 0; e < ns; e = e + 1) {
            if (v[e] == d) continue;
            s = s + 1;
            if (!is_2d(om)) {
                std::int32_t a = 0, b = 0, c = 0, q = 0;
                def::model::bc::face_nodes(*obj, e, &a, &b, &c, &q); // 140201210
                const std::int32_t nn = nodes(*obj);
                if (0 < a && a <= nn && 0 < b && b <= nn && 0 < c && c <= nn && 0 < q && q <= nn)
                    def::rt::crt::fprintf(f, " %7i %7i %7i %7i %7i %7i\n", s, a, b, c, q, v[e]);
            } else {
                std::int32_t a = 0, b = 0;
                def::model::bc::edge_slot_nodes(*obj, e, &a, &b);
                if (0 < a && a <= nodes(*obj) && 0 < b && b <= nodes(*obj))
                    def::rt::crt::fprintf(f, " %7i %7i %7i %7i\n", s, a, b, v[e]);
            }
        }
    }
    return 0;
}

// @src 1402f6a10  double value per boundary edge (2D) / face (3D)
int write_edge_face_double(std::FILE* f, const char* name, std::uint32_t no, LLOBJ_DATA* obj,
                           const double* v, std::int32_t m, double d)
{
    if (f == nullptr) return 2;
    if (obj == nullptr || v == nullptr) return 0;
    const std::int32_t ns = def::model::bc::edge_face_slot_count(*obj);
    std::int32_t count = 0;
    bool uniform = true;
    if (1 < m) return 1;
    DB_STEP_DATA& om = *obj->owner;
    for (std::int32_t e = 0; e < ns; e = e + 1) {
        std::int32_t n1 = 0, n2 = 0;
        bool valid = !is_2d(om);
        if (!valid) {
            def::model::bc::edge_slot_nodes(*obj, e, &n1, &n2);
            valid = 0 < n1 && n1 <= nodes(*obj) && 0 < n2 && n2 <= nodes(*obj);
        }
        if (valid) {
            if (v[e] != *v) uniform = false;
            if (v[e] != d) count = count + 1;
        }
    }
    if (uniform) {
        def::rt::crt::fprintf(f, "%-6s %7i %7i %20.10E\n", name, no, 0, *v);
    } else {
        def::rt::crt::fprintf(f, "%-6s %7i %7i %20.10E\n", name, no, count, d);
        std::uint32_t s = 0;
        for (std::int32_t e = 0; e < ns; e = e + 1) {
            if (!(v[e] != d)) continue;
            s = s + 1;
            if (!is_2d(om)) {
                std::int32_t a = 0, b = 0, c = 0, q = 0;
                def::model::bc::face_nodes(*obj, e, &a, &b, &c, &q);
                const std::int32_t nn = nodes(*obj);
                if (0 < a && a <= nn && 0 < b && b <= nn && 0 < c && c <= nn && 0 < q && q <= nn)
                    def::rt::crt::fprintf(f, " %7i %7i %7i %7i %7i %20.10E\n", s, a, b, c, q, v[e]);
            } else {
                std::int32_t a = 0, b = 0;
                def::model::bc::edge_slot_nodes(*obj, e, &a, &b);
                if (0 < a && a <= nodes(*obj) && 0 < b && b <= nodes(*obj))
                    def::rt::crt::fprintf(f, " %7i %7i %7i %20.10E\n", s, a, b, v[e]);
            }
        }
    }
    return 0;
}

// @src 1402f6e40  one custom state variable (CUSVAR)
int write_custom_var(std::FILE* f, const char* name, std::uint32_t no, std::int32_t var_id,
                     LLOBJ_DATA& obj, double d)
{
    def::model::StateVarBase* var = def::model::svkw::custom_find(obj, var_id); // 140185e00
    if (var == nullptr) return 1;
    const std::int32_t type = var->u0c;            // 140300ee0
    const double* data = var->value_data();        // 140301050
    std::int32_t m = var->dim1(false);             // 140300f60
    std::int32_t nr = var->dim0(false);            // 140300f00
    if (type == 0) {
        def::rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i %20.10E %7i\n", name, no, var_id, 0, nr, d, m);
        for (std::int32_t i = 0; i < nr; i = i + 1) {
            def::rt::crt::fprintf(f, " %7i", i + 1);
            if (write_doubles(f, m, data + i * m, 5, false) != 0) break;
        }
    } else {
        bool uniform = true;
        std::int32_t count = 0;
        std::int32_t fmt = 0;
        double* defs = static_cast<double*>(std::calloc(static_cast<std::size_t>(m), 8));
        analyse_values(data, &nr, &m, &d, &fmt, &uniform, &count, defs); // 1402f5360
        def::rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i %20.10E %7i\n", name, no, var_id, type, count, d,
                              m);
        if (!uniform) {
            for (std::int32_t i = 0; i < nr; i = i + 1) {
                bool any = false;
                for (std::int32_t j = 0; j < m; j = j + 1) {
                    if (data[i * m + j] != d) {
                        any = true;
                        break;
                    }
                }
                if (any) {
                    def::rt::crt::fprintf(f, " %7i", i + 1);
                    if (write_doubles(f, m, data + i * m, 5, false) != 0) break;
                }
            }
        }
        if (defs != nullptr) std::free(defs);
    }
    return 0;
}

// @src 1402a3690  phase-column gather (VOLFC, VOLFS): global phase columns -> the object's own;
// `matrix` (the 8th argument: 0 from 1402a4a80, 1 from 1402a7b60) writes the gathered array with
// 1402f7490 instead of 1402f5c10
void write_phase_columns(DB_STEP_DATA& m, std::FILE* f, const char* name, LLOBJ_DATA& obj,
                         std::uint32_t no, const double* v, std::int32_t mg, bool matrix = false)
{
    std::int32_t* map = static_cast<std::int32_t*>(std::calloc(static_cast<std::size_t>(mfield(m, 0x1320)), 4));
    std::int32_t ml = 0; // uninitialised in the original when obj+0x87c == 0
    def::model::svkw::phase_column_map(m, obj, map, &ml); // 140398350
    double* buf = static_cast<double*>(std::calloc(static_cast<std::size_t>(elems(obj) * ml), 8));
    for (std::int32_t e = 0; e < elems(obj); e = e + 1) {
        for (std::int32_t c = 0; c < mg; c = c + 1) {
            if (-1 < map[c]) buf[e * ml + map[c]] = v[e * mg + c];
        }
    }
    if (!matrix) {
        write_node_elem_double(f, name, no, buf, elems(obj), ml, 0.0, 1, 0); // 1402f5c10
    } else {
        write_matrix(f, buf, ml, elems(obj)); // 1402f7490
    }
    if (map != nullptr) std::free(map);
    if (buf != nullptr) std::free(buf);
}

// RZ and ELMCON writers (kw-g2geo)
void write_rz(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj, std::uint32_t no)
{
#ifdef DEF_KW_G2GEO
    g2geo::write_nodes(m, f, obj, no); // 1402a2db0
#else
    (void)m, (void)f, (void)obj, (void)no;
#endif
}
void write_elmcon(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj, std::uint32_t no)
{
#ifdef DEF_KW_G2GEO
    g2geo::write_elements(m, f, obj, no); // 1402a30c0
#else
    (void)m, (void)f, (void)obj, (void)no;
#endif
}

// part of @src 1402a4a80: the common 1402f5c10 call
void wd(std::FILE* f, int id, std::uint32_t no, LLOBJ_DATA& o, std::size_t off, bool nodal,
        std::int32_t m, std::int32_t fmt, std::int32_t k = 0)
{
    write_node_elem_double(f, kw(id), no, darr(o, off), nodal ? nodes(o) : elems(o), m, 0.0, fmt, k);
}
void wd_named(std::FILE* f, const char* name, std::uint32_t no, LLOBJ_DATA& o, std::size_t off,
              bool nodal, std::int32_t m)
{
    write_node_elem_double(f, name, no, darr(o, off), nodal ? nodes(o) : elems(o), m, 0.0, 0, 0);
}
void wi(std::FILE* f, int id, std::uint32_t no, LLOBJ_DATA& o, std::size_t off, bool nodal,
        std::int32_t m, std::int32_t d)
{
    write_node_elem_int(f, kw(id), no, iarr(o, off), nodal ? nodes(o) : elems(o), m, d);
}

} // namespace

// @src 1402a4a80  _DB_STEP_DATA::KWD_writeObjStateVariables
int write_state_var(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj, int code, std::uint32_t no,
                    std::int32_t k)
{
    DB_STEP_DATA& om = *obj.get_owner(); // 140158f50: the object's own model
    const std::int32_t fmt23 = is_2d(m) ? 3 : 0;
    if (static_cast<std::int32_t>(no) < 1) no = static_cast<std::uint32_t>(obj_no(obj));
    switch (code) {
    case 0: write_rz(m, f, obj, no); break;
    case 1: wd(f, 0xa0, no, obj, 0x8b0, true, ncomp(m, 1), 0); break;
    case 2: wd(f, 0xa5, no, obj, 0x8b8, true, ncomp(m, 2), 0); break;
    case 3: wd(f, 0xa6, no, obj, 0x8c0, true, ncomp(m, 3), 0); break;
    case 4: wd_named(f, "*FRZB", no, obj, 0x8c8, true, ncomp(m, 4)); break;
    case 5: wd(f, 0xa7, no, obj, 0x8d0, true, ncomp(m, 5), 0); break;
    case 6: wd_named(f, "*PRZB", no, obj, 0x8d8, true, ncomp(m, 6)); break;
    case 8:
        if (!is_2d(m)) {
            wd(f, 0xaa, no, obj, 0x8e0, true, ncomp(m, 8), 0);
        } else {
            // 2D: (T - DTMP, T) per node
            const std::int32_t n = nodes(obj);
            double* buf = static_cast<double*>(std::calloc(static_cast<std::size_t>(n << 1), 8));
            const double* t = darr(obj, 0x8e0);
            const double* dt = darr(obj, 0x900);
            for (std::int32_t i = 0; i < n; i = i + 1) {
                buf[i * 2 + 1] = t[i];
                buf[i * 2] = t[i] - dt[i];
            }
            write_node_elem_double(f, kw(0xaa), no, buf, n, 2, 0.0, 3, 0);
            if (buf != nullptr) std::free(buf);
        }
        break;
    case 9: wd(f, 0xab, no, obj, 0x8e8, true, ncomp(m, 9), 0); break;
    case 10: wd_named(f, "*HITN", no, obj, 0x8f0, true, ncomp(m, 10)); break;
    case 0xb: wd(f, 0xac, no, obj, 0x8f8, true, ncomp(m, 0xb), 0); break;
    case 0xc: wd_named(f, "*DTMP", no, obj, 0x900, true, ncomp(m, 0xc)); break;
    case 0xd: wd(f, 0xd6, no, obj, 0x908, true, ncomp(m, 0xd), 0); break;
    case 0xe: wd(f, 0xdd, no, obj, 0x910, true, ncomp(m, 0xe), 0); break;
    case 0xf: wd(f, 0xd5, no, obj, 0x918, true, ncomp(m, 0xf), 0); break;
    case 0x10: wd(f, 0xde, no, obj, 0x920, true, ncomp(m, 0x10), 0); break;
    case 0x11: wd(f, 0xc4, no, obj, 0x968, true, ncomp(m, 0x11), 3); break;
    case 0x12: wi(f, 0xa2, no, obj, 0x990, true, ncomp(m, 0x12), 0); break;
    case 0x13: wi(f, 0xa3, no, obj, 0x998, true, ncomp(m, 0x13), 0); break;
    case 0x14: wi(f, 0xa8, no, obj, 0x9a0, true, ncomp(m, 0x14), 0); break;
    case 0x15: wi(f, 0xa9, no, obj, 0x9a8, true, ncomp(m, 0x15), 0); break;
    case 0x16: wi(f, 0xdb, no, obj, 0x9b0, true, ncomp(m, 0x16), 0); break;
    case 0x17: wi(f, 0xdc, no, obj, 0x9b8, true, ncomp(m, 0x17), 0); break;
    case 0x18: wd(f, 0xff, no, obj, 0x928, true, ncomp(m, 0x18), 0); break;
    case 0x19:
        if (is_3d(m)) wd(f, 0x107, no, obj, 0x930, true, ncomp(m, 0x19), 0);
        break;
    case 0x1d: {
        bool skip = false;
        if (def::model::svkw::grain_model(m, &skip) == 2) // 1401d7160
            wd(f, 0xd1, no, obj, 0x950, true, ncomp(m, 0x1d), 4, 2);
        break;
    }
    case 0x22: wd(f, 0xdf, no, obj, 0x958, true, ncomp(m, 0x22), 4, 0); break;
    case 0x23: wd(f, 0xe1, no, obj, 0x960, true, ncomp(m, 0x23), 4, 0); break;
    case 0x25: write_elmcon(m, f, obj, no); break;
    case 0x26: { // STRESS
        const double* s = darr(obj, 0xa00);
        if (mfield(m, 0x890) == 3) {
            if (!is_2d(m)) {
                if (at<std::int32_t>(obj, 0x890) == 4) {
                    def::rt::crt::fprintf(f, "%-6s %7i %7i %20.10E %7i %20.10E %7i\n", kw(0xb3), no,
                                          elems(obj), 0.0, nodes(obj), 0.0, 8);
                    for (std::int32_t e = 0; e < elems(obj); e = e + 1) {
                        def::rt::crt::fprintf(f, " %7i\n", e + 1);
                        for (std::int32_t p = 0; p < 8; p = p + 1) {
                            const double* q = s + (e * 0x30 + p * 6);
                            def::rt::crt::fprintf(f, " %20.10E %20.10E %20.10E %20.10E %20.10E %20.10E\n",
                                                  q[0], q[1], q[2], q[3], q[4], q[5]);
                        }
                    }
                } else if (at<std::int32_t>(obj, 0x890) == 2) {
                    def::rt::crt::fprintf(f, "%-6s %7i %7i %20.10E %7i %20.10E %7i\n", kw(0xb3), no,
                                          elems(obj), 0.0, nodes(obj), 0.0, 1);
                    for (std::int32_t e = 0; e < elems(obj); e = e + 1) {
                        def::rt::crt::fprintf(f, " %7i\n", e + 1);
                        const double* q = s + e * 6;
                        def::rt::crt::fprintf(f, " %20.10E %20.10E %20.10E %20.10E %20.10E %20.10E\n",
                                              q[0], q[1], q[2], q[3], q[4], q[5]);
                    }
                }
            } else if (mfield(m, 0x850) == 3) {
                def::rt::crt::fprintf(f, "%-6s %7i %7i %20.10E %7i %7i %20.10E %7i\n", kw(0xb3), no,
                                      elems(obj), 0.0, ncomp(m, 0x26), nodes(obj), 0.0, 4);
                for (std::int32_t e = 0; e < elems(obj); e = e + 1) {
                    def::rt::crt::fprintf(f, " %7i\n", e + 1);
                    for (std::int32_t p = 0; p < 4; p = p + 1) {
                        const double* q = s + (e * 0x18 + p * 6);
                        def::rt::crt::fprintf(f, " %20.10E %20.10E %20.10E %20.10E %20.10E %20.10E\n",
                                              q[0], q[1], q[2], q[3], q[4], q[5]);
                    }
                }
            } else {
                def::rt::crt::fprintf(f, "%-6s %7i %7i %20.10E %7i %7i %20.10E %7i\n", kw(0xb3), no,
                                      elems(obj), 0.0, ncomp(m, 0x26), nodes(obj), 0.0, 4);
                for (std::int32_t e = 0; e < elems(obj); e = e + 1) {
                    def::rt::crt::fprintf(f, " %7i\n", e + 1);
                    for (std::int32_t p = 0; p < 4; p = p + 1) {
                        const double* q = s + (e * 0x10 + p * 4);
                        def::rt::crt::fprintf(f, " %20.10E %20.10E %20.10E %20.10E\n", q[0], q[1], q[2],
                                              q[3]);
                    }
                }
            }
        } else if (ncomp(om, 0x1a) == 0 || darr(obj, 0x948) == nullptr) {
            write_node_elem_double(f, kw(0xb3), no, s, elems(obj), ncomp(m, 0x26), 0.0, fmt23, 0);
        } else {
            write_node_elem_double2(f, kw(0xb3), no, s, elems(obj), ncomp(m, 0x26), 0.0, darr(obj, 0x948),
                                    nodes(obj), ncomp(m, 0x1a), 0.0, fmt23);
        }
        break;
    }
    case 0x27: wd_named(f, "*EPS", no, obj, 0xa08, false, ncomp(m, 0x27)); break; // str 14053bffc
    case 0x28: wd(f, 0xb0, no, obj, 0xa10, false, ncomp(m, 0x28), 1); break;
    case 0x29: wd_named(f, "*EFSTS", no, obj, 0xa18, false, ncomp(m, 0x29)); break;
    case 0x2a: wd_named(f, "*EFEPS", no, obj, 0xa20, false, ncomp(m, 0x2a)); break;
    case 0x2b: { // STRAIN
        const double* s = darr(obj, 0xa28);
        if (mfield(m, 0x88c) == 3) {
            if (!is_2d(m)) {
                if (at<std::int32_t>(obj, 0x890) == 4) {
                    def::rt::crt::fprintf(f, "%-6s %7i %7i %20.10E %7i %20.10E %7i\n", kw(0xaf), no,
                                          elems(obj), 0.0, nodes(obj), 0.0, 8);
                    for (std::int32_t e = 0; e < elems(obj); e = e + 1) {
                        def::rt::crt::fprintf(f, " %7i\n", e + 1);
                        for (std::int32_t p = 0; p < 8; p = p + 1)
                            def::rt::crt::fprintf(f, " %20.10E\n", s[p + e * 8]);
                    }
                } else if (at<std::int32_t>(obj, 0x890) == 2) {
                    def::rt::crt::fprintf(f, "%-6s %7i %7i %20.10E %7i %20.10E %7i\n", kw(0xaf), no,
                                          elems(obj), 0.0, nodes(obj), 0.0, 1);
                    for (std::int32_t e = 0; e < elems(obj); e = e + 1) {
                        def::rt::crt::fprintf(f, " %7i\n", e + 1);
                        def::rt::crt::fprintf(f, " %20.10E\n", s[e]);
                    }
                }
            } else {
                def::rt::crt::fprintf(f, "%-6s %7i %7i %20.10E %7i %20.10E %7i\n", kw(0xaf), no, elems(obj),
                                      0.0, nodes(obj), 0.0, 4);
                for (std::int32_t e = 0; e < elems(obj); e = e + 1) {
                    def::rt::crt::fprintf(f, " %7i\n", e + 1);
                    for (std::int32_t p = 0; p < 4; p = p + 1)
                        def::rt::crt::fprintf(f, " %20.10E\n", s[p + e * 4]);
                }
            }
        } else if (ncomp(om, 0x1b) == 0 || darr(obj, 0x938) == nullptr) {
            write_node_elem_double(f, kw(0xaf), no, s, elems(obj), ncomp(m, 0x2b), 0.0, 0, 0);
        } else {
            write_node_elem_double2(f, kw(0xaf), no, s, elems(obj), ncomp(m, 0x2b), 0.0, darr(obj, 0x938),
                                    nodes(obj), ncomp(m, 0x1b), 0.0, 0);
        }
        break;
    }
    case 0x2c: wd(f, 0xb1, no, obj, 0xa30, false, ncomp(m, 0x2c), 0); break;
    case 0x2d: { // DAMAGE
        const double* s = darr(obj, 0xa38);
        if (mfield(m, 0x888) == 3) {
            std::int32_t ip = 1;
            if (!is_2d(m)) {
                if (at<std::int32_t>(obj, 0x890) == 4) ip = 8;
                if (at<std::int32_t>(obj, 0x890) == 2) ip = 1;
                write_node_elem_ip(f, kw(0xb2), no, s, elems(obj), ncomp(m, 0x2d), 0.0, ip, 5);
            } else {
                write_node_elem_ip(f, kw(0xb2), no, s, elems(obj), ncomp(m, 0x2d), 0.0, 4, 5);
            }
        } else if (ncomp(om, 0x1c) == 0 || darr(obj, 0x940) == nullptr) {
            if (ncomp(m, 0x2d) == 1)
                write_node_elem_double(f, kw(0xb2), no, s, elems(obj), ncomp(m, 0x2d), 0.0, 0, 0);
            else
                write_node_elem_double(f, kw(0xb2), no, s, elems(obj), ncomp(m, 0x2d), 0.0, 5, 0);
        } else {
            // 1402a5c40 / 1402a5cf0: both branches of the `model+0x1668 == 1` test make this call
            write_node_elem_double2(f, kw(0xb2), no, s, elems(obj), ncomp(m, 0x2d), 0.0, darr(obj, 0x940),
                                    nodes(obj), ncomp(m, 0x1c), 0.0, 5);
        }
        break;
    }
    case 0x2e: wd(f, 0xfe, no, obj, 0xa40, false, ncomp(m, 0x2e), 2); break;
    case 0x30: wd_named(f, "*HTGN", no, obj, 0xa50, false, ncomp(m, 0x30)); break;
    case 0x31: write_phase_columns(m, f, kw(0xcd), obj, no, darr(obj, 0xa58), ncomp(m, 0x31)); break;
    case 0x32: write_phase_columns(m, f, kw(0xd8), obj, no, darr(obj, 0xa60), ncomp(m, 0x32)); break;
    case 0x33: wd(f, 0xce, no, obj, 0xa68, false, ncomp(m, 0x33), 1); break;
    case 0x34: wd(f, 0xcf, no, obj, 0xa70, false, ncomp(m, 0x34), 1); break;
    case 0x35: wd(f, 0xd0, no, obj, 0xa78, false, ncomp(m, 0x35), 1); break;
    case 0x36:
        if (ncomp(m, 0x36) != 0 && darr(obj, 0xa80) != nullptr) wd(f, 0xd2, no, obj, 0xa80, false, ncomp(m, 0x36), 4, 0);
        break;
    case 0x37:
        if (ncomp(m, 0x37) != 0 && darr(obj, 0xa88) != nullptr) wd(f, 0xd3, no, obj, 0xa88, false, ncomp(m, 0x37), 4, 0);
        break;
    case 0x38: wd(f, 0xd9, no, obj, 0xa90, false, ncomp(m, 0x38), 0); break;
    case 0x39: wd(f, 0xe9, no, obj, 0xa98, false, ncomp(m, 0x39), 0); break;
    case 0x3a: wd(f, 0xed, no, obj, 0xaa0, false, ncomp(m, 0x3a), 0); break;
    case 0x3b: wd(f, 0xee, no, obj, 0xaa8, false, ncomp(m, 0x3b), 0); break;
    case 0x3c: wd(f, 0xc5, no, obj, 0xac0, false, ncomp(m, 0x3c), 3); break;
    case 0x3d: wi(f, 0xae, no, obj, 0x9f8, false, ncomp(m, 0x3d), 1); break;
    case 0x3e: wd(f, 0xef, no, obj, 0xab0, false, ncomp(m, 0x3e), 0); break;
    case 0x3f:
        if (ncomp(m, 0x3f) != 0 && darr(obj, 0xab8) != nullptr) wd(f, 0xe0, no, obj, 0xab8, false, ncomp(m, 0x3f), 4, 0);
        break;
    case 0x41: write_edge_face_int(f, kw(0xf1), no, &obj, iarr(obj, 0xae8), ncomp(m, 0x41), 0); break;
    case 0x42: write_edge_face_int(f, kw(0xf4), no, &obj, iarr(obj, 0xaf0), ncomp(m, 0x42), 0); break;
    case 0x43: write_edge_face_double(f, kw(0xf7), no, &obj, darr(obj, 0xaf8), ncomp(m, 0x43), 0.0); break;
    case 0x44: write_edge_face_int(f, kw(0xf2), no, &obj, iarr(obj, 0xb00), ncomp(m, 0x44), 0); break;
    case 0x45: write_edge_face_int(f, kw(0xf5), no, &obj, iarr(obj, 0xb08), ncomp(m, 0x45), 0); break;
    case 0x46: write_edge_face_double(f, kw(0xf8), no, &obj, darr(obj, 0xb10), ncomp(m, 0x46), 0.0); break;
    case 0x47: write_edge_face_int(f, kw(0xf3), no, &obj, iarr(obj, 0xb18), ncomp(m, 0x47), 0); break;
    case 0x48: write_edge_face_int(f, kw(0xf6), no, &obj, iarr(obj, 0xb20), ncomp(m, 0x48), 0); break;
    case 0x49: write_edge_face_double(f, kw(0xf9), no, &obj, darr(obj, 0xb28), ncomp(m, 0x49), 0.0); break;
    case 0x4a: write_edge_face_int(f, kw(0xfa), no, &obj, iarr(obj, 0xb30), ncomp(m, 0x4a), 0); break;
    case 0x4b: write_edge_face_int(f, kw(0xfb), no, &obj, iarr(obj, 0xb38), ncomp(m, 0x4b), 0); break;
    case 0x4c: write_edge_face_double(f, kw(0xfc), no, &obj, darr(obj, 0xb40), ncomp(m, 0x4c), 0.0); break;
    case 0x4e: write_custom_var(f, kw(0xc6), no, k, obj, 0.0); break;
    default:
        def::rt::crt::printf("ERROR: _DB_STEP_DATA::KWD_writeObjStateVariables doesn't handle eObjVarTyp %d", code);
        return 3;
    }
    return 0;
}

// ---- registry write handlers (FILE*, model, {objNo}) -------------------------------------------

namespace {

// part of every handler: the object, its printed number (obj+0x3f24 when > 0) and the call
int handler(std::FILE* f, DB_STEP_DATA& m, const KeyRef& ref, int code)
{
    std::int32_t no = ref[0]; // 140159a70(v, 0)
    LLOBJ_DATA* o = def::key::g2obj::find_object(m, no); // 14017aaf0
    if (0 < out_no(*o)) no = out_no(*o);
    write_state_var(m, f, *o, code, static_cast<std::uint32_t>(no), 0);
    return 0;
}

struct HandlerRow {
    int id;
    int code;
};

// @src 1402b4280  NDTMP (8)
// @src 1402b4340  STRAIN (0x2b)
// @src 1402b4d40  NWEAR (0x18)
// @src 1402b4e00  STRESS (0x26)
// @src 1402b4ec0  DAMAGE (0x2d)
// @src 1402b4f80  DRZ (1)
// @src 1402b5040  URZ (2)
// @src 1402b5100  FRZ (3)
// @src 1402b51c0  PRZ (5)
// @src 1402b5280  NDHEAT (9)
// @src 1402b5340  NDFLUX (0xb)
// @src 1402b5400  TIMERC (0x22)
// @src 1402b54c0  NRECVL (0x23)
// @src 1402b5580  USRNOD (0x11)
// @src 1402b5640  STNCMP (0x28)
// @src 1402b5700  DENSTY (0x2c)
// @src 1402b57c0  YLDS (0x39)
// @src 1402b5880  USRELM (0x3c)
// @src 1402b5940  VOLFC (0x31)
// @src 1402b5a00  VOLFS (0x32)
// @src 1402b5ac0  DVOLF (0x33)
// @src 1402b5b80  GRAIN (0x35)
// @src 1402b5c40  GRAIN2 (0x1d)
// @src 1402b5d00  SIZESH (0x36)
// @src 1402b5dc0  TXTODF (0x37)
// @src 1402b5e80  ERECVL (0x3f)
// @src 1402b5f40  TICF (0x34)
// @src 1402b6000  DATOM (0xd)
// @src 1402b60c0  HDNOBJ (0x38)
// @src 1402b6180  BCCCRB (0x16)
// @src 1402b6240  CRBFLX (0xe)
// @src 1402b6300  VOTAGE (0xf)
// @src 1402b63c0  BCCRHT (0x17)
// @src 1402b6480  RHTFLX (0x10)
// @src 1402b6540  ZEFI (0x3a)
// @src 1402b6600  ZMFI (0x3b)
// @src 1402b66c0  CURRNT (0x3e)
// @src 1402b6780  MATAXI (0x2e)
constexpr HandlerRow kHandlers[] = {
    {170, 0x08}, {175, 0x2b}, {255, 0x18}, {179, 0x26}, {178, 0x2d},
    {171, 0x09}, {172, 0x0b}, {223, 0x22}, {225, 0x23}, {196, 0x11},
    {176, 0x28}, {177, 0x2c}, {233, 0x39}, {197, 0x3c}, {205, 0x31}, {216, 0x32}, {206, 0x33},
    {208, 0x35}, {209, 0x1d}, {210, 0x36}, {211, 0x37}, {224, 0x3f}, {207, 0x34}, {214, 0x0d},
    {217, 0x38}, {219, 0x16}, {221, 0x0e}, {213, 0x0f}, {220, 0x17}, {222, 0x10}, {237, 0x3a},
    {238, 0x3b}, {239, 0x3e}, {254, 0x2e},
};

constexpr int kHDNEST = 218;

// @src 1402b4c40  HDNEST: one line from the property sub-structure
int write_hdnest(std::FILE* f, DB_STEP_DATA& m, const KeyRef& ref)
{
    std::int32_t no = ref[0];
    LLOBJ_DATA* o = def::key::g2obj::find_object(m, no);
    if (0 < out_no(*o)) no = out_no(*o);
    def::rt::crt::fprintf(f, "%-6s %7i %7i %7i %20.10E %20.10E\n", kw(kHDNEST), no, at<std::int32_t>(*o, 0x1f48),
                          at<std::int32_t>(*o, 0x1f44), at<double>(*o, 0x1f50), at<double>(*o, 0x1f58));
    return 0;
}

} // namespace

bool handles_write(int id)
{
    if (id == kHDNEST) return true;
    for (const HandlerRow& h : kHandlers)
        if (h.id == id) return true;
    return false;
}

int write_keyword(int id, std::FILE* f, DB_STEP_DATA& m, const KeyRef& ref)
{
    if (id == kHDNEST) return write_hdnest(f, m, ref);
    for (const HandlerRow& h : kHandlers)
        if (h.id == id) return handler(f, m, ref, h.code);
    return 0;
}

// @src 1402a7b60  VOLFC (0x31) / VOLFS (0x32) as a bare matrix (1402a3690 with matrix = 1);
// `no` < 1 takes the object number. Called by the STEP.DAT writer 1402ac8a0 (defcmd/step_dat.hpp).
int write_phase_matrix(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj, int id, std::int32_t no)
{
    if (no < 1) no = obj_no(obj);
    if (id == 0x31) {
        write_phase_columns(m, f, kw(0xcd), obj, static_cast<std::uint32_t>(no), darr(obj, 0xa58),
                            ncomp(m, 0x31), true);
    } else {
        if (id != 0x32) return 3;
        write_phase_columns(m, f, kw(0xd8), obj, static_cast<std::uint32_t>(no), darr(obj, 0xa60),
                            ncomp(m, 0x32), true);
    }
    return 0;
}

} // namespace def::key::g2sv
