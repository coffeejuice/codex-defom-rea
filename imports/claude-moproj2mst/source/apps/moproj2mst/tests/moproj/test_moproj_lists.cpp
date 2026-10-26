#include "../support/test_paths.hpp"
// Packet moproj-lists: the four item lists of the .moproj writer (0x1400a1110 Object_List,
// 0x1400a16b0 Material_List, 0x1400a12f0 Lubricant_List, 0x1400a14d0 Equipment_List, with
// 0x1400a0f60 / 0x1400a0210) against oracle B. Each case opens the project the GUI opened (an
// oracle-A corpus copy, or the case's _before copy), loads and saves its active operation as File >
// Save does before step 3 (MDT loader, packet S), rebuilds the lists in the DOM and serialises it
// with toString(1); each list element must equal, byte for byte, the one in the .moproj the GUI
// saved. A case whose load or save-current is not ported yet is skipped (its lists may depend on
// it), never compared.
// ctest-labels: m2m-moproj m2m-moproj-lists
#include <testkit/check.hpp>

#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>

#include "m2m/engine.hpp"
#include "m2m/model.hpp"
#include "m2m/moproj_write.hpp"
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

fs::path oracleB()
{
    return m2m_test::configuredPath("M2M_ORACLE_B");
}

// The text of <tag ...>...</tag> (or <tag .../>) at nesting depth 1, empty when absent
std::string listText(const std::string& s, const std::string& tag)
{
    const std::size_t a = s.find("\n <" + tag);
    if (a == std::string::npos) return {};
    const std::size_t lineEnd = s.find('\n', a + 1);
    if (s.compare(lineEnd - 2, 2, "/>") == 0) return s.substr(a + 1, lineEnd - a - 1);
    const std::string close = "\n </" + tag + ">";
    const std::size_t b = s.find(close, a);
    if (b == std::string::npos) return {};
    return s.substr(a + 1, b + close.size() - a - 1);
}

enum class R { Pass, Fail, Skip };

R runCase(const fs::path& in, const char* moproj, const char* caseB, int& lists)
{
    const fs::path ob = oracleB() / caseB / moproj;
    if (!fs::exists(in / moproj) || !fs::exists(ob)) {
        std::printf("SKIP %s: no input or oracle B\n", caseB);
        return R::Skip;
    }
    const qt::String file{(in / moproj).u16string()};
    const qt::String db = app::changeSuffix(file, qt::L(".DB"));
    Project p;
    ProjectScope scope(p);
    Ctx ctx;
    initProject(p, qt::absolutePath(db), qt::completeBaseName(db));
    makeRegistry(p);
    if (!openProject(p, file, ctx) || !p.openedOk) return R::Fail;
    activateSubProject(p, 0, ctx);
    engine::setEngineVersion(u"");
    engine::FileEffects open, save;
    try {
        Operation* op = engine::openActiveOperation(p, ctx, open);
        if (!op) return R::Fail;
        engine::saveCurrentOperation(p, *op, ctx, save);
        moproj::itemLists(*p.dom, p, {&open, &save}, false);
    } catch (const Error& e) {
        if (e.code != M2M_E_UNSUPPORTED) throw;
        std::printf("SKIP %s: %s\n", caseB, e.what());
        return R::Skip;
    }
    const std::string got = qt::String(xml::toString(*p.dom, 1)).utf8(), want = readAll(ob);
    bool ok = true;
    for (const char* tag : {"Object_List", "Material_List", "Lubricant_List", "Equipment_List"}) {
        const std::string g = listText(got, tag), w = listText(want, tag);
        if (w.empty()) {
            ok = false;
            std::printf("FAIL %s %s: not in the GUI's file\n", caseB, tag);
        } else if (g != w) {
            ok = false;
            std::size_t i = 0;
            while (i < g.size() && i < w.size() && g[i] == w[i]) ++i;
            std::printf("FAIL %s %s at byte %zu (%zu vs %zu bytes)\n", caseB, tag, i, g.size(), w.size());
        } else {
            ++lists;
        }
    }
    if (ok) std::printf("PASS %s: 4 lists\n", caseB);
    return ok ? R::Pass : R::Fail;
}

}  // namespace

TEST_CASE(moproj_lists_oracle_b)
{
    const char* corpus = std::getenv("M2M_CORPUS");
    if (!corpus) return;
    const fs::path c(corpus), o = oracleB();
    struct C { fs::path in; const char* moproj; const char* b; };
    const C cases[] = {
        {c / "11_THEXP", "THEXP.moproj", "R11"},
        {c / "11_THEXP", "THEXP.moproj", "R11b"},
        {c / "19_XY_H01", "XY_H01.moproj", "R19"},
        {c / "46_MO2_Boolean_Oprations", "MO2_Boolean_Oprations.moproj", "R46"},
        {c / "49_MO2_Boolean_Oprations", "MO2_Boolean_Oprations.moproj", "R49"},
        {c / "11_THEXP", "THEXP.moproj", "78_R11_THEXP"},
        {c / "19_XY_H01", "XY_H01.moproj", "79_R19_XY_H01"},
        {c / "46_MO2_Boolean_Oprations", "MO2_Boolean_Oprations.moproj", "80_R46_MO2_Boolean_Oprations"},
        {c / "49_MO2_Boolean_Oprations", "MO2_Boolean_Oprations.moproj", "81_R49_MO2_Boolean_Oprations"},
        {c / "52_BEND_2D", "BEND_2D.moproj", "82_R52_BEND_2D"},
        {c / "47_MO2_Extrusion_and_Boolean_Cycles", "MO2_Extrusion_and_Boolean_Cycles.moproj",
         "83_R47_MO2_Extrusion_and_Boolean_Cycles"},
        {c / "50_MO2_Extrusion_and_Boolean_Cycles", "MO2_Extrusion_and_Boolean_Cycles.moproj",
         "84_R50_MO2_Extrusion_and_Boolean_Cycles"},
        {o / "93_B_MB3ACT_before", "Gear_blank.moproj", "93_B_MB3ACT"},
        {o / "94_B_MB2ACT_before", "MO2_Operation_Cycles2.moproj", "94_B_MB2ACT"},
        {o / "95_B_FE3ACT_before", "Gear_blank.moproj", "95_B_FE3ACT"},
        {o / "96_B_FNOEQ_before", "Gear_blank.moproj", "96_B_FNOEQ"},
    };
    int pass = 0, skip = 0, lists = 0;
    for (const C& k : cases) {
        const R r = runCase(k.in, k.moproj, k.b, lists);
        CHECK(r != R::Fail);
        if (r == R::Pass) ++pass;
        if (r == R::Skip) ++skip;
    }
    std::printf("%d pass (%d lists), %d skipped\n", pass, lists, skip);
}

int main()
{
    return testkit::run_all();
}
