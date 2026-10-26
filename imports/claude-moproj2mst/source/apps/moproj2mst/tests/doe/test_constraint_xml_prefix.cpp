// ctest-labels: m2m-doe
// CP124: actual retained Constraint owners to immediate XML, then refresh guard.
#include <testkit/check.hpp>

#include <algorithm>
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
const std::string rows =
    "<Constraint Id=\"1081\" Value=\"0\" ObjectNo=\"3\" RegionNo=\"7\"/>"
    "<Constraint Id=\"1010\" Value=\"0\" ObjectNo=\"5\" RegionNo=\"8\"/>"
    "<Constraint Id=\"1081\" Value=\"0\" ObjectNo=\"9\" RegionNo=\"2\"/>";
std::string table(const std::string& records)
{
    return "<sfDoeInputTable><DoeCommonMeshPath path=\"mesh\"/>"
        "<DoeVariables/><Constraints>" + records + "</Constraints><Operations/></sfDoeInputTable>";
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
class MinimalViews final : public moproj::DiagramViews {
public:
    void viewCentre(const SubProject&, double& x, double& y) override { x = 1; y = 2; }
    xml::NodePtr dispView(const SubProject&) override { return xml::createElement(qt::L("SfDispView")); }
    bool expanded(const SubProject&, int) override { return true; }
    moproj::RectF rect(const SubProject&, int) override { return {}; }
    std::vector<moproj::ExpansionItem> expansion(const SubProject&, int) override { return {}; }
};
struct Rig {
    engine::DiagramEffectDriver driver;
    engine::ConversionSession session{driver};
    SubProject* sub = nullptr;
    xml::NodePtr oldSub;
    qt::String path;
    std::string original;
    std::size_t writesBeforeSave = 0;
    explicit Rig(doe::RecordingXmlFault fault = {}, const std::string& records = rows)
    {
        auto& p = session.project();
        const auto folder = qt::L("C:/PROBLEM/constraint_xml");
        initProject(p, folder, qt::L("records"));
        p.path = folder + qt::kSep + qt::L("records.moproj");
        const auto control = projectDir(p) + qt::kSep + qt::L("DoeStudies") + qt::kSep +
            qt::L("ConstraintXmlStudy") + qt::kSep + qt::L("Control") + qt::kSep;
        path = control + qt::L("DoeInputTable.XML");
        fault.expectedPath = path;
        fault.expectedAttemptOrdinal = 0;
        p.doeIo = doe::makeRecordingStudyIo({fault});
        p.doeIo->bindModelManager(session.modelManagerOwner());
        p.doeIo->bindSessionNotificationState(session.sessionNotificationStateOwner());
        p.subs.push_back(makeSubProject(p, 2, 7));
        sub = p.subs.back().get();
        p.activeSub = 1; // Actual C++ Session fixture; no C ABI selection claim.
        p.openedOk = true;
        p.dom = std::make_shared<xml::Document>();
        p.dom->root = xml::createElement(qt::L("Project"));
        p.dom->root->children.push_back(xml::createElement(qt::L("Sub_Project")));
        oldSub = xml::createElement(qt::L("Sub_Project"));
        p.dom->root->children.push_back(oldSub);
        original = table(records);
        CHECK(p.doeIo->writeTextFile(path, original));
        CHECK(p.doeIo->writeTextFile(control + qt::L("DEF_DOE.PAR"), "0\r\n1\t4\t\r\n"));
        auto input = xml::createElement(qt::L("Sub_Project"));
        auto study = xml::createElement(qt::L("SfDoeStudy"));
        xml::setAttribute(*study, qt::L("name"), qt::L("ConstraintXmlStudy"));
        input->children.push_back(study);
        Ctx ctx;
        ctx.session = &session;
        parseSubProject(p, *input, ctx); // Real lazy read creates the retained records.
        writesBeforeSave = p.doeIo->textWrites().size();
    }
    std::string save(engine::SaveEffects& saved)
    {
        auto& p = session.project();
        MinimalViews views;
        Ctx ctx;
        ctx.session = &session;
        engine::FileEffects opened;
        return refused([&] { (void)engine::saveProject(p, ctx, views, opened, saved); });
    }
    void checkStopped(const engine::SaveEffects& saved)
    {
        const auto& p = session.project();
        CHECK(p.dom->root->children[1] == oldSub && oldSub->children.empty());
        CHECK(p.doeIo->textWrites().size() == writesBeforeSave);
        CHECK(saved.project.writes.empty());
        CHECK(session.sessionNotificationStateOwner()->attempts.empty());
    }
};
void checkReturnThenBoundary(const doe::StudyXmlJournal& journal)
{
    const auto& events = journal.events();
    REQUIRE(events.size() >= 2);
    CHECK(events[events.size() - 2].kind == doe::XmlJournalEventKind::NormalWriterReturn);
    CHECK(events.back().kind == doe::XmlJournalEventKind::RefreshBoundaryReached);
    CHECK(events.back().attempt == 0 && events[events.size() - 2].attempt == 0);
}
}

