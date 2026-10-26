#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include "../support/test_paths.hpp"
// Candidate-only active dispatch from the retained B:120_B_DOE Sub_Project 1.
// Expected first stop: session records encounter retained DBLIST/ReportList after study setup.
// This is not public API routing, a Save test, or original GUI acceptance.
// ctest-labels: m2m-mdt m2m-connector m2m-doe
#include <windows.h>
#include <bcrypt.h>
#pragma comment(lib, "bcrypt.lib")

#include <testkit/check.hpp>

#include <climits>
#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <map>
#include <process.h>
#include <stdexcept>
#include <string>
#include <vector>

#include "m2m/doe_io.hpp"
#include "m2m/engine.hpp"
#include "m2m/model.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/qt4path.hpp"

namespace fs = std::filesystem;
using namespace m2m;
using qt::L;

namespace {

struct ManifestEntry {
    bool directory = false;
    std::uintmax_t size = 0;
    std::string sha256;
    bool operator==(const ManifestEntry&) const = default;
};
using Manifest = std::map<std::wstring, ManifestEntry>;

std::string sha256(const unsigned char* bytes, size_t size)
{
    if (size > static_cast<size_t>(ULONG_MAX)) throw std::runtime_error("input exceeds BCryptHash limit");
    unsigned char digest[32]{};
    if (BCryptHash(BCRYPT_SHA256_ALG_HANDLE, nullptr, 0,
                   const_cast<PUCHAR>(bytes), static_cast<ULONG>(size), digest,
                   static_cast<ULONG>(sizeof(digest))) != 0)
        throw std::runtime_error("BCrypt SHA-256 failed");
    static constexpr char hex[] = "0123456789abcdef";
    std::string result;
    result.reserve(64);
    for (const unsigned char b : digest) {
        result.push_back(hex[b >> 4]);
        result.push_back(hex[b & 0x0f]);
    }
    return result;
}

std::string sha256File(const fs::path& path)
{
    std::ifstream in(path, std::ios::binary);
    if (!in) throw std::runtime_error("cannot read fixture file: " + path.string());
    const std::vector<unsigned char> bytes((std::istreambuf_iterator<char>(in)),
                                           std::istreambuf_iterator<char>());
    return sha256(bytes.data(), bytes.size());
}

std::string sha256Text(const std::string& bytes)
{
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
            value.sha256 = sha256File(entry.path());
        }
        result.emplace(entry.path().lexically_relative(root).generic_wstring(), std::move(value));
    }
    return result;
}

void copyTree(const fs::path& from, const fs::path& to)
{
    fs::create_directories(to);
    fs::copy(from, to, fs::copy_options::recursive | fs::copy_options::overwrite_existing);
}

const xml::Node* firstChild(const xml::Node* parent, const char* tag)
{
    if (!parent) return nullptr;
    const auto children = parent->childElements(L(tag));
    return children.empty() ? nullptr : children.front();
}

const xml::Node* requestedTargetNode(const xml::Node* item)
{
    const xml::Node* connectors = firstChild(item, "Connectors");
    const xml::Node* connections = firstChild(connectors, "Connections");
    return firstChild(connections, "ItemIndex");
}

FlowItem* itemByOperationId(SubProject& sub, int id)
{
    for (const auto& item : sub.flow.items)
        if (item && item->opId == id) return item.get();
    return nullptr;
}

}  // namespace

