// kw-g1a: KEY reader and writer for keyword group 1, sections "Process Definition" (TITLE ..
// ELMNOD) and "Stopping & Step Controls" (NSTART .. ODMAX, plus the read-only DSMAX_NSTEP).
// Ids 22..60, cases 0x00..0x26 of the group 1 parser FUN_14039aa90.
//
// Specs: docs/re/keywords/<KW>.md and _common.md. The group 1 shells that dispatch here (and to
// kw-g1b for ids 61..91) are in <defkey/group1.hpp>.
#pragma once

#include <cstdint>
#include <cstdio>
#include <vector>

namespace def::model {
class DB_STEP_DATA;
class DYNARY;
class Cls_1401e7b10;
}

namespace def::key::g1a {

inline constexpr int kFirstId = 22;  // TITLE
inline constexpr int kLastId = 60;   // ODMAX

// Cases 0x00..0x26 of FUN_14039aa90. Returns the parser's code: 0 continue, 2 "Reading Error"
// (any helper failure); 2 also for an id outside 22..60.
int read_keyword(model::DB_STEP_DATA& m, std::FILE* f, int id);

// FUN_1403d4990 from the "Process Definition" banner up to (not including) the "Iteration
// Controls" banner. Returns 0, or the first non-zero code of a per-keyword writer (3 for a
// DTTYPE table whose ndim is neither 0 nor 1); the KEY write stops there.
int write_sections(std::FILE* f, model::DB_STEP_DATA& m);

// Registry column 2 (KEY write handlers, called by FUN_1402f79c0 and by FUN_1403d4990).
// The original receives a heap vector it destroys; group 1 writers never read it.
using KeyWriteVec = std::vector<std::int32_t>;
int write_emax(std::FILE* f, model::DB_STEP_DATA* m, const KeyWriteVec& vec);    // 1403d7720
int write_tmax(std::FILE* f, model::DB_STEP_DATA* m, const KeyWriteVec& vec);    // 1403d77a0
int write_smax(std::FILE* f, model::DB_STEP_DATA* m, const KeyWriteVec& vec);    // 1403d7820
int write_vmin(std::FILE* f, model::DB_STEP_DATA* m, const KeyWriteVec& vec);    // 1403d7910
int write_lmax(std::FILE* f, model::DB_STEP_DATA* m, const KeyWriteVec& vec);    // 1403d7a00
int write_demax(std::FILE* f, model::DB_STEP_DATA* m, const KeyWriteVec& vec);   // 1403d7af0
int write_dsmax(std::FILE* f, model::DB_STEP_DATA* m, const KeyWriteVec& vec);   // 1403d7b70
int write_dtmax(std::FILE* f, model::DB_STEP_DATA* m, const KeyWriteVec& vec);   // 1403d7bf0
int write_dttype(std::FILE* f, model::DB_STEP_DATA* m, const KeyWriteVec& vec);  // 1403d7ce0
int write_slderr(std::FILE* f, model::DB_STEP_DATA* m, const KeyWriteVec& vec);  // 1403d7ef0
int write_dtpmax(std::FILE* f, model::DB_STEP_DATA* m, const KeyWriteVec& vec);  // 1403d7f70
int write_dvmax(std::FILE* f, model::DB_STEP_DATA* m, const KeyWriteVec& vec);   // 1403d80a0
int write_mdsobj(std::FILE* f, model::DB_STEP_DATA* m, const KeyWriteVec& vec);  // 1403d8130
int write_cnterr(std::FILE* f, model::DB_STEP_DATA* m, const KeyWriteVec& vec);  // 1403d81d0

// Registry column 3 of DTTYPE (FUN_1403d7e80): *out = the DTTYPE table (M+0x938) unless the
// simulation-control pointer M+0x750 is null. Returns 0.
int dttype_table(model::DB_STEP_DATA* m, void* unused, const KeyWriteVec& vec,
                 model::DYNARY** out);

// STNOUT and ELMNOD writers that FUN_1403d4990 calls directly (not in the registry).
int write_stnout(model::DB_STEP_DATA* m, std::FILE* f);  // 1403d6580
int write_elmnod(model::DB_STEP_DATA* m, std::FILE* f);  // 1403d6620

// Calls into code other packets own. Tests install recorders; with a hook null the real function
// runs: FUN_1401d0fc0 (def::model::call_1401d0fc0), FUN_140235c30 (def::rt::ftn::set_unit_system).
struct ModelHooks {
    // FUN_1401d0fc0(M, 0): rebuild the state-variable layout table M+0x15f8 after GEOTYP,
    // STNOUT and ELMNOD (model skeleton).
    void (*rebuild_state_layout)(model::DB_STEP_DATA* m, int arg) = nullptr;
    // FUN_140235c30(unit): global unit flags 0x1406fe778 / 0x14070ba70 and the unit tables
    // (FUN_1403f8b70 -> FUN_1403f9f20), after UNIT changes the unit system (units module).
    void (*set_unit_system)(int unit) = nullptr;
};
ModelHooks& model_hooks();

} // namespace def::key::g1a
