// ctest-labels: m2m-doe
// Actual per-call notifier entry only; no key/map/receiver or parser completion.
#include <testkit/check.hpp>

#include <memory>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "m2m/common.hpp"
#include "m2m/doe_activation.hpp"
#include "m2m/doe_io.hpp"
#include "m2m/doe_session_notifications.hpp"
#include "m2m/doe_study.hpp"
#include "m2m/doe_variable_record.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine/conversion_session.hpp"
#include "m2m/model.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/qt4path.hpp"
#include "../support/test_paths.hpp"
#include "../../src/doe/input_table_reader.hpp"
#include "../../src/doe/keyword_constructor_record.hpp"
#include "../../src/doe/variable_notify.hpp"

using namespace m2m;
namespace {
const std::string kKeyword =
    "<Keyword nominalValue=\"0.1\" unitType=\"0\" name=\"TLOC\" parmType=\"double\" parameter=\"-1\">"
    "<NominalData><string>TLOC    \t0.1</string></NominalData>"
    "<KwOperations><KwOperation id=\"1\"/></KwOperations></Keyword>";
const std::string kEmptyOperationsKeyword =
    "<Keyword nominalValue=\"0.1\" unitType=\"0\" name=\"TLOC\" parmType=\"double\" parameter=\"-1\">"
    "<NominalData><string>TLOC    \t0.1</string></NominalData>"
    "<KwOperations/></Keyword>";
std::string row(const char* name, const char* id, const std::string& body = "")
{
    return "<DoeVariable name=\"" + std::string(name) + "\" varId=\"" + id +
        "\" numSamples=\"3\" dimension=\"3\">" + body + "</DoeVariable>";
}
std::string table(const std::string& rows)
{ return "<sfDoeInputTable><DoeVariables>" + rows + "</DoeVariables><Operations/></sfDoeInputTable>"; }
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
    engine::ConversionSession* routedSession = nullptr;
    std::string bytes;
    String studies, control;
    std::size_t seedWrites = 0;
    explicit Rig(std::string rows = row("A", "7", kKeyword)) : bytes(table(rows))
    {
        auto& project = session.project();
        const String folder((m2m_test::scratchRoot() / "doe-keyword-notifier-entry").u16string());
        initProject(project, folder, qt::L("missing"));
        project.path = folder + qt::kSep + qt::L("missing.moproj");
        project.doeIo = doe::makeRecordingStudyIo();
        project.doeIo->bindModelManager(session.modelManagerOwner());
        project.doeIo->bindSessionNotificationState(session.sessionNotificationStateOwner());
        project.subs.push_back(makeSubProject(project, 2, 7));
        sub = project.subs.back().get();
        project.activeSub = 1;
        studies = projectDir(project) + qt::kSep + qt::L("DoeStudies");
        control = studies + qt::kSep + qt::L("NotifierStudy") + qt::kSep + qt::L("Control") + qt::kSep;
        CHECK(project.doeIo->writeTextFile(control + qt::L("DoeInputTable.XML"), bytes));
        CHECK(project.doeIo->writeTextFile(control + qt::L("DEF_DOE.PAR"), "0\r\n1\t8\t\r\n"));
        seedWrites = project.doeIo->textWrites().size();
    }
    xml::NodePtr element()
    {
        auto element = xml::createElement(qt::L("SfDoeStudy"));
        xml::setAttribute(*element, qt::L("name"), qt::L("NotifierStudy"));
        return element;
    }
    void read(engine::ConversionSession* actualSession, bool hasContext = true)
    {
        auto input = xml::createElement(qt::L("Sub_Project"));
        input->children.push_back(element());
        Ctx context;
        context.session = hasContext ? actualSession : nullptr;
        routedSession = context.session;
        parseSubProject(session.project(), *input, context);
    }
    doe::InputTableNotifierContext context()
    {
        auto& project = session.project();
        return {&session, &project, sub, sub->ownedDoeStudy.get(), project.doeIo.get(), project.doeIo};
    }
};
void constructorOnly(const doe::HeaderVariableRecord& variable)
{
    REQUIRE(variable.items.size() == 1);
    const auto* record = variable.items[0].keyword();
    REQUIRE(record);
    CHECK(record->keywordId == 8 && record->operations == std::vector<int>({1}));
    CHECK(record->nominalText.isNull() && record->upperText.isNull() && record->lowerText.isNull());
    CHECK(record->parameter == -1 && record->saveSelector == -1 && !record->function && !record->enumeration);
    CHECK(record->nominalData == nullptr && record->upperData == nullptr && record->lowerData == nullptr);
}
}

