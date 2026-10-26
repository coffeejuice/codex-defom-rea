#include "../support/test_paths.hpp"
// ctest-labels: corpus
// P11 cross-check: the Cogging writer (src/ops/cogging*.cpp) against the s1-03 Python prototype
// (tools/cogw.py), on models dumped by gen_xcheck.py. Shared emitters, the stream's number text and
// the model accessors are test doubles here that print the prototype's "GEN:<addr>" markers, so the
// comparison covers exactly the cogging-specific lines and the simulation count.
// Without an argument the test runs gen_xcheck.py (python -I, explicit argv) on every Cogging.MDT of
// the corpus ($M2M_CORPUS) into ./p11_xcheck first. With <dir> it uses dumps made before.
// The writer's sources are compiled into this file with namespace m2m renamed to m2m_xc, so the
// doubles below never clash with the core library's symbols (stubs or the real code once merged).
#include <process.h>
#include <windows.h>

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

#define m2m m2m_xc
#include "../../src/ops/cogging_data.cpp"
#include "../../src/ops/cogging.cpp"
#include "../../src/ops/cogging_blocks.cpp"
#include "../../src/ops/props_base.cpp"   // Props::applyValue is out of line (P15)
#include "../../src/model/project_owned_storage.cpp" // renamed Project storage owner for this double
#include "../../src/model/operation_info_guard.cpp" // renamed OPI guard identity
#include "../../src/model/operation_info_model_cache.cpp" // renamed canonical OPI+160 observation
#include "../../src/model/current_operation_binding.cpp" // renamed guarded current binding

