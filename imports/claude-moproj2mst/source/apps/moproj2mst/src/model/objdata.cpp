// Setters of the per-operation data that ParseKeywordText (0x140096390, P09) fills: TLOC, the
// InterObject block, OBJPOS records, state variables and SVSCAL (owner: P08, moved from P09 by the
// coordinator). Decompiles: s1-01 dc/kw0.c, open3.c, open5.c, acc0.c; gui_mo/out_p09 d4.c-d9.c.
#include "m2m/model.hpp"

namespace m2m {

// @src 0x1400abc90
void setTloc(Operation& op, double t) { op.tloc = t; }

// @src 0x1400ab6f0  (called on the ICB from 0x140065790; ICB+0x70 is OPI+0xe0)
void setLubSystem(Operation& op, std::uint8_t v) { op.lubSystem = v; }

// @src 0x140065270
void setIcbFlag(ContactBlock& c, int n, std::uint8_t v) { c.flag[static_cast<size_t>(n - 1)] = v; }

// @src 0x1400ab7c0
void setIcbEnable(ContactBlock& c, std::uint8_t v) { c.enable = v; }

// @src 0x1400ab7e0
void setIcbGen(ContactBlock& c, std::uint8_t v) { c.gen = v; }

// @src 0x1400658e0  The original leaves the doubles at +0x90 and +0xa8 as operator new returned
// them; no writer reads them, and here they are 0.
void initObjpos(ObjPos& p)
{
    p.list.clear();
    p.mode = 1;
    p.object = 0;
    p.i08 = 0;
    p.dbl.fill(0.0);   // +0x10..+0xc8, including the bytes +0xb0/+0xb1 and the int +0xb4
}

// @src 0x140061e30
ObjPos* newObjpos(Operation& op, std::unique_ptr<ObjPos> p)
{
    if (!p) {
        p = std::make_unique<ObjPos>();   // new 0xd8
        initObjpos(*p);
    }
    op.objpos.push_back(std::move(p));   // QList<ObjPos*>::append (OPI+0x60)
    return op.objpos.back().get();
}

// @src 0x1400abc40
void objposClearList(ObjPos& p) { p.list.clear(); }

// @src 0x1400abc60
void objposAppend(ObjPos& p, int v) { p.list.push_back(v); }

// ---- state variables (OBJ+0x30) ------------------------------------------------------------------

// @src 0x1400ab800
void svSetNdtmpFlag(StateVars& sv, std::uint8_t v) { sv.ndtmp.set = v; }
// @src 0x1400ab820
void svSetNdtmp(StateVars& sv, double v) { sv.ndtmp.v = v; }
// @src 0x1400ab840
void svSetStrainFlag(StateVars& sv, std::uint8_t v) { sv.strain.set = v; }
// @src 0x1400ab860
void svSetStrain(StateVars& sv, double v) { sv.strain.v = v; }
// @src 0x1400ab880
void svSetStressFlag(StateVars& sv, std::uint8_t v) { sv.stress.set = v; }
// @src 0x1400ab8a0
void svSetStress(StateVars& sv, double v) { sv.stress.v = v; }
// @src 0x1400ab8c0
void svSetDamageFlag(StateVars& sv, std::uint8_t v) { sv.damage.set = v; }
// @src 0x1400ab8e0
void svSetUrzA(StateVars& sv, double v) { sv.urz[0] = v; }
// @src 0x1400ab900
void svSetUrzB(StateVars& sv, double v) { sv.urz[1] = v; }
// @src 0x1400ab920
void svSetDrzB(StateVars& sv, double v) { sv.drz[1] = v; }
// @src 0x1400ab940
void svSetGrainFlag(StateVars& sv, std::uint8_t v) { sv.grain.set = v; }
// @src 0x1400ab960
void svSetDenstyFlag(StateVars& sv, std::uint8_t v) { sv.densty.set = v; }
// @src 0x1400ab980
void svSetDensty(StateVars& sv, double v) { sv.densty.v = v; }
// @src 0x1400ab9a0
void svSetAvgstrFlag(StateVars& sv, std::uint8_t v) { sv.avgstr.set = v; }
// @src 0x1400ab9c0
void svSetAvgstr(StateVars& sv, double v) { sv.avgstr.v = v; }
// @src 0x1400ab9e0
void svSetLmtstr(StateVars& sv, double v) { sv.lmtstr.v = v; }
// @src 0x1400aba00
void svSetSvscalFlag(StateVars& sv, std::uint8_t v) { sv.svscal = v; }

// ---- SVSCAL table (sv+0x108) ---------------------------------------------------------------------

// @src 0x14089b970  clamp the dims: dim0..2 below 0 -> 0, nvals below 1 -> 1
static void svTableClampDims(StateVars& sv)
{
    for (int k = 0; k < 3; ++k)
        if (sv.svTabDim[k] < 0) sv.svTabDim[k] = 0;
    if (sv.svTabDim[3] < 1) sv.svTabDim[3] = 1;
    sv.svCount = sv.svTabDim[0];
}

// @src 0x14089bc90  number of values of the table
static int svTableSize(const StateVars& sv)
{
    int n = sv.svTabDim[3];
    if (sv.svTabKind == 0) {
        const int id = sv.svTabId;
        if (sv.svTabDim[3] < 2 && id != 0xb5 && id != 0xa7 && id != 0xa8 && id != 0xaa && id != 0xab)
            n = 0;
        return n;
    }
    for (int k = 0; k < sv.svTabKind; ++k) n *= sv.svTabDim[k];
    if (n < 0) n = 0;
    return n;
}

// @src 0x14089b840  (0x14089b1f0 frees the old arrays; only the dim-0 axis is modelled, svX:
// the axes of dims 1 and 2 are not read by any writer)
static void svTableAllocate(StateVars& sv)
{
    sv.svX.clear();
    sv.svTable.clear();
    svTableClampDims(sv);
    for (int k = 0; k < sv.svTabKind; ++k)
        if (k == 0 && sv.svTabDim[0] > 0) sv.svX.assign(static_cast<size_t>(sv.svTabDim[0]), 0.0);
    int n = svTableSize(sv);
    if (n > 0) sv.svTable.assign(static_cast<size_t>(n), 0.0);
}

// @src 0x1400abab0  (the original reads with its own tokenizer state, mode 0, on the same text)
int svscalReadTable(StateVars& sv, KeyTokenizer& tok)
{
    if (sv.svTabKind == 0 && sv.svTabDim[3] < 2) return 0;
    svTableAllocate(sv);
    if (sv.svTabKind == 1) {
        for (int r = 0; r < sv.svTabDim[0]; ++r) {
            tok.tok(10, &sv.svX[static_cast<size_t>(r)], 0);
            const int nv = sv.svTabDim[3];
            for (int c = 0; c < nv; ++c) tok.tok(10, &sv.svTable[static_cast<size_t>(r * nv + c)], 0);
        }
    }
    return 0;
}

// @src 0x1400aba20
bool svscalSet(StateVars& sv, int a, int b, KeyTokenizer& tok)
{
    if (a == 1) sv.svLayout = 7;   // +0x118 (= table +0x10)
    sv.svCount = b;                // +0x124 (= table dim 0)
    sv.svTabDim[0] = b;
    return svscalReadTable(sv, tok) == 0;
}

}  // namespace m2m
