// ctest-labels: m2m-mdt
#include "../support/test_paths.hpp"
#include <testkit/check.hpp>

#include "m2m/engine/simctrl_ppv_file_reader.hpp"
#include "m2m/qt4path.hpp"

#include <filesystem>
#include <fstream>
#include <string>

using namespace m2m;
using namespace m2m::engine::simctrl_ppv;

TEST_CASE(simctrl_ppv_file_reader_reads_ascii_lf_and_missing_file)
{
    Project project;
    const auto path = qt::L("C:/scratch/SimCtrl.PPV");
    project.overlay.emplace(qt::cleanPath(path), "1\n0 true 2 3 4.5 6.5\n");

    const FileLines read = readPpvFileLines(project, path);
    REQUIRE(read.status == FileReadStatus::Read);
    REQUIRE(read.lines.size() == 2u);
    CHECK(read.lines[0] == qt::L("1"));
    CHECK(read.lines[1] == qt::L("0 true 2 3 4.5 6.5"));

    const auto missingPath = std::filesystem::current_path() /
        "m2m-ppv-reader-absent" / "missing.PPV";
    REQUIRE(!std::filesystem::exists(missingPath));
    const FileLines missing = readPpvFileLines(
        project, qt::String::latin1(missingPath.generic_string().c_str()));
    CHECK(missing.status == FileReadStatus::OpenFailed);
    CHECK(missing.lines.empty());
}

TEST_CASE(simctrl_ppv_file_reader_detects_unicode_boms_and_strips_crlf)
{
    Project project;
    std::string utf16le("\xff\xfe", 2);
    for (const char16_t ch : std::u16string(u"1\r\n0 true 2 3 4.5 6.5\r\n")) {
        utf16le.push_back(static_cast<char>(ch & 0xff));
        utf16le.push_back(static_cast<char>((ch >> 8) & 0xff));
    }
    project.overlay.emplace(qt::cleanPath(qt::L("C:/scratch/utf16.PPV")), utf16le);
    project.overlay.emplace(qt::cleanPath(qt::L("C:/scratch/utf8-bom-ascii.PPV")),
                             std::string("\xef\xbb\xbf" "1\n0 true 2 3 4.5 6.5\n"));
    project.overlay.emplace(qt::cleanPath(qt::L("C:/scratch/ascii-crlf.PPV")), "1\r\n0 true\r\n");

    const FileLines utf16 = readPpvFileLines(project, qt::L("C:/scratch/utf16.PPV"));
    REQUIRE(utf16.status == FileReadStatus::Read);
    REQUIRE(utf16.lines.size() == 2u);
    CHECK(utf16.lines[0] == qt::L("1"));
    CHECK(utf16.lines[1] == qt::L("0 true 2 3 4.5 6.5"));

    const FileLines ascii = readPpvFileLines(project, qt::L("C:/scratch/ascii-crlf.PPV"));
    REQUIRE(ascii.status == FileReadStatus::Read);
    REQUIRE(ascii.lines.size() == 2u);
    CHECK(ascii.lines[0] == qt::L("1"));
    CHECK(ascii.lines[1] == qt::L("0 true"));

    const FileLines utf8BomAscii = readPpvFileLines(
        project, qt::L("C:/scratch/utf8-bom-ascii.PPV"));
    REQUIRE(utf8BomAscii.status == FileReadStatus::Read);
    REQUIRE(utf8BomAscii.lines.size() == 2u);
    CHECK(utf8BomAscii.lines[0] == qt::L("1"));
    CHECK(utf8BomAscii.lines[1] == qt::L("0 true 2 3 4.5 6.5"));
}

