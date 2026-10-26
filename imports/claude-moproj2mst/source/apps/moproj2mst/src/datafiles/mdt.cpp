// P06: Cogging.MDT reader (s1-03 5; decompiles s1-03 mdt_d0c..d4c.c).
// One function per binary function. The MDT reader is positional: fscanf("%s <conv>\n") skips each
// keyword without checking it. Locals the original leaves uninitialised start at 0 here.
#include <cmath>
#include <cstring>

#include "cscanf.hpp"
#include "m2m/qt4path.hpp"
#include "readers.hpp"

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

// Per-file state the original keeps in globals: the five default HEAT slots (0x143db1a20 + k*0x80),
// the default PASS (0x143da7e00) and the default BITE (0x143da7cc0).
// The HEAT slots, default PASS and default BITE are process globals the MDT writer reads back
// (packet MDT): they live in CogGlobals::proc and outlive the file.
struct MdtState {
    explicit MdtState(CogGlobals& gl)
        : g(gl), proc(cogProcess(gl)), slot(proc.slot), defaultPass(proc.defaultPass), defaultBite(proc.defaultBite)
    {
    }
    CogGlobals& g;
    data::CogProcess& proc;
    int ctorUnit = 1;                      // unit global when the SfOprCogData ctor ran
    int ver = 0;
    CogHeat (&slot)[5];                    // 0x143db1a20
    CogPass& defaultPass;                  // 0x143da7e00
    CogBite& defaultBite;                  // 0x143da7cc0
    CogDie defaultDie;                     // 0x143dadd70 (0x140794ce0)
    CogManip defaultManip;                 // 0x143da8fe0 (0x140793100)
};

void requireInput(bool ok, const char* what)
{
    if (!ok) throw Error(M2M_E_INPUT, std::string("Cogging.MDT: ") + what);
}

// (int)float as CVTTSS2SI: out of range and NaN give 0x80000000.
int truncToInt(float v)
{
    if (!(v > -2147483904.0f && v < 2147483648.0f)) return static_cast<int>(0x80000000u);
    return static_cast<int>(v);
}

// strlen(buf) then remove one trailing '\n' (the readers' fgets clean-up)
void chopNewline(std::string& s)
{
    std::size_t n = std::strlen(s.c_str());
    s.resize(n);
    if (n > 0 && s[n - 1] == '\n') s.resize(n - 1);
}

// fgets into a fixed buffer: on end of file the buffer keeps what it held
void fgetsKeep(TextFile& f, std::string& buf, int n)
{
    std::string line;
    if (f.fgets(line, n)) buf = line;
}

}  // namespace

// @src 0x14076b5c0  skip(k): 3 lines for k in {0,1} (and k > 4), 5 for k in {2,3}, 1 for k == 4
static void skipLines(TextFile& f, int k)
{
    int n = 3;
    if (1 < k) {
        if (k < 4) n = 5;
        else if (k == 4) n = 1;
    }
    std::string buf;
    for (int i = 0; i < n; ++i) fgetsCog(f, buf, 0x1000);   // @src 0x1407fba10 -> 0x1407fb960
}

// @src 0x140797a10  HEAT ctor
static void heatCtor(CogHeat& h, int unit)
{
    h.active = 0;
    h.nstep = 1;
    h.stpinc = 1;
    h.tmax = 0.0;
    h.dtpmax = 0.0;
    h.solmtt = 1;
    if (unit == 2) {                       // 0x14076bd50
        h.ihtcof = 0.004;
        h.ihtcof2 = 0.004;
    } else {
        h.ihtcof = 4.0;
        h.ihtcof2 = 4.0;
    }
    h.userKey = "USER_HT.KEY";
}

// @src 0x140797ef0  HEAT_LoadFile(h, f, var, ver)
static void heatLoad(CogHeat& h, TextFile& f, int ver)
{
    std::string kw;
    float fv = 0.0f;                       // afStack_b0
    std::int32_t flag = 0;                 // iStack_b8
    double dv = 0.0;                       // uStack_20
    double pair[2] = {};
    skipLines(f, 0);
    cscan(f, "%s %d\n", {&kw, &h.active});
    cscan(f, "%s %d\n", {&kw, &h.nstep});
    cscan(f, "%s %d\n", {&kw, &h.stpinc});
    cscan(f, "%s %f\n", {&kw, &fv});
    h.tmax = static_cast<double>(fv);
    cscan(f, "%s %f\n", {&kw, &fv});
    h.dtpmax = static_cast<double>(fv);
    if (ver < 0xce7) {
        cscan(f, "%s %f\n", {&kw, &fv});
    } else {
        cscan(f, "%s  %d  %f\n", {&kw, &flag, &fv});
        if (flag == 1) {
            // The original loops while (float)i < count and never ends when the pairs run out
            // with a huge count; refuse such a file instead of hanging.
            requireInput(fv < 1.0e7f, "ENVTMP table count out of range");
            for (int i = 0; static_cast<float>(i) < fv; i = i + 1)
                cscan(f, "%lf %lf\n", {&pair[0], &pair[1]});
        }
    }
    cscan(f, "%s %lf\n", {&kw, &dv});
    h.ihtcof = dv;
    if (0xce4 < ver) {
        cscan(f, "%s %lf\n", {&kw, &dv});
        h.ihtcof2 = dv;
    }
    cscan(f, "%s \n", {&kw});
    fgetsKeep(f, h.userKey, 0x4f);
    chopNewline(h.userKey);
    if (h.userKey == "---") h.userKey = " ";
}

// @src 0x140790700  BITE ctor (with 0x140790870: HEAT ctor + slot 3) = 0x1407908a0
static void biteCtor(CogBite& b, const MdtState& s)
{
    b = CogBite{};
    b.nstep = 100;
    b.stpinc = 10;
    b.pdie = 2;
    b.smax = 0.0;
    b.lmax = 0.0;
    b.dtsub = 1.0;
    b.f0c = 1;
    b.stepMod = 0;
    b.dsmax = s.g.unit == 2 ? 0.01 : 0.25;
    b.dtmax = 0.0;
    b.clampForce = 0.0;
    b.ihtcof = s.g.unit == 2 ? 0.004 : 4.0;
    b.ihtcof2 = b.ihtcof;
    b.fricTyp = 0;
    b.frcfac = 0.4;
    b.userKey = "USER_HT.KEY";
    b.nomBite = 0.0;
    b.ht = s.slot[3];                      // 0x140797a10 then 0x140797d50(slot 3) copies all 0x80 bytes
}

