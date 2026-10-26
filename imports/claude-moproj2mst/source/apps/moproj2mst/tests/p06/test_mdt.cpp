// P06: Cogging.MDT reader against the s1-03 Python prototype on every corpus MDT
// (golden data/mdt_golden.txt from gen_mdt_golden.py), plus the byte effects of s1-03 5.
#include <testkit/check.hpp>

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <sstream>

#include "../../src/datafiles/readers.hpp"

using namespace m2m::datafiles;
using m2m::data::CogHeat;
using m2m::data::CogModel;

namespace {

int unitRead = 0;

std::string h(double v)
{
    std::uint64_t u;
    std::memcpy(&u, &v, 8);
    char b[32];
    std::snprintf(b, sizeof b, "%016llx", static_cast<unsigned long long>(u));
    return b;
}

std::string fmt(const char* f, ...)
{
    char b[2048];
    va_list ap;
    va_start(ap, f);
    std::vsnprintf(b, sizeof b, f, ap);
    va_end(ap);
    return b;
}

void heat(std::vector<std::string>& out, const std::string& pre, const CogHeat& t)
{
    out.push_back(fmt("%s.ht %d %d %d %s %s %s %s|%s", pre.c_str(), t.active, t.nstep, t.stpinc, h(t.tmax).c_str(),
                      h(t.dtpmax).c_str(), h(t.ihtcof).c_str(), h(t.ihtcof2).c_str(), t.userKey.c_str()));
}

std::vector<std::string> dump(const CogModel& m)
{
    std::vector<std::string> out;
    const auto& c = m.cog;
    const auto& B = c.billet;
    const auto& D = c.die;
    const auto& M = c.manip;
    out.push_back(fmt("ver %d", m.mdtVersion));
    out.push_back(fmt("billet %d %s %s %s %s %s %s %s %s %d %d|%s|%s %d %d", B.geoTyp, h(B.length).c_str(),
                      h(B.dia).c_str(), h(B.inDia).c_str(), h(B.height).c_str(), h(B.width).c_str(),
                      h(B.radius).c_str(), h(B.layers).c_str(), h(B.tmpbil).c_str(), B.bccMod, B.mergeL,
                      B.keyBil.c_str(), h(B.axialSub).c_str(), B.axialType, B.doBilStraight));
    out.push_back(fmt("die %s %d %s %s %s %s %d %d %d", h(D.length).c_str(), D.dieMov, h(D.tmpdie).c_str(),
                      h(D.spddie).c_str(), h(D.cycles).c_str(), h(D.totStroke).c_str(), D.four ? 4 : 2,
                      int(D.four), D.movType));
    out.push_back(fmt("manip %s %s %s %s %d %d", h(M.length).c_str(), h(M.startPos).c_str(), h(M.leftDis).c_str(),
                      h(M.rightDis).c_str(), M.useMstMan, M.useSpring));
    out.push_back(fmt("sim %d %d", unitRead, int(c.useManip)));
    out.push_back(fmt("reheats %d", static_cast<int>(c.reheats.size())));
    for (std::size_t r = 0; r < c.reheats.size(); ++r) {
        const auto& R = c.reheats[r];
        out.push_back(fmt("r%d passes %d", int(r), static_cast<int>(R.passes.size())));
        for (std::size_t i = 0; i < R.passes.size(); ++i) {
            const auto& p = R.passes[i];
            std::string pre = fmt("r%dp%d", int(r), int(i));
            out.push_back(fmt("%s %d %s %s %s %d %s %s %d %d %d %d %s %s %s %d %s %s", pre.c_str(), p.noBite,
                              h(p.rotAng).c_str(), h(p.biteAngle).c_str(), h(p.minDieDis).c_str(), p.diePosMethod,
                              h(p.dieStrtPos).c_str(), h(p.dieStopPos).c_str(), p.dieStrtNod, p.dieStopNod,
                              p.dieMovDir, p.autoBite, h(p.leftManDis).c_str(), h(p.rightManDis).c_str(),
                              h(p.horizDieShift).c_str(), p.bitesToSkip, h(p.radialFeedPerBite).c_str(),
                              h(p.dwellTime).c_str()));
            heat(out, pre, p.ht);
            std::size_t nb = p.autoBite == 0 ? p.bites.size() : 1;
            for (std::size_t j = 0; j < nb; ++j) {
                const auto& b = p.bites[j];
                std::string bp = pre + fmt("b%d", int(j));
                out.push_back(fmt("%s %d %d %d %s %s %s %s %d %s %s %d %s %s %s|%s", bp.c_str(), b.nstep, b.stpinc,
                                  b.pdie, h(b.smax).c_str(), h(b.lmax).c_str(), h(b.clampForce).c_str(),
                                  h(b.dtsub).c_str(), b.stepMod, h(b.dsmax).c_str(), h(b.dtmax).c_str(), b.fricTyp,
                                  h(b.frcfac).c_str(), h(b.ihtcof).c_str(), h(b.nomBite).c_str(),
                                  b.userKey.c_str()));
                heat(out, bp, b.ht);
            }
        }
    }
    if (m.mdtVersion > 10999)
        out.push_back(fmt("pd %d %d %d %d %d %s %s %d %d %s %s %s %s %s %d %d %d %s %s %d %d", m.processType,
                          m.temperatureType, m.useMandrelInt, int(m.useAdaptiveReheat), m.checkingMethod,
                          h(m.tempRange[0]).c_str(), h(m.tempRange[1]).c_str(), m.solverType,
                          m.useDifferentDies & 0xff, h(m.adaptiveHeatingTime).c_str(), h(m.swagingStroke).c_str(),
                          h(m.dwellTime).c_str(), h(m.cycleTime).c_str(), h(m.decompressionTime).c_str(),
                          m.bottomFixed, int(m.heatTransfer), int(m.remainingCycleTimeDwell),
                          h(m.clampForce).c_str(), h(m.clampSpeed).c_str(), int(m.rotationalSymmetry),
                          m.numStepsDsmax));
    return out;
}

std::string readAll(const std::filesystem::path& p)
{
    std::ifstream in(p, std::ios::binary);
    std::ostringstream ss;
    ss << in.rdbuf();
    return ss.str();
}

std::string corpusFile(const char* rel)
{
    const char* corpus = std::getenv("M2M_CORPUS");
    if (!corpus) return {};
    return readAll(std::filesystem::path(corpus) / rel);
}

const char* kB1 = "01_COG_P1_B1/Operations/Task00003/Cogging.MDT";

}  // namespace

