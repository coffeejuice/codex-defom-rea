#include "../support/test_paths.hpp"
// ctest-labels: m2m-moproj
// The original blob helper gates Data creation on QFile(ReadOnly).open, not
// file existence or nonempty bytes. No GUI or complete Save is simulated.
#include <testkit/check.hpp>

#include <filesystem>
#include <fstream>
#include <string>

#include "m2m/blob.hpp"
#include "m2m/engine.hpp"
#include "../../src/moproj/write_internal.hpp"

namespace fs = std::filesystem;
using namespace m2m;

namespace {

fs::path freshFile()
{
    static unsigned serial = 0;
    const auto dir = m2m_test::scratchOutput(m2m_test::scratchRoot() /
        ("project-blob-" + std::to_string(GetCurrentProcessId()) + "-" +
         std::to_string(GetTickCount64()) + "-" + std::to_string(++serial)));
    REQUIRE(!fs::exists(dir));
    fs::create_directories(dir);
    return dir / "side.KEY";
}

void writeFile(const fs::path& path, const std::string& bytes)
{
    std::ofstream file(path, std::ios::binary);
    REQUIRE(file.is_open());
    file.write(bytes.data(), static_cast<std::streamsize>(bytes.size()));
    REQUIRE(file.good());
}

struct DenyReaders {
    HANDLE handle;
    explicit DenyReaders(const fs::path& path)
        : handle(CreateFileW(path.c_str(), GENERIC_READ | GENERIC_WRITE, 0,
                             nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr))
    {
        REQUIRE(handle != INVALID_HANDLE_VALUE);
    }
    ~DenyReaders() { if (handle != INVALID_HANDLE_VALUE) CloseHandle(handle); }
    DenyReaders(const DenyReaders&) = delete;
    DenyReaders& operator=(const DenyReaders&) = delete;
};

std::string blobBytes(const xml::NodePtr& node)
{
    REQUIRE(node);
    CHECK(node->tag == qt::L("Data"));
    std::string error;
    const auto bytes = qt::qUncompress(qt::fromHex(node->text().utf8()), &error);
    CHECK(error.empty());
    return bytes;
}

} // namespace

TEST_CASE(project_blob_missing_and_readable_empty_have_distinct_dom_results)
{
    const auto path = freshFile();
    const qt::String name(path.u16string());
    Project project;
    CHECK(!moproj::blobElement(project, {}, name));
    writeFile(path, "");
    const auto empty = moproj::blobElement(project, {}, name);
    REQUIRE(empty);
    CHECK(empty->tag == qt::L("Data"));
    CHECK_EQ(empty->text().utf8(), std::string("00000000"));
    const std::string payload("a\0\r\nb", 5);
    writeFile(path, payload);
    CHECK_EQ(blobBytes(moproj::blobElement(project, {}, name)), payload);
}

TEST_CASE(project_blob_existing_but_read_denied_omits_data_until_unlocked)
{
    const auto path = freshFile();
    const qt::String name(path.u16string());
    const std::string payload("retained\0KEY\r\n", 14);
    writeFile(path, payload);
    Project project;
    {
        DenyReaders denied(path);
        REQUIRE(qt::fileExists(name)); // existence alone must not create Data
        std::ifstream probe(path, std::ios::binary);
        REQUIRE(!probe.is_open()); // failed setup is a failure, never a skipped assertion
        std::string bytes;
        CHECK(!moproj::readProjectFile(project, {}, name, bytes));
        CHECK(!moproj::blobElement(project, {}, name));
    }
    CHECK_EQ(blobBytes(moproj::blobElement(project, {}, name)), payload);
}

TEST_CASE(project_blob_virtual_effects_keep_precedence_over_unreadable_disk)
{
    const auto path = freshFile();
    const qt::String name(path.u16string());
    writeFile(path, "physical");
    Project project;
    DenyReaders denied(path);
    REQUIRE(qt::fileExists(name));
    project.overlay[qt::cleanPath(name)] = "";
    const auto emptyOverlay = moproj::blobElement(project, {}, name);
    REQUIRE(emptyOverlay);
    CHECK_EQ(emptyOverlay->text().utf8(), std::string("00000000"));
    engine::FileEffects earlier;
    earlier.writes.push_back({name, "earlier virtual bytes"});
    CHECK_EQ(blobBytes(moproj::blobElement(project, {&earlier}, name)),
             std::string("earlier virtual bytes"));
    engine::FileEffects latest;
    latest.deletes.push_back(name);
    CHECK(!moproj::blobElement(project, {&earlier, &latest}, name));
}

int main() { return testkit::run_all(); }
