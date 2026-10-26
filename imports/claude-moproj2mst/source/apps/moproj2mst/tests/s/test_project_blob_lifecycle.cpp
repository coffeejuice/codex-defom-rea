#include "../support/test_paths.hpp"
// ctest-labels: m2m-s
// Candidate full-Save coverage through private per-call services. Deny only
// one side-file read during parent serialization; never script output status.
#include <testkit/check.hpp>

#include <algorithm>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "m2m/blob.hpp"
#include "m2m/mst/file_device.hpp"
#include "m2m/qt4path.hpp"
#include "m2m/xml.hpp"
#include "../../src/core/conversion_call_services.hpp"
#include "../../src/moproj/project_output_device.hpp"

namespace fs = std::filesystem;
using namespace m2m;

namespace {

std::string readFile(const fs::path& path)
{
    std::ifstream file(path, std::ios::binary);
    return {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
}

void writeFile(const fs::path& path, const std::string& bytes)
{
    std::ofstream file(path, std::ios::binary | std::ios::trunc);
    REQUIRE(file.is_open());
    file << bytes;
    REQUIRE(file.good());
}

struct Rig {
    fs::path project, side, bwz, ini, mst, remesh;
    std::string expectedSide;
    Rig()
    {
        const auto input = m2m_test::configuredPath("M2M_ORACLE_B") /
                           "133_B_DBSTEP_BOOL3_before";
        expectedSide = readFile(input / "Operations/Task00001/ObjProperties00001.KEY");
        REQUIRE(expectedSide.size() == 169);
        static unsigned serial = 0;
        const auto root = m2m_test::scratchOutput(m2m_test::scratchRoot() /
            ("blob-lifecycle-" + std::to_string(GetCurrentProcessId()) + "-" +
             std::to_string(GetTickCount64()) + "-" + std::to_string(++serial)));
        REQUIRE(!fs::exists(root));
        fs::copy(input, root, fs::copy_options::recursive);
        project = root / "COG_P1_B1.moproj";
        side = root / "Operations/Task00001/ObjProperties00001.KEY";
        bwz = root / "Operations/Task00002/Boolean.BWZ";
        ini = root / "Operations/Task00002/DEF_MACHDIST_CTRL2.INI";
        mst = root / "native-output.MST";
        remesh = root / "LOCAL_REMESH.DAT";
        for (const auto& path : {bwz, ini, mst, remesh}) REQUIRE(!fs::exists(path));
        REQUIRE(!fs::exists(root / "LOC_DEN_CTRL.DAT"));
        writeFile(root / "LOC_DEN_CTRL.DAT",
                  "FILTER_DISTANCE 7\nINTERNAL_ELEMENT_SIZE_CONTROL_METHOD 4\nDONE\n");
    }
};

class ReadDenial {
public:
    ~ReadDenial() { (void)release(); }
    ReadDenial() = default;
    ReadDenial(const ReadDenial&) = delete;
    ReadDenial& operator=(const ReadDenial&) = delete;
    void acquire(const fs::path& path)
    {
        handle_ = CreateFileW(path.c_str(), GENERIC_READ | GENERIC_WRITE, 0,
                              nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
        REQUIRE(handle_ != INVALID_HANDLE_VALUE);
    }
    bool held() const { return handle_ != INVALID_HANDLE_VALUE; }
    bool release()
    {
        if (!held()) return true;
        if (!CloseHandle(handle_)) return false;
        handle_ = INVALID_HANDLE_VALUE;
        return true;
    }
private:
    HANDLE handle_ = INVALID_HANDLE_VALUE;
};

struct Observation {
    std::vector<std::string> events;
    std::string sideBefore, projectAtError;
    unsigned factoryCalls = 0, callbacks = 0, closeReturns = 0, errorQueries = 0;
    std::size_t acceptedBytes = 0;
    bool opened = false, callbackWhileOpen = false, closedAtError = false;
    bool denialAtCallback = false, denialAtError = false;
    bool releaseSucceeded = false, laterOutputsAbsentAtError = false;
    int realError = -1;
};

class Output final : public moproj::detail::ProjectOutputDevice {
public:
    Output(const qt::String& path, const Rig& files, Observation& seen, ReadDenial& denial)
        : file_(path), files_(files), seen_(seen), denial_(denial)
    {
        seen_.opened = file_.opened();
        file_.setAboutToClose([this] {
            ++seen_.callbacks;
            seen_.events.push_back("aboutToClose");
            seen_.callbackWhileOpen = file_.opened();
            seen_.denialAtCallback = denial_.held();
            notifyAboutToClose();
        });
    }
    ~Output() override
    {
        file_.close();
        file_.setAboutToClose({});
    }
    std::int64_t write(std::string_view bytes) override
    {
        const auto accepted = file_.write(bytes);
        if (accepted > 0) seen_.acceptedBytes += static_cast<std::size_t>(accepted);
        return accepted;
    }
    bool flush() override { return file_.flush(); }
    bool opened() const override { return file_.opened(); }
    void close() override
    {
        if (!file_.opened()) return;
        file_.close();
        ++seen_.closeReturns;
        seen_.events.push_back("close returned");
    }
    int error() const override
    {
        ++seen_.errorQueries;
        seen_.events.push_back("error query");
        seen_.closedAtError = !file_.opened();
        seen_.denialAtError = denial_.held();
        seen_.realError = file_.error();
        seen_.projectAtError = readFile(files_.project); // physical closed output, not Project::dom
        seen_.laterOutputsAbsentAtError = !fs::exists(files_.bwz) && !fs::exists(files_.ini) &&
            !fs::exists(files_.mst) && !fs::exists(files_.remesh);
        seen_.releaseSucceeded = denial_.release(); // later readers remain outside this fault
        if (seen_.denialAtError) seen_.events.push_back("denial released");
        return seen_.realError;
    }
private:
    mst::FileDevice file_;
    const Rig& files_;
    Observation& seen_;
    ReadDenial& denial_;
};

class Factory final : public moproj::detail::ProjectOutputFactory {
public:
    Factory(const Rig& files, Observation& seen, bool deny)
        : files_(files), seen_(seen), deny_(deny) {}
    std::unique_ptr<moproj::detail::ProjectOutputDevice> open(const qt::String& path) override
    {
        ++seen_.factoryCalls;
        REQUIRE(qt::cleanPath(path) == qt::cleanPath(qt::String(files_.project.u16string())));
        seen_.events.push_back("factory/open");
        seen_.sideBefore = readFile(files_.side);
        REQUIRE(seen_.sideBefore == files_.expectedSide);
        auto output = std::make_unique<Output>(path, files_, seen_, denial_);
        REQUIRE(output->opened());
        if (deny_) {
            denial_.acquire(files_.side);
            REQUIRE(qt::fileExists(qt::String(files_.side.u16string())));
            std::ifstream probe(files_.side, std::ios::binary);
            REQUIRE(!probe.is_open()); // no skip or weaker fallback if denial setup fails
            seen_.events.push_back("denial acquired");
        }
        return output;
    }
    bool denialHeld() const { return denial_.held(); }
private:
    const Rig& files_;
    Observation& seen_;
    bool deny_;
    ReadDenial denial_;
};

struct Run {
    Result result;
    ConversionDetails details;
    Observation seen;
    std::string expectedSide, sideAfter, projectAfter, bwz, ini, mst, remesh;
};

Run run(bool deny)
{
    Rig files;
    Run output;
    Factory factory(files, output.seen, deny);
    detail::ConversionCallServices services{&factory};
    output.result = detail::convertDetailedWithServices(files.project.u16string(),
        files.mst.u16string(), M2M_FLAG_WRITE_SAVE, 1252, output.details, services);
    REQUIRE(!factory.denialHeld());
    output.expectedSide = files.expectedSide;
    output.sideAfter = readFile(files.side);
    output.projectAfter = readFile(files.project);
    output.bwz = readFile(files.bwz);
    output.ini = readFile(files.ini);
    output.mst = readFile(files.mst);
    output.remesh = readFile(files.remesh);
    return output;
}

xml::NodePtr uniqueChild(const xml::Node& parent, const char* tag)
{
    xml::NodePtr found;
    unsigned count = 0;
    for (const auto& child : parent.children)
        if (child->kind == xml::Node::Element && child->tag == qt::L(tag)) {
            found = child;
            ++count;
        }
    REQUIRE(count == 1);
    return found;
}

xml::NodePtr selectedObject(xml::Document& doc)
{
    REQUIRE(doc.root && doc.root->tag == qt::L("Project"));
    const auto list = uniqueChild(*doc.root, "Operation_List");
    xml::NodePtr operation;
    unsigned count = 0;
    for (const auto& child : list->children)
        if (child->kind == xml::Node::Element && child->tag == qt::L("Operation") &&
            uniqueChild(*child, "Id")->text() == qt::L("1")) {
            operation = child;
            ++count;
        }
    REQUIRE(count == 1);
    CHECK(uniqueChild(*operation, "Name")->text() == qt::L("HeatTransfer"));
    CHECK(uniqueChild(*operation, "Type")->text() == qt::L("SfOprHeatTransfer"));
    const auto objects = uniqueChild(*operation, "Objects");
    xml::NodePtr object;
    count = 0;
    for (const auto& child : objects->children)
        if (child->kind == xml::Node::Element && child->tag == qt::L("Object") &&
            child->attribute(qt::L("Id")) == qt::L("1")) {
            object = child;
            ++count;
        }
    REQUIRE(count == 1);
    return object;
}

std::string serializedNode(const xml::NodePtr& node)
{
    xml::Document doc;
    doc.root = node;
    return qt::String(xml::toString(doc, 1)).utf8();
}

void checkSave(const Run& output, bool denied)
{
    CHECK_EQ(output.result.code, M2M_OK);
    REQUIRE(output.details.returnedCode.has_value());
    CHECK_EQ(*output.details.returnedCode, M2M_OK);
    REQUIRE(output.details.saveCall.has_value());
    const auto& call = *output.details.saveCall;
    CHECK(call.projectWriteAttempted && call.projectWriteReturned && call.projectWriteSucceeded);
    CHECK(call.mstWriteAttempted && call.mstWriteReturned && call.mstWriteSucceeded);
    CHECK_EQ(output.seen.factoryCalls, 1u);
    CHECK(output.seen.opened && output.seen.acceptedBytes > 0);
    CHECK_EQ(output.seen.callbacks, 1u);
    CHECK_EQ(output.seen.closeReturns, 1u);
    CHECK_EQ(output.seen.errorQueries, 1u);
    CHECK(output.seen.callbackWhileOpen && output.seen.closedAtError);
    CHECK(output.seen.denialAtCallback == denied);
    CHECK(output.seen.denialAtError == denied);
    CHECK(output.seen.releaseSucceeded && output.seen.laterOutputsAbsentAtError);
    CHECK_EQ(output.seen.realError, 0);
    std::vector<std::string> events{"factory/open"};
    if (denied) events.push_back("denial acquired");
    for (const auto* event : {"aboutToClose", "close returned", "error query"}) events.push_back(event);
    if (denied) events.push_back("denial released");
    CHECK(output.seen.events == events);
    CHECK_EQ(output.projectAfter, output.seen.projectAtError);
    CHECK_EQ(output.sideAfter, output.expectedSide);
    CHECK(output.bwz.find("WZD_BOOLEAN_CUTTER_OBJECT_NO") != std::string::npos);
    CHECK(output.ini.find("WZD_MACHDIST_OPERATION_BOOLEAN") != std::string::npos);
    CHECK(output.mst.find("END OF MASTER FILE") != std::string::npos);
    CHECK(output.remesh.find("KEYWORD_DRIVEN") != std::string::npos);
    CHECK(output.remesh.find("INTERNAL_ELEMENT_SIZE_CONTROL_METHOD 4") != std::string::npos);
    CHECK(output.remesh.find("FILTER_DISTANCE 7") != std::string::npos);
}

} // namespace

TEST_CASE(save_read_denied_object_blob_omits_data_but_serializes_parent_and_continues)
{
    const Run control = run(false);
    const Run denied = run(true);
    checkSave(control, false);
    checkSave(denied, true);
    CHECK_EQ(control.expectedSide, denied.expectedSide);
    CHECK_EQ(control.bwz, denied.bwz);
    CHECK_EQ(control.ini, denied.ini);
    CHECK_EQ(control.mst, denied.mst);
    CHECK_EQ(control.remesh, denied.remesh);

    xml::Document controlDoc, deniedDoc;
    REQUIRE(xml::setContent(controlDoc, control.projectAfter, nullptr, nullptr, nullptr));
    REQUIRE(xml::setContent(deniedDoc, denied.projectAfter, nullptr, nullptr, nullptr));
    const auto controlObject = selectedObject(controlDoc);
    const auto deniedObject = selectedObject(deniedDoc);
    const auto controlKey = uniqueChild(*controlObject, "ObjPropertiesKey");
    const auto deniedKey = uniqueChild(*deniedObject, "ObjPropertiesKey");
    const auto controlData = uniqueChild(*controlKey, "Data");
    std::string error;
    CHECK_EQ(qt::qUncompress(qt::fromHex(controlData->text().utf8()), &error), control.expectedSide);
    CHECK(error.empty());
    CHECK(deniedKey->children.empty());
    CHECK_EQ(serializedNode(deniedKey), std::string("<ObjPropertiesKey/>\n"));
    auto& children = controlKey->children;
    children.erase(std::remove(children.begin(), children.end(), controlData), children.end());
    CHECK_EQ(serializedNode(controlObject), serializedNode(deniedObject));
}

int main() { return testkit::run_all(); }