TEST_CASE(corpus_mdt_matches_prototype)
{
    const char* corpus = std::getenv("M2M_CORPUS");
    REQUIRE(corpus != nullptr);
    auto golden = testkit::repo_root() / "apps/moproj2mst/tests/p06/data/mdt_golden.txt";
    std::ifstream g(golden);
    REQUIRE(g.good());
    std::vector<std::pair<std::string, std::vector<std::string>>> cases;
    for (std::string line; std::getline(g, line);) {
        if (line.rfind("FILE ", 0) == 0) cases.push_back({line.substr(5), {}});
        else if (!cases.empty()) cases.back().second.push_back(line);
    }
    REQUIRE(cases.size() == 27);
    int files = 0;
    for (const auto& [rel, want] : cases) {
        std::string raw = readAll(std::filesystem::path(corpus) / rel);
        REQUIRE(!raw.empty());
        TextFile f(raw);
        CogModel m;
        cogModelDefaults(m, 1);
        m2m::Project::CogGlobals glob;
        REQUIRE(readMdtFile(f, m, glob) == MdtResult::Ok);
        unitRead = glob.unit - 1;
        auto got = dump(m);
        bool same = got.size() == want.size();
        for (std::size_t i = 0; same && i < got.size(); ++i) {
            if (got[i] != want[i]) {
                std::printf("%s line %zu\n  got  %s\n  want %s\n", rel.c_str(), i, got[i].c_str(), want[i].c_str());
                same = false;
            }
        }
        if (got.size() != want.size()) std::printf("%s: %zu lines, want %zu\n", rel.c_str(), got.size(), want.size());
        CHECK(same);
        ++files;
    }
    CHECK_EQ(files, 27);
}

TEST_CASE(cycle_time_loses_low_byte)
{
    // COG_P1_B1: Cycle_Time 2.46; the %d of Remaining_Cycle_Time_Dwell (0) clears its low byte.
    std::string raw = corpusFile(kB1);
    REQUIRE(!raw.empty());
    TextFile f(raw);
    CogModel m;
    cogModelDefaults(m, 1);
    m2m::Project::CogGlobals glob;
    REQUIRE(readMdtFile(f, m, glob) == MdtResult::Ok);
    double v = 2.46;
    unsigned char b[8];
    std::memcpy(b, &v, 8);
    b[0] = 0;
    std::memcpy(&v, b, 8);
    CHECK_BITS(m.cycleTime, v);
    CHECK(m.cycleTime != 2.46);
    CHECK_EQ(m.temperatureType, 2);
    CHECK_EQ(int(m.heatTransfer), 1);
}

