// defkey/kw_g2nbc.hpp: the "kw-g2nbc" slot of the group 2 Packet API (defkey/kw_g2obj.hpp).
// The nodal and local BC keywords it names (BCC*, BCCFNC, LOC*) were folded into packet kw-g2bc
// (defkey/kw_g2bc.hpp), which handles their parser cases and handlers; this header only routes
// the shell's write_local_bcs call there.
#pragma once

#include <cstdint>
#include <cstdio>
#include <vector>

#include <defkey/kw_g2bc.hpp>

namespace def::key::g2nbc {

using KeyRef = std::vector<std::int32_t>;

inline bool handles(int) { return false; }  // kw-g2bc handles these ids
inline int read_case(def::model::DB_STEP_DATA&, std::FILE*, def::model::LLOBJ_DATA&, int) { return 2; }
inline bool handles_write(int) { return false; }
inline int write_keyword(int, std::FILE*, def::model::DB_STEP_DATA&, const KeyRef&) { return 0; }

// FUN_1402ac560: LOCTMP, LOCATM, LOCDEF lists (writer-map step 53).
inline int write_local_bcs(def::model::DB_STEP_DATA& m, std::FILE* f, def::model::LLOBJ_DATA& obj,
                           std::uint32_t n)
{
    return def::key::g2bc::write_local_bcs(m, f, obj, n);
}

}  // namespace def::key::g2nbc
