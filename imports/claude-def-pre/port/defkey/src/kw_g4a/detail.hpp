// kw_g4a/detail.hpp: code that the original inlines twice (FSTRES and CREEP), kept once here.
#pragma once

#include <cstdint>
#include <cstdio>

namespace def::model {
class DYNARY;
}

namespace def::key::g4a::detail {

// N-variable table read (FSTRES t 13/15 in FUN_140384a50 case 0x2, CREEP t 7/8 in case 0xe):
// "%i" count line -> *cnt; A (type_a, n0 = cnt) rows "<k> <p> <q>"; B (type_b) ncomp = sum (int)k;
// C (type_c) ncomp = prod (int)k. Returns 0, or 2 on EOF (count line or any body).
int read_nvar_table(std::FILE* fp, std::int32_t* cnt, def::model::DYNARY* a, int type_a, def::model::DYNARY* b,
                    int type_b, def::model::DYNARY* c, int type_c);

// N-variable table write (FSTRES M 10/0xc in FUN_1403d0c90, CREEP M 7/8 in FUN_1403d22a0):
// *cnt = A.n0 (the writer updates the model), " %7i\n" count, rows " %7i %7i %7i\n" of
// (int)k, (int)p, (int)q, per row k values "%15.7E" from B then "\n", then C with body mode 1.
void write_nvar_table(std::FILE* fp, std::int32_t* cnt, def::model::DYNARY& a, const def::model::DYNARY& b,
                      const def::model::DYNARY& c);

} // namespace def::key::g4a::detail
