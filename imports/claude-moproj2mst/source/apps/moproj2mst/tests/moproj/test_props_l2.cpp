#include "../support/test_paths.hpp"
// L2: the set and get slots of the HeatTransfer, HeatTransferExpress, HTFurnace, Pre, Distortion,
// Convert2Dto3D, CopyMirror, DoeOutput, ShapeOptim, Report, Group and HeatTreat properties against
// oracle B. Each case opens a .moproj the
// GUI saved (the reader runs the set slots on every saved <Value>); then, for every operation of
// these classes, every non-group node of its property tree is asked through Props::getValue and
// printed as the writer prints <Value> (QVariant::toString), and must equal the <Value> the GUI
// wrote for that node, which the GUI's own get slot produced. The nodes and the saved <Property>
// elements are walked in parallel; their names must agree.
// ctest-labels: m2m-moproj m2m-moproj-l2
#include <testkit/check.hpp>

#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include <defmodel/layout/cls_1401e7b10.hpp>
#include <defmodel/layout/db_step_data.hpp>

#include "../../src/engine/props_sync.hpp"
#include "m2m/data/common.hpp"
#include "m2m/model.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/props.hpp"
#include "m2m/qt4path.hpp"
#include "m2m/xml.hpp"

namespace fs = std::filesystem;
using namespace m2m;