// @src 0x140790d80  BITE deformation data
static void biteDeformLoad(CogBite& b, TextFile& f, int ver)
{
    std::string kw;
    float fv = 0.0f;                       // afStack_188
    double dv = 0.0;                       // uStack_f8
    std::string buf;                       // auStack_e8 (199)
    skipLines(f, 0);
    cscan(f, "%s %d\n", {&kw, &b.nstep});
    cscan(f, "%s %d\n", {&kw, &b.stpinc});
    cscan(f, "%s %d\n", {&kw, &b.pdie});
    cscan(f, "%s %f\n", {&kw, &fv});
    b.smax = static_cast<double>(fv);
    cscan(f, "%s %f\n", {&kw, &fv});
    b.lmax = static_cast<double>(fv);
    if (4000 < ver) {
        cscan(f, "%s %f\n", {&kw, &fv});
        b.clampForce = static_cast<double>(fv);
        // > 0.0 also sets a cogging global (0x14076bf10) that no MST writer reads
    }
    cscan(f, "%s %f\n", {&kw, &fv});
    b.dtsub = static_cast<double>(fv);
    cscan(f, "%s %d\n", {&kw, &b.stepMod});
    cscan(f, "%s %f\n", {&kw, &fv});
    b.dsmax = static_cast<double>(fv);
    cscan(f, "%s %f\n", {&kw, &fv});
    b.dtmax = static_cast<double>(fv);
    cscan(f, "%s %d\n", {&kw, &b.fricTyp});
    cscan(f, "%s %f\n", {&kw, &fv});
    b.frcfac = static_cast<double>(fv);
    cscan(f, "%s %lf\n", {&kw, &dv});
    b.ihtcof = dv;
    if (0xce4 < ver) {
        cscan(f, "%s %lf\n", {&kw, &dv});
        b.ihtcof2 = dv;
    }
    cscan(f, "%s \n", {&kw});
    fgetsKeep(f, buf, 199);
    chopNewline(buf);
    std::size_t slash = buf.rfind('/');    // strchr(p, '/') loop: the text after the last '/'
    b.userKey = slash == std::string::npos ? buf : buf.substr(slash + 1);
    if (b.userKey == "---") b.userKey = " ";
    if (0xc20 < ver) {
        cscan(f, "%s %f\n", {&kw, &fv});
        b.nomBite = static_cast<double>(fv);
    }
}

// @src 0x140790ca0  BITE_LoadFile: skip, bite HEAT (+0xc0), deformation data
static void biteLoad(CogBite& b, TextFile& f, int ver)
{
    skipLines(f, 0);
    heatLoad(b.ht, f, ver);
    biteDeformLoad(b, f, ver);
}

// The default manipulator 0x143da8fe0 that the SfOprCogData ctor builds (0x140793100 ->
// 0x140793270); nothing on the open path changes it afterwards.
// @src 0x140794bb0
static double manipDefaultLeftDis(const MdtState& s) { return s.defaultManip.leftDis; }
// @src 0x140794be0
static double manipDefaultRightDis(const MdtState& s) { return s.defaultManip.rightDis; }
// @src 0x140794c40
static int manipDefaultUseMstMan(const MdtState& s) { return s.defaultManip.useMstMan; }

// @src 0x140791580  PASS reset(pass, nBites): bites from the default bite (0x1407908e0), HEAT from
// slot 4 (0x140791750)
static void passReset(CogPass& p, int nBites, const MdtState& s)
{
    p.autoBite = 0;
    p.noBite = nBites;
    p.rotAng = 0.0;
    p.biteAngle = 0.0;
    p.minDieDis = 0.0;
    p.diePosMethod = 0;
    p.dieStrtPos = 0.0;
    p.dieStopPos = 0.0;
    p.dieStrtNod = 0;
    p.dieStopNod = 0;
    p.horizDieShift = 0.0;
    p.dieMovDir = -1;
    p.leftManDis = 0.0;
    p.rightManDis = 0.0;
    p.fe0 = 0;
    p.bitesToSkip = 0;
    p.radialFeedPerBite = 0.0;
    p.dwellTime = 0.0;
    p.bites.assign(nBites > 0 ? static_cast<std::size_t>(nBites) : 0u, s.defaultBite);
    p.ht = s.slot[4];
}

