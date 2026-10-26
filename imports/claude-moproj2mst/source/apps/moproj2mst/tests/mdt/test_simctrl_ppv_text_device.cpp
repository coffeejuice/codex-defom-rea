// ctest-labels: m2m-mdt
#include "../support/test_paths.hpp"
#include <testkit/check.hpp>

#include "m2m/engine/simctrl_ppv_text_device.hpp"

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>
#include <string_view>
#include <vector>

using namespace m2m;
using namespace m2m::engine::simctrl_ppv;

namespace {

struct FaultDevice final : mst::OutputDevice {
    bool isOpen = true;
    int failWriteAt = -1;
    int failFlushAt = -1;
    int writes = 0;
    int flushes = 0;
    std::vector<std::string> events;
    std::string accepted;

    std::int64_t write(std::string_view bytes) override
    {
        events.push_back("write:" + std::string(bytes));
        const int index = writes++;
        if (index == failWriteAt) return -1;
        accepted.append(bytes);
        return static_cast<std::int64_t>(bytes.size());
    }

    bool flush() override
    {
        events.push_back("flush");
        const int index = flushes++;
        return index != failFlushAt;
    }

    void close() override
    {
        if (!isOpen) return;
        events.push_back("close.flush");
        (void)flush();
        events.push_back("close.about_to_close");
        notifyAboutToClose();
        isOpen = false;
        events.push_back("close.done");
    }

    bool opened() const override { return isOpen; }
};

} // namespace

TEST_CASE(simctrl_ppv_text_device_writes_each_line_with_crlf_and_closes_after_flushes)
{
    FaultDevice file;
    const std::vector<std::string> lines{"1", "0\tfalse\t2\t3\t4.5\t6.5\t0"};

    const auto result = writePpvTextSegments(file, 1252, lines);
    CHECK(result.openAttempted);
    CHECK(result.opened);
    CHECK(result.writerReturned);
    CHECK(!result.streamFailed);
    CHECK_EQ(file.accepted, std::string("1\r\n0\tfalse\t2\t3\t4.5\t6.5\t0\r\n"));
    const std::vector<std::string> expected{
        "write:1", "write:\r\n", "flush",
        "write:0\tfalse\t2\t3\t4.5\t6.5\t0", "write:\r\n", "flush",
        "close.flush", "flush", "close.about_to_close", "close.done",
    };
    CHECK(file.events == expected);
}

TEST_CASE(simctrl_ppv_text_device_open_failure_skips_stream_and_close)
{
    FaultDevice file;
    file.isOpen = false;
    const std::vector<std::string> lines{"1", "row"};

    const auto result = writePpvTextSegments(file, 1252, lines);
    CHECK(result.openAttempted);
    CHECK(!result.opened);
    CHECK(!result.writerReturned);
    CHECK(!result.streamFailed);
    CHECK(file.events.empty());
}

TEST_CASE(simctrl_ppv_text_device_file_adapter_opens_truncates_and_writes_text_lines)
{
    namespace fs = std::filesystem;
    const fs::path root = m2m_test::scratchOutput(m2m_test::scratchRoot() /
        ("ppv-text-device-" + std::to_string(GetCurrentProcessId()) + "-" +
         std::to_string(GetTickCount64())));
    const fs::path path = root / "Operations" / "Task00001" / "SimCtrl.PPV";
    fs::create_directories(path.parent_path());
    {
        std::ofstream existing(path, std::ios::binary);
        REQUIRE(existing.is_open());
        existing << "stale bytes";
    }
    const std::vector<std::string> lines{"1", "row"};

    const auto result = writePpvTextFile(qt::String(path.u16string()), 1252, lines);
    CHECK(result.openAttempted);
    CHECK(result.opened);
    CHECK(result.writerReturned);
    CHECK(!result.streamFailed);
    std::ifstream in(path, std::ios::binary);
    const std::string bytes{std::istreambuf_iterator<char>(in),
                            std::istreambuf_iterator<char>()};
    CHECK_EQ(bytes, std::string("1\r\nrow\r\n"));
    in.close();
    fs::remove_all(root);
}

TEST_CASE(simctrl_ppv_text_device_post_open_write_failure_sticks_but_native_return_is_true)
{
    FaultDevice file;
    file.failWriteAt = 0;
    const std::vector<std::string> lines{"count", "later-row"};

    const auto result = writePpvTextSegments(file, 1252, lines);
    CHECK(result.opened);
    CHECK(result.writerReturned); // Native checks open, not later stream/file status.
    CHECK(result.streamFailed);
    const std::vector<std::string> expected{
        "write:count", "close.flush", "flush", "close.about_to_close", "close.done",
    };
    CHECK(file.events == expected);
    CHECK_EQ(file.writes, 1); // Sticky QTextStream failure suppresses later rows.
}

TEST_CASE(simctrl_ppv_text_device_failed_crlf_write_sticks_but_native_return_is_true)
{
    FaultDevice file;
    file.failWriteAt = 1; // First line text accepted; the CRLF write then fails.
    const std::vector<std::string> lines{"first", "second"};

    const auto result = writePpvTextSegments(file, 1252, lines);
    CHECK(result.opened);
    CHECK(result.writerReturned);
    CHECK(result.streamFailed);
    const std::vector<std::string> expected{
        "write:first", "write:\r\n", "flush",
        "close.flush", "flush", "close.about_to_close", "close.done",
    };
    CHECK(file.events == expected);
    CHECK_EQ(file.writes, 2); // The second source line is not attempted.
}

TEST_CASE(simctrl_ppv_text_device_flush_failure_sticks_but_native_return_is_true)
{
    FaultDevice file;
    file.failFlushAt = 0;
    const std::vector<std::string> lines{"count", "later"};

    const auto result = writePpvTextSegments(file, 1252, lines);
    CHECK(result.opened);
    CHECK(result.writerReturned);
    CHECK(result.streamFailed);
    const std::vector<std::string> expected{
        "write:count", "write:\r\n", "flush",
        "close.flush", "flush", "close.about_to_close", "close.done",
    };
    CHECK(file.events == expected);
    CHECK_EQ(file.writes, 2);
}

int main() { return testkit::run_all(); }