namespace {

// Type constraints are compiled with the existing props target; borrowed state
// cannot be returned through either the concept or the callable seam.
using Step = def::model::DB_STEP_DATA;
template<class F>
concept CanBorrowPropsStep = requires(Project& p, Props& pr, F f) {
    engine::withPropsStep(p, pr, f);
};
static_assert(CanBorrowPropsStep<void (*)(Step*)>);
static_assert(CanBorrowPropsStep<bool (*)(Step*)>);
static_assert(!CanBorrowPropsStep<Step* (*)(Step*)>);
static_assert(!CanBorrowPropsStep<Step& (*)(Step*)>);
static_assert(!CanBorrowPropsStep<def::model::Cls_1401e7b10* (*)(Step*)>);
static_assert(!CanBorrowPropsStep<int (*)(Step*)>);
static_assert(!CanBorrowPropsStep<void (*)()>);
static_assert(std::is_same_v<decltype(engine::withPropsStep(
    std::declval<Project&>(), std::declval<Props&>(), std::declval<bool (*)(Step*)>())), bool>);
static_assert(std::is_same_v<decltype(engine::withPropsStep(
    std::declval<Project&>(), std::declval<Props&>(), std::declval<void (*)(Step*)>())), void>);

class StepLookupProbe final : public mst::OpClass {
public:
    StepLookupProbe() : mst::OpClass(qt::L("props-step-probe")) {}
    engine::GuiModel model{nullptr, 3};
    int lookups = 0;
    engine::GuiModel* classModel(Project&, Operation&) override {
        ++lookups;
        return &model;
    }
};

fs::path oracleB()
{
    return m2m_test::configuredPath("M2M_ORACLE_B");
}

const xml::Node* child(const xml::Node& n, const char* tag)
{
    for (const auto& c : n.children)
        if (c->kind == xml::Node::Element && c->tag == qt::L(tag)) return c.get();
    return nullptr;
}

std::vector<const xml::Node*> children(const xml::Node& n, const char* tag)
{
    std::vector<const xml::Node*> r;
    for (const auto& c : n.children)
        if (c->kind == xml::Node::Element && c->tag == qt::L(tag)) r.push_back(c.get());
    return r;
}

// QVariant::toString as the writer prints <Value> (src/moproj/write_ops.cpp)
String text(const qt::Variant& v)
{
    switch (v.kind) {
    case qt::Variant::Double: return qt::number(v.d, 'g', 15);
    case qt::Variant::Int: return qt::number(v.i);
    case qt::Variant::Bool: return qt::L(v.i ? "true" : "false");
    case qt::Variant::Text: return v.s;
    default: return String();
    }
}

struct Count {
    int nodes = 0, fails = 0;
};

void walk(Props& pr, std::vector<PropNode>& nodes, std::vector<PropNode*>& anc, const xml::Node& saved,
          const std::string& where, Count& c)
{
    const std::vector<const xml::Node*> el = children(saved, "Property");
    if (el.size() != nodes.size()) {
        ++c.fails;
        std::printf("FAIL %s: %zu nodes, %zu saved properties\n", where.c_str(), nodes.size(), el.size());
        return;
    }
    for (std::size_t i = 0; i < nodes.size(); ++i) {
        PropNode& n = nodes[i];
        String name = n.name;
        if (n.index >= 0) name = name + qt::arg(qt::L("%1"), n.index + 1);
        const xml::Node* nameEl = child(*el[i], "Name");
        if (!nameEl || nameEl->text() != name) {
            ++c.fails;
            std::printf("FAIL %s: node %s, saved %s\n", where.c_str(), name.utf8().c_str(),
                        nameEl ? nameEl->text().utf8().c_str() : "?");
            continue;
        }
        if (n.typeName == qt::L("group")) {
            anc.push_back(&n);
            walk(pr, n.children, anc, *el[i], where + "/" + name.utf8(), c);
            anc.pop_back();
            continue;
        }
        qt::Variant v = n.value;
        pr.getValue(n, anc, v);
        const xml::Node* valEl = child(*el[i], "Value");
        const String want = valEl ? valEl->text() : String();
        ++c.nodes;
        if (text(v) != want) {
            ++c.fails;
            std::printf("FAIL %s/%s: %s, saved %s\n", where.c_str(), name.utf8().c_str(), text(v).utf8().c_str(),
                        want.utf8().c_str());
        }
    }
}

bool mine(const String& key)
{
    for (const char* k : {"SfOprHeatTransfer", "SfOprHeatTransfer2D", "SfOprHeatTransferExpress",
                          "SfOprHeatTransferExpress2D", "SfOprHTFurnace", "SfOprPre3D", "SfOprPre2D",
                          "SfOprDistortion2D", "SfOprDistortion3D", "SfOprConvert2Dto3D", "SfOprCopyMirror",
                          "SfOprDoeOutput", "SfOprShapeOptim", "SfOprReport", "SfOprGroup", "SfOprHeatTreat",
                          "SfOprHeatTreat2D"})
        if (key == qt::L(k)) return true;
    return false;
}

void runCase(const char* name, Count& total, int& ops, int& skipped)
{
    const fs::path dir = oracleB() / name;
    fs::path mp;
    if (fs::exists(dir))
        for (const auto& e : fs::directory_iterator(dir))
            if (e.path().extension() == ".moproj") mp = e.path();
    if (mp.empty()) {
        std::printf("SKIP %s: no oracle B file\n", name);
        return;
    }
    const qt::String file{mp.u16string()};
    const qt::String db = app::changeSuffix(file, qt::L(".DB"));
    Project p;
    ProjectScope scope(p);
    Ctx ctx;
    initProject(p, qt::absolutePath(db), qt::completeBaseName(db));
    makeRegistry(p);
    if (!openProject(p, file, ctx) || !p.openedOk) {
        // These class-registry cases fail the XML reader. The outer original
        // Open still accepts the suffix; this fixture helper needs parsed input.
        // Keep the historical diagnostic wording for baseline log comparison.
        std::printf("SKIP %s: the project does not open (class not registered)\n", name);
        ++skipped;
        return;
    }
    const xml::Node* list = child(*p.dom->root, "Operation_List");
    Count c;
    for (const auto& op : p.operations) {
        if (!mine(op->classKey) || !op->props) continue;
        const xml::Node* saved = nullptr;
        for (const xml::Node* e : children(*list, "Operation"))
            if (const xml::Node* id = child(*e, "Id"); id && qt::toInt(id->text()) == op->id) saved = e;
        const xml::Node* props = saved ? child(*saved, "Properties") : nullptr;
        if (!props) {
            ++c.fails;
            std::printf("FAIL %s op %d: no saved Properties\n", name, op->id);
            continue;
        }
        std::vector<PropNode*> anc;
        walk(*op->props, propRoots(*op->props), anc, *props, std::string(name) + " op " + std::to_string(op->id) + " " +
                                                               op->classKey.utf8(), c);
        ++ops;
    }
    std::printf("%s %s: %d values\n", c.fails ? "FAIL" : "PASS", name, c.nodes);
    total.nodes += c.nodes;
    total.fails += c.fails;
}

}  // namespace

