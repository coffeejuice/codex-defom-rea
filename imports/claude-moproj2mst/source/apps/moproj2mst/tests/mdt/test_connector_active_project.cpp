#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include "../support/test_paths.hpp"
// Candidate-only integration: parse the retained complete B:120_B_DOE project and explicitly
// activate Sub_Project 1, whose requested operation-id edge is Cogging (3) -> DoeOutput (6).
// This does not invoke operation loading, Save, or the original GUI.
// ctest-labels: m2m-mdt m2m-connector
#include <windows.h>
#include <bcrypt.h>
#pragma comment(lib, "bcrypt.lib")

#include <testkit/check.hpp>

#include <filesystem>
#include <fstream>
#include <iterator>
#include <climits>
#include <cstdint>
#include <map>
#include <process.h>
#include <stdexcept>
#include <string>
#include <vector>

#include "m2m/doe_io.hpp"
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

std::string sha256File(const fs::path& path)
{
    std::ifstream in(path, std::ios::binary);
    if (!in) throw std::runtime_error("cannot read fixture file: " + path.string());
    const std::vector<unsigned char> bytes((std::istreambuf_iterator<char>(in)),
                                           std::istreambuf_iterator<char>());
    unsigned char digest[32]{};
    if (bytes.size() > static_cast<size_t>(ULONG_MAX) ||
        BCryptHash(BCRYPT_SHA256_ALG_HANDLE, nullptr, 0,
                   const_cast<PUCHAR>(bytes.data()), static_cast<ULONG>(bytes.size()), digest,
                   static_cast<ULONG>(sizeof(digest))) != 0)
        throw std::runtime_error("SHA-256 failed for fixture file: " + path.string());
    static constexpr char hex[] = "0123456789abcdef";
    std::string result;
    result.reserve(64);
    for (const unsigned char b : digest) {
        result.push_back(hex[b >> 4]);
        result.push_back(hex[b & 0x0f]);
    }
    return result;
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

TEST_CASE(retained_complete_project_cogging_to_doe_output_edge)
{
    const fs::path sourceDir = m2m_test::configuredPath("M2M_ORACLE_B") / "120_B_DOE";
    const fs::path sourceProject = sourceDir / "COG_P1_B1.moproj";
    const std::string expectedProjectHash =
        "60addc2722e85f92a088fa07f77e8a966928ab5b934f161df87dcf6a417ed30e";
    CHECK(fs::is_regular_file(sourceProject));
    if (!fs::is_regular_file(sourceProject)) return;

    CHECK_EQ(sha256File(sourceProject), expectedProjectHash);
    const Manifest sourceBefore = manifest(sourceDir);

    const fs::path scratch = m2m_test::scratchOutput(
        m2m_test::scratchRoot() / "connector_active_project" / std::to_string(_getpid()));
    std::error_code ec;
    fs::remove_all(scratch, ec);
    if (ec) throw fs::filesystem_error("clear connector test scratch", scratch, ec);
    fs::create_directories(scratch);
    fs::copy(sourceDir, scratch, fs::copy_options::recursive | fs::copy_options::overwrite_existing);

    const fs::path projectPath = scratch / sourceProject.filename();
    const Manifest before = manifest(scratch);
    CHECK(sourceBefore == before);
    bool suffixAccepted = false;
    bool projectWalkSucceeded = false;
    bool secondSubActivated = false;
    bool xmlRequestMatches = false;
    bool registryFlagsMatch = false;
    bool graphMatches = false;
    int errorCode = 0;
    std::string errorText;
    std::string edgeReceipt = "not reached";

    try {
        qt::CurrentDirScope keepDir(qt::currentPath());
        if (!keepDir.ok()) throw std::runtime_error("cannot preserve test thread current directory");

        Project p;
        Ctx ctx;
        p.doeIo = doe::makeRecordingStudyIo();
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
            if (diagramItems.size() > 5) {
                const xml::Node* sourceItem = diagramItems[2];
                const xml::Node* targetItem = diagramItems[5];
                const xml::Node* targetIndex = requestedTargetNode(sourceItem);
                xmlRequestMatches = qt::toInt(sourceItem->attribute(L("Operation_Id"))) == 3 &&
                                    qt::toInt(targetItem->attribute(L("Operation_Id"))) == 6 &&
                                    targetIndex && qt::toInt(targetIndex->text()) == 5;
            }

            activateSubProject(p, 1, ctx);
            secondSubActivated = p.activeSub == 1 && p.subs[1]->loaded && !p.loadingSub;
            FlowItem* cogging = itemByOperationId(*p.subs[1], 3);
            FlowItem* doeOutput = itemByOperationId(*p.subs[1], 6);
            const auto cogClass = p.registry.find(L("SfOprCogging"));
            const auto doeClass = p.registry.find(L("SfOprDoeOutput"));
            registryFlagsMatch = cogClass != p.registry.end() && cogClass->second &&
                                 cogClass->second->dimFlags == 0x1a &&
                                 doeClass != p.registry.end() && doeClass->second &&
                                 doeClass->second->dimFlags == 0x20097;
            graphMatches = cogging && doeOutput && cogging->className == L("SfOprCogging") &&
                           doeOutput->className == L("SfOprDoeOutput") &&
                           cogging->next == doeOutput && doeOutput->prev == cogging;
            edgeReceipt = std::string("3(SfOprCogging)->6(SfOprDoeOutput), flags=") +
                          (registryFlagsMatch ? "0x1a/0x20097" : "unexpected") +
                          (graphMatches ? ", accepted" : ", not accepted");
            std::printf("open=%s openedOk=%s activeSub=%d loaded=%s xmlRequest=%s edge=%s flags=%s warnings=%zu diagnostics=%zu\n",
                        suffixAccepted ? "accepted" : "rejected", projectWalkSucceeded ? "true" : "false",
                        p.activeSub, secondSubActivated ? "true" : "false",
                        xmlRequestMatches ? "3->6 via item[2]->item[5]" : "unexpected",
                        edgeReceipt.c_str(), registryFlagsMatch ? "0x1a/0x20097" : "unexpected",
                        ctx.warnings.size(), ctx.originalDiagnostics.size());
        } else {
            edgeReceipt = "project open or second sub-project unavailable";
            std::printf("open=%s openedOk=%s subs=%zu edge=%s warnings=%zu diagnostics=%zu\n",
                        suffixAccepted ? "accepted" : "rejected", projectWalkSucceeded ? "true" : "false",
                        p.subs.size(), edgeReceipt.c_str(), ctx.warnings.size(),
                        ctx.originalDiagnostics.size());
        }
    } catch (const Error& error) {
        errorCode = error.code;
        errorText = error.what();
    } catch (const std::exception& error) {
        errorCode = -1;
        errorText = error.what();
    }

    if (!errorText.empty())
        std::printf("connector project check stopped: code=%d message=%s\n", errorCode, errorText.c_str());
    CHECK(suffixAccepted);
    CHECK(projectWalkSucceeded);
    CHECK(xmlRequestMatches);
    CHECK(secondSubActivated);
    CHECK(registryFlagsMatch);
    CHECK(graphMatches);

    const Manifest after = manifest(scratch);
    CHECK(before == after);
    CHECK(sourceBefore == manifest(sourceDir));
}

int main()
{
    return testkit::run_all();
}