// @src 0x140792810  PASS_LoadFile (fields, then the pass HEAT)
static void passLoad(CogPass& p, TextFile& f, MdtState& s)
{
    const int ver = s.ver;
    std::string kw;
    std::int32_t nb = 0;                   // aiStack_b0
    float fv = 0.0f;                       // fStack_b8
    std::int32_t i1 = 0, i2 = 0, i3 = 0, i4 = 0;   // uStack_b4, auStack_20, uStack_24, uStack_28
    cscan(f, "%s %d\n", {&kw, &nb});
    if (nb == 0) nb = 1;
    p.bites.clear();                       // 0x140790c60
    passReset(p, nb, s);
    if (ver < 0xc21) cscan(f, "%s %f\n", {&kw, &fv});
    cscan(f, "%s %f\n", {&kw, &fv});
    p.rotAng = static_cast<double>(fv);
    if (2999 < ver) {
        cscan(f, "%s %f\n", {&kw, &fv});
        p.biteAngle = static_cast<double>(fv);
        cscan(f, "%s %f\n", {&kw, &fv});
        p.minDieDis = static_cast<double>(fv);
    }
    if (5999 < ver) {
        cscan(f, "%s %d\n", {&kw, &i1});
        p.diePosMethod = i1;
    }
    if (3999 < ver) {
        cscan(f, "%s %f\n", {&kw, &fv});
        p.dieStrtPos = static_cast<double>(fv);
        cscan(f, "%s %f\n", {&kw, &fv});
        p.dieStopPos = static_cast<double>(fv);
    }
    if (5999 < ver) {
        cscan(f, "%s %d\n", {&kw, &i2});
        p.dieStrtNod = i2;
        cscan(f, "%s %d\n", {&kw, &i2});
        p.dieStopNod = i2;
    }
    if (0xfa1 < ver) {
        cscan(f, "%s %d\n", {&kw, &i3});
        p.dieMovDir = i3;
    }
    if (0xc1b < ver) {
        cscan(f, "%s %d\n", {&kw, &i4});
        p.autoBite = i4;
    }
    if (ver < 0x1771) {
        p.leftManDis = manipDefaultLeftDis(s);     // 0x140794bb0
        p.rightManDis = manipDefaultRightDis(s);   // 0x140794be0
        p.fe0 = manipDefaultUseMstMan(s);          // 0x140794c40
    } else {
        cscan(f, "%s %f\n", {&kw, &fv});
        p.leftManDis = static_cast<double>(fv);
        cscan(f, "%s %f\n", {&kw, &fv});
        p.rightManDis = static_cast<double>(fv);
    }
    if (0x277d < ver) {
        cscan(f, "%s %f\n", {&kw, &fv});
        p.horizDieShift = static_cast<double>(fv);
    }
    if (10999 < ver) {
        nb = 0;
        cscan(f, "%s %d\n", {&kw, &nb});
        p.bitesToSkip = nb;
        fv = 0.0f;
        cscan(f, "%s %f\n", {&kw, &fv});
        p.radialFeedPerBite = static_cast<double>(fv);
        fv = 0.0f;
        cscan(f, "%s %f\n", {&kw, &fv});
        p.dwellTime = static_cast<double>(fv);
    }
    heatLoad(p.ht, f, ver);
}

// @src 0x140792630  pass with its bites (all of them, or the first when AutoBite is set)
static void passWithBitesLoad(CogPass& p, TextFile& f, MdtState& s)
{
    skipLines(f, 0);
    passLoad(p, f, s);
    if (p.autoBite == 0) {
        for (int i = 0; i < p.noBite; i = i + 1) {
            requireInput(static_cast<std::size_t>(i) < p.bites.size(), "bite count");
            biteLoad(p.bites[static_cast<std::size_t>(i)], f, s.ver);
        }
    } else {
        requireInput(!p.bites.empty(), "bite count");
        biteLoad(p.bites[0], f, s.ver);
    }
}

// @src 0x140791870  PASS array alloc(nPass, init, nBites): every pass is reset (0x140791580) and
// takes the default PASS's fields, with the start/stop positions of the default die and the
// manipulator distances of the default manipulator, which also go into the default PASS. The
// direction it then computes goes only into the default PASS (+0xc8).
static std::vector<CogPass> passArrayAlloc(int nPass, int init, int nBites, MdtState& s)
{
    std::vector<CogPass> out;
    if (nPass == 0) return out;
    out.resize(nPass > 0 ? static_cast<std::size_t>(nPass) : 0u);   // calloc(nPass, 0x108)
    CogPass& G = s.defaultPass;
    G.dieStrtPos = s.defaultDie.startPos;            // 0x140795230 copy of the default die, +0x440
    G.dieStopPos = s.defaultDie.stopPos;             // +0x448
    int dir = static_cast<int>(static_cast<double>(s.defaultDie.movDir));   // 0x140796530
    G.leftManDis = s.defaultManip.leftDis;           // 0x1407937c0 copy of the default manipulator
    G.rightManDis = s.defaultManip.rightDis;
    G.fe0 = s.defaultManip.useMstMan;
    if (init != 0) {
        for (int i = 0; i < nPass; i = i + 1) {
            CogPass& p = out[static_cast<std::size_t>(i)];
            passReset(p, nBites, s);
            p.rotAng = G.rotAng;
            p.biteAngle = G.biteAngle;
            p.autoBite = G.autoBite;
            p.minDieDis = G.minDieDis;
            p.diePosMethod = G.diePosMethod;
            p.dieStrtPos = G.dieStrtPos;
            p.dieStopPos = G.dieStopPos;
            p.dieStrtNod = G.dieStrtNod;
            p.dieStopNod = G.dieStopNod;
            p.leftManDis = G.leftManDis;
            p.rightManDis = G.rightManDis;
            p.fe0 = G.fe0;
            if (dir == 0) dir = -1;
            if (static_cast<double>(s.defaultDie.movType) == 1.0) {    // 0x140796550
                const int k = i + 1;
                G.dieMovDir = ((k % 2) == 0 ? -1 : 1) * dir;
            } else {
                G.dieMovDir = dir;
            }
            if (G.dieMovDir == 0) G.dieMovDir = -1;   // also prints the "calculated as zero" note
        }
    }
    return out;
}

// @src 0x14076bfe0  REHEAT init(r, nPass, nBites): passes (0x140791870), three HEATs from slots 0..2
static void reheatInit(CogReheat& r, int nPass, int nBites, MdtState& s)
{
    r.nPass = nPass;
    r.passes = passArrayAlloc(nPass, 1, nBites, s);
    for (int k = 0; k < 3; ++k) r.heat[k] = s.slot[k];    // 0x140797a10 + 0x140797d50(k)
}

// @src 0x14076c9b0  REHEAT_LoadFile
static void reheatLoad(CogReheat& r, TextFile& f, MdtState& s)
{
    std::string kw;
    std::int32_t n = 0;                    // auStack_18
    skipLines(f, 0);
    cscan(f, "%s %d\n", {&kw, &n});
    r.passes.clear();                      // 0x1407925a0
    reheatInit(r, n, 1, s);
    heatLoad(r.heat[0], f, s.ver);
    heatLoad(r.heat[1], f, s.ver);
    heatLoad(r.heat[2], f, s.ver);
    for (int i = 0; i < r.nPass; i = i + 1) passWithBitesLoad(r.passes[static_cast<std::size_t>(i)], f, s);
}

