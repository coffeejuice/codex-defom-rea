// Phase-pair list operations and DIM routing (packet kw-g57). Declarations: defmodel/phase/phase_ops.hpp.
#include <defmodel/phase/phase_ops.hpp>

#include <defmodel/layout/db_step_data.hpp>

namespace def::model {

// @src 1401b05f0  count (+0x10)
std::int32_t phase_pair_count(const PhasePairList* l) { return l->count; }

// @src 140339df0  append a new pair (operator new 0x780, ctor 140333f60 with the list owner)
PhasePair* phase_pair_append(PhasePairList* l)
{
    PhasePair* p = new PhasePair(l->owner);
    if (l->count == 0) {
        l->tail = p;
        l->head = p;
    } else {
        l->tail->next = p;
        l->tail = p;
    }
    l->count = l->count + 1;
    return p;
}

// @src 14033a040  find by (from +0x0, to +0x4)
PhasePair* phase_pair_find(PhasePairList* l, std::int32_t from, std::int32_t to)
{
    PhasePair* p = l->head;
    while (p != nullptr && (p->from != from || p->to != to)) p = p->next;
    return p;
}

// @src 140339fe0  n-th pair (1-based; n < 1 gives the head)
PhasePair* phase_pair_nth(PhasePairList* l, std::int32_t n)
{
    std::int32_t i = 0;
    PhasePair* p = l->head;
    while (p != nullptr && (i = i + 1, i < n)) p = p->next;
    return p;
}

// @src 1401d6450  number of pairs with PHASTF on -> DB_STEP_DATA +0x13f4
void count_active_phase_pairs(DB_STEP_DATA* m)
{
    m->n_phase_on = 0;
    for (std::int32_t i = 0; i < phase_pair_count(&m->phase_pairs); i = i + 1) {
        PhasePair* p = phase_pair_nth(&m->phase_pairs, i + 1);
        if (p != nullptr && p->phastf_on == 1) m->n_phase_on = m->n_phase_on + 1;
    }
}

// @src 1401d8120  companion (+0x33e8)
DB_STEP_DATA* companion_model(DB_STEP_DATA* m) { return m->companion; }

// @src 1401d81b0  the model groups 1-6 read into: the companion when DIM selected it
DB_STEP_DATA* keyword_target_model(DB_STEP_DATA* m)
{
    if (m->companion != nullptr && m->use_companion != 0) m = m->companion;
    return m;
}

// @src 1401d8050  force the companion to the other dimension (vtable slot 0 = set_dimension)
void sync_companion_dimension(DB_STEP_DATA* m)
{
    if (m->companion != nullptr) {
        if (m->dimension == 3 && !(m->companion->dimension == 2)) { // 1400015f0, 1400015e0
            std::int32_t d = 2;
            m->companion->set_dimension(&d);
        }
        if (m->dimension == 2 && !(m->companion->dimension == 3)) {
            std::int32_t d = 3;
            m->companion->set_dimension(&d);
        }
    }
}

// @src 1401d8140  DIM d: flag = (companion dimension == d), after syncing the companion
void set_dim_routing(DB_STEP_DATA* m, const std::int32_t* dim)
{
    if (m->companion != nullptr) {
        sync_companion_dimension(m);
        const std::int32_t cd = m->companion->dimension; // 140158e90
        if (cd == *dim)
            m->use_companion = 1;
        else
            m->use_companion = 0;
    }
}

} // namespace def::model