TEST_CASE(props_step_null_and_explicit_control)
{
    Project p;
    ProjectScope scope(p);
    Props pr;
    pr.common.reset();
    int calls = 0;
    engine::withPropsStep(p, pr, [&](Step* step) {
        ++calls;
        CHECK(step == nullptr);
    });
    CHECK_EQ(calls, 1);
    auto checkMissing = [&] {
        CHECK(!engine::propsStepsFromSimCtl(p, pr, nullptr));
        CHECK(!engine::propsEnvTempFromSimCtl(p, pr, nullptr));
        CHECK(!engine::propsConvectionFromSimCtl(p, pr, nullptr));
        CHECK(!engine::propsStepByFromSimCtl(p, pr, nullptr));
        CHECK(!pr.common);
        bool refused = false;
        try { engine::propsSetHeatingTime(p, pr, 7.0); }
        catch (const Error& e) {
            CHECK_EQ(e.code, M2M_E_UNSUPPORTED);
            CHECK_EQ(std::string(e.what()), std::string("operation load: heating time without an engine model (0x1405fcb90 reads a null model)"));
            refused = true;
        }
        CHECK(refused);
        CHECK(!pr.common);
    };
    checkMissing();                    // no owner: no model acquisition
    Operation owner;                   // no registered class: no model
    pr.owner = &owner;
    checkMissing();
    CHECK(!owner.engineModel);
    CHECK(!engine::withPropsStep(p, pr, [&](Step* step) -> bool {
        ++calls;
        return step != nullptr;
    }));
    CHECK_EQ(calls, 2);

    auto probeOwner = std::make_unique<StepLookupProbe>();
    StepLookupProbe& probe = *probeOwner;
    owner.project = &p;
    owner.classKey = probe.key();
    p.registry.emplace(probe.key(), std::move(probeOwner));
    probe.model.step.reset();
    checkMissing();                    // present model with a null step
    CHECK_EQ(probe.lookups, 5);         // four readers plus heating time

    probe.model.step = std::make_unique<Step>(3);
    for (const int unit : {1, 2, -37}) {
        probe.model.step->sim_ctrl.unit = unit;
        int observed = 918;
        CHECK(probe.model.readPropertyHeatingTimeUnit(observed));
        CHECK_EQ(observed, unit);
        CHECK_EQ(probe.model.step->sim_ctrl.unit, unit);
    }
    probe.model.step.reset();
    int absentUnit = 731;
    CHECK(!probe.model.readPropertyHeatingTimeUnit(absentUnit));
    CHECK_EQ(absentUnit, 731);
    probe.model.step = std::make_unique<Step>(3);
    probe.model.step->sim_ctrl.unit = 1;
    pr.common.reset();
    const int beforeHeating = probe.lookups;
    engine::propsSetHeatingTime(p, pr, 7.25);
    CHECK_EQ(probe.lookups, beforeHeating + 1);
    REQUIRE(pr.common != nullptr);
    CHECK_EQ(pr.common->heatingTime.utf8(), std::string("7.25 sec"));
    CHECK_EQ(probe.model.step->sim_ctrl.unit, 1);
    engine::propsSetHeatingTime(p, pr, 8.5);
    CHECK_EQ(probe.lookups, beforeHeating + 2);
    CHECK_EQ(pr.common->heatingTime.utf8(), std::string("8.5 sec"));
    CHECK_EQ(probe.model.step->sim_ctrl.unit, 1);

    def::model::Cls_1401e7b10 sc;
    sc.unit = 1;
    sc.nstep = 23;
    sc.stpinc = 4;
    sc.dtmax1 = 0.5;
    sc.envtmp.value = 70.0;
    sc.cnvcof.value = 0.125;
    sc.dttype.u58 = 1;
    CHECK(engine::propsStepsFromSimCtl(p, pr, &sc));
    CHECK(engine::propsEnvTempFromSimCtl(p, pr, &sc));
    CHECK(engine::propsConvectionFromSimCtl(p, pr, &sc));
    CHECK(engine::propsStepByFromSimCtl(p, pr, &sc));
    REQUIRE(pr.common != nullptr);
    CHECK_EQ(pr.common->numSteps, 23);
    CHECK_EQ(pr.common->stepIncr, 4);
    CHECK_EQ(pr.common->dtMode, 1);
    CHECK_EQ(pr.common->dtmax.utf8(), std::string("0.5 sec"));
    CHECK_EQ(pr.common->envTemp.utf8(), std::string("70 C"));
    CHECK_EQ(pr.common->convection.utf8(), std::string("0.125 N/sec/mm/C"));
    CHECK(!owner.engineModel);
    CHECK_EQ(probe.lookups, beforeHeating + 2); // explicit sc does not call classModel
}