TEST_CASE(active_doe_output_stops_at_retained_ds_dblist_guard_after_study_setup)
{
    const fs::path sourceDir = m2m_test::configuredPath("M2M_ORACLE_B") / "120_B_DOE";
    const fs::path sourceProject = sourceDir / "COG_P1_B1.moproj";
    const fs::path sourceDs = sourceDir / "DoeStudies" / "DoeStudy1" / "Control" / "COG_P1_B1.DS";
    const std::string projectHash =
        "60addc2722e85f92a088fa07f77e8a966928ab5b934f161df87dcf6a417ed30e";
    const std::string dsHash =
        "97e4aaa28aa545dd08360e98191fba3b2cfb6c39098e10aeb3b45d02c33e563d";
    CHECK(fs::is_regular_file(sourceProject));
    CHECK(fs::is_regular_file(sourceDs));
    if (!fs::is_regular_file(sourceProject) || !fs::is_regular_file(sourceDs)) return;

    CHECK_EQ(sha256File(sourceProject), projectHash);
    CHECK_EQ(sha256File(sourceDs), dsHash);
    CHECK_EQ(fs::file_size(sourceDs), 2013u);
    const Manifest sourceBefore = manifest(sourceDir);

    const fs::path scratch = m2m_test::scratchOutput(
        m2m_test::scratchRoot() / "doeoutput_active_guard" / std::to_string(_getpid()));
    std::error_code ec;
    fs::remove_all(scratch, ec);
    if (ec) throw fs::filesystem_error("clear DoeOutput test scratch", scratch, ec);
    copyTree(sourceDir, scratch);
    const Manifest before = manifest(scratch);
    CHECK(sourceBefore == before);

    bool suffixAccepted = false;
    bool projectWalkSucceeded = false;
    bool subprojectSelected = false;
    bool activeDoeOutput = false;
    bool connectorStillLinked = false;
    bool registryFlagsMatch = false;
    bool expectedRefusal = false;
    bool dsReadVerified = false;
    bool studyWritesVerified = false;
    bool noFileEffectsWrites = false;
    int errorCode = 0;
    std::string errorText;
    std::vector<std::string> warnings;
    std::shared_ptr<doe::StudyIo> io = doe::makeRecordingStudyIo();

    try {
        qt::CurrentDirScope keepDir(qt::currentPath());
        if (!keepDir.ok()) throw std::runtime_error("cannot preserve test thread current directory");

        Project p;
        ProjectScope projectScope(p);
        Ctx ctx;
        p.doeIo = io;
        io->setQtWarningSink([&ctx](const std::string& warning) {
            ctx.originalDiagnostics.push_back(warning);
        });

        const fs::path projectPath = scratch / sourceProject.filename();
        const qt::String file{projectPath.u16string()};
        const qt::String db = app::changeSuffix(file, L(".DB"));
        initProject(p, qt::absolutePath(db), qt::completeBaseName(db));
        makeRegistry(p);
        suffixAccepted = openProject(p, file, ctx);
        projectWalkSucceeded = p.openedOk;

        if (suffixAccepted && projectWalkSucceeded && p.subs.size() > 1) {
            const xml::Node* subElement = subProjectElement(p, 1);
            const auto diagramItems = subElement ? subElement->childElements(L("DiagramItem"))
                                                 : std::vector<const xml::Node*>{};
            bool requestIsExpected = false;
            if (diagramItems.size() > 5) {
                const xml::Node* from = diagramItems[2];
                const xml::Node* to = diagramItems[5];
                const xml::Node* requestedTarget = requestedTargetNode(from);
                requestIsExpected = qt::toInt(from->attribute(L("Operation_Id"))) == 3 &&
                                    qt::toInt(to->attribute(L("Operation_Id"))) == 6 &&
                                    requestedTarget && qt::toInt(requestedTarget->text()) == 5;
            }
            CHECK(requestIsExpected);

            activateSubProject(p, 1, ctx);
            subprojectSelected = p.activeSub == 1 && p.subs[1]->type == 2 &&
                                 p.subs[1]->loaded && !p.loadingSub &&
                                 p.subs[1]->flow.activeItem == 5;
            FlowItem* cogging = itemByOperationId(*p.subs[1], 3);
            FlowItem* doeOutput = itemByOperationId(*p.subs[1], 6);
            activeDoeOutput = doeOutput && p.subs[1]->flow.items.size() > 5 &&
                              p.subs[1]->flow.items[5].get() == doeOutput &&
                              doeOutput->className == L("SfOprDoeOutput");
            connectorStillLinked = cogging && doeOutput && cogging->next == doeOutput &&
                                   doeOutput->prev == cogging;
            const auto cogClass = p.registry.find(L("SfOprCogging"));
            const auto doeClass = p.registry.find(L("SfOprDoeOutput"));
            registryFlagsMatch = cogClass != p.registry.end() && cogClass->second &&
                                 cogClass->second->dimFlags == 0x1a &&
                                 doeClass != p.registry.end() && doeClass->second &&
                                 doeClass->second->dimFlags == 0x20097;

            if (subprojectSelected && activeDoeOutput && connectorStillLinked && registryFlagsMatch) {
                engine::FileEffects effects;
                try {
                    (void)engine::openActiveOperation(p, ctx, effects);
                } catch (const Error& error) {
                    errorCode = error.code;
                    errorText = error.what();
                }
                expectedRefusal = errorCode == M2M_E_UNSUPPORTED &&
                    errorText == "DOE session 0x140650e60 DBLIST/ReportList import remains unsupported";
                const fs::path expectedDs = scratch / sourceDs.lexically_relative(sourceDir);
                const qt::String expectedDsPath{expectedDs.u16string()};
                for (const doe::ReadAttempt& read : io->reads()) {
                    if (qt::cleanPath(read.path) != qt::cleanPath(expectedDsPath)) continue;
                    dsReadVerified = read.opened && read.readAttempted && read.readSucceeded &&
                                     read.sizeObserved && read.sizeBeforeRead == 2013u &&
                                     read.bytes.size() == 2013u && sha256Text(read.bytes) == dsHash;
                    break;
                }
                const char* const sampleNames[] = {
                    "Samples_LHS.par", "Samples_FF.par", "Samples_USR.par"};
                const auto& textWrites = io->textWrites();
                studyWritesVerified = textWrites.size() == 3;
                for (std::size_t i = 0; i < textWrites.size() && i < 3; ++i) {
                    const fs::path expectedPath = scratch / "DoeStudies" / "DoeStudy1" /
                        "Control" / sampleNames[i];
                    studyWritesVerified = studyWritesVerified &&
                        qt::cleanPath(textWrites[i].path) ==
                            qt::cleanPath(qt::String{expectedPath.u16string()}) &&
                        textWrites[i].requestedBytes == std::to_string(i) + "\r\n" &&
                        textWrites[i].opened && textWrites[i].writeSucceeded &&
                        textWrites[i].streamGoodAfterClose;
                }
                noFileEffectsWrites = effects.writes.empty() && effects.deletes.empty() &&
                                      effects.mkdirs.empty() && effects.directoryAttempts.empty() &&
                                      effects.removalAttempts.empty() && effects.renames.empty();
                warnings = io->qtWarnings();
                std::printf("open=%s openedOk=%s sub=%d activeItem=%d op=6 class=DoeOutput edge=3->6 "
                            "flags=0x1a/0x20097 refusalCode=%d dsRead=%s studyTextWrites=%zu "
                            "fileEffectWrites=%zu fileEffectDeletes=%zu warnings=%zu\n",
                            suffixAccepted ? "accepted" : "rejected",
                            projectWalkSucceeded ? "true" : "false", p.activeSub,
                            p.subs[1]->flow.activeItem, errorCode,
                            dsReadVerified ? "verified" : "not-verified", textWrites.size(),
                            effects.writes.size(), effects.deletes.size(), warnings.size());
                if (!errorText.empty()) std::printf("first refusal: %s\n", errorText.c_str());
            }
        }
    } catch (const Error& error) {
        errorCode = error.code;
        errorText = error.what();
    } catch (const std::exception& error) {
        errorCode = -1;
        errorText = error.what();
    }

    if (!errorText.empty())
        std::printf("DoeOutput active guard setup/refusal: code=%d message=%s\n", errorCode, errorText.c_str());
    CHECK(suffixAccepted);
    CHECK(projectWalkSucceeded);
    CHECK(subprojectSelected);
    CHECK(activeDoeOutput);
    CHECK(connectorStillLinked);
    CHECK(registryFlagsMatch);
    CHECK(expectedRefusal);
    CHECK(dsReadVerified);
    CHECK(studyWritesVerified);
    CHECK(noFileEffectsWrites);

    const Manifest after = manifest(scratch);
    CHECK(before == after);
    CHECK(sourceBefore == manifest(sourceDir));
}

int main()
{
    return testkit::run_all();
}
