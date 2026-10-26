// Inter-material test used by the material KEY writer 1401c5ea0 (header: defmodel/gui_twins.hpp).
// Decompile: deform-rea-work/out/mdt3/f.c.
#include <defmodel/gui_twins.hpp>

#include <defmodel/layout/material.hpp>
#include <defmodel/layout/phase_pair.hpp>

namespace def::model {

// @src 140339ba0  pair (from, to) both in the mixture list of `mat` (pair active)
bool phase_pair_in_mixture(const PhasePair* pair, const Material* mat)
{
    bool from_in = false;
    bool to_in = false;
    if (mat->mstmtr_flag != 1) return false;
    if (pair->phastf_on != 1) return false;
    for (std::int32_t i = 0; i < mat->mstmtr_count; i = i + 1) {
        if (pair->from == mat->mstmtr_list[i]) from_in = true;
        if (pair->to == mat->mstmtr_list[i]) to_in = true;
    }
    return from_in && to_in;
}

} // namespace def::model
