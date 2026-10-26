// KwdWriteUtl line writers of the state-variable writer (packet kw-g2sv).
// Spec: docs/re/keywords/group2-statevar-writers.md. Decompile: deform-rea-work/out/g2/D_lw.c.
#include <defkey/kw_g2sv.hpp>

#include <cstdlib>

#include <defkey/table_io.hpp>
#include <defmodel/bc/boundary_topology.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defrt/stdlib/legacy_printf.hpp>

#include "sv_internal.hpp"

namespace def::key::g2sv {

using namespace detail;

// @src 1402f5360  uniformity, defaults and record count
void analyse_values(const double* v, const std::int32_t* n, const std::int32_t* m, const double* d,
                    const std::int32_t* fmt, bool* uniform, std::int32_t* count, double* defs)
{
    if (*fmt == 2) {
        for (std::int32_t i = 0; i < *n && *uniform; i = i + 1) {
            for (std::int32_t j = 0; j < *m && *uniform; j = j + 1) {
                if (v[i * *m + j] != v[j]) *uniform = false;
            }
        }
    } else {
        for (std::int32_t i = 0; i < *n * *m && *uniform; i = i + 1) {
            if (v[i] != *v) *uniform = false;
        }
    }
    if (!*uniform) {
        for (std::int32_t j = 0; j < *m; j = j + 1) defs[j] = *d;
        for (std::int32_t i = 0; i < *n; i = i + 1) {
            for (std::int32_t j = 0; j < *m; j = j + 1) {
                if (v[i * *m + j] != *d) {
                    *count = *count + 1;
                    break;
                }
            }
        }
    } else {
        for (std::int32_t j = 0; j < *m; j = j + 1) defs[j] = v[j];
    }
}

namespace {

// @src 1402f55b0  the same for `ip` points of `m` values per entity
void analyse_values_ip(const double* v, const std::int32_t* n, const std::int32_t* m, const double* d,
                       const std::int32_t* fmt, bool* uniform, std::int32_t* count, double* defs,
                       const std::int32_t* ip)
{
    if (*fmt == 2) {
        for (std::int32_t i = 0; i < *n && *uniform; i = i + 1) {
            for (std::int32_t p = 0; p < *ip && *uniform; p = p + 1) {
                for (std::int32_t j = 0; j < *m && *uniform; j = j + 1) {
                    if (v[p * *m + j + i * *m * *ip] != v[j]) *uniform = false;
                }
            }
        }
    } else {
        for (std::int32_t i = 0; i < *n * *ip * *m && *uniform; i = i + 1) {
            if (v[i] != *v) *uniform = false;
        }
    }
    if (!*uniform) {
        for (std::int32_t j = 0; j < *m; j = j + 1) defs[j] = *d;
        for (std::int32_t i = 0; i < *n; i = i + 1) {
            std::int32_t same = 0;
            for (std::int32_t p = 0; p < *ip; p = p + 1) {
                std::int32_t j = 0;
                while (j < *m && v[p * *m + j + i * *m * *ip] == *d) {
                    same = same + 1;
                    j = j + 1;
                }
            }
            if (same != *ip * *m) *count = *count + 1;
        }
    } else {
        for (std::int32_t j = 0; j < *m; j = j + 1) defs[j] = v[j];
    }
}

FILE* crt_stdout() { return stdout; } // FUN_14047903c() + 0x30

} // namespace

// @src 1402f5c10  writeNodeElemVarDouble
int write_node_elem_double(std::FILE* f, const char* name, std::uint32_t no, const double* v,
                           std::int32_t n, std::int32_t m, double d, std::int32_t fmt,
                           std::int32_t k)
{
    int r = 0;
    std::int32_t count = 0;
    bool uniform = true;
    if (n == 0 || m == 0 || v == nullptr || name == nullptr) return 0;
    if (f == nullptr) return 2;
    double* defs = static_cast<double*>(std::calloc(static_cast<std::size_t>(m), 8));
    analyse_values(v, &n, &m, &d, &fmt, &uniform, &count, defs);
    switch (fmt) {
    case 0:
        def::rt::crt::fprintf(f, "%-6s %7i %7i %20.10E\n", name, no, count, defs[0]);
        break;
    case 1:
        def::rt::crt::fprintf(f, "%-6s %7i %7i %7i %20.10E\n", name, no, count, m, defs[0]);
        break;
    case 2:
        def::rt::crt::fprintf(f, "%-6s %7i %7i %7i", name, no, count, m);
        for (std::int32_t j = 0; j < m; j = j + 1) def::rt::crt::fprintf(f, " %20.10E", defs[j]);
        def::rt::crt::fprintf(f, "\n");
        break;
    case 3:
        def::rt::crt::fprintf(f, "%-6s %7i %7i %20.10E %7i\n", name, no, count, defs[0], m);
        break;
    case 4:
        def::rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i %20.10E\n", name, no, count, k, m, defs[0]);
        break;
    case 5:
        def::rt::crt::fprintf(f, "%-6s %7i %7i %20.10E %7i %20.10E %7i\n", name, no, count, defs[0], 0,
                              0.0, m);
        break;
    default:
        def::rt::crt::fprintf(crt_stdout(), "KwdWriteUtl::writeNodeElemVarDouble() - Not supported format\n");
        break;
    }
    if (!uniform) {
        for (std::int32_t i = 0; i < n; i = i + 1) {
            bool any = false;
            for (std::int32_t j = 0; j < m; j = j + 1) {
                if (v[i * m + j] != d) {
                    any = true;
                    break;
                }
            }
            if (any) {
                def::rt::crt::fprintf(f, " %7i", i + 1);
                r = write_doubles(f, m, v + i * m, 5, false); // 1402f2e80
                if (r != 0) break;
            }
        }
    }
    if (defs != nullptr) std::free(defs);
    return r;
}

// @src 1402f4df0  integer per-entity writer
int write_node_elem_int(std::FILE* f, const char* name, std::uint32_t no, const std::int32_t* v,
                        std::int32_t n, std::int32_t m, std::int32_t d)
{
    bool uniform = true;
    if (n == 0 || m == 0 || v == nullptr || name == nullptr) return 0;
    if (f == nullptr) return 2;
    for (std::int32_t i = 0; i < n * m; i = i + 1) {
        if (v[i] != *v) {
            uniform = false;
            break;
        }
    }
    std::int32_t count = 0;
    if (uniform) {
        def::rt::crt::fprintf(f, "%-6s %7i %7i %7i\n", name, no, 0, *v);
    } else {
        for (std::int32_t i = 0; i < n; i = i + 1) {
            for (std::int32_t j = 0; j < m; j = j + 1) {
                if (v[i * m + j] != d) {
                    count = count + 1;
                    break;
                }
            }
        }
        def::rt::crt::fprintf(f, "%-6s %7i %7i %7i\n", name, no, count, d);
        for (std::int32_t i = 0; i < n; i = i + 1) {
            bool any = false;
            for (std::int32_t j = 0; j < m; j = j + 1) {
                if (v[i * m + j] != d) {
                    any = true;
                    break;
                }
            }
            if (any) {
                def::rt::crt::fprintf(f, " %7i", i + 1);
                const int r = write_ints(f, m, v + i * m, 10, false); // 1402f2fe0
                if (r != 0) return r;
            }
        }
    }
    return 0;
}

// @src 1402f60c0  writeNodeElemVarDouble2
int write_node_elem_double2(std::FILE* f, const char* name, std::uint32_t no, const double* v1,
                            std::int32_t n1, std::int32_t m1, double d1, const double* v2,
                            std::int32_t n2, std::int32_t m2, double d2, std::int32_t fmt)
{
    int r = 0;
    std::int32_t count1 = 0, count2 = 0;
    bool uni1 = true, uni2 = true;
    if (n1 == 0 || m1 == 0 || v1 == nullptr || name == nullptr) return 0;
    if (n2 == 0 || m2 == 0 || v2 == nullptr) return 0;
    if (f == nullptr) return 2;
    double* defs1 = static_cast<double*>(std::calloc(static_cast<std::size_t>(m1), 8));
    double* defs2 = static_cast<double*>(std::calloc(static_cast<std::size_t>(m2), 8));
    analyse_values(v1, &n1, &m1, &d1, &fmt, &uni1, &count1, defs1);
    analyse_values(v2, &n2, &m2, &d2, &fmt, &uni2, &count2, defs2);
    if (fmt == 0) {
        def::rt::crt::fprintf(f, "%-6s %7i %7i %20.10E %7i %20.10E\n", name, no, count1, defs1[0],
                              count2, defs2[0]);
    } else if (fmt == 3) {
        def::rt::crt::fprintf(f, "%-6s %7i %7i %20.10E %7i %7i %20.10E\n", name, no, count1, defs1[0],
                              m1, count2, defs2[0]);
    } else if (fmt == 5) {
        def::rt::crt::fprintf(f, "%-6s %7i %7i %20.10E %7i %20.10E %7i\n", name, no, count1, defs1[0],
                              count2, defs2[0], m1);
    } else {
        def::rt::crt::fprintf(crt_stdout(), "KwdWriteUtl::writeNodeElemVarDouble2() - Not supported format\n");
    }
    if (!uni1) {
        for (std::int32_t i = 0; i < n1; i = i + 1) {
            bool any = false;
            for (std::int32_t j = 0; j < m1; j = j + 1) {
                if (v1[i * m1 + j] != d1) {
                    any = true;
                    break;
                }
            }
            if (any) {
                def::rt::crt::fprintf(f, " %7i", i + 1);
                r = write_doubles(f, m1, v1 + i * m1, 5, false);
                if (r != 0) break;
            }
        }
    }
    if (!uni2) {
        for (std::int32_t i = 0; i < n2; i = i + 1) {
            bool any = false;
            for (std::int32_t j = 0; j < m2; j = j + 1) {
                if (v2[i * m2 + j] != d2) {
                    any = true;
                    break;
                }
            }
            if (any) {
                def::rt::crt::fprintf(f, " %7i", i + 1);
                r = write_doubles(f, m2, v2 + i * m2, 5, false);
                if (r != 0) break;
            }
        }
    }
    if (defs1 != nullptr) std::free(defs1);
    if (defs2 != nullptr) std::free(defs2);
    return r;
}

// @src 1402f58d0  integration-point writer
int write_node_elem_ip(std::FILE* f, const char* name, std::uint32_t no, const double* v,
                       std::int32_t n, std::int32_t m, double d, std::int32_t ip, std::int32_t fmt)
{
    int r = 0;
    std::int32_t count = 0;
    bool uniform = true;
    if (n == 0 || m == 0 || v == nullptr || name == nullptr) return 0;
    if (f == nullptr) return 2;
    double* defs = static_cast<double*>(std::calloc(static_cast<std::size_t>(m), 8));
    analyse_values_ip(v, &n, &m, &d, &fmt, &uniform, &count, defs, &ip); // 1402f55b0
    def::rt::crt::fprintf(f, "%-6s %7i %7i %20.10E %7i %20.10E %7i %7i\n", name, no, count, defs[0], 0,
                          0.0, m, ip);
    if (!uniform) {
        for (std::int32_t i = 0; i < n; i = i + 1) {
            bool any = false;
            for (std::int32_t p = 0; p < ip; p = p + 1) {
                for (std::int32_t j = 0; j < m; j = j + 1) {
                    if (v[m * p + j + m * ip * i] != d) {
                        any = true;
                        break;
                    }
                }
                if (any) break;
            }
            if (any) {
                def::rt::crt::fprintf(f, " %7i\n", i + 1);
                for (std::int32_t p = 0; p < ip; p = p + 1) {
                    r = write_doubles(f, m, v + (i * m * ip + p * m), 5, false);
                    if (r != 0) break;
                }
                if (r != 0) break;
            }
        }
    }
    if (defs != nullptr) std::free(defs);
    return r;
}

// @src 1402f7490  bare matrix: "rows  cols", then each row as "%15.7E  " values and a newline
int write_matrix(std::FILE* f, const double* v, std::int32_t cols, std::int32_t rows)
{
    def::rt::crt::fprintf(f, "%d  %d\n", rows, cols);
    if (cols < 1 || rows < 1) return 1;
    for (std::int32_t r = 0; r < rows; r = r + 1) {
        for (std::int32_t c = 0; c < cols; c = c + 1) def::rt::crt::fprintf(f, "%15.7E  ", v[cols * r + c]);
        def::rt::crt::fprintf(f, "\n");
    }
    return 0;
}

} // namespace def::key::g2sv
