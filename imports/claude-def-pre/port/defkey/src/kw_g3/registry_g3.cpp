// Registry handler columns 2 (KEY write of one pair's record) and 3 (address of a pair's table)
// for group 3. Packet kw-g3. Spec: docs/re/keywords/_common.md section 6.
//
// The original takes std::vector<int> by value ({pairIndex, master, slave}) and destroys it
// itself; the port takes the vector by value too. pairIndex >= 1 selects the pairIndex-th pair,
// otherwise the pair is found by (master, slave).
#include <defkey/kw_g3/kw_g3.hpp>

#include <cstdint>
#include <cstring>

#include <defmodel/layout/db_step_data.hpp>

namespace def::key::g3 {

namespace {

// part of every handler: the pair the vector names (may be NULL). Like the original, elements
// 1 and 2 are read without a size check when element 0 is < 1 (the column-2 handlers check
// size >= 2 first, the column-3 handlers do not check at all).
Cls_1401f46e0* pair_of(DB_STEP_DATA* m, const std::vector<int>& v)
{
    if (v[0] < 1) return pair_find(&m->inter_obj, v[1], v[2]); // 1401facf0
    return pair_nth(&m->inter_obj, v[0]);                       // 1401fac90
}

// pointer arithmetic on a possibly NULL pair, as the original does
DYNARY* table_at(Cls_1401f46e0* p, std::uintptr_t off)
{
    return reinterpret_cast<DYNARY*>(reinterpret_cast<std::uintptr_t>(p) + off);
}

} // namespace

// @src 1403cab90  FRCROT
int handler2_frcrot(std::FILE* f, DB_STEP_DATA* m, std::vector<int> v)
{
    if (v.size() < 2) return 3;
    return write_frcrot(f, pair_of(m, v));
}

// @src 1403cb3e0  IHTCOF (master/slave from the pair)
int handler2_ihtcof(std::FILE* f, DB_STEP_DATA* m, std::vector<int> v)
{
    if (v.size() < 2 || m == nullptr) return 3;
    Cls_1401f46e0* p = pair_of(m, v);
    write_ihtcof(m, f, p, p->u168, p->u16c);
    return 0;
}

// @src 1403cae30  SEPRES
int handler2_sepres(std::FILE* f, DB_STEP_DATA* m, std::vector<int> v)
{
    if (v.size() < 2) return 3;
    return write_sepres(f, pair_of(m, v));
}

// @src 1403cafb0  SEPDEN
int handler2_sepden(std::FILE* f, DB_STEP_DATA* m, std::vector<int> v)
{
    if (v.size() < 2) return 3;
    return write_sepden(f, m, pair_of(m, v));
}

// @src 1403cb1f0  INTRST
int handler2_intrst(std::FILE* f, DB_STEP_DATA* m, std::vector<int> v)
{
    if (v.size() < 2) return 3;
    return write_intrst(f, pair_of(m, v));
}

// @src 1403cb5f0  FRCFAC (master/slave from the vector; needs 3 elements)
int handler2_frcfac(std::FILE* f, DB_STEP_DATA* m, std::vector<int> v)
{
    if (v.size() < 3 || m == nullptr) return 3;
    const int ms = v[1];
    const int sl = v[2];
    write_frcfac(m, f, pair_of(m, v), ms, sl);
    return 0;
}

// @src 1403cba00  WMODEL
int handler2_wmodel(std::FILE* f, DB_STEP_DATA* m, std::vector<int> v)
{
    if (v.size() < 2) return 3;
    return write_wmodel(f, pair_of(m, v));
}

// @src 1403cbbd0  RGDCNT
int handler2_rgdcnt(std::FILE* f, DB_STEP_DATA* m, std::vector<int> v)
{
    if (v.size() < 2) return 3;
    return write_rgdcnt(f, pair_of(m, v));
}

// @src 1403cbdf0  CNTMTH
int handler2_cntmth(std::FILE* f, DB_STEP_DATA* m, std::vector<int> v)
{
    if (v.size() < 2) return 3;
    return write_cntmth(f, m, pair_of(m, v));
}

// @src 1403cac80  FRCROT table (+0x240), only for an existing pair
int handler3_frcrot(DB_STEP_DATA* m, const char*, std::vector<int> v, DYNARY** out)
{
    Cls_1401f46e0* p = pair_of(m, v);
    if (p != nullptr) *out = &p->u240;
    return 0;
}

// @src 1403cb520  IHTCOF table (+0x1b8); the original stores the address even for a NULL pair
int handler3_ihtcof(DB_STEP_DATA* m, const char*, std::vector<int> v, DYNARY** out)
{
    *out = table_at(pair_of(m, v), 0x1b8);
    return 0;
}

// @src 1403cb300  INTRST table (+0x2b8), only for an existing pair
int handler3_intrst(DB_STEP_DATA* m, const char*, std::vector<int> v, DYNARY** out)
{
    Cls_1401f46e0* p = pair_of(m, v);
    if (p != nullptr) *out = &p->u2b8;
    return 0;
}

// @src 1403cb740  FRCFAC table: friction 2 (+0x90) for the label "Fric Frac 2", else friction 1
int handler3_frcfac(DB_STEP_DATA* m, const char* label, std::vector<int> v, DYNARY** out)
{
    Cls_1401f46e0* p = pair_of(m, v);
    if (p != nullptr) {
        if (std::strcmp(label, "Fric Frac 2") == 0)
            *out = &p->u000.u090;
        else
            *out = &p->u000.u018;
    }
    return 0;
}

} // namespace def::key::g3
