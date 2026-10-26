// C++ driver side of the cube build: one cube per object of the model (packet p67-7, group
// "cube"; GENCTC_3d_search.md sec. 2.1, GENCTC.md sec. 5.2).
#include <defcmd/ctc3d/cube.hpp>
#include <defcmd/ctc3d/surface.hpp>

#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/geo_kw/surface_model.hpp>
#include <defmodel/layout/cls_1401f46e0.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/obj_geo3d.hpp>
#include <defmodel/step/db_step.hpp>
#include <defmodel/window/inter_object_ops.hpp>

namespace def::cmd::ctc3d {

namespace {

template <class T>
T& at(void* p, std::size_t off)
{
    return *reinterpret_cast<T*>(static_cast<std::byte*>(p) + off);
}

} // namespace

// @src 1401a7a30  some pair with direction 1 has slave +0x168 == obj+8, or direction 2 has +0x16c
bool fun_1401a7a30(def::model::DB_STEP_DATA* model, def::model::LLOBJ_DATA* obj)
{
    i32 slave = 0;
    for (i32 i = 1;; ++i) {
        if (def::model::pair_count(&model->inter_obj) < i) { // 1400015d0 on model+0x13c8
            return false;
        }
        def::model::Cls_1401f46e0* p = def::model::pair_nth(&model->inter_obj, i); // 1401fac90
        if (p == nullptr || p->u170 == 0) {
            continue;
        }
        if (p->u170 == 1) {
            slave = p->u168;
        } else if (p->u170 == 2) {
            slave = p->u16c;
        } else {
            continue;
        }
        if (at<i32>(obj, 8) == slave) {
            return true;
        }
    }
}

// @src 1401a7980  FE boundary surface (true) when the object is a slave with a mesh, or is
// non-rigid (+0xc != 0) with a mesh (+0x87c), or rigid (+0xc == 0) without a rigid surface
bool fun_1401a7980(def::model::DB_STEP_DATA* model, def::model::LLOBJ_DATA* obj)
{
    if (obj == nullptr) {
        return false;
    }
    if (fun_1401a7a30(model, obj)) {
        return at<i32>(obj, 0x87c) != 0;
    }
    if (at<i32>(obj, 0xc) == 0) {
        return !def::model::geo_kw::has_surface_flag(*obj); // 140160d90
    }
    return at<i32>(obj, 0x87c) != 0;
}

// @src 1401a7710  free the old cubes, a table of 3*nobj records, then 14004f56b for every object
// slot with a mesh or a rigid surface (instance 1, 4 nodes per polygon)
void fun_1401a7710(def::model::DB_STEP_DATA* model)
{
    i32 one_inst = 1;  // local_38
    i32 one_base = 1;  // local_44
    i32 zero = 0;      // local_40 (cube verbose flag)
    i32 flag = 0;      // local_34
    i32 four = 4;      // local_48
    i32 n = at<i32>(model, 0x1378) * 3;
    fun_140225820(&zero);
    fun_140225640(&n, &zero);
    for (i32 k = 0; k < at<i32>(model, 0x1378); ++k) {
        auto* obj = static_cast<def::model::LLOBJ_DATA*>(def::model::step_object_at(*model, k + 1)); // 1401d6ed0
        if (obj == nullptr) {
            continue;
        }
        if (!def::model::geo_kw::has_surface_flag(*obj) && at<i32>(obj, 0x87c) == 0) {
            continue;
        }
        if (!fun_1401a7980(model, obj)) {
            // rigid surface 0 (140198620): counts 1400034d0 / 140003500, arrays 140005510 /
            // 140006410, in the binary's call order
            i32 np = def::model::geo_kw::surface(*obj, 0)->node_count();
            i32 nf = def::model::geo_kw::surface(*obj, 0)->elem_count();
            i32* faces = def::model::geo_kw::geo3d_elems(def::model::geo_kw::surface(*obj, 0));
            double* x = def::model::geo_kw::geo3d_coords(def::model::geo_kw::surface(*obj, 0));
            fun_14004f56b(&at<i32>(obj, 8), &one_inst, x, faces, &four, &one_base, &np, &nf, &zero, &flag);
        } else {
            fun_14004f56b(&at<i32>(obj, 8), &one_inst, at<double*>(obj, 0x8a8), at<i32*>(obj, 0xbf8), &four, &one_base,
                          &at<i32>(obj, 0x880), &at<i32>(obj, 0xbf0), &zero, &flag);
        }
    }
}

} // namespace def::cmd::ctc3d
