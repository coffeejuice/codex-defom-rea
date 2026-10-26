// ctest-labels: m2m-doe
// Empty child reader only; existing records do not prove variable parse/delivery.
#include <testkit/check.hpp>

#include <initializer_list>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "m2m/common.hpp"
#include "m2m/doe_io.hpp"
#include "m2m/doe_study.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine/conversion_session.hpp"
#include "m2m/model.hpp"
#include "m2m/moproj_write.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/qt4path.hpp"
#include "../support/test_paths.hpp"

using namespace m2m;
namespace {
std::string table(const std::string& children)
{ return "<sfDoeInputTable>" + children + "</sfDoeInputTable>"; }
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
struct Variables {
    doe::Study study;
    std::vector<std::unique_ptr<doe::Var>> owners;
    explicit Variables(std::initializer_list<int> numbers)
    {
        for (int number : numbers) {
            auto variable = std::make_unique<doe::Var>();
            variable->number = number;
            variable->index = 90 + static_cast<int>(owners.size());
            variable->name = qt::L("existing");
            variable->ops = {2, 5};
            study.vars.push_back(variable.get());
            owners.push_back(std::move(variable));
        }
    }
};

struct ActiveRig {
    engine::DiagramEffectDriver driver;
    engine::ConversionSession session{driver};
    SubProject* sub = nullptr;
    xml::NodePtr oldSub;
    String control;
    std::size_t seedWrites = 0;
    ActiveRig()
    {
        auto& p = session.project();
        const String folder((m2m_test::scratchRoot() / "doe-empty-reindex").u16string());
        initProject(p, folder, qt::L("reindex"));
        p.path = folder + qt::kSep + qt::L("reindex.moproj");
        p.doeIo = doe::makeRecordingStudyIo();
        p.doeIo->bindModelManager(session.modelManagerOwner());
        p.doeIo->bindSessionNotificationState(session.sessionNotificationStateOwner());
        p.subs.push_back(makeSubProject(p, 2, 7));
        sub = p.subs.back().get();
        p.activeSub = 1; // Explicit fixture, not a new C ABI selection rule.
        p.openedOk = true;
        for (int number : {-4, 0, 7}) {
            auto variable = std::make_unique<doe::Var>();
            variable->number = number;
            variable->index = 81;
            sub->ownedDoeStudy->vars.push_back(variable.get());
            // Concrete plain Var deletion, no derived ownership shortcut.
            sub->ownedDoeStudy->scalarPrefixVariableOwners.push_back(std::move(variable));
        }
        p.dom = std::make_shared<xml::Document>();
        p.dom->root = xml::createElement(qt::L("Project"));
        p.dom->root->children.push_back(xml::createElement(qt::L("Sub_Project")));
        oldSub = xml::createElement(qt::L("Sub_Project"));
        p.dom->root->children.push_back(oldSub);
        control = projectDir(p) + qt::kSep + qt::L("DoeStudies") + qt::kSep +
            qt::L("ReindexedStudy") + qt::kSep + qt::L("Control") + qt::kSep;
        CHECK(p.doeIo->writeTextFile(control + qt::L("DoeInputTable.XML"),
            table("<DoeVariables/><Operations><Operation id=\"5\"/></Operations>")));
        CHECK(p.doeIo->writeTextFile(control + qt::L("DEF_DOE.PAR"), "0\r\n1\t4\t\r\n"));
        seedWrites = p.doeIo->textWrites().size();
    }
    void read()
    {
        auto input = xml::createElement(qt::L("Sub_Project"));
        auto study = xml::createElement(qt::L("SfDoeStudy"));
        xml::setAttribute(*study, qt::L("name"), qt::L("ReindexedStudy"));
        input->children.push_back(study);
        Ctx ctx;
        ctx.session = &session;
        parseSubProject(session.project(), *input, ctx);
    }
};
class MinimalViews final : public moproj::DiagramViews {
public:
    void viewCentre(const SubProject&, double& x, double& y) override { x = 1; y = 2; }
    xml::NodePtr dispView(const SubProject&) override { return xml::createElement(qt::L("SfDispView")); }
    bool expanded(const SubProject&, int) override { return true; }
    moproj::RectF rect(const SubProject&, int) override { return {}; }
    std::vector<moproj::ExpansionItem> expansion(const SubProject&, int) override { return {}; }
};
}

