// Object tests the DEF_GUI_MO_64 3D viewer calls as byte-identical twins (header:
// defmodel/gui_twins_view.hpp; spec docs/re/moproj2mst/mdt/V/spec_V.md). Decompiles:
// deform-rea-work/gui_mo/s1/mdt/V/v3.c (GUI) and the DEF_PRE functions at the tags.
#include <defmodel/gui_twins_view.hpp>

#include <cstddef>
#include <cstdlib>
#include <cstring>

#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/obj_geo3d.hpp>
#include <defmodel/llobj/cls_14017ef20.hpp>
#include <defmodel/node_access.hpp>

namespace def::model {

namespace {

template <class T>
T& raw(void* p, std::size_t off)
{
    return *reinterpret_cast<T*>(static_cast<unsigned char*>(p) + off);
}

}  // namespace

// @src 140160060  mesh coordinates plausible, else "*Corrupt* " name prefix (GUI twin 0x1408340f0)
bool llobj_mesh_coords_plausible(LLOBJ_DATA* obj)
{
    std::int32_t i = 0;                                                  // local_88
    std::int32_t j = 0;                                                  // local_a0
    std::int32_t dim = 0;                                                // local_a4
    const std::int32_t* dims = &obj->get_owner()->u15f8[0];             // *(obj+0x3a98) + 0x15f8
    const double* x = fun_1401808d0(reinterpret_cast<Cls_14017ef20*>(obj->s8a0));   // obj+0x8a0
    double d = 0.0;                                                      // local_98
    bool ok = true;                                                      // local_a8
    dim = *dims;
    i = 0;
    for (;;) {
        if (raw<std::int32_t>(obj, 0x880) <= i || 9 < i) {
            if (!ok) {
                // The binary's frame: char local_78[64], int local_38, char local_30[16]. strncpy
                // leaves local_78[53..63] unwritten (uninitialised stack) for a name of 53 or more
                // characters, and strcat then reads into them; the port zeroes the frame, so such a
                // name is cut at 53 characters. Shorter names: strncpy pads with zeros, identical.
                struct {
                    char tmp[64];   // local_78
                    std::int32_t len;   // local_38
                    char pre[16];   // local_30
                } f{};
                std::memcpy(f.pre, "*Corrupt* ", 0xb);                   // str 1427a8098 (GUI)
                f.len = static_cast<std::int32_t>(std::strlen(f.pre));
                std::strncpy(f.tmp, obj->name, static_cast<std::size_t>(0x3f - f.len));
                std::strcpy(obj->name, f.pre);                           // 140484f90
                std::strcat(obj->name, f.tmp);                           // 140484ef0
            }
            return ok;
        }
        for (j = 0; j < dim; ++j) {
            d = x[i * dim + j];
            if (1e9 < d || d < -1e9) {                                   // DAT_140522f50, sign flip
                ok = false;
                break;
            }
        }
        ++i;
    }
}

// @src 1402fabc0  nodes per face of a surface, 0 / 3 / 4 (GUI twin 0x140929bd0)
std::int32_t geo3d_face_node_count(OBJ_GEO3D* s)
{
    std::int32_t n = s->elem_count();                                    // 140003500
    std::int32_t r = 0;
    if (n == 0) {
        r = 0;
    } else {
        const std::int32_t* p0 = &s->elems.mutable_at(0);                // 140005100
        const std::int32_t* p3 = &s->elems.mutable_at(3);
        const std::int32_t a3 = std::abs(*p3);                           // 140484ab8
        bool tri = *p0 == a3;
        if (1 < n && tri) {
            const std::int32_t* p4 = &s->elems.mutable_at(4);
            const std::int32_t* p7 = &s->elems.mutable_at(7);
            n = std::abs(*p7);
            if (*p4 != n) tri = false;
        }
        r = tri ? 3 : 4;
    }
    return r;
}

// @src 140160fe0  object kind 0 (GUI twin 0x140835070)
bool llobj_is_rigid(const LLOBJ_DATA* obj)
{
    return *reinterpret_cast<const std::int32_t*>(reinterpret_cast<const unsigned char*>(obj) + 0xc) == 0;
}

}  // namespace def::model
