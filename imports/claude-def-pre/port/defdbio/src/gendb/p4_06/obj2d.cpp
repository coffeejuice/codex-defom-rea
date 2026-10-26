// GENDB writer of the 2D object list (packet p4-06): FUN_14017cb60 and the 2D boundary-record
// writer FUN_140176830 with their helpers. Ported from the decompile, call arguments checked
// against the listing (out/p406/p406_lst.txt).
#include <defdbio/gendb/p4_06_obj2d.hpp>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

#include <defdbio/gendb/ext_p4_06.hpp>
#include <defmodel/layout/obj_geo2d.hpp>
#include <defmodel/llobj/cls_140182880.hpp>
#include <defmodel/members/ftn_geom.hpp>
#include <defmodel/statevar/geo2d_kernels.hpp>
#include <defrt/stdlib/legacy_printf.hpp>
#include <defrt/stdlib/refcount.hpp>

namespace def::dbio::gendb {

using model::DB_STEP_DATA;
using model::field;
using model::LLOBJ_DATA;

namespace {

// operator new of n ints as the binary sizes it (4 * n, -1 on overflow: MUL + CMOVO). The binary
// releases these buffers with the CRT free (140478b30 / 140479908) and its operator new is malloc
// based, so malloc / free are used here.
std::int32_t* new_ints(std::int32_t n)
{
    const std::uint64_t un = static_cast<std::uint64_t>(static_cast<std::int64_t>(n));
    const std::uint64_t bytes = un > (UINT64_MAX / 4) ? UINT64_MAX : un * 4;
    return static_cast<std::int32_t*>(std::malloc(static_cast<std::size_t>(bytes)));
}

// boundary b of an object (Cls_14017e740 array at obj+0xbe0, 0x30 bytes each)
inline std::byte* boundary(LLOBJ_DATA* obj, std::int32_t b)
{
    return reinterpret_cast<std::byte*>(obj) + 0xbe0 + static_cast<std::ptrdiff_t>(b) * 0x30;
}
inline std::int32_t* boundary_nodes(LLOBJ_DATA* obj, std::int32_t b) { return field<std::int32_t*>(boundary(obj, b), 8); }

} // namespace

// @src 14019a240  DB_WRITE_VAR +0x120
std::int32_t gendb_bnd_row_cursor(const DB_STEP_DATA* m) { return field<std::int32_t>(m, 0x120); }

// @src 14019a260  set DB_WRITE_VAR +0x120
void gendb_set_bnd_row_cursor(DB_STEP_DATA* m, const std::int32_t* v) { field<std::int32_t>(m, 0x120) = *v; }

// @src 140221390  binary exponentiation (std::_Pow_int)
double pow_int_impl(double x, std::int32_t n)
{
    std::uint32_t k = static_cast<std::uint32_t>(n);
    if (n < 0) {
        k = static_cast<std::uint32_t>(-n);
    }
    double r = 1.0; // DAT_140509c90
    double b = x;
    while (true) {
        if ((k & 1) != 0) {
            r = r * b;
        }
        k = k >> 1;
        if (k == 0) {
            break;
        }
        b = b * b;
    }
    return n < 0 ? 1.0 / r : r;
}

// @src 1402204a0  thunk of 140221390
double pow_int(double x, std::int32_t n) { return pow_int_impl(x, n); }

// @src 140167900
std::int32_t find_edge_element(LLOBJ_DATA* obj, const std::int32_t* n1, const std::int32_t* n2)
{
    const double* xy = ext::f1401808d0(reinterpret_cast<std::byte*>(obj) + 0x8a0);
    const std::int32_t* conn = ext::f140183cb0(reinterpret_cast<std::byte*>(obj) + 0x9e0);
    if (field<std::int32_t>(obj, 0x87c) == 0 || xy == nullptr || conn == nullptr) {
        return -1;
    }
    for (std::int32_t e = 0; e < field<std::int32_t>(obj, 0x884); ++e) {
        const std::int32_t* row = conn + e * field<std::int32_t>(obj, 0x894);
        for (std::int32_t k = 0; k < field<std::int32_t>(obj, 0x894); ++k) {
            if (row[k] == *n1 && row[(k + 1) % 4] == *n2) {
                return e + 1;
            }
        }
    }
    return 0;
}

// @src 1401671b0
std::int32_t find_principal_record(LLOBJ_DATA* obj, const double* llbnd, const std::int32_t* width)
{
    const std::int32_t first = field<std::int32_t>(obj, 0x3588);
    if (first == field<std::int32_t>(obj, 0x358c)) {
        return first;
    }
    std::int32_t found = 0;
    for (std::int32_t r = first; r <= field<std::int32_t>(obj, 0x358c); ++r) {
        if (llbnd[(r - 1) * *width + 1] == 1.0) { // DAT_140509c90; UCOMISD, NaN is not equal
            found = r;
            break;
        }
    }
    return found;
}

// @src 14017ed40
bool boundary_bounding_box(const void* bnd, double* min, double* max, const double* xy)
{
    bool first = true;
    const std::int32_t n = field<std::int32_t>(bnd, 0);
    if (n < 1) {
        return false;
    }
    const std::int32_t* nodes = field<std::int32_t*>(bnd, 8);
    for (std::int32_t i = 0; i < n - 1; ++i) {
        if (nodes[i] == nodes[i + 1]) {
            continue;
        }
        const double* p = xy + (nodes[i] * 2 - 2);
        if (first) {
            for (std::int32_t k = 0; k < 2; ++k) {
                min[k] = p[k];
                max[k] = p[k];
            }
            first = false;
        } else {
            for (std::int32_t k = 0; k < 2; ++k) {
                if (min[k] > p[k]) { // 14017eea0 COMISD min, p; JBE skips
                    min[k] = p[k];
                }
                if (p[k] > max[k]) { // 14017eedf COMISD p, max; JBE skips
                    max[k] = p[k];
                }
            }
        }
    }
    return true;
}

// @src 1401ff7f0
bool principal_curve(LLOBJ_DATA* obj, std::int32_t* index)
{
    double box[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0}; // [0..1] overall min, [2..3] curve min, [4..5] overall max
    double cmax[2] = {0.0, 0.0};                    // curve max
    double radius = 0.0;                            // local_38 (largest radius, unused)
    bool first = true;
    *index = 0;
    if (!ext::f140160cc0(obj)) {
        return false;
    }
    for (std::int32_t c = 0; c < ext::f140198310(obj); ++c) {
        auto* geo = static_cast<model::OBJ_GEO2D*>(ext::f1401982c0(obj, &c));
        if (!geo->bounding_box(box + 2, cmax, &radius)) {
            continue;
        }
        if (!first) {
            std::int32_t score = 0;
            for (std::int32_t k = 0; k < 2; ++k) {
                if (box[k + 2] <= box[k]) {
                    box[k] = box[k + 2];
                    score = score + static_cast<std::int32_t>(pow_int(10.0, k)) * 100; // DAT_14051bee8
                }
                if (box[k + 4] <= cmax[k]) {
                    box[k + 4] = cmax[k];
                    score = score + static_cast<std::int32_t>(pow_int(10.0, k));
                }
            }
            if (score == 0x457) {
                *index = c + 1;
            }
        } else {
            for (std::int32_t k = 0; k < 2; ++k) {
                box[k] = box[k + 2];
                box[k + 4] = cmax[k];
            }
            *index = c + 1;
            first = false;
        }
    }
    return true;
}

// @src 1401ffa90
bool principal_boundary(LLOBJ_DATA* obj, std::int32_t* index)
{
    double box[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    bool first = true;
    double cmax[2] = {0.0, 0.0};
    *index = 0;
    if (field<std::int32_t>(obj, 0x87c) == 0) {
        return false;
    }
    for (std::int32_t b = 0; b < field<std::int32_t>(obj, 0xbd8); ++b) {
        if (!boundary_bounding_box(boundary(obj, b), box + 2, cmax, field<double*>(obj, 0x8a8))) {
            continue;
        }
        if (!first) {
            std::int32_t score = 0;
            for (std::int32_t k = 0; k < 2; ++k) {
                if (box[k + 2] <= box[k]) { // 1401ffc34 COMISD box[k], box[k+2]; JC skips
                    box[k] = box[k + 2];
                    score = score + static_cast<std::int32_t>(pow_int(10.0, k)) * 100;
                }
                if (box[k + 4] <= cmax[k]) { // 1401ffc84 COMISD cmax[k], box[k+4]; JC skips
                    box[k + 4] = cmax[k];
                    score = score + static_cast<std::int32_t>(pow_int(10.0, k));
                }
            }
            if (score == 0x457) {
                *index = b + 1;
            }
        } else {
            for (std::int32_t k = 0; k < 2; ++k) {
                box[k] = box[k + 2];
                box[k + 4] = cmax[k];
            }
            *index = b + 1;
            first = false;
        }
    }
    return true;
}

// @src 140276d70
std::int32_t write_curve_rows(void* geo2d, double* out)
{
    auto* geo = static_cast<model::OBJ_GEO2D*>(geo2d);
    std::int32_t nent = 0;
    if (geo->curve_type == 2) {
        std::memcpy(out, geo->points, static_cast<std::size_t>(static_cast<std::int64_t>(geo->n_points * 6) << 3));
        nent = geo->n_points;
    } else {
        nent = 0;
        std::int32_t n = geo->n_points;
        if (geo->fun_1402765e0()) {
            n = geo->n_points;
        }
        model::fun_140345f5b(geo->points, &n);
        model::geo2d::points_to_entities(&n, geo->points, &nent, out);
    }
    return nent;
}

// @src 140176830
void write_boundary_record_2d(LLOBJ_DATA* obj, const std::int32_t* type, const std::int32_t* principal, double* rec,
                              std::int32_t* iibdy, std::int32_t* iibel, std::int32_t* iind, const std::int32_t* last,
                              double* llgry, std::int32_t* iifnc, double* llfnc, const std::int32_t* index)
{
    (void)iind;
    DB_STEP_DATA* m = field<DB_STEP_DATA*>(obj, 0x3a98);
    if (!rt::dimension_is_2(m)) {
        return;
    }
    std::int32_t b = *index - 1; // local_70 (also the curve index passed to 1401982c0)
    (void)reinterpret_cast<model::Cls_140182880*>(reinterpret_cast<std::byte*>(obj) + 0x9e0)->width(); // 140185530, unused
    const std::int32_t node0 = ext::f140199ce0(m);
    const std::int32_t elem0 = ext::f140199d20(m);
    rec[0] = static_cast<double>(*type);
    rec[1] = static_cast<double>(*principal);
    if (1 < *type) {
        const std::int32_t count = field<std::int32_t>(boundary(obj, b), 0);
        (void)field<std::int32_t>(boundary(obj, b), 0x10); // local_50, unused
        std::int32_t row = ext::f14019a0c0(m);
        std::int32_t erow = gendb_bnd_row_cursor(m);
        std::int32_t elem = 0;
        const std::int32_t n = field<std::int32_t>(boundary(obj, b), 0);
        rec[2] = static_cast<double>(count);
        rec[3] = static_cast<double>(row + 1);
        for (std::int32_t i = 0; i < n - 1; ++i) {
            row = row + 1;
            erow = erow + 1;
            iibdy[row - 1] = boundary_nodes(obj, b)[i] + node0;
            elem = find_edge_element(obj, boundary_nodes(obj, b) + i, boundary_nodes(obj, b) + (i + 1));
            if (elem < 1) {
                elem = 1;
            }
            iibel[row - 1] = elem + elem0;
        }
        row = row + 1;
        erow = erow + 1;
        iibdy[row - 1] = boundary_nodes(obj, b)[0] + node0;
        if (*last == 0) {
            erow = erow + 1;
        } else if (*last == -1) {
            row = erow;
        }
        ext::f14019a0e0(m, &row);
        gendb_set_bnd_row_cursor(m, &erow);
    }
    if (*type < 3) {
        std::int32_t c0 = 0;
        auto* geo = static_cast<model::OBJ_GEO2D*>(ext::f1401982c0(obj, &c0));
        std::int32_t grow = ext::f14019a100(m);
        rec[5] = static_cast<double>(grow * 6 + 1);
        const std::int32_t kind = geo->curve_type == 1 ? 1 : 2;
        rec[6] = static_cast<double>(kind);
        if (ext::f140198310(obj) == 1) {
            const std::int32_t nrow = write_curve_rows(geo, llgry + grow * 6);
            rec[4] = static_cast<double>(nrow);
            grow = grow + nrow;
        } else {
            geo = static_cast<model::OBJ_GEO2D*>(ext::f1401982c0(obj, &b));
            const std::int32_t nrow = write_curve_rows(geo, llgry + grow * 6);
            rec[4] = static_cast<double>(nrow);
            grow = grow + nrow;
        }
        ext::f14019a120(m, &grow);
    }
    std::int32_t cursor = ext::f14019a220(m);
    ext::f1401912a0(reinterpret_cast<std::byte*>(obj) + 0x2160, const_cast<std::int32_t*>(type), rec, iifnc, llfnc, m,
                    &cursor);
}

// @src 14017cb60
void write_objects_2d(model::Cls_14017a6c0* list, std::int32_t* mar, double* a, std::int32_t* mt, std::int32_t* at,
                      std::int32_t* iifnc, double* llfnc, double* llwin, double* llwrz)
{
    DB_STEP_DATA* m = list->owner;
    std::int32_t* irec = nullptr; // local_128
    double* lrec = nullptr;       // local_138
    LLOBJ_DATA* obj = nullptr;    // local_168
    const std::int32_t nobj = ext::f14019a280(list);
    std::int32_t* iiobj = mar + (mt[3] - 1);
    const std::int32_t llobj_start = at[5];
    double* llbnd = a + (at[6] - 1);
    std::int32_t* iibdy = mar + (mt[4] - 1);
    std::int32_t* iibel = mar + (mt[5] - 1);
    std::int32_t* iind = mar + (mt[6] - 1);
    double* llgry = a + (at[8] - 1);
    const std::int32_t w_iiobj = mt[0x124 / 4];
    const std::int32_t w_llobj = at[0x244 / 4];
    std::int32_t w_llbnd = at[0x248 / 4]; // local_120 (passed by address to 1401671b0 / 14017e190)
    const std::int32_t iind_start = mt[6];  // local_12c
    const std::int32_t w_llwin = at[0x254 / 4];

    std::int32_t max_no = ext::f14029c410(&m->materials); // local_178
    std::int32_t* slots = nullptr;                         // local_160
    if (max_no != 0) {
        slots = new_ints(max_no);
        ext::f14029ba10(&m->materials, slots, &max_no);
    }
    std::int32_t conn_pos = iind_start;  // local_140
    std::int32_t conn_prev = iind_start; // local_148
    char meshed = 0;                     // local_190
    std::int32_t dead_sum = 0;           // [rsp+0x70]: accumulated, never read
    for (std::int32_t i = 0; i < nobj; ++i) {
        obj = list->at(i + 1);
        if (field<std::int32_t>(obj, 0xc) == 7) {
            continue;
        }
        irec = iiobj + w_iiobj * i;
        lrec = a + (llobj_start - 1) + w_llobj * i;
        std::int32_t nbnd = 0; // local_11c
        meshed = 1;
        if (field<std::int32_t>(obj, 0xc) == 0 &&
            (ext::f1401f3d90(&m->sim_ctrl) || field<std::int32_t>(obj, 0x880) < 4 ||
             field<std::int32_t>(obj, 0x884) < 1)) {
            // rigid object written as geometry only: one LLBND record per curve
            std::int32_t type = 1;      // local_104[1]
            std::int32_t principal = 1; // local_104[0]
            std::int32_t last = 0;      // local_104[2]
            nbnd = ext::f140198310(obj);
            const std::int32_t ncurve = nbnd; // local_108
            if (nbnd == 1) {
                principal = 1;
                last = -1;
                std::int32_t index = 1;
                std::int32_t cur = ext::f14019a220(m);
                const std::int32_t off = cur * w_llbnd;
                cur = cur + 1;
                ext::f14019a2d0(m, &cur);
                write_boundary_record_2d(obj, &type, &principal, llbnd + off, iibdy, iibel, iind, &last, llgry, iifnc,
                                         llfnc, &index);
            } else {
                std::int32_t pc = 0; // local_f4[2]
                principal_curve(obj, &pc);
                for (std::int32_t c = 0; c < ncurve; ++c) {
                    principal = (c + 1 == pc) ? 1 : 0;
                    last = 0;
                    if (c + 1 == ncurve) {
                        last = -1;
                    }
                    std::int32_t index = c + 1;
                    std::int32_t cur = ext::f14019a220(m);
                    const std::int32_t off = cur * w_llbnd;
                    cur = cur + 1;
                    ext::f14019a2d0(m, &cur);
                    write_boundary_record_2d(obj, &type, &principal, llbnd + off, iibdy, iibel, iind, &last, llgry,
                                             iifnc, llfnc, &index);
                }
            }
            meshed = 0;
        }
        if (meshed != 0) {
            std::int32_t node_cnt = ext::f140199ce0(m); // local_a0
            std::int32_t elem_cnt = ext::f140199d20(m); // local_cc
            const std::int32_t per_elem =
                reinterpret_cast<model::Cls_140182880*>(reinterpret_cast<std::byte*>(obj) + 0x9e0)->width(); // local_ac
            conn_prev = conn_pos;
            conn_pos = conn_pos + field<std::int32_t>(obj, 0x884) * per_elem;
            nbnd = field<std::int32_t>(obj, 0xbd8);
            std::int32_t pb = 0; // local_a4
            principal_boundary(obj, &pb);
            std::int32_t type = 3; // local_8c
            if (field<std::int32_t>(obj, 0xc) == 0 && ext::f140203980(obj)) {
                type = 2;
            }
            std::int32_t principal = 0; // local_d0
            std::int32_t last = 0;      // local_80
            for (std::int32_t bi = 0; bi < nbnd; ++bi) {
                principal = (bi + 1 == pb) ? 1 : 0;
                last = 0;
                if (bi + 1 == nbnd) {
                    last = -1;
                }
                std::int32_t index = bi + 1;
                std::int32_t cur = ext::f14019a220(m);
                const std::int32_t off = cur * w_llbnd;
                cur = cur + 1;
                ext::f14019a2d0(m, &cur);
                write_boundary_record_2d(obj, &type, &principal, llbnd + off, iibdy, iibel, iind, &last, llgry, iifnc,
                                         llfnc, &index);
                ext::f140174b40(obj, mar, a, mt, at, "after WriteBdryInfoToDB");
            }
            ext::f14016f8e0(obj, mar, a, mt, at, &node_cnt);
            ext::f140174b40(obj, mar, a, mt, at, "after WriteNodeInfoToDB");
            ext::f140174cd0(obj, mar, mt, &conn_prev, &elem_cnt, &node_cnt);
            ext::f140174b40(obj, mar, a, mt, at, "after WriteElemIntegerInfoToDB");
            const std::int32_t nfaces = field<std::int32_t>(obj, 0x88c);   // local_98
            std::int32_t nedge = nfaces + -2 + nbnd * 2;                  // local_9c
            const std::int32_t cur = ext::f14019a220(m);                  // local_a8
            const std::int32_t off0 = (cur - nbnd) * w_llbnd;             // local_b4
            const std::int32_t first_face = static_cast<std::int32_t>(llbnd[off0 + 3]); // local_dc[2], CVTTSD2SI
            std::int32_t nscratch = ext::f140165230(obj);                 // local_b8
            std::int32_t* scratch = new_ints(nscratch);                   // local_88
            for (std::int32_t k = 0; k < nscratch; ++k) {
                scratch[k] = 0;
            }
            std::int32_t nscr_used = 0;                // local_c8[0]
            std::int32_t face_off = first_face - 1;    // local_30
            ext::f1401774f0(obj, mar, a, mt, at, &face_off, &nedge, scratch, &nscr_used);
            ext::f140174b40(obj, mar, a, mt, at, "after WriteEdgeFaceInfoToDB");
            ext::f140172c90(obj, mar, a, mt, at, &elem_cnt, &node_cnt);
            ext::f140174b40(obj, mar, a, mt, at, "after WriteElemInfoToDB");
            std::int32_t p90 = 0;           // local_90
            std::int32_t pdc1 = 0;          // local_dc[1]
            std::int32_t* extra = nullptr;  // local_c0 (passed by value, freed afterwards)
            std::int32_t p94 = 0;           // local_94
            ext::f1401707e0(obj, mar, mt, &node_cnt, scratch, extra, &nscratch, &pdc1, &p90, &p94);
            ext::f140174b40(obj, mar, a, mt, at, "after WriteNodeIntegerInfoToDB");
            std::int32_t face_off2 = first_face - 1; // local_2c
            ext::f140178460(obj, mar, mt, &node_cnt, &face_off2, &nedge, iifnc, llfnc, scratch, extra, &p90, &p94,
                            &nscr_used);
            ext::f140174b40(obj, mar, a, mt, at, "after WriteBdryCndFuncToDB");
            std::free(scratch); // 140478b30 (free)
            std::free(extra);
            dead_sum = dead_sum + nedge + 2;
        }
        const std::int32_t bnd_cursor = ext::f14019a220(m); // local_118
        const std::int32_t one = 1;                          // local_110
        ext::f14016cf10(obj, irec, lrec, (one - iind_start) + 1, bnd_cursor, nbnd, iifnc, llfnc, slots, &meshed);
        ext::f140174b40(obj, mar, a, mt, at, "after WriteGeneralInfoToDB");
        char flag_a = 1;      // local_10c (remesh windows; also WINTMP/WINATM/LOCTMP/LOCATM)
        char flag_win = 1;    // local_112
        char flag_locdef = 1; // local_114
        char flag_winprs = 1; // local_113
        if (field<std::int32_t>(obj, 0xc) == 0 && ext::f1401f3d90(&m->sim_ctrl)) {
            flag_a = 0;
            flag_win = 0;
        } else if (field<std::int32_t>(obj, 0xc) == 0 && meshed == 0) {
            flag_a = 0;
            flag_win = 0;
        } else {
            flag_a = 1;
            flag_win = 1;
        }
        if (ext::f1401f3c30(&m->sim_ctrl) == 0 || meshed == 0) {
            flag_locdef = 0;
        } else {
            flag_locdef = 1;
        }
        if (field<std::int32_t>(obj, 0xc) == 0) {
            flag_winprs = 0;
        } else if (meshed == 0) {
            flag_winprs = 0;
        } else {
            flag_winprs = 1;
        }
        std::int32_t* objid = &field<std::int32_t>(obj, 8);
        ext::f14018a760(reinterpret_cast<std::byte*>(obj) + 0x2d10, irec, lrec, iifnc, llfnc, llwin, w_llwin, llwrz,
                        &flag_a, m, objid);
        ext::f140196840(reinterpret_cast<std::byte*>(obj) + 0x2e68, irec, lrec, iifnc, llfnc, llwin, w_llwin, llwrz,
                        &flag_win, &flag_locdef, &flag_winprs, m, objid);
        ext::f140174b40(obj, mar, a, mt, at, "after BdryCnd.WriteToDB");
        ext::f14016d200(obj, irec, lrec, (one - iind_start) + 1, bnd_cursor, nbnd, iifnc, llfnc, slots, &meshed);
        ext::f140187a60(reinterpret_cast<std::byte*>(obj) + 0x1ea0, irec, lrec, iifnc, llfnc, m, objid);
    }
    (void)dead_sum;

    std::int32_t pdie = field<std::int32_t>(m, 0x8b8); // local_130 (S+0x168 PDIE)
    obj = ext::f14017aaf0(list, pdie);
    if (obj == nullptr) {
        if (ext::f1401f3c30(&m->sim_ctrl) != 0) {
            // str 140522858
            rt::crt::fprintf(stdout, "Invalid Pricipal Die Defined; PDIE = %d\n", pdie);
            std::int32_t code = 0xe; // local_5c
            std::int32_t zero = 0;   // local_60
            ext::f14001cd90(&code, nullptr /* DAT_140522884 */, &pdie, &zero);
        }
    } else {
        std::int32_t* iisim = mar + (mt[8] - 1); // local_68
        iisim[0x8c / 4] = field<std::int32_t>(obj, 0x3588);
        iisim[0x24 / 4] = iisim[0x8c / 4];
        const std::int32_t pr = find_principal_record(obj, llbnd, &w_llbnd);
        if (0 < pr) {
            iisim[0x8c / 4] = pr;
            iisim[0x24 / 4] = iisim[0x8c / 4];
        }
    }
    ext::f14017e190(list, a + (at[0x2c / 4] - 1), mar + (mt[9] - 1), llbnd, &w_llbnd);
    if (max_no != 0) {
        std::int32_t* elmat = mar + (mt[7] - 1); // local_50 (element material numbers)
        const std::int32_t nel = ext::f140199d20(m);
        for (std::int32_t e = 0; e < nel; ++e) {
            const std::int32_t no = elmat[e];
            if (0 < no) {
                elmat[e] = slots[no - 1];
            }
        }
    }
    if (slots != nullptr) {
        std::free(slots); // 140479908
    }
}

} // namespace def::dbio::gendb
