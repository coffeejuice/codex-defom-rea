// p8-02: the GFREAD 3D table FUN_140243a70 (STL, GEO, PDA/PAT, UNV, NASTRAN), the loader
// callback, the scaling pass and the GetTickCount timers of the surface checker.
// Spec: docs/re/commands/GFREAD_STL_GEO_NAS.md sections 2, 4; GFREAD.md section 3.
#include <cmath>
#include <new>

#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

#include <defmodel/geo_kw/boundary.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/obj_geo2d.hpp>
#include <defmodel/layout/obj_geo3d.hpp>
#include <defrt/stdlib/refcount.hpp>

#include "ext_p67.hpp"
#include "ext_p8_09.hpp"
#include "stl_nas_internal.hpp"

namespace def::cmd::stlnas {

namespace {
// FUN_140158f50(obj) + 0x15f8: the model's per-variable component table
const i32* component_table(LLOBJ_DATA* obj)
{
    return obj->get_owner()->u15f8;
}
}  // namespace

// @src 140243a70
bool fun_140243a70(const void* loader_p, const char* file, LLOBJ_DATA* obj, i32 type)
{
    const LoaderView* loader = static_cast<const LoaderView*>(loader_p);
    bool result = true;  // local_178
    if (type == 0 || type == 1) return result;
    if (type == 2) {
        alignas(8) unsigned char geo[0x40];  // local_168, _DEF_AMGGEO
        ext::fun_140246d00(geo);
        char close = 0;  // local_20
        result = ext::fun_140246f70(geo, file, &close, 3, nullptr);
        if (result) {
            result = ext::fun_14024bac0(geo, obj, 3);
            ext::fun_1402059a0(obj, 0);
        }
        ext::fun_140246d40(geo);
    } else if (type == 3) {
        StlObj stl;  // local_128
        fun_140244ea0(&stl);
        char check = 1;                                     // local_1f
        const float tol = static_cast<float>(loader->tol);  // local_1c
        result = fun_140244fa0(&stl, file, &tol, &check);
        if (result) {
            result = fun_140246ad0(&stl, obj);
            // _DAT_14051bed8 = 0.0, DAT_140509c90 = 1.0, DAT_14051f1b8 = 1.0E-6
            if (result && 0.0 < loader->scale && 1.0E-6 < std::fabs(loader->scale - 1.0)) {
                fun_1401fffa0(obj, &loader->scale);
            }
        }
        fun_140244f00(&stl);
    } else if (type == 4 || type == 5) {
        ScratchObj p;  // local_b8 (PDA) / local_88 (UNV)
        const bool pda = type == 4;
        if (pda) {
            fun_14024c170(&p);
            result = fun_14024c290(&p, file, &loader->scale);
        } else {
            fun_14024d790(&p);
            result = fun_14024d8b0(&p, file, &loader->scale);
        }
        if (result) {
            const i32 mode = pda ? fun_14024c270(&p) : fun_14024d890(&p);
            if (mode == 2) {
                if (!fun_140244e60(loader, 1)) {
                    result = false;
                } else {
                    const i32* comp = component_table(obj);
                    result = pda ? fun_14024d650(&p, obj, comp) : fun_14024ef70(&p, obj, comp);
                    if (result) {
                        def::model::geo_kw::build_boundary(*obj);  // 1401fc290
                        char compact = 1;  // local_18 / local_17
                        ext::fun_1401fc730(obj, &compact);
                    }
                }
            } else {
                result = pda ? fun_14024d4f0(&p, obj) : fun_14024ee10(&p, obj);
            }
        }
        if (pda) {
            fun_14024c1d0(&p);
        } else {
            fun_14024d7f0(&p);
        }
    } else if (type == 10) {
        alignas(NasObj) unsigned char raw[sizeof(NasObj)];  // local_58
        NasObj* nas = fun_14024f0a0(reinterpret_cast<NasObj*>(raw));
        result = fun_14024f1d0(nas, file);
        if (result) {
            if (fun_14024f1b0(nas) == 2) {
                if (!fun_140244e60(loader, 1)) {
                    result = false;
                } else {
                    result = fun_140251d60(nas, obj, component_table(obj));
                    if (result) {
                        def::model::geo_kw::build_boundary(*obj);  // 1401fc290
                        char compact = 1;  // local_16
                        ext::fun_1401fc730(obj, &compact);
                    }
                }
            } else {
                (void)fun_140251be0(nas, obj);  // result ignored
                result = fun_140251d60(nas, obj, component_table(obj));
            }
        }
        fun_14024f110(nas);
    }
    return result;
}

// @src 140244e60
bool fun_140244e60(const LoaderView* loader, i32 arg)
{
    if (loader->callback == nullptr) return true;
    return loader->callback(arg) != 0;
}

// @src 1401fffa0
void fun_1401fffa0(LLOBJ_DATA* obj, const double* factor)
{
    namespace gk = def::model::geo_kw;
    if (!def::rt::dimension_is_3(obj->owner)) {  // 1400015f0 on obj+0x3a98
        if (gk::geometry_has_edges(*obj)) {     // 140160d20
            for (i32 i = 0; i < gk::curve_count(*obj); i = i + 1) {  // 140198310
                gk::curve(*obj, i)->scale(factor);                    // 1401982c0, slot 6
            }
        }
    } else if (gk::has_surface_flag(*obj)) {  // 140160d90
        for (i32 i = 0; i < gk::surface_count(*obj); i = i + 1) {  // 1401985a0
            gk::surface(*obj, i)->scale(factor);                    // 140198620, slot 6
        }
    }
}

// @src 140235710
void fun_140235710(float* t)
{
    *t = static_cast<float>(GetTickCount());
}

// @src 140222b60
float fun_140222b60()
{
    float t[6];  // local_18
    fun_140235710(t);
    return t[0];
}

// @src 140222b80
float fun_140222b80()
{
    float t[6];
    fun_140235710(t);
    return t[0];
}

}  // namespace def::cmd::stlnas
