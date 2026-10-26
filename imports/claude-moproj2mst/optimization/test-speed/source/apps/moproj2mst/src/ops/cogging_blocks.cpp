// SfOprCogging block writers PHT, PDWL, BDEF, BDCT, BDW, BHT, SHT (s1-03 4.3-4.8).
// Every "\t" is its own literal in the original; ints and doubles stream as in the original
// (`<< 0` an int, `<< 0.0` a double), so the number text comes from the same QTextStream overload.
#include "cogging.hpp"

#include "m2m/qt4path.hpp"

namespace m2m::ops::cog {

using mst::endl;

namespace {

// Block head of PHT/PDWL/BDEF (first: next block is the operation's first; firstOp: 0x140050bc0).
void headFirst(Blk& k, int p, bool* first, bool* firstOp)
{
    mst::emitSimHeader(k.w, *k.sim, k.op);                               // 0x140131c10
    if (!*first) {
        mst::emitDbread(k.w, k.db, *k.sim, k.dbStep);                     // 0x1401343e0
        mst::emitSimCtrl(k.w, k.prj, k.db, p, k.op);                      // 0x140134bf0
    } else {
        *first = false;
        if (!*firstOp) mst::emitDbread(k.w, k.db, *k.sim, k.dbStep);
        mst::emitSimCtrl(k.w, k.prj, k.db, p, k.op);
        mst::emitMaterials(k.w, k.prj, k.db, p, k.op);                    // 0x140134db0
        mst::emitMeshSettings(k.w, k.prj, k.db, p, k.op);                 // 0x1401360e0
        mst::emitObjects(k.w, k.prj, k.db, p, k.op, false);               // 0x140135220
        mst::emitContact(k.w, k.prj, k.db, p, k.op);                      // 0x140135da0
    }
}

// Block head of BDCT/BDW/BHT/SHT.
void head2(Blk& k, int p)
{
    mst::emitSimHeader(k.w, *k.sim, k.op);
    mst::emitDbread(k.w, k.db, *k.sim, k.dbStep);
    mst::emitSimCtrl(k.w, k.prj, k.db, p, k.op);
}

// Block tail: KFREAD user key, SIMNAM, OPRNAM, CURSIM, GENDB.
void tail(Blk& k, const String& name)
{
    mst::emitKfreadUser(k.w, k.op);                                      // 0x140134680
    mst::emitSimnam(k.w, name);                                          // 0x1401347e0
    mst::emitOprnam(k.w, k.op->name);                                    // 0x140134730 (0x140050ac0)
    mst::emitCursim(k.w, *k.sim, flowNumber(*k.op), 0);                  // 0x140134900 (0x140050b20, 0x140050b10)
    mst::emitGendb(k.w, k.db, *k.sim, 2, false);                         // 0x140134a10
}

// QString(fmt).arg(r, 0, 10, ' ').arg(p, 0, 10, ' ')[.arg(b, 0, 10, ' ')]
String blockName(const char* fmt, int r, int p)
{
    return qt::arg(qt::arg(qt::L(fmt), r, 0, 10, u' '), p, 0, 10, u' ');
}
String blockName(const char* fmt, int r, int p, int b)
{
    return qt::arg(blockName(fmt, r, p), b, 0, 10, u' ');
}

// Manipulator start lines (the two "13" records and their partners).
void manipStart(mst::Stream& out, double v)
{
    out << "OBJPOS" << "\t" << 4 << "\t" << 13 << "\t" << 0 << "\t" << -1 << "\t" << 0.0 << "\t" << 0.0
        << "\t" << 1 << "\t" << 2 << "\t" << v << "\t" << 1 << endl;
    out << 5 << endl;
    out << "OBJPOS" << "\t" << 6 << "\t" << 13 << "\t" << 0 << "\t" << 1 << "\t" << 0.0 << "\t" << 0.0
        << "\t" << 1 << "\t" << 2 << "\t" << v << "\t" << 1 << endl;
    out << 7 << endl;
}

// v = manip.StartPos + manip.Length / 2.0
double manipStartPos(const CogModel& m)
{
    return m.cog.manip.startPos + m.cog.manip.length / 2.0;
}

// PHT/PDWL: first block of a pass writes the manipulator start and contact lines.
void manipFirstPass(Blk& k, bool* manipFirst)
{
    if (!*manipFirst) return;
    *manipFirst = false;
    if (!useManip(k.m)) return;
    mst::Stream& out = k.w.ts;
    manipStart(out, manipStartPos(k.m));
    out << "OBJPOS" << "\t" << 4 << "\t" << 14 << "\t" << 5 << "\t" << 0 << "\t" << 0.001 << "\t" << 1 << endl;
    out << "OBJPOS" << "\t" << 6 << "\t" << 14 << "\t" << 7 << "\t" << 0 << "\t" << 0.001 << "\t" << 1 << endl;
    out << "OBJPOS" << "\t" << 4 << "\t" << 14 << "\t" << 5 << "\t" << 1 << "\t" << 0.001 << "\t" << 1 << endl;
    out << "OBJPOS" << "\t" << 6 << "\t" << 14 << "\t" << 7 << "\t" << 1 << "\t" << 0.001 << "\t" << 1 << endl;
}

// Die retraction: "OBJPOS \t<id>\t11\t<x>\t<y>\t<z>" for every die the filter accepts.
template <class F>
void retract(Blk& k, F accept)
{
    mst::Stream& out = k.w.ts;
    const int nD = dieCount(k.m);
    const double half = -cogSize(k.m.cog, 1, 2) / 2.0;   // sign flip 0x1421d3d30, then / 2.0
    for (int i = 0; i < nD; i++) {
        if (!accept(dieId(i))) continue;
        double d[3];
        dieDir(i, d);
        const double z = half * d[2], y = half * d[1], x = half * d[0];
        out << "OBJPOS " << "\t" << dieId(i) << "\t" << 11 << "\t" << x << "\t" << y << "\t" << z << endl;
    }
}

void steps(mst::Stream& out, const data::CogHeat& h)
{
    out << "NSTEP" << "\t" << h.nstep << endl;
    out << "STPINC" << "\t" << h.stpinc << endl;
    out << "SOLMTT" << "\t" << h.solmtt << endl;
}

void trans(mst::Stream& out)
{
    out << "TRANS " << "\t" << 1 << "\t" << 0 << "\t" << -1 << "\t" << -1 << "\t" << -1 << "\t" << -1 << endl;
}

// Cycle time of BDW/BHT: 1 / CYCLES for die movement type 4, else Cycle_Time.
double cycleTime(const CogModel& m)
{
    return (m.cog.die.movType == 4) ? 1.0 / m.cog.die.cycles : m.cycleTime;
}

// "OBJPOS\t1\t3\t0\t0\t0\t1\t0\t0\t<a>" [+ "\t4\n4\t5\t6\t7\t"] "\n"
void rotLine(Blk& k, double a)
{
    mst::Stream& out = k.w.ts;
    const int n = 4;
    out << "OBJPOS" << "\t" << 1 << "\t" << 3 << "\t" << 0.0 << "\t" << 0.0 << "\t" << 0.0 << "\t" << 1.0
        << "\t" << 0.0 << "\t" << 0.0 << "\t" << a;
    if (useManip(k.m)) {
        out << "\t" << n << endl;
        for (int i = 0; i < n; i++) out << manipId(i) << "\t";
    }
    out << endl;
}

}  // namespace

// @src 0x1407532b0
void writePHT(Blk& k, int r, int p, bool* first, bool* firstOp, int state, bool* manipFirst)
{
    mst::Stream& out = k.w.ts;
    headFirst(k, p, first, firstOp);
    manipFirstPass(k, manipFirst);
    const CogPass& P = pass(k.m.cog, r, p);
    if (state != 3) retract(k, [&](int id) { return id == 3 || state != 2; });
    steps(out, P.ht);
    out << "DTMAX " << "\t" << P.ht.tmax / static_cast<double>(P.ht.nstep) << endl;
    out << "DSMAX " << "\t" << 0 << endl;
    mst::emitContactFlags(k.w, k.prj, k.db, *k.sim, k.op);              // 0x140136040
    trans(out);
    tail(k, blockName("RH%1-PS%2-PHT", r, p));
}

// @src 0x140754250
void writePDWL(Blk& k, int r, int p, bool* first, bool* firstOp, int state, bool* manipFirst)
{
    mst::Stream& out = k.w.ts;
    headFirst(k, p, first, firstOp);
    manipFirstPass(k, manipFirst);
    const CogPass& P = pass(k.m.cog, r, p);
    if (state != 3) retract(k, [](int) { return true; });
    out << "OBJPOS" << "\t" << 3 << "\t" << 12 << "\t" << 1e-05 << "\t" << 0.0 << "\t" << 0.0 << "\t" << 1.0
        << "\t" << 1 << "\t" << 0.0 << "\t" << 0.0 << endl;
    steps(out, P.ht);
    out << "DTMAX " << "\t" << P.dwellTime / static_cast<double>(P.ht.nstep) << endl;
    out << "DSMAX " << "\t" << 0 << endl;
    mst::emitContactFlags(k.w, k.prj, k.db, *k.sim, k.op);
    trans(out);
    tail(k, blockName("RH%1-PS%2-PDWL", r, p));
}

// @src 0x140755b90
void writeBDEF(Blk& k, int r, int p, int b, bool* first, bool* firstOp, int state, bool* manipFirst)
{
    mst::Stream& out = k.w.ts;
    const CogModel& m = k.m;
    const CogData& c = m.cog;
    headFirst(k, p, first, firstOp);
    const CogPass& P = pass(c, r, p);
    const CogBite* B = bite(c, r, p, b);
    (void)B;
    const int useSpring = c.manip.useSpring;                            // manip+0x8b4
    const double v = manipStartPos(m);
    const int nD = dieCount(m);
    const bool manip = useManip(m);
    (void)m.useMandrel;                                                 // 0x14075dc00: read, unused
    const int dbs = c.billet.doBilStraight;                             // billet+0x1090
    if ((dbs == 1 && (b != 1 || p != 1)) || (dbs == 2 && b == 1 && 1 < p)) {
        out << "EXECMD" << "\t" << "3" << endl;
        out << "COG_GEO_MANIP.EXE" << "\t" << qt::fileName(k.db) << endl;
        out << "KFREAD" << "\t" << 1 << endl;
        out << "COG_STRAIGHT_BILLET.KEY" << endl;
    }
    if (c.billet.axialSub != 0.0 && 1 < b) {
        const double a = (c.billet.axialType == 0) ? c.billet.axialSub : -c.billet.axialSub;
        out << "GENSUB" << "\t" << 1 << "\t" << a << endl;
        out << "EXECMD" << "\t" << "3" << endl;
        out << "DEF_BRK.EXE" << endl;
        out << "KFREAD" << "\t" << 1 << endl;
        out << "NEWBRICK.KEY" << endl;
    }
    double tx = translationX(c, r, p, b);
    bool c13 = false;
    if (b == 1) {
        if (P.diePosMethod != 3 && (1 < p || !*firstOp)) c13 = true;
        if (P.rotAng != 0.0) rotLine(k, P.rotAng);
    } else {
        const double prev = translationX(c, r, p, b - 1);
        tx = tx - prev;
    }
    if (!c13) {
        out << "OBJPOS" << "\t" << 2 << "\t" << 1 << "\t" << tx << "\t" << 0.0 << "\t" << 0.0 << "\t" << 0.0
            << "\t" << 0.0 << "\t" << 0.0 << "\t" << 0.0;
    } else {
        out << "OBJPOS" << "\t" << 2 << "\t" << 13 << "\t" << 0 << "\t" << P.dieMovDir << "\t" << 0.0 << "\t"
            << 0.0 << "\t" << 1 << "\t" << P.diePosMethod << "\t";
        if (P.diePosMethod == 1)
            out << static_cast<int>(P.dieMovDir != pass(c, r, 1).dieMovDir);
        else
            out << P.dieStrtPos;
    }
    out << "\t" << nD - 1 << endl;
    for (int i = 1; i < nD; i++) out << dieId(i) << "\t";
    out << endl;
    if (P.biteAngle != 0.0 && 1 < b) rotLine(k, P.biteAngle);

    const double dObj12 = 1e-05;                                        // 0x1422395f8
    const int kObj12 = 12;
    const bool dead = false;                                            // bStack_168, never set
    double feed = P.radialFeedPerBite;                                  // pass+0xf8
    if (b == 1) {
        if (1 < p) {
            feed = (pass(c, r, p - 1).minDieDis - P.minDieDis) / 2.0;
            for (int i = 0; i < nD; i++) out << "DSTROKE" << "\t" << dieId(i) << "\t" << feed << endl;
        }
        const double h = P.horizDieShift;                               // pass+0xe8
        if (h != 0.0) {
            double d[3];
            if (h <= 1e-6) {
                if (h <= 1e-6 && 1e-6 != h) {
                    dieDir(2, d);
                    out << "OBJPOS " << "\t" << 8 << "\t" << 11 << "\t" << h * d[0] << "\t" << h * d[1] << "\t"
                        << h * d[2] << endl;
                    dieDir(3, d);
                    out << "OBJPOS " << "\t" << 9 << "\t" << 11 << "\t" << h * d[0] << "\t" << h * d[1] << "\t"
                        << h * d[2] << endl;
                    out << "DSTROKE" << "\t" << 2 << "\t" << -h << endl;
                    out << "DSTROKE" << "\t" << 3 << "\t" << -h << endl;
                }
            } else {
                dieDir(0, d);
                out << "OBJPOS " << "\t" << 2 << "\t" << 11 << "\t" << h * d[0] << "\t" << h * d[1] << "\t"
                    << h * d[2] << endl;
                dieDir(1, d);
                out << "OBJPOS " << "\t" << 3 << "\t" << 11 << "\t" << h * d[0] << "\t" << h * d[1] << "\t"
                    << h * d[2] << endl;
                out << "DSTROKE" << "\t" << 8 << "\t" << -h << endl;
                out << "DSTROKE" << "\t" << 9 << "\t" << -h << endl;
            }
        }
    } else if (m.processType == 1 && 0.0 < feed) {                      // 0x1413ee240
        for (int i = 0; i < nD; i++) out << "DSTROKE" << "\t" << dieId(i) << "\t" << feed << endl;
    }
    // Die retraction and the "OBJPOS 2 12" record (the original's alternative branch with per-die
    // "12" records is dead: its flag is constant 1).
    {
        const double half = cogSize(c, 1, 2) / 2.0;
        for (int i = 0; i < nD; i++) {
            const int id = dieId(i);
            if ((id != 3 || !dead) && (id == 3 || state != 2)) {
                double d[3];
                dieDir(i, d);
                const double z = -half * d[2], y = -half * d[1], x = -half * d[0];
                out << "OBJPOS " << "\t" << id << "\t" << 11 << "\t" << x << "\t" << y << "\t" << z << endl;
            }
        }
        out << "OBJPOS" << "\t" << 2 << "\t" << kObj12 << "\t" << dObj12 << "\t" << 0.0 << "\t" << 0.0 << "\t"
            << 0.0 << "\t" << 1 << "\t" << 0.0 << "\t" << 0.0;
        out << "\t" << nD - 1 - static_cast<int>(dead) << endl;
        for (int i = 1; i < nD; i++)
            if (dieId(i) != 3 || !dead) out << dieId(i) << "\t";
        out << endl;
    }
    if (m.processType == 0) {
        for (int i = 0; i < nD; i++) {
            out << "FORCE" << "\t" << dieId(i) << "\t" << 0 << "\t" << 0 << "\t" << 0 << endl;
            if (c.die.movType == 1)
                out << "STROKE " << "\t" << dieId(i) << "\t" << 0 << "\t" << 0 << "\t" << 0 << endl;
        }
    }
    if (manip) {
        if (*manipFirst) {
            *manipFirst = false;
            manipStart(out, v);
        }
        out << "OBJPOS" << "\t" << 4 << "\t" << 14 << "\t" << 5 << "\t" << 0 << "\t" << dObj12 << "\t" << 1 << endl;
        out << "OBJPOS" << "\t" << 6 << "\t" << 14 << "\t" << 7 << "\t" << 0 << "\t" << dObj12 << "\t" << 1 << endl;
        out << "GENAXS" << "\t" << 7 << "\t" << c.manip.leftDis << "\t" << c.manip.rightDis << endl;
    }
    out << "URZ" << "\t" << 1 << "\t1 0.0 0.0 0.0" << endl;
    out << "1\t 0.0 0.0 0.0" << endl;
    if (manip && useSpring != 0) {
        for (int i = 0; i < 4; i++) {
            double d[3];
            manipDir(i, d);                                             // result unused
            out << "STROKE " << "\t" << manipId(i) << "\t0.0 0.0 0.0" << endl;
        }
    }
    mst::emitContactFlags(k.w, k.prj, k.db, *k.sim, k.op);
    out << "MDSOBJ" << "\t" << 2 << "\t" << 3 << "\t" << 3 << "\t" << P.minDieDis << endl;
    if (c.die.movType == 4) out << "SMAX" << "\t" << 0 << "\t" << 0 << "\t" << c.die.totStroke << endl;
    if (m.byte538 != 0 && m.swagingStroke != 0.0 && m.numStepsDsmax != 0)   // 0x14075dd70: never set
        out << "DSMAX_NSTEP" << "\t" << m.swagingStroke << "\t" << m.numStepsDsmax << endl;
    String name = blockName("RH%1-PS%2-BDEF%3", r, p, b);
    if (P.autoBite != 0) name += qt::L("(Auto)");
    tail(k, name);
}

// @src 0x140758fd0
void writeBDCT(Blk& k, int r, int p, int b, double decomp)
{
    mst::Stream& out = k.w.ts;
    head2(k, p);
    const CogBite* B = bite(k.m.cog, r, p, b);
    steps(out, B->ht);
    out << "DTMAX " << "\t" << decomp / static_cast<double>(B->ht.nstep) << endl;
    out << "DSMAX " << "\t" << 0 << endl;
    trans(out);
    tail(k, blockName("RH%1-PS%2-BDCT%3", r, p, b));
}

// @src 0x140758620
void writeBDW(Blk& k, int r, int p, int b, double dwell)
{
    mst::Stream& out = k.w.ts;
    head2(k, p);
    const CogPass& P = pass(k.m.cog, r, p);
    const CogBite* B = bite(k.m.cog, r, p, b);
    retract(k, [](int id) { return id != 3; });
    steps(out, B->ht);
    out << "DSMAX " << "\t" << 0 << endl;
    out << "DTMAX " << "\t" << dwell / static_cast<double>(B->ht.nstep) << endl;
    if (dwell == 0.0) {
        const double cyc = cycleTime(k.m);
        const double dec = k.m.decompressionTime;
        if (dec + dwell < cyc)
            out << "TCYCLE" << "\t" << B->ht.nstep << "\t" << P.bitesToSkip << "\t" << cyc << "\t" << dec << "\t"
                << dwell << endl;
    }
    mst::emitContactFlags(k.w, k.prj, k.db, *k.sim, k.op);
    trans(out);
    tail(k, blockName("RH%1-PS%2-BDW%3", r, p, b));
}

// @src 0x140755170
void writeBHT(Blk& k, int r, int p, int b, int state)
{
    mst::Stream& out = k.w.ts;
    head2(k, p);
    const CogPass& P = pass(k.m.cog, r, p);
    const CogBite* B = bite(k.m.cog, r, p, b);
    const int bottom = k.m.bottomFixed;                                 // 0x140765b70
    retract(k, [&](int id) { return (id != 3 || bottom != 2) && (id == 3 || state != 2); });
    steps(out, B->ht);
    out << "DTMAX " << "\t" << B->ht.tmax / static_cast<double>(B->ht.nstep) << endl;
    out << "DSMAX " << "\t" << 0 << endl;
    const double cyc = cycleTime(k.m);
    if (k.m.decompressionTime + k.m.dwellTime < cyc)
        out << "TCYCLE" << "\t" << B->ht.nstep << "\t" << P.bitesToSkip << "\t" << cyc << "\t"
            << k.m.decompressionTime << "\t" << k.m.dwellTime << endl;
    mst::emitContactFlags(k.w, k.prj, k.db, *k.sim, k.op);
    trans(out);
    String name = blockName("RH%1-PS%2-BHT%3", r, p, b);
    if (P.autoBite != 0) name += qt::L("(Auto)");
    tail(k, name);
}

// @src 0x1407595c0
void writeSHT(Blk& k, int r, int p, double tr1, double aht)
{
    mst::Stream& out = k.w.ts;
    head2(k, p);
    const CogPass& P = pass(k.m.cog, r, p);
    retract(k, [](int) { return true; });
    steps(out, P.ht);
    out << "DSMAX " << "\t" << 0 << endl;
    out << "ENVTMP" << "\t" << 0 << "\t" << tr1 << endl;
    out << "DTMAX " << "\t" << aht / static_cast<double>(P.ht.nstep) << endl;
    mst::emitContactFlags(k.w, k.prj, k.db, *k.sim, k.op);
    trans(out);
    tail(k, blockName("RH%1-PS%2-SHT", r, p));
}

}  // namespace m2m::ops::cog