TEST_CASE(doe_empty_variables_reindexes_existing_records_without_replacing_owners)
{
    Variables vars{4, 4, 9};
    const auto aliases = vars.study.vars;
    vars.owners[0]->link = vars.owners[1].get();
    CHECK(doe::readInputTable(table("<DoeVariables/>"), vars.study));
    CHECK(vars.study.vars == aliases);
    REQUIRE(vars.owners.size() == 3);
    for (std::size_t index = 0; index < vars.owners.size(); ++index) {
        CHECK_EQ(vars.owners[index]->index, static_cast<int>(index));
        CHECK(vars.owners[index]->name == qt::L("existing"));
        CHECK(vars.owners[index]->ops == std::vector<int>({2, 5}));
    }
    CHECK_EQ(vars.owners[0]->number, 4);
    CHECK_EQ(vars.owners[1]->number, 4);
    CHECK_EQ(vars.owners[2]->number, 9);
    CHECK(vars.owners[0]->link == vars.owners[1].get());
}

TEST_CASE(doe_empty_variables_duplicate_alias_keeps_last_index)
{
    Variables vars{2};
    vars.study.vars.push_back(vars.owners[0].get());
    CHECK(doe::readInputTable(table("<DoeVariables/>"), vars.study));
    CHECK(vars.study.vars[0] == vars.study.vars[1]);
    CHECK_EQ(vars.owners[0]->index, 1);
    CHECK_EQ(vars.owners[0]->number, 2);
}

TEST_CASE(doe_empty_variables_null_fault_retains_only_prior_indexes_and_headers)
{
    Variables vars{0, 6, 8};
    vars.study.vars[1] = nullptr;
    CHECK(refused([&] { (void)doe::readInputTable(table(
        "<DoeRoiType type=\"5\"/><DoeVariables/><DoeRoiType type=\"19\"/>"), vars.study); })
        .find("0x14091c800") != std::string::npos);
    CHECK_EQ(vars.owners[0]->index, 0);
    CHECK_EQ(vars.owners[1]->index, 91);
    CHECK_EQ(vars.owners[2]->index, 92);
    CHECK_EQ(vars.owners[0]->number, 0); // Outer number pass was not reached.
    CHECK_EQ(vars.study.roiType, 5); // Later sibling was not reached.
}

TEST_CASE(doe_empty_variables_reindex_precedes_later_payload_guard_and_not_id_pass)
{
    Variables regions{0, 5};
    regions.study.regions = {qt::L("prior")};
    CHECK(!refused([&] { (void)doe::readInputTable(table(
        "<DoeVariables/><RegionsOfInterest><Region/></RegionsOfInterest>"
        "<Constraints><Constraint/></Constraints>"), regions.study); }).empty());
    CHECK_EQ(regions.owners[0]->index, 0);
    CHECK_EQ(regions.owners[1]->index, 1);
    CHECK_EQ(regions.owners[0]->number, 0);
    REQUIRE(regions.study.regions.size() == 1);
    CHECK(regions.study.regions.front().isNull());
    Variables constraints{0};
    constraints.study.constraints = 4;
    CHECK(!refused([&] { (void)doe::readInputTable(table(
        "<DoeVariables/><Constraints><Constraint/></Constraints>"), constraints.study); }).empty());
    CHECK_EQ(constraints.owners[0]->index, 0);
    CHECK_EQ(constraints.owners[0]->number, 0);
    CHECK_EQ(constraints.study.constraints, 0);
}

