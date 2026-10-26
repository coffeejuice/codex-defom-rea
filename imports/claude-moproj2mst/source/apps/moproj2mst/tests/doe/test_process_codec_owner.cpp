// ctest-labels: m2m-doe
// Inert process identity only; no codec/source/data or native cleanup admission.
#include <testkit/check.hpp>

#include <memory>
#include <optional>
#include <string>
#include <type_traits>

#include "m2m/common.hpp"
#include "m2m/convert_details.hpp"
#include "m2m/doe_io.hpp"
#include "m2m/doe_qstring_resource_owner.hpp"
#include "m2m/doe_session_notifications.hpp"
#include "m2m/doe_study.hpp"
#include "m2m/engine/conversion_session.hpp"
#include "m2m/engine/process_codec_owner.hpp"
#include "m2m/model.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/qt4path.hpp"
#include "../support/test_paths.hpp"
#include "../../src/doe/input_table_reader.hpp"
#include "../../src/core/process_codec_session.hpp"

using namespace m2m;
namespace {
using Knowledge = doe::QStringCStringCodecKnowledge;
constexpr engine::ProcessCodecOwner kCompileTimeOwner;
constexpr auto kDefaultFacts = kCompileTimeOwner.snapshot();
static_assert(kDefaultFacts.knowledge == Knowledge::Unknown);
static_assert(!kDefaultFacts.moduleGeneration && !kDefaultFacts.cStringsPointer &&
              !kDefaultFacts.codecObjectGeneration);
static_assert(std::is_nothrow_default_constructible_v<engine::ProcessCodecOwner>);
static_assert(std::is_trivially_destructible_v<engine::ProcessCodecOwner>);
static_assert(!std::is_copy_constructible_v<engine::ProcessCodecOwner>);
static_assert(!std::is_move_constructible_v<engine::ProcessCodecOwner>);

void unknown(const engine::ProcessCodecOwner& owner)
{
    const auto state = owner.snapshot();
    CHECK(state.knowledge == Knowledge::Unknown);
    CHECK(!state.moduleGeneration && !state.cStringsPointer && !state.codecObjectGeneration);
    CHECK(owner.sourceBoundary() == engine::ProcessCodecSourceBoundary::CodecKnowledgeUnavailable);
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
}

TEST_CASE(process_codec_owner_default_is_constant_evaluable_unknown_without_a_data_owner)
{
    engine::ProcessCodecOwner owner;
    unknown(owner);
    CHECK_EQ(engine::ProcessCodecOwner::cStringsSymbolRva, 0x2f3e10u);
    CHECK_EQ(std::string(engine::ProcessCodecOwner::qtCoreModuleSha256),
             std::string(doe::kQStringPathQtCoreSha256));
    // Mutable diagnostics are copies, never a source setter or a live read token.
    auto diagnostic = owner.snapshot();
    diagnostic.knowledge = Knowledge::Null;
    diagnostic.cStringsPointer = 0;
    CHECK(diagnostic.knowledge == Knowledge::Null);
    unknown(owner);
}

TEST_CASE(process_codec_owner_sessions_share_only_explicit_process_identity)
{
    engine::ProcessCodecOwner firstProcess, secondProcess;
    engine::DiagramEffectDriver firstDriver, secondDriver, thirdDriver, legacyDriver;
    engine::ConversionSession first(firstDriver, firstProcess);
    engine::ConversionSession same(secondDriver, firstProcess);
    engine::ConversionSession other(thirdDriver, secondProcess);
    engine::ConversionSession legacy(legacyDriver);
    CHECK(first.processCodecOwner() == &firstProcess);
    CHECK(same.processCodecOwner() == first.processCodecOwner());
    CHECK(other.processCodecOwner() == &secondProcess && other.processCodecOwner() != first.processCodecOwner());
    CHECK(legacy.processCodecOwner() == nullptr);
    CHECK(first.sessionNotificationStateOwner() != same.sessionNotificationStateOwner());
    doe::InputTableNotifierContext context;
    context.session = &first; // Existing synchronous context borrows the same Session.
    CHECK(context.session->processCodecOwner() == &firstProcess);
    unknown(firstProcess);
    unknown(secondProcess);
}

TEST_CASE(process_codec_owner_actual_run_construction_seam_keeps_legacy_modes_unbound)
{
    engine::ProcessCodecOwner process;
    engine::DiagramEffectDriver driver;
    const unsigned profiles[] = {0, M2M_FLAG_WRITE_MDT, M2M_FLAG_STRICT,
        M2M_FLAG_WRITE_SAVE, M2M_FLAG_WRITE_SAVE | M2M_FLAG_WRITE_MDT | M2M_FLAG_STRICT};
    for (const unsigned flags : profiles) {
        std::optional<engine::ConversionSession> storage;
        auto& session = detail::emplaceProcessCodecSession(storage, driver, process, flags);
        CHECK(storage.has_value());
        CHECK(session.processCodecOwner() == ((flags & M2M_FLAG_WRITE_SAVE) ? &process : nullptr));
        unknown(process);
        // This is the same internal construction policy used by run, not a
        // public/native observation hook or a new process-codec producer.
    }
}

TEST_CASE(process_codec_owner_survives_project_storage_release_and_session_destruction)
{
    engine::ProcessCodecOwner process;
    engine::DiagramEffectDriver driver;
    {
        engine::ConversionSession session(driver, process);
        REQUIRE(session.liveProject());
        // Explicit adapter storage release on a fresh unbound diagram profile;
        // this is not a claim that original normal Close callbacks completed.
        session.releaseProjectAfterCloseEffects();
        CHECK(session.liveProject() == nullptr);
        CHECK(session.processCodecOwner() == &process);
        unknown(process);
    }
    unknown(process); // Session disposal does not revoke/reset the outer owner.
    engine::ConversionSession next(driver, process);
    CHECK(next.processCodecOwner() == &process);
    unknown(process);
}

TEST_CASE(process_codec_owner_encloses_exception_unwind_and_details_publication)
{
    engine::ProcessCodecOwner process; // Same lexical ordering as the real run.
    engine::DiagramEffectDriver driver;
    ConversionDetails details;
    const engine::ProcessCodecOwner* observed = nullptr;
    bool caught = false;
    try {
        engine::ConversionSession session(driver, process);
        observed = session.processCodecOwner();
        throw Error(M2M_E_UNSUPPORTED, "test source boundary");
    } catch (const Error& error) {
        caught = true;
        CHECK(observed == &process); // Owner is live after Session teardown.
        unknown(process);
        details.returnedCode = error.code;
    }
    CHECK(caught && details.returnedCode == M2M_E_UNSUPPORTED);
    CHECK(!details.xmlJournal && !details.ioMode && !details.saveCall);
    unknown(process); // Still live after the caller-owned details publication.
}

TEST_CASE(process_codec_owner_real_detailed_argument_error_keeps_status_and_output_surface)
{
    ConversionDetails details;
    details.returnedCode = 999;
    const auto result = convertDetailed(u"", u"", 0, 1252, details);
    CHECK_EQ(result.code, M2M_E_ARG);
    CHECK_EQ(result.message, std::string("no project path"));
    CHECK(details.returnedCode == result.code);
    CHECK(!details.xmlJournal && !details.ioMode && !details.saveCall);
}

TEST_CASE(process_codec_owner_isolated_null_cannot_admit_the_actual_notifier_key)
{
    engine::ProcessCodecOwner process;
    engine::DiagramEffectDriver driver;
    engine::ConversionSession session(driver, process);
    doe::QStringResourceProcessScope isolated;
    isolated.setIsolatedCodecProfile(Knowledge::Null, "explicit isolated test profile only");
    CHECK(isolated.codec().knowledge == Knowledge::Null);
    unknown(process);
    auto& project = session.project();
    const String folder((m2m_test::scratchRoot() / "doe-process-codec-owner").u16string());
    initProject(project, folder, qt::L("missing"));
    project.path = folder + qt::kSep + qt::L("missing.moproj");
    project.doeIo = doe::makeRecordingStudyIo();
    project.doeIo->bindModelManager(session.modelManagerOwner());
    project.doeIo->bindSessionNotificationState(session.sessionNotificationStateOwner());
    project.subs.push_back(makeSubProject(project, 2, 7));
    auto* sub = project.subs.back().get();
    project.activeSub = 1;
    const auto control = projectDir(project) + qt::kSep + qt::L("DoeStudies") + qt::kSep +
        qt::L("ProcessStudy") + qt::kSep + qt::L("Control") + qt::kSep;
    CHECK(project.doeIo->writeTextFile(control + qt::L("DoeInputTable.XML"),
        "<sfDoeInputTable><DoeVariables><DoeVariable name=\"A\" varId=\"7\" dimension=\"3\" numSamples=\"3\">"
        "<Keyword nominalValue=\"0.1\" unitType=\"0\" name=\"TLOC\" parmType=\"double\" parameter=\"-1\">"
        "<NominalData><string>TLOC    \t0.1</string></NominalData>"
        "<KwOperations><KwOperation id=\"1\"/></KwOperations></Keyword>"
        "</DoeVariable></DoeVariables></sfDoeInputTable>"));
    const auto writes = project.doeIo->textWrites().size();
    const auto notifier = session.sessionNotificationStateOwner();
    notifier->flag90 = true;
    const auto keys = notifier->emptyKeys;
    const auto attempts = notifier->attempts.size();
    auto input = xml::createElement(qt::L("Sub_Project"));
    auto study = xml::createElement(qt::L("SfDoeStudy"));
    xml::setAttribute(*study, qt::L("name"), qt::L("ProcessStudy"));
    input->children.push_back(study);
    Ctx context;
    context.session = &session;
    CHECK(refused([&] { parseSubProject(project, *input, context); })
        .find("key construction") != std::string::npos);
    CHECK(context.session->processCodecOwner() == &process);
    unknown(process);
    CHECK(!notifier->flag90 && notifier->attempts.size() == attempts && notifier->emptyKeys == keys);
    CHECK(sub->ownedDoeStudy->vars.size() == 1 && sub->ownedDoeStudy->vars[0]->items.size() == 1);
    CHECK(project.doeIo->reads().size() == 1 && project.doeIo->textWrites().size() == writes);
    doe::QStringPathLease destination;
    CHECK(!refused([&] { isolated.constructSourceResource(destination); }).empty());
    CHECK(destination.state() == doe::QStringPathLeaseState::Empty && isolated.occurrenceCount() == 0);
    unknown(process);
}

int main() { return testkit::run_all(); }
