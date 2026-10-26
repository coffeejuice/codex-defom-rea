// p4-02b: function counts of the phase pairs (inter-material relations), the simulation control
// (with the furnace model) and the inter-object records.
#include <defdbio/gendb/counts_frame.hpp>

#include <defdbio/gendb/counts_ext.hpp>
#include <defmodel/dynary_init.hpp>
#include <defmodel/mesh2d/material_ops.hpp>
#include <defmodel/step/db_step.hpp>
#include <defrt/stdlib/refcount.hpp>

namespace def::dbio::gendb {

using model::field;

namespace {

DYNARY* tab(void* base, std::size_t off) { return reinterpret_cast<DYNARY*>(static_cast<char*>(base) + off); }

void c1(DYNARY* t, std::int32_t* n, std::int32_t* len)
{
    *n = *n + 1;
    *len = gendb_ext().fun_140130200(t) + *len;
}

// one function of length 140130160 without 140134120
void c2_plain(DYNARY* t, std::int32_t* n, std::int32_t* len)
{
    std::int32_t v = gendb_ext().fun_140130160(t);
    *n = *n + 1;
    *len = *len + v;
}

// one function of length 14012fff0 (3D table, no preparation)
void c3_plain(DYNARY* t, std::int32_t* n, std::int32_t* len)
{
    *n = *n + 1;
    *len = gendb_ext().fun_14012fff0(t) + *len;
}

std::int32_t at_least_1(std::int32_t v) { return v < 1 ? 1 : v; }

} // namespace

// ---- phase pairs -----------------------------------------------------------------------------

// @src 140336300  DB length of the kinetics data of a phase pair, by its model (+0x14)
std::int32_t phase_pair_kinetics_length(PhasePair* p)
{
    std::int32_t r = 0;
    switch (field<std::int32_t>(p, 0x14)) {
    case 0:
    case 1:
    case 2:
    case 3: {
        r = field<std::int32_t>(p, 0xb8);
        std::int32_t a = field<std::int32_t>(p, 0x94);
        std::int32_t b = field<std::int32_t>(p, 0x98);
        std::int32_t c = field<std::int32_t>(p, 0x9c);
        std::int32_t s = a + b + c;
        r = at_least_1(a) * at_least_1(b) * at_least_1(c) * at_least_1(r) + s + r;
        break;
    }
    case 4: {
        std::int32_t d = field<std::int32_t>(p, 0x238);
        std::int32_t a = field<std::int32_t>(p, 0x214);
        std::int32_t b = field<std::int32_t>(p, 0x218);
        std::int32_t c = field<std::int32_t>(p, 0x21c);
        r = a + b + c;
        r = at_least_1(d) * at_least_1(a) * at_least_1(b) * at_least_1(c) + r;
        break;
    }
    case 5:
        r = 0xb;
        break;
    case 6:
        r = field<std::int32_t>(p, 0x2e4) * 2 + 4;
        break;
    case 7:
        r = 6;
        break;
    case 8:
        r = 4;
        break;
    case 9:
        r = 7;
        break;
    case 10:
        r = field<std::int32_t>(p, 0x414) << 1;
        break;
    case 0xb: {
        r = field<std::int32_t>(p, 0x138);
        std::int32_t a = field<std::int32_t>(p, 0x114);
        std::int32_t b = field<std::int32_t>(p, 0x118);
        std::int32_t c = field<std::int32_t>(p, 0x11c);
        std::int32_t s = a + b + c;
        r = at_least_1(a) * at_least_1(b) * at_least_1(c) * at_least_1(r) + s + r;
        break;
    }
    case 0xc:
    case 0xd:
    case 0xe:
    case 0xf:
        r = field<std::int32_t>(p, 0x1c0);
        break;
    default:
        break;
    }
    return r;
}

// @src 140335de0  property-function counts of one phase pair (only when PHASTF != 0)
void phase_pair_counts(PhasePair* p, std::int32_t* n, std::int32_t* len)
{
    if (p->phastf == 0) {
        return;
    }
    std::int32_t v = phase_pair_kinetics_length(p);
    *n = *n + 1;
    *len = *len + v;
    if (tab(p, 0x478)->ndim == 1) {
        c1(tab(p, 0x478), n, len);
    }
    if (tab(p, 0x4f0)->ndim == 1) {
        c1(tab(p, 0x4f0), n, len);
    } else if (tab(p, 0x4f0)->ndim == 2) {
        gendb_ext().fun_140134120(tab(p, 0x4f0));
        c2_plain(tab(p, 0x4f0), n, len);
    }
    if (field<std::int32_t>(p, 0x5e4) != 0) {
        if (tab(p, 0x5e8)->ndim == 1) {
            c1(tab(p, 0x5e8), n, len);
        } else if (tab(p, 0x5e8)->ndim == 2) {
            c2_plain(tab(p, 0x5e8), n, len);
        }
    }
    if (p->phastf == 2) {
        if (tab(p, 0x568)->ndim == 1) {
            c1(tab(p, 0x568), n, len);
        } else if (tab(p, 0x568)->ndim == 2) {
            c2_plain(tab(p, 0x568), n, len);
        }
    }
    if (field<std::int32_t>(p, 0x660) != 0 && tab(p, 0x678)->ndim == 1) {
        c1(tab(p, 0x678), n, len);
    }
    if (tab(p, 0x6f0)->ndim == 1) {
        c1(tab(p, 0x6f0), n, len);
    } else if (tab(p, 0x6f0)->ndim == 2) {
        c2_plain(tab(p, 0x6f0), n, len);
    }
}

// @src 140339ec0  unlink p from the list and delete it (14033a990); false when not in the list
bool phase_pair_list_remove(PhasePairList* l, PhasePair* p)
{
    PhasePair* prev = l->head;
    PhasePair* cur = l->head;
    while (cur != nullptr && cur != p) {
        prev = cur;
        cur = cur->next;
    }
    if (cur == nullptr) {
        return false;
    }
    prev->next = cur->next;
    if (l->head == cur) {
        l->head = cur->next;
    }
    if (l->tail == cur) {
        l->tail = prev;
    }
    if (cur != nullptr) {
        delete cur; // 14033a990(cur, 1)
    }
    l->count = l->count - 1;
    return true;
}

// @src 14033a1d0  remove the pairs whose materials are missing, and the PHASTF pairs of a mixture
void phase_pair_list_prune(PhasePairList* l)
{
    DB_STEP_DATA* owner = l->owner;
    PhasePair* p = l->head;
    while (p != nullptr) {
        bool drop = false;
        if (p->from < 1 || p->to < 1) {
            drop = true;
        } else {
            Material* a = model::material_list_find(&owner->materials, p->from);
            Material* b = model::material_list_find(&owner->materials, p->to);
            if (a == nullptr || b == nullptr) {
                drop = true;
            } else if ((model::material_is_mixture(a) || model::material_is_mixture(b)) &&
                       p->phastf != 0) {
                drop = true;
            }
        }
        if (drop) {
            PhasePair* next = p->next;
            phase_pair_list_remove(l, p);
            p = next;
        } else {
            p = p->next;
        }
    }
}

// @src 14033a300  pairs with both materials set and PHASTF != 0
std::int32_t phase_pair_list_active(PhasePairList* l)
{
    std::int32_t n = 0;
    for (PhasePair* p = l->head; p != nullptr; p = p->next) {
        if (0 < p->from && 0 < p->to && p->phastf != 0) {
            n = n + 1;
        }
    }
    return n;
}

// @src 14033a370  function counts of the pairs whose two materials are used
void phase_pair_list_function_counts(PhasePairList* l, std::int32_t* n, std::int32_t* len,
                                     std::int32_t*)
{
    for (PhasePair* p = l->head; p != nullptr; p = p->next) {
        Material* a = model::step_material_by_id(*l->owner, p->from);
        Material* b = model::step_material_by_id(*l->owner, p->to);
        if (a != nullptr && b != nullptr && field<std::int32_t>(a, 4) != 0 &&
            field<std::int32_t>(b, 4) != 0) { // 1401b0620 twice
            phase_pair_counts(p, n, len);
        }
    }
}

// ---- simulation control ----------------------------------------------------------------------

// @src 1401ee650  simulation-control functions: furnace or ENVTMP, CNVCOF, ENVATM, ACVCOF,
// DTTYPE, the TIMEID/ERECID/NRECID lists, VFACTR, FRQNCY
void sim_ctrl_counts(Cls_1401e7b10* s, std::int32_t* n, std::int32_t* len)
{
    if (s->furnace_on == 0) {
        if (s->envtmp.ndim != 0) {
            *n = *n + 1;
            *len = *len + model::dynary_value_count(&s->envtmp) * 2;
        }
    } else {
        furnace_counts(&s->furnace, n, len);
    }
    if (s->cnvcof.ndim != 0) {
        *n = *n + 1;
        *len = *len + model::dynary_value_count(&s->cnvcof) * 2;
    }
    if (s->envatm.ndim != 0) {
        *n = *n + 1;
        *len = *len + model::dynary_value_count(&s->envatm) * 2;
    }
    if (s->acvcof.ndim != 0) {
        *n = *n + 1;
        *len = *len + model::dynary_value_count(&s->acvcof) * 2;
    }
    if (s->dttype.ndim != 0) {
        *n = *n + 1;
        *len = *len + model::dynary_value_count(&s->dttype) * 2;
    }
    if (0 < s->timeid.n[0] && 0 < s->timeid.ncomp) {
        dynary_count_plus10(&s->timeid, n, len);
    }
    if (0 < s->erecid.n[0] && 0 < s->erecid.ncomp) {
        dynary_count_plus10(&s->erecid, n, len);
    }
    if (0 < s->nrecid.n[0] && 0 < s->nrecid.ncomp) {
        dynary_count_plus10(&s->nrecid, n, len);
    }
    if (s->vfactr_mode == 1 && 0 < s->vfactr_n) {
        *n = *n + 1;
        *len = gendb_ext().fun_140130290(&s->vfactr) + *len;
    }
    if (0 < s->frqncy_rows && 0 < s->frqncy_cols) {
        *n = *n + 1;
        *len = gendb_ext().fun_140130290(&s->frqncy) + *len;
    }
}

// @src 1401f3f50  recompute SMODE (1401f3f80) and return it
std::int32_t sim_ctrl_smode(Cls_1401e7b10* s)
{
    gendb_ext().fun_1401f3f80(s);
    return s->smode;
}

// @src 140375dc0  furnace model: one header function of 0x14 words, then each curve (1401304e0)
void furnace_counts(Cls_1403753f0* f, std::int32_t* n, std::int32_t* len)
{
    *n = *n + 1;
    *len = *len + 0x14;
    std::int32_t cnt = f->curve_count();
    for (std::int32_t i = 1; i <= cnt; i = i + 1) {
        DYNARY* t = f->curves.at(i);
        if (t != nullptr) {
            dynary_count_plus10(t, n, len);
        }
    }
}

// ---- inter-object records --------------------------------------------------------------------

// @src 1401fa170  counts of the inter-object record of each object pair (i <= j); DB_WRITE_VAR
// +0xe4 = 1 when a record has a contact type set (+0x340)
void inter_object_list_counts(Cls_1401fa020* l, std::int32_t* n, std::int32_t* len,
                              std::int32_t* nwin, std::int32_t* lwin)
{
    GendbExt& x = gendb_ext();
    DB_STEP_DATA* owner = l->owner;
    std::int32_t any = 0;
    std::int32_t nobj = object_count_field(owner);
    for (std::int32_t i = 1; i <= nobj; i = i + 1) {
        LLOBJ_DATA* a = x.fun_14017aaf0(&owner->objects, i); // 1401d6ea0
        if (a == nullptr || field<std::int32_t>(a, 0xc) == 7) {
            continue;
        }
        for (std::int32_t j = i; j <= nobj; j = j + 1) {
            LLOBJ_DATA* b = x.fun_14017aaf0(&owner->objects, j);
            if (b == nullptr || field<std::int32_t>(b, 0xc) == 7) {
                continue;
            }
            Cls_1401f46e0* r = x.fun_1401facf0(l, i, j);
            if (r != nullptr) {
                inter_object_counts(r, n, len, nwin, lwin, &any);
            }
        }
    }
    set_u0e4(owner, &any);
}

// @src 1401f98d0  number of contact windows (list at +0x188, count +0x14)
std::int32_t inter_object_window_count(Cls_1401f46e0* r) { return field<std::int32_t>(r, 0x188 + 0x14); }

// @src 1401f49c0  functions of one inter-object record (only when +0x170 is set), then its
// contact windows (14025d3b0)
void inter_object_counts(Cls_1401f46e0* r, std::int32_t* n, std::int32_t* len, std::int32_t* nwin,
                         std::int32_t* lwin, std::int32_t* any)
{
    if (field<std::int32_t>(r, 0x170) == 0) {
        return;
    }
    DB_STEP_DATA* owner = field<DB_STEP_DATA*>(r, 0x398);
    if (tab(r, 0x18)->ndim == 1) {
        c1(tab(r, 0x18), n, len);
    } else if (tab(r, 0x18)->ndim == 3) {
        c3_plain(tab(r, 0x18), n, len);
    }
    if (field<std::int32_t>(r, 0) == 4) {
        if (tab(r, 0x90)->ndim == 1) {
            c1(tab(r, 0x90), n, len);
        } else if (tab(r, 0x90)->ndim == 3) {
            c3_plain(tab(r, 0x90), n, len);
        }
    }
    if (rt::dimension_is_2(owner) && tab(r, 0x240)->ndim == 1) {
        c1(tab(r, 0x240), n, len);
    }
    if (tab(r, 0x1b8)->ndim == 1) {
        c1(tab(r, 0x1b8), n, len);
    }
    if (tab(r, 0x2b8)->ndim == 1) {
        c1(tab(r, 0x2b8), n, len);
    }
    std::int32_t ct = field<std::int32_t>(r, 0x340);
    if (ct != 0) {
        *any = 1;
    }
    if (ct == 1) {
        *n = *n + 1;
        *len = *len + 4;
    } else if (ct == 2) {
        *n = *n + 1;
        *len = *len + 2;
    }
    std::int32_t cnt = inter_object_window_count(r);
    for (std::int32_t k = 0; k < cnt; k = k + 1) {
        void* w = gendb_ext().fun_140269900(reinterpret_cast<char*>(r) + 0x188, k + 1);
        if (w != nullptr) {
            gendb_ext().fun_14025d3b0(w, n, len, nwin, lwin, owner);
        }
    }
}

} // namespace def::dbio::gendb
