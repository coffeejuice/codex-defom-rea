// ctest-labels: m2m-doe
// Supplied-owner dimension stage only; no global manager/public Save admission.
#include <testkit/check.hpp>

#include <memory>
#include <optional>
#include <utility>
#include <string>
#include <vector>

#include "m2m/common.hpp"
#include "m2m/doe_io.hpp"
#include "m2m/doe_session_notifications.hpp"
#include "m2m/doe_study.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine/conversion_session.hpp"
#include "m2m/model.hpp"
#include "m2m/moproj_write.hpp"
#include "m2m/qt4path.hpp"
#include "m2m/mst/writer.hpp"
#include "../../src/doe/variable_dimension_prefix.hpp"

using namespace m2m;
namespace {
std::string row(const char* name, int id, const char* link, const char* samples = "3")
{
    return "<DoeVariable name=\"" + std::string(name) + "\" varId=\"" + std::to_string(id) +
        "\" dimension=\"3\" numSamples=\"" + samples + "\" LinkTo=\"" + link +
        "\"><Description>retained description</Description></DoeVariable>";
}
template<class Work> std::string refused(const Work& work)
{
    try { work(); }
    catch (const Error& error) {
        CHECK_EQ(error.code, M2M_E_UNSUPPORTED);
        return error.what();
    }
    CHECK(false);
    return {};
}
struct Rig {
    engine::DiagramEffectDriver driver;
    engine::ConversionSession session{driver};
    SubProject* sub = nullptr;
    xml::NodePtr oldSub;
    explicit Rig(const std::string& rows)
    {
        auto& p = session.project();
        const auto folder = qt::L("C:/PROBLEM/variable_dim");
        initProject(p, folder, qt::L("records"));
        p.path = folder + qt::kSep + qt::L("records.moproj");
        const auto control = projectDir(p) + qt::kSep + qt::L("DoeStudies") + qt::kSep +
            qt::L("DimensionStudy") + qt::kSep + qt::L("Control") + qt::kSep;
        p.doeIo = doe::makeRecordingStudyIo();
        p.doeIo->bindModelManager(session.modelManagerOwner());
        p.doeIo->bindSessionNotificationState(session.sessionNotificationStateOwner());
        p.subs.push_back(makeSubProject(p, 2, 7));
        sub = p.subs.back().get();
        p.activeSub = 1;
        p.openedOk = true;
        p.dom = std::make_shared<xml::Document>();
        p.dom->root = xml::createElement(qt::L("Project"));
        p.dom->root->children.push_back(xml::createElement(qt::L("Sub_Project")));
        oldSub = xml::createElement(qt::L("Sub_Project"));
        p.dom->root->children.push_back(oldSub);
        CHECK(p.doeIo->writeTextFile(control + qt::L("DoeInputTable.XML"),
            "<sfDoeInputTable><DoeVariables>" + rows + "</DoeVariables><Operations/></sfDoeInputTable>"));
        CHECK(p.doeIo->writeTextFile(control + qt::L("DEF_DOE.PAR"), "0\r\n1\t4\t\r\n"));
        auto input = xml::createElement(qt::L("Sub_Project"));
        auto study = xml::createElement(qt::L("SfDoeStudy"));
        xml::setAttribute(*study, qt::L("name"), qt::L("DimensionStudy"));
        input->children.push_back(study);
        Ctx ctx;
        ctx.session = &session;
        parseSubProject(p, *input, ctx);
    }
};
void seedSamples(doe::Study& study)
{
    study.samples.mode = 2;
    for (int mode = 0; mode < 3; ++mode) {
        study.samples.tables[mode] = {{mode + 0.25, 9.0}};
        study.samples.counts[mode] = mode + 3;
        study.samples.invalid[mode] = false;
    }
}
class MinimalViews final : public moproj::DiagramViews {
public:
    void viewCentre(const SubProject&, double& x, double& y) override { x = 1; y = 2; }
    xml::NodePtr dispView(const SubProject&) override { return xml::createElement(qt::L("SfDispView")); }
    bool expanded(const SubProject&, int) override { return true; }
    moproj::RectF rect(const SubProject&, int) override { return {}; }
    std::vector<moproj::ExpansionItem> expansion(const SubProject&, int) override { return {}; }
};

Operation& addOperation(Project& project, int id, std::optional<bool> is2D = {})
{
    auto operation = std::make_unique<Operation>();
    operation->id = id;
    operation->project = &project;
    operation->classKey = qt::L("UnregisteredDimensionFixture");
    if (is2D.has_value()) {
        auto type = mst::makeBoolean(*is2D); // Existing source-qualified2D/3D dimension flags.
        const auto key = type->key();
        operation->classKey = key;
        project.registry[key] = std::move(type);
    }
    project.operations.push_back(std::move(operation));
    return *project.operations.back();
}
using Stop = doe::VariableDimensionPrefixStop;
}

