// ctest-labels: m2m-doe
// Isolated Study mutation; public reconciliation/file/refresh guards remain.
#include <testkit/check.hpp>

#include <memory>
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
#include "../../src/doe/empty_variable_prune.hpp"
#include "../../src/doe/geometry_row_prefix.hpp"

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
        const auto folder = qt::L("C:/PROBLEM/variable_prune");
        initProject(p, folder, qt::L("records"));
        p.path = folder + qt::kSep + qt::L("records.moproj");
        const auto control = projectDir(p) + qt::kSep + qt::L("DoeStudies") + qt::kSep +
            qt::L("PruneStudy") + qt::kSep + qt::L("Control") + qt::kSep;
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
        xml::setAttribute(*study, qt::L("name"), qt::L("PruneStudy"));
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
}

TEST_CASE(empty_variable_prune_removes_adjacent_positive_and_zero_sample_cycles_without_io)
{
    Rig rig(row("A", 7, "B") + row("B", 8, "A", "") +
        row("C", 9, "B", "5") + row("Self", 10, "Self", ""));
    const auto study = rig.sub->ownedDoeStudy;
    REQUIRE(study && study->headerVariableOwners.size() == 4);
    CHECK(study->vars[0]->link == study->vars[1] && study->vars[1]->link == study->vars[0]);
    CHECK(study->vars[3]->link == study->vars[3]);
    CHECK(study->headerVariableOwners[0]->numSamples == 3);
    CHECK(study->headerVariableOwners[1]->numSamples == 0);
    seedSamples(*study);
    const auto samples = study->samples;
    const auto selected = study->selectedVariableIndex;
    const auto io = rig.session.project().doeIo;
    const auto writes = io->textWrites().size(), reads = io->reads().size(), dirs = io->directories().size();
    doe::pruneEmptyHeaderVariables(*study);
    CHECK(study->vars.empty() && study->headerVariableOwners.empty());
    CHECK(study->samples.invalid[0] && study->samples.invalid[1] && study->samples.invalid[2]);
    CHECK(study->samples.mode == samples.mode && study->samples.tables == samples.tables);
    CHECK(study->samples.counts == samples.counts && study->selectedVariableIndex == selected);
    CHECK(rig.sub->ownedDoeStudy == study);
    CHECK(io->textWrites().size() == writes && io->reads().size() == reads && io->directories().size() == dirs);
    CHECK(io->xmlWrites().empty() && rig.session.sessionNotificationStateOwner()->attempts.empty());
    doe::pruneEmptyHeaderVariables(*study); // No additional mutation on an empty owner.
    CHECK(study->samples.tables == samples.tables && study->samples.counts == samples.counts);
}

TEST_CASE(empty_variable_prune_retains_counted_rows_and_clears_only_incoming_removed_links)
{
    Rig rig(row("Drop", 7, "Keyword") + row("Keyword", 8, "Drop", "") +
        row("Geometry", 9, "Keyword") + row("TailDrop", 10, "Drop", ""));
    const auto study = rig.sub->ownedDoeStudy;
    REQUIRE(study->headerVariableOwners.size() == 4);
    auto* const keyword = study->headerVariableOwners[1].get();
    auto* const geometry = study->headerVariableOwners[2].get();
    keyword->items.emplace_back(); // Count-only fixture, not Keyword reader admission.
    const doe::GeometryRowPrefixInput request{qt::L("nominal.GEO"), qt::L("upper.GEO"), qt::L("lower.GEO"),
        1, 2, 9, std::nullopt, std::nullopt};
    CHECK(doe::applyGeometryRowPrefix(*geometry, request).appended); // Existing isolated row projection.
    seedSamples(*study);
    const auto keywordIndex = keyword->index, geometryIndex = geometry->index;
    const auto selected = study->selectedVariableIndex;
    const auto samples = study->samples;
    doe::pruneEmptyHeaderVariables(*study);
    REQUIRE(study->vars.size() == 2 && study->headerVariableOwners.size() == 2);
    CHECK(study->vars[0] == keyword && study->vars[1] == geometry);
    CHECK(study->headerVariableOwners[0].get() == keyword && study->headerVariableOwners[1].get() == geometry);
    CHECK(keyword->link == nullptr && geometry->link == keyword);
    CHECK(keyword->index == keywordIndex && geometry->index == geometryIndex); // No reindex.
    CHECK(keyword->number == 8 && geometry->number == 9 && study->selectedVariableIndex == selected);
    CHECK(keyword->description == qt::L("retained description"));
    CHECK(keyword->items.size() == 1 && geometry->files.size() == 1);
    CHECK(study->samples.mode == samples.mode && study->samples.tables == samples.tables);
    CHECK(study->samples.counts == samples.counts);
    CHECK(study->samples.invalid[0] && study->samples.invalid[1] && study->samples.invalid[2]);
    study->samples.invalid.fill(false);
    doe::pruneEmptyHeaderVariables(*study); // Neither retained row is eligible.
    CHECK(!study->samples.invalid[0] && !study->samples.invalid[1] && !study->samples.invalid[2]);
    CHECK(study->vars[0] == keyword && study->vars[1] == geometry && geometry->link == keyword);
}