namespace m2m {
// This renamed packet double never constructs a Session receiver registry.
SubProject::~SubProject() noexcept = default;

namespace qt {
String String::latin1(const char* s) { return latin1(std::string_view(s ? s : "")); }
String String::latin1(std::string_view s)
{
    std::u16string u;
    for (unsigned char c : s) u += static_cast<char16_t>(c);
    return String(std::move(u));
}
// QString::arg(int): replace the lowest %n (enough for the "RH%1-PS%2-..." names)
String arg(const String& fmt, int a, int, int, char16_t)
{
    std::u16string s = fmt.u16();
    int best = 100;
    size_t pos = std::u16string::npos;
    for (size_t i = 0; i + 1 < s.size(); i++)
        if (s[i] == u'%' && s[i + 1] >= u'1' && s[i + 1] <= u'9' && s[i + 1] - u'0' < best) {
            best = s[i + 1] - u'0';
            pos = i;
        }
    if (pos == std::u16string::npos) return fmt;
    std::string n = std::to_string(a);
    s.replace(pos, 2, std::u16string(n.begin(), n.end()));
    return String(s);
}
// props_base.cpp (P15) needs these three; the cross-check never applies property values
String number(int v) { return String::latin1(std::to_string(v)); }
int toInt(const String& s, bool* ok) { if (ok) *ok = true; return std::atoi(std::string(s.u16().begin(), s.u16().end()).c_str()); }
double toDouble(const String& s, bool* ok) { if (ok) *ok = true; return std::atof(std::string(s.u16().begin(), s.u16().end()).c_str()); }
String fileName(const String& p)
{
    const auto& s = p.u16();
    size_t k = s.find_last_of(u"/\\");
    return k == std::u16string::npos ? p : String(s.substr(k + 1));
}
}  // namespace qt

namespace mst {
// This standalone double appends bytes immediately; it has no pending text.
Stream& Stream::flush() { return *this; }

Stream& Stream::operator<<(const char* lit) { buf_ += lit; return *this; }
Stream& Stream::operator<<(int v) { buf_ += std::to_string(v); return *this; }
Stream& Stream::operator<<(double v)
{
    // prototype g(): "%g", zero (either sign) prints "0"
    if (v == 0.0) { buf_ += "0"; return *this; }
    char b[64];
    std::snprintf(b, sizeof b, "%g", v);
    buf_ += b;
    return *this;
}
Stream& Stream::operator<<(const String& s) { for (char16_t c : s.u16()) buf_ += static_cast<char>(c); return *this; }
Stream& Stream::endl() { buf_ += "\n"; return *this; }

static void mark(W& w, const char* m) { w.ts << m << endl; }
void emitSimHeader(W& w, int n, Operation*) { w.ts << "GEN:140131c10 SIMULATION " << n << endl; }
void emitDbread(W& w, const String&, int, int) { mark(w, "GEN:1401343e0"); }
void emitSimCtrl(W& w, Project&, const String&, int, Operation*) { mark(w, "GEN:140134bf0"); }
void emitMaterials(W& w, Project&, const String&, int, Operation*) { mark(w, "GEN:140134db0"); }
void emitMeshSettings(W& w, Project&, const String&, int, Operation*) { mark(w, "GEN:1401360e0"); }
void emitObjects(W& w, Project&, const String&, int, Operation*, bool) { mark(w, "GEN:140135220"); }
void emitContact(W& w, Project&, const String&, int, Operation*) { mark(w, "GEN:140135da0"); }
void emitContactFlags(W& w, Project&, const String&, int, Operation*) { mark(w, "GEN:140136040"); }
void emitKfreadUser(W& w, Operation*) { mark(w, "GEN:140134680"); }
void emitSimnam(W& w, const String& name) { w.ts << "SIMNAM" << endl << name << endl; }
void emitOprnam(W& w, const String&) { mark(w, "GEN:OPRNAM/CURSIM/GENDB"); }
void emitCursim(W&, int, int, int) {}
void emitGendb(W&, const String&, int, int, bool) {}

void OpClass::writeMst(W&, Project&, const String&, int*, Operation*, int, int) {}
void OpClass::writeSetup(W&, Project&, const String&, int, Operation*) {}
bool OpClass::writesDimNstart() const { return false; }
void OpClass::writeExtra(W&, Project&, const String&, int, Operation*) {}
int OpClass::dim() const { return 3; }
void OpClass::simList(int, int*, Operation*, std::vector<SimEntry>&) {}
std::shared_ptr<Props> OpClass::makeProps(int) const { return nullptr; }
String OpClass::displayName() const { return key(); }
Props* OpClass::propsOf(Operation* op) const { return op ? op->props.get() : nullptr; }
}  // namespace mst

static bool g_firstOp = false;
bool isChainHead(const Operation&) { return g_firstOp; }
int flowNumber(const Operation&) { return 5; }
std::vector<PropNode> defaultPropTree(const String&) { return {}; }
void Props::postLoad(Project&, Ctx&) {}
namespace datafiles {
void cogModelDefaults(data::CogModel&, int) {}
}
namespace mst {
void simListCogging(OpClass&, int, int*, Operation*, std::vector<SimEntry>&) {}
}
void CoggingProps::postLoad(Project&, Ctx&) {}
}  // namespace m2m

using namespace m2m;

static std::map<std::string, std::string> g_kv;
static double D(const std::string& k) { auto it = g_kv.find(k); return it == g_kv.end() ? 0.0 : std::strtod(it->second.c_str(), nullptr); }
static int I(const std::string& k) { auto it = g_kv.find(k); return it == g_kv.end() ? 0 : std::atoi(it->second.c_str()); }

