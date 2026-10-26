// Min / max of object state variables and the derived arrays they need (packet p4-07).
// Declarations: defmodel/hdrstats/object_stats.hpp. All C++ (MSVC, unoptimised): the decompile's
// expression order is the compiled one.
#include <defmodel/hdrstats/object_stats.hpp>

#include <cmath>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/state_var_base.hpp>
#include <defrt/stdlib/legacy_printf.hpp>
#include <defrt/stdlib/libm.hpp>
#include <defrt/stdlib/refcount.hpp>

namespace def::model::p407 {

namespace {

template <class T>
T& at(void* base, std::size_t off)
{
    return *reinterpret_cast<T*>(static_cast<std::byte*>(base) + off);
}

std::int32_t nodes(LLOBJ_DATA* obj) { return obj->u87c[1]; }  // +0x880
std::int32_t elems(LLOBJ_DATA* obj) { return obj->u87c[2]; }  // +0x884

// model state-variable layout word at `off` (DB_STEP_DATA::u15f8 starts at 0x15f8)
std::int32_t sv_dim(DB_STEP_DATA* m, std::size_t off) { return at<std::int32_t>(m, off); }

constexpr double k_zero = 0.0;            // 14051bed8
constexpr double k_eight = 8.0;           // 140522f40
constexpr double k_four = 4.0;            // 14051f248
constexpr double k_three = 3.0;           // 14051f1c8
constexpr double k_two = 2.0;             // 140509fb8
constexpr float k_two_f = 2.0f;           // 140530be0
constexpr double k_big = 9999.0;          // 140530be8
constexpr double k_mbig = -9999.0;        // 140530bf0
constexpr double k_halfpi = 1.5707963267948966; // 14051f238 (0x3ff921fb54442d18)

// The scan shared by 14020cae0 and 14020d320. kind 1 / 2: value or component *comp of `width`;
// kind 3 / 4: length of the row of `width` values.
void scan(LLOBJ_DATA* obj, double* arr, std::int32_t kind, std::int32_t width, double* vmin,
          double* vmax, const std::int32_t* comp, const bool* init)
{
    std::int32_t rows = 0;
    if (kind == 1 || kind == 3) {
        rows = nodes(obj);
    } else if (kind == 2 || kind == 4) {
        rows = elems(obj);
    }
    if (kind == 1 || kind == 2) {
        if (*init) {
            *vmax = *arr;
            *vmin = *vmax;
        }
        for (std::int32_t i = 0; i < rows; i = i + 1) {
            double v;
            if (width < 2) {
                v = arr[i];
            } else {
                v = arr[i * width + -1 + *comp];
            }
            if (v < *vmin) *vmin = v;
            if (*vmax <= v && v != *vmax) *vmax = v;
        }
    } else if (kind == 3 || kind == 4) {
        if (*init) {
            *vmax = *arr;
            *vmin = *vmax;
        }
        for (std::int32_t i = 0; i < rows; i = i + 1) {
            double v = 0.0;
            for (std::int32_t k = 0; k < width; k = k + 1) {
                const double c = arr[i * width + k];
                v = v + c * c;
            }
            if (v != k_zero) v = std::sqrt(v); // 140484644
            if (v < *vmin) *vmin = v;
            if (*vmax <= v && v != *vmax) *vmax = v;
        }
    }
}

} // namespace

// @src 14020cae0
void obj_min_max(LLOBJ_DATA* obj, const std::int32_t* code, double* vmin, double* vmax,
                 const std::int32_t* comp, const bool* init)
{
    double* arr = nullptr;
    std::int32_t kind = 0;
    std::int32_t width = 0;
    *vmin = 0.0;
    *vmax = 0.0;
    if (obj->owner == nullptr) return;
    DB_STEP_DATA* m = obj->owner;
    void* nset = obj->s8a0;
    void* eset = obj->s9e0;
    switch (*code) {
    case 800:
        arr = node_var_01(nset);
        kind = 3;
        width = sv_dim(m, 0x15fc);
        break;
    case 0x136:
        arr = node_var_1b(nset);
        kind = 1;
        width = sv_dim(m, 0x1664);
        break;
    case 100:
        arr = ext::fun_1401840d0(eset);
        kind = 2;
        width = sv_dim(m, 0x16ac);
        break;
    case 0x6e:
        arr = ext::fun_1401812a0(nset);
        kind = 1;
        width = sv_dim(m, 0x1668);
        break;
    case 200:
        arr = ext::fun_140184050(eset);
        kind = 2;
        width = sv_dim(m, 0x16a8);
        break;
    case 300:
        arr = elem_var_2b(eset);
        kind = 2;
        width = sv_dim(m, 0x16a4);
        break;
    case 400:
        arr = elem_var_2a(eset);
        kind = 2;
        width = sv_dim(m, 0x16a0);
        break;
    case 500:
        arr = elem_var_29(eset);
        kind = 2;
        width = sv_dim(m, 0x169c);
        break;
    case 600:
        arr = node_var_08(nset);
        kind = 1;
        width = sv_dim(m, 0x1618);
        break;
    case 700:
        arr = node_var_02(nset);
        kind = 3;
        width = sv_dim(m, 0x1600);
        break;
    case 0xaf0:
        arr = elem_var_38(eset);
        kind = 2;
        width = sv_dim(m, 0x16d8);
        break;
    case 900:
        arr = ext::fun_1401815b0(nset);
        kind = 1;
        width = ext::fun_1401cbbc0(obj->owner);
        break;
    case 1000:
        arr = ext::fun_140184a30(eset);
        kind = 2;
        width = ext::fun_1401cbc80(obj->owner);
        break;
    case 0x640:
        if (def::rt::dimension_is_2(obj->owner)) {
            const bool mark = true;
            arr = ext::fun_140184150(eset, &mark);
            kind = 2;
            width = sv_dim(m, 0x16b0);
        }
        break;
    case 0xa28:
        arr = node_var_0d(nset);
        kind = 1;
        width = 1;
        break;
    case 5000:
        arr = node_var_0f(nset);
        kind = 1;
        width = 1;
        break;
    case 0x157c:
        arr = ext::fun_1401814b0(nset);
        kind = 1;
        width = sv_dim(m, 0x1680);
        break;
    case 0x15f4:
        arr = ext::fun_140181530(nset);
        kind = 1;
        width = sv_dim(m, 0x1684);
        break;
    default:
        break;
    }
    if (arr == nullptr) {
        *vmin = 0.0;
        *vmax = 0.0;
        return;
    }
    scan(obj, arr, kind, width, vmin, vmax, comp, init);
}

// @src 14020d320
void obj_stress_min_max(LLOBJ_DATA* obj, const std::int32_t* code, const std::int32_t* kind,
                        double* vmin, double* vmax, const std::int32_t* comp, const bool* init)
{
    double* arr = nullptr;
    std::int32_t mode = 0;
    std::int32_t width = 0;
    std::int32_t rows = 0;
    double* full = nullptr;
    double* mag = nullptr;
    *vmin = 0.0;
    *vmax = 0.0;
    if (obj->owner == nullptr) return;
    DB_STEP_DATA* m = obj->owner;
    if (*code == 500) {
        bool known = true;
        if (*kind == 2 || *kind == 4 || *kind == 5) {
            mode = 4;
        } else if (*kind == 1) {
            mode = 2;
        } else if (*kind == 0) {
            mode = 2;
        } else {
            known = false;
        }
        if (known) {
            width = sv_dim(m, 0x1690);
            full = alloc_sv_array(obj, &mode, code, &at<std::int32_t>(m, 0x15f8), &rows);
            mag = alloc_sv_mag_array(obj, code, kind);
            std::int32_t offset = 0;
            std::int32_t c500 = 500;
            double* src = elem_var_26(obj->s9e0);
            sv_derive(obj, &c500, src, full, kind, &width, &offset, mag, nullptr);
            if (*kind == 2 || *kind == 4 || *kind == 5) {
                arr = mag;
            } else if (*kind == 1) {
                arr = full;
            } else if (*kind == 0) {
                arr = elem_var_29(obj->s9e0);
                for (std::int32_t i = 0; i < rows; i = i + 1) arr[i] = full[i];
            }
            mode = 2;
            width = sv_dim(m, 0x169c);
        }
    }
    if (arr == nullptr) {
        *vmin = 0.0;
        *vmax = 0.0;
        return;
    }
    scan(obj, arr, mode, width, vmin, vmax, comp, init);
    if (full != nullptr) std::free(full);
    if (mag != nullptr) std::free(mag);
}

// @src 14020f220  "LLOBJ_AllocSVArray"
double* alloc_sv_array(LLOBJ_DATA* obj, const std::int32_t* mode, const std::int32_t* code,
                       const std::int32_t* dims, std::int32_t* rows)
{
    std::int32_t n = 0; // uninitialised in the original when *mode is unknown
    std::int32_t md = *mode;
    std::int32_t nc;
    if (!obj_kind_is_2(obj)) {
        nc = 3;
    } else {
        nc = 2;
    }
    if (obj_flag_3612(obj)) {
        if (*mode == 2) {
            md = 1;
        } else if (*mode == 4) {
            md = 3;
        }
    }
    if (md == 1) {
        n = nodes(obj);
        *rows = nodes(obj);
    } else if (md == 2) {
        if (*code == 500) {
            n = elems(obj) * nc;
        } else {
            n = elems(obj);
        }
        *rows = elems(obj);
    } else if (md == 3) {
        if (*code == 700) {
            n = nodes(obj) * dims[2];
        } else if (*code == 800) {
            n = nodes(obj) * dims[1];
        } else if (*code == 0x2c24) {
            n = nodes(obj) * dims[4];
        } else if (*code == 0x1fe) {
            n = nodes(obj) * dims[0x1a];
        } else if (*code == 0x5dc) {
            n = nodes(obj) * dims[6];
        } else if (*code == 14000) {
            n = nodes(obj) * *dims;
        } else {
            n = nodes(obj) * nc;
        }
        *rows = nodes(obj);
    } else if (md == 4) {
        n = elems(obj) * nc;
        *rows = elems(obj);
    } else {
        // FUN_14047903c() + 0x30: the CRT's second FILE, stdout; str 140530928
        def::rt::crt::fprintf(stdout, "LLOBJ_AllocSVArray : nSVTypeMod %d unknown", md);
    }
    return ext::fun_140257150(n);
}

// @src 14020f4d0  "LLOBJ_DATA::AllocSVMagArray"
double* alloc_sv_mag_array(LLOBJ_DATA* obj, const std::int32_t* code, const std::int32_t* kind)
{
    if (!sv_kind_is_principal(*code, *kind)) return nullptr;
    std::int32_t n = 0; // uninitialised in the original after the error message
    const std::int32_t c = *code;
    switch (c) {
    case 700:
    case 800:
    case 0x1fe:
    case 0x5dc:
    case 0x2c24:
    case 14000:
        n = nodes(obj);
        break;
    case 300:
    case 400:
    case 500:
    case 3000:
    case 0xb54:
    case 0xc1c:
    case 0xc80:
    case 0xce4:
    case 0xd48:
    case 0x14b4:
    case 0x1518:
        n = elems(obj);
        break;
    default:
        // str 140530958
        def::rt::crt::fprintf(stdout, "Error in LLOBJ_DATA::AllocSVMagArray()");
        break;
    }
    if (obj_flag_3612(obj) && n != nodes(obj)) n = nodes(obj);
    return ext::fun_140257150(n);
}

// @src 140211d30
bool sv_kind_is_principal(std::int32_t code, std::int32_t kind)
{
    switch (code) {
    case 0x14b4:
    case 0x1518:
        return kind == 0;
    case 700:
    case 800:
    case 0x5dc:
    case 0x2c24:
    case 14000:
        return true;
    case 300:
    case 400:
    case 500:
    case 0x1fe:
    case 0xb54:
    case 3000:
    case 0xc1c:
    case 0xc80:
    case 0xce4:
    case 0xd48:
        return kind == 2 || kind == 3 || kind == 4 || kind == 5;
    default:
        return false;
    }
}

// @src 14020f6a0
void sv_derive(LLOBJ_DATA* obj, const std::int32_t* code, double* src, double* dst,
               const std::int32_t* kind, std::int32_t* width, std::int32_t* offset, double* aux,
               StateVarBase* sv)
{
    double* avg = nullptr;
    bool averaged = false;
    std::int32_t n0 = 0;
    std::int32_t n1 = 0;
    std::int32_t n2 = 0;
    std::int32_t id = 0;
    if (sv != nullptr) {
        id = sv->get_id();          // 140300ea0
        n0 = sv->dim0(false);       // 140300f00
        n1 = sv->dim1(false);       // 140300f60
        n2 = sv->dim2(false);       // 140300fc0
    }
    if (1 < n2) {
        double* buf = ext::fun_140257150(n0 * n1);
        StateVarBase* own = &obj->ub70;
        sv_set_id(own, id);
        own->vslot1_140301710(n0, 1, n2);
        const std::int32_t c = *code;
        if (c == 100 || c == 300) {
            for (std::int32_t e = 0; e < elems(obj); e = e + 1) {
                for (std::int32_t p = 0; p < n2; p = p + 1)
                    sv_set_value(own, e, p, 0, src[n1 * n2 * e + n1 * p]);
            }
        } else if (c == 500) {
            auto copy_point = [&](std::int32_t p) {
                for (std::int32_t e = 0; e < elems(obj); e = e + 1) {
                    for (std::int32_t k = 0; k < n1; k = k + 1)
                        buf[n1 * e + k] = src[n1 * p + k + n1 * n2 * e];
                }
            };
            if (*kind == 1) {
                for (std::int32_t p = 0; p < n2; p = p + 1) {
                    copy_point(p);
                    sv_derive_rows(obj, code, buf, dst, kind, width, offset, aux);
                    for (std::int32_t e = 0; e < elems(obj); e = e + 1) sv_set_value(own, e, p, 0, dst[e]);
                }
            } else if (*kind < 2 || 4 < *kind) {
                if (*kind == 5) {
                    for (std::int32_t p = 0; p < n2; p = p + 1) {
                        copy_point(p);
                        sv_derive_rows(obj, code, buf, dst, kind, width, offset, aux);
                        for (std::int32_t e = 0; e < elems(obj); e = e + 1) sv_set_value(own, e, p, 0, aux[e]);
                    }
                } else if (5 < *kind && *kind < 0xc) {
                    for (std::int32_t e = 0; e < elems(obj); e = e + 1) {
                        for (std::int32_t p = 0; p < n2; p = p + 1)
                            sv_set_value(own, e, p, 0, src[n1 * p + *kind + -6 + n1 * n2 * e]);
                    }
                }
            } else {
                for (std::int32_t p = 0; p < n2; p = p + 1) {
                    copy_point(p);
                    sv_derive_rows(obj, code, buf, dst, kind, width, offset, aux);
                    for (std::int32_t e = 0; e < elems(obj); e = e + 1) sv_set_value(own, e, p, 0, aux[e]);
                }
            }
        }
        if (buf != nullptr) std::free(buf);
        avg = ext::fun_140257150(n0 * n1);
        averaged = true;
        if (c == 100 || c == 300) {
            if (!def::rt::dimension_is_2(obj->owner)) {
                for (std::int32_t e = 0; e < elems(obj); e = e + 1) {
                    double s = 0.0;
                    for (std::int32_t p = 0; p < 8; p = p + 1) s = s + src[p + e * 8];
                    avg[e] = s / k_eight;
                }
            } else {
                for (std::int32_t e = 0; e < elems(obj); e = e + 1) {
                    double s = 0.0;
                    for (std::int32_t p = 0; p < 4; p = p + 1) s = s + src[p + e * 4];
                    avg[e] = s / k_four;
                }
            }
        } else if (c == 500) {
            if (!def::rt::dimension_is_2(obj->owner)) {
                for (std::int32_t e = 0; e < elems(obj); e = e + 1) {
                    double s[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
                    for (std::int32_t p = 0; p < 8; p = p + 1) {
                        for (std::int32_t k = 0; k < 6; k = k + 1) s[k] = s[k] + src[e * 0x30 + k + p * 6];
                    }
                    for (std::int32_t k = 0; k < 6; k = k + 1) avg[e * 6 + k] = s[k] / k_eight;
                }
            } else if (at<std::int32_t>(obj->owner, 0x850) == 3) {
                for (std::int32_t e = 0; e < elems(obj); e = e + 1) {
                    double s[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
                    for (std::int32_t p = 0; p < 4; p = p + 1) {
                        for (std::int32_t k = 0; k < 6; k = k + 1) s[k] = s[k] + src[e * 0x18 + k + p * 6];
                    }
                    for (std::int32_t k = 0; k < 6; k = k + 1) avg[e * 6 + k] = s[k] / k_four;
                }
            } else {
                for (std::int32_t e = 0; e < elems(obj); e = e + 1) {
                    double s[4] = {0.0, 0.0, 0.0, 0.0};
                    for (std::int32_t p = 0; p < 4; p = p + 1) {
                        for (std::int32_t k = 0; k < 4; k = k + 1) s[k] = s[k] + src[e * 0x10 + k + p * 4];
                    }
                    for (std::int32_t k = 0; k < 4; k = k + 1) avg[e * 4 + k] = s[k] / k_four;
                }
            }
        }
    }
    if (*code == 300 && *kind == 0) {
        if (averaged) {
            std::memcpy(dst, avg, static_cast<std::size_t>(static_cast<std::int64_t>(n0) << 3));
            if (avg != nullptr) std::free(avg);
        } else {
            std::memcpy(dst, src, static_cast<std::size_t>(static_cast<std::int64_t>(n0) << 3));
        }
    } else if (*code == 100) {
        if (averaged) {
            std::memcpy(dst, avg, static_cast<std::size_t>(static_cast<std::int64_t>(n0) << 3));
            if (avg != nullptr) std::free(avg);
        } else {
            std::memcpy(dst, src, static_cast<std::size_t>(static_cast<std::int64_t>(n0) << 3));
        }
    } else if (avg == nullptr) {
        sv_derive_rows(obj, code, src, dst, kind, width, offset, aux);
    } else {
        sv_derive_rows(obj, code, avg, dst, kind, width, offset, aux);
        if (avg != nullptr) std::free(avg);
    }
}

// @src 140210e90
void sv_derive_rows(LLOBJ_DATA* obj, const std::int32_t* code, double* src, double* dst,
                    const std::int32_t* kind, const std::int32_t* width,
                    const std::int32_t* offset, double* aux)
{
    std::int32_t pk = 1; // kind passed to the principal-value routines
    double eff = 0.0, pmax = 0.0, pmid = 0.0, pmin = 0.0;
    double p1 = 0.0, p2 = 0.0, ang = 0.0;
    double tmax = 0.0, tp1 = 0.0, tp2 = 0.0; // second 14032d1b0 call (kind 5)
    std::int32_t plane = 0;
    (void)plane;
    if (def::rt::dimension_is_3(obj->owner)) {
        plane = obj->u87c[6]; // +0x894 (unused besides this load, as in the original)
    } else {
        plane = 4;
    }
    const std::int32_t c = *code;
    if (c < 0xbb9) {
        if (c != 3000 && c != 300 && c != 400) {
            if (c == 500 || c == 0x1fe || c == 0xb54) {
                pk = 1;
                if (*kind == 4) pk = 2;
            }
            goto rows;
        }
    } else if (c != 0xc1c && c != 0xc80 && c != 0xce4 && c != 0xd48) {
        goto rows;
    }
    pk = 0;
    if (*kind == 4) pk = 3;
rows:
    std::int32_t n = elems(obj);
    if (*code == 0x1fe) n = nodes(obj);
    if (*kind == 0) {
        for (std::int32_t i = 0; i < n; i = i + 1) {
            double* s = src + (*width * i + *offset);
            if (!def::rt::dimension_is_2(obj->owner)) {
                principal_3d(&pk, s, &eff, &pmax, &pmid, &pmin);
            } else if (at<std::int32_t>(obj->owner, 0x850) == 3) {
                principal_3d(&pk, s, &eff, &pmax, &pmid, &pmin);
            } else {
                principal_2d(&pk, s, &eff, &pmax, &p1, &p2, &ang);
            }
            dst[i] = static_cast<double>(static_cast<float>(eff));
        }
    } else if (*kind == 1) {
        for (std::int32_t i = 0; i < n; i = i + 1) {
            const double* s = src + (*width * i + *offset);
            dst[i] = static_cast<double>(static_cast<float>((s[0] + s[1] + s[2]) / k_three));
        }
    } else if (!sv_kind_is_principal(*code, *kind)) {
        if (5 < *kind) {
            for (std::int32_t i = 0; i < n; i = i + 1) dst[i] = src[*width * i + *offset + (*kind + -6)];
        }
    } else if (!def::rt::dimension_is_2(obj->owner)) {
        const std::int32_t stride = 3;
        for (std::int32_t i = 0; i < n; i = i + 1) {
            const double* s = src + (*width * i + *offset);
            float a[9];
            float d[4];
            float v[10];
            a[0] = static_cast<float>(s[0]);
            a[4] = static_cast<float>(s[1]);
            a[8] = static_cast<float>(s[2]);
            a[1] = static_cast<float>(s[3]);
            a[2] = static_cast<float>(s[5]);
            a[5] = static_cast<float>(s[4]);
            const std::int32_t cc = *code;
            if (cc == 300 || cc == 3000 || cc == 0xc1c || cc == 0xc80 || cc == 0xce4 ||
                cc == 0xd48 || cc == 400) {
                a[1] = static_cast<float>(static_cast<double>(a[1]) / k_two);
                a[2] = static_cast<float>(static_cast<double>(a[2]) / k_two);
                a[5] = static_cast<float>(static_cast<double>(a[5]) / k_two);
            }
            a[3] = a[1];
            a[6] = a[2];
            a[7] = a[5];
            const std::int32_t nrot = jacobi3(a, d, v);
            if (nrot < 0x32) {
                eigsrt3(d, v);
                std::int32_t k = 0; // uninitialised in the original for *kind == 5
                if (*kind == 2) {
                    k = 0;
                } else if (*kind == 3) {
                    k = 1;
                } else if (*kind == 4) {
                    k = 2;
                }
                if (*kind == 5) {
                    const float f = fabs_float(d[0] - d[2]);
                    aux[i] = static_cast<double>(f / k_two_f);
                    dst[i] = aux[i];
                } else {
                    dst[i * stride] = static_cast<double>(v[k] * d[k]);
                    dst[i * stride + 1] = static_cast<double>(v[k + 3] * d[k]);
                    dst[i * stride + 2] = static_cast<double>(v[k + 6] * d[k]);
                    aux[i] = static_cast<double>(d[k]);
                }
            }
        }
    } else if (at<std::int32_t>(obj->owner, 0x850) == 3) {
        for (std::int32_t i = 0; i < n; i = i + 1) {
            double* s = src + (*width * i + *offset);
            principal_3d(&pk, s, &eff, &pmax, &pmid, &pmin);
            const std::int32_t kk = *kind;
            if (kk == 2) {
                dst[i] = pmax;
            } else if (kk == 3) {
                dst[i] = pmid;
            } else if (kk == 4) {
                dst[i] = pmin;
            } else if (kk == 5) {
                dst[i] = std::fabs(pmax - pmin) / k_two; // 140484558
                if (aux != nullptr) aux[i] = dst[i];
            }
        }
    } else {
        for (std::int32_t i = 0; i < n; i = i + 1) {
            double* s = src + (*width * i + *offset);
            if (*kind == 5) {
                pk = 1;
                principal_2d(&pk, s, &eff, &pmax, &p1, &p2, &ang);
                pk = 2;
                principal_2d(&pk, s, &eff, &pmin, &tp1, &tp2, &tmax);
            } else {
                principal_2d(&pk, s, &eff, &pmax, &p1, &p2, &ang);
            }
            if (*kind == 2) {
                if (pmax == p1 || pmax == p2) {
                    dst[i * 2] = pmax * def::rt::libm::crt_cos(ang);
                    dst[i * 2 + 1] = pmax * def::rt::libm::crt_sin(ang);
                } else if (0.0 <= pmax) {
                    dst[i * 2] = k_big;
                    dst[i * 2 + 1] = k_big;
                } else {
                    dst[i * 2] = k_mbig;
                    dst[i * 2 + 1] = k_mbig;
                }
            } else if (*kind == 4) {
                if (pmax == p1 || pmax == p2) {
                    dst[i * 2] = pmax * def::rt::libm::crt_cos(ang + k_halfpi);
                    dst[i * 2 + 1] = pmax * def::rt::libm::crt_sin(ang + k_halfpi);
                } else if (0.0 <= pmax) {
                    dst[i * 2] = k_big;
                    dst[i * 2 + 1] = k_big;
                } else {
                    dst[i * 2] = k_mbig;
                    dst[i * 2 + 1] = k_mbig;
                }
            }
            if (*kind == 5) {
                aux[i] = std::fabs(pmax - pmin) / k_two;
                dst[i] = aux[i];
            } else {
                aux[i] = pmax;
            }
        }
    }
}

// @src 140211ef0
void obj_component_min_max(LLOBJ_DATA* obj, const std::int32_t* code, double* out)
{
    double* arr = nullptr;
    bool elem = false;
    std::int32_t width = 0;
    std::int32_t rows = 0;
    if (obj->owner == nullptr) return;
    DB_STEP_DATA* m = obj->owner;
    if (*code == 300) {
        arr = elem_var_28(obj->s9e0);
        elem = true;
        width = sv_dim(m, 0x1698);
    } else if (*code == 400) {
        arr = elem_var_27(obj->s9e0);
        elem = true;
        width = sv_dim(m, 0x1694);
    } else if (*code == 500) {
        arr = elem_var_26(obj->s9e0);
        elem = true;
        width = sv_dim(m, 0x1690);
    }
    if (arr == nullptr) return;
    if (elem) rows = elems(obj);
    const std::int32_t ncomp = width < 7 ? width : 6;
    std::int32_t off = 0;
    if (*code == 300) {
        const std::int32_t k2 = obj_kind_140220520(obj);
        std::int32_t c3000 = 3000;
        off = strain_block_offset(obj, &c3000) * k2 * 2;
        if (off < 0) off = 0;
    }
    double vmin = 0.0;
    double vmax = 0.0;
    if (elem) {
        for (std::int32_t k = 0; k < ncomp; k = k + 1) {
            for (std::int32_t e = 0; e < rows; e = e + 1) {
                const double v = arr[e * width + k + off];
                if (e == 0) {
                    vmin = v;
                    vmax = v;
                } else {
                    if (vmin > v) vmin = v;
                    if (v > vmax) vmax = v;
                }
            }
            out[k * 2] = vmin;
            out[k * 2 + 1] = vmax;
        }
    }
}

} // namespace def::model::p407
