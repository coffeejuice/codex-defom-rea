#include "../support/test_paths.hpp"
// MDT L4 (moproj-ops): the SfOprMultiBlowProperties get slots (qt_metacall 0x14061ab80) against
// oracle B 93_B_MB3ACT and 94_B_MB2ACT: <case>_before is opened and its active MultiBlow operation
// loaded; for every MultiBlow-owned property the slot's value, as the writer turns it into text
// (QVariant::toString: QString::number(d, 'g', 15) for a double), must equal the <Value> the GUI
// saved in <case>/<name>.moproj for that operation (expected values: the saved file).
// The SfOprBaseProperties names (NumSteps .. TimePerStep, FrictionType, FrictionCoef,
// HeatTransCoef) are L3's base slots and are not checked here.
// ctest-labels: m2m-mdt-l4
#include <testkit/check.hpp>

#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <map>
#include <regex>
#include <set>
#include <string>

#include "m2m/data/multiblow.hpp"
#include "m2m/engine.hpp"
#include "m2m/model.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/props.hpp"
#include "m2m/qt4path.hpp"

namespace fs = std::filesystem;
using namespace m2m;

namespace {

fs::path oracleB()
{
    if (const char* e = std::getenv("M2M_ORACLE_B")) return fs::path(e);
    return m2m_test::configuredPath("M2M_ORACLE_B");
}

std::string readAll(const fs::path& f)
{
    std::ifstream in(f, std::ios::binary);
    return std::string(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
}

const std::set<std::u16string> kOwned = {
    u"Equipment", u"EquipName", u"HammerEnergy", u"HammerMass", u"HammerEfficiency", u"HammerCounterblow",
    u"ScrewEnergy", u"ScrewEfficiency", u"MomentOfInertia", u"LeadScrewPitch", u"MovDirX", u"MovDirY",
    u"MovDirZ", u"UseWorkpieceRotation", u"InitTemp", u"InitStrain", u"UseEfficiencyFunctionData",
    u"UseHeating", u"HeatingTemperature", u"RoomTemperature", u"TransferInTime", u"HeatingTime",
    u"TransferOutTime", u"TransferTimeStep", u"HeatingTimeStep", u"UseAdaptiveReheat", u"TriggerHeatTemp",
    u"TriggerHeatTempUpperLimit", u"HeatStopTemp", u"UseDwell", u"DefaultDwellTime", u"DwellTimeStep",
    u"DwellHeatCoeff", u"UseRelativeEnergy", u"NumHeats", u"NumBlows", u"Temperature", u"BlowEnergy",
    u"BlowEfficiency", u"BlowDwellTime", u"BlowReheat", u"BlowFlip", u"BlowRotAngle", u"BlowRotSelfCenter",
    u"BlowRotCenterX", u"BlowRotCenterY", u"BlowRotCenterZ", u"BlowRotAxisX", u"BlowRotAxisY", u"BlowRotAxisZ"};

// The values are read after the Save's "save current operation" (0x140052ec0), as the GUI writes
// the .moproj after it: with the MultiBlow operation active its Process editor writes the form
// back first (vt+0xd0, src/ops/multiblow_process.cpp), which turns the Dwell unit texts into
// numbers. No known differences.
const std::set<std::string> kKnown = {};

// QVariant::toString as the writer applies it (src/moproj/write_ops.cpp)
std::string text(const qt::Variant& v)
{
    switch (v.kind) {
    case qt::Variant::Double: return qt::number(v.d, 'g', 15).utf8();
    case qt::Variant::Int: return qt::number(v.i).utf8();
    case qt::Variant::Text: return v.s.utf8();
    case qt::Variant::Bool: return v.i ? "true" : "false";
    default: return {};
    }
}

// Saved <Name>/<Value> pairs of the first property tree that has a MultiBlow root
std::map<std::string, std::string> savedValues(const fs::path& moproj)
{
    const std::string s = readAll(moproj);
    std::map<std::string, std::string> out;
    const std::size_t at = s.find("<Name>NumHeats</Name>");
    if (at == std::string::npos) return out;
    const std::size_t from = s.rfind("<Properties", at);
    const std::size_t to = s.find("</Properties>", at);
    const std::string seg = s.substr(from, to - from);
    static const std::regex re("<Name>([^<]*)</Name>\\s*<TypeName>[^<]*</TypeName>\\s*<Value>([^<]*)</Value>");
    for (std::sregex_iterator it(seg.begin(), seg.end(), re), end; it != end; ++it) out[(*it)[1]] = (*it)[2];
    return out;
}

void walk(MultiBlowProps& pr, std::vector<PropNode>& nodes, std::vector<PropNode*>& ancestors,
          const std::map<std::string, std::string>& saved, int& checked, int& bad)
{
    for (PropNode& n : nodes) {
        if (n.typeName.u16() == u"group") {
            ancestors.push_back(&n);
            walk(pr, n.children, ancestors, saved, checked, bad);
            ancestors.pop_back();
            continue;
        }
        if (!kOwned.count(n.name.u16())) continue;
        std::string name = n.name.utf8();
        if (n.index >= 0) name += std::to_string(n.index + 1);
        qt::Variant v;
        CHECK(pr.getValue(n, ancestors, v));
        const auto it = saved.find(name);
        if (kKnown.count(name)) {
            std::printf("KNOWN %s: %s, GUI %s\n", name.c_str(), text(v).c_str(),
                        it == saved.end() ? "(not saved)" : it->second.c_str());
            continue;
        }
        if (it == saved.end()) {
            std::printf("NOT SAVED %s\n", name.c_str());
            ++bad;
            continue;
        }
        ++checked;
        if (text(v) != it->second) {
            std::printf("DIFF %s: %s, GUI %s\n", name.c_str(), text(v).c_str(), it->second.c_str());
            ++bad;
        }
    }
}

void runCase(const char* caseB, const char* moproj, int minChecked)
{
    const fs::path before = oracleB() / (std::string(caseB) + "_before");
    if (!fs::exists(before)) {
        std::printf("SKIP %s: no oracle B\n", caseB);
        return;
    }
    Project p;
    ProjectScope scope(p);
    Ctx ctx;
    const qt::String file{(before / moproj).u16string()};
    const qt::String db = app::changeSuffix(file, qt::L(".DB"));
    initProject(p, qt::absolutePath(db), qt::completeBaseName(db));
    makeRegistry(p);
    REQUIRE(openProject(p, file, ctx) && p.openedOk);
    activateSubProject(p, 0, ctx);
    engine::FileEffects fx;
    Operation* op = engine::openActiveOperation(p, ctx, fx);
    REQUIRE(op != nullptr);
    engine::setEngineVersion(u"");
    engine::FileEffects save;
    engine::saveCurrentOperation(p, *op, ctx, save);
    auto* pr = dynamic_cast<MultiBlowProps*>(op->props.get());
    REQUIRE(pr != nullptr);
    const auto saved = savedValues(oracleB() / caseB / moproj);
    REQUIRE(!saved.empty());
    int checked = 0, bad = 0;
    std::vector<PropNode*> ancestors;
    walk(*pr, pr->tree, ancestors, saved, checked, bad);
    std::printf("%s: %d checked, %d bad\n", caseB, checked, bad);
    CHECK(checked >= minChecked);
    CHECK_EQ(bad, 0);
}

}  // namespace

TEST_CASE(multiblow_get_93)
{
    runCase("93_B_MB3ACT", "Gear_blank.moproj", 40);
}

// 93 with Dwell "0.2 sec" / "2.0 N/sec/mm/C" before the open (oracle B probe): the GUI saved 0.2 / 2
TEST_CASE(multiblow_get_93d)
{
    runCase("93D_B_MB3ACT_DWELL", "Gear_blank.moproj", 40);
}

TEST_CASE(multiblow_get_94)
{
    runCase("94_B_MB2ACT", "MO2_Operation_Cycles2.moproj", 40);
}

// 0x140612e80 / 0x1406132b0: EquipmentSpec holds the hammer fields (MBD+9) or the screw fields
// (MBD+0xa), in the template order of 0x143d9f000..0x143d9f038; setEquipment 0x140612de0 sets
// +0xa = value, +9 = !+0xa and rebuilds (the disassembly's order, hand-read)
TEST_CASE(multiblow_equipment_spec)
{
    auto names = [](const PropNode& g) {
        std::string r;
        for (const PropNode& c : g.children) r += c.name.utf8() + ",";
        return r;
    };
    data::MultiBlowData d;                  // ctor: Equipment = 1 (screw), +9 = 0
    std::vector<PropNode> t = data::multiBlowPropTree(3, d);
    const PropNode* spec = nullptr;
    for (const PropNode& g : t[0].children)
        if (g.name.u16() == u"EquipmentSpec") spec = &g;
    REQUIRE(spec != nullptr);
    CHECK_EQ(names(*spec), std::string("ScrewEnergy,ScrewEfficiency,MomentOfInertia,LeadScrewPitch,"));
    qt::Variant off;
    off.kind = qt::Variant::Text;
    off.s = qt::L("false");
    data::mbSetEquipment(d, off);
    data::equipmentSpecRebuild(const_cast<PropNode&>(*spec), d);
    CHECK_EQ(names(*spec), std::string("HammerEnergy,HammerMass,HammerEfficiency,HammerCounterblow,"));
}

int main()
{
    return testkit::run_all();
}
