// p1-03 (defdbio) calls into other packets (see ext.hpp), wired to the owners at the Phase 1
// gate: p1-10's Fortran string helpers (in defrt), p1-12's DB signature check and p1-13's
// header and directory reads.
#include "ext.hpp"

#include <defdbio/dbcore/dbops.hpp>
#include <defdbio/resultset/db_result_sets.hpp>
#include <defrt/ftnstr/ftnstr.hpp>

#include <cstring>

namespace def::dbio::ext {

// part of @src 140323bf0  C string to an n-character Fortran field
std::int32_t fun_140323bf0(char* dst, const char* src, std::int32_t n)
{
    return static_cast<std::int32_t>(rt::to_fortran(dst, src, n));
}

// part of @src 1403239f0  trim a Fortran field
void fun_1403239f0(char* s, std::int32_t n) { rt::trim_fortran(s, n); }

// part of @src 1402248c0  DEFORM DB signature check
std::int32_t fun_1402248c0(const char* name, std::int32_t* err) { return dbcore::is_deform_db(name, err); }

// part of @src 1403d84b0  header read; the outputs are written only when the header was read
void fun_1403d84b0(char* fname, std::int32_t* len, char* title80, char* smi1, char* smi2,
                   char* smi3, char* smi4, std::int32_t* istart, std::int32_t* ismtyp,
                   std::int32_t* nrsets, std::int32_t* nxtrec, std::int32_t* family,
                   std::int32_t* version, std::int32_t* err, std::int32_t* print)
{
    resultset::DbHeader h;
    resultset::read_db_header(fname, len, &h, err, print);
    if (*err != 0) {
        return;
    }
    std::memcpy(title80, h.title, sizeof h.title);
    std::memcpy(smi1, h.smi1, sizeof h.smi1);
    std::memcpy(smi2, h.smi2, sizeof h.smi2);
    std::memcpy(smi3, h.smi3, sizeof h.smi3);
    std::memcpy(smi4, h.smi4, sizeof h.smi4);
    *istart = h.istart;
    *ismtyp = h.ismtyp;
    *nrsets = h.nrsets;
    *nxtrec = h.nxtrec;
    *family = h.family;
    *version = h.version;
}

// part of @src 1403d96fc  result-set directory read
void fun_1403d96fc(char* fname, std::int32_t* len, std::int32_t* nrsets, std::int32_t* words,
                   std::int32_t* cols, std::int32_t* version, std::int32_t* err, std::int32_t* print)
{
    resultset::read_result_sets(fname, len, nrsets, words, cols, version, err, print);
}

} // namespace def::dbio::ext
