// defcmd/brdext.hpp: packet p8-05, the border extraction behind BRDEXT and REMESH step 1
// (docs/re/commands/BRDEXT.md). Entry points: def::cmd::cmd_brdext (defcmd/cmd_p8_05.hpp) and
// def::cmd::brdext_object (defcmd/meshgen/p8_seams.hpp). The functions below are exposed for the
// tier-1 tests and for REMESH (p8-08).
#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <defcmd/meshgen/meshgen_base.hpp>

namespace def::model {
class LLOBJ_DATA;
}

namespace def::cmd::brdext {

// The one-byte local object of 14013eb80 (ctor 14013d8d0, dtor 14013d8f0: empty). Its byte is set
// to 0 (140158e40) and read by 140158d40 in 14013e230: 0 -> USRWIN.DAT is deleted.
struct Cls_14013d8d0 {
    std::uint8_t keep_usrwin = 0;
};

// One slot of the std::string array 14013d230 allocates with new[] (0x28 bytes, the VS2010
// basic_string layout: +0x00 the allocator's 8 bytes, which no constructor writes, +0x08 buffer,
// +0x18 size, +0x20 capacity). qsort swaps slots byte-wise; the comparator 14013ca60 reads the
// slot itself as a char* (see compare_suffix). `text` stands for bytes +0x08..+0x27.
struct SuffixSlot {
    char alloc_bytes[8];   // +0x00 never written in the original (heap contents of the new[] block)
    std::uint32_t index;   // stands for +0x08..+0x27: index of the slot's string
    std::byte pad[0x28 - 12];
};
static_assert(sizeof(SuffixSlot) == 0x28);

// @src 14013ca60  qsort comparator: builds std::string((const char*)slot) for both (the slot's own
// bytes from +0x00, not its text), abs(sscanf "%d") of each (1401b0870, 140484ab8); -1 / 0 / 1.
int compare_suffix(const void* a, const void* b);

// @src 1401b1630  list `pattern` with FindFirstFileA/FindNextFileA; every entry that is not a
// directory goes to `out` (push_back, 1401b3490). Returns 0, or -1 when nothing matches.
std::int32_t list_files(std::string pattern, std::vector<std::string>& out);

// @src 1401b1770  _getcwd(buf, 0x104), "." when it fails
void get_cwd(char* buf);

// @src 1401b17f0  fopen_s(name, "r") succeeds (the file is closed again)
bool file_readable(std::string name);

// @src 14013cc10  the suffixes of the *.DB files in dir (dir ends in '\') whose stem starts with
// prob (exact) and is not prob itself, sorted with 14013ca60 into arr[0..]; arr[k] = "" for every
// other k < number of files. Returns the first k whose abs(%d of arr[k]) >= abs(*step), else -1.
std::int32_t pick_suffix(const std::int32_t* step, const std::string& prob, std::string dir,
                         std::vector<std::string>& arr);

// @src 14013d230  DB lookup in cwd: found -> prob += suffix, db = cwd + "\" + prob + ".DB", true;
// not found -> both unchanged, false.
bool find_db(const std::int32_t* step, std::string& prob, std::string cwd, std::string& db);

// @src 14013d900  with a non-empty prob: DB lookup on a copy, then remove <name>.GEO, <name>.RES,
// NBCD.DAT
void remove_outputs(Cls_14013d8d0& self, std::string prob, const std::int32_t* step);

// @src 14013e230  write AMG_DXT.INI / AMG_DXT3.INI, launch AMG_DXT / AMG_DXT3, read <prob>.GEO
// back into obj. 0 ok, 1 failure.
std::uint32_t run_extraction(Cls_14013d8d0& self, std::string prob, const std::int32_t* step,
                             model::LLOBJ_DATA* obj);

// @src 14013dba0  checks, DB lookup, 3D DB version check, run_extraction, messages
bool extract(Cls_14013d8d0& self, std::string prob, std::string db, std::int32_t* step,
             model::LLOBJ_DATA* obj);

// @src 14013eb80  border-extraction driver of a bound _MeshGenBase; always true
bool extract_border(MeshGenBase& mg);

} // namespace def::cmd::brdext
