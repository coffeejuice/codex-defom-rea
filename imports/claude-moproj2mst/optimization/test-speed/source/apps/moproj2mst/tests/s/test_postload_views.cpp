#include "../support/test_paths.hpp"
// Packet S: the views step after the load (0x1400547b0 -> OB vt+0x230, equipment sync) against
// oracle B 93_B_MB3ACT and 94_B_MB2ACT: fresh GUI saves with a MultiBlow operation active. The
// pre-open copies (<case>_before) are the input; the saved folders give the expected result:
//   93: Equipment00002.KEY and Equipment00003.KEY deleted, Equipment_List ids {1, 4, 5, 6}, the
//       active operation's Equipment Id 1, slots 1 and 3 EquipmentID -1, slot 2 keeps 1;
//   94: Equipment00001.KEY and Equipment00003.KEY deleted, ids {2, 4, 5, 6}, Equipment Id 2,
//       slots 1 and 3 -1, slot 2 keeps 2;
//   96 (corpus 57 with the active Forming's Equipment Id set to -1): Forming vt+0xc0 0x140c06130
//       creates "Equipment (7)" = Equipments/Equipment00007.KEY (bytes = the saved file), ids
//       {1..7}, Equipment Id 7, slots 2, 7, 3; nothing deleted.
// (hand-read from the saved .moproj files and the folder listings). Skipped while the MultiBlow
// loader slots (L4) are not merged.
// ctest-labels: m2m-s
#include <testkit/check.hpp>

#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "m2m/engine.hpp"
#include "m2m/model.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/qt4path.hpp"

namespace fs = std::filesystem;
using namespace m2m;

namespace {

fs::path oracleB()
{
    return m2m_test::configuredPath("M2M_ORACLE_B");
}

std::string lower(std::string s)
{
    for (char& c : s)
        if (c >= 'A' && c <= 'Z') c = static_cast<char>(c + 32);
    return s;
}

struct Expect {
    const char* dir;
    const char* moproj;
    std::set<std::string> deleted;
    std::set<int> ids;
    int opEquipment;
    std::vector<int> slotEquipment;
    const char* savedDir = nullptr;            // saved folder whose files the written ones must equal
    std::vector<std::string> written;          // relative paths (lower case) that must be written
    const char* needs = nullptr;               // set: skipped until that packet lands
};

std::string readAll(const fs::path& f)
{
    std::ifstream in(f, std::ios::binary);
    return std::string(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
}

bool runCase(const Expect& x)
{
    if (x.needs) {                              // the class's vt+0xa8 / vt+0x228 are another packet's
        std::printf("SKIP %s: waits for %s\n", x.dir, x.needs);
        return true;
    }
    const fs::path dir = oracleB() / x.dir;
    if (!fs::exists(dir)) {
        std::printf("SKIP %s: no oracle B\n", x.dir);
        return true;
    }
    const qt::String file{(dir / x.moproj).u16string()};
    const qt::String db = app::changeSuffix(file, qt::L(".DB"));
    Project p;
    ProjectScope scope(p);
    Ctx ctx;
    initProject(p, qt::absolutePath(db), qt::completeBaseName(db));
    makeRegistry(p);
    if (!openProject(p, file, ctx) || !p.openedOk) return false;
    activateSubProject(p, 0, ctx);
    engine::setEngineVersion(u"");
    engine::FileEffects fx;
    Operation* op = nullptr;
    try {
        op = engine::openActiveOperation(p, ctx, fx);   // SfOprBase vt+0x58 runs postLoadViews
        if (!op) return false;
    } catch (const Error& e) {
        if (e.code != M2M_E_UNSUPPORTED) throw;
        std::printf("SKIP %s: %s\n", x.dir, e.what());
        return true;
    }
    bool ok = true;
    std::set<std::string> del;
    for (const String& d : fx.deletes) del.insert(lower(m2m_test::effectRelative(fs::path(d.u16()), dir).generic_string()));
    if (del != x.deleted) {
        ok = false;
        std::printf("FAIL %s: deleted files differ\n", x.dir);
        for (const auto& d : del) std::printf("  port deletes %s\n", d.c_str());
    }
    std::set<int> ids;
    for (const auto& r : p.equipment.rows)
        if (r) ids.insert(r->id);
    if (ids != x.ids) {
        ok = false;
        std::printf("FAIL %s: equipment ids differ\n", x.dir);
    }
    if (op->equipmentId != x.opEquipment) {
        ok = false;
        std::printf("FAIL %s: operation Equipment Id %d, want %d\n", x.dir, op->equipmentId, x.opEquipment);
    }
    for (std::size_t s = 0; s < x.slotEquipment.size(); ++s) {
        ObjEntry* e = objEntry(*op, static_cast<int>(s));
        const int got = e ? e->equipmentId : 0;
        if (got != x.slotEquipment[s]) {
            ok = false;
            std::printf("FAIL %s: slot %zu EquipmentID %d, want %d\n", x.dir, s + 1, got, x.slotEquipment[s]);
        }
    }
    std::map<std::string, std::string> last;    // final bytes per written path
    for (const auto& w : fx.writes) last[lower(m2m_test::effectRelative(fs::path(w.path.u16()), dir).generic_string())] = w.bytes;
    for (const std::string& r : x.written) {
        const auto it = last.find(r);
        const std::string want = readAll(oracleB() / x.savedDir / r);
        if (it == last.end() || it->second != want) {
            ok = false;
            std::printf("FAIL %s: %s %s\n", x.dir, r.c_str(), it == last.end() ? "not written" : "differs");
        }
    }
    if (ok) std::printf("PASS %s\n", x.dir);
    return ok;
}

}  // namespace

TEST_CASE(postload_views_equipment_oracle_b)
{
    const Expect cases[] = {
        {"93_B_MB3ACT_before", "Gear_blank.moproj",
         {"equipments/equipment00002.key", "equipments/equipment00003.key"}, {1, 4, 5, 6}, 1, {-1, 1, -1}, nullptr, {}},
        {"94_B_MB2ACT_before", "MO2_Operation_Cycles2.moproj",
         {"equipments/equipment00001.key", "equipments/equipment00003.key"}, {2, 4, 5, 6}, 2, {-1, 2, -1}, nullptr, {}},
        {"96_B_FNOEQ_before", "Gear_blank.moproj", {}, {1, 2, 3, 4, 5, 6, 7}, 7, {2, 7, 3}, "96_B_FNOEQ",
         {"equipments/equipment00007.key"}},
    };
    for (const Expect& x : cases) CHECK(runCase(x));
}

int main()
{
    return testkit::run_all();
}
