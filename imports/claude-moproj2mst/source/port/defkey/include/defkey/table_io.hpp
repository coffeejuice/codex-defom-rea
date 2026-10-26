// defkey/table_io.hpp: KEY reading and writing of the DYNARY table object (0x78-byte curve,
// defmodel/layout/dynary.hpp; template family in defmodel/dynary_init.hpp).
// Spec: docs/re/keywords/_common.md section 3, _common_g457.md (ujpfry) for the headers.
//
// Status codes as in the binary: readers return 0 or 1 (1 = EOF before a value, which the group
// parsers usually turn into "Reading Error"); writers return 0, or 2 for a null FILE.
// Numbers are read with crt::fscanf("%Flf") (free format: values may span lines) and written with
// " %20.10E" (std::fprintf; needs the legacy printf options, see crt_scan.hpp).
#pragma once

#include <cstdint>
#include <cstdio>

#include <defmodel/layout/dynary.hpp>

namespace def::key {

using def::model::DYNARY;

// ---- readers ---------------------------------------------------------------------------------

// 14041a800: the table body. Does nothing unless ndim != 0 or ncomp > 1. Re-allocates
// (dynary_alloc), then reads:
//   ndim 0:    ncomp values;
//   ndim 1:    per point: abscissa, then ncomp values;
//   ndim 2..3: all abscissae of dimension 0, then 1 (then 2), then all values in storage order.
// If dynary_value_count > 0, one fgets swallows the rest of the last data line. A token that does
// not convert makes fscanf return 0 (not -1): the read goes on and leaves 0s. Callers pass a
// third argument that the function ignores.
int read_table_body(std::FILE* f, DYNARY* t, int unused = 0);

// 14041ac10: ncomp values of a constant vector (ndim 0, ncomp > 0), after dynary_alloc;
// returns 1 when the table is not such a vector.
int read_table_vector(std::FILE* f, DYNARY* t);

// 14041b270: "type a b c" property header on the rest of the line (%i %Flf %Flf %Flf), then the
// body. dynary_reinit_keep(t, 0), then by type: 0 constant a; 1 ndim 1 var 3 (temperature);
// 2 var 9; 3 var 0xc; 6 var 8; with n0 = (int)a; 4 ndim 2 var (3, 0xc), 5 (3, 8), 7 (3, 4) with
// n = ((int)a, (int)b); other types keep the reset shape. When want_extra >= 0 and extra != null:
// types 0..3 store b in *extra if at least 3 fields converted, type 4 stores c if at least 4.
int read_table_header(std::FILE* f, DYNARY* t, int want_extra, double* extra);

// 14041ace0: region entry "region ndim var|ncomp a b" (%i %i %i %Flf %Flf via read_iii_dd).
// ndim 0: ncomp = field 3, alloc, then constant = a when ncomp < 2, else ncomp values read
// free-format. ndim 1: var = field 3, n0 = (int)a; n0 == 0: constant = b, ncomp 0; else
// ncomp = (int)b; alloc; then per point abscissa + ncomp values. Other ndim -> 1. The region
// number is read but not stored. Swallows the rest of the line like read_table_body.
int read_region_table(std::FILE* f, DYNARY* t);

// 14041b060: region entry whose values are 64-byte strings, one line each (read_string_line,
// copied blank-padded to 64 bytes into values + 64*i). ndim must be 0 or 1 (else 1); ndim 0
// reads nothing more.
int read_region_string_table(std::FILE* f, DYNARY* t);

// ---- writers ---------------------------------------------------------------------------------

// 1402f2e80: n doubles, " %20.10E" each; before item i > 0 with i % per_line == 0 a "\n" and 8
// spaces; a final "\n". alt uses " %20.10E," instead. 2 if f is null or per_line < 1; 0 without
// output if n < 1 or v is null.
int write_doubles(std::FILE* f, int n, const double* v, int per_line, bool alt = false);
// 1402f2fe0: the same for ints, " %7i" (alt " %7i,").
int write_ints(std::FILE* f, int n, const std::int32_t* v, int per_line, bool alt = false);

// 1402f3130: the table body. mode 0 (KEY writers): nothing when some n[i] < 1 or ndim 0;
// ndim 1: per point " %20.10E" abscissa then the ncomp values (write_doubles, 5 per line);
// ndim 2..3: abscissae per dimension then all values, per_line = max(n). Mode 1 or 2 add the
// counts first (" %7i\n" for ndim 1, write_ints for ndim 2..3), skip the n < 1 guard and write
// the constant of ndim 0 (" %20.10E\n" if ncomp == 1, else ncomp values); mode 2 also uses the
// comma variants for ndim 2..3. Mode 6 writes the ndim 0 constant like 1. 0 for a null table.
int write_table_body(std::FILE* f, const DYNARY* t, int mode);

// 1402f34f0: region entry, the counterpart of read_region_table (only mode 1 writes):
// ndim 0: " %7i %7i %7i %20.10E\n" (region, 0, ncomp, constant) when ncomp < 2, else
// " %7i %7i %7i\n" and the ncomp values; ndim 1, n0 == 0: " %7i %7i %7i %7i %20.10E\n"
// (region, 1, var, 0, constant); ndim 1, n0 > 0: " %7i %7i %7i %7i %7i\n" (region, 1, var, n0,
// ncomp) then per point the abscissa (" %20.10E", or the 1-based index " %7i" when var == 0) and
// its ncomp values.
int write_region_table(std::FILE* f, const DYNARY* t, int mode, int region);

// 1402f3840: region entry of a string table (counterpart of read_region_string_table), mode 1,
// ndim 1 only: " %7i %7i %7i %7i %7i\n" (region, 1, var, n0, ncomp), then each 64-byte string
// with trailing blanks trimmed, "%s\n".
int write_region_string_table(std::FILE* f, const DYNARY* t, int mode, int region);

// 1402f3aa0: "NAME mat type n..." property header (counterpart of read_table_header) with an
// optional double extra (has_extra == 1 and extra != null), then write_table_body(mode 0).
// "COARSE" has its own two-line form and prints "\nWarning: data for keyword %s has error. Its
// data type will be clear(0).\n" (keyword 0x7e) to stdout when the extra is missing.
int write_table_header_d(std::FILE* f, const char* name, int mat, const DYNARY* t, int has_extra,
                         const double* extra);
// 1402f40f0: the same with an int extra (HEATCP style); no COARSE case, var 8 maps to type 3.
int write_table_header_i(std::FILE* f, const char* name, int mat, const DYNARY* t, int has_extra,
                         const std::int32_t* extra);

} // namespace def::key
