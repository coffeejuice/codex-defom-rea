#define _CRT_SECURE_NO_WARNINGS
#include <defkey/table_io.hpp>

#include <cstdlib>
#include <cstring>
#include <vector>

#include <defkey/crt_scan.hpp>
#include <defkey/line_io.hpp>
#include <defmodel/dynary_init.hpp>
#include <defmodel/keyword_registry.hpp>
#include <defrt/ftnstr/ftnstr.hpp>
#include <defrt/stdlib/legacy_printf.hpp>

namespace def::key {

using def::model::dynary_alloc;
using def::model::dynary_counts_positive;
using def::model::dynary_reinit_keep;
using def::model::dynary_value_count;

namespace {

int fscan_double(std::FILE* f, double* d) { return crt::fscanf(f, "%Flf", d); }

int max_count(const DYNARY* t)
{
    int m = 0;
    for (int i = 0; i < t->ndim; ++i) m = m < t->n[i] ? t->n[i] : m;
    return m;
}

} // namespace

// @src 14041a800
int read_table_body(std::FILE* f, DYNARY* t, int /*unused*/)
{
    char buf[kLineSize];
    if (t->ndim != 0 || 1 < t->ncomp) {
        dynary_alloc(t);
        const int nd = t->ndim;
        if (nd == 0) {
            if (t->ncomp < 2) return 0;
            for (int k = 0; k < t->ncomp; ++k)
                if (fscan_double(f, t->values + k) == -1) return 1;
        } else if (nd == 1) {
            for (int p = 0; p < t->n[0]; ++p) {
                if (fscan_double(f, t->axis[0] + p) == -1) return 1;
                const int nc = t->ncomp;
                for (int k = 0; k < nc; ++k)
                    if (fscan_double(f, t->values + (p * nc + k)) == -1) return 1;
            }
        } else if (1 < nd && nd < 4) {
            for (int d = 0; d < t->ndim; ++d)
                for (int p = 0; p < t->n[d]; ++p)
                    if (fscan_double(f, t->axis[d] + p) == -1) return 1;
            // Read into a temporary (operator new in the original, leaked on the early return),
            // then copied into the value array.
            const int total = dynary_value_count(t);
            std::vector<double> tmp(static_cast<std::size_t>(total));
            for (int k = 0; k < total; ++k)
                if (fscan_double(f, tmp.data() + k) == -1) return 1;
            if (total > 0) std::memcpy(t->values, tmp.data(), static_cast<std::size_t>(total) << 3);
        }
        if (0 < dynary_value_count(t)) std::fgets(buf, kLineSize, f);
    }
    return 0;
}

// @src 14041ac10
int read_table_vector(std::FILE* f, DYNARY* t)
{
    if (t->ndim != 0 || t->ncomp < 1) return 1;
    dynary_alloc(t);
    for (int k = 0; k < t->ncomp; ++k)
        if (fscan_double(f, t->values + k) == -1) return 1;
    return 0;
}

// @src 14041b270
int read_table_header(std::FILE* f, DYNARY* t, int want_extra, double* extra)
{
    char buf[kLineSize];
    std::int32_t type = 0;
    double a = 0.0, b = 0.0, c = 0.0;
    if (std::fgets(buf, kLineSize, f) == nullptr) return 1;
    replace_char(buf, ',', ' ');
    const int nconv = crt::sscanf(buf, "%i %Flf %Flf %Flf", &type, &a, &b, &c);
    if (nconv == -1) return 1;
    dynary_reinit_keep(t, 0);
    switch (type) {
    case 0: t->ndim = 0; t->value = a; break;
    case 1: t->ndim = 1; t->var[0] = 3; t->n[0] = static_cast<std::int32_t>(a); break;
    case 2: t->ndim = 1; t->var[0] = 9; t->n[0] = static_cast<std::int32_t>(a); break;
    case 3: t->ndim = 1; t->var[0] = 0xc; t->n[0] = static_cast<std::int32_t>(a); break;
    case 4:
        t->ndim = 2; t->var[0] = 3; t->var[1] = 0xc;
        t->n[0] = static_cast<std::int32_t>(a); t->n[1] = static_cast<std::int32_t>(b);
        break;
    case 5:
        t->ndim = 2; t->var[0] = 3; t->var[1] = 8;
        t->n[0] = static_cast<std::int32_t>(a); t->n[1] = static_cast<std::int32_t>(b);
        break;
    case 6: t->ndim = 1; t->var[0] = 8; t->n[0] = static_cast<std::int32_t>(a); break;
    case 7:
        t->ndim = 2; t->var[0] = 3; t->var[1] = 4;
        t->n[0] = static_cast<std::int32_t>(a); t->n[1] = static_cast<std::int32_t>(b);
        break;
    default: break;
    }
    if (-1 < want_extra && extra != nullptr && -1 < type) {
        if (type < 4) {
            if (2 < nconv) *extra = b;
        } else if (type == 4 && 3 < nconv) {
            *extra = c;
        }
    }
    return read_table_body(f, t, 0);
}

// @src 14041ace0
int read_region_table(std::FILE* f, DYNARY* t)
{
    char buf[kLineSize];
    std::int32_t region = 0, nd = 0, third = 0;
    double a = 0.0, b = 0.0;
    read_iii_dd(f, &region, &nd, &third, &a, &b); // status ignored, as in the original
    t->ndim = nd;
    if (t->ndim == 0) {
        t->ncomp = third;
        dynary_alloc(t);
        if (t->ncomp < 2) {
            t->value = a;
        } else {
            for (int k = 0; k < t->ncomp; ++k)
                if (fscan_double(f, t->values + k) == -1) return 1;
        }
    } else {
        if (t->ndim != 1) return 1;
        t->var[0] = third;
        t->n[0] = static_cast<std::int32_t>(a);
        if (t->n[0] == 0) {
            t->value = b;
            t->ncomp = 0;
        } else {
            t->ncomp = static_cast<std::int32_t>(b);
        }
        dynary_alloc(t);
        for (int p = 0; p < t->n[0]; ++p) {
            if (fscan_double(f, t->axis[0] + p) == -1) return 1;
            const int nc = t->ncomp;
            for (int k = 0; k < nc; ++k)
                if (fscan_double(f, t->values + (p * nc + k)) == -1) return 1;
        }
    }
    if (0 < dynary_value_count(t)) std::fgets(buf, kLineSize, f);
    return 0;
}

// @src 14041b060
int read_region_string_table(std::FILE* f, DYNARY* t)
{
    std::int32_t region = 0, nd = 0, third = 0;
    double a = 0.0, b = 0.0;
    char line[0x40] = {}; // stack in the original; on EOF the previous line is copied again
    read_iii_dd(f, &region, &nd, &third, &a, &b);
    t->ndim = nd;
    if (t->ndim != 0) {
        if (t->ndim != 1) return 1;
        t->var[0] = third;
        t->n[0] = static_cast<std::int32_t>(a);
        t->ncomp = static_cast<std::int32_t>(b);
        dynary_alloc(t);
        for (int p = 0; p < t->n[0]; ++p) {
            // values + (p*64)/8 doubles = byte offset 64*p
            char* dst = reinterpret_cast<char*>(t->values) + static_cast<std::int64_t>(p) * 0x40;
            read_string_line(f, line, 0x40); // status ignored
            def::rt::to_fortran(dst, line, 0x40); // 140323bf0 (p1-10), length unused
        }
    }
    return 0;
}

// @src 1402f2e80
int write_doubles(std::FILE* f, int n, const double* v, int per_line, bool alt)
{
    if (f == nullptr || per_line < 1) return 2;
    if (n < 1 || v == nullptr) return 0;
    for (int i = 0; i < n; ++i) {
        if (i != 0 && i % per_line == 0) {
            def::rt::crt::fprintf(f, "\n");
            def::rt::crt::fprintf(f, "        ");
        }
        if (!alt)
            def::rt::crt::fprintf(f, " %20.10E", v[i]);
        else
            def::rt::crt::fprintf(f, " %20.10E,", v[i]); // strcpy " %20.10E" + strcat "," (str 1405417fc)
    }
    def::rt::crt::fprintf(f, "\n");
    return 0;
}

// @src 1402f2fe0
int write_ints(std::FILE* f, int n, const std::int32_t* v, int per_line, bool alt)
{
    if (f == nullptr || per_line < 1) return 2;
    if (n < 1 || v == nullptr) return 0;
    for (int i = 0; i < n; ++i) {
        if (i != 0 && i % per_line == 0) {
            def::rt::crt::fprintf(f, "\n");
            def::rt::crt::fprintf(f, "        ");
        }
        if (!alt)
            def::rt::crt::fprintf(f, " %7i", v[i]);
        else
            def::rt::crt::fprintf(f, " %7i,", v[i]); // str 140542444 + str 140541c44
    }
    def::rt::crt::fprintf(f, "\n");
    return 0;
}

// @src 1402f3130
int write_table_body(std::FILE* f, const DYNARY* t, int mode)
{
    if (f == nullptr) return 2;
    if (t == nullptr) return 0;
    if (mode != 1 && mode != 2 && !dynary_counts_positive(t)) return 0;
    const bool with_counts = mode == 1 || mode == 2;
    const int per_line = max_count(t);
    const int nd = t->ndim;
    int r;
    if (nd == 0) {
        if (!with_counts && mode != 6) return 0;
        if (t->ncomp == 1) {
            def::rt::crt::fprintf(f, " %20.10E\n", t->value);
            return 0;
        }
        if (t->ncomp < 2) return 0;
        r = write_doubles(f, t->ncomp, t->values, 5, false);
    } else {
        if (nd == 1) {
            if (with_counts) def::rt::crt::fprintf(f, " %7i\n", t->n[0]);
            for (int p = 0; p < t->n[0]; ++p) {
                def::rt::crt::fprintf(f, " %20.10E", t->axis[0][p]);
                r = write_doubles(f, t->ncomp, t->values + p * t->ncomp, 5, false);
                if (r != 0) return r;
            }
            return 0;
        }
        if (nd < 2 || 3 < nd) return 0;
        if (with_counts) {
            r = write_ints(f, t->ndim, t->n, t->ndim, mode == 2);
            if (r != 0) return r;
        }
        for (int d = 0; d < t->ndim; ++d) {
            r = write_doubles(f, t->n[d], t->axis[d], per_line, mode == 2);
            if (r != 0) return r;
        }
        r = write_doubles(f, dynary_value_count(t), t->values, per_line, mode == 2);
    }
    return r;
}

// @src 1402f34f0
int write_region_table(std::FILE* f, const DYNARY* t, int mode, int region)
{
    if (f == nullptr) return 2;
    if (t == nullptr) return 0;
    if (t->ndim == 0) {
        if (mode == 1) {
            if (t->ncomp < 2) {
                def::rt::crt::fprintf(f, " %7i %7i %7i %20.10E\n", region, t->ndim, t->ncomp, t->value);
            } else {
                def::rt::crt::fprintf(f, " %7i %7i %7i\n", region, t->ndim, t->ncomp);
                const int r = write_doubles(f, t->ncomp, t->values, 5, false);
                if (r != 0) return r;
            }
        }
    } else if (t->ndim == 1 && mode == 1) {
        if (t->n[0] == 0) {
            def::rt::crt::fprintf(f, " %7i %7i %7i %7i %20.10E\n", region, t->ndim, t->var[0], t->n[0], t->value);
        } else {
            def::rt::crt::fprintf(f, " %7i %7i %7i %7i %7i\n", region, t->ndim, t->var[0], t->n[0], t->ncomp);
            for (int p = 0; p < t->n[0]; ++p) {
                if (t->var[0] == 0)
                    def::rt::crt::fprintf(f, " %7i", p + 1);
                else
                    def::rt::crt::fprintf(f, " %20.10E", t->axis[0][p]);
                const int r = write_doubles(f, t->ncomp, t->values + p * t->ncomp, 5, false);
                if (r != 0) return r;
            }
        }
    }
    return 0;
}

// @src 1402f3840
int write_region_string_table(std::FILE* f, const DYNARY* t, int mode, int region)
{
    if (f == nullptr) return 2;
    if (t == nullptr) return 0;
    if (t->ndim != 0 && t->ndim == 1 && mode == 1) {
        def::rt::crt::fprintf(f, " %7i %7i %7i %7i %7i\n", region, t->ndim, t->var[0], t->n[0], t->ncomp);
        char line[0x40 + 1];
        for (int p = 0; p < t->n[0]; ++p) {
            const char* src = reinterpret_cast<const char*>(t->values) + static_cast<std::int64_t>(p) * 0x40;
            // strncpy into a 64-byte local, then 1403239f0(local, 64) terminates at the trim point.
            // The original's local is 64 bytes, so an untrimmed 64-char entry writes the NUL one
            // byte past it; the port gives it a 65th byte.
            std::strncpy(line, src, 0x40);
            def::rt::trim_fortran(line, 0x40);
            def::rt::crt::fprintf(f, "%s\n", line);
        }
    }
    return 0;
}

// @src 1402f3aa0
int write_table_header_d(std::FILE* f, const char* name, int mat, const DYNARY* t, int has_extra,
                         const double* extra)
{
    if (f == nullptr) return 2;
    const bool coarse = std::strcmp(name, "COARSE") == 0;
    if (t->ndim == 0) {
        if (has_extra < 1) {
            def::rt::crt::fprintf(f, "%-6s %7i %7i %20.10E\n", name, mat, 0, t->value);
        } else if (has_extra == 1 && extra != nullptr) {
            if (coarse) {
                def::rt::crt::fprintf(f, "%-6s %7i %7i\n", name, mat, static_cast<std::int32_t>(*extra));
                def::rt::crt::fprintf(f, " %7i %20.10E\n", 0, t->value);
            } else {
                def::rt::crt::fprintf(f, "%-6s %7i %7i %20.10E %20.10E\n", name, mat, 0, t->value, *extra);
            }
        }
    } else if (t->ndim == 1) {
        int type;
        if (t->var[0] == 3) type = 1;
        else if (t->var[0] == 9) type = 2;
        else if (t->var[0] == 8) type = 6;
        else type = 3;
        if (has_extra < 1)
            def::rt::crt::fprintf(f, "%-6s %7i %7i %7i\n", name, mat, type, t->n[0]);
        else if (has_extra == 1 && extra != nullptr)
            def::rt::crt::fprintf(f, "%-6s %7i %7i %7i %20.10E\n", name, mat, type, t->n[0], *extra);
    } else if (t->ndim == 2) {
        if (coarse) {
            const int type = (t->var[0] == 3 && t->var[1] == 8) ? 5 : 0;
            if (has_extra == 1 && extra != nullptr) {
                def::rt::crt::fprintf(f, "%-6s %7i %7i\n", name, mat, static_cast<std::int32_t>(*extra));
                def::rt::crt::fprintf(f, " %7i %7i %7i\n", type, t->n[0], t->n[1]);
            } else {
                def::rt::crt::printf("\nWarning: data for keyword %s has error. Its data type will be clear(0).\n",
                            def::model::keyword_name(0x7e));
                def::rt::crt::fprintf(f, "%-6s %7i %7i\n", name, mat, 0);
            }
        } else {
            int type = -1;
            if (t->var[0] == 3 && t->var[1] == 0xc) type = 4;
            else if (t->var[0] == 3 && t->var[1] == 4) type = 7;
            else if (t->var[0] == 3 && t->var[1] == 0x14) type = 4;
            if (type != -1) {
                if (has_extra < 1)
                    def::rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i\n", name, mat, type, t->n[0], t->n[1]);
                else if (has_extra == 1 && extra != nullptr)
                    def::rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i %20.10E\n", name, mat, type, t->n[0], t->n[1], *extra);
            }
        }
    }
    return write_table_body(f, t, 0);
}

// @src 1402f40f0
int write_table_header_i(std::FILE* f, const char* name, int mat, const DYNARY* t, int has_extra,
                         const std::int32_t* extra)
{
    if (f == nullptr) return 2;
    if (t->ndim == 0) {
        if (has_extra < 1)
            def::rt::crt::fprintf(f, "%-6s %7i %7i %20.10E\n", name, mat, 0, t->value);
        else if (has_extra == 1 && extra != nullptr)
            def::rt::crt::fprintf(f, "%-6s %7i %7i %20.10E %7i\n", name, mat, 0, t->value, *extra);
    } else if (t->ndim == 1) {
        int type;
        if (t->var[0] == 3) type = 1;
        else if (t->var[0] == 9) type = 2;
        else type = 3;
        if (has_extra < 1)
            def::rt::crt::fprintf(f, "%-6s %7i %7i %7i\n", name, mat, type, t->n[0]);
        else if (has_extra == 1 && extra != nullptr)
            def::rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i\n", name, mat, type, t->n[0], *extra);
    } else if (t->ndim == 2) {
        if (has_extra < 1)
            def::rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i\n", name, mat, 4, t->n[0], t->n[1]);
        else if (has_extra == 1 && extra != nullptr)
            def::rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i %7i\n", name, mat, 4, t->n[0], t->n[1], *extra);
    }
    return write_table_body(f, t, 0);
}

} // namespace def::key
