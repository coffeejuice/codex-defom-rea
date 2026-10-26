// defmodel/material/material_ctor.hpp: Material construction and the material-list bookkeeping
// that the group 4 KEY reader drives (packet kw-g4b).
//
// The constructors of Material and of the sub-objects this packet owns (DIFCOE Cls_140289930,
// GRNDAT Cls_14028b850 / Cls_14028a140 / Cls_14028b640 / Cls_14028a4c0, TXTURE Cls_14028c500) are
// defined in src/material/material_ctor.cpp; the other sub-object ctors (FSTRES, CREEP,
// BURGRS..RECVRY, FRCMOD/UTSDAT, ANISO) belong to kw-g4a. Destructors are p1-08's
// (defmodel/mesh2d/material_ops.hpp). Free functions because the layout headers are edited by the
// skeleton thread only. Specs: docs/re/keywords/_common_g457.md section 5, MTNAME.md, GRNDAT.md.
#pragma once

#include <cstdint>

#include <defmodel/layout/cls_14028a4c0.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/material.hpp>
#include <defmodel/layout/material_list.hpp>

namespace def::model {

// 14028cdc0: number = 0, name = "", the scalar defaults (FRAE2H 0.9, ...), the DYNARY templates
// of HDNTIM (0x53), JOMINY (0x52), DIFBND (0xaf), FPERV (0, 0), GBENGY (0x5d), NUCSIZ (0xb6), then
// material_clear_u004.
void material_init(Material* m);

// 14028d030: the two ints at +0x4 and +0x8 = 0.
void material_clear_u004(Material* m);

// 14028a580: GRNDAT record init (DYNARY templates 0x5e/0x5f/0x60, n = 12, values = 0, d = 0,
// i = 0, is_main = 0).
void grndat_record_init(Cls_14028a4c0* r);

// 14029ccd0: record value count; n > 12 is cut to 12 (also in *n) with
// "Error - SubbdryVarSize is incorrect.\n" on stdout.
void grndat_record_set_n(Cls_14028a4c0* r, std::int32_t* n);

// 14028b050: appends a new record (operator new(0x1f0) + ctor) to a GRNDAT record list.
Cls_14028a4c0* grndat_record_list_add(Cls_14028b640::RecordList* l);

// 14029c060: appends a new Material (operator new(0x2218), ctor with list->owner) at the tail.
Material* material_list_add(MaterialList* list);

// 14029ca10: frees numbers / index_of; unless free_only, rebuilds them for the current nodes:
// numbers[i] = nth(i+1)->number, index_of[i] = material_list_index_of(list, that number).
void material_list_rebuild_index(MaterialList* list, bool free_only);

// 14029c870: number of nodes whose number is smaller than `number`, or -1 if no node has it.
std::int32_t material_list_index_of(const MaterialList* list, std::int32_t number);

} // namespace def::model
