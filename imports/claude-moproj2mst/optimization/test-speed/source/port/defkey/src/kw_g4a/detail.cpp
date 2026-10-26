#include "kw_g4a/detail.hpp"

#include "kw_g4a/io.hpp"

#include <defmodel/layout/dynary.hpp>

namespace def::key::g4a::detail {

// part of @src 140384a50  (inlined in cases 0x2 and 0xe)
int read_nvar_table(std::FILE* fp, std::int32_t* cnt, def::model::DYNARY* a, int type_a, def::model::DYNARY* b,
                    int type_b, def::model::DYNARY* c, int type_c)
{
    if (io::line_i(fp, cnt, false) != 0) return 2;
    io::dyn_reinit_keep(a, type_a);
    a->n[0] = *cnt;
    if (io::read_body(fp, a) != 0) return 2;
    // 32-bit int arithmetic of the original (wraps)
    std::uint32_t sum = 0;
    std::uint32_t prod = 1;
    for (std::int32_t i = 0; i < a->n[0]; ++i) {
        sum = sum + static_cast<std::uint32_t>(static_cast<std::int32_t>(a->axis[0][i]));
        prod = prod * static_cast<std::uint32_t>(static_cast<std::int32_t>(a->axis[0][i]));
    }
    io::dyn_reinit_keep(b, type_b);
    b->ncomp = static_cast<std::int32_t>(sum);
    if (io::read_body(fp, b) != 0) return 2;
    io::dyn_reinit_keep(c, type_c);
    c->ncomp = static_cast<std::int32_t>(prod);
    if (io::read_body(fp, c) != 0) return 2;
    return 0;
}

// part of @src 1403d0c90  (also inlined in 1403d22a0)
void write_nvar_table(std::FILE* fp, std::int32_t* cnt, def::model::DYNARY& a, const def::model::DYNARY& b,
                      const def::model::DYNARY& c)
{
    *cnt = a.n[0];
    io::kfprintf(fp, " %7i\n", *cnt);
    for (std::int32_t i = 0; i < a.n[0]; ++i)
        io::kfprintf(fp, " %7i %7i %7i\n", static_cast<std::int32_t>(a.axis[0][i]),
                     static_cast<std::int32_t>(a.values[i * 2]), static_cast<std::int32_t>(a.values[i * 2 + 1]));
    std::int32_t k = 0;
    for (std::int32_t i = 0; i < a.n[0]; ++i) {
        for (std::int32_t j = 0; j < static_cast<std::int32_t>(a.axis[0][i]); ++j) {
            io::kfprintf(fp, "%15.7E", b.values[k]);
            k = k + 1;
        }
        io::kfprintf(fp, "\n");
    }
    io::write_body(fp, &c, 1);
}

} // namespace def::key::g4a::detail
