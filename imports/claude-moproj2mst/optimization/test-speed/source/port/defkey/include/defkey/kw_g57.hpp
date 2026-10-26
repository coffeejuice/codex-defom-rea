// defkey/kw_g57.hpp: KEY reader and writer of registry group 5 (phase transformation: PHASTF,
// TTTD, TTTN, PHASLH, PHASVL, TRNSFP, PHEQVF, COARSN, IPSNRG, VARSEL) and group 7 (DIM),
// packet kw-g57.
//
// Specs: docs/re/keywords/<KW>.md, _common_g457.md (sections 4-6), group4_5_7-index.md.
// Status: docs/re/status/kw-g57.tsv. Model side: defmodel/phase/phase_ops.hpp (pair list, DIM
// routing) and the PhasePair ctor (src/phase/phase_pair_ctor.cpp); the PhasePairList ctor/clear
// and the PhasePair dtor are p1-10's.
#pragma once

#include <cstdint>
#include <cstdio>
#include <vector>

#include <defmodel/layout/dynary.hpp>
#include <defmodel/layout/phase_pair.hpp>

namespace def::model {
class DB_STEP_DATA;
}

namespace def::key::g57 {

using def::model::DB_STEP_DATA;
using def::model::DYNARY;
using def::model::PhasePair;

// Keyword ids
inline constexpr int kPHASTF = 0x86;
inline constexpr int kTTTD = 0x87;
inline constexpr int kTTTN = 0x88;
inline constexpr int kPHASLH = 0x89;
inline constexpr int kPHASVL = 0x8a;
inline constexpr int kTRNSFP = 0x8b;
inline constexpr int kPHEQVF = 0x8c;
inline constexpr int kCOARSN = 0x8d;
inline constexpr int kIPSNRG = 0x8e;
inline constexpr int kVARSEL = 0x8f;
inline constexpr int kDIM = 0x120;

// ---- KEY reader -------------------------------------------------------------------------------
// FUN_140383120: one group 5 keyword; `m` is what FUN_1401d81b0 returned. Returns 0, 2 or 3.
int read_phase_keyword(DB_STEP_DATA* m, std::FILE* f, int id);
// FUN_14037e530: the group 7 parser (DIM only); `m` is the model itself. Returns 0 or 2.
int read_dim_keyword(DB_STEP_DATA* m, std::FILE* f, int id);

// ---- KEY writer -------------------------------------------------------------------------------
// FUN_1403cbf00: the block of one phase pair (only when PHASTF is on); `index` is its 1-based list
// position, which the handlers use to find the pair again. First non-zero handler result aborts.
int write_phase_pair(DB_STEP_DATA* m, std::FILE* f, PhasePair* pair, std::int32_t index);

// Registry handler column 2 (KEY write), {pairIndex}
int write_tttd(std::FILE* f, DB_STEP_DATA* m, std::vector<int> v);    // 1403ccd00
int write_phaslh(std::FILE* f, DB_STEP_DATA* m, std::vector<int> v);  // 1403cc4a0
int write_phasvl(std::FILE* f, DB_STEP_DATA* m, std::vector<int> v);  // 1403cc5e0
int write_trnsfp(std::FILE* f, DB_STEP_DATA* m, std::vector<int> v);  // 1403cc720
int write_pheqvf(std::FILE* f, DB_STEP_DATA* m, std::vector<int> v);  // 1403cca00
int write_coarsn(std::FILE* f, DB_STEP_DATA* m, std::vector<int> v);  // 1403ccb60
int write_ipsnrg(std::FILE* f, DB_STEP_DATA* m, std::vector<int> v);  // 1403ccc50

// Registry handler column 3 (data accessor), {pairIndex}
int access_tttd(DB_STEP_DATA* m, const char* label, std::vector<int> v, DYNARY** out);    // 1403cdb20
int access_phaslh(DB_STEP_DATA* m, const char* label, std::vector<int> v, DYNARY** out);  // 1403cc550
int access_phasvl(DB_STEP_DATA* m, const char* label, std::vector<int> v, DYNARY** out);  // 1403cc690
int access_trnsfp(DB_STEP_DATA* m, const char* label, std::vector<int> v, DYNARY** out);  // 1403cc970
int access_pheqvf(DB_STEP_DATA* m, const char* label, std::vector<int> v, DYNARY** out);  // 1403ccad0

// Group 5 header writers (the material-side ones are key-common's table_io.hpp)
int write_header_pair(std::FILE* f, const char* name, std::int32_t from, std::int32_t to,
                      const DYNARY* t);                                          // 1402f43a0
int write_header_pair_t(std::FILE* f, const char* name, std::int32_t from, std::int32_t to,
                        std::int32_t t_code, const DYNARY* t);                   // 1402f4530
int write_header_pair_dd(std::FILE* f, const char* name, std::int32_t from, std::int32_t to,
                         double c1, double c2, const DYNARY* t);                 // 1402f4690

} // namespace def::key::g57