// geometry name line: "%s %d\n" GeoMod, "%s \n", fgets(+4, 0x3ff), trailing '\n' removed
static void geoFileLine(TextFile& f, int& geoMod, std::string& geoFile)
{
    std::string kw;
    cscan(f, "%s %d\n", {&kw, &geoMod});
    cscan(f, "%s \n", {&kw});
    fgetsKeep(f, geoFile, 0x3ff);
    chopNewline(geoFile);
}

// @src 0x140796f90  BILLET
static void billetLoad(CogBillet& b, TextFile& f, int ver, CogGlobals& g)
{
    std::string kw;
    float fv = 0.0f;                       // afStack_98
    skipLines(f, 2);
    cscan(f, "%s %d\n", {&kw, &b.geoTyp});
    if (0x27d7 < ver) geoFileLine(f, b.geoMod, b.geoFile);
    cscan(f, "%s %f\n", {&kw, &fv});
    b.length = static_cast<double>(fv);
    cscan(f, "%s %f\n", {&kw, &fv});
    b.dia = static_cast<double>(fv);
    if (9999 < ver) {
        cscan(f, "%s %f\n", {&kw, &fv});
        b.inDia = static_cast<double>(fv);
    }
    cscan(f, "%s %f\n", {&kw, &fv});
    b.height = static_cast<double>(fv);
    if (0xce5 < ver) {
        cscan(f, "%s %f\n", {&kw, &fv});
        b.width = static_cast<double>(fv);
    }
    cscan(f, "%s %f\n", {&kw, &fv});
    b.radius = static_cast<double>(fv);
    cscan(f, "%s %f\n", {&kw, &fv});
    b.layers = static_cast<double>(fv);
    cscan(f, "%s %d\n", {&kw, &b.mgnelm});
    cscan(f, "%s %f\n", {&kw, &fv});
    b.mgsizr = static_cast<double>(fv);
    cscan(f, "%s %d\n", {&kw, &b.mgtelm});
    cscan(f, "%s %f\n", {&kw, &fv});
    b.tmpbil = static_cast<double>(fv);
    cscan(f, "%s %d\n", {&kw, &b.bccMod});
    cscan(f, "%s %d\n", {&kw, &b.mergeL});
    g.mergeL = b.mergeL;                   // global 0x1437e3a7c
    cscan(f, "%s %s\n", {&kw, &b.keyBil});   // also copied to global 0x143db1600
    if (0xc1d < ver) {
        cscan(f, "%s %f\n", {&kw, &fv});
        b.axialSub = static_cast<double>(fv);
        g.axialSub = b.axialSub;           // global 0x143db1a00
    }
    if (0xc1f < ver) cscan(f, "%s %d\n", {&kw, &b.axialType});
    if (0x13eb < ver) cscan(f, "%s %d\n", {&kw, &b.doBilStraight});
    // ver > 0xc1b: KEY side read of KeyBil (0x14079a900, relative to the current directory) into
    // globals no MST writer reads (s1-03 5); not ported.
    if (0x27e1 < ver) cscan(f, "%s %d\n", {&kw, &b.martMod});
    if (0x27eb < ver) cscan(f, "%s %d\n", {&kw, &b.meshMod});
}

// @src 0x140795760  DIE
static void dieLoad(CogDie& d, TextFile& f, MdtState& s)
{
    const int ver = s.ver;
    std::string kw;
    float fv = 0.0f;                       // afStack_a8
    double dv = 0.0;                       // uStack_28
    skipLines(f, 2);
    cscan(f, "%s %d\n", {&kw, &d.geoTyp});
    if (0x27d7 < ver) geoFileLine(f, d.geoMod, d.geoFile);
    cscan(f, "%s %f\n", {&kw, &fv});
    d.length = static_cast<double>(fv);
    cscan(f, "%s %f\n", {&kw, &fv});
    d.width = static_cast<double>(fv);
    cscan(f, "%s %f\n", {&kw, &fv});
    d.height = static_cast<double>(fv);
    cscan(f, "%s %f\n", {&kw, &fv});
    d.radius = static_cast<double>(fv);
    cscan(f, "%s %f\n", {&kw, &fv});
    d.layers = static_cast<double>(fv);
    cscan(f, "%s %d\n", {&kw, &d.mgnelm});
    cscan(f, "%s %f\n", {&kw, &fv});
    d.mgsizr = static_cast<double>(fv);
    cscan(f, "%s %d\n", {&kw, &d.mgtelm});
    cscan(f, "%s %d\n", {&kw, &s.g.dieMov});           // global 0x1437e3444
    d.dieMov = s.g.dieMov;
    cscan(f, "%s %f\n", {&kw, &fv});
    d.tmpdie = static_cast<double>(fv);
    cscan(f, "%s %f\n", {&kw, &fv});
    d.spddie = static_cast<double>(fv);
    if (0x277e < ver) {
        cscan(f, "%s %lf\n", {&kw, &dv});
        d.cycles = dv;
        cscan(f, "%s %lf\n", {&kw, &dv});
        d.totStroke = dv;
        cscan(f, "%s %lf\n", {&kw, &dv});
        d.rodLength = dv;
    }
    if (ver < 0xc80) {
        s.g.noDies = 2;
        d.four = 0;
    } else {
        cscan(f, "%s %d\n", {&kw, &s.g.noDies});       // global 0x1437e3440
    }
    d.four = s.g.noDies == 4 ? 1 : 0;
    cscan(f, "%s %s\n", {&kw, &d.keyDie[0]});
    cscan(f, "%s %s\n", {&kw, &d.keyDie[1]});
    if (s.g.noDies == 4) {
        cscan(f, "%s %s\n", {&kw, &d.keyDie[2]});
        cscan(f, "%s %s\n", {&kw, &d.keyDie[3]});
    }
    if (2999 < ver) cscan(f, "%s %d\n", {&kw, &d.refPos});
    if (0xc1b < ver) {
        cscan(f, "%s %f\n", {&kw, &fv});
        d.overlap = static_cast<double>(fv);
    }
    if (0xc1d < ver) {
        cscan(f, "%s %f\n", {&kw, &fv});
        d.startPos = static_cast<double>(fv);
    }
    if (0xc1e < ver) {
        cscan(f, "%s %d\n", {&kw, &d.movDir});
        cscan(f, "%s %d\n", {&kw, &d.movType});
    }
    if (0xc1f < ver) {
        cscan(f, "%s %f\n", {&kw, &fv});
        d.stopPos = static_cast<double>(fv);
    }
    // ver > 0xc1b: KEY side reads of KeyDie1..4 (0x14079a900, 0x140795260); not ported (s1-03 5).
    if (0x27e1 < ver) cscan(f, "%s %d\n", {&kw, &d.martMod});
    if (0x27eb < ver) cscan(f, "%s %d\n", {&kw, &d.meshMod});
}

