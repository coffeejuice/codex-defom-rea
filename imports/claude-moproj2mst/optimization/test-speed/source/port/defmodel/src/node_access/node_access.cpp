// defmodel/node_access/node_access.cpp: allocate-on-access accessors of the node set
// Cls_14017ef20 and the model "layout sync" 1401d48d0 (defmodel/node_access.hpp).
// Decompiles: deform-rea-work/out/p673/extra.c (1401808d0, 14017f020, 14017ffe0, 1401d48d0),
// out/p673/e/acc.c (1401809c0, 1401d2b70, 1401d4400); listings out/p673/e/acc_L.txt.
#include <defmodel/node_access.hpp>
#include <defmodel/statevar_kw/statevar_kw.hpp>

#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/step/name_arrays.hpp>

namespace def::model {

// 14017f020: ported by kw-g2sv (defmodel/statevar_kw/statevar_kw.hpp svkw::set8a0_dims)
std::int32_t* fun_14017f020(Cls_14017ef20* set)
{
    return svkw::set8a0_dims(set);
}

// @src 1401808d0  node coordinates: allocate when missing (key 0), sync the model, return set+8
double* fun_1401808d0(Cls_14017ef20* set)
{
    std::int32_t* dims = fun_14017f020(set);
    fun_14017ffe0(set, set->n, dims, 0);
    if (set->u130 != nullptr) {
        std::int32_t which = 0;  // 14018090d
        fun_1401d48d0(static_cast<DB_STEP_DATA*>(set->u130), &which);
    }
    return set->u008;
}

// @src 1401809c0  state variable id 2: resize it (key 2), sync the model, return set+0x18
double* fun_1401809c0(Cls_14017ef20* set)
{
    std::int32_t* dims = fun_14017f020(set);
    fun_14017ffe0(set, set->n, dims, 2);
    if (set->u130 != nullptr) {
        std::int32_t which = 2;  // 140180a00
        fun_1401d48d0(static_cast<DB_STEP_DATA*>(set->u130), &which);
    }
    return static_cast<double*>(set->u010[1]);
}

// @src 1401d48d0  re-read the object arrays for code *which (-1: all) from the DB when the
// model says they are out of date (1401d2b70); 1401d2b70 always returns 1, so this returns at
// once (1401d48fb..1401d48fd jump to the epilogue 1401d5608). The dispatch below is the dead
// branch, kept in the decompile's order. 1401d48e1 stores -1 as a fifth stack argument of the
// first four 1401d4400 calls; 1401d4400 takes three, the value is unused.
void fun_1401d48d0(DB_STEP_DATA* model, const std::int32_t* which)
{
    if (names_up_to_date(model)) return;  // 1401d2b70

    const std::int32_t w = *which;
    auto hit = [w](std::int32_t code) { return w == code || w == -1; };
    auto read = [model](std::int32_t id, char flag) { fun_1401d4400(model, &id, &flag); };

    if (hit(0)) read(0x0b, 1);
    if (hit(1)) read(0x0c, 1);
    if (hit(2)) read(0x0d, 1);
    if (hit(3)) read(0x0f, 1);
    if (hit(4)) read(0x10, 1);
    if (hit(5)) read(0x11, 1);
    if (hit(6)) read(0x12, 1);
    if (hit(8)) read(0x14, 1);
    if (hit(9)) read(0x15, 1);
    if (hit(10)) read(0x16, 1);
    if (w == 0x0b || w == 0x0e || w == 0x10 || w == -1) read(0x17, 1);
    if (hit(0x0c)) read(0x37, 1);
    if (hit(0x0d)) read(0x1a, 1);
    if (hit(0x0f)) read(0x19, 1);
    if (hit(0x11)) fun_1401cf930(model);
    if (hit(0x18)) read(0x1b, 1);
    if (hit(0x19)) read(0x13, 1);
    if (hit(0x1a)) read(0x1d, 1);
    if (hit(0x1b)) read(0x22, 1);
    if (hit(0x1c)) read(0x26, 1);
    if (hit(0x1d)) read(0x2e, 1);
    if (hit(0x26)) read(0x1d, 0);
    if (hit(0x27)) read(0x1e, 0);
    if (hit(0x28)) read(0x1f, 0);
    if (hit(0x29)) read(0x20, 0);
    if (hit(0x2a)) read(0x21, 0);
    if (hit(0x2b)) read(0x22, 0);
    if (hit(0x2c)) read(0x23, 0);
    if (hit(0x2d)) read(0x26, 0);
    if (hit(0x2e)) read(0x1c, 0);
    if (hit(0x30)) read(0x25, 0);
    if (hit(0x31)) read(0x29, 0);
    if (hit(0x32)) read(0x2b, 0);
    if (hit(0x33)) read(0x2a, 0);
    if (hit(0x34)) read(0x2d, 0);
    if (hit(0x35)) read(0x2e, 0);
    if (hit(0x38)) read(0x2c, 0);
    if (hit(0x39)) read(0x31, 0);
    if (hit(0x3a)) read(0x32, 0);
    if (hit(0x3c)) fun_1401cfc30(model);
    // 1401cc180: the step version (model+0x1b64)
    if (10999 < model->step_version) {
        if (hit(0x22)) read(0x3a, 1);
        if (hit(0x23)) read(0x3d, 1);
        if (hit(0x36)) read(0x2f, 0);
        if (hit(0x3b)) read(0x38, 0);
        if (hit(0x37)) read(0x39, 0);
        if (hit(0x3f)) read(0x3b, 0);
        if (hit(0x3e)) read(0x3c, 0);
    }
}

}  // namespace def::model