using EntryFunction = doe::SetOprHasDoeEntryOutcome (*)(
    const doe::InputTableNotifierContext&, doe::Study&, int, bool);
static_assert(std::is_same_v<decltype(&doe::enterSetOprHasDoeNotifierPrefix), EntryFunction>);

TEST_CASE(keyword_notifier_wired_unsuppressed_entry_clears_actual_flag_before_key_guard)
{
    Rig rig(row("A", "7") + row("B", "8") + row("A", "19",
        "<Description>B before</Description>" + kKeyword + "<Description>after</Description>"));
    const auto study = rig.sub->ownedDoeStudy;
    study->samples.invalid.fill(false);
    study->samples.tables[0] = {{91.0}};
    const auto counts = study->samples.counts;
    const auto owner = rig.session.sessionNotificationStateOwner();
    owner->flag90 = true; // Explicit test input, not a claim of a native producer.
    owner->emptyKeys.insert("sentinel");
    const auto keys = owner->emptyKeys;
    const auto attempts = owner->attempts.size();
    CHECK(refused([&] { rig.read(&rig.session); }).find("key construction") != std::string::npos);
    CHECK(!owner->flag90 && !owner->suppressed);
    CHECK(owner->emptyKeys == keys && owner->attempts.size() == attempts);
    REQUIRE(study->headerVariableOwners.size() == 2);
    constructorOnly(*study->headerVariableOwners[0]);
    CHECK(study->headerVariableOwners[1]->items.empty());
    CHECK(study->headerVariableOwners[1]->description == qt::L("B before"));
    CHECK(study->headerVariableOwners[1]->number == 19 && study->selectedVariableIndex == 1);
    CHECK(study->samples.invalid[0] && study->samples.invalid[1] && study->samples.invalid[2]);
    CHECK(study->samples.tables[0] == doe::SampleTable({{91.0}}) && study->samples.counts == counts);
    const auto io = rig.session.project().doeIo;
    CHECK(io->reads().size() == 1 && io->reads()[0].path == rig.control + qt::L("DoeInputTable.XML"));
    CHECK(io->textWrites().size() == rig.seedWrites && io->xmlWrites().empty());
    engine::FileEffects effects;
    CHECK(refused([&] { (void)doe::saveInputTable(*study, rig.studies, effects); })
        .find("0x1400d2b60") != std::string::npos);
    CHECK(effects.writes.empty() && !owner->flag90);
}

