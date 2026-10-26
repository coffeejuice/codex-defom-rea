// MDT: Cogging.MDT writer of File > Save (docs/re/moproj2mst/mdt/spec.md; decompiles
// deform-rea-work/gui_mo/s1/mdt/w_*.c). One function per binary function, in the original's call
// order; numbers go through the static CRT's printf (def::rt::crt, %15.7E with 3-digit exponents).
#include <algorithm>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iterator>

#include <defrt/stdlib/legacy_printf.hpp>

#include "m2m/engine.hpp"
#include "m2m/longpath.hpp"
#include "m2m/qt4path.hpp"
#include "readers.hpp"
#include "cogging_file.hpp"

namespace m2m::datafiles {

using data::CogBillet;
using data::CogBite;
using data::CogData;
using data::CogDie;
using data::CogHeat;
using data::CogManip;
using data::CogModel;
using data::CogPass;
using data::CogReheat;

namespace {

// The FILE* of fopen(path, "wt"): text mode turns every '\n' into CR LF.
struct Out {
    std::string bytes;
    std::FILE* native = nullptr;
    void put(const std::string& s)
    {
        if (native) {
            // 0x14146da0c: each original printf result is ignored. A failed
            // fragment does not suppress later printf calls as QTextStream does.
            (void)std::fprintf(native, "%s", s.c_str());
            return;
        }
        for (char c : s) {
            if (c == '\n') bytes += '\r';
            bytes += c;
        }
    }
};


void pf(Out& o, const char* fmt, const char* key, int v) { o.put(def::rt::crt::format(fmt, key, v)); }
void pf(Out& o, const char* fmt, const char* key, double v) { o.put(def::rt::crt::format(fmt, key, v)); }
void pf(Out& o, const char* fmt, const char* key, const std::string& v)
{
    o.put(def::rt::crt::format(fmt, key, v.c_str()));
}

// "%s %6d\n", "%s %15.7E\n", "%s %s\n"
void I(Out& o, const char* key, int v) { pf(o, "%s %6d\n", key, v); }
void E(Out& o, const char* key, double v) { pf(o, "%s %15.7E\n", key, v); }
void S(Out& o, const char* key, const std::string& v) { pf(o, "%s %s\n", key, v); }

// 0x1437d7dd0: the separator the banner modes print ("*" x 78)
const char* const kSepLine = "******************************************************************************";

void unsupported(const std::string& what)
{
    throw Error(M2M_E_UNSUPPORTED, "Cogging.MDT: " + what);
}

}  // namespace

// @src 0x14076b450  banner(f, mode, fmt, ...): vsprintf into a 4 KB buffer, then by mode
static void banner(Out& o, int mode, const std::string& text)
{
    const char* t = text.c_str();
    switch (mode) {
    case 1: o.put(def::rt::crt::format("* %s\n* %s\n* %s\n", kSepLine, t, kSepLine)); break;   // 0x1437d7e30
    case 2: o.put(def::rt::crt::format("* %s\n*\n* %s\n*\n* %s\n", kSepLine, t, kSepLine)); break;   // 0x1437d7e40
    case 3: o.put(def::rt::crt::format("*\n*\n* %s\n*\n*\n", t)); break;   // 0x1437d7e58
    case 4: o.put(def::rt::crt::format("%s\n", t)); break;                 // 0x1437d7e54
    default: o.put(def::rt::crt::format("*\n* %s\n*\n", t)); break;         // 0x1437d7e68
    }
}

// UserKey lines: "---" when the key's first byte is a blank (the reader's "---" -> " ")
static const std::string& userKeyText(const std::string& k)
{
    static const std::string dashes = "---";   // 0x1437e400c / 0x1437e29f4
    return (!k.empty() && k[0] == ' ') ? dashes : k;
}

// @src 0x140798220  HEAT(h, f, k): k only range-checked (0..4)
static void heatWrite(Out& o, const CogHeat& h)
{
    banner(o, 0, "Heat Transfer");
    I(o, "Active ", h.active);
    I(o, "NSTEP  ", h.nstep);
    I(o, "STPINC ", h.stpinc);
    E(o, "TMAX   ", h.tmax);
    E(o, "DTPMAX ", h.dtpmax);
    o.put(def::rt::crt::format("%s\t0\t0\n", "ENVTMP "));
    E(o, "IHTCOF ", h.ihtcof);
    E(o, "IHTCOF ", h.ihtcof2);
    S(o, "UserKey", userKeyText(h.userKey));
}

// @src 0x140791220  BITE deformation data
static void biteDeformWrite(Out& o, const CogBite& b)
{
    banner(o, 0, "Bite Deformation");
    I(o, "NSTEP  ", b.nstep);
    I(o, "STPINC ", b.stpinc);
    I(o, "PDIE   ", b.pdie);
    E(o, "SMAX   ", b.smax);
    E(o, "LMAX   ", b.lmax);
    E(o, "ClampForce", b.clampForce);
    E(o, "DTSUB  ", b.dtsub);
    I(o, "StepMod", b.stepMod);
    E(o, "DSMAX  ", b.dsmax);
    E(o, "DTMAX  ", b.dtmax);
    I(o, "FricTyp", b.fricTyp);
    E(o, "FRCFAC ", b.frcfac);
    E(o, "IHTCOF ", b.ihtcof);
    E(o, "IHTCOF ", b.ihtcof2);
    S(o, "UserKey", userKeyText(b.userKey));
    E(o, "NomBite", b.nomBite);
}

// @src 0x140790d10  BITE(b, f, n): "Bite n", its HEAT (+0xc0), deformation data
static void biteWrite(Out& o, const CogBite& b, int n)
{
    banner(o, 0, def::rt::crt::format("Bite %d", n));
    heatWrite(o, b.ht);
    biteDeformWrite(o, b);
}

// @src 0x140792d40  PASS fields, then the pass HEAT (+0x28)
static void passFieldsWrite(Out& o, const CogPass& p)
{
    if (p.autoBite == 0) I(o, "NoBite     ", p.noBite);
    else I(o, "NoBite     ", 1);
    E(o, "RotAng       ", p.rotAng);
    E(o, "BiteAngle    ", p.biteAngle);
    E(o, "MinDieDis    ", p.minDieDis);
    I(o, "DiePosMethod ", p.diePosMethod);
    E(o, "DieStrtPos   ", p.dieStrtPos);
    E(o, "DieStopPos   ", p.dieStopPos);
    I(o, "DieStrtNod   ", p.dieStrtNod);
    I(o, "DieStopNod   ", p.dieStopNod);
    I(o, "DieMovDir    ", p.dieMovDir);
    I(o, "AutoBite     ", p.autoBite);
    E(o, "LeftManDis   ", p.leftManDis);
    E(o, "RightManDis  ", p.rightManDis);
    E(o, "HorizDieShift  ", p.horizDieShift);
    I(o, "BitesToSkip  ", p.bitesToSkip);
    E(o, "RadialFeedPerBite  ", p.radialFeedPerBite);
    E(o, "DwellTime  ", p.dwellTime);
    heatWrite(o, p.ht);
}

static const CogBite& biteAt(const CogPass& p, int i)
{
    if (i < 0 || static_cast<std::size_t>(i) >= p.bites.size())
        throw Error(M2M_E_INPUT, "Cogging.MDT: pass bite count does not match its bites");
    return p.bites[static_cast<std::size_t>(i)];
}

// @src 0x140792720  "Pass n", fields, then all bites (AutoBite: the first only)
static void passWrite(Out& o, const CogPass& p, int n)
{
    banner(o, 0, def::rt::crt::format("Pass %d", n));
    passFieldsWrite(o, p);
    if (p.autoBite == 0) {
        for (int i = 0; i < p.noBite; i = i + 1) biteWrite(o, biteAt(p, i), i + 1);
    } else {
        biteWrite(o, biteAt(p, 0), 1);
    }
}

// @src 0x140797550  BILLET
static void billetWrite(Out& o, const CogBillet& b)
{
    banner(o, 2, "Billet");
    I(o, "GeoTyp    ", b.geoTyp);
    I(o, "GeoMod    ", b.geoMod);
    S(o, "GeoFil    ", b.geoFile);
    E(o, "Length    ", b.length);
    E(o, "Dia       ", b.dia);
    E(o, "InDia     ", b.inDia);
    E(o, "Height    ", b.height);
    E(o, "Width     ", b.width);
    E(o, "Radius    ", b.radius);
    E(o, "Layers    ", b.layers);
    I(o, "MGNELM    ", b.mgnelm);
    E(o, "MGSIZR    ", b.mgsizr);
    I(o, "MGTELM    ", b.mgtelm);
    E(o, "TMPBIL    ", b.tmpbil);
    I(o, "BccMod    ", b.bccMod);
    I(o, "MergeL    ", b.mergeL);
    S(o, "KeyBil    ", b.keyBil);
    E(o, "AxialSub  ", b.axialSub);
    I(o, "AxialType ", b.axialType);
    I(o, "DoBilStraight ", b.doBilStraight);
    I(o, "MatrMod    ", b.martMod);
    I(o, "MeshMod    ", b.meshMod);
}

// @src 0x140795620  die KEY name k (1..NoDies) from the table the ctor 0x140794ce0 fills
// ("DIE%d.KEY" at 0x143dacd70 + (k-1)*0x400; no reader writes the table)
static std::string dieKeyName(int k, const CogGlobals& g)
{
    if (k < 1 || g.noDies < k) unsupported("die key index out of range");   // 0x140b2f9d0: exits
    return def::rt::crt::format("DIE%d.KEY", k);
}

// @src 0x140795fd0  DIE (sets the NoDies global from die+0x14a8)
static void dieWrite(Out& o, const CogDie& d, CogGlobals& g)
{
    banner(o, 2, "Die");
    I(o, "GeoTyp   ", d.geoTyp);
    I(o, "GeoMod   ", d.geoMod);
    S(o, "GeoFile   ", d.geoFile);
    E(o, "Length   ", d.length);
    E(o, "Width    ", d.width);
    E(o, "Height   ", d.height);
    E(o, "Radius   ", d.radius);
    E(o, "Layers   ", d.layers);
    I(o, "MGNELM   ", d.mgnelm);
    E(o, "MGSIZR   ", d.mgsizr);
    I(o, "MGTELM   ", d.mgtelm);
    I(o, "DieMov   ", g.dieMov);                  // global 0x1437e3444
    E(o, "TMPDIE   ", d.tmpdie);
    if (d.totStroke <= 0.0 || d.cycles <= 0.0) E(o, "SPDDIE   ", d.spddie);   // 0x1421ad1d0 = 0.0
    else E(o, "SPDDIE   ", 0.0);
    E(o, "CYCLES   ", d.cycles);
    E(o, "TOTSTROKE   ", d.totStroke);
    E(o, "RODLENGTH   ", d.rodLength);
    g.noDies = d.four == 1 ? 4 : 2;               // global 0x1437e3440
    I(o, "NoDies   ", g.noDies);
    S(o, "KeyDie1  ", dieKeyName(1, g));
    S(o, "KeyDie2  ", dieKeyName(2, g));
    if (g.noDies == 4) {
        S(o, "KeyDie3  ", dieKeyName(3, g));
        S(o, "KeyDie4  ", dieKeyName(4, g));
    }
    I(o, "RefPos   ", d.refPos);
    E(o, "Overlap  ", d.overlap);
    E(o, "StartPos ", d.startPos);
    I(o, "MovDir   ", d.movDir);
    I(o, "MovType  ", d.movType);
    E(o, "StopPos  ", d.stopPos);
    I(o, "MartMod    ", d.martMod);
    I(o, "MeshMod    ", d.meshMod);
}

// @src 0x1407946d0  MANIPULATOR
static void manipWrite(Out& o, const CogManip& m)
{
    banner(o, 2, "Manipulator");
    I(o, "GeoTyp    ", m.geoTyp);
    I(o, "GeoMod    ", m.geoMod);
    S(o, "GeoFile   ", m.geoFile);
    E(o, "Length    ", m.length);
    E(o, "Width     ", m.width);
    E(o, "Height    ", m.height);
    E(o, "Radius3   ", m.radius3);
    E(o, "Layers    ", m.layers);
    I(o, "MGNELM    ", m.mgnelm);
    E(o, "MGSIZR    ", m.mgsizr);
    I(o, "MGTELM    ", m.mgtelm);
    E(o, "TMPDIE    ", m.tmpdie);
    S(o, "KeyMan1   ", m.keyMan[0]);
    S(o, "KeyMan2   ", m.keyMan[1]);
    S(o, "KeyMan3   ", m.keyMan[2]);
    S(o, "KeyMan4   ", m.keyMan[3]);
    E(o, "StartPos  ", m.startPos);
    E(o, "LeftDis   ", m.leftDis);
    E(o, "RightDis  ", m.rightDis);
    I(o, "UseMstMan ", m.useMstMan);
    I(o, "UseSpring ", m.useSpring);
    E(o, "Stiffness ", m.stiffness);
    E(o, "Preload   ", m.preload);
    E(o, "MaxDisp   ", m.maxDisp);
    E(o, "Width1    ", m.width1);
    E(o, "Height1   ", m.height1);
    E(o, "Radius1   ", m.radius1);
    E(o, "Radius2   ", m.radius2);
    I(o, "MartMod    ", m.martMod);
    I(o, "MeshMod    ", m.meshMod);
}

// @src 0x14076aed0  SIMULATION PARAMETERS (cogging globals)
static void simParamsWrite(Out& o, const CogData& c, CogGlobals& g)
{
    const data::CogProcess& pr = cogProcess(g);
    banner(o, 2, "Simulation Parameters");
    I(o, "UNIT     ", g.unit - 1);                   // 0x14076bd50
    E(o, "POSERR   ", pr.poserr);                    // 0x1407991b0
    E(o, "INTERR   ", pr.interr);                    // 0x1407991f0
    E(o, "CVGERR1  ", pr.cvgerr[0]);                 // 0x14076be30(1)
    E(o, "CVGERR2  ", pr.cvgerr[1]);                 // 0x14076be30(2)
    E(o, "AVGSTR   ", pr.avgstr);                    // 0x14076bfb0
    E(o, "LMTSTR   ", pr.lmtstr);                    // 0x14076bf70
    E(o, "CTLPCT   ", pr.ctlpct);                    // 0x140796ba0
    I(o, "UseManip ", c.useManip);                   // byte cog+0xc8, zero-extended
}

// @src 0x140791f40  simulations of a pass
static int passSimCount(const CogPass& p)
{
    int n = 0;
    if (p.autoBite == 0) {
        for (int i = 0; i < p.noBite; i = i + 1) {
            if (biteAt(p, i).ht.active != 0) n = n + 1;
            n = n + 1;
        }
    } else {
        n = biteAt(p, 0).ht.active != 0 ? 1 : 0;
        n = n + 1;
    }
    if (p.ht.active != 0) n = n + 1;
    return n;
}

// @src 0x14076c340  simulations of a reheat
static int reheatSimCount(const CogReheat& r)
{
    int n = r.heat[0].active != 0 ? 1 : 0;
    if (r.heat[1].active != 0) n = n + 1;
    if (r.heat[2].active != 0) n = n + 1;
    for (int i = 0; i < r.nPass; i = i + 1) n = n + passSimCount(r.passes.at(static_cast<std::size_t>(i)));
    return n;
}

// @src 0x14076b8e0  simulation label(kind, rh, ps, bite, flag)
static std::string simLabel(int kind, int rh, int ps, int bite, int flag)
{
    using def::rt::crt::format;
    switch (kind) {
    case 0: return format("RH%02d-HI", rh);
    case 1: return format("RH%02d-HeatUp", rh);
    case 2: return format("RH%02d-HO", rh);
    case 3: return flag == 1 ? format("RH%02d-PH%02d-Auto", rh, ps) : format("RH%02d-PH%02d-Fix", rh, ps);
    case 4:
        if (flag == 1) return format("RH%02d-PS%02d-BH-Auto", rh, ps);
        if (flag == 2) return format("RH%02d-PS%02d-BH-Fix", rh, ps);
        return format("RH%02d-PS%02d-BH%03d", rh, ps, bite);
    case 5:
        if (flag == 1) return format("RH%02d-PS%02d-BD-Auto", rh, ps);
        if (flag == 2) return format("RH%02d-PS%02d-BD-Fix", rh, ps);
        return format("RH%02d-PS%02d-BD%03d", rh, ps, bite);
    default: unsupported("simulation kind out of range");   // 0x140b2f9d0: exits
    }
    return {};
}

// @src 0x14076b7f0  "%4d %s\n" (via 0x140797de0 / 0x140790ad0, which also count)
static void simLine(Out& o, int& counter, const std::string& label)
{
    o.put(def::rt::crt::format("%4d %s\n", counter, label.c_str()));
    counter = counter + 1;
}

// @src 0x140791ce0  SIMLST lines of a pass
static void passSimLines(Out& o, const CogPass& p, int rh, int ps, int& counter)
{
    if (p.autoBite == 0) {
        for (int i = 0; i < p.noBite; i = i + 1) {
            const CogBite& b = biteAt(p, i);
            if (b.ht.active != 0) simLine(o, counter, simLabel(4, rh, ps, i + 1, 0));
            simLine(o, counter, simLabel(5, rh, ps, i + 1, 0));
        }
    } else {
        const CogBite& b = biteAt(p, 0);
        if (b.ht.active != 0) simLine(o, counter, simLabel(4, rh, ps, 1, 1));
        simLine(o, counter, simLabel(5, rh, ps, 1, 1));
    }
    if (p.ht.active != 0) simLine(o, counter, simLabel(3, rh, ps, 0, p.autoBite == 0 ? 0 : 1));
}

// @src 0x14076c1c0  SIMLST lines of a reheat
static void reheatSimLines(Out& o, const CogReheat& r, int rh, int& counter)
{
    if (r.heat[0].active != 0) simLine(o, counter, simLabel(0, rh, 0, 0, 0));
    if (r.heat[1].active != 0) simLine(o, counter, simLabel(1, rh, 0, 0, 0));
    if (r.heat[2].active != 0) simLine(o, counter, simLabel(2, rh, 0, 0, 0));
    for (int i = 0; i < r.nPass; i = i + 1) passSimLines(o, r.passes.at(static_cast<std::size_t>(i)), rh, i + 1, counter);
}

// @src 0x14076cb50  "Reheat n", NoPass, three HEATs, the passes
static void reheatWrite(Out& o, const CogReheat& r, int n)
{
    banner(o, 0, def::rt::crt::format("Reheat %d", n));
    I(o, "NoPass", r.nPass);
    heatWrite(o, r.heat[0]);
    heatWrite(o, r.heat[1]);
    heatWrite(o, r.heat[2]);
    for (int i = 0; i < r.nPass; i = i + 1) passWrite(o, r.passes.at(static_cast<std::size_t>(i)), i + 1);
}

// @src 0x14076a8c0  master data (everything before Process Data)
static void masterWrite(Out& o, const CogData& c, CogGlobals& g, const std::string& header)
{
    data::CogProcess& pr = cogProcess(g);
    banner(o, 2, header);                                        // "%s %s" 0x143dbd100, 0x143dbcbd0
    banner(o, 2, def::rt::crt::format("MASTER DATA FILE %d", 11000));
    S(o, "PRBID  ", c.prbid);
    S(o, "TITLE  ", c.title);
    banner(o, 0, "Defaults");
    banner(o, 2, "Reheats");
    heatWrite(o, pr.slot[0]);                                    // 0x140797d50(k) copies
    heatWrite(o, pr.slot[1]);
    heatWrite(o, pr.slot[2]);
    banner(o, 2, "Pass");
    passFieldsWrite(o, pr.defaultPass);                          // 0x140791cb0 copy
    banner(o, 2, "Bite");
    biteWrite(o, pr.defaultBite, 1);                             // 0x140790aa0 copy
    billetWrite(o, c.billet);
    dieWrite(o, c.die, g);
    manipWrite(o, c.manip);
    simParamsWrite(o, c, g);
    banner(o, 2, "Simulation Data");
    if (c.nReheats > 0 && c.reheats.size() < static_cast<std::size_t>(c.nReheats))
        throw Error(M2M_E_INPUT, "Cogging.MDT: reheat count does not match its reheats");
    int total = 0;                                               // 0x1407697d0
    for (int i = 0; i < c.nReheats; i = i + 1) total = total + reheatSimCount(c.reheats[static_cast<std::size_t>(i)]);
    I(o, "SIMLST ", total);
    int counter = 1;
    for (int i = 0; i < c.nReheats; i = i + 1) reheatSimLines(o, c.reheats[static_cast<std::size_t>(i)], i + 1, counter);
    I(o, "NoReheats", c.nReheats);
    // The second loop (14076ac03..14076ac72) accumulates its pointer: iteration i writes element
    // n-1 + i(i+1)/2, so with two or more reheats it writes the wrong reheat and then reads past
    // the array. Only the one-reheat file is defined.
    if (c.nReheats >= 2)
        unsupported("NoReheats " + std::to_string(c.nReheats) +
                    ": the GUI writes reheat data past the end of its array (spec section 3)");
    if (c.nReheats == 1) reheatWrite(o, c.reheats[0], 1);
}

static void writeMdt(Out& o, const CogModel& m, CogGlobals& g, const std::string& header)
{
    if (g.unit < 1 || g.unit > 2) unsupported("unit global out of range");
    masterWrite(o, m.cog, g, header);
    // @src 0x1407617e0 (Process Data)
    banner(o, 0, "Process Data");
    I(o, "Process_Type", m.processType);
    I(o, "Temperaure_Type", m.temperatureType);
    I(o, "Use_Mandrel", m.useMandrel);
    I(o, "Use_Adaptive_Reheat", m.useAdaptiveReheat);
    I(o, "Checking_Method", m.checkingMethod);
    o.put(def::rt::crt::format("%s %15.7E %15.7E\n", "Temperature_Range", m.tempRange[0], m.tempRange[1]));
    I(o, "Solver_Type", m.solverType);
    I(o, "Use_DifferentDies", m.useDifferentDies & 0xff);   // byte +0x14
    E(o, "Adaptive_Heating_Time", m.adaptiveHeatingTime);
    E(o, "Swaging_Stroke", m.swagingStroke);
    E(o, "Dwell_Time", m.dwellTime);
    E(o, "Cycle_Time", m.cycleTime);
    E(o, "Decompression_Time", m.decompressionTime);
    I(o, "Bottom_Fixed", m.bottomFixed);
    I(o, "Heat_Transfer", m.heatTransfer);
    I(o, "Remaining_Cycle_Time_Dwell", m.remainingCycleTimeDwell);
    E(o, "Clamp_Force", m.clampForce);
    E(o, "Clamp_Speed", m.clampSpeed);
    I(o, "RotationalSymmetry", m.rotationalSymmetry);
    I(o, "Num_Steps_DSMAX", m.numStepsDsmax);
}

std::string writeMdtBytes(const CogModel& m, CogGlobals& g, const std::string& header)
{
    Out o;
    writeMdt(o, m, g, header);
    return o.bytes;
}

// @src 0x1407fb240  version text: "11.0" plus the pack.dat suffix (the part after strncpy(.., 99))
static std::string versionText(const std::string& packDat, bool havePack)
{
    std::string version = "11.0";
    if (!havePack) return version;                 // no DEFORM_DIR, or pack.dat cannot be opened
    std::string text;                              // fopen "rt": CR LF -> LF
    for (std::size_t i = 0; i < packDat.size(); ++i) {
        if (packDat[i] == 0x1a) break;
        if (packDat[i] == '\r' && i + 1 < packDat.size() && packDat[i + 1] == '\n') continue;
        text += packDat[i];
    }
    std::size_t pos = 0;
    while (pos < text.size()) {
        // fgets(buf, 0x3ff)
        std::size_t e = pos;
        while (e < text.size() && e - pos < 0x3fe && text[e] != '\n') ++e;
        if (e < text.size() && text[e] == '\n' && e - pos < 0x3fe) ++e;
        std::string line = text.substr(pos, e - pos);
        pos = e;
        std::size_t v = line.find("VERSION");
        if (v == std::string::npos) continue;
        std::size_t len = line.size();
        std::size_t end = len - 1;                 // pcStack_c58: last character
        if (line[len - 1] == '\n') {
            line[len - 1] = '\0';
            end = len - 2;
        }
        std::size_t p = v + 7;
        while (p < end && line[p] == ' ') ++p;
        const std::string cstr = line.c_str();   // strstr stops at the NUL written above
        const bool beta = cstr.find("Beta") != std::string::npos;
        const bool rc = cstr.find("RC") != std::string::npos;
        const bool sp = cstr.find("SP") != std::string::npos;
        const bool custom = cstr.find("CUSTOM") != std::string::npos;
        std::string suffix;
        if (!beta && !rc && !sp) {
            if (!custom) {
                suffix = " (Service Pack " + std::string(line.c_str() + std::min(p, cstr.size()));
            } else {
                while (p < end && line[p] == ' ') ++p;
                suffix = " (" + std::string(line.c_str() + std::min(p, cstr.size()));
            }
        } else {
            suffix = " (" + std::string(line.c_str() + std::min(p, cstr.size()));
        }
        suffix += ")  ";
        if (version.size() + suffix.size() < 99) version += suffix;
        break;
    }
    return version;
}

std::string mdtHeaderText(const std::u16string& deformDir)
{
    std::string pack = "VERSION 2\n";             // pack.dat of the v11.0.2 installation
    bool have = true;
    if (!deformDir.empty()) {
        String path = qt::String(deformDir) + qt::L("/PACK/") + qt::L("pack.dat");
        std::ifstream in(std::filesystem::path(win32Path(path.u16())), std::ios::binary);
        have = static_cast<bool>(in);
        if (have) pack.assign(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
    }
    return "DEFORM-2D/3D " + versionText(pack, have);   // "%s %s" of 0x143dbd100, 0x143dbcbd0
}

// The operation of the active diagram item (the GUI's current operation, 0x140047a40 -> 0x1400b5e30),
// or null.
static Operation* activeOperation(Project& p)
{
    if (p.activeSub < 0 || static_cast<std::size_t>(p.activeSub) >= p.subs.size() || !p.subs[p.activeSub]) return nullptr;
    const FlowChart& flow = p.subs[static_cast<std::size_t>(p.activeSub)]->flow;
    if (flow.activeItem < 0 || static_cast<std::size_t>(flow.activeItem) >= flow.items.size()) return nullptr;
    const FlowItem* it = flow.items[static_cast<std::size_t>(flow.activeItem)].get();
    if (!it || it->group) return nullptr;
    return operationById(p, it->opId);
}

// File > Open loads the active diagram item's operation (0x14003a3c0 -> 0x140051c30, OB vt+0x58)
// and File > Save first saves it (0x140052ec0 -> vt+0x68 0x1405ef510). Only a cogging operation's
// step changes a Cogging.MDT (its vt+0xd8 0x14075c260 updates the cogging model), so the other
// classes are not run here. The files that load and save write themselves (task KEY files,
// equipment) are collected in FileEffects and not written by this converter.
void saveActiveCogging(Project& p, Ctx& ctx)
{
    Operation* cur = activeOperation(p);
    if (!cur || !cur->props) return;
    auto* cp = dynamic_cast<CoggingProps*>(cur->props.get());
    if (!cp || !cp->model) return;
    engine::FileEffects fx;
    try {
        Operation* op = engine::openActiveOperation(p, ctx, fx);
        if (op != cur) unsupported("it did not load (class gate)");
        engine::saveCurrentOperation(p, *op, ctx, fx);
    } catch (const Error& e) {
        throw Error(e.code, "the project's active operation \"" + cur->name.utf8() +
                                "\" is a cogging operation, which File > Save saves first: " + e.what());
    }
}

void saveCoggingMdts(Project& p, const std::string& header, bool produce, std::vector<SideFile>* out)
{
    if (produce) {
        // 0x1400980d0 first saves the current operation (saveActiveCogging below must have run):
        // a cogging operation's MDT is written from the model its Save geometry step updated
        Operation* cur = activeOperation(p);
        if (cur && cur->props) {
            auto* cp = dynamic_cast<CoggingProps*>(cur->props.get());
            if (cp && cp->model && !cur->hasEngineModel())
                unsupported("the project's active operation \"" + cur->name.utf8() +
                            "\" is a cogging operation that was not opened and saved first "
                            "(saveActiveCogging, spec section 6)");
        }
    }
    for (auto& opPtr : p.operations)              // vt+0x1b8 / vt+0x1c0 rows
        if (opPtr) saveCoggingMdt(p, *opPtr, header, produce, false, out);
}

bool saveCoggingMdt(Project& p, Operation& op, const std::string& header,
                    bool produce, bool native, std::vector<SideFile>* out)
{
    auto* cp = dynamic_cast<CoggingProps*>(op.props.get());
    if (!cp) return false;
    if (!cp->model) return true;
    // CogModel+0x548 points back to its properties, whose QPointer owner is
    // checked by0x1407616d0. The existing recording profile uses its row owner.
    if (native && !cp->owner) return true;
    const String dir = taskDirAbs(native ? *cp->owner : op);
    if (dir.isNull()) return true;
    const String path = qt::convertSeparators(dir + qt::kSep + qt::L("Cogging.MDT"));
    if (!produce) {
        p.p06Cog.noDies = cp->model->cog.die.four == 1 ? 4 : 2;
        return true;
    }
    if (native) {
        detail::CoggingFile file(path);         // 0x1407618ba: open BEFORE writer/global effects
        if (!file.get()) return true;           // 0x1407618bf ->c4b returns1 on failed open
        Out output;
        output.native = file.get();
        writeMdt(output, *cp->model, p.p06Cog, header);
        file.close();                           // independent of ignored fprintf status
        return true;
    }
    // Preserve the aggregate recording profile and its existing explicit refusal.
    for (char16_t c : path.u16())
        if (c > 0xff)
            unsupported("the path " + path.utf8() + " has characters the GUI's fopen cannot carry");
    out->push_back(SideFile{path, writeMdtBytes(*cp->model, p.p06Cog, header)});
    return true;
}

}  // namespace m2m::datafiles