static void load(const std::string& path, data::CogModel& m)
{
    g_kv.clear();
    std::ifstream f(path);
    std::string k, v;
    while (f >> k >> v) g_kv[k] = v;
    m.processType = I("pd.Process_Type");
    m.temperatureType = I("pd.Temperaure_Type");
    m.useMandrel = static_cast<std::uint8_t>(I("pd.Use_Mandrel"));
    m.checkingMethod = I("pd.Checking_Method");
    m.bottomFixed = I("pd.Bottom_Fixed");
    m.numStepsDsmax = I("pd.Num_Steps_DSMAX");
    m.useAdaptiveReheat = static_cast<std::uint8_t>(I("pd.Use_Adaptive_Reheat"));
    m.heatTransfer = static_cast<std::uint8_t>(I("pd.Heat_Transfer"));
    m.remainingCycleTimeDwell = static_cast<std::uint8_t>(I("pd.Remaining"));
    m.rotationalSymmetry = static_cast<std::uint8_t>(I("pd.RotationalSymmetry"));
    m.tempRange[0] = D("pd.TR0");
    m.tempRange[1] = D("pd.TR1");
    m.adaptiveHeatingTime = D("pd.Adaptive_Heating_Time");
    m.swagingStroke = D("pd.Swaging_Stroke");
    m.dwellTime = D("pd.Dwell_Time");
    m.cycleTime = D("pd.Cycle_Time");
    m.decompressionTime = D("pd.Decompression_Time");
    auto& c = m.cog;
    c.useManip = static_cast<std::uint8_t>(I("UseManip"));
    c.billet.geoTyp = I("billet.GeoTyp");
    c.billet.axialType = I("billet.AxialType");
    c.billet.doBilStraight = I("billet.DoBilStraight");
    c.billet.length = D("billet.Length");
    c.billet.dia = D("billet.Dia");
    c.billet.inDia = D("billet.InDia");
    c.billet.height = D("billet.Height");
    c.billet.width = D("billet.Width");
    c.billet.axialSub = D("billet.AxialSub");
    c.die.four = static_cast<std::uint8_t>(I("die.four"));
    c.die.movType = I("die.MovType");
    c.die.length = D("die.Length");
    c.die.cycles = D("die.CYCLES");
    c.die.totStroke = D("die.TOTSTROKE");
    c.manip.useSpring = I("manip.UseSpring");
    c.manip.length = D("manip.Length");
    c.manip.startPos = D("manip.StartPos");
    c.manip.leftDis = D("manip.LeftDis");
    c.manip.rightDis = D("manip.RightDis");
    c.nReheats = I("nReheats");
    c.reheats.assign(static_cast<size_t>(c.nReheats), {});
    for (int r = 1; r <= c.nReheats; r++) {
        auto& R = c.reheats[static_cast<size_t>(r - 1)];
        R.nPass = I("R" + std::to_string(r) + ".nPass");
        R.passes.assign(static_cast<size_t>(R.nPass), {});
        for (int p = 1; p <= R.nPass; p++) {
            auto& P = R.passes[static_cast<size_t>(p - 1)];
            std::string pre = "R" + std::to_string(r) + ".P" + std::to_string(p) + ".";
            P.noBite = I(pre + "nBites");
            P.autoBite = I(pre + "AutoBite");
            P.diePosMethod = I(pre + "DiePosMethod");
            P.dieStrtNod = I(pre + "DieStrtNod");
            P.dieStopNod = I(pre + "DieStopNod");
            P.dieMovDir = I(pre + "DieMovDir");
            P.bitesToSkip = I(pre + "BitesToSkip");
            P.rotAng = D(pre + "RotAng");
            P.biteAngle = D(pre + "BiteAngle");
            P.minDieDis = D(pre + "MinDieDis");
            P.dieStrtPos = D(pre + "DieStrtPos");
            P.dieStopPos = D(pre + "DieStopPos");
            P.horizDieShift = D(pre + "HorizDieShift");
            P.radialFeedPerBite = D(pre + "RadialFeedPerBite");
            P.dwellTime = D(pre + "DwellTime");
            P.ht.nstep = I(pre + "HT.NSTEP");
            P.ht.stpinc = I(pre + "HT.STPINC");
            P.ht.solmtt = I(pre + "HT.SOLMTT");
            P.ht.tmax = D(pre + "HT.TMAX");
            int nb = I(pre + "nb");
            P.bites.assign(static_cast<size_t>(nb), {});
            for (int b = 1; b <= nb; b++) {
                auto& B = P.bites[static_cast<size_t>(b - 1)];
                std::string bp = pre + "B" + std::to_string(b) + ".";
                B.nomBite = D(bp + "NomBite");
                B.ht.nstep = I(bp + "HT.NSTEP");
                B.ht.stpinc = I(bp + "HT.STPINC");
                B.ht.solmtt = I(bp + "HT.SOLMTT");
                B.ht.tmax = D(bp + "HT.TMAX");
            }
        }
    }
}

