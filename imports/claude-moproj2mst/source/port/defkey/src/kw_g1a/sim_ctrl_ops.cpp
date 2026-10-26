// kw-g1a: simulation-control and model accessors used by the group 1 reader and writer.
// See sim_ctrl_ops.hpp for why they live here.
#define _CRT_SECURE_NO_WARNINGS  // the original's plain CRT calls (strncpy, sscanf, fscanf)
#include "sim_ctrl_ops.hpp"

#include <cstring>
#include <emmintrin.h>

#include <defmodel/layout/llobj_data.hpp>

#include <defmodel/dynary_init.hpp>
#include <defmodel/linked/accessors.hpp>
#include <defmodel/llobj/object_list.hpp>
#include <defmodel/step/db_step.hpp>
#include <defrt/stdlib/refcount.hpp>

namespace def::key::g1a::ops {

// FUN_1400015e0: model is 2D (p1-01's def::rt::dimension_is_2)
bool is_2d(const DB_STEP_DATA& m) { return def::rt::dimension_is_2(&m); }

// FUN_1400015f0: model is 3D (p1-01's def::rt::dimension_is_3)
bool is_3d(const DB_STEP_DATA& m) { return def::rt::dimension_is_3(&m); }

std::int32_t trunc_i32(double v) { return _mm_cvttsd_si32(_mm_set_sd(v)); }

// @src 1401f3e90  FUN_1401f3e90
void set_trans0(Cls_1401e7b10& s, std::int32_t v) { s.trans[0] = v; }

// @src 1401f3eb0  FUN_1401f3eb0
void set_trans1(Cls_1401e7b10& s, std::int32_t v) { s.trans[1] = v; }

// @src 1401f3c10  FUN_1401f3c10
std::int32_t trans0(const Cls_1401e7b10& s) { return s.trans[0]; }

// @src 1401f3c30  FUN_1401f3c30
std::int32_t trans1(const Cls_1401e7b10& s) { return s.trans[1]; }

// @src 1401f3ff0  FUN_1401f3ff0: SMODE (S+0xf4) and, for 1..3, TRANS[0..1]
void set_smode(Cls_1401e7b10& s, std::int32_t mode)
{
    s.smode = mode;
    if (mode == 3) {
        set_trans1(s, 1);
        set_trans0(s, 1);
    } else if (mode == 2) {
        set_trans1(s, 0);
        set_trans0(s, 1);
    } else if (mode == 1) {
        set_trans1(s, 1);
        set_trans0(s, 0);
    }
}

// @src 1401f3f80  FUN_1401f3f80: SMODE = 3 if TRANS[0] and TRANS[1], else 1 if TRANS[0] == 0, else 2
void recompute_smode(Cls_1401e7b10& s)
{
    if (trans0(s) != 0 && trans1(s) != 0) {
        s.smode = 3;
        return;
    }
    if (trans0(s) == 0) {
        s.smode = 1;
    } else {
        s.smode = 2;
    }
}

// @src 1401ee580  FUN_1401ee580: S+0x240
std::int32_t time_flag(const Cls_1401e7b10& s) { return s.dttype.u58; }

// FUN_1401ee5a0 (p1-03's sim_ctrl_set_u240)
void set_time_flag(Cls_1401e7b10& s, const std::int32_t* v) { def::model::sim_ctrl_set_u240(s, v); }

// FUN_1401ee510 (p1-03's sim_ctrl_update_u240)
void recompute_time_flag(Cls_1401e7b10& s) { def::model::sim_ctrl_update_u240(s); }

// FUN_1401f4390 (p1-06's linked::set_dsmax)
void set_dsmax(Cls_1401e7b10& s, double v) { def::model::linked::set_dsmax(&s, v); }

// @src 14039d2b0  FUN_14039d2b0
void set_odmax(Cls_1401e7b10& s, double v) { s.odmax = v; }

// @src 1403d8490  FUN_1403d8490
double get_odmax(const Cls_1401e7b10& s) { return s.odmax; }

// @src 1401f4100  FUN_1401f4100: ENVTMP/CNVCOF/ENVATM/ACVCOF tables back to template 0, then the
// unit-dependent constants (S+0xfc == 1 selects the first set, anything else the second)
void reset_unit_defaults(Cls_1401e7b10& s)
{
    model::dynary_init(&s.envtmp, 0);
    model::dynary_init(&s.cnvcof, 0);
    model::dynary_init(&s.envatm, 0);
    model::dynary_init(&s.acvcof, 0);
    if (s.unit == 1) {
        s.unte2h = 1.0;  // _DAT_140509c90  UNTE2H
        s.blzman = 5.669e-11;  // DAT_14052e8b0   BLZMAN
        s.envtmp.value = 20.0;  // DAT_14051f1f0   ENVTMP
        s.cnvcof.value = 0.02;  // DAT_14052e8a8   CNVCOF
        s.envatm.value = 1.69;  // DAT_14052e8a0   ENVATM
        s.acvcof.value = 2.5e-4;  // DAT_14052e888   ACVCOF
        s.envmpr = 1.26e-9;  // DAT_14052e880   ENVMPR
        s.envmpt = 8.8e-15;  // DAT_14052e878   ENVMPT
    } else {
        s.unte2h = 0.107;  // DAT_14052e8c8
        s.blzman = 3.306e-15;  // DAT_14052e8c0
        s.envtmp.value = 68.0;  // DAT_14051f1f8
        s.cnvcof.value = 7.7e-6;  // DAT_14052e8b8
        s.envatm.value = 1.69;  // DAT_14052e8a0
        s.acvcof.value = 1e-5;  // DAT_140522f98
        s.envmpr = 3.19e-8;  // DAT_14052e898
        s.envmpt = 2.25e-13;  // DAT_14052e890
    }
}

// @src 1401d6ea0  FUN_1401d6ea0
LLOBJ_DATA* find_object(DB_STEP_DATA& m, std::int32_t id) { return find_in_list(m.objects, id); }

// FUN_14017aaf0 (kw-g4b's def::model::object_list_find)
LLOBJ_DATA* find_in_list(model::Cls_14017a6c0& list, std::int32_t id) { return def::model::object_list_find(&list, id); }

double object_stroke(const LLOBJ_DATA& obj)
{
    double v;
    std::memcpy(&v, obj.s2160 + (0x22e0 - 0x2160), 8);
    return v;
}

} // namespace def::key::g1a::ops
