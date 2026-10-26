// Calls from packet p1-03 (defdbio part) into functions other packets own and have not landed.
// One definition file per owner (ext_<owner>.cpp), bodies DEF_NOT_PORTED; the merge replaces
// each body with a call to the owner's function. Signatures follow the call sites in
// FUN_1401dc3f0 (all arguments by reference, Fortran style).
#pragma once

#include <cstdint>

namespace def::dbio::ext {

// ---- p1-10 (Fortran string helpers) -- ext_p1_10.cpp
// 140323bf0: copy the C string src into the n-character Fortran field dst, blank padded;
// returns the length passed on to the Fortran routines.
std::int32_t fun_140323bf0(char* dst, const char* src, std::int32_t n);
// 1403239f0: cut trailing blanks/tabs/CR/LF/NULs of s[0..n) with a terminator.
void fun_1403239f0(char* s, std::int32_t n);

// ---- p1-12 (DB signature check) -- ext_p1_12.cpp
// 1402248c0: nonzero when the file is a DEFORM DB; *err gets the read error.
std::int32_t fun_1402248c0(const char* name, std::int32_t* err);

// ---- p1-13 (datarm23 header and directory reads) -- ext_p1_13.cpp
// 1403d84b0: header read. Arguments in call order (1401dc760).
void fun_1403d84b0(char* fname, std::int32_t* len, char* title80, char* smi1, char* smi2,
                   char* smi3, char* smi4, std::int32_t* istart, std::int32_t* ismtyp,
                   std::int32_t* nrsets, std::int32_t* nxtrec, std::int32_t* family,
                   std::int32_t* version, std::int32_t* err, std::int32_t* print);
// 1403d96fc: directory read into words (n entries); arguments in call order (1401dca79).
void fun_1403d96fc(char* fname, std::int32_t* len, std::int32_t* nrsets, std::int32_t* words,
                   std::int32_t* cols, std::int32_t* version, std::int32_t* err,
                   std::int32_t* print);

} // namespace def::dbio::ext
