// ctest-labels: m2m-doe
// Region names only; no geometry payload/Qt container-allocation parity implied.
#include <testkit/check.hpp>

#include <memory>
#include <string>
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
        const String folder((m2m_test::scratchRoot() / "doe-region-names").u16string());
        initProject(p, folder, qt::L("regions"));
        p.path = folder + qt::kSep + qt::L("regions.moproj");
        p.doeIo = doe::makeRecordingStudyIo();
        p.doeIo->bindModelManager(session.modelManagerOwner());
        p.doeIo->bindSessionNotificationState(session.sessionNotificationStateOwner());
        p.subs.push_back(makeSubProject(p, 2, 7));
        sub = p.subs.back().get();
        p.activeSub = 1; // Explicit actual-owner fixture, not public C ABI selection.
        p.openedOk = true;
        sub->ownedDoeStudy->regions = {qt::L("prior")};
        p.dom = std::make_shared<xml::Document>();
        p.dom->root = xml::createElement(qt::L("Project"));
        p.dom->root->children.push_back(xml::createElement(qt::L("Sub_Project")));
        oldSub = xml::createElement(qt::L("Sub_Project"));
        p.dom->root->children.push_back(oldSub);
        control = projectDir(p) + qt::kSep + qt::L("DoeStudies") + qt::kSep +
            qt::L("RegionStudy") + qt::kSep + qt::L("Control") + qt::kSep;
        CHECK(p.doeIo->writeTextFile(control + qt::L("DoeInputTable.XML"), table(
            "<RegionsOfInterest><Region name=\"first\"/><Region/><Region name=\"first\"/>"
            "</RegionsOfInterest><DoeVariables/><Operations/>")));
        CHECK(p.doeIo->writeTextFile(control + qt::L("DEF_DOE.PAR"), "0\r\n1\t4\t\r\n"));
        seedWrites = p.doeIo->textWrites().size();
    }
    void read()
    {
        auto input = xml::createElement(qt::L("Sub_Project"));
        auto study = xml::createElement(qt::L("SfDoeStudy"));
        xml::setAttribute(*study, qt::L("name"), qt::L("RegionStudy"));
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

TEST_CASE(doe_region_names_select_exact_direct_children_and_preserve_values)
{
    doe::Study study;
    study.regions = {qt::L("prior")};
    study.constraints = 7;
    study.roiType = 3;
    CHECK(doe::readInputTable(table(
        "<RegionsOfInterest>text<!--comment--><Region name=\" first \"/>"
        "<region name=\"wrong-case\"/><Group><Region name=\"nested\"/></Group>"
        "<Region name=\"dup\"/><Region name=\"dup\"/><Region/><Region name=\"\"/>"
        "<Region name=\"&#x3A9;\"/></RegionsOfInterest>"), study));
    REQUIRE(study.regions.size() == 6);
    CHECK(study.regions[0] == qt::L(" first "));
    CHECK(study.regions[1] == qt::L("dup") && study.regions[2] == qt::L("dup"));
    CHECK(study.regions[3].isNull());
    CHECK(study.regions[4].isEmpty() && !study.regions[4].isNull());
    CHECK(study.regions[5] == String(u"\u03a9"));
    CHECK_EQ(study.constraints, 7);
    CHECK_EQ(study.roiType, 3);
    CHECK(study.vars.empty());
}

TEST_CASE(doe_region_names_unknown_only_list_still_clears_and_returns_true)
{
    doe::Study study;
    study.regions = {qt::L("prior")};
    CHECK(doe::readInputTable(table(
        "<RegionsOfInterest><![CDATA[raw]]><Unknown><Region name=\"nested\"/></Unknown>"
        "<region name=\"case\"/></RegionsOfInterest>"), study));
    CHECK(study.regions.empty());
}

TEST_CASE(doe_region_names_repeated_lists_replace_rather_than_merge)
{
    doe::Study study;
    CHECK(doe::readInputTable(table(
        "<RegionsOfInterest><Region name=\"first\"/></RegionsOfInterest>"
        "<RegionsOfInterest><Region name=\"last\"/><Region name=\"last\"/></RegionsOfInterest>"), study));
    CHECK(study.regions == std::vector<String>({qt::L("last"), qt::L("last")}));
    CHECK(doe::readInputTable(table(
        "<RegionsOfInterest><Region name=\"discarded\"/></RegionsOfInterest><RegionsOfInterest/>"), study));
    CHECK(study.regions.empty());
}

TEST_CASE(doe_region_names_later_nonempty_variable_guard_keeps_prefix)
{
    doe::Study study;
    doe::Var variable;
    variable.number = 0;
    variable.index = 44;
    study.vars.push_back(&variable);
    CHECK(refused([&] { (void)doe::readInputTable(table(
        "<RegionsOfInterest><Region name=\"parsed\"/></RegionsOfInterest>"
        "<DoeVariables><DoeVariable/></DoeVariables>"), study); })
        .find("DoeVariables reader") != std::string::npos);
    CHECK(study.regions == std::vector<String>({qt::L("parsed")}));
    CHECK_EQ(variable.number, 0);
    CHECK_EQ(variable.index, 44);
}

TEST_CASE(doe_region_names_later_constraint_guard_keeps_indexes_names_but_not_id_pass)
{
    doe::Study study;
    doe::Var variable;
    variable.number = 0;
    variable.index = 44;
    study.vars.push_back(&variable);
    study.constraints = 5;
    CHECK(refused([&] { (void)doe::readInputTable(table(
        "<DoeVariables/><RegionsOfInterest><Region name=\"parsed\"/></RegionsOfInterest>"
        "<Constraints><Constraint/></Constraints>"), study); })
        .find("Constraints reader") != std::string::npos);
    CHECK(study.regions == std::vector<String>({qt::L("parsed")}));
    CHECK_EQ(variable.index, 0);
    CHECK_EQ(variable.number, 0);
    CHECK_EQ(study.constraints, 0);
}

TEST_CASE(doe_region_names_earlier_guard_and_parse_failure_prevent_clear)
{
    doe::Study study;
    study.regions = {qt::L("prior")};
    CHECK(!refused([&] { (void)doe::readInputTable(table(
        "<DoeVariables><DoeVariable/></DoeVariables>"
        "<RegionsOfInterest><Region name=\"unreached\"/></RegionsOfInterest>"), study); }).empty());
    CHECK(study.regions == std::vector<String>({qt::L("prior")}));
    CHECK(!doe::readInputTable(
        "<sfDoeInputTable><RegionsOfInterest><Region></sfDoeInputTable>", study));
    CHECK(study.regions == std::vector<String>({qt::L("prior")}));
}

TEST_CASE(doe_region_names_actual_active_owner_read_continues_samples_in_order)
{
    ActiveRig rig;
    auto& p = rig.session.project();
    const auto owner = rig.sub->ownedDoeStudy;
    rig.read();
    CHECK(activeSubProject(p) == rig.sub);
    CHECK(rig.sub->ownedDoeStudy == owner && rig.sub->doe == owner.get());
    REQUIRE(owner->regions.size() == 3);
    CHECK(owner->regions.front() == qt::L("first") && owner->regions.back() == qt::L("first"));
    CHECK(owner->regions[1].isNull());
    REQUIRE(p.doeIo->textWrites().size() == rig.seedWrites + 3);
    const char* names[] = {"Samples_LHS.par", "Samples_FF.par", "Samples_USR.par"};
    for (std::size_t mode = 0; mode < 3; ++mode)
        CHECK(p.doeIo->textWrites()[rig.seedWrites + mode].path == rig.control + qt::L(names[mode]));
    REQUIRE(p.doeIo->reads().size() == 2);
    CHECK(p.doeIo->reads().back().path == rig.control + qt::L("DEF_DOE.PAR"));
    CHECK(owner->samples.tables[0] == doe::SampleTable({{4.0}}));
    CHECK(p.doeIo->xmlWrites().empty());
}

TEST_CASE(doe_region_names_actual_owner_save_serializes_names_before_refresh_guard)
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
        .find("main-window +458 is unavailable") != std::string::npos);
    CHECK(p.dom->root->children[1] == rig.oldSub && rig.oldSub->children.empty());
    CHECK(p.doeIo->textWrites().size() == writesBefore);
    REQUIRE(saved.project.xmlJournal != nullptr);
    REQUIRE(saved.project.xmlJournal->attempts().size() == 1);
    const auto& attempt = saved.project.xmlJournal->attempts().front();
    CHECK(attempt.path == rig.control + qt::L("DoeInputTable.XML"));
    CHECK(attempt.opened && attempt.closed && attempt.refreshBoundaryReached);
    REQUIRE(attempt.normalWriterReturn.has_value());
    CHECK(attempt.normalWriterReturn->writerSucceeded);
    const auto bytes = saved.project.xmlJournal->recordedFile(attempt.path);
    REQUIRE(bytes.has_value());
    xml::Document document;
    String error;
    int line = 0, column = 0;
    REQUIRE(xml::setContent(document, *bytes, &error, &line, &column));
    REQUIRE(document.root != nullptr);
    const auto lists = document.root->childElements(qt::L("RegionsOfInterest"));
    REQUIRE(lists.size() == 1);
    const auto regions = lists.front()->childElements(qt::L("Region"));
    REQUIRE(regions.size() == 3);
    CHECK(regions[0]->attribute(qt::L("name")) == qt::L("first"));
    CHECK(regions[1]->hasAttribute(qt::L("name")));
    CHECK(regions[1]->attribute(qt::L("name")).isEmpty());
    CHECK(regions[2]->attribute(qt::L("name")) == qt::L("first"));
    CHECK(saved.project.writes.empty()); // Immediate XML cannot be replayed.
}

int main() { return testkit::run_all(); }
