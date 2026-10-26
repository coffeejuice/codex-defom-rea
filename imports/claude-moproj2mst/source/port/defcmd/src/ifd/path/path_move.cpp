// Path movement helpers of the interference "arbitrary path" branch: movement (Cls_14018b220 at
// LLOBJ_DATA+0x2160) queries, placing an object at a path time, re-placing every path object,
// and the simulation time get/set (Cls_1401e7b10 at DB_STEP_DATA+0x750).
// Spec: docs/re/commands/OBJPOS_interference.md sections 4.1, 8.2, 8.3.
#include <defcmd/ifd/path.hpp>

#include <defmodel/layout/cls_1401e7b10.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/dynary.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/step/db_step.hpp>
#include <defmodel/window/inter_object_ops.hpp>
#include <defrt/ftn/helpers.hpp>

#include <cstddef>
#include <defrt/rdata.hpp>

namespace def::cmd::ifd {

namespace {

template <class T>
T& at(void* p, std::size_t off)
{
    return *reinterpret_cast<T*>(static_cast<std::byte*>(p) + off);
}
template <class T>
const T& at(const void* p, std::size_t off)
{
    return *reinterpret_cast<const T*>(static_cast<const std::byte*>(p) + off);
}

// rdata 14051f260  1e-12
constexpr double kMoveEps = def::rt::rdata::d_14051f260;

} // namespace

// @src 1401f3ae0  Cls_1401e7b10 (simulation control): TNOW 1 (+0x640) when +0x650 == 0, else
// TNOW 2 (+0x648)
double sim_time(void* simctl)
{
    const auto* s = static_cast<const model::Cls_1401e7b10*>(simctl);
    if (s->tnow3 == 0) return s->tnow1;
    return s->tnow2;
}

// @src 1401f3b30  Cls_1401e7b10: set the time selected the same way
void set_sim_time(void* simctl, double* t)
{
    auto* s = static_cast<model::Cls_1401e7b10*>(simctl);
    if (s->tnow3 == 0) {
        s->tnow1 = *t;
    } else {
        s->tnow2 = *t;
    }
}

// @src 14019ed10  put every path-moving object (by list position 1..model+0x1378) at the current
// time
void fun_14019ed10(DB_STEP_DATA* m)
{
    double t = 0.0;
    t = sim_time(&m->sim_ctrl);
    for (std::int32_t k = 0; k < m->u1378; k = k + 1) {
        auto* obj = static_cast<LLOBJ_DATA*>(model::step_object_at(*m, k + 1)); // 1401d6ed0
        if (obj != nullptr && at<std::int32_t>(obj, 0xc) != 7 &&            // 140160f10
            path::movement_on_path(&obj->s2160)) {
            path::place_on_path(obj, &t);
        }
    }
}

namespace path {

// @src 14021e470  Cls_14018b220: type 6 (arbitrary path) with a non-zero path table
bool movement_on_path(const void* mv)
{
    if (at<std::int32_t>(mv, 0x68) == 6) {
        return !table_all_zero(&at<DYNARY>(mv, 0x448));
    }
    return false;
}

// @src 140194cb0  Cls_14018b220: current position (+0x630 for type 6, else +0x170)
double* movement_position(void* mv)
{
    if (at<std::int32_t>(mv, 0x68) == 6) return &at<double>(mv, 0x630);
    return &at<double>(mv, 0x170);
}

// @src 140194d40  Cls_14018b220: the path transform matrix
bool movement_path_matrix(const void* mv, double* m)
{
    m[0] = at<double>(mv, 0x4e0);
    m[3] = at<double>(mv, 0x4e8);
    m[6] = at<double>(mv, 0x4f0);
    m[1] = at<double>(mv, 0x4f8);
    m[4] = at<double>(mv, 0x500);
    m[7] = at<double>(mv, 0x508);
    m[2] = at<double>(mv, 0x4c8);
    m[5] = at<double>(mv, 0x4d0);
    m[8] = at<double>(mv, 0x4d8);
    return true;
}

// @src 14021ee40  Cls_14018b220: the path as a time curve. +0x458 is var[0] of the table at
// +0x448; +0x4c0 is the int right after it.
void movement_path_curve(const void* mv, DYNARY* out)
{
    model::dynary_copy(out, &at<DYNARY>(mv, 0x448)); // 14012eff0
    if (at<std::int32_t>(mv, 0x458) != 1) {
        path_speed_to_time(out);
    }
    if (at<std::int32_t>(mv, 0x4c0) == 2) {
        double m[11]; // local_58[88]
        movement_path_matrix(mv, m);
        path_transform(out, m);
    }
}

// @src 1402007d0  LLOBJ_DATA: place at a path time. The result of 1401351c0 is not tested (P
// stays 0 when it fails). 14021d0b0 gets &0 / &1 (bytes in the original).
bool place_on_path(LLOBJ_DATA* obj, const double* t)
{
    double p[3] = {0.0, 0.0, 0.0};
    double delta[3] = {0.0, 0.0, 0.0};
    if (at<std::int32_t>(obj, 0x21c8) != 6) return false;
    void* mv = &obj->s2160;
    if (!movement_on_path(mv)) return false;
    DYNARY curve;                       // 14012d050
    movement_path_curve(mv, &curve);
    path_at_time(&curve, t, p);
    rt::ftn::vec3_sub(p, movement_position(mv), delta); // 14023b1f0
    bool moved = false;
    if (rt::ftn::vec3_length(delta) > kMoveEps) {       // 14023b610
        char b = 1;
        std::int32_t a = 0;
        ext::translate(obj, delta, &a, &b);             // 14021d0b0
        moved = true;
    }
    const double* cur = movement_position(mv);
    at<double>(obj, 0x22d0) = cur[0];
    at<double>(obj, 0x22d8) = cur[1];
    if (model::model_is_3d(obj->owner)) {               // 1400015f0
        at<double>(obj, 0x22e0) = cur[2];
    }
    return moved;
    // curve destroyed here (14012f410)
}

} // namespace path

} // namespace def::cmd::ifd
