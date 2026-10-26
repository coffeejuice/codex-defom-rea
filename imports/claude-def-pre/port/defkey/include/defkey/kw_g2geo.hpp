// defkey/kw_g2geo.hpp: keyword group 2 (objects), packet kw-g2geo: geometry and mesh keywords.
//
//   curve geometry   DIEGEO (curve and type-3 forms), TPLOGY, CSPMTX
//   surface          DIEGEO (surface form), POLYTP, DIEPNT, DIEEDG, FLDGEO, FLWNET
//   FE mesh          RZ, ELMCON, MTLGRP; nodal vectors DRZ, URZ, FRZ, PRZ, DRMESH (readers; the
//                    handlers 1402b4f80.. call kw-g2sv's state-variable writer)
//   other            SYMSUF, WPAXIS, CRDSYS, GEO23, GEOSEC, MSHSEC (the last four are parsed only)
//
// The group 2 shells (kw-g2obj, defkey/kw_g2obj.hpp, "Packet API") call these.
// Specs: docs/re/keywords/<KW>.md, group2-helpers.md, group2-writer-map.md (branch
// claude/project-thread-j4c7t9). Model code: port/defmodel/src/geo_kw/.
#pragma once

#include <cstdint>
#include <cstdio>
#include <vector>

namespace def::model {
class DB_STEP_DATA;
class LLOBJ_DATA;
}  // namespace def::model

namespace def::key::g2geo {

using KeyRef = std::vector<std::int32_t>;  // same as def::key::g2obj::KeyRef

// Parser ids whose case of FUN_14038b8a0 this packet ports.
bool handles(int id);
// The case body after the object lookup: 0, 2 ("Reading Error") or 3 ("Data Error").
int read_case(def::model::DB_STEP_DATA& m, std::FILE* f, def::model::LLOBJ_DATA& obj, int id);

// Registry column 2 (KEY write handlers) owned here: DRZ, URZ, FRZ, PRZ.
bool handles_write(int id);
int write_keyword(int id, std::FILE* f, def::model::DB_STEP_DATA& m, const KeyRef& ref);

// Block writers the object writer FUN_14029e250 calls; N is the printed object number.
// part of 14029e250: writer-map steps 19-22 (DIEGEO 1402aaa70, geometry 1402a26c0, 2D TPLOGY
// 1402a9f90, FLDGEO/FLWNET 1402a2b70) with their conditions.
void write_geometry(def::model::DB_STEP_DATA& m, std::FILE* f, def::model::LLOBJ_DATA& obj,
                    std::uint32_t n);
int write_nodes(def::model::DB_STEP_DATA& m, std::FILE* f, def::model::LLOBJ_DATA& obj,
                std::uint32_t n);  // 1402a2db0 (RZ)
int write_elements(def::model::DB_STEP_DATA& m, std::FILE* f, def::model::LLOBJ_DATA& obj,
                   std::uint32_t n);  // 1402a30c0 (ELMCON)
int write_symsuf(def::model::DB_STEP_DATA& m, std::FILE* f, def::model::LLOBJ_DATA& obj,
                 std::uint32_t n);  // 1402a3880
int write_wpaxis(def::model::DB_STEP_DATA& m, std::FILE* f, def::model::LLOBJ_DATA& obj,
                 std::uint32_t n);  // 1402a39e0

// @src 1402a1df0  the "short" per-object KEY writer of 1401c5d20: object banner, OBJTYP, OBJNAM,
// geometry (2D: the DIEGEO curves of geometry 0 inline; else 1402a26c0 when the object has a
// surface or curves), RZ, ELMCON. out_no 0 prints the object id (+8). 0 for a null object, 3
// when OBJTYP rejects the type, else 0 (sub-writer statuses ignored).
int write_object_short(def::model::DB_STEP_DATA& m, std::FILE* f, def::model::LLOBJ_DATA* obj,
                       std::uint32_t out_no);

}  // namespace def::key::g2geo
