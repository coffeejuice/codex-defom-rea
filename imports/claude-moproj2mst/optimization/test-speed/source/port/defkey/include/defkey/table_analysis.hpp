// defkey/table_analysis.hpp: FUN_1402f5360, the "uniform / default / record count" analysis the
// KEY writers run before printing a per-node or per-row double array in sparse form (DYNARY
// subtype 3 in FUN_1402a41c0; the state-variable writers use the same function). Packet kw-g2mg.
#pragma once

#include <cstdint>

namespace def::key {

// values: rows * ncomp doubles. mode 2: uniform means every row equals row 0 component by
// component; any other mode: every value equals values[0]. `uniform` must come in as 1 and is
// cleared when the array is not uniform. Then:
//   uniform:     defaults[k] = values[k] (k < ncomp), nrec unchanged;
//   not uniform: defaults[k] = *reference, and nrec += the number of rows with at least one
//                value != *reference.
// `defaults` holds ncomp doubles. rows and ncomp are read through pointers, as in the binary.
void analyze_table(const double* values, const std::int32_t* rows, const std::int32_t* ncomp,
                   const double* reference, const std::int32_t* mode, char* uniform,
                   std::int32_t* nrec, double* defaults);

} // namespace def::key