TEST_CASE(empty_variable_prune_empty_owner_preserves_sample_state)
{
    Rig rig("");
    const auto study = rig.sub->ownedDoeStudy;
    seedSamples(*study);
    const auto samples = study->samples;
    const auto selected = study->selectedVariableIndex;
    doe::pruneEmptyHeaderVariables(*study);
    CHECK(study->vars.empty() && study->headerVariableOwners.empty());
    CHECK(study->samples.tables == samples.tables && study->samples.counts == samples.counts);
    CHECK(study->samples.invalid == samples.invalid && study->samples.mode == samples.mode);
    CHECK(study->selectedVariableIndex == selected);
}

TEST_CASE(empty_variable_prune_unrepresented_owner_states_refuse_before_graph_mutation)
{
    for (int profile = 0; profile < 10; ++profile) {
        Rig rig(row("A", 7, "B") + row("B", 8, "A"));
        const auto study = rig.sub->ownedDoeStudy;
        REQUIRE(study->vars.size() == 2 && study->headerVariableOwners.size() == 2);
        auto* const a = study->vars[0];
        auto* const target = study->vars[1];
        doe::Var foreign;
        foreign.name = qt::L("Foreign");
        if (profile == 0) study->vars.push_back(nullptr);
        if (profile == 1) study->vars[1] = nullptr;
        if (profile == 2) study->vars[1] = &foreign;
        if (profile == 3) {
            study->scalarPrefixVariableOwners.push_back(std::make_unique<doe::Var>());
            study->vars[1] = study->scalarPrefixVariableOwners.back().get();
        }
        if (profile == 4) {
            study->geometryPrefixVariableOwners.push_back(std::make_unique<doe::Var>());
            study->vars[1] = study->geometryPrefixVariableOwners.back().get();
        }
        if (profile == 5) study->vars[1] = a;
        if (profile == 6) study->vars[1]->name = a->name;
        if (profile == 7) { a->name = qt::String(); study->vars[1]->name = qt::L(""); }
        if (profile == 8) study->headerVariableOwners.push_back(std::make_unique<doe::HeaderVariableRecord>(3));
        if (profile == 9) study->headerVariableOwners[1].reset(); // Raw alias is never dereferenced by admission.
        seedSamples(*study);
        const auto vars = study->vars;
        const auto owners = study->headerVariableOwners.size();
        const auto samples = study->samples;
        CHECK(refused([&] { doe::pruneEmptyHeaderVariables(*study); }).find("empty-variable prune") != std::string::npos);
        CHECK(study->vars == vars && study->headerVariableOwners.size() == owners);
        CHECK(study->headerVariableOwners[0].get() == a && a->link == target);
        CHECK(study->samples.invalid == samples.invalid && study->samples.tables == samples.tables);
        CHECK(study->samples.counts == samples.counts);
        CHECK(rig.session.project().doeIo->xmlWrites().empty());
    }
}

TEST_CASE(empty_variable_prune_is_not_inserted_ahead_of_public_save_reconciliation)
{
    Rig rig(row("A", 7, "B") + row("B", 8, "A", ""));
    const auto study = rig.sub->ownedDoeStudy;
    REQUIRE(study->vars.size() == 2);
    const auto vars = study->vars;
    seedSamples(*study);
    const auto samples = study->samples;
    auto& p = rig.session.project();
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
    CHECK(study->vars == vars && study->headerVariableOwners.size() == 2);
    CHECK(vars[0]->link == vars[1] && vars[1]->link == vars[0]);
    CHECK(study->samples.invalid == samples.invalid && study->samples.tables == samples.tables);
    CHECK(p.dom->root->children[1] == rig.oldSub && rig.oldSub->children.empty());
    CHECK(p.doeIo->textWrites().size() == writes && p.doeIo->xmlWrites().empty());
    CHECK(notifier->flag90 && notifier->attempts.empty() && saved.project.writes.empty());
}

int main() { return testkit::run_all(); }