TEST_CASE(variable_dimension_prefix_uses_only_first_id_and_skips_missing_before_items)
{
    Rig rig(row("A", 7, "") + row("B", 8, "") + row("C", 9, ""));
    const auto study = rig.sub->ownedDoeStudy;
    REQUIRE(study->headerVariableOwners.size() == 3);
    auto& a = *study->headerVariableOwners[0];
    auto& b = *study->headerVariableOwners[1];
    auto& c = *study->headerVariableOwners[2];
    a.dimension = b.dimension = c.dimension = 2; // Explicit existing-record value profile.
    a.items.emplace_back();
    b.items.emplace_back();
    b.ops = {999, 10};
    c.ops = {10};
    addOperation(rig.session.project(), 10); // Missing registry class defaults to3.
    const auto result = doe::reconcileHeaderDimensions(*study, rig.session.project());
    CHECK(result.stop == Stop::SuppliedRowsCompleted && !result.stoppedAt);
    CHECK(result.visitedRows == 3 && result.dimensionsStored == 1);
    CHECK(a.dimension == 2 && b.dimension == 2 && c.dimension == 3);
    CHECK(a.items.size() == 1 && b.items.size() == 1);
    CHECK(!result.completeForPublicSave);
}

TEST_CASE(variable_dimension_prefix_stores_known_class_and_default_values_without_other_state)
{
    Rig rig(row("A", 7, "B") + row("B", 8, "C") + row("C", 9, "A"));
    const auto study = rig.sub->ownedDoeStudy;
    REQUIRE(study->headerVariableOwners.size() == 3);
    auto& a = *study->headerVariableOwners[0];
    auto& b = *study->headerVariableOwners[1];
    auto& c = *study->headerVariableOwners[2];
    a.ops = b.ops = {10};
    c.ops = {11};
    b.dimension = c.dimension = 2;
    b.items.emplace_back(); // Equal dimension skips this unsupported item stage.
    a.files.emplace_back(); // Existing value fixture; no Geometry input/payload admission.
    a.files.front().path = qt::L("unchanged.GEO");
    auto& p = rig.session.project();
    addOperation(p, 10, true);
    addOperation(p, 11);
    seedSamples(*study);
    const auto samples = study->samples;
    const auto vars = study->vars;
    const auto selected = study->selectedVariableIndex;
    const int indexes[] = {a.index, b.index, c.index};
    const auto io = p.doeIo;
    const auto writes = io->textWrites().size(), reads = io->reads().size();
    const auto result = doe::reconcileHeaderDimensions(*study, p);
    CHECK(result.stop == Stop::SuppliedRowsCompleted && result.dimensionsStored == 2 && result.visitedRows == 3);
    CHECK(a.dimension == 2 && b.dimension == 2 && c.dimension == 3);
    CHECK(study->vars == vars && study->headerVariableOwners.size() == 3);
    CHECK(a.index == indexes[0] && b.index == indexes[1] && c.index == indexes[2]);
    CHECK(a.number == 7 && b.number == 8 && c.number == 9 && study->selectedVariableIndex == selected);
    CHECK(a.link == &b && b.link == &c && c.link == &a);
    CHECK(a.description == qt::L("retained description") && a.files.front().path == qt::L("unchanged.GEO"));
    CHECK(study->samples.invalid == samples.invalid && study->samples.tables == samples.tables);
    CHECK(study->samples.mode == samples.mode && study->samples.counts == samples.counts);
    CHECK(io->textWrites().size() == writes && io->reads().size() == reads && io->xmlWrites().empty());
    CHECK(rig.session.sessionNotificationStateOwner()->attempts.empty());
}

TEST_CASE(variable_dimension_prefix_keeps_store_before_item_boundary_and_leaves_later_rows)
{
    Rig rig(row("A", 7, "") + row("B", 8, "") + row("C", 9, ""));
    const auto study = rig.sub->ownedDoeStudy;
    REQUIRE(study->headerVariableOwners.size() == 3);
    for (auto& variable : study->headerVariableOwners) variable->ops = {10};
    study->headerVariableOwners[1]->items.emplace_back();
    addOperation(rig.session.project(), 10, true);
    seedSamples(*study);
    const auto samples = study->samples;
    const auto result = doe::reconcileHeaderDimensions(*study, rig.session.project());
    CHECK(result.stop == Stop::KeywordItemSnapshotBoundary && result.stoppedAt == 1);
    CHECK(result.visitedRows == 2 && result.dimensionsStored == 2);
    CHECK(study->headerVariableOwners[0]->dimension == 2);
    CHECK(study->headerVariableOwners[1]->dimension == 2); // Store precedes boundary.
    CHECK(study->headerVariableOwners[2]->dimension == 3); // Not visited.
    CHECK(study->headerVariableOwners[1]->items.size() == 1);
    CHECK(study->samples.invalid == samples.invalid && study->samples.tables == samples.tables);
    CHECK(!result.completeForPublicSave);
}

