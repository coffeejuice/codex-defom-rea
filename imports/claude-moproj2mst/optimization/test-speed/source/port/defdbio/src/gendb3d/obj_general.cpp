// gendb3d/obj_general.cpp: IIOBJ / LLOBJ general words of an object (packet p4-05).
// Word meanings: docs/re/dbmap/object.md (branch claude/project-thread-bz5k5e).
#include <defdbio/gendb3d/obj3d.hpp>

#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/llobj/cls_140182880.hpp>
#include <defrt/stdlib/refcount.hpp>

#include "gendb3d/access.hpp"
#include "gendb3d/ext_obj3d.hpp"
#include "step/ext.hpp"

namespace def::dbio::g3d {

// @src 14016cf10  WriteGeneralInfoToDB. iiobj[0] mesh flag (0 type 7, 1 meshed, 2 not), [1]/[2]
//                 object type codes, [3] sim flag (m+0x854), [4..7] node/element ranges (advance
//                 the running offsets), 3D [12] nodes per element 8/4 and [13] connectivity start,
//                 [8]/[9] first/last LLBND record (also kept at obj+0x3588/0x358c); LLOBJ words
//                 4.. the name as a 64-character Fortran field
void write_general_info(LLOBJ_DATA* obj, std::int32_t* iiobj, double* llobj, std::int32_t conn_first,
                        std::int32_t rec_last, std::int32_t rec_n, std::int32_t* /*iifnc*/,
                        double* /*llfnc*/, std::int32_t* /*mat_pos*/, const char* meshed)
{
    if (fld<std::int32_t>(obj, 0xc) == 7) {
        iiobj[0] = 0;
        return;
    }
    if (fld<std::int32_t>(obj, 0x87c) == 0 || *meshed == '\0') {
        iiobj[0] = 2;
    }
    else {
        iiobj[0] = 1;
    }
    iiobj[1] = 1;
    iiobj[2] = 0;
    switch (fld<std::int32_t>(obj, 0xc)) { // jump table 14016d1d0 (unsigned compare with 6)
    case 0: iiobj[1] = 0; break;
    case 1: iiobj[1] = 1; break;
    case 2: iiobj[2] = 1; break;
    case 3: iiobj[1] = 2; break;
    case 4: iiobj[1] = 3; break;
    case 6: iiobj[1] = 4; break;
    default: break;
    }
    iiobj[3] = 1;
    if (fld<std::int32_t>(owner_of(obj), 0x854) == 0) {
        iiobj[3] = 0;
    }
    if (iiobj[0] == 1) {
        const std::int32_t n0 = node_offset(owner_of(obj));
        const std::int32_t e0 = elem_offset(owner_of(obj));
        iiobj[4] = n0 + 1;
        iiobj[5] = n0 + fld<std::int32_t>(obj, 0x880);
        set_node_offset(owner_of(obj), &iiobj[5]);
        iiobj[6] = e0 + 1;
        iiobj[7] = e0 + fld<std::int32_t>(obj, 0x884);
        set_elem_offset(owner_of(obj), &iiobj[7]);
        if (rt::dimension_is_3(owner_of(obj))) {
            iiobj[0xc] = 8;
            if (reinterpret_cast<model::Cls_140182880*>(at(obj, 0x9e0))->width() == 4) { // 140185530
                iiobj[0xc] = 4;
            }
            iiobj[0xd] = conn_first;
        }
    }
    iiobj[8] = (rec_last - rec_n) + 1;
    fld<std::int32_t>(obj, 0x3588) = iiobj[8];
    iiobj[9] = rec_last;
    fld<std::int32_t>(obj, 0x358c) = iiobj[9];
    ::def::dbio::ext::fun_140323bf0(reinterpret_cast<char*>(llobj) + 0x20, obj->name, 0x40);
}

// @src 14016d200  LLOBJ word 48 = obj+0x3590 (phase count); word 49 = global function number of
//                 the object's curve u3598 (0 without phases), its values into LLFNC (140131210)
void write_general_info2(LLOBJ_DATA* obj, std::int32_t* /*iiobj*/, double* llobj, std::int32_t /*conn_first*/,
                         std::int32_t /*rec_last*/, std::int32_t /*rec_n*/, std::int32_t* iifnc,
                         double* llfnc, std::int32_t* /*mat_pos*/, const char* /*meshed*/)
{
    std::int32_t len = 0;
    llobj[0x30] = static_cast<double>(fld<std::int32_t>(obj, 0x3590));
    if (fld<std::int32_t>(obj, 0x3590) == 0) {
        llobj[0x31] = 0.0;
    }
    else {
        std::int32_t pos = 0;
        len = ext::f140130290(&obj->u3598);
        alloc_function(owner_of(obj), &len, iifnc, &fld<std::int32_t>(obj, 8), &llobj[0x31], &pos);
        write_func_to_db_array(&obj->u3598, llfnc + (pos - 1), nullptr);
    }
}

// @src 140203980  2D: 1402039d0, otherwise 140203a20 (the callee's bool is returned in AL)
bool object_has_geometry(LLOBJ_DATA* obj)
{
    if (rt::dimension_is_2(owner_of(obj))) {
        return ext::f1402039d0(obj);
    }
    return ext::f140203a20(obj);
}

} // namespace def::dbio::g3d