TEST_CASE(keyword_empty_operations_actual_session_route_commits_named_scalar_then_stops)
{
    // The second row links back to A. The third row is a duplicate A:
    // last-record header/Description setters therefore target B, while the
    // Keyword's original-name lookup targets A.
    Rig rig(row("A", "7") +
        "<DoeVariable name=\"B\" varId=\"8\" numSamples=\"3\" dimension=\"3\" LinkTo=\"A\">"
        "<Description>B before</Description></DoeVariable>" +
        row("A", "19", kEmptyOperationsKeyword +
            "<Description>unreached later Description</Description>"));
    const auto study = rig.sub->ownedDoeStudy;
    REQUIRE(study);
    const auto* const studyIdentity = study.get();
    const auto owner = rig.session.sessionNotificationStateOwner();
    const auto* const ownerIdentity = owner.get();
    owner->suppressed = false;
    owner->flag90 = true; // Deliberately detects accidental operation-1 notifier entry.
    owner->emptyKeys.insert("sentinel");
    const auto keys = owner->emptyKeys;
    const auto attempts = owner->attempts.size();
    const auto io = rig.session.project().doeIo;
    const auto* const ioIdentity = io.get();
    const auto readsBefore = io->reads().size();
    const auto writesBefore = io->textWrites().size();
    const auto xmlWritesBefore = io->xmlWrites().size();
    study->samples.invalid.fill(false);
    study->samples.tables[0] = {{91.0, 92.0}};
    study->samples.tables[1] = {{81.0}};
    study->samples.tables[2] = {{71.0, 72.0, 73.0}};
    study->samples.counts = {4, 5, 6};
    const auto sampleTables = study->samples.tables;
    const auto sampleCounts = study->samples.counts;

    const auto diagnostic = refused([&] { rig.read(&rig.session); });
    CHECK(diagnostic.find("0x140910abf") != std::string::npos);
    CHECK(diagnostic.find("before upper-text probe") != std::string::npos);
    CHECK(rig.routedSession == &rig.session);
    CHECK(study.get() == studyIdentity);
    CHECK(rig.session.sessionNotificationStateOwner().get() == ownerIdentity);
    CHECK(rig.session.project().doeIo.get() == ioIdentity);
    CHECK(!owner->suppressed && owner->flag90);
    CHECK(owner->emptyKeys == keys && owner->attempts.size() == attempts);

    REQUIRE(study->headerVariableOwners.size() == 2);
    const auto* a = study->headerVariableOwners[0].get();
    const auto* b = study->headerVariableOwners[1].get();
    REQUIRE(a && b);
    CHECK(study->vars[0] == a && study->vars[1] == b);
    CHECK(a->name == qt::L("A") && a->number == 7);
    CHECK(b->name == qt::L("B") && b->number == 19);
    CHECK(b->description == qt::L("B before"));
    CHECK_EQ(study->selectedVariableIndex, 1);
    CHECK(a->items.size() == 1 && b->items.empty());
    const auto* record = a->items[0].keyword();
    REQUIRE(record);
    CHECK_EQ(record->keywordId, 8);
    CHECK(record->operations.empty());
    CHECK(record->nominalText == qt::L("TLOC    \t0.1"));
    CHECK(record->parameter == -1 && !record->function && !record->enumeration);
    CHECK(b->link == nullptr); // LinkTo was queued; deferred resolution is after the guard.
    CHECK(study->samples.invalid[0] && study->samples.invalid[1] && study->samples.invalid[2]);
    CHECK(study->samples.tables == sampleTables && study->samples.counts == sampleCounts);

    REQUIRE(io->reads().size() == readsBefore + 1);
    CHECK(io->reads().back().path == rig.control + qt::L("DoeInputTable.XML"));
    CHECK(io->textWrites().size() == writesBefore && io->xmlWrites().size() == xmlWritesBefore);
    CHECK(io->textWrites().size() == rig.seedWrites);
}

TEST_CASE(keyword_notifier_wired_suppression_preserves_flag_and_stops_before_named_lookup)
{
    Rig rig;
    const auto study = rig.sub->ownedDoeStudy;
    const auto owner = rig.session.sessionNotificationStateOwner();
    owner->suppressed = true; // Explicit profile; no production suppression producer inferred.
    owner->flag90 = true;
    const auto keys = owner->emptyKeys;
    const auto attempts = owner->attempts.size();
    CHECK(refused([&] { rig.read(&rig.session); })
        .find("post-notifier named keyword lookup") != std::string::npos);
    CHECK(owner->suppressed && owner->flag90);
    CHECK(owner->emptyKeys == keys && owner->attempts.size() == attempts);
    REQUIRE(study->headerVariableOwners.size() == 1);
    constructorOnly(*study->headerVariableOwners[0]);
    CHECK(study->headerVariableOwners[0]->index == -1 && study->selectedVariableIndex == 0);
    CHECK(rig.session.project().doeIo->reads().size() == 1);
    CHECK(rig.session.project().doeIo->textWrites().size() == rig.seedWrites);
}

TEST_CASE(keyword_notifier_rejects_io_different_from_input_table_reader)
{
    Rig rig;
    auto& project = rig.session.project();
    const auto study = rig.sub->ownedDoeStudy;
    const auto owner = rig.session.sessionNotificationStateOwner();
    auto foreignReaderIo = doe::makeRecordingStudyIo();
    auto source = doe::InputTableSource{rig.control + qt::L("DoeInputTable.XML"), rig.context(), foreignReaderIo.get()};
    owner->flag90 = true;
    const auto attempts = owner->attempts.size();
    const auto message = refused([&] {
        (void)doe::readInputTableFromSource(rig.bytes, *study, std::move(source));
    });
    CHECK(message.find("notifier StudyIo differs from the I/O that read InputTable bytes") != std::string::npos);
    CHECK(owner->flag90 && owner->attempts.size() == attempts);
    REQUIRE(study->headerVariableOwners.size() == 1);
    constructorOnly(*study->headerVariableOwners[0]);
    CHECK(project.doeIo->reads().empty()); // Direct source fixture bypasses lazy file I/O.
}