// @src 0x140793cc0  MANIP (ver > 0xc1c)
static void manipLoad(CogManip& m, TextFile& f, int ver)
{
    if (!(0xc1c < ver)) return;
    std::string kw;
    float fv = 0.0f;                       // afStack_98
    skipLines(f, 2);
    cscan(f, "%s %d\n", {&kw, &m.geoTyp});
    if (0x27d7 < ver) geoFileLine(f, m.geoMod, m.geoFile);
    cscan(f, "%s %f\n", {&kw, &fv});
    m.length = static_cast<double>(fv);
    cscan(f, "%s %f\n", {&kw, &fv});
    m.width = static_cast<double>(fv);
    cscan(f, "%s %f\n", {&kw, &fv});
    m.height = static_cast<double>(fv);
    cscan(f, "%s %f\n", {&kw, &fv});
    m.radius3 = static_cast<double>(fv);
    cscan(f, "%s %f\n", {&kw, &fv});
    m.layers = static_cast<double>(fv);
    cscan(f, "%s %d\n", {&kw, &m.mgnelm});
    cscan(f, "%s %f\n", {&kw, &fv});
    m.mgsizr = static_cast<double>(fv);
    cscan(f, "%s %d\n", {&kw, &m.mgtelm});
    cscan(f, "%s %f\n", {&kw, &fv});
    m.tmpdie = static_cast<double>(fv);
    for (int k = 0; k < 4; ++k) cscan(f, "%s %s\n", {&kw, &m.keyMan[k]});
    cscan(f, "%s %f\n", {&kw, &fv});
    m.startPos = static_cast<double>(fv);
    auto rf = [&](double& dst) {
        cscan(f, "%s %f\n", {&kw, &fv});
        dst = static_cast<double>(fv);
    };
    if (ver < 0x2774) {
        if (0xc1f < ver) {
            rf(m.leftDis);
            rf(m.rightDis);
            cscan(f, "%s %d\n", {&kw, &m.useSpring});
            rf(m.stiffness);
            rf(m.preload);
            rf(m.maxDisp);
            rf(m.width1);
            rf(m.height1);
            rf(m.radius1);
            rf(m.radius2);
        }
    } else {
        rf(m.leftDis);
        rf(m.rightDis);
        cscan(f, "%s %f\n", {&kw, &fv});
        m.useMstMan = truncToInt(fv);
        cscan(f, "%s %d\n", {&kw, &m.useSpring});
        rf(m.stiffness);
        rf(m.preload);
        rf(m.maxDisp);
        rf(m.width1);
        rf(m.height1);
        rf(m.radius1);
        rf(m.radius2);
    }
    // KEY side reads of KeyMan1..4 (0x14079a900, 0x1407937f0); not ported (s1-03 5).
    if (0x27e1 < ver) cscan(f, "%s %d\n", {&kw, &m.martMod});
    if (0x27eb < ver) cscan(f, "%s %d\n", {&kw, &m.meshMod});
}

// @src 0x14076aca0  SIMULATION PARAMETERS (cogging globals; UseManip into cog+0xc8)
static void simParamsLoad(CogData& c, TextFile& f, MdtState& s)
{
    std::string kw;
    std::int32_t iv = 0;                   // iStack_98 (UNIT, then UseManip)
    float fv = 0.0f;                       // afStack_94
    skipLines(f, 2);
    cscan(f, "%s %d\n", {&kw, &iv});
    // 0x14076bd70: outside 1..2 the original prints "COG_FNC_SetUnits" through 0x140b2f9d0 and
    // exits the GUI
    requireInput(iv == 0 || iv == 1, "UNIT out of range (the GUI exits on it)");
    s.g.unit = iv + 1;
    cscan(f, "%s %f\n", {&kw, &fv});
    s.proc.poserr = static_cast<double>(fv);      // 0x1407991c0
    cscan(f, "%s %f\n", {&kw, &fv});
    s.proc.interr = static_cast<double>(fv);      // 0x140799200
    cscan(f, "%s %f\n", {&kw, &fv});
    s.proc.cvgerr[0] = static_cast<double>(fv);   // 0x14076be80(1)
    cscan(f, "%s %f\n", {&kw, &fv});
    s.proc.cvgerr[1] = static_cast<double>(fv);   // 0x14076be80(2)
    if (0x13ec < s.ver) {
        cscan(f, "%s %f\n", {&kw, &fv});
        s.proc.avgstr = static_cast<double>(fv);  // 0x14076bfc0
        cscan(f, "%s %f\n", {&kw, &fv});
        s.proc.lmtstr = static_cast<double>(fv);  // 0x14076bf80
    }
    cscan(f, "%s %f\n", {&kw, &fv});
    s.proc.ctlpct = static_cast<double>(fv);      // 0x140796bb0
    if (0xc1c < s.ver) {
        cscan(f, "%s %d\n", {&kw, &iv});
        c.useManip = iv != 0 ? 1 : 0;
    }
}

// @src 0x14076b710  header: 5 lines, the 3rd gives the version
static void headerLoad(TextFile& f, int& ver)
{
    std::string buf;
    fgetsCog(f, buf, 0x1000);
    fgetsCog(f, buf, 0x1000);
    fgetsCog(f, buf, 0x1000);
    TextFile line(buf);
    cscan(line, "* MASTER DATA FILE %d", {&ver});        // sscanf 0x14146dfd4
    fgetsCog(f, buf, 0x1000);
    fgetsCog(f, buf, 0x1000);
}

