// dbread/pending_w2.hpp: functions the W2 code of packet p-dbread calls that belong to another
// helper of the packet (docs/re/dbread_todo_survey.tsv). Declared with the binary's argument list;
// the owning helper defines them (the lead wires the final names).
#pragma once

#include <cstdint>

namespace def::model {
class DYNARY;
} // namespace def::model

namespace def::dbio::dbread {

// W1  1401309b0  plain-layout function `fid` (IIFNC/LLFNC) into a pre-shaped f; *flag != 0 takes
// the 2-D/3-D sizes from a real prefix of the data (BURGRS / ALPHA / NDISFM / RECVRY tables)
void fun_1401309b0(model::DYNARY* f, const std::int32_t* iifnc, const double* llfnc, std::int32_t fid,
                   const char* flag);

// W1  140131ea0  1-D table data (x, y[0..ncomp-1]) per point into f (n[0] points, ncomp from f)
void fun_140131ea0(model::DYNARY* f, const double* data);

// W1  140324020  byte-order fix of DB text: swaps 4-byte words (and then 8-byte pairs when
// ints_only == 0) of `nwords` words of s when host*100 + platform needs it
void fun_140324020(void* s, std::int32_t platform, std::int32_t nwords, std::int32_t ints_only);

} // namespace def::dbio::dbread
