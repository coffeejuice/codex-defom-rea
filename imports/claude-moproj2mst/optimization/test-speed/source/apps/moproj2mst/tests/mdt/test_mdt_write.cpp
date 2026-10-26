#include "../support/test_paths.hpp"
// MDT: Cogging.MDT writer of File > Save (0x1407617e0) against the original's output.
//   - oracle A: every corpus MDT saved by the v11.0.2 GUI (same Save as its .moproj, header
//     "Service Pack 2") is reproduced byte for byte from the project it belongs to;
//   - oracle B (deform-rea-work/gui_mo/oracle_b/MDT2..4, data/): corpus 54 re-saved by the GUI,
//     unchanged and with one operation's MDT edited, shows the globals of the last read going into
//     every MDT and KeyDie names written from the ctor's table;
//   - the cases the port refuses, and the version text of line 3.
// ctest-labels: m2m-mdt
#include <process.h>

#include <testkit/check.hpp>

#include <cstdio>
#include <cstdlib>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <map>
#include <string>
#include <vector>

#include "m2m/convert.hpp"
#include "m2m/model.hpp"
#include "../../src/datafiles/readers.hpp"

namespace fs = std::filesystem;
using namespace m2m;

namespace {

using Clock = std::chrono::steady_clock;
bool timingEnabled()
{
    static const bool enabled = [] {
        const char* value = std::getenv("M2M_MDT_TIMING");
        return value && value[0] == '1' && value[1] == '\0';
    }();
    return enabled;
}

struct PhaseTimer {
    const char* name;
    Clock::time_point start;
    bool active;
    explicit PhaseTimer(const char* phase) : name(phase), active(timingEnabled())
    {
        if (active) start = Clock::now();
    }
    ~PhaseTimer()
    {
        if (!active) return;
        const auto ms = std::chrono::duration<double, std::milli>(Clock::now() - start).count();
        totals()[name] += ms;
        ++counts()[name];
    }
    static std::map<std::string, double>& totals() { static std::map<std::string, double> v; return v; }
    static std::map<std::string, unsigned>& counts() { static std::map<std::string, unsigned> v; return v; }
};

void printTimings()
{
    if (!timingEnabled()) return;
    for (const auto& [name, ms] : PhaseTimer::totals())
        std::fprintf(stderr, "MDT timing phase=%s count=%u total_ms=%.3f\n",
                     name.c_str(), PhaseTimer::counts()[name], ms);
}

std::string readAll(const fs::path& p)
{
    std::ifstream in(p, std::ios::binary);
    return std::string(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
}

void writeAll(const fs::path& p, const std::string& s)
{
    std::ofstream out(p, std::ios::binary | std::ios::trunc);
    out.write(s.data(), static_cast<std::streamsize>(s.size()));
}

fs::path dataDir() { return fs::path(__FILE__).parent_path() / "data"; }

fs::path moprojIn(const fs::path& dir)
{
    for (const auto& e : fs::directory_iterator(dir))
        if (e.path().extension() == ".moproj") return e.path();
    return {};
}

// first differing line (1-based) for the failure message, 0 when equal
int firstDiff(const std::string& a, const std::string& b)
{
    int line = 1;
    for (std::size_t i = 0; i < a.size() && i < b.size(); ++i) {
        if (a[i] != b[i]) return line;
        if (a[i] == '\n') ++line;
    }
    return a.size() == b.size() ? 0 : line;
}

// Converts with M2M_FLAG_WRITE_MDT into memory; the MST goes to a scratch path that is not written.
Result convertMdts(const fs::path& moproj, SideBytes& side, unsigned flags = M2M_FLAG_WRITE_MDT)
{
    PhaseTimer timer("convert");
    std::string mst;
    std::u16string outPath;
    const fs::path out = m2m_test::scratchRoot() / ("m2m_mdt_test_" + std::to_string(_getpid()) + ".MST");
    return convertToBytes(moproj.u16string(), out.u16string(), flags, 1252, mst, outPath, side);
}

std::string taskOf(const std::u16string& path)
{
    const fs::path p(path);
    return p.parent_path().filename().string();
}

fs::path scratchCopy(const fs::path& src, const char* name)
{
    const fs::path dst = m2m_test::scratchRoot() / (std::string(name) + "_" + std::to_string(_getpid()));
    std::error_code ec;
    fs::remove_all(dst, ec);
    fs::copy(src, dst, fs::copy_options::recursive);
    return dst;
}

}  // namespace

TEST_CASE(corpus_sp2_mdts_reproduced)
{
    const char* corpus = std::getenv("M2M_CORPUS");
    REQUIRE(corpus != nullptr);
    int files = 0, beta = 0;
    std::vector<std::string> refused;
    {
    PhaseTimer timer("corpus-scan-and-compare");
    for (const auto& c : fs::directory_iterator(corpus)) {
        if (!c.is_directory() || !fs::exists(c.path() / "Operations")) continue;
        bool has = false;
        for (const auto& t : fs::directory_iterator(c.path() / "Operations"))
            if (fs::exists(t.path() / "Cogging.MDT")) has = true;
        if (!has) continue;
        SideBytes side;
        Result r = convertMdts(moprojIn(c.path()), side);
        const std::string name = c.path().filename().string();
        if (r.code == M2M_E_UNSUPPORTED && r.message.find("active operation") != std::string::npos) {
            refused.push_back(name.substr(0, 2));   // spec section 6
            std::printf("  %s refused: %s\n", name.c_str(), r.message.c_str());
            continue;
        }
        CHECK_EQ(r.code, 0);
        if (r.code != 0) std::printf("  %s: %s\n", name.c_str(), r.message.c_str());
        for (const auto& [path, bytes] : side) {
            PhaseTimer compareTimer("mdt-read-and-byte-compare");
            // a file the GUI wrote from a model it never read (doubles, not widened floats):
            // compare with the GUI's re-save (oracle B) instead
            const fs::path resave = dataDir() / (name.substr(0, 2) + "." + taskOf(path) + ".resave.MDT");
            const std::string orig = readAll(fs::exists(resave) ? resave : fs::path(path));
            if (orig.find("(Beta ReleaseCandidate1)") != std::string::npos) {
                ++beta;                        // saved by an older build: see oracle B below
                continue;
            }
            ++files;
            const int d = firstDiff(bytes, orig);
            CHECK_EQ(d, 0);
            if (d) std::printf("  %s: first difference at line %d\n", fs::path(path).string().c_str(), d);
        }
    }
    std::string ref;
    for (const auto& n : refused) ref += n + " ";
    std::printf("  refused (active cogging operation): %s\n", ref.c_str());
    std::printf("  compared %d, beta %d\n", files, beta);
    // 27 corpus MDTs: 17 the v11.0.2 GUI saved after reading them, 39 and 53 against their oracle-B
    // re-saves, and 8 whose active diagram item is a cogging operation: File > Open loads it and
    // Save saves it first (geometry step 0x14075c260), compared with the corpus files or, for 42
    // and 54 whose GUI re-save differs from them, with the oracle-B re-saves
    CHECK_EQ(files, 27);
    CHECK_EQ(beta, 0);
    CHECK_EQ(ref, std::string(""));
    }
}

TEST_CASE(oracle_b_lab2_last_read_globals)
{
    const char* corpus = std::getenv("M2M_CORPUS");
    REQUIRE(corpus != nullptr);
    const fs::path src = fs::path(corpus) / "54_Cogging_Multiple_Ops_Lab2";
    struct Case {
        const char* name;
        const char* editTask;
    } cases[] = {{"MDT2_LAB2", nullptr}, {"MDT3_LAB2_E1", "Task00001"}, {"MDT4_LAB2_E3", "Task00003"}};
    for (const Case& k : cases) {
        PhaseTimer caseTimer("oracle-b-case");
        const fs::path dir = scratchCopy(src, "m2m_mdt_lab2");
        // Active 0 is a cogging operation, which the port refuses (spec section 6); the oracle-B
        // cases MDTA2..4 have Active 2 (the Forming operation) and give the same MDTs as MDT2..4.
        const fs::path mp = moprojIn(dir);
        std::string text = readAll(mp);
        const std::size_t at = text.find("Active=\"0\"");
        REQUIRE(at != std::string::npos);
        text[at + 8] = '2';
        writeAll(mp, text);
        if (k.editTask)
            writeAll(dir / "Operations" / k.editTask / "Cogging.MDT",
                     readAll(dataDir() / (std::string(k.name) + "." + k.editTask + ".in.MDT")));
        SideBytes side;
        Result r = convertMdts(moprojIn(dir), side);
        CHECK_EQ(r.code, 0);
        REQUIRE(side.size() == 2);
        CHECK_EQ(taskOf(side[0].first), std::string("Task00001"));   // OprManager row order
        CHECK_EQ(taskOf(side[1].first), std::string("Task00003"));
        for (const auto& [path, bytes] : side) {
            const std::string want = readAll(dataDir() / (std::string(k.name) + "." + taskOf(path) + ".out.MDT"));
            REQUIRE(!want.empty());
            const int d = firstDiff(bytes, want);
            CHECK_EQ(d, 0);
            if (d) std::printf("  %s %s: first difference at line %d\n", k.name, taskOf(path).c_str(), d);
        }
        std::error_code ec;
        fs::remove_all(dir, ec);
    }
}

TEST_CASE(no_flag_no_side_files)
{
    const char* corpus = std::getenv("M2M_CORPUS");
    REQUIRE(corpus != nullptr);
    SideBytes side;
    Result r = convertMdts(moprojIn(fs::path(corpus) / "01_COG_P1_B1"), side, 0);
    CHECK_EQ(r.code, 0);
    CHECK(side.empty());
}

TEST_CASE(two_reheats_refused)
{
    data::CogModel m;
    datafiles::cogModelDefaults(m, 1);
    Project::CogGlobals g;
    m.cog.nReheats = 2;
    m.cog.reheats.push_back(m.cog.reheats[0]);
    int code = 0;
    try {
        datafiles::writeMdtBytes(m, g, "DEFORM-2D/3D 11.0");
    } catch (const Error& e) {
        code = e.code;
    }
    CHECK_EQ(code, M2M_E_UNSUPPORTED);
    m.cog.nReheats = 1;
    m.cog.reheats.resize(1);
    std::string one = datafiles::writeMdtBytes(m, g, "DEFORM-2D/3D 11.0");
    CHECK(one.find("NoReheats      1\r\n*\r\n* Reheat 1\r\n") != std::string::npos);
}

TEST_CASE(defaults_model_layout)
{
    // A model straight from the ctor (no MDT read), SI: the values the writer prints.
    data::CogModel m;
    datafiles::cogModelDefaults(m, 1);
    Project::CogGlobals g;
    datafiles::cogModelDefaults(m, 1, g);
    const std::string b = datafiles::writeMdtBytes(m, g, "DEFORM-2D/3D 11.0 (Service Pack 2)  ");
    const std::string top = "* " + std::string(78, '*') + "\r\n*\r\n";
    CHECK_EQ(b.substr(0, top.size()), top);
    CHECK(b.find("* DEFORM-2D/3D 11.0 (Service Pack 2)  \r\n") != std::string::npos);
    CHECK(b.find("CVGERR1    5.0000000E-003\r\n") != std::string::npos);     // ctor 0x14076b3d0
    CHECK(b.find("POSERR     1.0000000E-003\r\n") != std::string::npos);     // .data
    CHECK(b.find("CTLPCT     5.0000000E+001\r\n") != std::string::npos);
    CHECK(b.find("KeyDie1   DIE1.KEY\r\n") != std::string::npos);
    CHECK(b.find("KeyDie3") == std::string::npos);
    CHECK(b.find("ENVTMP \t0\t0\r\n") != std::string::npos);
    CHECK_EQ(g.noDies, 2);
    m.cog.die.four = 1;
    const std::string b4 = datafiles::writeMdtBytes(m, g, "x");
    CHECK(b4.find("NoDies         4\r\nKeyDie1   DIE1.KEY\r\nKeyDie2   DIE2.KEY\r\nKeyDie3   DIE3.KEY\r\nKeyDie4   DIE4.KEY\r\n") !=
          std::string::npos);
    CHECK_EQ(g.noDies, 4);
}

TEST_CASE(header_text_from_pack_dat)
{
    CHECK_EQ(datafiles::mdtHeaderText({}), std::string("DEFORM-2D/3D 11.0 (Service Pack 2)  "));
    const fs::path d = m2m_test::scratchRoot() / ("m2m_mdt_pack_" + std::to_string(_getpid()));
    std::error_code ec;
    fs::remove_all(d, ec);
    CHECK_EQ(datafiles::mdtHeaderText(d.u16string()), std::string("DEFORM-2D/3D 11.0"));   // no pack.dat
    fs::create_directories(d / "PACK");
    writeAll(d / "PACK" / "pack.dat", "NAME X\r\nVERSION Beta ReleaseCandidate1\r\n");
    CHECK_EQ(datafiles::mdtHeaderText(d.u16string()), std::string("DEFORM-2D/3D 11.0 (Beta ReleaseCandidate1)  "));
    writeAll(d / "PACK" / "pack.dat", "VERSION   3\n");
    CHECK_EQ(datafiles::mdtHeaderText(d.u16string()), std::string("DEFORM-2D/3D 11.0 (Service Pack 3)  "));
    fs::remove_all(d, ec);
    // Optional imported installation fixture; never probe the installed application.
    if (const char* fixture = std::getenv("M2M_INSTALL_FIXTURE")) {
        const fs::path inst = fixture;
        REQUIRE(fs::exists(inst / "PACK" / "pack.dat"));
        CHECK_EQ(datafiles::mdtHeaderText(inst.u16string()), std::string("DEFORM-2D/3D 11.0 (Service Pack 2)  "));
    } else {
        std::puts("SKIP optional imported PACK/pack.dat header check: M2M_INSTALL_FIXTURE unset");
    }
}

int main()
{
    const int result = testkit::run_all();
    printTimings();
    return result;
}