TEST_CASE(variable_dimension_prefix_later_operation_owner_refusal_retains_earlier_store)
{
    Rig rig(row("A", 7, "") + row("B", 8, ""));
    Rig foreign("");
    const auto study = rig.sub->ownedDoeStudy;
    REQUIRE(study->headerVariableOwners.size() == 2);
    study->headerVariableOwners[0]->ops = {10};
    study->headerVariableOwners[1]->ops = {20};
    addOperation(rig.session.project(), 10, true);
    auto& bad = addOperation(rig.session.project(), 20, true);
    bad.project = &foreign.session.project(); // Diagnostic unrepresented owner relation.
    seedSamples(*study);
    const auto samples = study->samples;
    CHECK(refused([&] { (void)doe::reconcileHeaderDimensions(*study, rig.session.project()); })
        .find("matched operation does not belong to the supplied Project") != std::string::npos);
    CHECK(study->headerVariableOwners[0]->dimension == 2 && study->headerVariableOwners[1]->dimension == 3);
    CHECK(study->samples.invalid == samples.invalid && study->samples.tables == samples.tables);
}

TEST_CASE(variable_dimension_prefix_supplied_project_can_differ_from_saved_study_project)
{
    Rig saved(row("A", 7, "")), selected("");
    const auto study = saved.sub->ownedDoeStudy;
    REQUIRE(study->headerVariableOwners.size() == 1);
    study->headerVariableOwners[0]->ops = {10};
    addOperation(saved.session.project(), 10, false);
    addOperation(selected.session.project(), 10, true);
    const auto result = doe::reconcileHeaderDimensions(*study, selected.session.project());
    CHECK(result.stop == Stop::SuppliedRowsCompleted && result.dimensionsStored == 1);
    CHECK(study->headerVariableOwners[0]->dimension == 2);
    CHECK(study->sub == saved.sub && saved.sub->project == &saved.session.project());
    CHECK(saved.sub->ownedDoeStudy == study); // No re-selection/replacement of the saved owner.
}

TEST_CASE(variable_dimension_prefix_malformed_study_ownership_refuses_before_stores)
{
    for (int profile = 0; profile < 5; ++profile) {
        Rig rig(row("A", 7, "") + row("B", 8, ""));
        const auto study = rig.sub->ownedDoeStudy;
        REQUIRE(study->headerVariableOwners.size() == 2);
        auto* const a = study->headerVariableOwners[0].get();
        a->ops = {10};
        addOperation(rig.session.project(), 10, true);
        doe::Var foreign;
        if (profile == 0) study->vars.push_back(nullptr);
        if (profile == 1) study->vars[1] = nullptr;
        if (profile == 2) study->vars[1] = &foreign;
        if (profile == 3) study->vars[1] = a;
        if (profile == 4) study->headerVariableOwners[1].reset();
        const auto vars = study->vars;
        CHECK(refused([&] { (void)doe::reconcileHeaderDimensions(*study, rig.session.project()); })
            .find("header") != std::string::npos);
        CHECK(a->dimension == 3 && study->vars == vars && study->headerVariableOwners.size() == 2);
    }
}

TEST_CASE(variable_dimension_prefix_private_entry_does_not_replace_public_reconciliation)
{
    Rig rig(row("A", 7, ""));
    const auto study = rig.sub->ownedDoeStudy;
    REQUIRE(study->headerVariableOwners.size() == 1);
    study->headerVariableOwners[0]->ops = {10};
    auto& p = rig.session.project();
    addOperation(p, 10, true);
    seedSamples(*study);
    const auto samples = study->samples;
    const auto writes = p.doeIo->textWrites().size();
    auto notifier = rig.session.sessionNotificationStateOwner();
    notifier->flag90 = true;
    MinimalViews views;
    Ctx ctx;
    ctx.session = &rig.session;
    engine::FileEffects opened;
    engine::SaveEffects saved;
    CHECK(refused([&] { (void)engine::saveProject(p, ctx, views, opened, saved); })
        .find("variable dimension reconciliation remains unsupported") != std::string::npos);
    CHECK(study->headerVariableOwners[0]->dimension == 3 && study->vars.size() == 1);
    CHECK(study->samples.invalid == samples.invalid && study->samples.tables == samples.tables);
    CHECK(p.dom->root->children[1] == rig.oldSub && rig.oldSub->children.empty());
    CHECK(p.doeIo->textWrites().size() == writes && p.doeIo->xmlWrites().empty());
    CHECK(notifier->flag90 && notifier->attempts.empty() && saved.project.writes.empty());
}

int main() { return testkit::run_all(); }