TEST_CASE(doe_empty_variables_earlier_or_nonempty_payload_guard_prevents_reindex)
{
    Variables regions{0};
    CHECK(!refused([&] { (void)doe::readInputTable(table(
        "<RegionsOfInterest><Region/></RegionsOfInterest>"
        "<Constraints><Constraint/></Constraints><DoeVariables/>"), regions.study); }).empty());
    CHECK_EQ(regions.owners[0]->index, 90);
    CHECK_EQ(regions.owners[0]->number, 0);
    Variables populated{0};
    CHECK(refused([&] { (void)doe::readInputTable(table(
        "<DoeVariables><DoeVariable name=\"new\"/></DoeVariables>"), populated.study); })
        .find("DoeVariables reader") != std::string::npos);
    CHECK_EQ(populated.owners[0]->index, 90);
    CHECK_EQ(populated.owners[0]->number, 0);
}

TEST_CASE(doe_empty_variables_repeated_empty_readers_do_not_repeat_outer_number_pass)
{
    Variables vars{-9, -2};
    CHECK(doe::readInputTable(table(
        "<DoeVariables>text<!--comment--><![CDATA[raw]]></DoeVariables><DoeVariables/>"), vars.study));
    CHECK_EQ(vars.owners[0]->index, 0);
    CHECK_EQ(vars.owners[1]->index, 1);
    CHECK_EQ(vars.owners[0]->number, 0);
    CHECK_EQ(vars.owners[1]->number, 1); // Only one outer pass, after both children.
}

TEST_CASE(doe_empty_variables_actual_owner_lazy_read_continues_sample_order)
{
    ActiveRig rig;
    auto& p = rig.session.project();
    const auto owner = rig.sub->ownedDoeStudy;
    const auto aliases = owner->vars;
    rig.read();
    CHECK(activeSubProject(p) == rig.sub);
    CHECK(rig.sub->ownedDoeStudy == owner && rig.sub->doe == owner.get());
    CHECK(owner->vars == aliases);
    for (std::size_t index = 0; index < owner->vars.size(); ++index)
        CHECK_EQ(owner->vars[index]->index, static_cast<int>(index));
    CHECK_EQ(owner->vars[0]->number, 8);
    CHECK_EQ(owner->vars[1]->number, 9);
    CHECK_EQ(owner->vars[2]->number, 7);
    REQUIRE(p.doeIo->textWrites().size() == rig.seedWrites + 3);
    const char* names[] = {"Samples_LHS.par", "Samples_FF.par", "Samples_USR.par"};
    for (std::size_t mode = 0; mode < 3; ++mode)
        CHECK(p.doeIo->textWrites()[rig.seedWrites + mode].path == rig.control + qt::L(names[mode]));
    REQUIRE(p.doeIo->reads().size() == 2);
    CHECK(p.doeIo->reads().back().path == rig.control + qt::L("DEF_DOE.PAR"));
    CHECK(owner->samples.tables[0] == doe::SampleTable({{4.0}}));
    CHECK(p.doeIo->xmlWrites().empty());
}

TEST_CASE(doe_empty_variables_actual_owner_save_keeps_variable_reconciliation_guard)
{
    ActiveRig rig;
    rig.read();
    auto& p = rig.session.project();
    const auto writesBefore = p.doeIo->textWrites().size();
    MinimalViews views;
    Ctx ctx;
    ctx.session = &rig.session;
    engine::FileEffects opened;
    engine::SaveEffects saved;
    CHECK(refused([&] { (void)engine::saveProject(p, ctx, views, opened, saved); })
        .find("InputTable variable dimension reconciliation remains unsupported") != std::string::npos);
    CHECK(p.dom->root->children[1] == rig.oldSub);
    CHECK(rig.oldSub->children.empty());
    CHECK(p.doeIo->xmlWrites().empty());
    CHECK(p.doeIo->textWrites().size() == writesBefore);
    CHECK(saved.project.writes.empty());
    CHECK_EQ(rig.sub->ownedDoeStudy->vars[0]->index, 0);
    CHECK_EQ(rig.sub->ownedDoeStudy->vars[0]->number, 8);
}

int main() { return testkit::run_all(); }