static std::string slurp(const std::string& p)
{
    std::ifstream f(p, std::ios::binary);
    std::stringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

static int generate(const std::string& dir)
{
    const char* repo = std::getenv("DEF_PORT_REPO");
    const char* corpus = std::getenv("M2M_CORPUS");
    if (!repo || !corpus) { std::cerr << "DEF_PORT_REPO / M2M_CORPUS not set\n"; return -1; }
    std::string script = std::string(repo) + "/apps/moproj2mst/tests/p11/gen_xcheck.py";
    std::string tools = std::string(repo) + "/docs/re/moproj2mst/s1-03/tools";
    const char* args[] = {"python", "-B", "-I", script.c_str(), tools.c_str(), dir.c_str(), "--corpus", corpus, nullptr};
    std::fflush(nullptr);
    return static_cast<int>(_spawnvp(_P_WAIT, "python", args));
}

int main(int argc, char** argv)
{
    SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX);
    std::string dir = argc >= 2 ? argv[1] : (m2m_test::scratchRoot() / "p11_xcheck").string();
    if (argc < 2 && generate(dir) != 0) { std::cerr << "gen_xcheck.py failed\n"; return 1; }
    std::ifstream cases(dir + "/cases.txt");
    std::string line;
    int n = 0, bad = 0;
    while (std::getline(cases, line)) {
        if (line.empty()) continue;
        std::string name = line.substr(0, line.find('\t'));
        for (int fo = 0; fo < 2; fo++) {
            auto props = std::make_shared<CoggingProps>();
            props->model = std::make_shared<data::CogModel>();
            load(dir + "/" + name + ".model", *props->model);
            Project prj{};
            Operation op{};
            op.props = props;
            op.name = qt::L("Cog");
            mst::Stream ts(1252);
            mst::State st;
            Ctx ctx;
            mst::W w{ts, st, ctx};
            auto cls = mst::makeCogging();
            int sim = 4;
            g_firstOp = fo != 0;
            std::string got;
            try {
                cls->writeMst(w, prj, qt::L("X.DB"), &sim, &op, 0, 0);
                got = ts.bytes() + "END " + std::to_string(sim) + "\n";
            } catch (const std::exception& e) {
                got = std::string("EXCEPTION ") + e.what() + "\n";
            }
            std::string exp = slurp(dir + "/" + name + ".f" + std::to_string(fo) + ".expect");
            ++n;
            if (got != exp) {
                ++bad;
                size_t i = 0;
                while (i < got.size() && i < exp.size() && got[i] == exp[i]) ++i;
                size_t ls = exp.rfind('\n', i == 0 ? 0 : i - 1);
                ls = (ls == std::string::npos) ? 0 : ls + 1;
                std::cout << "DIFF " << line << " firstOp=" << fo << " at byte " << i << "\n  exp: "
                          << exp.substr(ls, exp.find('\n', i) - ls) << "\n  got: "
                          << got.substr(ls, got.find('\n', i) - ls) << "\n";
                std::ofstream(dir + "/" + name + ".f" + std::to_string(fo) + ".got", std::ios::binary) << got;
            }
        }
    }
    std::cout << (n - bad) << "/" << n << " identical\n";
    return bad == 0 && n > 0 ? 0 : 1;
}
