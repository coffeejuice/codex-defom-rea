// Keyword group 1 (simulation control): the KEY parser and writer shells.
//
// read_group1 is FUN_14039aa90, which the KEY interpreter FUN_1401b6520 calls for every group 1
// keyword; write_group1 is FUN_1403d4990, which the KEY writer FUN_1401c5af0 calls after the
// banner. The shells own the dispatch only: ids 22..60 and the first two writer sections are
// kw-g1a (<defkey/kw_g1a.hpp>), ids 61..91 and the last three sections are kw-g1b
// (<defkey/kw_g1b.hpp>).
#pragma once

#include <cstdio>

namespace def::model {
class DB_STEP_DATA;
}

namespace def::key {

// FUN_14039aa90(obj, f, id): returns 0 (continue), 2 ("Reading Error", also for an id that is not
// a group 1 keyword) or 3 ("Data Error").
int read_group1(model::DB_STEP_DATA* m, std::FILE* f, int id);

// FUN_1403d4990(M, f): every group 1 section of a KEY file. Returns 0, or the first non-zero
// code of a per-keyword writer, which aborts the KEY write.
int write_group1(model::DB_STEP_DATA* m, std::FILE* f);

// True when kw-g1b's part (ids 61..91, the last three writer sections) is linked in.
bool group1_has_g1b();

} // namespace def::key
