// ctest-labels: m2m-doe
// Actual-reader partial state only; the notifier/public Save remain guarded.
#include <testkit/check.hpp>

#include <fstream>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "m2m/common.hpp"
#include "m2m/doe_io.hpp"
#include "m2m/doe_session_notifications.hpp"
#include "m2m/doe_study.hpp"
#include "m2m/doe_variable_record.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine/conversion_session.hpp"
#include "m2m/model.hpp"
#include "m2m/moproj_write.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/qt4path.hpp"
#include "../support/test_paths.hpp"
#include "../../src/doe/keyword_constructor_record.hpp"

using namespace m2m;
namespace {
std::string keyword()
{
    return "<Keyword nominalValue=\"0.1\" unitType=\"0\" name=\"TLOC\" parmType=\"double\" parameter=\"-1\">"
        "<NominalData><string>TLOC    \t0.1</string></NominalData>"
        "<KwOperations><KwOperation id=\"1\"/></KwOperations></Keyword>";
}
std::string frcrotKeyword()
{
    return "<Keyword nominalValue=\"0\" unitType=\"0\" name=\"FRCROT\" parmType=\"function\" parameter=\"-1\" funcEntryNo=\"0\">"
        "<NominalData><string>FRCROT 1 1 0 5</string></NominalData>"
        "<UpperData><string>FRCROT 1 1 0 6</string></UpperData>"
        "<LowerData><string>FRCROT 1 1 0 4</string></LowerData>"
        "<KwOperations/><KwEntities><KwEntity id=\"1\"/></KwEntities></Keyword>";
}
std::string changedFrcrot(const std::string& from, const std::string& to)
{
    auto text = frcrotKeyword();
    const auto at = text.find(from);
    if (at == std::string::npos) throw std::runtime_error("test FRCROT replacement is missing");
    text.replace(at, from.size(), to);
    return text;
}
std::string changed(const std::string& from, const std::string& to)
{
    auto text = keyword();
    const auto at = text.find(from);
    if (at == std::string::npos) throw std::runtime_error("test Keyword replacement is missing");
    text.replace(at, from.size(), to);
    return text;
}
std::string row(const char* name, const char* id, const std::string& body = "",
                const std::string& dimension = " dimension=\"3\"", const std::string& link = "")
{
    return "<DoeVariable name=\"" + std::string(name) + "\" varId=\"" + id +
        "\" numSamples=\"3\"" + dimension + link + ">" + body + "</DoeVariable>";
}
std::string table(const std::string& rows)
{ return "<sfDoeInputTable><DoeVariables>" + rows + "</DoeVariables><Operations/></sfDoeInputTable>"; }
bool readRows(doe::Study& study, const std::string& rows)
{ return doe::readInputTable(table(rows), study); }
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
void constructorOnly(const doe::HeaderVariableRecord& variable)
{
    REQUIRE(variable.items.size() == 1);
    const auto* record = variable.items[0].keyword();
    REQUIRE(record);
    CHECK(record->keywordId == 8 && record->unitType == 0 && record->nominalValue == 0.1);
    CHECK(record->parameterName.isNull());
    CHECK(record->parameter == -1 && record->saveSelector == -1);
    CHECK(record->nominalText.isNull() && record->upperText.isNull() && record->lowerText.isNull());
    CHECK(!record->function && !record->enumeration && !record->lowerDefined && !record->upperDefined);
    CHECK(record->nominalData == nullptr && record->lowerData == nullptr && record->upperData == nullptr);
    CHECK(record->operations == std::vector<int>({1}) && record->entities.empty());
    CHECK(variable.ops == std::vector<int>({1}));
}
std::string retainedInput()
{
    const auto path = m2m_test::configuredPath("M2M_ORACLE_B") /
        "121_B_DOE_VAR" / "DoeStudies" / "DoeStudy1" / "Control" / "DoeInputTable.XML";
    std::ifstream file(path, std::ios::binary);
    if (!file) throw std::runtime_error("retained B121 input is unavailable");
    return {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
}
struct ActiveRig {
    engine::DiagramEffectDriver driver;
    engine::ConversionSession session{driver};
    SubProject* sub = nullptr;
    xml::NodePtr oldSub;
    String control;
    std::size_t seedWrites = 0;
    explicit ActiveRig(const std::string& bytes)
    {
        auto& project = session.project();
        const String folder((m2m_test::scratchRoot() / "doe-keyword-reader-prefix").u16string());
        initProject(project, folder, qt::L("missing"));
        project.path = folder + qt::kSep + qt::L("missing.moproj");
        project.doeIo = doe::makeRecordingStudyIo();
        project.doeIo->bindModelManager(session.modelManagerOwner());
        project.doeIo->bindSessionNotificationState(session.sessionNotificationStateOwner());
        project.subs.push_back(makeSubProject(project, 2, 7));
        sub = project.subs.back().get();
        project.activeSub = 1;
        project.openedOk = true;
        project.dom = std::make_shared<xml::Document>();
        project.dom->root = xml::createElement(qt::L("Project"));
        project.dom->root->children.push_back(xml::createElement(qt::L("Sub_Project")));
        oldSub = xml::createElement(qt::L("Sub_Project"));
        project.dom->root->children.push_back(oldSub);
        control = projectDir(project) + qt::kSep + qt::L("DoeStudies") + qt::kSep +
            qt::L("KeywordPrefixStudy") + qt::kSep + qt::L("Control") + qt::kSep;
        CHECK(project.doeIo->writeTextFile(control + qt::L("DoeInputTable.XML"), bytes));
        CHECK(project.doeIo->writeTextFile(control + qt::L("DEF_DOE.PAR"), "0\r\n1\t4\t\r\n"));
        seedWrites = project.doeIo->textWrites().size();
    }
    void read()
    {
        auto input = xml::createElement(qt::L("Sub_Project"));
        auto study = xml::createElement(qt::L("SfDoeStudy"));
        xml::setAttribute(*study, qt::L("name"), qt::L("KeywordPrefixStudy"));
        input->children.push_back(study);
        Ctx context;
        context.session = &session;
        parseSubProject(session.project(), *input, context);
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

TEST_CASE(keyword_reader_retained_b121_owns_constructor_prefix_before_notifier)
{
    doe::Study study;
    study.samples.invalid.fill(false);
    study.samples.tables[0] = {{99.0}};
    const auto counts = study.samples.counts;
    CHECK(refused([&] { (void)doe::readInputTable(retainedInput(), study); })
        .find("before unresolved SetOprHasDoe") != std::string::npos);
    REQUIRE(study.headerVariableOwners.size() == 1 && study.vars.size() == 1);
    const auto& variable = *study.headerVariableOwners[0];
    CHECK(study.vars[0] == &variable && variable.name == qt::L("Transfer Time"));
    CHECK(variable.number == 1 && variable.index == -1 && variable.dimension == 3);
    CHECK(study.selectedVariableIndex == 0 && !study.retainedScalarPrefix());
    CHECK(study.scalarPrefixVariableOwners.empty() && study.geometryPrefixVariableOwners.empty());
    constructorOnly(variable);
    CHECK(study.samples.invalid[0] && study.samples.invalid[1] && study.samples.invalid[2]);
    CHECK(study.samples.tables[0] == doe::SampleTable({{99.0}}) && study.samples.counts == counts);
}

TEST_CASE(keyword_reader_a_b_a_named_append_differs_from_last_header_without_selection_change)
{
    doe::Study study;
    REQUIRE(readRows(study, row("A", "7", "<Description>A original</Description>") + row("B", "8")));
    auto& a = *study.headerVariableOwners[0];
    auto& b = *study.headerVariableOwners[1];
    a.dimension = 9;
    b.dimension = 11;
    a.index = 70;
    b.index = 71;
    b.link = &a;
    study.samples.invalid.fill(false);
    const auto message = refused([&] {
        (void)readRows(study, row("A", "19", "<Description>B before</Description>" + keyword() +
            "<Description>unreached</Description><Geometry/>", "", " LinkTo=\"B\""));
    });
    CHECK(message.find("before unresolved SetOprHasDoe") != std::string::npos);
    constructorOnly(a);
    CHECK(a.dimension == 3); // Reader-local3, not the named receiver's previous9.
    CHECK(a.number == 7 && a.index == 70 && a.description == qt::L("A original"));
    CHECK(b.dimension == 11 && b.number == 19 && b.index == 71);
    CHECK(b.description == qt::L("B before") && b.items.empty() && b.files.empty());
    CHECK(b.link == &a); // Deferred B->B request never resolves on unwind.
    CHECK(study.selectedVariableIndex == 1); // Name lookup does not select A.
    CHECK(!study.samples.invalid[0] && !study.samples.invalid[1] && !study.samples.invalid[2]);
}

TEST_CASE(keyword_reader_unsupported_profiles_keep_prior_header_description_without_keyword)
{
    const std::vector<std::string> guarded = {
        "<Keyword name=\"TLOC\"/>",
        changed("unitType=\"0\"", "unitType=\"00\""),
        changed("parmType=\"double\"", "parmType=\"function\""),
        changed("parameter=\"-1\"", "parameter=\"0\""),
        changed("nominalValue=\"0.1\"", "nominalValue=\"0.10\""),
        changed("name=\"TLOC\"", "name=\"TLOC\" parameterName=\"\""),
        changed("<NominalData>", "<NominalData><!--first raw node-->"),
        changed("<NominalData>", "<NominalData>nonempty first text"),
        changed("<KwOperation id=\"1\"/>", "<KwOperation id=\"2\"/>"),
        changed("<KwOperation id=\"1\"/>", "<KwOperation id=\"1\"/><KwOperation id=\"1\"/>"),
        changed("</Keyword>", "<KwEntities/></Keyword>"),
        changed("</Keyword>", "<UpperData><string>x</string></UpperData></Keyword>")
    };
    for (const auto& candidate : guarded) {
        doe::Study study;
        CHECK(refused([&] { (void)readRows(study, row("A", "7", "<Description>before</Description>" +
            candidate + "<Description>after</Description>")); }).find("Keyword entry 0x14090f5bf") != std::string::npos);
        REQUIRE(study.headerVariableOwners.size() == 1);
        const auto& record = *study.headerVariableOwners[0];
        CHECK(record.items.empty() && record.ops.empty() && record.description == qt::L("before"));
        CHECK(record.number == 7 && record.index == -1 && record.dimension == 3);
    }
}

TEST_CASE(keyword_reader_retry_and_legacy_target_do_not_replace_existing_owner)
{
    doe::Study study;
    CHECK(refused([&] { (void)readRows(study, row("A", "7", keyword())); })
        .find("before unresolved SetOprHasDoe") != std::string::npos);
    auto& record = *study.headerVariableOwners[0];
    const auto* identity = record.items[0].keyword();
    record.dimension = 17;
    study.samples.invalid.fill(false);
    CHECK(refused([&] { (void)readRows(study, row("A", "9", keyword(), "")); })
        .find("empty canonical keyword list") != std::string::npos);
    CHECK(record.items.size() == 1 && record.items[0].keyword() == identity && record.dimension == 17);
    CHECK(record.number == 9 && record.ops == std::vector<int>({1}));
    CHECK(!study.samples.invalid[0] && !study.samples.invalid[1] && !study.samples.invalid[2]);
    doe::Study legacy;
    REQUIRE(readRows(legacy, row("A", "7")));
    legacy.headerVariableOwners[0]->items.emplace_back(qt::L("upper"), qt::L("lower"));
    CHECK(refused([&] { (void)readRows(legacy, row("A", "9", keyword())); })
        .find("empty canonical keyword list") != std::string::npos);
    CHECK(legacy.headerVariableOwners[0]->items[0].kind() == doe::ItemKind::LegacyValue);
}

TEST_CASE(keyword_reader_actual_lazy_owner_retains_prefix_without_samples_notifier_or_save_release)
{
    ActiveRig rig(table(row("A", "7", "<Description>before</Description>" + keyword() +
        "<Description>after</Description><Geometry/>", " dimension=\"3\"", " LinkTo=\"A\"")));
    const auto study = rig.sub->ownedDoeStudy;
    study->samples.invalid.fill(false);
    study->samples.tables[0] = {{91.0}};
    const auto counts = study->samples.counts;
    CHECK(refused([&] { rig.read(); }).find("before unresolved SetOprHasDoe") != std::string::npos);
    REQUIRE(study->headerVariableOwners.size() == 1);
    const auto& record = *study->headerVariableOwners[0];
    constructorOnly(record);
    CHECK(study == rig.sub->ownedDoeStudy && rig.sub->doe == study.get());
    CHECK(study->name == qt::L("KeywordPrefixStudy") && record.description == qt::L("before"));
    CHECK(record.link == nullptr && record.index == -1 && record.files.empty());
    CHECK(study->samples.invalid[0] && study->samples.invalid[1] && study->samples.invalid[2]);
    CHECK(study->samples.tables[0] == doe::SampleTable({{91.0}}) && study->samples.counts == counts);
    const auto io = rig.session.project().doeIo;
    CHECK(io->reads().size() == 1 && io->reads()[0].path == rig.control + qt::L("DoeInputTable.XML"));
    CHECK(io->textWrites().size() == rig.seedWrites && io->xmlWrites().empty());
    CHECK(rig.session.sessionNotificationStateOwner()->attempts.empty());
    MinimalViews views;
    Ctx context;
    context.session = &rig.session;
    engine::FileEffects opened;
    engine::SaveEffects saved;
    CHECK(refused([&] { (void)engine::saveProject(rig.session.project(), context, views, opened, saved); })
        .find("InputTable variable dimension reconciliation remains unsupported") != std::string::npos);
    CHECK(rig.session.project().dom->root->children[1] == rig.oldSub && rig.oldSub->children.empty());
    CHECK(io->textWrites().size() == rig.seedWrites && io->xmlWrites().empty() && saved.project.writes.empty());
    CHECK(rig.session.sessionNotificationStateOwner()->attempts.empty());
}

TEST_CASE(keyword_reader_actual_type2_frcrot_named_owner_hands_off_values_before_converter_call)
{
    ActiveRig rig(table(row("A", "19", "<Description>B before</Description>" + frcrotKeyword() +
        "<Description>unreached</Description><Geometry/>", "", " LinkTo=\"B\"")));
    const auto study = rig.sub->ownedDoeStudy;
    REQUIRE(readRows(*study, row("A", "7", "<Description>A original</Description>") + row("B", "8")));
    REQUIRE(study->headerVariableOwners.size() == 2);
    auto& a = *study->headerVariableOwners[0];
    auto& b = *study->headerVariableOwners[1];
    a.dimension = 9;
    b.dimension = 11;
    a.index = 70;
    b.index = 71;
    b.link = &a;
    study->samples.invalid.fill(false);
    study->samples.tables[0] = {{91.0}};
    const auto counts = study->samples.counts;
    const auto message = refused([&] { rig.read(); });
    CHECK(message.find("0x14091111d") != std::string::npos);
    CHECK(message.find("M2M_E_UNSUPPORTED") != std::string::npos);
    REQUIRE(a.items.size() == 1);
    const auto* record = a.items[0].keyword();
    REQUIRE(record);
    CHECK(record->keywordId == 276 && record->unitType == 0 && record->nominalValue == 0.0);
    CHECK(record->parameterName.isNull() && record->parameter == -1 && record->saveSelector == -1);
    CHECK(record->function && !record->enumeration);
    CHECK(record->nominalText == qt::L("FRCROT 1 1 0 5"));
    CHECK(record->upperText == qt::L("FRCROT 1 1 0 6"));
    CHECK(record->lowerText == qt::L("FRCROT 1 1 0 4"));
    CHECK(record->operations.empty() && record->entities == std::vector<int>({1}));
    const std::vector<int> invocationSelector{record->entities.front(), record->entities.front()};
    CHECK(invocationSelector == std::vector<int>({1, 1}));
    CHECK(record->nominalData == nullptr && record->upperData == nullptr && record->lowerData == nullptr);
    CHECK(a.ops.empty() && a.dimension == 3 && a.number == 7 && a.index == 70);
    CHECK(a.description == qt::L("A original") && a.link == nullptr && a.files.empty());
    CHECK(b.dimension == 11 && b.number == 19 && b.index == 71 && b.items.empty());
    CHECK(b.description == qt::L("B before") && b.link == &a && b.files.empty());
    CHECK(study->selectedVariableIndex == 1 && study == rig.sub->ownedDoeStudy && rig.sub->doe == study.get());
    CHECK(!study->samples.invalid[0] && !study->samples.invalid[1] && !study->samples.invalid[2]);
    CHECK(study->samples.tables[0] == doe::SampleTable({{91.0}}) && study->samples.counts == counts);
    const auto io = rig.session.project().doeIo;
    CHECK(io->reads().size() == 1 && io->reads()[0].path == rig.control + qt::L("DoeInputTable.XML"));
    CHECK(io->textWrites().size() == rig.seedWrites && io->xmlWrites().empty());
    CHECK(rig.session.sessionNotificationStateOwner()->attempts.empty());
}

TEST_CASE(keyword_reader_actual_type2_frcrot_guards_malformed_profiles_before_constructor)
{
    const std::vector<std::string> guarded = {
        changedFrcrot("name=\"FRCROT\"", "name=\"OTHER\""),
        changedFrcrot("unitType=\"0\"", "unitType=\"00\""),
        changedFrcrot("funcEntryNo=\"0\"", "funcEntryNo=\"1\""),
        changedFrcrot("funcEntryNo=\"0\"", "funcEntryNo=\"0\" parameterName=\"\""),
        changedFrcrot("<NominalData>", "<NominalData><!--first raw node-->"),
        changedFrcrot("<UpperData>", "<UpperData extra=\"1\">"),
        changedFrcrot("FRCROT 1 1 0 4", "FRCROT 1 1 0 4\n"),
        changedFrcrot("FRCROT 1 1 0 5", "FRCROT 1 1 0 5&#233;"),
        changedFrcrot("<KwOperations/>", "<KwOperations><KwOperation id=\"1\"/></KwOperations>"),
        changedFrcrot("<KwOperations/>", "<KwOperations><!--raw child--></KwOperations>"),
        changedFrcrot("</NominalData>", "</NominalData><!--raw Keyword child-->"),
        changedFrcrot("</NominalData>", "</NominalData>raw Keyword text"),
        changedFrcrot("<KwEntities>", "<KwEntities><!--raw entity child-->"),
        changedFrcrot("<KwEntities>", "<KwEntities>raw entity text"),
        changedFrcrot("<KwEntity id=\"1\"/>", "<KwEntity id=\"2\"/>"),
        changedFrcrot("<KwEntity id=\"1\"/>", "<KwEntity id=\"1\"/><KwEntity id=\"2\"/>"),
        changedFrcrot("</Keyword>", "<Other/></Keyword>")
    };
    for (const auto& candidate : guarded) {
        ActiveRig rig(table(row("A", "7", "<Description>before</Description>" + candidate +
            "<Description>after</Description>")));
        const auto study = rig.sub->ownedDoeStudy;
        const auto message = refused([&] { rig.read(); });
        CHECK(message.find("Keyword entry 0x14090f5bf") != std::string::npos);
        REQUIRE(study->headerVariableOwners.size() == 1);
        const auto& variable = *study->headerVariableOwners[0];
        CHECK(variable.items.empty() && variable.ops.empty() && variable.description == qt::L("before"));
        CHECK(rig.session.sessionNotificationStateOwner()->attempts.empty());
        const auto io = rig.session.project().doeIo;
        CHECK(io->textWrites().size() == rig.seedWrites && io->xmlWrites().empty());
    }
}

int main() { return testkit::run_all(); }
