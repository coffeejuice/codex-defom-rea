// Group 2 parser cases of the state-variable keywords (part of FUN_14038b8a0) and their special
// readers (packet kw-g2sv). Specs: docs/re/keywords/<KW>.md, group2-helpers.md,
// group2-statevar-writers.md "Readers that pair with these writers".
#include <defkey/kw_g2sv.hpp>

#include <cstdlib>
#include <cstring>

#include <defkey/crt_scan.hpp>
#include <defkey/line_io.hpp>
#include <defkey/node_elem_var.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/statevar_kw/statevar_kw.hpp>
#include <defmodel/step/name_arrays.hpp>
#include <defrt/stdlib/legacy_printf.hpp>

#include "sv_internal.hpp"

namespace def::key::g2sv {

using namespace detail;
namespace svkw = def::model::svkw;

namespace {

void* set8a0(LLOBJ_DATA& o) { return o.s8a0; }
void* set9e0(LLOBJ_DATA& o) { return o.s9e0; }

// @src 140417280  readNodeElemVarDouble2: "count1 def1 count2 def2" (mode 0) or
// "count1 def1 - count2 def2" (mode 3); defaults fill both arrays, then the records
int read_node_elem_double2(std::FILE* f, double* v1, std::int32_t n1, std::int32_t m1, double* v2,
                           std::int32_t n2, std::int32_t m2, std::int32_t mode, const char* line)
{
    char buf[0x400];
    std::int32_t conv = 0, ign = 0, count1 = 0, count2 = 0;
    double def1 = 0.0, def2 = 0.0;
    int st = 0;
    if (line == nullptr) {
        if (std::fgets(buf, 0x400, f) == nullptr) return 1;
    } else {
        std::strncpy(buf, line, 0x400);
    }
    if (mode == 0) {
        conv = crt::sscanf(buf, "%i %Flf %i %Flf", &count1, &def1, &count2, &def2);
    } else if (mode == 3) {
        conv = crt::sscanf(buf, "%i %Flf %i %i %Flf", &count1, &def1, &ign, &count2, &def2);
    } else {
        def::rt::crt::fprintf(stdout, "KwdReadUtl::readNodeElemVarDouble2() - not supported format.\n");
    }
    const bool has2 = v2 != nullptr && m2 != 0;
    if (conv < 1) return 1;
    for (std::int32_t i = 0; i < n1; i = i + 1)
        for (std::int32_t j = 0; j < m1; j = j + 1) v1[i * m1 + j] = def1;
    if (has2) {
        for (std::int32_t i = 0; i < n2; i = i + 1)
            for (std::int32_t j = 0; j < m2; j = j + 1) v2[i * m2 + j] = def2;
    }
    if (count1 != 0) st = read_value_records(f, v1, count1, n1, m1); // 1404199a0
    if (!has2) {
        if (count2 != 0) {
            double* scratch = static_cast<double*>(std::malloc(static_cast<std::size_t>(n2 * m2) * 8));
            st = read_value_records(f, scratch, count2, n2, m2);
            std::free(scratch);
        }
    } else if (count2 != 0) {
        st = read_value_records(f, v2, count2, n2, m2);
    }
    return st;
}

// @src 140418910  readNodeElemVarDouble4 (DAMAGE, element array plus nodal companion)
int read_damage4(std::FILE* f, LLOBJ_DATA& obj, std::int32_t code, std::int32_t n1, std::int32_t* m1,
                 std::int32_t ncode2, std::int32_t n2, std::int32_t* m2, std::int32_t mode, const char* line)
{
    char buf[0x400];
    std::int32_t conv = 0, k = 0, count1 = 0, count2 = 0;
    double def1 = 0.0, def2 = 0.0;
    int st = 0;
    if (line == nullptr) {
        if (std::fgets(buf, 0x400, f) == nullptr) return 1;
    } else {
        std::strncpy(buf, line, 0x400);
    }
    if (mode == 0)
        conv = crt::sscanf(buf, "%i %Flf %i %Flf", &count1, &def1, &count2, &def2);
    else if (mode == 5)
        conv = crt::sscanf(buf, "%i %Flf %i %Flf %i", &count1, &def1, &count2, &def2, &k);
    else
        def::rt::crt::fprintf(stdout, "KwdReadUtl::readNodeElemVarDouble4() - not supported format.\n");
    if (conv < 1) return 1;
    if (0 < k) *m1 = k;
    if (code != 0x2d) return 1;
    svkw::set9e0_resize(set9e0(obj), 0x2d, *m1); // 140183840
    double* v1 = svkw::damage_values(set9e0(obj)); // 1401840d0
    const bool has2 = *m2 != 0;
    double* v2 = nullptr;
    if (has2) {
        if (0 < k) *m2 = k;
        if (ncode2 != 0x1c) return 1;
        svkw::set8a0_resize(set8a0(obj), 0x1c, *m2); // 140180310
        v2 = svkw::damage_node_values(set8a0(obj));  // 1401812a0
    }
    for (std::int32_t i = 0; i < n1; i = i + 1)
        for (std::int32_t j = 0; j < *m1; j = j + 1) v1[i * *m1 + j] = def1;
    if (has2) {
        for (std::int32_t i = 0; i < n2; i = i + 1)
            for (std::int32_t j = 0; j < *m2; j = j + 1) v2[i * *m2 + j] = def2;
    }
    if (count1 != 0) st = read_value_records_k(f, v1, count1, n1, *m1, k); // 140419b90
    if (!has2) {
        if (count2 != 0) {
            // 14041984c: a buffer is allocated but the records go to the (null) nodal array with
            // m = 0, so only indices (k == 0) or k values per record are consumed
            void* scratch = std::malloc(static_cast<std::size_t>(n2 * *m2) * 8);
            st = read_value_records_k(f, v2, count2, n2, *m2, k);
            std::free(scratch);
        }
    } else if (count2 != 0) {
        st = read_value_records_k(f, v2, count2, n2, *m2, k);
    }
    return st;
}

// @src 140418e40  DAMAGE integration-point layout
int read_damage_ip(std::FILE* f, LLOBJ_DATA& obj, std::int32_t code, std::int32_t n, std::int32_t* m,
                   const char* line)
{
    char buf[0x400];
    std::int32_t count = 0, i3 = 0, mm = 0, ip = 1, idx = 0;
    double def = 0.0, d4 = 0.0;
    if (line == nullptr) {
        if (std::fgets(buf, 0x400, f) == nullptr) return 1;
    } else {
        std::strncpy(buf, line, 0x400);
    }
    const int conv = crt::sscanf(buf, "%i %Flf %i %Flf %i %i", &count, &def, &i3, &d4, &mm, &ip);
    if (conv < 1) return 1;
    if (ip < 1) ip = 1;
    if (0 < mm) *m = mm;
    if (code != 0x2d) return 1;
    svkw::set9e0_resize(set9e0(obj), 0x2d, *m);
    double* v = svkw::damage_values(set9e0(obj));
    for (std::int32_t e = 0; e < n; e = e + 1)
        for (std::int32_t p = 0; p < ip; p = p + 1)
            for (std::int32_t j = 0; j < *m; j = j + 1) v[*m * p + j + *m * ip * e] = def;
    if (count != 0) {
        double* tmp = static_cast<double*>(std::calloc(static_cast<std::size_t>(*m), 8));
        for (std::int32_t r = 0; r < count; r = r + 1) {
            if (crt::fscanf(f, "%i", &idx) == -1) return 1;
            if (idx <= n && 0 < idx) {
                for (std::int32_t p = 0; p < ip; p = p + 1) {
                    for (std::int32_t j = 0; j < *m; j = j + 1) {
                        if (crt::fscanf(f, "%Flf", &tmp[j]) == -1) return 1;
                        v[*m * p + j + *m * ip * (idx + -1)] = tmp[j];
                    }
                }
            }
        }
        if (tmp != nullptr) std::free(tmp);
    }
    return 0;
}

// @src 140417c80  TIMERC / NRECVL / SIZESH / TXTODF / ERECVL: "count flag W default"
int read_recovery(std::FILE* f, LLOBJ_DATA& obj, std::int32_t code, std::int32_t n, std::int32_t* m,
                  std::int32_t mode)
{
    char buf[0x400];
    std::int32_t count = 0, flag = 0, w = 0;
    double defs[10] = {};
    double* v = nullptr;
    const std::int32_t old_m = *m;
    if (std::fgets(buf, 0x400, f) == nullptr) return 1;
    if (mode != 4) return 1;
    const int conv = crt::sscanf(buf, "%i %i %i %Flf", &count, &flag, &w, &defs[0]);
    const std::int32_t nd = conv + -3;
    if (w < 1) return 1;
    if (*m != w) *m = w;
    const std::int32_t mnew = w;
    if (conv < 1) return 1;
    if (flag == 0) {
        switch (code - 0x1d) {
        case 0: {
            const bool no_mark = false;
            v = svkw::grain2_values(set8a0(obj), &no_mark); // 140181420
            break;
        }
        case 5:
            svkw::set8a0_resize(set8a0(obj), 0x22, *m);
            v = svkw::timerc_values(set8a0(obj));
            break;
        case 6:
            svkw::set8a0_resize(set8a0(obj), 0x23, *m);
            v = svkw::nrecvl_values(set8a0(obj));
            break;
        case 0x19:
            svkw::set9e0_resize(set9e0(obj), 0x36, *m);
            v = svkw::sizesh_values(set9e0(obj));
            break;
        case 0x1a:
            svkw::set9e0_resize(set9e0(obj), 0x37, *m);
            v = svkw::txtodf_values(set9e0(obj));
            break;
        case 0x22:
            svkw::set9e0_resize(set9e0(obj), 0x3f, *m);
            v = svkw::erecvl_values(set9e0(obj));
            break;
        default:
            return 1;
        }
    } else {
        if (flag != 1) return 1;
        const std::int32_t keep = old_m < w ? old_m : w;
        if (code == 0x23) {
            // keep the old values of the first min(old, W) components
            const double* old = svkw::nrecvl_values(set8a0(obj));
            const std::int32_t nn = nodes(obj);
            double* copy = static_cast<double*>(std::malloc(static_cast<std::size_t>(nn) * 8 * static_cast<std::size_t>(old_m)));
            std::memcpy(copy, old, static_cast<std::size_t>(nn) * 8 * static_cast<std::size_t>(old_m));
            svkw::set8a0_resize(set8a0(obj), 0x23, mnew);
            v = svkw::nrecvl_values(set8a0(obj));
            for (std::int32_t i = 0; i < nn; i = i + 1)
                for (std::int32_t j = 0; j < keep; j = j + 1) v[i * mnew + j] = copy[i * old_m + j];
            std::free(copy);
        } else {
            if (code != 0x3f) return 1;
            const double* old = svkw::erecvl_values(set9e0(obj));
            const std::int32_t ne = elems(obj);
            double* copy = static_cast<double*>(std::malloc(static_cast<std::size_t>(ne) * 8 * static_cast<std::size_t>(old_m)));
            std::memcpy(copy, old, static_cast<std::size_t>(ne) * 8 * static_cast<std::size_t>(old_m));
            svkw::set9e0_resize(set9e0(obj), 0x3f, mnew);
            v = svkw::erecvl_values(set9e0(obj));
            for (std::int32_t i = 0; i < ne; i = i + 1)
                for (std::int32_t j = 0; j < keep; j = j + 1) v[i * mnew + j] = copy[i * old_m + j];
            std::free(copy);
        }
    }
    if (flag == 0) {
        if (0 < nd) {
            if (nd == *m) {
                for (std::int32_t i = 0; i < n; i = i + 1)
                    for (std::int32_t j = 0; j < *m; j = j + 1) v[i * *m + j] = defs[j];
            } else {
                for (std::int32_t i = 0; i < n; i = i + 1)
                    for (std::int32_t j = 0; j < *m; j = j + 1) v[i * *m + j] = defs[0];
            }
        }
    } else if (flag == 1) {
        std::int32_t sel[100] = {};
        double val[100] = {};
        for (std::int32_t j = 0; j < mnew; j = j + 1)
            if (crt::fscanf(f, "%i", &sel[j]) == -1) return 1;
        for (std::int32_t j = 0; j < mnew; j = j + 1)
            if (crt::fscanf(f, "%Flf", &val[j]) == -1) return 1;
        std::int32_t ne;
        if (code == 0x3f)
            ne = elems(obj);
        else {
            if (code != 0x23) return 1;
            ne = nodes(obj);
        }
        for (std::int32_t i = 0; i < ne; i = i + 1)
            for (std::int32_t j = 0; j < mnew; j = j + 1)
                if (sel[j] == 1) v[i * mnew + j] = val[j];
    }
    return read_value_records_k(f, v, count, n, *m, w); // 140419b90
}

// @src 1404176a0  GRAIN2: "count - W default" (twin of 140417c80 without the flag)
int read_grain2(std::FILE* f, LLOBJ_DATA& obj, std::int32_t code, std::int32_t n, std::int32_t* m,
                std::int32_t mode)
{
    char buf[0x400];
    std::int32_t count = 0, ign = 0, w = 0;
    double defs[10] = {};
    double* v = nullptr;
    if (std::fgets(buf, 0x400, f) == nullptr) return 1;
    if (mode != 4) return 1;
    const int conv = crt::sscanf(buf, "%i %i %i %Flf", &count, &ign, &w, &defs[0]);
    const std::int32_t nd = conv + -3;
    if (w < 1) return 1;
    if (*m != w) *m = w;
    if (conv < 1) return 1;
    switch (code - 0x1d) {
    case 0: {
        const bool no_mark = false;
        v = svkw::grain2_values(set8a0(obj), &no_mark);
        break;
    }
    case 5:
        svkw::set8a0_resize(set8a0(obj), 0x22, *m);
        v = svkw::timerc_values(set8a0(obj));
        break;
    case 6:
        svkw::set8a0_resize(set8a0(obj), 0x23, *m);
        v = svkw::nrecvl_values(set8a0(obj));
        break;
    case 0x19:
        svkw::set9e0_resize(set9e0(obj), 0x36, *m);
        v = svkw::sizesh_values(set9e0(obj));
        break;
    case 0x1a:
        svkw::set9e0_resize(set9e0(obj), 0x37, *m);
        v = svkw::txtodf_values(set9e0(obj));
        break;
    case 0x22:
        svkw::set9e0_resize(set9e0(obj), 0x3f, *m);
        v = svkw::erecvl_values(set9e0(obj));
        break;
    default:
        return 1;
    }
    if (0 < nd) {
        if (nd == *m) {
            for (std::int32_t i = 0; i < n; i = i + 1)
                for (std::int32_t j = 0; j < *m; j = j + 1) v[i * *m + j] = defs[j];
        } else {
            for (std::int32_t i = 0; i < n; i = i + 1)
                for (std::int32_t j = 0; j < *m; j = j + 1) v[i * *m + j] = defs[0];
        }
    }
    return read_value_records_k(f, v, count, n, *m, w);
}

// @src 1404192d0  GRAIN: "count W d1 ... d20"
int read_grain(std::FILE* f, LLOBJ_DATA& obj, std::int32_t code, std::int32_t n, std::int32_t* m,
               std::int32_t mode)
{
    char buf[0x400];
    std::int32_t count = 0, w = 0;
    double d[20] = {};
    if (mode == 2 && 0x14 < *m) return 1;
    if (std::fgets(buf, 0x400, f) == nullptr) return 1;
    if (mode != 2) return 1;
    const int conv = crt::sscanf(
        buf, "%i %i %Flf %Flf %Flf %Flf %Flf %Flf %Flf %Flf %Flf %Flf %Flf %Flf %Flf %Flf %Flf %Flf %Flf %Flf %Flf %Flf",
        &count, &w, &d[0], &d[1], &d[2], &d[3], &d[4], &d[5], &d[6], &d[7], &d[8], &d[9], &d[10], &d[11],
        &d[12], &d[13], &d[14], &d[15], &d[16], &d[17], &d[18], &d[19]);
    const std::int32_t nd = conv + -2;
    if (w < 1) return 1;
    if (*m != w) *m = w;
    if (conv < 1) return 1;
    if (code != 0x35) return 1;
    svkw::set9e0_resize(set9e0(obj), 0x35, *m);
    double* v = svkw::grain_values(set9e0(obj)); // 140184520
    if (0 < nd) {
        if (nd == *m) {
            for (std::int32_t i = 0; i < n; i = i + 1)
                for (std::int32_t j = 0; j < *m; j = j + 1) v[i * *m + j] = d[j];
        } else {
            for (std::int32_t i = 0; i < n; i = i + 1)
                for (std::int32_t j = 0; j < *m; j = j + 1) v[i * *m + j] = j < nd ? d[j] : 0.0;
        }
    }
    return read_value_records_k(f, v, count, n, *m, w);
}

// @src 1403981c0  VOLFC / VOLFS: the object's own phase columns scattered to the global ones
int read_phase_columns(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj, double* v, std::int32_t n,
                       std::int32_t mg)
{
    std::int32_t* map = static_cast<std::int32_t*>(std::calloc(static_cast<std::size_t>(mfield(m, 0x1320)), 4));
    std::int32_t ml = 0;
    svkw::phase_column_map(m, obj, map, &ml); // 140398350
    double* buf = static_cast<double*>(std::calloc(static_cast<std::size_t>(n * ml), 8));
    const int st = read_node_values(f, buf, n, &ml, 1); // 140416c90 mode 1
    if (st == 0) {
        for (std::int32_t e = 0; e < n; e = e + 1)
            for (std::int32_t c = 0; c < mg; c = c + 1)
                if (-1 < map[c]) v[e * mg + c] = buf[e * ml + map[c]];
        if (map != nullptr) std::free(map);
        if (buf != nullptr) std::free(buf);
    }
    return st; // on failure the two buffers are not freed (as in the original)
}

// STRAIN / STRESS: the integration-point branches
int read_strain(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj)
{
    char line[0x400];
    std::int32_t count = 0, nn = 0, k = 1, idx = 0;
    double def = 0.0, def2 = 0.0;
    int st = 0;
    std::fgets(line, 0x400, f); // status unchecked (fgetpos before it is unused)
    double* v = darr(obj, 0xa28);
    const int conv = crt::sscanf(line, "%i %Flf %i %Flf %i", &count, &def, &nn, &def2, &k);
    if (conv != 5) {
        return read_node_elem_double2(f, v, elems(obj), ncomp(m, 0x2b), darr(obj, 0x938), nodes(obj),
                                      ncomp(m, 0x1b), 0, line) == 0 ? 0 : 2; // 140417280
    }
    if (!is_2d(m)) {
        if (k == 8) {
            for (std::int32_t r = 0; r < count; r = r + 1) {
                if (read_i(f, &idx, false) != 0) return 2;
                st = 0;
                for (std::int32_t p = 0; p < 8; p = p + 1) st = read_d(f, &v[r * 8 + p]);
            }
        } else {
            for (std::int32_t r = 0; r < count; r = r + 1) {
                st = read_i(f, &idx, false);
                if (st != 0) return 2;
                st = read_d(f, &v[r]);
            }
        }
    } else {
        for (std::int32_t r = 0; r < count; r = r + 1) {
            if (read_i(f, &idx, false) != 0) return 2;
            st = 0;
            for (std::int32_t p = 0; p < 4; p = p + 1) st = read_d(f, &v[r * 4 + p]);
        }
    }
    if (count == 0) std::memset(v, 0, static_cast<std::size_t>(k * elems(obj)) << 3);
    return st == 0 ? 0 : 2;
}

int read_stress(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj)
{
    char line[0x400];
    std::int32_t count = 0, nn = 0, k = 1, mc = 0, idx = 0;
    double def = 0.0, def2 = 0.0;
    int st = 0;
    std::fgets(line, 0x400, f);
    double* v = darr(obj, 0xa00);
    if (!is_2d(m)) {
        const int conv = crt::sscanf(line, "%i %Flf %i %Flf %i", &count, &def, &nn, &def2, &k);
        if (conv != 5)
            return read_node_elem_double2(f, v, elems(obj), ncomp(m, 0x26), darr(obj, 0x948), nodes(obj),
                                          ncomp(m, 0x1a), 0, line) == 0 ? 0 : 2;
        if (k == 8) {
            for (std::int32_t r = 0; r < count; r = r + 1) {
                if (read_i(f, &idx, false) != 0) return 2;
                st = 0;
                for (std::int32_t p = 0; p < 8; p = p + 1) {
                    double* q = v + (r * 0x30 + p * 6);
                    double* const out[6] = {q, q + 1, q + 2, q + 3, q + 4, q + 5};
                    st = read_d6(f, out); // 140415080
                }
            }
        } else {
            for (std::int32_t r = 0; r < count; r = r + 1) {
                st = read_i(f, &idx, false);
                if (st != 0) return 2;
                double* q = v + r * 6;
                double* const out[6] = {q, q + 1, q + 2, q + 3, q + 4, q + 5};
                st = read_d6(f, out);
            }
        }
        if (count == 0)
            std::memset(v, 0, static_cast<std::size_t>(k * ncomp(m, 0x26) * elems(obj)) << 3);
    } else {
        const int conv = crt::sscanf(line, "%i %Flf %i %i %Flf %i", &count, &def, &mc, &nn, &def2, &k);
        if (conv != 6)
            return read_node_elem_double2(f, v, elems(obj), ncomp(m, 0x26), darr(obj, 0x948), nodes(obj),
                                          ncomp(m, 0x1a), 3, line) == 0 ? 0 : 2;
        if (mc == 6) {
            for (std::int32_t r = 0; r < count; r = r + 1) {
                if (read_i(f, &idx, false) != 0) return 2;
                st = 0;
                for (std::int32_t p = 0; p < 4; p = p + 1) {
                    double* q = v + (r * 0x18 + p * 6);
                    double* const out[6] = {q, q + 1, q + 2, q + 3, q + 4, q + 5};
                    st = read_d6(f, out);
                }
            }
            if (mfield(m, 0x850) != 3) {
                mfield(m, 0x850) = 3;
                def::rt::crt::fprintf(
                    stdout, "\n *** Info : Geometry type has been changed to torsional type due to STRESS Keyword.\n");
            }
        } else {
            for (std::int32_t r = 0; r < count; r = r + 1) {
                if (read_i(f, &idx, false) != 0) return 2;
                st = 0;
                for (std::int32_t p = 0; p < 4; p = p + 1) {
                    double* q = v + (r * 0x10 + p * 4);
                    st = read_d4(f, q, q + 1, q + 2, q + 3); // 140414e40
                }
            }
        }
        if (count == 0) std::memset(v, 0, static_cast<std::size_t>(k * mc * elems(obj)) << 3);
    }
    return st == 0 ? 0 : 2;
}

int read_damage(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj)
{
    char line[0x400];
    std::int32_t count = 0, i3 = 0, mm = 0, ip = 1;
    double def = 0.0, d4 = 0.0;
    std::fgets(line, 0x400, f);
    const int conv = crt::sscanf(line, "%i %Flf %i %Flf %i %i", &count, &def, &i3, &d4, &mm, &ip);
    int st;
    if (conv == 6) {
        st = read_damage_ip(f, obj, 0x2d, elems(obj), &ncomp(m, 0x2d), line); // 140418e40
    } else {
        const std::int32_t mode = conv == 5 ? 5 : 0;
        st = read_damage4(f, obj, 0x2d, elems(obj), &ncomp(m, 0x2d), 0x1c, nodes(obj), &ncomp(m, 0x1c), mode,
                          line); // 140418910
    }
    return st == 0 ? 0 : 2;
}

int read_usr(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj, bool nodal)
{
    std::int32_t count = 0, k = 0;
    double def = 0.0;
    if (read_i_d_i(f, &count, &def, &k) != 0) return 2; // 140415590
    if (nodal) {
        def::model::resize_node_names(&m, k); // 1401d21c0 (kw-g1b)
        for (std::int32_t i = 0; i < nodes(obj) * ncomp(m, 0x11); i = i + 1) svkw::usrnod_values(set8a0(obj))[i] = def;
        return read_value_records(f, svkw::usrnod_values(set8a0(obj)), count, nodes(obj), ncomp(m, 0x11)) == 0 ? 0 : 2;
    }
    if (k == 0) k = 2;
    def::model::resize_element_names(&m, k); // 1401d2090 (kw-g1b)
    for (std::int32_t i = 0; i < elems(obj) * ncomp(m, 0x3c); i = i + 1) svkw::usrelm_values(set9e0(obj))[i] = def;
    return read_value_records(f, svkw::usrelm_values(set9e0(obj)), count, elems(obj), ncomp(m, 0x3c)) == 0 ? 0 : 2;
}

int read_cusvar(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj)
{
    std::int32_t id = 0, type = 0, count = 0, mm = 0;
    double def = 0.0;
    if (read_iii_d_i(f, &id, &type, &count, &def, &mm) != 0) return 2; // 140416700
    def::model::StateVarCustom* v = svkw::custom_define(obj, id, type, mm, count); // 140185d20
    svkw::custom_fill(v, def);                                                    // 140301620
    double* data = v->value_data();
    const std::int32_t w = v->dim1(false);
    const std::int32_t nr = v->dim0(false);
    const int st = read_value_records(f, data, count, nr, w); // 1404199a0
    if (st == 0) svkw::set_first_int(svkw::model_custom_registry(m), id); // 14019a4e0, 14032f630
    return st == 0 ? 0 : 2;
}

int rnv(std::FILE* f, LLOBJ_DATA& o, std::size_t off, bool nodal, std::int32_t* m, std::int32_t mode)
{
    return read_node_values(f, darr(o, off), nodal ? nodes(o) : elems(o), m, mode) == 0 ? 0 : 2; // 140416c90
}

// Parser ids of this packet. DRZ, URZ, FRZ, PRZ and DRMESH (cases 0x10, 0x15..0x17, 0x77) are
// kw-g2geo's (mesh family).
constexpr int kIds[] = {170, 171, 172, 175, 176, 177, 178, 179, 196, 197, 198, 205, 206, 207, 208, 209,
                        210, 211, 213, 214, 216, 218, 223, 224, 225, 233, 237, 238, 239, 254, 255};

} // namespace

// @src 14032a140  number of custom variables (obj+0xb58)
int custom_count(LLOBJ_DATA& obj) { return svkw::custom_count(obj); }

// @src 140185f80  i-th custom variable or null
void* custom_at(LLOBJ_DATA& obj, int i) { return svkw::custom_at(obj, i); }

// @src 14032ec90  custom-variable id
std::int32_t custom_id(void* entry) { return static_cast<def::model::StateVarCustom*>(entry)->custom_id; }

bool handles(int id)
{
    for (int k : kIds)
        if (k == id) return true;
    return false;
}

// part of @src 14038b8a0  the kw-g2sv cases of the group 2 parser
int read_case(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj, int id)
{
    switch (id - 0x90) {
    case 0x1a: {                                                      // NDTMP
        if (!is_2d(m)) return rnv(f, obj, 0x8e0, true, &ncomp(m, 8), 0);
        std::int32_t count = 0, k = 0;
        double def = 0.0;
        if (read_i_d_i(f, &count, &def, &k) != 0) return 2;
        double* t = darr(obj, 0x8e0);
        double* dt = darr(obj, 0x900);
        for (std::int32_t i = 0; i < nodes(obj); i = i + 1) {
            t[i] = def;
            dt[i] = 0.0;
        }
        int st;
        if (k == 2) {
            double* buf = static_cast<double*>(std::calloc(static_cast<std::size_t>(nodes(obj) << 1), 8));
            for (std::int32_t i = 0; i < nodes(obj) * 2; i = i + 1) buf[i] = def;
            st = read_value_records(f, buf, count, nodes(obj), 2);
            for (std::int32_t i = 0; i < nodes(obj); i = i + 1) {
                t[i] = buf[i * 2 + 1];
                dt[i] = buf[i * 2 + 1] - buf[i * 2];
            }
            if (buf != nullptr) std::free(buf);
        } else {
            st = read_value_records(f, t, count, nodes(obj), ncomp(m, 8));
        }
        return st == 0 ? 0 : 2;
    }
    case 0x1b: return rnv(f, obj, 0x8e8, true, &ncomp(m, 9), 0);      // NDHEAT
    case 0x1c: return rnv(f, obj, 0x8f8, true, &ncomp(m, 0xb), 0);    // NDFLUX
    case 0x1f: return read_strain(m, f, obj);                         // STRAIN
    case 0x20: return rnv(f, obj, 0xa10, false, &ncomp(m, 0x28), 1);  // STNCMP
    case 0x21: return rnv(f, obj, 0xa30, false, &ncomp(m, 0x2c), 0);  // DENSTY
    case 0x22: return read_damage(m, f, obj);                         // DAMAGE
    case 0x23: return read_stress(m, f, obj);                         // STRESS
    case 0x34: return read_usr(m, f, obj, true);                      // USRNOD
    case 0x35: return read_usr(m, f, obj, false);                     // USRELM
    case 0x36: return read_cusvar(m, f, obj);                         // CUSVAR
    case 0x3d:                                                        // VOLFC
        return read_phase_columns(m, f, obj, darr(obj, 0xa58), elems(obj), ncomp(m, 0x31)) == 0 ? 0 : 2;
    case 0x3e: return rnv(f, obj, 0xa68, false, &ncomp(m, 0x33), 1);  // DVOLF
    case 0x3f: return rnv(f, obj, 0xa70, false, &ncomp(m, 0x34), 1);  // TICF
    case 0x40: {                                                      // GRAIN
        const int st = read_grain(f, obj, 0x35, elems(obj), &ncomp(m, 0x35), 2);
        if (st == 0 && mfield(m, 0x898) != 0) mfield(m, 0x898) = 0;
        return st == 0 ? 0 : 2;
    }
    case 0x41: {                                                      // GRAIN2
        const int st = read_grain2(f, obj, 0x1d, nodes(obj), &ncomp(m, 0x1d), 4);
        // 1401d70c0(model, &2): a query whose result the case discards (it reads the object and
        // material lists only); not called here
        if (st == 0 && mfield(m, 0x898) != 1) mfield(m, 0x898) = 1;
        return st == 0 ? 0 : 2;
    }
    case 0x42: return read_recovery(f, obj, 0x36, elems(obj), &ncomp(m, 0x36), 4) == 0 ? 0 : 2; // SIZESH
    case 0x43: return read_recovery(f, obj, 0x37, elems(obj), &ncomp(m, 0x37), 4) == 0 ? 0 : 2; // TXTODF
    case 0x45: return rnv(f, obj, 0x918, true, &ncomp(m, 0xf), 0);    // VOTAGE
    case 0x46: return rnv(f, obj, 0x908, true, &ncomp(m, 0xd), 0);    // DATOM
    case 0x48:                                                        // VOLFS
        return read_phase_columns(m, f, obj, darr(obj, 0xa60), elems(obj), ncomp(m, 0x32)) == 0 ? 0 : 2;
    case 0x4a: {                                                      // HDNEST
        std::int32_t a = 0, b = 0;
        double d1 = 0.0, d2 = 0.0;
        const int st = read_ii_dd(f, &a, &b, &d1, &d2); // 140415ff0
        at<std::int32_t>(obj, 0x1f48) = a;
        if (at<std::int32_t>(obj, 0x1f48) != 0) {
            at<std::int32_t>(obj, 0x1f44) = b;
            at<double>(obj, 0x1f50) = d1;
            at<double>(obj, 0x1f58) = d2;
        }
        return st == 0 ? 0 : 2;
    }
    case 0x4f: return read_recovery(f, obj, 0x22, nodes(obj), &ncomp(m, 0x22), 4) == 0 ? 0 : 2; // TIMERC
    case 0x50: return read_recovery(f, obj, 0x3f, elems(obj), &ncomp(m, 0x3f), 4) == 0 ? 0 : 2; // ERECVL
    case 0x51: return read_recovery(f, obj, 0x23, nodes(obj), &ncomp(m, 0x23), 4) == 0 ? 0 : 2; // NRECVL
    case 0x59: return rnv(f, obj, 0xa98, false, &ncomp(m, 0x39), 0);  // YLDS
    case 0x5d: return rnv(f, obj, 0xaa0, false, &ncomp(m, 0x3a), 0);  // ZEFI
    case 0x5e: return rnv(f, obj, 0xaa8, false, &ncomp(m, 0x3b), 0);  // ZMFI
    case 0x5f: return rnv(f, obj, 0xab0, false, &ncomp(m, 0x3e), 0);  // CURRNT
    case 0x6e: {                                                      // MATAXI
        ncomp(m, 0x2e) = is_3d(m) ? 9 : 1;
        const bool mark = false;
        double* v = svkw::mataxi_values(set9e0(obj), &mark); // 140184150
        return read_node_values(f, v, elems(obj), &ncomp(m, 0x2e), 2) == 0 ? 0 : 2;
    }
    case 0x6f: {                                                      // NWEAR
        ncomp(m, 0x18) = 5;
        const bool mark = false;
        double* v = svkw::nwear_values(set8a0(obj), &mark); // 140181190
        if (v == nullptr) return 0;
        return read_node_values(f, v, nodes(obj), &ncomp(m, 0x18), 0) == 0 ? 0 : 2;
    }
    default:
        return 2;
    }
}

} // namespace def::key::g2sv
