// defkey/kw_g2bc.hpp: keyword group 2 (objects), packet kw-g2bc: the boundary-condition keywords.
//
//   edge/face BCs   ECCDEF ECCTMP ECCATM ECDEFN ECTMFN ECATFN ECPRES ECHFLX ECAFLX ECCRHT ECRHFN
//                   ECRFLX (parser cases 0x61..0x6c, writer FUN_1402abf20, handlers 1402b6840..)
//   window BCs      WINPRS WINTMP WINATM WINRSE (cases 0x52..0x55; writer-map steps 48-52 and
//                   FUN_1402ab880 for type-5 objects)
//   nodal BCs       BCCFNC BCCDEF BCCDFN BCCANG BCCTMP BCCTFN BCCCRB BCCRHT CRBFLX RHTFLX HDNOBJ
//   local BCs       LOCTMP LOCATM LOCDEF (writer FUN_1402ac560), RSEDEF (FUN_1402b7140)
//
// The group 2 shells (kw-g2obj, defkey/kw_g2obj.hpp, "Packet API") call these. BCCDEF, BCCDFN,
// BCCTMP and BCCTFN are written by the state-variable writer FUN_1402a4a80 (kw-g2sv); this packet
// ports their readers. HDNEST belongs to kw-g2sv.
// Specs: docs/re/keywords/<KW>.md, group2-helpers.md, group2-writer-map.md.
#pragma once

#include <cstdint>
#include <cstdio>
#include <vector>

namespace def::model {
class DB_STEP_DATA;
class LLOBJ_DATA;
}  // namespace def::model

namespace def::key::g2bc {

using KeyRef = std::vector<std::int32_t>;  // same as def::key::g2obj::KeyRef

// Parser ids whose case of FUN_14038b8a0 this packet ports.
bool handles(int id);
// The case body after the object lookup: 0, 2 ("Reading Error") or 3 ("Data Error").
int read_case(def::model::DB_STEP_DATA& m, std::FILE* f, def::model::LLOBJ_DATA& obj, int id);

// Registry column 2 (KEY write handlers) owned here.
bool handles_write(int id);
int write_keyword(int id, std::FILE* f, def::model::DB_STEP_DATA& m, const KeyRef& ref);

// Block writers the object writer FUN_14029e250 calls; N is the printed object number.
int write_winprs_block(def::model::DB_STEP_DATA& m, std::FILE* f, def::model::LLOBJ_DATA& obj,
                       std::uint32_t n);  // 1402ab880 (type-5 objects)
void write_window_lists(def::model::DB_STEP_DATA& m, std::FILE* f, def::model::LLOBJ_DATA& obj,
                        std::uint32_t n);  // part of 14029e250: writer-map steps 48-52
int write_edge_face_bcs(def::model::DB_STEP_DATA& m, std::FILE* f, def::model::LLOBJ_DATA& obj,
                        std::uint32_t n);  // 1402abf20
int write_local_bcs(def::model::DB_STEP_DATA& m, std::FILE* f, def::model::LLOBJ_DATA& obj,
                    std::uint32_t n);  // 1402ac560
int write_rsedef(std::FILE* f, def::model::DB_STEP_DATA& m, const KeyRef& ref);  // 1402b7140

}  // namespace def::key::g2bc