// @src 0x14076a460  the sections before Process Data
static void mdtSectionsLoad(CogData& c, TextFile& f, MdtState& s)
{
    std::string kw;
    skipLines(f, 2);
    headerLoad(f, s.ver);
    CogHeat heat;                          // auStack_4a8
    heatCtor(heat, s.g.unit);
    cscan(f, "%s %s\n", {&kw, &c.prbid});
    cscan(f, "%s %[^\n]s", {&kw, &c.title});
    cscan(f, "\n", {});
    skipLines(f, 0);
    skipLines(f, 2);
    heatLoad(heat, f, s.ver);
    s.slot[0] = heat;                      // 0x140797b90
    heatLoad(heat, f, s.ver);
    s.slot[1] = heat;
    heatLoad(heat, f, s.ver);
    s.slot[2] = heat;
    skipLines(f, 2);
    CogPass pass;                          // auStack_718
    passReset(pass, 0, s);
    passLoad(pass, f, s);
    s.defaultPass = pass;                  // 0x140791c50 (frees the old bites first: 0x1407916e0
                                           // runs the empty BITE dtor 0x1407908d0 per bite, then
                                           // 0x140790c60); the local pass is freed the same way
    skipLines(f, 2);
    CogBite bite;                          // auStack_5f8
    biteCtor(bite, s);
    biteLoad(bite, f, s.ver);
    s.defaultBite = bite;                  // 0x140790a50
    billetLoad(c.billet, f, s.ver, s.g);
    dieLoad(c.die, f, s);
    manipLoad(c.manip, f, s.ver);
    simParamsLoad(c, f, s);
    skipLines(f, 2);
    std::int32_t nSim = 0;                 // aiStack_608
    cscan(f, "%s %d\n", {&kw, &nSim});
    std::string line;
    for (int i = 0; i < nSim; i = i + 1) fgetsCog(f, line, 100);
    std::int32_t nReheat = 0;              // auStack_428
    cscan(f, "%s %d\n", {&kw, &nReheat});
    c.reheats.clear();                     // 0x14076c920
    c.nReheats = nReheat;
    c.reheats.assign(nReheat > 0 ? static_cast<std::size_t>(nReheat) : 0u, CogReheat{});   // 0x14076c100
    for (auto& r : c.reheats) reheatInit(r, 1, 1, s);   // 0x14076c100(n, 1, 1, 1)
    for (int i = 0; i < c.nReheats; i = i + 1) reheatLoad(c.reheats[static_cast<std::size_t>(i)], f, s);
}

// ---- the scalar block of the model as bytes, so the %d writes overlap as in the original --------
namespace {

struct Image {
    unsigned char b[0x540] = {};
    template <class T> void put(unsigned off, T v) { std::memcpy(b + off, &v, sizeof v); }
    template <class T> T get(unsigned off) const { T v; std::memcpy(&v, b + off, sizeof v); return v; }
    void* at(unsigned off) { return b + off; }
};

void encode(const CogModel& m, Image& im)
{
    im.put<std::int32_t>(0x000, m.processType);
    im.put<std::int32_t>(0x004, m.temperatureType);
    im.put<std::int32_t>(0x008, m.useMandrelInt);
    im.put<std::uint8_t>(0x008, m.useMandrel);
    im.put<std::int32_t>(0x00c, m.f0c);
    im.put<std::int32_t>(0x010, m.f10);
    im.put<std::int32_t>(0x014, m.useDifferentDies);
    im.put<std::int32_t>(0x018, m.f18);
    im.put<std::uint8_t>(0x015, m.rotationalSymmetry);
    im.put<std::uint8_t>(0x4cc, m.useAdaptiveReheat);
    im.put<std::int32_t>(0x4d0, m.checkingMethod);
    im.put<double>(0x4d8, m.tempRange[0]);
    im.put<double>(0x4e0, m.tempRange[1]);
    im.put<double>(0x4e8, m.adaptiveHeatingTime);
    im.put<std::int32_t>(0x4f0, m.solverType);
    im.put<double>(0x4f8, m.swagingStroke);
    im.put<std::int32_t>(0x500, m.numStepsDsmax);
    im.put<std::uint8_t>(0x504, m.heatTransfer);
    im.put<std::uint8_t>(0x505, m.remainingCycleTimeDwell);
    im.put<double>(0x508, m.cycleTime);
    im.put<double>(0x510, m.dwellTime);
    im.put<double>(0x518, m.decompressionTime);
    im.put<std::int32_t>(0x520, m.bottomFixed);
    im.put<double>(0x528, m.clampForce);
    im.put<double>(0x530, m.clampSpeed);
    im.put<std::uint8_t>(0x538, m.byte538);
}

void decode(const Image& im, CogModel& m)
{
    m.processType = im.get<std::int32_t>(0x000);
    m.temperatureType = im.get<std::int32_t>(0x004);
    m.useMandrelInt = im.get<std::int32_t>(0x008);
    m.useMandrel = im.get<std::uint8_t>(0x008);
    m.f0c = im.get<std::int32_t>(0x00c);
    m.f10 = im.get<std::int32_t>(0x010);
    m.useDifferentDies = im.get<std::int32_t>(0x014);
    m.rotationalSymmetry = im.get<std::uint8_t>(0x015);
    m.f18 = im.get<std::int32_t>(0x018);
    m.useAdaptiveReheat = im.get<std::uint8_t>(0x4cc);
    m.checkingMethod = im.get<std::int32_t>(0x4d0);
    m.tempRange[0] = im.get<double>(0x4d8);
    m.tempRange[1] = im.get<double>(0x4e0);
    m.adaptiveHeatingTime = im.get<double>(0x4e8);
    m.solverType = im.get<std::int32_t>(0x4f0);
    m.swagingStroke = im.get<double>(0x4f8);
    m.numStepsDsmax = im.get<std::int32_t>(0x500);
    m.heatTransfer = im.get<std::uint8_t>(0x504);
    m.remainingCycleTimeDwell = im.get<std::uint8_t>(0x505);
    m.cycleTime = im.get<double>(0x508);
    m.dwellTime = im.get<double>(0x510);
    m.decompressionTime = im.get<double>(0x518);
    m.bottomFixed = im.get<std::int32_t>(0x520);
    m.clampForce = im.get<double>(0x528);
    m.clampSpeed = im.get<double>(0x530);
    m.byte538 = im.get<std::uint8_t>(0x538);
}

}  // namespace