TEST_CASE(simctrl_ppv_file_reader_guards_unqualified_encoding_profiles)
{
    Project project;
    project.overlay.emplace(qt::cleanPath(qt::L("C:/scratch/utf8-bom-nonascii.PPV")),
                             std::string("\xef\xbb\xbf" "caf\xc3\xa9\n"));
    project.overlay.emplace(qt::cleanPath(qt::L("C:/scratch/utf8-bom-malformed.PPV")),
                             std::string("\xef\xbb\xbf" "\xe2(\xa1\n"));
    project.overlay.emplace(qt::cleanPath(qt::L("C:/scratch/bomless-nonascii.PPV")),
                             std::string("\xc3\xa9\n"));
    project.overlay.emplace(qt::cleanPath(qt::L("C:/scratch/odd.PPV")),
                             std::string("\xff\xfe" "1", 3));
    project.overlay.emplace(qt::cleanPath(qt::L("C:/scratch/surrogate.PPV")),
                             std::string("\xff\xfe\x00\xd8", 4));

    for (const auto* name : {"utf8-bom-nonascii.PPV", "utf8-bom-malformed.PPV",
                             "bomless-nonascii.PPV", "odd.PPV", "surrogate.PPV"}) {
        const auto path = qt::String::latin1((std::string("C:/scratch/") + name).c_str());
        const FileLines guarded = readPpvFileLines(project, path);
        CHECK(guarded.status == FileReadStatus::UnsupportedEncodingOrLineEnding);
        CHECK(guarded.lines.empty());
    }
}

TEST_CASE(simctrl_ppv_physical_reader_distinguishes_open_failure_empty_file_and_embedded_cr)
{
    namespace fs = std::filesystem;
    const fs::path root = m2m_test::scratchOutput(m2m_test::scratchRoot() /
        ("ppv-read-outcomes-" + std::to_string(GetCurrentProcessId()) + "-" +
         std::to_string(GetTickCount64())));
    const fs::path emptyPath = root / "empty.PPV";
    const fs::path embeddedPath = root / "embedded-cr.PPV";
    const fs::path missingPath = root / "missing" / "SimCtrl.PPV";
    fs::create_directories(root);
    {
        std::ofstream empty(emptyPath, std::ios::binary);
        REQUIRE(empty.is_open());
    }
    {
        std::ofstream embedded(embeddedPath, std::ios::binary);
        REQUIRE(embedded.is_open());
        embedded << "1\rinside\n";
    }
    REQUIRE(!fs::exists(missingPath));

    const FileLines failed = readPpvFileLinesFromDisk(qt::String(missingPath.u16string()));
    CHECK(failed.status == FileReadStatus::OpenFailed);
    CHECK(failed.lines.empty());

    const FileLines empty = readPpvFileLinesFromDisk(qt::String(emptyPath.u16string()));
    CHECK(empty.status == FileReadStatus::Read);
    CHECK(empty.lines.empty()); // Successful empty file is distinct from open failure.

    const FileLines embedded = readPpvFileLinesFromDisk(qt::String(embeddedPath.u16string()));
    REQUIRE(embedded.status == FileReadStatus::Read);
    REQUIRE(embedded.lines.size() == 1u);
    CHECK(embedded.lines[0] == qt::String(u"1\rinside"));
    fs::remove_all(root);
}

TEST_CASE(simctrl_ppv_disk_reader_bypasses_overlay_on_physical_collision)
{
    namespace fs = std::filesystem;
    const fs::path root = m2m_test::scratchOutput(m2m_test::scratchRoot() /
        ("ppv-disk-reader-" + std::to_string(GetCurrentProcessId()) + "-" +
         std::to_string(GetTickCount64())));
    const fs::path diskPath = root / "Operations" / "Task00001" / "SimCtrl.PPV";
    fs::create_directories(diskPath.parent_path());
    {
        std::ofstream disk(diskPath, std::ios::binary);
        REQUIRE(disk.is_open());
        disk << "1\n0 false 7 8 9 10\n";
    }

    Project project;
    const qt::String path(diskPath.u16string());
    project.overlay.emplace(qt::cleanPath(path), "1\n0 false 2 3 4 5\n");

    const FileLines physical = readPpvFileLinesFromDisk(path);
    REQUIRE(physical.status == FileReadStatus::Read);
    REQUIRE(physical.lines.size() == 2u);
    CHECK(physical.lines[1] == qt::L("0 false 7 8 9 10"));

    const FileLines virtualized = readPpvFileLines(project, path);
    REQUIRE(virtualized.status == FileReadStatus::Read);
    REQUIRE(virtualized.lines.size() == 2u);
    CHECK(virtualized.lines[1] == qt::L("0 false 2 3 4 5"));
    CHECK_EQ(project.overlay.at(qt::cleanPath(path)), "1\n0 false 2 3 4 5\n");

    fs::remove_all(root);
}

int main() { return testkit::run_all(); }