TEST_CASE(constraint_xml_actual_lazy_owner_writes_ordered_rows_before_refresh_refusal)
{
    Rig rig;
    auto& p = rig.session.project();
    const auto study = rig.sub->ownedDoeStudy;
    REQUIRE(study && study->constraintRecords.size() == 3);
    const auto* first = study->constraintRecords[0].get();
    const auto* duplicate = study->constraintRecords[2].get();
    const auto tables = study->samples.tables;
    const auto invalid = study->samples.invalid;
    engine::SaveEffects saved;
    CHECK(rig.save(saved).find("main-window +458 is unavailable") != std::string::npos);
    rig.checkStopped(saved);
    CHECK(rig.sub->ownedDoeStudy == study && study->constraintRecords[0].get() == first);
    CHECK(study->constraintRecords[2].get() == duplicate && first != duplicate);
    CHECK(study->samples.tables == tables && study->samples.invalid == invalid);
    REQUIRE(saved.project.xmlJournal != nullptr);
    const auto& journal = *saved.project.xmlJournal;
    REQUIRE(journal.attempts().size() == 1);
    const auto& attempt = journal.attempts().front();
    CHECK(attempt.path == rig.path && attempt.opened && attempt.closed && attempt.errorAfterClose == 0);
    REQUIRE(attempt.normalWriterReturn.has_value());
    CHECK(attempt.normalWriterReturn->writerSucceeded && !attempt.normalWriterReturn->streamFailed);
    CHECK(attempt.refreshBoundaryReached);
    checkReturnThenBoundary(journal);
    const auto bytes = journal.recordedFile(rig.path);
    REQUIRE(bytes.has_value());
    CHECK(attempt.recordedBytesAfterClose == bytes);
    std::string accepted;
    for (const auto& span : attempt.recordingRawSpans) accepted += span.acceptedRecordingBytes;
    CHECK(accepted == *bytes); // Actual memory-file mutation, never native durability.
    xml::Document doc;
    REQUIRE(xml::setContent(doc, *bytes, nullptr, nullptr, nullptr));
    REQUIRE(doc.root != nullptr);
    const auto children = doc.root->childElements();
    REQUIRE(children.size() == 7);
    CHECK(children[4]->tag == qt::L("DoeVariables"));
    CHECK(children[5]->tag == qt::L("Operations"));
    CHECK(children[6]->tag == qt::L("Constraints"));
    const auto records = children[6]->childElements(qt::L("Constraint"));
    REQUIRE(records.size() == 3);
    const std::vector<int> ids{1081, 1010, 1081}, objects{3, 5, 9}, regions{7, 8, 2};
    for (std::size_t i = 0; i < records.size(); ++i) {
        CHECK(records[i]->attrs.size() == 4);
        CHECK(records[i]->attribute(qt::L("Id")) == qt::number(ids[i]));
        CHECK(records[i]->attribute(qt::L("Value")) == qt::L("0"));
        CHECK(records[i]->attribute(qt::L("ObjectNo")) == qt::number(objects[i]));
        CHECK(records[i]->attribute(qt::L("RegionNo")) == qt::number(regions[i]));
        CHECK(!study->constraintRecords[i]->sourceField10Bits);
    }
}

