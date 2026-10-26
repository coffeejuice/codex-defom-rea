// GENDB check 5, inter-object data (packet p4-04): 1401fa310 and the per-relation checks.
// Cls_1401f46e0 fields used (decompile offsets): +0x000 friction kind (4 = two definitions),
// +0x004/+0x005 user-routine flags, +0x008/+0x00c user routine numbers, +0x010/+0x014 friction
// types, +0x018/+0x090 friction curves, +0x108.. constants per type (u108[]), +0x168/+0x16c the
// two objects, +0x170 relation defined, +0x188 contact windows, +0x1b8 IHTCOF, +0x2b8 INTRST.
#include <cstdio>
#include <cstring>

#include <defdbio/gendb/checks_p4_04.hpp>
#include <defmodel/dynary_init.hpp>
#include <defmodel/step/db_step.hpp>
#include <defrt/stdlib/legacy_printf.hpp>
#include <defrt/stdlib/refcount.hpp>

#include "ext_p4_04.hpp"

namespace def::dbio::gendb {

namespace {
using def::model::field;
FILE* out() { return stdout; } // __iob_func() + 0x30
double u108(const Cls_1401f46e0& r, std::size_t off) { return field<double>(&r, off); }

// part of @src 1401f5100: one friction definition (type, user flag, routine number, curve, and
// the constants used by types 6, 7 and 8)
void check_one_friction(Cls_1401f46e0& rec, const char* name, std::uint8_t user, std::int32_t type,
                        std::int32_t routine, const DYNARY* curve, std::size_t c6a, std::size_t c6b,
                        std::size_t c7, std::size_t c8, std::int32_t* code, std::int32_t* warn)
{
    const auto zero = [&] {
        *warn = 1;
        def::rt::crt::fprintf(out(), "WARNING: %s between Objects (%6d %6d) is ZERO\n", name, rec.u168, rec.u16c);
        message_hook_14001cda0();
    };
    if (user == 0) {
        if (-1 < type) {
            if (type < 6) {
                if (!def::model::dynary_counts_positive(curve) || dynary_all_zero(curve)) zero();
            } else if (type == 6) {
                if (u108(rec, c6a) == 0.0 || u108(rec, c6b) == 0.0) zero();
            } else if (type == 7) {
                if (u108(rec, c7) == 0.0) zero();
            } else if (type == 8 && u108(rec, c8) == 0.0) {
                zero();
            }
        }
    } else if (routine == 0) {
        *code = 7;
        *warn = 1;
        def::rt::crt::fprintf(out(), "WARNING: %s User Routine no. between Objects (%6d %6d) is not assigned\n", name,
                              rec.u168, rec.u16c);
        message_hook_14001cda0();
    }
}
} // namespace

// @src 14001cda0  a bare `ret` in this build
void message_hook_14001cda0() {}

// @src 14012fe90
bool dynary_values_zero(const DYNARY* t)
{
    if (t->ndim == 0) {
        if (t->value != 0.0) return false;
    } else {
        const std::int32_t n = def::model::dynary_value_count(t);
        for (std::int32_t i = 0; i < n; ++i) {
            if (t->values[i] != 0.0) return false;
        }
    }
    return true;
}

// @src 14012fd80
bool dynary_all_zero(const DYNARY* t)
{
    if (t->ndim == 0) {
        if (t->value != 0.0) return false;
    } else {
        for (std::int32_t d = 0; d < t->ndim; ++d) {
            for (std::int32_t i = 0; i < t->n[d]; ++i) {
                if (t->axis[d][i] != 0.0) return false;
            }
        }
        const std::int32_t n = def::model::dynary_value_count(t);
        for (std::int32_t i = 0; i < n; ++i) {
            if (t->values[i] != 0.0) return false;
        }
    }
    return true;
}

// @src 1401f3cd0
bool sim_htmthd_is_1(const Cls_1401e7b10& sim) { return ext::fun_1401f3cb0(&sim) == 1; }

// @src 14025d220  LLWINDEF_DATA: +0x178 point count, +0x180 coordinates (double*), +0x210 kind,
// +0x888 owner (DB_STEP_DATA*)
bool window_valid(const void* w)
{
    bool r = true;
    const std::int32_t n = field<std::int32_t>(w, 0x178);
    const double* xy = field<double*>(w, 0x180);
    if (!def::rt::dimension_is_3(field<void*>(w, 0x888))) {
        if (n < 1) {
            r = false;
        } else {
            bool all_zero = true;
            for (std::int32_t i = 0; i < n * 2; ++i) {
                if (xy[i] != 0.0) {
                    all_zero = false;
                    break;
                }
            }
            if (all_zero) r = false;
        }
    } else {
        const std::int32_t kind = field<std::int32_t>(w, 0x210);
        r = 0 < kind;
        if (kind == 1) {
            if (n == 8) {
                bool all_zero = true;
                for (std::int32_t i = 0; i < n * 3; ++i) {
                    if (xy[i] != 0.0) {
                        all_zero = false;
                        break;
                    }
                }
                if (all_zero) r = false;
            } else {
                r = false;
            }
        }
    }
    return r;
}

// @src 1401f4fa0
void inter_obj_check_curve(Cls_1401f46e0& rec, const char* name, bool active, const DYNARY* curve,
                           std::int32_t, std::int32_t* warn)
{
    if (active && (!def::model::dynary_counts_positive(curve) || dynary_all_zero(curve))) {
        *warn = 1;
        def::rt::crt::fprintf(out(), "WARNING: %s between Objects (%6d %6d) is ZERO\n", name, rec.u168, rec.u16c);
        message_hook_14001cda0();
    }
}

// @src 1401f5060
void inter_obj_check_window(Cls_1401f46e0& rec, const char* name, const void* window, std::int32_t,
                            std::int32_t* err)
{
    if (!window_valid(window)) {
        *err = 1;
        def::rt::crt::fprintf(out(), "Error: %s between Objects (%6d %6d) is not valid\n", name, rec.u168, rec.u16c);
        message_hook_14001cda0();
    }
}

// @src 1401f5100  name "FRCFAC" (7-byte copy of str 14052f694); the message code is 1, or 7 for
// an unassigned user routine
void inter_obj_check_friction(Cls_1401f46e0& rec, bool active, std::int32_t* warn)
{
    char name[8] = {};
    std::memcpy(name, "FRCFAC", 7);
    std::int32_t code = 1;
    if (!active) {
        return;
    }
    check_one_friction(rec, name, rec.u000.u004, rec.u000.u010, rec.u000.u008, &rec.u000.u018, 0x108, 0x110, 0x118,
                       0x128, &code, warn);
    code = 1;
    if (rec.u000.u000 == 4) {
        check_one_friction(rec, name, rec.u000.u005, rec.u000.u014, rec.u000.u00c, &rec.u000.u090, 0x138, 0x140,
                           0x148, 0x158, &code, warn);
    }
}

// @src 1401f4ce0
void inter_obj_check_record(Cls_1401f46e0& rec, std::int32_t* count, std::int32_t* err, std::int32_t* warn)
{
    if (rec.u170 != 0) {
        *count = *count + 1;
        message_hook_14001cda0(); // (6, "CNTACT", rec + 0x168, rec + 0x16c)
        Cls_1401e7b10& sim = rec.owner->sim_ctrl;
        const bool deform = 0 < ext::fun_1401f3c30(&sim);
        inter_obj_check_friction(rec, deform, warn);
        const bool heat = 0 < ext::fun_1401f3c10(&sim);
        inter_obj_check_curve(rec, "IHTCOF", heat, &rec.u1b8, 2, warn);
        inter_obj_check_curve(rec, "INTRST", sim_htmthd_is_1(sim), &rec.u2b8, 3, warn);
        const std::int32_t nwin = ext::fun_1401f98d0(&rec);
        for (std::int32_t i = 0; i < nwin; ++i) {
            void* w = ext::fun_140269900(&rec.u188, i + 1);
            if (w != nullptr) {
                inter_obj_check_window(rec, "FRCWIN", w, 4, err);
            }
        }
    }
}

// @src 1401fa310
void inter_obj_check(Cls_1401fa020& list, std::int32_t* err, std::int32_t* warn)
{
    std::int32_t count = 0;
    DB_STEP_DATA* const owner = list.owner;
    const std::int32_t n = ext::fun_1401f4610(owner);
    for (std::int32_t i = 1; i <= n; ++i) {
        LLOBJ_DATA* a = ext::fun_1401d6ea0(*owner, i);
        if (a != nullptr && field<std::int32_t>(a, 0xc) != 7) {
            for (std::int32_t j = i; j <= n; ++j) {
                LLOBJ_DATA* b = ext::fun_1401d6ea0(*owner, j);
                if (b != nullptr && field<std::int32_t>(b, 0xc) != 7) {
                    Cls_1401f46e0* rec = ext::fun_1401facf0(&list, i, j);
                    if (rec != nullptr) {
                        inter_obj_check_record(*rec, &count, err, warn);
                    }
                }
            }
        }
    }
    if (1 < owner->u1378 && count == 0) {
        *warn = 1;
        def::rt::crt::fprintf(out(), "WARNING: No Inter-Object Relation Defined\n");
        message_hook_14001cda0(); // (5, "CNTACT", 0, 0)
    }
}

} // namespace def::dbio::gendb