TEST_CASE(int_writes_overlap_neighbours)
{
    // Remaining 0x12000000 puts 0x12 into Cycle_Time's low byte; RotationalSymmetry 0x0a000201
    // writes +0x15..+0x18: byte +0x15 = 1, +0x16 = 2, +0x17 = 0, +0x18 (low byte of f18) = 0x0a.
    std::string raw = corpusFile(kB1);
    REQUIRE(!raw.empty());
    auto rep = [&](const std::string& from, const std::string& to) {
        auto at = raw.find(from);
        REQUIRE(at != std::string::npos);
        raw.replace(at, from.size(), to);
    };
    rep("Remaining_Cycle_Time_Dwell      0", "Remaining_Cycle_Time_Dwell      301989888");
    rep("RotationalSymmetry      0", "RotationalSymmetry      167772673");
    TextFile f(raw);
    CogModel m;
    cogModelDefaults(m, 1);
    m2m::Project::CogGlobals glob;
    REQUIRE(readMdtFile(f, m, glob) == MdtResult::Ok);
    unsigned char b[8];
    std::memcpy(b, &m.cycleTime, 8);
    CHECK_EQ(int(b[0]), 0x12);
    CHECK_EQ(int(m.remainingCycleTimeDwell), 0);
    CHECK_EQ(int(m.rotationalSymmetry), 1);
    CHECK_EQ(m.useDifferentDies, 0x00020101);      // byte 0 = Use_DifferentDies 1, bytes 1..3 = 01 02 00
    CHECK_EQ(m.f18, 0x0a);                          // default 1, low byte replaced
}

TEST_CASE(corrupt_process_data)
{
    std::string raw = corpusFile(kB1);
    REQUIRE(!raw.empty());
    auto at = raw.find("Process_Type      0");
    REQUIRE(at != std::string::npos);
    raw.replace(at, 19, "Process_Typo      1");
    TextFile f(raw);
    CogModel m;
    cogModelDefaults(m, 1);
    m2m::Project::CogGlobals glob;
    CHECK(readMdtFile(f, m, glob) == MdtResult::Corrupt);
    CHECK_EQ(m.processType, 1);                     // stored before the keyword check
    CHECK_EQ(m.temperatureType, 1);                 // the rest keeps the defaults
}

TEST_CASE(ctor_defaults_when_mdt_missing)
{
    CogModel si;
    cogModelDefaults(si, 1);
    CHECK_EQ(si.cog.prbid, std::string("COGGING"));
    CHECK(si.cog.billet.length == 1000.0);
    CHECK_EQ(si.cog.billet.geoTyp, 1);
    CHECK(si.cog.billet.tmpbil == 68.0);
    CHECK_EQ(si.cog.billet.keyBil, std::string("BILLET.KEY"));
    CHECK(si.cog.die.length == 375.0);
    CHECK_EQ(si.cog.die.keyDie[1], std::string("DIE2.KEY"));
    CHECK_EQ(si.cog.die.movType, 1);
    CHECK(si.cog.manip.leftDis == 250.0);
    CHECK(si.cog.manip.preload == 222400.0);
    CHECK_EQ(si.cog.manip.useSpring, 1);
    REQUIRE(si.cog.reheats.size() == 1);
    REQUIRE(si.cog.reheats[0].passes.size() == 1);
    const auto& p = si.cog.reheats[0].passes[0];
    CHECK(p.leftManDis == 250.0);                   // from the default manipulator (0x140791870)
    CHECK_EQ(p.dieMovDir, -1);
    REQUIRE(p.bites.size() == 1);
    CHECK_EQ(p.bites[0].nstep, 100);
    CHECK(p.bites[0].dsmax == 0.25);
    CHECK(p.ht.ihtcof == 4.0);

    CogModel en;
    cogModelDefaults(en, 2);
    CHECK(en.cog.billet.length == 40.0);
    CHECK(en.cog.manip.leftDis == 15.0);
    CHECK(en.cog.reheats[0].passes[0].leftManDis == 15.0);
    CHECK(en.cog.reheats[0].passes[0].bites[0].dsmax == 0.01);
}

int main()
{
    return testkit::run_all();
}
