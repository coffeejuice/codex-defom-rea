// Line writers behind the P09 emitters (owner: P09): OBJPOS lines, per-object state variables,
// NRECVL/ERECVL recovery lists, SVSCAL and its table lookup. Spec: s1-02 6.3, 6.4; decompiles in
// s1-02 a2s.c/a4s.c/a9s.c and gui_mo/out_p09/d1.c (0x14089c2a0). Literals checked against the exe
// copy; "\t" is the 1-byte literal 14211f184 unless noted.
#include "m2m/mst/emit.hpp"

namespace m2m::mst {

namespace {

// The state-variable flags are 32-bit fields of which the writers test the low byte (MOV AL,[..]).
bool flagByte(std::int32_t f) { return (f & 0xff) != 0; }

// QList<int>::at of the OBJPOS id list, 1-based (0x140066700)
int objposListAt(const ObjPos& p, int j) { return p.list[static_cast<size_t>(j - 1)]; }

// OBJPOS id list tail of both line writers, then the line end.
void objposTail(W& w, const ObjPos& p)
{
    Stream& ts = w.ts;
    if (static_cast<int>(p.list.size()) > 0) {               // 0x1400666e0
        ts << "\t" << static_cast<int>(p.list.size()) << endl;
        for (int i = 0; i < static_cast<int>(p.list.size()); ++i) {
            ts << objposListAt(p, i + 1);
            if (i != static_cast<int>(p.list.size()) - 1) ts << "\t";
        }
    }
    ts << endl;
}

[[noreturn]] void undefinedRead(const char* what)
{
    throw Error(M2M_E_INPUT, std::string("input the original reads out of range: ") + what);
}

}  // namespace

// @src 0x140065f80
void emitObjposLine3D(W& w, const ObjPos& p)
{
    Stream& ts = w.ts;
    ts << "OBJPOS" /*1421249e8*/ << "\t" << p.object << "\t" << p.mode << "\t";
    switch (p.mode - 1) {
    case 0:
    case 10:
        ts << p.d(0x30) << "\t" << p.d(0x38) << "\t" << p.d(0x40) << "\t" << 0.0 << "\t" << 0.0
           << "\t" << 0.0 << "\t" << 0.0;
        break;
    case 1:
    case 0xb:
    case 0x65:
    case 0x6f:
        ts << p.d(0x10) << "\t" << p.d(0x18) << "\t" << p.d(0x20) << "\t" << p.d(0x28) << "\t"
           << p.i08 << "\t" << 0.0 << "\t" << 0.0;
        break;
    case 2:
        ts << p.d(0x48) << "\t" << p.d(0x50) << "\t" << p.d(0x58) << "\t" << p.d(0x60) << "\t"
           << p.d(0x68) << "\t" << p.d(0x70) << "\t" << p.d(0x78);
        break;
    case 5:
        ts << p.d(0x60) << "\t" << p.d(0x68) << "\t" << p.d(0x70) << "\t" << p.d(0x78) << "\t"
           << 0.0 << "\t" << 0.0 << "\t" << 0.0;
        break;
    case 6:
    case 7:
        ts << p.d(0x10) << "\t" << p.d(0x18) << "\t" << p.d(0x20) << "\t" << p.d(0x28) << "\t"
           << 0.0 << "\t" << 0.0 << "\t" << 0.0;
        break;
    case 8:
        ts << p.d(0x10) << "\t" << p.d(0x18) << "\t" << p.d(0x20) << "\t" << p.d(0x28) << "\t"
           << p.d(0x60) << "\t" << p.d(0x68) << "\t" << p.d(0x70);
        break;
    default:
        break;
    }
    objposTail(w, p);
}

// @src 0x140065af0
void emitObjposLine2D(W& w, const ObjPos& p)
{
    Stream& ts = w.ts;
    ts << "OBJPOS" /*1421249e8*/ << "\t" << p.object << "\t" << p.mode << "\t";
    switch (p.mode - 1) {
    case 0:
    case 10:
        ts << p.d(0x30) << "\t" << p.d(0x38) << "\t" << 0.0 << "\t" << 0.0;
        break;
    case 1:
    case 0xb:
    case 0x65:
    case 0x6f:
        ts << p.d(0x10) << "\t" << p.i08 << "\t" << p.d(0x18) << "\t" << p.d(0x20);
        break;
    case 2:
        ts << p.d(0x48) << "\t" << p.d(0x50) << "\t" << p.d(0x78) << "\t" << 0.0;
        break;
    case 3:
        ts << p.d(0x80) << "\t" << p.d(0x88) << "\t" << p.d(0x98) << "\t" << p.d(0xa0);
        break;
    case 5:
        ts << p.d(0x78) << "\t" << 0.0 << "\t" << 0.0 << "\t" << 0.0;
        break;
    default:
        break;
    }
    objposTail(w, p);
}

// @src 0x1400613b0
void emitObjectStateVars(W& w, Operation& sim)
{
    Stream& ts = w.ts;
    int count = objSlots(sim);
    for (int i = 0; i < count; ++i) {
        ObjEntry* o = objEntry(sim, i);
        if (!o) continue;
        if (!is3D(sim)) {                                    // 0x14005f1c0
            emitSv2D(w, o->sv);                              // 0x14004e1c0
            emitSvscal(w, o->sv);
        } else {
            emitSv3D(w, o->sv);                              // 0x14004e210
            emitSvscal(w, o->sv);
        }
        std::vector<RecoveryEntry>* A = recoveryListA(sim);  // 0x14005a400 (not null-checked)
        std::shared_ptr<std::vector<bool>> F = sim.recoveryFlags;   // 0x140970130 (may be null)
        std::vector<RecoveryEntry>* B = sim.recoveryB.get(); // 0x1401cc700 (not null-checked)
        if (!A || !B) undefinedRead("recovery list (null QList in the original)");
        int nA = static_cast<int>(A->size());
        int n0 = 0, n1 = 0;
        // entries by 0x140067100 (QList at(j) after a bounds check, then the entry pointer)
        for (int j = 0; j < nA; ++j) ((*A)[static_cast<size_t>(j)].type == 0 ? n0 : n1)++;
        for (int j = 0; j < static_cast<int>(B->size()); ++j)
            ((*B)[static_cast<size_t>(j)].type == 0 ? n0 : n1)++;

        // Flag of list A entry j: F[j] when F is non-empty, else the default.
        auto flagA = [&](int j, bool dflt) {
            bool f = dflt;
            if (F && !F->empty()) {                          // 0x140307410
                if (j >= static_cast<int>(F->size())) undefinedRead("recovery flag list");
                f = (*F)[static_cast<size_t>(j)];            // 0x14003f7e0
            }
            return f;
        };
        // One recovery block for entry type t with n entries: header, flags, values.
        auto block = [&](int t, int n, const char* kw, const char* sep[4], const char* tab[3],
                         bool dflt) {
            ts << kw << o->number /*0x14004d560*/ << tab[0] << 0 << tab[1] << 1 << tab[2] << n
               << endl;
            int k = 0;
            for (int j = 0; j < nA; ++j) {
                if ((*A)[static_cast<size_t>(j)].type != t) continue;
                ++k;
                bool f = flagA(j, dflt);
                if (k == n) ts << (f ? 1 : 0) << endl;
                else ts << (f ? 1 : 0) << sep[0];
            }
            for (int j = 0; j < static_cast<int>(B->size()); ++j) {
                if ((*B)[static_cast<size_t>(j)].type != t) continue;
                ++k;
                if (k == n) ts << 1 << endl;
                else ts << 1 << sep[1];
            }
            k = 0;
            for (int j = 0; j < nA; ++j) {
                if ((*A)[static_cast<size_t>(j)].type != t) continue;
                ++k;
                double v = (*A)[static_cast<size_t>(j)].value;   // BC+0x18
                if (k == n) ts << v << endl;
                else ts << v << sep[2];
            }
            for (int j = 0; j < static_cast<int>(B->size()); ++j) {
                if ((*B)[static_cast<size_t>(j)].type != t) continue;
                ++k;
                double v = (*B)[static_cast<size_t>(j)].value;
                if (k == n) ts << v << endl;
                else ts << v << sep[3];
            }
        };
        // Every type other than 0 counts as 1 above, but only type 1 entries are printed.
        if (n0 > 0) {
            const char* sep[4] = {",    " /*14212368c*/, ",    " /*142123694*/,
                                  ",    " /*14212369c*/, ",    " /*1421236a4*/};
            const char* tab[3] = {"\t" /*14212285c*/, "\t" /*14212286c*/, "\t" /*14212299c*/};
            block(0, n0, "NRECVL    \t" /*142123680*/, sep, tab, false);
        }
        if (n1 > 0) {
            const char* sep[4] = {",    " /*1421236bc*/, ",    " /*1421236c4*/,
                                  ",    " /*1421236cc*/, ",    " /*1421236d4*/};
            const char* tab[3] = {"\t" /*1421229ac*/, "\t" /*142122a64*/, "\t" /*142122a74*/};
            block(1, n1, "ERECVL    \t" /*1421236b0*/, sep, tab, true);
        }
    }
}

// @src 0x14004e830
void emitSv2D(W& w, const StateVars& sv)
{
    Stream& ts = w.ts;
    const int no = sv.objNumber;                             // +0x104
    if (flagByte(sv.ndtmp.set))
        ts << "NDTMP    \t" /*14211f188*/ << no << "\t" << 0 << "\t" << sv.ndtmp.v << endl;
    if (flagByte(sv.strain.set))
        ts << "STRAIN    \t" /*14211f178*/ << no << "\t" << 0 << "\t" << sv.strain.v << endl;
    if (flagByte(sv.stress.set))
        ts << "STRESS    \t" /*14211f168*/ << no << "\t" << 0 << "\t" << sv.stress.v << endl;
    if (flagByte(sv.damage.set))
        ts << "DAMAGE    \t" /*14211f158*/ << no << "\t" << 0 << "\t" << sv.damage.v << endl;
    if (flagByte(sv.urzSet))
        ts << "URZ    \t" /*14211f148*/ << no << "\t" << 0 << "\t" << sv.urz[0] << "\t" << sv.urz[1]
           << "\t" << endl;
    if (flagByte(sv.drzSet))
        ts << "DRZ    \t" /*14211f138*/ << no << "\t" << 0 << "\t" << sv.drz[0] << "\t" << sv.drz[1]
           << "\t" << endl;
    if (flagByte(sv.grain.set))
        ts << "GRAIN    \t" /*14211f128*/ << no << "\t" << 0 << "\t" << 16 << "\t" << 0 << "\t"
           << sv.grain.v << "\t" << sv.grain.v << endl;
    if (flagByte(sv.sizesh.set))
        ts << "SIZESH    \t" /*14211f118*/ << no << "\t" << sv.sizesh.v << "\t" << "0" /*14211f114*/
           << "\t" << "0" << "\t" << "0" << endl;
    if (flagByte(sv.densty.set))
        ts << "DENSTY    \t" /*14211f108*/ << no << "\t" << 0 << "\t" << sv.densty.v << endl;
    if (flagByte(sv.avgstr.set))
        ts << "AVGSTR    \t" /*14211f0f8*/ << no << "\t" << sv.avgstr.v << endl;
    if (flagByte(sv.lmtstr.set))
        ts << "LMTSTR    \t" /*14211f0e8*/ << no << "\t" << sv.lmtstr.v << endl;
}

// @src 0x14004f1c0
void emitSv3D(W& w, const StateVars& sv)
{
    Stream& ts = w.ts;
    const int no = sv.objNumber;
    if (flagByte(sv.ndtmp.set))
        ts << "NDTMP    \t" /*14211f188*/ << no << "\t" << 0 << "\t" << sv.ndtmp.v << endl;
    if (flagByte(sv.strain.set))
        ts << "STRAIN    \t" /*14211f178*/ << no << "\t" << 0 << "\t" << sv.strain.v << endl;
    if (flagByte(sv.stress.set))
        ts << "STRESS    \t" /*14211f168*/ << no << "\t" << 0 << "\t" << sv.stress.v << endl;
    if (flagByte(sv.damage.set))
        ts << "DAMAGE    \t" /*14211f158*/ << no << "\t" << 0 << "\t" << sv.damage.v << endl;
    if (flagByte(sv.urzSet))
        ts << "URZ    \t" /*14211f148*/ << no << "\t" << 0 << "\t" << sv.urz[0] << "\t" << sv.urz[1]
           << "\t" << sv.urz[2] << endl;
    if (flagByte(sv.drzSet))
        ts << "DRZ    \t" /*14211f138*/ << no << "\t" << 0 << "\t" << sv.drz[0] << "\t" << sv.drz[1]
           << "\t" << sv.drz[2] << endl;
    if (flagByte(sv.grain.set))
        ts << "GRAIN    \t" /*14211f128*/ << no << "\t" << 0 << "\t" << 16 << "\t" << 0 << "\t" << 0
           << "\t" << sv.grain.v << endl;
    if (flagByte(sv.sizesh.set))
        ts << "SIZESH    \t" /*14211f118*/ << no << "\t" << sv.sizesh.v << "\t" << "0" /*14211f114*/
           << "\t" << "0" << "\t" << "0" << endl;
    if (flagByte(sv.densty.set))
        ts << "DENSTY    \t" /*14211f108*/ << no << "\t" << 0 << "\t" << sv.densty.v << endl;
    if (flagByte(sv.avgstr.set))
        ts << "AVGSTR    \t" /*14211f0f8*/ << no << "\t" << sv.avgstr.v << endl;
    if (flagByte(sv.lmtstr.set))
        ts << "LMTSTR    \t" /*14211f0e8*/ << no << "\t" << sv.lmtstr.v << endl;
    if (sv.nrecvlFlag != 0 && sv.nrecvlCount > 0)
        ts << "NRECVL    \t" /*14211f1c0*/ << no << "\t0\t0\t" /*14211f1b4*/ << sv.nrecvlCount << "\t"
           << static_cast<int>(sv.nrecvlFlag) << endl;
    if (sv.erecvlFlag != 0 && sv.erecvlCount > 0)
        ts << "ERECVL    \t" /*14211f1a8*/ << no << "\t0\t0\t" /*14211f1b4*/ << sv.erecvlCount << "\t"
           << sv.erecvlValue << endl;
}

// @src 0x14004f050
void emitSvscal(W& w, const StateVars& sv)
{
    Stream& ts = w.ts;
    if (!sv.svscal /*0x14004f1a0: +0x180*/) return;
    ts << "SVSCAL    \t" /*14211f198*/ << sv.objNumber << "\t" << 1 << "\t" << sv.svCount << endl;
    for (int j = 0; j < sv.svCount; ++j) {
        double y = svTableValue(sv, j, 0, 0, 0);
        if (j >= static_cast<int>(sv.svX.size())) undefinedRead("SVSCAL x values");
        ts << sv.svX[static_cast<size_t>(j)] /*+0x130*/ << "\t" << y << endl;
    }
}

// @src 0x14089c490
double svTableValue(const StateVars& sv, int i, int j, int k, int l)
{
    int idx = svTableIndex(sv, i, j, k, l);
    if (sv.svTable.empty() /*+0x48 null*/ || idx < 0) return 0.0;
    if (idx >= static_cast<int>(sv.svTable.size())) undefinedRead("SVSCAL table");
    return sv.svTable[static_cast<size_t>(idx)];
}

// @src 0x14089c2a0
int svTableIndex(const StateVars& sv, int i, int j, int k, int l)
{
    auto atLeast1 = [](int v) { return v < 1 ? 1 : v; };
    const int kind = sv.svTabKind;                           // +0x04
    const int d1 = sv.svTabDim[0], d2 = sv.svTabDim[1], d3 = sv.svTabDim[2], d4 = sv.svTabDim[3];
    if (kind == 0 || i < 0) return -1;
    if (!(i < atLeast1(d1)) || j < 0) return -1;
    if (!(j < atLeast1(d2)) || k < 0) return -1;
    if (!(k < atLeast1(d3)) || l < 0) return -1;
    if (!(l < atLeast1(d4))) return -1;
    if (kind != 1) {
        int a = atLeast1(d1), b = atLeast1(d2), c = atLeast1(d3);
        return j * a + i + l * a * b * c + k * a * b;
    }
    return i * d4 + l;
}

}  // namespace m2m::mst