TEST_CASE(keyword_notifier_source_less_and_legacy_lazy_calls_keep_previous_boundary)
{
    for (int route = 0; route < 3; ++route) {
        Rig rig;
        const auto study = rig.sub->ownedDoeStudy;
        const auto owner = rig.session.sessionNotificationStateOwner();
        owner->flag90 = true;
        const auto message = refused([&] {
            if (route == 0) (void)doe::readInputTable(rig.bytes, *study);
            else if (route == 1)
                doe::readLazyStudy(*study, *rig.element(), rig.studies, *rig.session.project().doeIo);
            else rig.read(nullptr, false);
        });
        CHECK(message.find("notifier invocation") != std::string::npos);
        CHECK(owner->flag90 && owner->attempts.empty());
        REQUIRE(study->headerVariableOwners.size() == 1);
        constructorOnly(*study->headerVariableOwners[0]);
        CHECK(rig.session.project().doeIo->reads().size() == (route == 0 ? 0u : 1u));
        CHECK(rig.session.project().doeIo->textWrites().size() == rig.seedWrites);
    }
}

TEST_CASE(keyword_notifier_wrong_session_guard_retains_actual_constructor_prefix)
{
    Rig rig;
    engine::DiagramEffectDriver otherDriver;
    engine::ConversionSession other(otherDriver);
    const auto owner = rig.session.sessionNotificationStateOwner();
    const auto foreign = other.sessionNotificationStateOwner();
    owner->flag90 = true;
    foreign->flag90 = true;
    CHECK(refused([&] { rig.read(&other); }).find("notifier entry") != std::string::npos);
    CHECK(owner->flag90 && foreign->flag90);
    const auto study = rig.sub->ownedDoeStudy;
    REQUIRE(study->headerVariableOwners.size() == 1);
    constructorOnly(*study->headerVariableOwners[0]);
    CHECK(study->samples.invalid[0] && study->samples.invalid[1] && study->samples.invalid[2]);
    CHECK(rig.session.project().doeIo->reads().size() == 1);
}

TEST_CASE(keyword_notifier_context_identity_refusals_preserve_flag_and_committed_owner)
{
    for (int profile = 0; profile < 9; ++profile) {
        Rig rig;
        auto& project = rig.session.project();
        const auto study = rig.sub->ownedDoeStudy;
        CHECK(!refused([&] { (void)doe::readInputTable(rig.bytes, *study); }).empty());
        auto context = rig.context();
        const auto owner = rig.session.sessionNotificationStateOwner();
        const auto oldIo = project.doeIo;
        owner->flag90 = true;
        const auto keys = owner->emptyKeys;
        const auto attempts = owner->attempts.size();
        switch (profile) {
        case 0: context.project = nullptr; break;
        case 1: context.sub = nullptr; break;
        case 2: context.study = nullptr; break;
        case 3: context.io = nullptr; break;
        case 4:
            project.doeIo = doe::makeRecordingStudyIo();
            project.doeIo->bindModelManager(rig.session.modelManagerOwner());
            project.doeIo->bindSessionNotificationState(owner);
            break;
        case 5:
            // A bound I/O cannot replace its notifier. Construct this negative
            // I/O with a foreign owner initially, then test the entry check.
            project.doeIo = doe::makeRecordingStudyIo();
            project.doeIo->bindModelManager(rig.session.modelManagerOwner());
            project.doeIo->bindSessionNotificationState(std::make_shared<doe::SessionNotificationState>());
            context.io = project.doeIo.get();
            context.ioOwner = project.doeIo;
            break;
        case 6: context.ioOwner.reset(); break;
        case 7: project.subs.erase(project.subs.begin() + 1); break;
        case 8: break; // Wrong bool argument below: int operation must precede true.
        }
        if (profile == 4 || profile == 5) CHECK(oldIo != project.doeIo);
        CHECK(refused([&] {
            (void)doe::enterSetOprHasDoeNotifierPrefix(context, *study, 1, profile != 8);
        }).find("notifier entry") != std::string::npos);
        CHECK(owner->flag90 && owner->emptyKeys == keys && owner->attempts.size() == attempts);
        REQUIRE(study->headerVariableOwners.size() == 1);
        constructorOnly(*study->headerVariableOwners[0]);
        // Removed SUB and I/O aliases are compared before dereference. The
        // retained Study/old-I/O leases do not make that context dispatchable.
    }
}

int main() { return testkit::run_all(); }
