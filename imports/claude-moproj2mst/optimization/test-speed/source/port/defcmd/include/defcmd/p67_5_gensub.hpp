// defcmd/p67_5_gensub.hpp: the parts of GENSUB (dispatcher case 0x14, packet p67-5) that tests
// call directly. Spec: docs/re/commands/GENSUB.md. The command entry is def::cmd::cmd_gensub
// (p67_5.hpp).
#pragma once

#include <cstdint>
#include <cstdio>
#include <stdexcept>
#include <string>
#include <vector>

namespace def::model {
class DB_STEP_DATA;
class LLOBJ_DATA;
} // namespace def::model

namespace def::cmd::gensub {

// ---- the VS2008 CRT scanf subset the converter uses (gensub_scan.cpp) ---------------------------

// 1404ab960 / 1404a1544 / 1404b04c0: what scanf("%f") stores for the collected number text
// (_fassign_l flag 0 -> _atoflt_l -> __strgtold12_l + _ld12tof). Not a correctly rounded strtof:
// the CRT multiplies by 80-bit powers of ten and rounds exact ties of the result down.
float crt_atoflt(const char* text);

// _input_l (140493dec) for the directives %d %s %f (with '*' and 'l'), white space and literal
// characters, reading from a FILE (fscanf, 140484924) or a string (sscanf, 14047c354). `args`
// are the targets in order (suppressed conversions take none). Returns what the CRT returns.
int crt_fscanf(std::FILE* f, const char* fmt, void* const* args);
int crt_sscanf(const char* s, const char* fmt, void* const* args);

// ---- the converter and its helpers (gensub.cpp) -------------------------------------------------

// The original never returns from FUN_14037e5c0 when a RZ or ELMCON row loop reaches the end of
// AX_BIL.KEY before all rows were read (fscanf keeps returning -1). The port throws this instead
// of hanging; the original's process would hang at that point with nothing more written.
struct InputHang : std::runtime_error {
    using std::runtime_error::runtime_error;
};

// Files the original leaves open for the rest of the process: AX_BIL.KEY after a converter
// error / the 4-node return, DEF_SUB.INI after a missing object. The port keeps them open too
// (that is why the original cannot delete AX_BIL.KEY afterwards); a test closes them with
// close_leaked_files() where the original process would end.
std::vector<std::FILE*>& leaked_files();
void close_leaked_files();

// Out-of-bounds reads of the original's MESH.DAT writer (STNCMP / MATAXI rows whose element
// index stayed 0, USRNOD rows past the C-entry array). The values it prints there come from heap
// memory outside the arrays and cannot be determined; the port prints 0.0 and counts them here.
std::int32_t& oob_reads();

// FUN_14037e5c0(model, in, out, &obj, &nodes, &elems, &meshNo): AX_BIL.KEY -> MESH.DAT.
// Returns 0, 1 (open failure / read error with DEF_CTRL.OUT message) or 4 (4-node ELMCON).
int convert_key_to_mesh(model::DB_STEP_DATA& m, const char* in_name, const char* out_name, std::int32_t* o_obj,
                        std::int32_t* o_nodes, std::int32_t* o_elems, const std::int32_t* mesh_no);

// FUN_1401b85d0: deletes the file if it can be opened; 1 when it does not exist or was deleted.
int delete_if_exists(const char* name);

// FUN_1401b83d0 / FUN_1401b8320: line reader (getc up to n - 1 characters including '\n');
// returns strlen(buf), 0 at the end of the file with nothing read.
std::size_t read_line(char* buf, int n, std::FILE* f);

} // namespace def::cmd::gensub
