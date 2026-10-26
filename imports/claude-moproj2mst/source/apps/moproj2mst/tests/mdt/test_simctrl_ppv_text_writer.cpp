// ctest-labels: m2m-mdt
#include "../support/test_paths.hpp"
#include <testkit/check.hpp>

#include "m2m/engine/simctrl_ppv_text_writer.hpp"

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
    int writes = 0;
    std::vector<std::string> events;

    std::int64_t write(std::string_view bytes) override
    {
        events.push_back("write:" + std::string(bytes));
        return writes++ == failWriteAt ? -1 : static_cast<std::int64_t>(bytes.size());
    }
    bool flush() override { events.push_back("flush"); return true; }
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
}

TEST_CASE(simctrl_ppv_text_writer_physical_file_orders_shared_then_local_rows)
{
    namespace fs = std::filesystem;
    const fs::path root = m2m_test::scratchOutput(m2m_test::scratchRoot() /
        ("ppv-text-writer-" + std::to_string(GetCurrentProcessId()) + "-" +
         std::to_string(GetTickCount64())));
    const fs::path path = root / "Operations" / "Task00001" / "SimCtrl.PPV";
    fs::create_directories(path.parent_path());
    {
        std::ofstream prior(path, std::ios::binary);
        REQUIRE(prior.is_open());
        prior << "old";
    }
    const std::vector<Point> local{{4, 5, 1.25, 2.5}};
    const std::vector<Point> shared{{7, 8, 9.5, 10.5}};
    const std::vector<bool> flags{true};

    const auto result = writePpvTextWriterFile(qt::String(path.u16string()), 1252,
                                                true, local, &shared, &flags);
    CHECK(result.status == TextWriterStatus::Returned);
    CHECK(result.openAttempted && result.opened && result.writerReturned);
    CHECK(!result.streamFailed);
    std::ifstream input(path, std::ios::binary);
    const std::string bytes{std::istreambuf_iterator<char>(input),
                            std::istreambuf_iterator<char>()};
    CHECK_EQ(bytes, std::string("2\r\n"
        "0\ttrue\t7\t8\t9.5\t10.5\t1\r\n"
        "0\tfalse\t4\t5\t1.25\t2.5\t0\r\n"));
    input.close();
    fs::remove_all(root);
}

TEST_CASE(simctrl_ppv_text_writer_local_empty_skips_path_codec_and_open)
{
    namespace fs = std::filesystem;
    const fs::path path = m2m_test::scratchOutput(m2m_test::scratchRoot() /
        ("ppv-text-writer-empty-" + std::to_string(GetCurrentProcessId()) + "-" +
         std::to_string(GetTickCount64())) / "SimCtrl.PPV");
    const std::vector<Point> local;
    const std::vector<Point> shared{{7, 8, 9.5, 10.5}};
    const auto result = writePpvTextWriterFile(qt::String(path.u16string()), 0,
                                                false, local, &shared, nullptr);
    CHECK(result.status == TextWriterStatus::LocalListEmpty);
    CHECK(!result.openAttempted);
    CHECK(!fs::exists(path));
}

TEST_CASE(simctrl_ppv_text_writer_open_failure_has_no_stream_return)
{
    namespace fs = std::filesystem;
    const fs::path path = m2m_test::scratchOutput(m2m_test::scratchRoot() /
        ("ppv-text-writer-no-parent-" + std::to_string(GetCurrentProcessId()) + "-" +
         std::to_string(GetTickCount64())) / "missing" / "SimCtrl.PPV");
    REQUIRE(!fs::exists(path.parent_path()));
    const std::vector<Point> local{{4, 5, 1.25, 2.5}};

    const auto result = writePpvTextWriterFile(qt::String(path.u16string()), 1252,
                                                true, local, nullptr, nullptr);
    CHECK(result.status == TextWriterStatus::OpenFailed);
    CHECK(result.openAttempted);
    CHECK(!result.opened);
    CHECK(!result.writerReturned);
}

TEST_CASE(simctrl_ppv_text_writer_returns_true_after_injected_post_open_write_failure)
{
    FaultDevice file;
    file.failWriteAt = 0;
    const std::vector<Point> local{{4, 5, 1.25, 2.5}};

    const auto result = writePpvTextWriterAfterOpen(file, 1252, local, nullptr, nullptr);
    CHECK(result.status == TextWriterStatus::Returned);
    CHECK(result.opened);
    CHECK(result.writerReturned);
    CHECK(result.streamFailed);
    const std::vector<std::string> expected{
        "write:1",
        "close.flush",
        "flush",
        "close.about_to_close",
        "close.done",
    };
    CHECK(file.events == expected);
}

int main() { return testkit::run_all(); }
