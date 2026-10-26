#include "../support/test_paths.hpp"
// MDT L2S: save current operation 0x140052ec0 for the L2 classes against oracle B. The inputs of
// 86_B_PRE3F and 87_B_HTFF are rebuilt as oracle_b/_tools/mk2.py "first" made them (corpus 57 copy,
// first operation turned into SfOprPre3D "Pre" / SfOprHTFurnace "HT Furnace", its properties
// emptied); their active item is that first operation. Every KEY the port's save records must equal
// the file the GUI wrote in Save step 2 (mtime in the second before the saved .moproj), and the
// set of files must be the same (as tests/s/test_save_current.cpp).
// ctest-labels: m2m-mdt m2m-mdt-l2
#include <process.h>

#include <testkit/check.hpp>

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <set>
#include <string>

#include "m2m/engine.hpp"
#include "m2m/model.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/qt4path.hpp"

namespace fs = std::filesystem;
using namespace m2m;

namespace {

std::string readAll(const fs::path& f)
{
    std::ifstream in(f, std::ios::binary);
    return std::string(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
}

std::string lower(std::string s)
{
    for (char& c : s)
        if (c >= 'A' && c <= 'Z') c = static_cast<char>(c + 32);
    return s;
}

fs::path oracleB()
{
    return m2m_test::configuredPath("M2M_ORACLE_B");
}

void replaceFirst(std::string& s, std::size_t from, std::size_t to, const std::string& open, const std::string& close,
                  const std::string& value)
{
    const std::size_t a = s.find(open, from);
    if (a == std::string::npos || a >= to) return;
    const std::size_t b = s.find(close, a);
    s.replace(a + open.size(), b - a - open.size(), value);
}

// mk2.py first(d, typ, name, {})
bool makeInput(const fs::path& src, const fs::path& d, const char* typ, const char* name)
{
    std::error_code ec;
    fs::remove_all(d, ec);
    fs::create_directories(d);
    fs::copy(src, d, fs::copy_options::recursive, ec);
    if (ec) return false;
    fs::path mp;
    for (const auto& e : fs::directory_iterator(d)) {
        const std::string ext = lower(e.path().extension().string());
        if (ext == ".mst") fs::rename(e.path(), fs::path(e.path().string() + ".oracleA"));
        if (ext == ".png") fs::remove(e.path());
        if (ext == ".moproj") mp = e.path();
    }
    std::string s = readAll(mp);
    std::string t;
    for (char c : s)
        if (c != '\r') t += c;                      // text-mode read
    s = t;
    std::size_t i = s.find("  <Operation>\n");
    std::size_t j = s.find("  </Operation>\n", i);
    std::string blk = s.substr(i, j - i);
    const std::size_t ta = blk.find("<Type>") + 6;
    const std::string oldType = blk.substr(ta, blk.find("</Type>", ta) - ta);
    replaceFirst(blk, 0, blk.size(), "<Name>", "</Name>", name);
    const std::string ot = "<Type>" + oldType + "</Type>";
    for (std::size_t k = blk.find(ot); k != std::string::npos; k = blk.find(ot, k + 1))
        blk.replace(k, ot.size(), std::string("<Type>") + typ + "</Type>");
    const std::size_t pa = blk.find("   <Properties Number=");
    if (pa != std::string::npos) {
        const std::string pend = "\n   </Properties>\n";
        const std::size_t pb = blk.find(pend, pa);
        if (pb != std::string::npos) blk.replace(pa, pb + pend.size() - pa, "   <Properties Number=\"0\"/>\n");
    }
    s = s.substr(0, i) + blk + s.substr(j);
    const std::size_t k = s.find("<DiagramItem Expanded=\"");
    const std::size_t k2 = s.find("</DiagramItem>", k);
    std::string it = s.substr(k, k2 - k);
    if (it.find("Operation_Id=\"1\"") == std::string::npos) return false;
    replaceFirst(it, 0, it.size(), "<ClassName>", "</ClassName>", typ);
    replaceFirst(it, 0, it.size(), "<ItemName>", "</ItemName>", name);
    s = s.substr(0, k) + it + s.substr(k2);
    std::ofstream(mp, std::ios::binary) << s;
    return true;
}

std::set<std::string> guiStep2(const fs::path& dir)
{
    fs::path moproj;
    for (const auto& e : fs::directory_iterator(dir))
        if (e.path().extension() == ".moproj") moproj = e.path();
    const auto t0 = fs::last_write_time(moproj);
    std::set<std::string> out;
    for (const auto& e : fs::recursive_directory_iterator(dir)) {
        if (!e.is_regular_file()) continue;
        const auto t = e.last_write_time();
        if (t < t0 && t0 - t < std::chrono::seconds(1)) out.insert(lower(fs::relative(e.path(), dir).generic_string()));
    }
    return out;
}

bool runCase(const char* caseB, const char* typ, const char* name)
{
    const char* corpus = std::getenv("M2M_CORPUS");
    const fs::path ob = oracleB() / caseB;
    if (!corpus || !fs::exists(ob)) {
        std::printf("SKIP %s: no corpus or oracle B\n", caseB);
        return true;
    }
    const fs::path dirA = m2m_test::scratchRoot() / ("m2m_l2s_" + std::to_string(_getpid())) / caseB;
    if (!makeInput(fs::path(corpus) / "57_Gear_blank", dirA, typ, name)) return false;
    const qt::String file{(dirA / "Gear_blank.moproj").u16string()};
    const qt::String db = app::changeSuffix(file, qt::L(".DB"));
    Project p;
    ProjectScope scope(p);
    Ctx ctx;
    initProject(p, qt::absolutePath(db), qt::completeBaseName(db));
    makeRegistry(p);
    if (!openProject(p, file, ctx) || !p.openedOk) return false;
    activateSubProject(p, 0, ctx);
    engine::setEngineVersion(u"");
    engine::FileEffects open, save;
    Operation* op = engine::openActiveOperation(p, ctx, open);
    if (!op || op->classKey.utf8() != typ) return false;
    engine::saveCurrentOperation(p, *op, ctx, save);
    const std::set<std::string> want = guiStep2(ob);
    std::set<std::string> got;
    bool ok = true;
    for (const auto& w : save.writes) {
        const std::string rel = lower(m2m_test::effectRelative(fs::path(w.path.u16()), dirA).generic_string());
        got.insert(rel);
        const fs::path exp = ob / rel;
        const std::string e = fs::exists(exp) ? readAll(exp) : std::string("<missing>");
        if (e != w.bytes) {
            ok = false;
            if (const char* dump = m2m_test::scratchSetting("M2M_S_DUMP")) {
                const fs::path dd = fs::path(dump) / caseB / rel;
                fs::create_directories(m2m_test::scratchOutput(dd).parent_path());
                std::ofstream(m2m_test::scratchOutput(dd), std::ios::binary) << w.bytes;
            }
            std::printf("DIFF %s %s (%zu vs %zu bytes)\n", caseB, rel.c_str(), w.bytes.size(), e.size());
        }
    }
    for (const auto& r : want)
        if (!got.count(r)) {
            ok = false;
            std::printf("MISSING %s %s (the GUI wrote it)\n", caseB, r.c_str());
        }
    for (const auto& r : got)
        if (!want.count(r)) {
            ok = false;
            std::printf("EXTRA %s %s (the GUI did not write it in step 2)\n", caseB, r.c_str());
        }
    std::printf("%s %s: %zu files\n", ok ? "PASS" : "FAIL", caseB, got.size());
    return ok;
}

}  // namespace

TEST_CASE(save_l2_oracle_b)
{
    CHECK(runCase("86_B_PRE3F", "SfOprPre3D", "Pre"));
    CHECK(runCase("87_B_HTFF", "SfOprHTFurnace", "HT Furnace"));
}

int main()
{
    return testkit::run_all();
}