// @src 0x140796700  BILLET ctor
static void billetCtor(CogBillet& b, const MdtState& s)
{
    b = CogBillet{};
    b.geoMod = 0;
    b.martMod = 0;
    b.geoFile = "DEFORM.GEO";              // 0x1437e3aa0
    b.geoTyp = 1;
    b.bccMod = 1;                          // 0x140796b30: global 0x1437e3a78 (.data 1, no writer here)
    b.axialType = 0;                       // global 0x143db1a0c (.bss)
    b.mergeL = s.g.mergeL;                 // 0x140796b60: global 0x1437e3a7c
    b.axialSub = s.g.axialSub;             // 0x1407978e0: global 0x143db1a00
    b.tmpbil = 68.0;                       // 0x140799260(object record 1, 0): record +0x158 (0x140798620)
    if (s.ctorUnit == 2) {                 // 0x14076bd50
        b.length = 40.0;
        b.dia = 20.0;
        b.inDia = 0.0;
        b.height = 20.0;
        b.width = 20.0;
        b.radius = 2.0;
    } else {
        b.length = 1000.0;
        b.dia = 500.0;
        b.inDia = 0.0;
        b.height = 500.0;
        b.width = 500.0;
        b.radius = 50.0;
    }
    b.layers = 10.0;
    b.meshMod = 0;
    b.mgnelm = 50;                         // +0x85c = 10000 is not kept
    b.mgsizr = 1.0;
    b.mgtelm = 4;
    b.keyBil = "BILLET.KEY";               // global 0x143db1600, reset by 0x140796650 just before
    b.doBilStraight = 0;
}

// @src 0x140794e70  DIE ctor (key names from the global table 0x143dacd70: "DIE%d.KEY")
static void dieCtor(CogDie& d, const MdtState& s)
{
    d = CogDie{};
    d.geoMod = 0;
    d.geoFile = "DEFORM.GEO";              // 0x1437e3458
    d.geoTyp = 0;
    d.dieMov = s.g.dieMov;                 // 0x140795590: global 0x1437e3444
    d.tmpdie = 68.0;                       // die object record 1 (0x143dac0b0) +0x158
    d.spddie = 1.0;
    d.cycles = 0.0;                        // record +0x190
    d.totStroke = 0.0;                     // record +0x180
    d.rodLength = 0.0;                     // record +0x188
    d.refPos = 1;
    d.movDir = 0;
    d.movType = 1;
    if (s.ctorUnit == 2) {
        d.length = 15.0;
        d.width = 10.0;
        d.height = 15.0;
        d.radius = 2.0;
    } else {
        d.length = 375.0;
        d.width = 250.0;
        d.height = 375.0;
        d.radius = 50.0;
    }
    d.layers = 5.0;
    d.overlap = 0.5;
    d.startPos = 0.0;
    d.stopPos = 0.0;
    d.meshMod = 0;
    d.mgnelm = 50;
    d.mgsizr = 2.0;
    d.mgtelm = 4;
    d.four = 0;
    d.keyDie[0] = "DIE1.KEY";
    d.keyDie[1] = "DIE2.KEY";
}

// @src 0x140793270  MANIP ctor (key names from the global table 0x143daaca0: "MAN%d.KEY")
static void manipCtor(CogManip& m, const MdtState& s)
{
    m = CogManip{};
    m.geoMod = 0;
    m.geoFile = "DEFORM.GEO";              // 0x1437e2dc0
    m.geoTyp = 0;
    m.tmpdie = 68.0;                       // manipulator object record 1 (0x143da8320) +0x158
    m.layers = 5.0;
    m.useMstMan = 0;
    m.useSpring = 1;
    if (s.ctorUnit == 2) {
        m.length = 10.0;
        m.width = 17.0;
        m.height = 10.0;
        m.radius3 = 0.4;
        m.radius1 = 0.8;
        m.radius2 = 0.4;
        m.width1 = 2.0;
        m.height1 = 5.0;
        m.startPos = 10.0;
        m.leftDis = 15.0;
        m.rightDis = 15.0;
        m.stiffness = 100.0;
        m.preload = 50.0;
        m.maxDisp = 100.0;
    } else {
        m.length = 375.0;
        m.width = 425.0;
        m.height = 250.0;
        m.radius3 = 10.0;
        m.radius1 = 20.0;
        m.radius2 = 10.0;
        m.width1 = 50.0;
        m.height1 = 125.0;
        m.startPos = 250.0;
        m.leftDis = 250.0;
        m.rightDis = 250.0;
        m.stiffness = 175.0;
        m.preload = 222400.0;
        m.maxDisp = 2540.0;
    }
    // +0x490 = 45.0 is not kept
    m.meshMod = 0;
    m.mgnelm = 50;
    m.mgsizr = 2.0;
    m.mgtelm = 4;
    m.keyMan[0] = "MAN1.KEY";
    m.keyMan[1] = "MAN2.KEY";
    m.keyMan[2] = "MAN3.KEY";
    m.keyMan[3] = "MAN4.KEY";
}