TEST_CASE(constraint_xml_open_false_keeps_prior_bytes_and_reaches_same_refresh_guard)
{
    doe::RecordingXmlFault fault;
    fault.opened = false;
    fault.label = "CP124 explicit open failure";
    Rig rig(fault);
    engine::SaveEffects saved;
    CHECK(rig.save(saved).find("main-window +458 is unavailable") != std::string::npos);
    rig.checkStopped(saved);
    REQUIRE(saved.project.xmlJournal != nullptr);
    const auto& journal = *saved.project.xmlJournal;
    REQUIRE(journal.attempts().size() == 1);
    const auto& attempt = journal.attempts().front();
    CHECK(!attempt.opened && !attempt.errorAfterClose && attempt.requestedStreamBytes.empty());
    REQUIRE(attempt.normalWriterReturn.has_value());
    CHECK(!attempt.normalWriterReturn->writerSucceeded && attempt.refreshBoundaryReached);
    CHECK(journal.recordedFile(rig.path) == rig.original);
    checkReturnThenBoundary(journal);
}

TEST_CASE(constraint_xml_postclose_false_retains_accepted_xml_and_reaches_refresh_guard)
{
    doe::RecordingXmlFault fault;
    fault.errorAfterClose = 7;
    fault.label = "CP124 explicit post-close failure";
    Rig rig(fault);
    engine::SaveEffects saved;
    CHECK(rig.save(saved).find("main-window +458 is unavailable") != std::string::npos);
    rig.checkStopped(saved);
    REQUIRE(saved.project.xmlJournal != nullptr);
    const auto& journal = *saved.project.xmlJournal;
    REQUIRE(journal.attempts().size() == 1);
    const auto& attempt = journal.attempts().front();
    CHECK(attempt.opened && attempt.closed && attempt.errorAfterClose == 7);
    REQUIRE(attempt.normalWriterReturn.has_value());
    CHECK(!attempt.normalWriterReturn->writerSucceeded && !attempt.normalWriterReturn->streamFailed);
    CHECK(attempt.normalWriterReturn->stickyLastError ==
        qt::arg(qt::L("Error writing file, %1"), rig.path));
    REQUIRE(attempt.recordedBytesAfterClose.has_value());
    CHECK(attempt.recordedBytesAfterClose->find("<Constraints>") != std::string::npos);
    CHECK(journal.recordedFile(rig.path) == attempt.recordedBytesAfterClose);
    CHECK(attempt.refreshBoundaryReached);
    checkReturnThenBoundary(journal);
}

TEST_CASE(constraint_xml_incoherent_or_null_owner_refuses_after_open_before_body)
{
    for (int profile = 0; profile < 3; ++profile) {
        Rig rig;
        const auto study = rig.sub->ownedDoeStudy;
        if (profile == 0) study->constraints = 2;
        if (profile == 1) study->constraintRecords[1].reset();
        if (profile == 2) study->constraints = -1;
        engine::SaveEffects saved;
        CHECK(rig.save(saved).find("constraint owner remains unsupported after XML open") != std::string::npos);
        rig.checkStopped(saved);
        REQUIRE(saved.project.xmlJournal != nullptr);
        const auto& journal = *saved.project.xmlJournal;
        REQUIRE(journal.attempts().size() == 1);
        const auto& attempt = journal.attempts().front();
        CHECK(attempt.opened && attempt.closed && attempt.requestedStreamBytes.empty());
        CHECK(!attempt.errorAfterClose && !attempt.normalWriterReturn && !attempt.refreshBoundaryReached);
        CHECK(journal.recordedFile(rig.path) == "");
    }
}

TEST_CASE(constraint_xml_empty_owner_keeps_existing_empty_xml_bytes)
{
    Rig rig({}, "");
    engine::SaveEffects saved;
    CHECK(rig.save(saved).find("main-window +458 is unavailable") != std::string::npos);
    rig.checkStopped(saved);
    REQUIRE(saved.project.xmlJournal != nullptr);
    const auto bytes = saved.project.xmlJournal->recordedFile(rig.path);
    REQUIRE(bytes.has_value());
    CHECK(*bytes == "<!DOCTYPE DoeInputTableFile>\r\n"
        "<sfDoeInputTable Version=\"1\">\r\n"
        " <DoeCommonMeshPath path=\"mesh\"/>\r\n"
        " <DoeCommonMeshOrigName originalName=\"\"/>\r\n"
        " <DoeDoPositioning bool=\"false\"/>\r\n"
        " <DoeRoiType type=\"0\"/>\r\n"
        " <DoeVariables/>\r\n"
        " <Operations/>\r\n"
        "</sfDoeInputTable>\r\n");
}

int main() { return testkit::run_all(); }