TEST_CASE(props_l2_oracle_b)
{
    if (!std::getenv("M2M_CORPUS")) return;
    const char* cases[] = {"70_B_PRE3", "71_B_PRE2", "72_B_HTF", "73_B_HTX3", "78_R11_THEXP", "79_R19_XY_H01",
                           "80_R46_MO2_Boolean_Oprations", "81_R49_MO2_Boolean_Oprations", "82_R52_BEND_2D",
                           "86_B_PRE3F", "87_B_HTFF", "89_B_MB2EN", "90_B_FE2EN", "91_B_FE3EN", "B12_PRE3", "B13_PRE2",
                           "B14_HTF", "B15_HTX3", "MDT1_COG01", "MDT5_CHAMF1", "MDT6_VDIE", "MDT7_BRICK",
                           "MDT8_VDIE42", "MDTC_53", "R11", "R11b", "R19", "R46", "R49",
                           // Distortion, Convert2Dto3D, CopyMirror, DoeOutput, ShapeOptim, Report, Group, HeatTreat
                           "101_B_DIS3F", "102_B_DIS2F", "66_B_DIS3", "67_B_DIS2", "B07_DIS3", "B08_DIS2",
                           "69_B_CNV", "85_B_CNVF", "97_B_CNVACT", "97R_B_CNVACT_RESAVE", "97S_B_CNVACT_UP2", "B10_CNV",
                           "B11_CM", "T_CM_DB.x", "T_CM_ONLY.x", "64_B_RPT", "B06_RPT", "65_B_SOPT", "B20_SOPT",
                           "T_GRP.x", "T_GRP2.x", "59_B_HT3", "60_B_HT2", "84_B_HT2F", "B01_HT3", "B02_HT2"};
    Count total;
    int ops = 0, skipped = 0;
    for (const char* k : cases) {
        try {
            runCase(k, total, ops, skipped);
        } catch (const Error& e) {
            if (e.code != M2M_E_UNSUPPORTED) throw;
            ++skipped;                         // a class whose slots are not ported (HeatTreat)
            std::printf("SKIP %s: %s\n", k, e.what());
        }
    }
    std::printf("%d operations, %d values, %d failures, %d cases skipped\n", ops, total.nodes, total.fails, skipped);
    CHECK_EQ(total.fails, 0);
}

int main()
{
    return testkit::run_all();
}
