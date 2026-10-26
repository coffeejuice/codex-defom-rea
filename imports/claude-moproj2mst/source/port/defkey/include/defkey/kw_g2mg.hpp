// defkey/kw_g2mg.hpp: keyword group 2, packet kw-g2mg: the mesh-generation keywords MGNELM,
// MGTELM, MGSIZR, MGWCUV, MGWTMP, MGWSTN, MGWSTR, MGWUSR, MGGRID, MGERR, MGUSER, MGSIZM (parser
// cases 0x28..0x33), OPSTOP (0x79) and DYNARY (0x7a), the mesh-settings block writer FUN_1402a9180
// (which also prints RMSTEP and calls kw-g2obj's RMDPTH/RMSTRK/RMTIME handlers), and the OPSTOP and
// DYNARY list writers. Plugged into kw-g2obj's shells (defkey/kw_g2obj.hpp, "Packet API").
// Specs: docs/re/keywords/<KW>.md (MGNELM.md has the block order), group2-helpers.md.
#pragma once

#include <cstdint>
#include <cstdio>
#include <vector>

namespace def::model {
class DB_STEP_DATA;
class LLOBJ_DATA;
} // namespace def::model

namespace def::key::g2mg {

namespace model = def::model;
using KeyRef = std::vector<std::int32_t>;

// Parser ids whose FUN_14038b8a0 case is ported here: 184..195, 265, 266.
bool handles(int id);
// part of @src 14038b8a0  the case body after the object lookup: 0, 2 or 3.
int read_case(model::DB_STEP_DATA& m, std::FILE* f, model::LLOBJ_DATA& obj, int id);

// Registry handler2 ids ported here: MGSIZR, MGWCUV, MGWTMP, MGWSTN, MGWSTR, MGERR.
bool handles_write(int id);
int write_keyword(int id, std::FILE* f, model::DB_STEP_DATA& m, const KeyRef& ref);

int write_mgsizr(std::FILE* f, model::DB_STEP_DATA& m, const KeyRef& ref);  // 1402b44e0
int write_mgwcuv(std::FILE* f, model::DB_STEP_DATA& m, const KeyRef& ref);  // 1402b45b0
int write_mgwtmp(std::FILE* f, model::DB_STEP_DATA& m, const KeyRef& ref);  // 1402b4680
int write_mgwstn(std::FILE* f, model::DB_STEP_DATA& m, const KeyRef& ref);  // 1402b4750
int write_mgwstr(std::FILE* f, model::DB_STEP_DATA& m, const KeyRef& ref);  // 1402b4820
int write_mgerr(std::FILE* f, model::DB_STEP_DATA& m, const KeyRef& ref);   // 1402b48f0

// FUN_1402a9180(model, f, obj, N): RMDPTH .. MGUSER for one object (order in MGNELM.md). Returns
// 0, the first non-zero handler code, or 3 for a 3D window of unknown shape (the rest of the
// windows is not written). The caller ignores the result.
int write_mesh_block(model::DB_STEP_DATA& m, std::FILE* f, model::LLOBJ_DATA& obj, std::uint32_t no);
// FUN_1402a4020: "OPSTOP N 0 0", then every type-1 entry. 0 (3 for a null f or obj).
int write_opstop(model::DB_STEP_DATA& m, std::FILE* f, model::LLOBJ_DATA& obj, std::uint32_t no);
// FUN_1402a41c0: every DYNARY record. 0, or 3 at the first record it cannot write.
int write_dynary(model::DB_STEP_DATA& m, std::FILE* f, model::LLOBJ_DATA& obj, std::uint32_t no);

} // namespace def::key::g2mg