// @src 0x140767a20  SfOprCogData ctor, in its order: unit global, HEAT slots (0x1407979c0), default
// billet (0x140796650) and the cog's billet, default die (0x140794ce0) and the cog's die, default
// manipulator (0x140793100) and the cog's, default PASS (0x140791530), default BITE (0x1407906e0),
// one reheat (0x14076c100(1, 1, 1, 1)), PRBID/TITLE "COGGING".
static void cogDataCtor(CogData& c, MdtState& s)
{
    // @src 0x14076b3d0  cogging module globals (unit 2, CVGERR, AVGSTR, LMTSTR; the rest unused here)
    s.g.unit = 2;
    s.proc.cvgerr[0] = 0.005;              // 0x142239610
    s.proc.cvgerr[1] = 0.05;               // 0x1421ec2e0
    s.proc.lmtstr = 0.01;                  // 0x142203f60
    s.proc.avgstr = 1.0;                   // 0x1421249e0
    s.g.unit = s.ctorUnit;                 // 0x14076bd70(mainWindow vt+0x260)
    for (auto& h : s.slot) heatCtor(h, s.ctorUnit);
    billetCtor(c.billet, s);
    dieCtor(s.defaultDie, s);
    dieCtor(c.die, s);
    manipCtor(s.defaultManip, s);
    manipCtor(c.manip, s);
    passReset(s.defaultPass, 1, s);        // bites copied from the previous default BITE
    s.defaultPass.bites.clear();
    biteCtor(s.defaultBite, s);
    c.nReheats = 1;
    c.reheats.assign(1, CogReheat{});
    reheatInit(c.reheats[0], 1, 1, s);
    c.useManip = 1;
    c.prbid = "COGGING";
    c.title = "COGGING";
}

MdtResult readMdtFile(TextFile& f, CogModel& m, CogGlobals& g)
{
    MdtState s(g);
    s.ctorUnit = g.unit;
    // The process globals as the SfOprCogData ctor left them just before this read.
    {
        CogData scratch;
        cogDataCtor(scratch, s);
    }
    mdtSectionsLoad(m.cog, f, s);
    m.mdtVersion = s.ver;
    if (10999 < s.ver) {
        // @src 0x140761d90 (Process Data)
        Image im;
        encode(m, im);
        std::string kw;                    // auStack_1b8
        skipLines(f, 0);
        cscan(f, "%s %d\n", {&kw, im.at(0x000)});
        if (std::strcmp(kw.c_str(), "Process_Type") != 0) {
            decode(im, m);
            return MdtResult::Corrupt;
        }
        cscan(f, "%s %d\n", {&kw, im.at(0x004)});
        cscan(f, "%s %d\n", {&kw, im.at(0x008)});
        cscan(f, "%s %d\n", {&kw, im.at(0x4cc)});
        cscan(f, "%s %d\n", {&kw, im.at(0x4d0)});
        cscan(f, "%s %lg %lg\n", {&kw, im.at(0x4d8), im.at(0x4e0)});
        cscan(f, "%s %d\n", {&kw, im.at(0x4f0)});
        cscan(f, "%s %d\n", {&kw, im.at(0x014)});
        cscan(f, "%s %lg\n", {&kw, im.at(0x4e8)});
        cscan(f, "%s %lg\n", {&kw, im.at(0x4f8)});
        cscan(f, "%s %lg\n", {&kw, im.at(0x510)});
        cscan(f, "%s %lg\n", {&kw, im.at(0x508)});
        cscan(f, "%s %lg\n", {&kw, im.at(0x518)});
        cscan(f, "%s %d\n", {&kw, im.at(0x520)});
        cscan(f, "%s %d\n", {&kw, im.at(0x504)});
        cscan(f, "%s %d\n", {&kw, im.at(0x505)});         // also the low byte of Cycle_Time
        cscan(f, "%s %lg\n", {&kw, im.at(0x528)});
        cscan(f, "%s %lg\n", {&kw, im.at(0x530)});
        cscan(f, "%s %d\n", {&kw, im.at(0x015)});         // also +0x16..+0x18
        cscan(f, "%s %d\n", {&kw, im.at(0x500)});
        decode(im, m);
    }
    return MdtResult::Ok;
}

// @src 0x1407613b0  (0x1407656d0 -> 0x140767a20, then 0x1407614e0 and the process-data fields)
void cogModelDefaults(CogModel& m, int unit, CogGlobals& g)
{
    m = CogModel{};                        // 0x1407614e0 / 0x1407613b0 values are the member defaults
    MdtState s(g);
    s.ctorUnit = unit;
    cogDataCtor(m.cog, s);
}

// makeProps (no Project in its signature): the conversion's project when there is one, so the ctor's
// effects on the process globals stay (packet MDT); tests without a project get scratch globals.
void cogModelDefaults(CogModel& m, int unit)
{
    if (Project* p = currentProject()) {
        cogModelDefaults(m, unit, p->p06Cog);
        return;
    }
    CogGlobals g;
    cogModelDefaults(m, unit, g);
}

data::CogProcess& cogProcess(CogGlobals& g)
{
    if (!g.proc) g.proc = std::make_shared<data::CogProcess>();
    return *g.proc;
}

// @src 0x140760430 -> 0x140761c90 -> 0x140761d90
void readCoggingMdt(Project& p, Operation& op, CoggingProps& props, Ctx& ctx)
{
    // The GUI unit the SfOprCogData ctor (0x140767a20) reads; it is also the unit global the read
    // starts from. It only shapes defaults the MDT overwrites (s1-03 5).
    const int unit = ctx.guiUnitOverride == 2 || (ctx.guiUnitOverride == 0 && p.unit == 2) ? 2 : 1;
    if (!props.model) {
        props.model = std::make_shared<CogModel>();
        cogModelDefaults(*props.model, unit, p.p06Cog);
    }
    String dir = taskDirAbs(op);           // 0x140050df0
    if (dir.isNull()) return;              // no owner item: -1, nothing read
    String path = qt::convertSeparators(dir + qt::kSep + qt::L("Cogging.MDT"));   // 0x140761580
    p.p06Cog.unit = unit;
    TextFile f;
    if (!TextFile::open(p, path, f)) {
        ctx.warn("FILE MISSING! The MDT file " + qt::toAscii(path) +
                 " for the current cogging problem does not exist.");
        return;
    }
    if (readMdtFile(f, *props.model, p.p06Cog) == MdtResult::Corrupt)
        ctx.warn("FILE CORRUPT! The MDT file " + qt::toAscii(path) +
                 " for the current cogging problem is corrupt.");
}

// @src 0x1407603c0
bool loadCoggingMdtIfModel(Project& p, Operation& op, CoggingProps& props, Ctx& ctx)
{
    const bool has = props.model != nullptr;
    if (has) readCoggingMdt(p, op, props, ctx);   // 0x140761c90
    return has;
}

}  // namespace m2m::datafiles
