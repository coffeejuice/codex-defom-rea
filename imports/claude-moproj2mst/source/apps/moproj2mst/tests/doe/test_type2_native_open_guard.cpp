// ctest-labels: m2m-doe m2m-moproj
// Native .moproj writer prefix: physical open/truncation precedes the Type2 InputTable guard.
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include "../support/test_paths.hpp"

#include <windows.h>
#include <bcrypt.h>
#pragma comment(lib, "bcrypt.lib")

#include <testkit/check.hpp>

#include <cstdint>
#include <climits>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <map>
#include <memory>
#include <stdexcept>
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

namespace fs = std::filesystem;
using namespace m2m;

namespace {
struct ManifestEntry {
    bool directory = false;
    std::uintmax_t size = 0;
    std::string hash;
    bool operator==(const ManifestEntry&) const = default;
};
using Manifest = std::map<std::wstring, ManifestEntry>;

std::string sha256(const unsigned char* bytes, std::size_t size)
{
    if (size > static_cast<std::size_t>(ULONG_MAX))
        throw std::runtime_error("Type2 fixture file exceeds BCryptHash limit");
    unsigned char digest[32]{};
    if (BCryptHash(BCRYPT_SHA256_ALG_HANDLE, nullptr, 0,
                   const_cast<PUCHAR>(bytes), static_cast<ULONG>(size), digest,
                   static_cast<ULONG>(sizeof(digest))) != 0)
        throw std::runtime_error("Type2 fixture SHA-256 failed");
    static constexpr char digits[] = "0123456789abcdef";
    std::string result;
    result.reserve(64);
    for (const unsigned char b : digest) {
        result.push_back(digits[b >> 4]);
        result.push_back(digits[b & 0x0f]);
    }
    return result;
}

std::string readAll(const fs::path& path)
{
    std::ifstream input(path, std::ios::binary);
    if (!input) throw std::runtime_error("cannot read Type2 fixture: " + path.string());
    return {std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
}

std::string sha256File(const fs::path& path)
{
    const std::string bytes = readAll(path);
    return sha256(reinterpret_cast<const unsigned char*>(bytes.data()), bytes.size());
}

Manifest manifest(const fs::path& root)
{
    Manifest result;
    for (const auto& entry : fs::recursive_directory_iterator(root)) {
        ManifestEntry value;
        value.directory = entry.is_directory();
        if (!value.directory) {
            value.size = entry.file_size();
            value.hash = sha256File(entry.path());
        }
        result.emplace(entry.path().lexically_relative(root).generic_wstring(),
                       std::move(value));
    }
    return result;
}

void copyTree(const fs::path& source, const fs::path& destination)
{
    fs::create_directories(destination);
    fs::copy(source, destination,
             fs::copy_options::recursive | fs::copy_options::overwrite_existing);
}

class MinimalViews final : public moproj::DiagramViews {
public:
    int queried = 0;
    void viewCentre(const SubProject&, double& x, double& y) override
    { ++queried; x = 1; y = 2; }
    xml::NodePtr dispView(const SubProject&) override
    { ++queried; return xml::createElement(qt::L("SfDispView")); }
    bool expanded(const SubProject&, int) override { return true; }
    moproj::RectF rect(const SubProject&, int) override { return {}; }
    std::vector<moproj::ExpansionItem> expansion(const SubProject&, int) override { return {}; }
};

xml::NodePtr subProjectNode(xml::Document& document, std::size_t wanted)
{
    std::size_t index = 0;
    for (const auto& child : document.root->children) {
        if (child->kind != xml::Node::Element || child->tag != qt::L("Sub_Project"))
            continue;
        if (index++ == wanted) return child;
    }
    return {};
}
} // namespace

TEST_CASE(type2_native_project_writer_truncates_before_input_table_guard_and_retains_old_subtree)
{
    const fs::path source = m2m_test::configuredPath("M2M_ORACLE_B") / "120_B_DOE";
    const fs::path sourceProject = source / "COG_P1_B1.moproj";
    REQUIRE(fs::is_regular_file(sourceProject));
    CHECK_EQ(fs::file_size(sourceProject), 34774u);
    CHECK_EQ(sha256File(sourceProject),
             std::string("60addc2722e85f92a088fa07f77e8a966928ab5b934f161df87dcf6a417ed30e"));
    const Manifest sourceBefore = manifest(source);

    const fs::path scratch = m2m_test::scratchOutput(
        m2m_test::scratchRoot() / "type2_native_open_guard" /
        (std::to_string(GetCurrentProcessId()) + "-" +
         std::to_string(GetTickCount64())));
    const fs::path projectDir = scratch / "project";
    REQUIRE(!fs::exists(scratch));
    copyTree(source, projectDir);
    CHECK(sourceBefore == manifest(projectDir));

    const fs::path projectPath = projectDir / "COG_P1_B1.moproj";
    const std::string originalXml = readAll(projectPath);
    engine::DiagramEffectDriver driver;
    engine::ConversionSession session(driver);
    Project& project = session.project();
    initProject(project, qt::String(projectDir.u16string()), qt::L("COG_P1_B1"));
    project.path = qt::String(projectPath.u16string());
    project.doeIo = doe::makeNativeStudyIo();
    project.doeIo->bindModelManager(session.modelManagerOwner());
    project.doeIo->bindSessionNotificationState(session.sessionNotificationStateOwner());
    project.subs.push_back(makeSubProject(project, 2, 1));
    project.activeSub = 1; // Owned Type2 SUB from the retained source fixture.
    project.title = qt::L("mutated root title before Type2 guard");
    project.dom = std::make_shared<xml::Document>();
    qt::String parseError;
    int errorLine = 0, errorColumn = 0;
    REQUIRE(xml::setContent(*project.dom, originalXml, &parseError, &errorLine, &errorColumn));
    const fs::path inputTablePath = projectDir / "DoeStudies" / "DoeStudy1" /
                                    "Control" / "DoeInputTable.XML";
    REQUIRE(fs::is_regular_file(inputTablePath));
    REQUIRE(doe::readInputTable(readAll(inputTablePath), *project.subs[1]->ownedDoeStudy));
    REQUIRE(project.subs[1]->ownedDoeStudy->vars.empty());
    CHECK_EQ(project.subs[1]->ownedDoeStudy->constraints, 0);
    // Force the native Save prefix to create this file; an unchanged copy of
    // the retained after-state would not prove that the XML writer ran.
    const std::string retainedInputTableHash = sha256File(inputTablePath);
    CHECK_EQ(retainedInputTableHash,
             std::string("1777bb9948fb2d6f55411f9dcfdc4a7c9196230086c0f86720255d03e76571e0"));
    const std::string retainedInputTableBytes = readAll(inputTablePath);
    REQUIRE(fs::remove(inputTablePath));

    const auto oldType2Sub = subProjectNode(*project.dom, 1);
    REQUIRE(oldType2Sub != nullptr);
    CHECK(oldType2Sub->attribute(qt::L("Type")) == qt::L("2"));
    CHECK(oldType2Sub->attribute(qt::L("Id")) == qt::L("1"));
    xml::Document oldSubDocument;
    oldSubDocument.root = oldType2Sub;
    const std::u16string oldSubXml = xml::toString(oldSubDocument, 1);
    REQUIRE(!oldSubXml.empty());

    const std::string beforeBytes = readAll(projectPath);
    REQUIRE(beforeBytes == originalXml);
    MinimalViews views;
    const auto journalBefore = project.doeIo->xmlJournalOwner();
    REQUIRE(journalBefore != nullptr);
    REQUIRE(journalBefore->attempts().empty());
    REQUIRE(journalBefore->events().empty());
    engine::FileEffects observation;
    int errorCode = 0;
    std::string errorText;
    try {
        (void)moproj::saveProjectFileNative(project, false, views, {}, &session, &observation);
    } catch (const Error& error) {
        errorCode = error.code;
        errorText = error.what();
    }

    CHECK_EQ(errorCode, M2M_E_UNSUPPORTED);
    CHECK_EQ(errorText,
             std::string("DOE table owner: main-window +458 is unavailable"));
    // The inner XML writer returned normally and committed its actual native
    // file before the enclosing Save stopped at DoeTableRefresh delivery.
    REQUIRE(observation.xmlJournal != nullptr);
    CHECK(observation.xmlJournal == journalBefore);
    CHECK(project.doeIo->xmlJournalOwner() == journalBefore);
    REQUIRE(observation.xmlJournal->attempts().size() == 1u);
    const auto& xmlAttempt = observation.xmlJournal->attempts().front();
    CHECK(xmlAttempt.path == project.doeIo->xmlWrites().front().path);
    CHECK(xmlAttempt.opened && xmlAttempt.openResultObserved && xmlAttempt.closed);
    REQUIRE(xmlAttempt.errorAfterClose.has_value());
    CHECK_EQ(*xmlAttempt.errorAfterClose, 0);
    REQUIRE(xmlAttempt.normalWriterReturn.has_value());
    CHECK(xmlAttempt.normalWriterReturn->writerSucceeded);
    CHECK(!xmlAttempt.normalWriterReturn->streamFailed);
    CHECK(xmlAttempt.refreshBoundaryReached);
    CHECK(readAll(inputTablePath) == retainedInputTableBytes);
    CHECK_EQ(sha256File(inputTablePath), retainedInputTableHash);
    const std::vector<doe::XmlJournalEventKind> expectedEvents = {
        doe::XmlJournalEventKind::OpenRequested,
        doe::XmlJournalEventKind::OpenResult,
        doe::XmlJournalEventKind::Closing,
        doe::XmlJournalEventKind::StreamWriteRequested,
        doe::XmlJournalEventKind::StreamWriteReturned,
        doe::XmlJournalEventKind::Flush,
        doe::XmlJournalEventKind::Closed,
        doe::XmlJournalEventKind::ErrorAfterClose,
        doe::XmlJournalEventKind::NormalWriterReturn,
        doe::XmlJournalEventKind::RefreshBoundaryReached,
    };
    REQUIRE(observation.xmlJournal->events().size() == expectedEvents.size());
    for (std::size_t i = 0; i < expectedEvents.size(); ++i) {
        CHECK(observation.xmlJournal->events()[i].attempt == 0u);
        CHECK(observation.xmlJournal->events()[i].kind == expectedEvents[i]);
    }
    // FileEffects carries an observation alias only; native XML must not be
    // replayed as candidate writes or projected directory/removal effects.
    CHECK(observation.writes.empty());
    CHECK(observation.deletes.empty());
    CHECK(observation.mkdirs.empty());
    CHECK(observation.directoryAttempts.empty());
    CHECK(observation.removalAttempts.empty());
    CHECK(observation.renames.empty());
    CHECK_EQ(fs::file_size(projectPath), 0u); // Native FileDevice opened/truncated before body guard.
    REQUIRE(fs::is_regular_file(inputTablePath));
    CHECK_EQ(fs::file_size(inputTablePath), 249u);
    CHECK_EQ(sha256File(inputTablePath), retainedInputTableHash);
    CHECK_EQ(views.queried, 2); // View prefix ran before the Type2 node guard.
    CHECK(subProjectNode(*project.dom, 1) == oldType2Sub);
    xml::Document oldSubAfter;
    oldSubAfter.root = subProjectNode(*project.dom, 1);
    REQUIRE(oldSubAfter.root != nullptr);
    CHECK(xml::toString(oldSubAfter, 1) == oldSubXml);
    const auto titles = project.dom->root->childElements(qt::L("Title"));
    REQUIRE(!titles.empty());
    CHECK(titles.front()->attribute(qt::L("Name")) == project.title);
    CHECK(sourceBefore == manifest(source));
}

TEST_CASE(type2_native_journal_survives_refresh_guard_and_actual_session_teardown)
{
    const fs::path source = m2m_test::configuredPath("M2M_ORACLE_B") / "120_B_DOE";
    const fs::path sourceProject = source / "COG_P1_B1.moproj";
    REQUIRE(fs::is_regular_file(sourceProject));
    CHECK_EQ(sha256File(sourceProject),
             std::string("60addc2722e85f92a088fa07f77e8a966928ab5b934f161df87dcf6a417ed30e"));
    const Manifest sourceBefore = manifest(source);
    const fs::path scratch = m2m_test::scratchOutput(
        m2m_test::scratchRoot() / "type2_native_journal_lifetime" /
        (std::to_string(GetCurrentProcessId()) + "-" +
         std::to_string(GetTickCount64())));
    const fs::path projectDir = scratch / "project";
    REQUIRE(!fs::exists(scratch));
    copyTree(source, projectDir);
    CHECK(sourceBefore == manifest(projectDir));
    const fs::path projectPath = projectDir / "COG_P1_B1.moproj";
    const fs::path inputTablePath = projectDir / "DoeStudies" / "DoeStudy1" /
                                    "Control" / "DoeInputTable.XML";
    REQUIRE(fs::is_regular_file(inputTablePath));
    const std::string originalXml = readAll(projectPath);
    const std::string retainedInputTableBytes = readAll(inputTablePath);
    const std::string retainedInputTableHash = sha256File(inputTablePath);
    CHECK_EQ(retainedInputTableHash,
             std::string("1777bb9948fb2d6f55411f9dcfdc4a7c9196230086c0f86720255d03e76571e0"));
    const qt::String expectedPath = qt::cleanPath(qt::String(inputTablePath.u16string()));

    // Only the observation owner escapes. The actual native I/O and its Session
    // must die even though a caller still owns FileEffects and the const journal.
    engine::FileEffects observation;
    std::shared_ptr<const doe::StudyXmlJournal> journal;
    std::weak_ptr<doe::StudyIo> ioWeak;
    std::size_t eventsBeforeTeardown = 0;
    qt::String lastErrorAtReturn;
    {
        engine::DiagramEffectDriver driver;
        engine::ConversionSession session(driver);
        Project& project = session.project();
        initProject(project, qt::String(projectDir.u16string()), qt::L("COG_P1_B1"));
        project.path = qt::String(projectPath.u16string());
        project.doeIo = doe::makeNativeStudyIo();
        project.doeIo->bindModelManager(session.modelManagerOwner());
        project.doeIo->bindSessionNotificationState(session.sessionNotificationStateOwner());
        ioWeak = project.doeIo;
        project.subs.push_back(makeSubProject(project, 2, 1));
        project.activeSub = 1;
        project.dom = std::make_shared<xml::Document>();
        qt::String parseError;
        int errorLine = 0, errorColumn = 0;
        REQUIRE(xml::setContent(*project.dom, originalXml, &parseError, &errorLine, &errorColumn));
        REQUIRE(doe::readInputTable(retainedInputTableBytes, *project.subs[1]->ownedDoeStudy));
        REQUIRE(project.subs[1]->ownedDoeStudy->vars.empty());
        REQUIRE(project.subs[1]->ownedDoeStudy->constraints == 0);
        REQUIRE(fs::remove(inputTablePath));
        REQUIRE(!fs::exists(inputTablePath));
        MinimalViews views;
        int errorCode = 0;
        std::string errorText;
        try {
            (void)moproj::saveProjectFileNative(project, false, views, {}, &session, &observation);
        } catch (const Error& error) {
            errorCode = error.code;
            errorText = error.what();
        }
        CHECK_EQ(errorCode, M2M_E_UNSUPPORTED);
        CHECK_EQ(errorText, std::string("DOE table owner: main-window +458 is unavailable"));
        journal = observation.xmlJournal;
        REQUIRE(journal != nullptr);
        CHECK(journal == project.doeIo->xmlJournalOwner());
        REQUIRE(journal->attempts().size() == 1u);
        const auto& attempt = journal->attempts().front();
        CHECK(qt::cleanPath(attempt.path) == expectedPath);
        CHECK(attempt.opened && attempt.openResultObserved && attempt.closed);
        REQUIRE(attempt.errorAfterClose.has_value());
        CHECK_EQ(*attempt.errorAfterClose, 0);
        REQUIRE(attempt.normalWriterReturn.has_value());
        CHECK(attempt.normalWriterReturn->writerSucceeded);
        CHECK(!attempt.normalWriterReturn->streamFailed);
        CHECK(attempt.refreshBoundaryReached);
        lastErrorAtReturn = attempt.normalWriterReturn->stickyLastError;
        REQUIRE(!journal->events().empty());
        CHECK(journal->events().back().kind == doe::XmlJournalEventKind::RefreshBoundaryReached);
        CHECK(journal->events().back().attempt == 0u);
        eventsBeforeTeardown = journal->events().size();
        CHECK(!ioWeak.expired());
    }

    // The real Session/Project/StudyIo have been destroyed. Diagnostics remain
    // value observations; no native contents are inferred from stream counts.
    CHECK(ioWeak.expired());
    REQUIRE(journal != nullptr);
    CHECK(observation.xmlJournal == journal);
    REQUIRE(journal->attempts().size() == 1u);
    const auto& retained = journal->attempts().front();
    CHECK(qt::cleanPath(retained.path) == expectedPath);
    CHECK(retained.opened && retained.openResultObserved && retained.closed);
    REQUIRE(retained.errorAfterClose.has_value());
    CHECK_EQ(*retained.errorAfterClose, 0);
    REQUIRE(retained.normalWriterReturn.has_value());
    CHECK(retained.normalWriterReturn->writerSucceeded);
    CHECK(!retained.normalWriterReturn->streamFailed);
    CHECK(retained.normalWriterReturn->stickyLastError == lastErrorAtReturn);
    CHECK(retained.refreshBoundaryReached);
    CHECK(journal->events().size() == eventsBeforeTeardown);
    REQUIRE(!journal->events().empty());
    CHECK(journal->events().back().kind == doe::XmlJournalEventKind::RefreshBoundaryReached);
    CHECK(!retained.recordingStoreTracked);
    CHECK(retained.recordingRawSpans.empty());
    CHECK(!retained.recordedBytesAfterClose);
    CHECK(!journal->recordedFile(expectedPath));
    CHECK(observation.writes.empty() && observation.deletes.empty() && observation.mkdirs.empty());
    REQUIRE(fs::is_regular_file(inputTablePath));
    CHECK_EQ(fs::file_size(inputTablePath), 249u);
    CHECK(readAll(inputTablePath) == retainedInputTableBytes);
    CHECK_EQ(sha256File(inputTablePath), retainedInputTableHash);
    REQUIRE(fs::is_regular_file(projectPath));
    CHECK_EQ(fs::file_size(projectPath), 0u);
    CHECK(sourceBefore == manifest(source));
}

int main() { return testkit::run_all(); }
