// ctest-labels: m2m-doe
// Private planned INI text on the actual Study; no native I/O/public Save claim.
#include <testkit/check.hpp>

#include <memory>
#include <string>
#include <vector>

#include "m2m/common.hpp"
#include "m2m/doe_constraint_record.hpp"
#include "m2m/doe_io.hpp"
#include "m2m/doe_session_notifications.hpp"
#include "m2m/doe_study.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine/conversion_session.hpp"
#include "m2m/model.hpp"
#include "m2m/moproj_write.hpp"
#include "m2m/qt4path.hpp"

using namespace m2m;
namespace {
std::string row(int id, int object = 1, int region = 2)
{
    return "<Constraint Id=\"" + std::to_string(id) + "\" Value=\"0\" ObjectNo=\"" +
        std::to_string(object) + "\" RegionNo=\"" + std::to_string(region) + "\"/>";
}
const std::string mixed = row(1010) + row(200) + row(1081) + row(205) + row(1012);
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
    qt::String control;
    explicit Rig(const std::string& records = mixed)
    {
        auto& p = session.project();
        const auto folder = qt::L("C:/PROBLEM/constraint_ini");
        initProject(p, folder, qt::L("records"));
        p.path = folder + qt::kSep + qt::L("records.moproj");
        control = projectDir(p) + qt::kSep + qt::L("DoeStudies") + qt::kSep +
            qt::L("ConstraintIniStudy") + qt::kSep + qt::L("Control");
        p.doeIo = doe::makeRecordingStudyIo();
        p.doeIo->bindModelManager(session.modelManagerOwner());
        p.doeIo->bindSessionNotificationState(session.sessionNotificationStateOwner());
        p.subs.push_back(makeSubProject(p, 2, 7));
        sub = p.subs.back().get();
        p.activeSub = 1; // Actual C++ owner fixture; no C ABI Type2 selection claim.
        p.openedOk = true;
        p.dom = std::make_shared<xml::Document>();
        p.dom->root = xml::createElement(qt::L("Project"));
        p.dom->root->children.push_back(xml::createElement(qt::L("Sub_Project")));
        oldSub = xml::createElement(qt::L("Sub_Project"));
        p.dom->root->children.push_back(oldSub);
        CHECK(p.doeIo->writeTextFile(control + qt::kSep + qt::L("DoeInputTable.XML"),
            "<sfDoeInputTable><DoeVariables/><Constraints>" + records +
            "</Constraints><Operations/></sfDoeInputTable>"));
        CHECK(p.doeIo->writeTextFile(control + qt::kSep + qt::L("DEF_DOE.PAR"), "0\r\n1\t4\t\r\n"));
        auto input = xml::createElement(qt::L("Sub_Project"));
        auto study = xml::createElement(qt::L("SfDoeStudy"));
        xml::setAttribute(*study, qt::L("name"), qt::L("ConstraintIniStudy"));
        input->children.push_back(study);
        Ctx ctx;
        ctx.session = &session;
        parseSubProject(p, *input, ctx); // Import and retain concrete records on this Study.
    }
    void plan(engine::FileEffects& effects)
    {
        CHECK(doe::writeDefDoeIni(control, qt::L("model.DB"), *sub->ownedDoeStudy, false, false, effects));
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

TEST_CASE(def_doe_constraints_actual_owner_keeps_two_pass_numbering_and_section_order)
{
    Rig rig;
    const auto study = rig.sub->ownedDoeStudy;
    REQUIRE(study && study->constraintRecords.size() == 5);
    std::vector<const doe::ConstraintRecord*> identities;
    for (const auto& record : study->constraintRecords) identities.push_back(record.get());
    study->objNo = 4;
    study->oprNo = 8;
    study->designCounts = {1, 0, 1}; // Explicit existing planned-writer multiplicities.
    study->regions = {qt::L("RegionA")};
    const auto io = rig.session.project().doeIo;
    const auto writes = io->textWrites().size();
    engine::FileEffects effects;
    rig.plan(effects);
    REQUIRE(effects.writes.size() == 1);
    CHECK(effects.writes.front().path == rig.control + qt::kSep + qt::L("DEF_DOE.INI"));
    CHECK(effects.writes.front().bytes ==
        "NOMINAL_DB\r\n../../../model.DB\r\nOBJ_NO\r\n4\r\nOPR_NO\r\n8\r\n"
        "DO_NOT_POS\r\nROI_TYPE\r\n0\r\nDOE_DBS\r\n"
        "../Solutions/Run1/model.DB\r\n../Solutions/Run2/model.DB\r\n"
        "\r\nNUM_CON\t\t5\r\n"
        "  2 200    1      0    2\r\n"
        "  4 205    1      0    2\r\n"
        "  3 1010    1      0    2\r\n"
        "  4 1081    1      0.0    2\r\n"
        "  5 1012    1      0    2\r\n"
        "\r\nREGIONS    \t\t1\r\nRegions/RegionA.GEO\r\n");
    CHECK(io->textWrites().size() == writes && io->xmlWrites().empty()); // A plan is not executed I/O.
    CHECK(rig.sub->ownedDoeStudy == study && study->constraints == 5);
    for (std::size_t i = 0; i < identities.size(); ++i) {
        CHECK(study->constraintRecords[i].get() == identities[i]);
        CHECK(!identities[i]->sourceField10Bits);
    }
}

TEST_CASE(def_doe_constraints_duplicate_category2_rows_keep_fields_and_distinct_identity)
{
    Rig rig(row(200, 3, 5) + row(200, 7, 9));
    const auto study = rig.sub->ownedDoeStudy;
    REQUIRE(study->constraintRecords.size() == 2);
    CHECK(study->constraintRecords[0].get() != study->constraintRecords[1].get());
    engine::FileEffects effects;
    rig.plan(effects);
    REQUIRE(effects.writes.size() == 1);
    const std::string section = "\r\nNUM_CON\t\t2\r\n"
        "  1 200    3      0    5\r\n  2 200    7      0    9\r\n";
    const auto& bytes = effects.writes.front().bytes;
    REQUIRE(bytes.size() >= section.size());
    CHECK(bytes.substr(bytes.size() - section.size()) == section);
}

TEST_CASE(def_doe_constraints_without_category2_start_shared_second_counter_at_one)
{
    Rig rig(row(1081, 2, 4) + row(1010, 3, 6) + row(1081, 5, 8));
    engine::FileEffects effects;
    rig.plan(effects);
    REQUIRE(effects.writes.size() == 1);
    const std::string section = "\r\nNUM_CON\t\t3\r\n"
        "  1 1081    2      0.0    4\r\n"
        "  2 1010    3      0    6\r\n"
        "  3 1081    5      0.0    8\r\n";
    const auto& bytes = effects.writes.front().bytes;
    REQUIRE(bytes.size() >= section.size());
    CHECK(bytes.substr(bytes.size() - section.size()) == section);
}

TEST_CASE(def_doe_constraints_empty_owner_preserves_previous_planned_bytes)
{
    Rig rig("");
    engine::FileEffects effects;
    rig.plan(effects);
    REQUIRE(effects.writes.size() == 1);
    CHECK(effects.writes.front().bytes ==
        "NOMINAL_DB\r\n../../../model.DB\r\nOBJ_NO\r\n0\r\nOPR_NO\r\n0\r\n"
        "DO_NOT_POS\r\nROI_TYPE\r\n0\r\nDOE_DBS\r\n");
}

TEST_CASE(def_doe_constraints_unrepresented_owner_refuses_before_appending_a_plan)
{
    for (int profile = 0; profile < 5; ++profile) {
        Rig rig;
        const auto study = rig.sub->ownedDoeStudy;
        REQUIRE(study->constraintRecords.size() == 5);
        if (profile == 0) ++study->constraints;
        if (profile == 1) study->constraintRecords[2].reset();
        if (profile == 2) study->constraintRecords[2] = std::make_unique<doe::ConstraintRecord>(3, 200, 1, 2);
        if (profile == 3) study->constraints = -1;
        if (profile == 4) study->constraints = 0;
        engine::FileEffects effects;
        effects.writes.push_back({qt::L("prior.file"), "retained prior effect"});
        CHECK(refused([&] { rig.plan(effects); }).find("constraint owner/zero-value profile") != std::string::npos);
        REQUIRE(effects.writes.size() == 1);
        CHECK(effects.writes.front().path == qt::L("prior.file"));
        CHECK(effects.writes.front().bytes == "retained prior effect");
        CHECK(rig.session.project().doeIo->xmlWrites().empty());
    }
}

TEST_CASE(def_doe_constraints_private_plan_does_not_release_actual_save_refresh)
{
    Rig rig;
    engine::FileEffects planned;
    rig.plan(planned);
    REQUIRE(planned.writes.size() == 1);
    auto& p = rig.session.project();
    const auto textWrites = p.doeIo->textWrites().size();
    MinimalViews views;
    Ctx ctx;
    ctx.session = &rig.session;
    engine::FileEffects opened;
    engine::SaveEffects saved;
    CHECK(refused([&] { (void)engine::saveProject(p, ctx, views, opened, saved); })
        .find("main-window +458 is unavailable") != std::string::npos);
    CHECK(p.dom->root->children[1] == rig.oldSub && rig.oldSub->children.empty());
    CHECK(p.doeIo->textWrites().size() == textWrites && saved.project.writes.empty());
    CHECK(rig.session.sessionNotificationStateOwner()->attempts.empty());
    REQUIRE(saved.project.xmlJournal != nullptr);
    REQUIRE(saved.project.xmlJournal->attempts().size() == 1);
    CHECK(saved.project.xmlJournal->attempts().front().normalWriterReturn.has_value());
    CHECK(saved.project.xmlJournal->attempts().front().refreshBoundaryReached);
    CHECK(planned.writes.size() == 1); // Never replay the private plan through public Save.
}

int main() { return testkit::run_all(); }
