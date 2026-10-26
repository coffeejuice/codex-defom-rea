// p9-03b: state-variable accessors, the SV dispatcher 1401627e0 and small model helpers.
#define _CRT_SECURE_NO_WARNINGS
#include <cstring>

#include <defcmd/cnvt3d_sweep_b.hpp>
#include <defmodel/bc/node_arrays.hpp>
#include <defmodel/llobj/cls_140182880.hpp>
#include <defmodel/llobj/object_list.hpp>
#include <defmodel/llobj_ctor/llobj_ctor.hpp>
#include <defmodel/material/state_remap.hpp>
#include <defmodel/node_access.hpp>
#include <defmodel/statevar_kw/statevar_kw.hpp>
#include <defmodel/step/name_arrays.hpp>

#include "common.hpp"
#include "ext.hpp"

namespace def::cmd::cnvt3d_sweep {

namespace detail {

// @data 1405caa80  SV ids transferred by the sweeps (count 140161fd0)
const std::int32_t kSvIds[0x2e] = {
    0x01, 0x02, 0x03, 0x04, 0x06, 0x08, 0x0c, 0x09, 0x0a, 0x0b, 0x10, 0x0e, 0x11, 0x0d, 0x0f, 0x18,
    0x1a, 0x1b, 0x1c, 0x1d, 0x22, 0x23, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x30, 0x2d, 0x2e,
    0x3c, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3e, 0x3f};

double* coords(LLOBJ_DATA* o) { return model::fun_1401808d0(static_cast<model::Cls_14017ef20*>(node_set(o))); }

std::int32_t ipoints(LLOBJ_DATA* o, std::int32_t j)
{
    return static_cast<model::Cls_140182880*>(elem_set(o))->fun_140183c20(j);
}

// part of the accessor family 140180940..1401815b0 (node set, Cls_14017ef20)
double* node_sv(void* set, std::int32_t id, std::ptrdiff_t slot, bool mark)
{
    auto* s = static_cast<model::Cls_14017ef20*>(set);
    std::int32_t* dims = model::fun_14017f020(s);
    model::bc::node_set_alloc(set, at<std::int32_t>(set, 300), dims, id);
    if (mark && at<DB_STEP_DATA*>(set, 0x130) != nullptr) {
        const std::int32_t v = id;
        model::fun_1401d48d0(at<DB_STEP_DATA*>(set, 0x130), &v);
    }
    return at<double*>(set, slot);
}

// part of the accessor family 140183d50..1401849a0 (element set, Cls_140182880); the widths are
// 140182980: owner (+0xf0) + 0x15f8, or null without owner
double* elem_sv(void* set, std::int32_t id, std::ptrdiff_t slot, bool mark)
{
    DB_STEP_DATA* owner = at<DB_STEP_DATA*>(set, 0xf0);
    const std::int32_t* dims = owner == nullptr ? nullptr : owner->u15f8;
    model::bc::element_set_alloc(set, at<std::int32_t>(set, 0xe8), dims, at<std::int32_t>(set, 0xec), id);
    if (mark && owner != nullptr) {
        const std::int32_t v = id;
        model::fun_1401d48d0(owner, &v);
    }
    return at<double*>(set, slot);
}

} // namespace detail

using namespace detail;

// @src 140180ac0  node set: variable 4
double* fun_140180ac0(void* set) { return node_sv(set, 4, 0x28); }
// @src 140180b40  node set: variable 5
double* fun_140180b40(void* set) { return node_sv(set, 5, 0x30); }
// @src 140180bc0  node set: variable 6
double* fun_140180bc0(void* set) { return node_sv(set, 6, 0x38); }
// @src 140180d30  node set: variable 9 (NDHEAT)
double* fun_140180d30(void* set) { return node_sv(set, 9, 0x48); }
// @src 140180db0  node set: variable 0xa
double* fun_140180db0(void* set) { return node_sv(set, 0xa, 0x50); }
// @src 140180e30  node set: variable 0xb (NDFLUX)
double* fun_140180e30(void* set) { return node_sv(set, 0xb, 0x58); }
// 140180eb0: kw-g2sv's port (defmodel/statevar_kw dtmp_values), forwarded (p9/p10 integration)
double* fun_140180eb0(void* set) { return model::svkw::dtmp_values(set); }
// @src 140181010  node set: variable 0xe (CRBFLX)
double* fun_140181010(void* set) { return node_sv(set, 0xe, 0x70); }
// @src 140181110  node set: variable 0x10 (RHTFLX)
double* fun_140181110(void* set) { return node_sv(set, 0x10, 0x80); }
// @src 140181220  node set: variable 0x19
double* fun_140181220(void* set) { return node_sv(set, 0x19, 0x90); }
// 1401813a0: kw-g2sv's port (defmodel/statevar_kw stress_node_values), forwarded
double* fun_1401813a0(void* set) { return model::svkw::stress_node_values(set); }
// 1401815b0: kw-g2sv's port (defmodel/statevar_kw), forwarded
double* fun_1401815b0(void* set)
{
    return model::svkw::usrnod_values(set);
}
// 140181190: kw-g2sv's port (defmodel/statevar_kw), forwarded
double* fun_140181190(void* set, const char* mark)
{
    const bool b = *mark != 0;
    return model::svkw::nwear_values(set, &b);
}
// 1401812a0: kw-g2sv's port (defmodel/statevar_kw), forwarded
double* fun_1401812a0(void* set)
{
    return model::svkw::damage_node_values(set);
}
// 140181420: kw-g2sv's port (defmodel/statevar_kw), forwarded
double* fun_140181420(void* set, const char* skip)
{
    const bool b = *skip != 0;
    return model::svkw::grain2_values(set, &b);
}

// @src 140184270  element set: variable 0x30
double* fun_140184270(void* set) { return elem_sv(set, 0x30, 0x70); }
// @src 140184370  element set: variable 0x32 (VOLFS)
double* fun_140184370(void* set) { return elem_sv(set, 0x32, 0x80); }
// 140184400: kw-g2sv's port (defmodel/statevar_kw dvolf_values), forwarded
double* fun_140184400(void* set) { return model::svkw::dvolf_values(set); }
// 140184490: kw-g2sv's port (defmodel/statevar_kw ticf_values), forwarded
double* fun_140184490(void* set) { return model::svkw::ticf_values(set); }
// 140184760: kw-g2sv's port (defmodel/statevar_kw ylds_values), forwarded
double* fun_140184760(void* set) { return model::svkw::ylds_values(set); }
// @src 1401847f0  element set: variable 0x3a
double* fun_1401847f0(void* set) { return elem_sv(set, 0x3a, 0xc0); }
// @src 140184880  element set: variable 0x3b
double* fun_140184880(void* set) { return elem_sv(set, 0x3b, 0xc8); }
// @src 140184910  element set: variable 0x3e
double* fun_140184910(void* set) { return elem_sv(set, 0x3e, 0xd0); }
// 1401840d0: kw-g2sv's port (defmodel/statevar_kw), forwarded
double* fun_1401840d0(void* set)
{
    return model::svkw::damage_values(set);
}
// 140184150: kw-g2sv's port (defmodel/statevar_kw), forwarded
double* fun_140184150(void* set, const char* mark)
{
    const bool b = *mark != 0;
    return model::svkw::mataxi_values(set, &b);
}
// 140184520: kw-g2sv's port (defmodel/statevar_kw), forwarded
double* fun_140184520(void* set)
{
    return model::svkw::grain_values(set);
}
// 1401845b0: kw-g2sv's port (defmodel/statevar_kw), forwarded
double* fun_1401845b0(void* set)
{
    return model::svkw::sizesh_values(set);
}
// 140184640: kw-g2sv's port (defmodel/statevar_kw), forwarded
double* fun_140184640(void* set)
{
    return model::svkw::txtodf_values(set);
}
// 140184a30: kw-g2sv's port (defmodel/statevar_kw), forwarded
double* fun_140184a30(void* set)
{
    return model::svkw::usrelm_values(set);
}
// 1401849a0: kw-g2sv's port (defmodel/statevar_kw), forwarded
double* fun_1401849a0(void* set)
{
    return model::svkw::erecvl_values(set);
}

// @src 140161fd0  LLOBJ_DATA method: size of the SV id table
std::int32_t fun_140161fd0(const LLOBJ_DATA* /*obj*/) { return 0x2e; }

// @src 1401627e0  LLOBJ_DATA method: switch on id - 1. The accessors of other packets
// (140180940 1401809c0 140180c40 140180f90 140181090 140181320 140183d50..140184050 1401842f0
// 1401846d0, p4-07 / p4-03) follow the same pattern; they are reproduced through node_sv /
// elem_sv here (same calls, same slots) so this file does not depend on their ext wiring.
double* fun_1401627e0(LLOBJ_DATA* obj, std::int32_t id, bool* nodal)
{
    double* r = nullptr;
    void* ns = node_set(obj);
    void* es = elem_set(obj);
    DB_STEP_DATA* owner = obj->owner;  // +0x3a98
    switch (id - 1) {
    case 0: r = node_sv(ns, 1, 0x10); *nodal = true; break;          // 140180940
    case 1: r = node_sv(ns, 2, 0x18); *nodal = true; break;          // 1401809c0
    case 2: r = ext::g.fun_140180a40(ns); *nodal = true; break;      // 140180a40 (p8-08)
    case 3: r = fun_140180ac0(ns); *nodal = true; break;
    case 4: r = fun_140180b40(ns); *nodal = true; break;
    case 5: r = fun_140180bc0(ns); *nodal = true; break;
    case 7: r = node_sv(ns, 8, 0x40); *nodal = true; break;          // 140180c40
    case 8: r = fun_140180d30(ns); *nodal = true; break;
    case 9: r = fun_140180db0(ns); *nodal = true; break;
    case 10: r = fun_140180e30(ns); *nodal = true; break;
    case 0xb: r = fun_140180eb0(ns); *nodal = true; break;
    case 0xc: r = node_sv(ns, 0xd, 0x68); *nodal = true; break;      // 140180f90
    case 0xd: r = fun_140181010(ns); *nodal = true; break;
    case 0xe: r = node_sv(ns, 0xf, 0x78); *nodal = true; break;      // 140181090
    case 0xf: r = fun_140181110(ns); *nodal = true; break;
    case 0x10: r = fun_1401815b0(ns); *nodal = true; break;
    case 0x17: {
        const char mark = 1;
        r = fun_140181190(ns, &mark);
        *nodal = true;
        break;
    }
    case 0x18: r = fun_140181220(ns); *nodal = true; break;
    case 0x19:
        if (at<std::int32_t>(owner, 0x890) == 2) {
            r = fun_1401813a0(ns);
            *nodal = true;
        }
        break;
    case 0x1a:
        if (at<std::int32_t>(owner, 0x88c) == 2) {
            r = node_sv(ns, 0x1b, 0x98);  // 140181320
            *nodal = true;
        }
        break;
    case 0x1b:
        if (at<std::int32_t>(owner, 0x888) == 2) {
            r = fun_1401812a0(ns);
            *nodal = true;
        }
        break;
    case 0x1c:
        if (at<std::int32_t>(owner, 0x898) == 1) {
            const char skip = 0;
            r = fun_140181420(ns, &skip);
            *nodal = true;
        }
        break;
    case 0x25: r = elem_sv(es, 0x26, 0x20); *nodal = false; break;   // 140183d50
    case 0x26: r = elem_sv(es, 0x27, 0x28); *nodal = false; break;   // 140183dd0
    case 0x27: r = elem_sv(es, 0x28, 0x30); *nodal = false; break;   // 140183e50
    case 0x28: r = elem_sv(es, 0x29, 0x38); *nodal = false; break;   // 140183ed0
    case 0x29: r = elem_sv(es, 0x2a, 0x40); *nodal = false; break;   // 140183f50
    case 0x2a: r = elem_sv(es, 0x2b, 0x48); *nodal = false; break;   // 140183fd0
    case 0x2b: r = elem_sv(es, 0x2c, 0x50); *nodal = false; break;   // 140184050
    case 0x2c: r = fun_1401840d0(es); *nodal = false; break;
    case 0x2d: {
        const char mark = 1;
        r = fun_140184150(es, &mark);
        *nodal = false;
        break;
    }
    case 0x2f: r = fun_140184270(es); *nodal = false; break;
    case 0x30: r = elem_sv(es, 0x31, 0x78); *nodal = false; break;   // 1401842f0
    case 0x31: r = fun_140184370(es); *nodal = false; break;
    case 0x32: r = fun_140184400(es); *nodal = false; break;
    case 0x33: r = fun_140184490(es); *nodal = false; break;
    case 0x34: r = fun_140184520(es); *nodal = false; break;
    case 0x35: r = fun_1401845b0(es); *nodal = false; break;
    case 0x36: r = fun_140184640(es); *nodal = false; break;
    case 0x37: r = elem_sv(es, 0x38, 0xb0); *nodal = false; break;   // 1401846d0
    case 0x38: r = fun_140184760(es); *nodal = false; break;
    case 0x39: r = fun_1401847f0(es); *nodal = false; break;
    case 0x3a: r = fun_140184880(es); *nodal = false; break;
    case 0x3b: r = fun_140184a30(es); *nodal = false; break;
    case 0x3d: r = fun_140184910(es); *nodal = false; break;
    case 0x3e: r = fun_1401849a0(es); *nodal = false; break;
    default: break;
    }
    return r;
}

// @src 1401cbc20  DB_STEP_DATA method: node names; in 3D rebuilt first when out of date (never:
// 1401d2b70 always returns 1)
char** fun_1401cbc20(DB_STEP_DATA* m)
{
    if (m->dimension == 3) {  // 1400015f0
        if (!model::names_up_to_date(m) && at<std::int32_t>(m, 0x2724) == 0) model::fun_1401cf930(m);
    }
    return m->u33a0;
}

// @src 1401cbce0  DB_STEP_DATA method: element names (as 1401cbc20)
char** fun_1401cbce0(DB_STEP_DATA* m)
{
    if (m->dimension == 3) {  // 1400015f0
        if (!model::names_up_to_date(m) && at<std::int32_t>(m, 0x2764) == 0) model::fun_1401cfc30(m);
    }
    return m->u33b0;
}

// @src 1401cbd40  DB_STEP_DATA method
void fun_1401cbd40(DB_STEP_DATA* m, const std::int32_t* i, const char* s) { std::strncpy(m->u33a0[*i], s, 0x40); }

// @src 1401cbd90  DB_STEP_DATA method
void fun_1401cbd90(DB_STEP_DATA* m, const std::int32_t* i, const char* s) { std::strncpy(m->u33b0[*i], s, 0x40); }

// @src 1401d1660  DB_STEP_DATA method
void fun_1401d1660(DB_STEP_DATA* m, std::int32_t id)
{
    std::int32_t old[0x50];
    std::memcpy(old, m->u15f8, 0x13c);
    model::step_material_state_sizes(*m);  // 1401d0320
    if (0 < id) {
        LLOBJ_DATA* o = model::object_list_find(&m->objects, id);  // 1401d6ea0
        if (o != nullptr) model::llobj_ctor::remap_state(o, m->u15f8);  // 140199650(o, new, old)
    }
}

// @src 1401d72e0  DB_STEP_DATA method: STNOUT count i (sim control +0x11c)
std::int32_t fun_1401d72e0(DB_STEP_DATA* m, std::int32_t i)
{
    if (i < 0 || 6 < i) return 0;
    return at<std::int32_t>(m, 0x86c + static_cast<std::ptrdiff_t>(i) * 4);
}

} // namespace def::cmd::cnvt3d_sweep
