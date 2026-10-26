// Inter-object record operations (packet kw-g3): pair-list add/find/nth, mirror copy, DYNARY copy.
// Declarations and ownership: defmodel/window/inter_object_ops.hpp.
#include <defmodel/window/inter_object_ops.hpp>

#include <cstring>

#include <defmodel/dynary_init.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/window/window_list.hpp>

namespace def::model {

// 1400015e0 (owned by p1-01): model dimension == 2
bool model_is_2d(const DB_STEP_DATA* m) { return m->dimension == 2; }
// 1400015f0 (owned by p1-01): model dimension == 3
bool model_is_3d(const DB_STEP_DATA* m) { return m->dimension == 3; }

// @src 1400015d0  list count (+0x10)
std::int32_t pair_count(const Cls_1401fa020* l) { return l->count; }

// @src 1401faaa0  append a new record (operator new 0x3a0, owner = list owner)
Cls_1401f46e0* pair_append(Cls_1401fa020* l)
{
    Cls_1401f46e0* p = new Cls_1401f46e0(l->owner); // 1404797d8 operator new(0x3a0), 1401f46e0
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

// @src 1401fac90  n-th record (1-based; n < 1 gives the head)
Cls_1401f46e0* pair_nth(Cls_1401fa020* l, std::int32_t n)
{
    std::int32_t i = 0;
    Cls_1401f46e0* p = l->head;
    while (p != nullptr && (i = i + 1, i < n)) p = p->next;
    return p;
}

// @src 1401facf0  find by (master +0x168, slave +0x16c)
Cls_1401f46e0* pair_find(Cls_1401fa020* l, std::int32_t master, std::int32_t slave)
{
    Cls_1401f46e0* p = l->head;
    while (p != nullptr && (p->u168 != master || p->u16c != slave)) p = p->next;
    return p;
}

// @src 1401f9e80  copy everything from src, keeping dst's master, slave and direction
void pair_copy_keep_ids(Cls_1401f46e0* dst, const Cls_1401f46e0* src)
{
    const std::int32_t m = dst->u168;
    const std::int32_t s = dst->u16c;
    const std::int32_t d = dst->u170;
    pair_copy(dst, src);
    dst->u168 = m;
    dst->u16c = s;
    dst->u170 = d;
}

// @src 1401f9900  record copy
void pair_copy(Cls_1401f46e0* dst, const Cls_1401f46e0* src)
{
    dst->u168 = src->u168;
    dst->u16c = src->u16c;
    dst->u170 = src->u170;
    if (model_is_3d(dst->owner)) dst->u174 = src->u174;
    dst->u178 = src->u178;
    dst->u180 = src->u180;
    friction_copy(&dst->u000, &src->u000); // 1401f9d50
    ihtcof_copy(dst, src);                 // 1401f9e10
    if (model_is_2d(dst->owner)) {
        dst->u238 = src->u238;
        dynary_copy(&dst->u240, &src->u240); // 14012eff0
    }
    winlist_copy(&dst->u188, &src->u188); // 140269340
    // 0x108..0x160: friction parameter pairs, in the original's order
    Cls_1401fb020& df = dst->u000;
    const Cls_1401fb020& sf = src->u000;
    df.u108[0] = sf.u108[0];   // 0x108
    df.u108[1] = sf.u108[1];   // 0x110
    df.u108[6] = sf.u108[6];   // 0x138
    df.u108[7] = sf.u108[7];   // 0x140
    df.u108[2] = sf.u108[2];   // 0x118
    df.u108[3] = sf.u108[3];   // 0x120
    df.u108[8] = sf.u108[8];   // 0x148
    df.u108[9] = sf.u108[9];   // 0x150
    df.u108[4] = sf.u108[4];   // 0x128
    df.u108[5] = sf.u108[5];   // 0x130
    df.u108[10] = sf.u108[10]; // 0x158
    df.u108[11] = sf.u108[11]; // 0x160
    dynary_copy(&dst->u2b8, &src->u2b8); // 14012eff0
    dst->u330 = src->u330;
    dst->u334 = src->u334;
    dst->u338 = src->u338;
    dst->u340 = src->u340;
    for (int i = 0; i < 6; i = i + 1) dst->u348[i] = src->u348[i]; // 0x348..0x370
    dst->u378 = src->u378;
    dst->u37c = src->u37c;
    dst->u380 = src->u380;
    dst->u384 = src->u384;
    dst->u388 = src->u388;
}

// @src 1401f9d50  friction block copy (mode, fixed bytes, ids, types, both tables)
void friction_copy(Cls_1401fb020* dst, const Cls_1401fb020* src)
{
    dst->u000 = src->u000;
    dst->u004 = src->u004;
    dst->u005 = src->u005;
    dst->u008 = src->u008;
    dst->u00c = src->u00c;
    dst->u010 = src->u010;
    dst->u014 = src->u014;
    dynary_copy(&dst->u018, &src->u018); // 14012eff0
    dynary_copy(&dst->u090, &src->u090);
}

// @src 1401f9e10  IHTCOF copy (table and id)
void ihtcof_copy(Cls_1401f46e0* dst, const Cls_1401f46e0* src)
{
    dynary_copy(&dst->u1b8, &src->u1b8); // 14012eff0
    dst->u230 = src->u230;
    dst->u234 = src->u234;
}


// @src 14012eff0  table copy
void dynary_copy(DYNARY* dst, const DYNARY* src)
{
    dst->type = src->type;
    if (dst->type == 0x80) dst->u58 = src->u58;
    dst->value = src->value;
    dst->u54 = src->u54;
    dst->ndim = src->ndim;
    dst->ncomp = src->ncomp;
    dst->u50 = src->u50;
    for (std::int32_t i = 0; i < 3; i = i + 1) {
        dst->var[i] = src->var[i];
        dst->n[i] = src->n[i];
    }
    def::model::dynary_alloc(dst); // 14012fad0
    for (std::int32_t i = 0; i < dst->ndim && dst->axis[i] != nullptr && src->axis[i] != nullptr;
         i = i + 1) {
        std::memcpy(dst->axis[i], src->axis[i],
                    static_cast<std::size_t>(static_cast<std::int64_t>(dst->n[i]) << 3));
    }
    if (dst->values != nullptr && src->values != nullptr) {
        const std::int32_t total = def::model::dynary_value_count(dst);
        std::memcpy(dst->values, src->values,
                    static_cast<std::size_t>(static_cast<std::int64_t>(total) << 3));
    }
}


} // namespace def::model
