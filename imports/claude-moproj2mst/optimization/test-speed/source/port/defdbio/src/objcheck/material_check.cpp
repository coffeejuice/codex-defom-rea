// defdbio/src/objcheck/material_check.cpp: material and inter-material checks of GENDB
// (14029b980 / 14033a450 and their helpers), packet p4-03.
#include <defdbio/objcheck/objcheck.hpp>

#include <cstdint>
#include <cstdio>

#include <defrt/stdlib/legacy_printf.hpp>

#include "ext.hpp"
#include "raw.hpp"

namespace def::dbio::objcheck {

using namespace raw;

// @src 14028f130  "%s of Material %6d Undefined" when defined == '?' (every caller passes '!')
void material_undefined(void* mat, const char* key, char defined, std::int32_t* err)
{
    if (defined == '?') {
        *err = 1;
        def::rt::crt::fprintf(stdout, "ERROR  : %s of Material %6d Undefined\n", key, i32(mat, 0));
        std::int32_t code = 0;
        std::int32_t zero = 0;
        ext::fun_14001cd70(&code, key, mat, &zero);
    }
}

// @src 14028f1c0  warning when the table has an empty axis or is all zero
void material_zero_warning(void* mat, const char* key, const void* table, std::int32_t code, std::int32_t* warn)
{
    if (ext::fun_14012f610(table) && !dynary_all_zero(table)) return;
    *warn = 1;
    def::rt::crt::fprintf(stdout, "WARNING: %s of Material %6d is ZERO\n", key, i32(mat, 0));
    std::int32_t zero = 0;
    ext::fun_14001cd70(&code, key, mat, &zero);
}

// @src 14028f260  the same as an error
void material_zero_error(void* mat, const char* key, const void* table, std::int32_t code, std::int32_t* err)
{
    if (ext::fun_14012f610(table) && !dynary_all_zero(table)) return;
    *err = 1;
    def::rt::crt::fprintf(stdout, "ERROR  : %s of Material %6d is ZERO\n", key, i32(mat, 0));
    std::int32_t zero = 0;
    ext::fun_14001cd70(&code, key, mat, &zero);
}

// @src 14028ed70  one material. Flags at +8: bit 0 plastic, bit 4 elastic, bit 8 thermal.
void check_material(void* mat, std::int32_t* err, std::int32_t* warn)
{
    std::int32_t c2 = 2;
    std::int32_t zero = 0;
    ext::fun_14001cd70(&c2, "MTNAM", mat, &zero);
    const std::uint32_t flags = static_cast<std::uint32_t>(i32(mat, 8));
    if ((flags & 1) != 0) {
        material_undefined(mat, "FSTRES", '!', err);
        material_undefined(mat, "FRCMOD", '!', err);
    }
    if ((flags & 0x10) != 0) {
        material_undefined(mat, "YOUNG", '!', err);
        material_zero_warning(mat, "YOUNG", add(mat, 0x670), 5, warn);
        material_undefined(mat, "POISON", '!', err);
        material_zero_warning(mat, "POISON", add(mat, 0x6e8), 6, warn);
        material_undefined(mat, "EXPAND", '!', err);
        material_zero_warning(mat, "EXPAND", add(mat, 0x760), 7, warn);
    }
    const double ref = f64(mat, 0x7d8);
    if (ref <= 1e-06 && 1e-06 != ref) {   // DAT_14051f1b8
        *warn = 1;
        def::rt::crt::fprintf(stdout, "WARNING: %s of Material %6d is ZERO\n", "EXPAND(Ref. T)", i32(mat, 0));
        std::int32_t code = 0xc;
        std::int32_t z = 0;
        ext::fun_14001cd70(&code, "EXPAND(Ref. T)", mat, &z);
    }
    if ((flags & 0x100) != 0) {
        material_undefined(mat, "THRCND", '!', err);
        if (i32(mat, 0x1ccc) == 0) material_zero_error(mat, "THRCND", add(mat, 0x7e0), 8, err);
        material_undefined(mat, "HEATCP", '!', err);
        if (i32(mat, 0x1ccc) == 0) material_zero_error(mat, "HEATCP", add(mat, 0x858), 9, err);
        material_undefined(mat, "EMSVTY", '!', err);
        material_zero_warning(mat, "EMSVTY", add(mat, 0x8d8), 10, warn);
    }
    void* step = ptr(mat, 0x2208);
    if (step != nullptr) {
        char* s = add(step, 0x854);   // simulation control +0x104
        if (0 < i32(step, 0x85c)) material_undefined(mat, "MSTMTR", '!', err);
        if (0 < i32(s, 0xc)) material_undefined(mat, "DIFCOE", '!', err);
        if (i32(s, 0x14) == 1) material_undefined(mat, "ELRST", '!', err);
        if (i32(s, 0x14) == 2) {
            material_undefined(mat, "PMEAB", '!', err);
            material_undefined(mat, "PMITT", '!', err);
        }
    }
}

// @src 14029b980  every material whose +4 is set (1401b0620)
void check_material_list(void* list, std::int32_t* err, std::int32_t* warn)
{
    for (std::int32_t i = 0; i < ext::fun_140199c50(list); ++i) {
        void* m = ext::fun_14029c260(list, i + 1);
        if (m != nullptr && ext::fun_1401b0620(m)) check_material(m, err, warn);
    }
}

// @src 140336250  "%s between Materials (%6d %6d) Undefined" when (mask & DB_STEP_DATA+0x13c) and
// defined == '?' (every caller passes '!')
void phase_pair_undefined(void* pair, const char* key, std::uint32_t mask, char defined, std::int32_t* err)
{
    std::uint32_t m = step_u13c(static_cast<const DB_STEP_DATA*>(ptr(pair, 0x778)));
    if ((mask & m) != 0 && defined == '?') {
        *err = 1;
        def::rt::crt::fprintf(stdout, "ERROR  : %s between Materials (%6d %6d) Undefined\n", key, i32(pair, 0),
                              i32(pair, 4));
        std::int32_t code = 0;
        ext::fun_14001cd80(&code, key, pair, add(pair, 4));
    }
}

// @src 140336130  one material pair: PHASTF, and when it is on (+8) the transformation data
void check_phase_pair(void* pair, std::int32_t* n_on, std::int32_t* err)
{
    phase_pair_undefined(pair, "PHASTF", 2, '!', err);
    if (i32(pair, 8) != 0) {
        std::int32_t code = 1;
        ext::fun_14001cd80(&code, "PHASTF", pair, add(pair, 4));
        *n_on = *n_on + 1;
        phase_pair_undefined(pair, "TTTD", 2, '!', err);   // DAT_14054ff30
        phase_pair_undefined(pair, "PHASLH", 2, '!', err);
        phase_pair_undefined(pair, "PHASVL", 2, '!', err);
        phase_pair_undefined(pair, "TRNSFP", 2, '!', err);
    }
}

// @src 14033a450  every pair whose two materials exist and are active; a warning when the model
// has phases (DB_STEP_DATA+0x85c > 0) but no pair is on
void check_phase_pair_list(void* list, std::int32_t* err, std::int32_t* warn)
{
    std::int32_t n_on = 0;
    void* step = ptr(list, 0x18);
    for (void* p = ptr(list, 0); p != nullptr; p = ptr(p, 0x770)) {
        void* a = ext::fun_1401d7060(step, i32(p, 0));
        void* b = ext::fun_1401d7060(step, i32(p, 4));
        if (a != nullptr && b != nullptr && ext::fun_1401b0620(a) && ext::fun_1401b0620(b))
            check_phase_pair(p, &n_on, err);
    }
    if (step != nullptr && 0 < i32(step, 0x85c) && n_on == 0) {
        *warn = 1;
        def::rt::crt::fprintf(stdout, "WARNING: No Inter-Material Relation Defined\n");
        std::int32_t code = 3;
        std::int32_t z1 = 0;
        std::int32_t z2 = 0;
        ext::fun_14001cd80(&code, "PHASTF", &z1, &z2);
    }
}

} // namespace def::dbio::objcheck
