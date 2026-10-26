#include "../support/test_paths.hpp"
// Candidate-only public Save regression for a Cogging.MDT write-open denial after Open.
// ctest-labels: m2m-s
#include <testkit/check.hpp>
#include <fstream>
#include <functional>
#include <iterator>
#include <memory>
#include <string>
#include <string_view>
#include <windows.h>

#include "m2m/convert.hpp"
#include "m2m/convert_details.hpp"
#include "m2m/mst/file_device.hpp"
#include "../../src/core/conversion_call_services.hpp"
#include "../../src/moproj/project_output_device.hpp"

namespace fs = std::filesystem;
using namespace m2m;

namespace {
std::string read(const fs::path& path)
{
    std::ifstream input(path, std::ios::binary);
    return {std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
}

struct Handle {
    HANDLE value = INVALID_HANDLE_VALUE;
    explicit Handle(const fs::path& path)
    {
        // Existing-file reads remain allowed; subsequent CRT write access is denied.
        value = CreateFileW(path.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr,
                            OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
        REQUIRE(value != INVALID_HANDLE_VALUE);
    }
    ~Handle() { if (value != INVALID_HANDLE_VALUE) CloseHandle(value); }
    Handle(const Handle&) = delete;
};

struct Rig {
    fs::path root, project, cogging, bwz, ini, mst;
    Rig()
    {
        static unsigned long serial = 0;
        root = base_ / ("c105_" + std::to_string(GetCurrentProcessId()) + "_" +
                        std::to_string(GetTickCount64()) + "_" + std::to_string(++serial));
        REQUIRE(root.is_absolute() && root.parent_path() == base_);
        fs::create_directories(base_);
        REQUIRE(fs::create_directory(root));
        owned_ = true;
        try {
            fs::copy(m2m_test::configuredPath("M2M_ORACLE_B") / "133_B_DBSTEP_BOOL3_before",
                     root, fs::copy_options::recursive);
            for (const auto& entry : fs::recursive_directory_iterator(root))
                REQUIRE(entry.path().native().size() < 100);
            for (const auto& path : {root, root / "COG_P1_B1.moproj",
                     root / "Operations" / "Task00003" / "Cogging.MDT",
                     root / "Operations" / "Task00002" / "Boolean.BWZ",
                     root / "Operations" / "Task00002" / "DEF_MACHDIST_CTRL2.INI",
                     root / "native-output.MST"})
                REQUIRE(path.native().size() < 100);
        } catch (...) {
            cleanup();
            throw;
        }
        project = root / "COG_P1_B1.moproj";
        cogging = root / "Operations" / "Task00003" / "Cogging.MDT";
        bwz = root / "Operations" / "Task00002" / "Boolean.BWZ";
        ini = root / "Operations" / "Task00002" / "DEF_MACHDIST_CTRL2.INI";
        mst = root / "native-output.MST";
        for (const auto& path : {project, cogging, bwz, ini, mst})
            REQUIRE(path.native().size() < 100);
    }
    ~Rig() { cleanup(); }
private:
    bool bounded() const
    {
        return root.is_absolute() && root.parent_path() == base_ &&
            root.filename().string().starts_with("c105_");
    }
    void cleanup() noexcept
    {
        if (!owned_ || !bounded()) return;
        std::error_code ignored;
        fs::remove_all(root, ignored); // Only the uniquely-created C:/PROBLEM leaf.
        if (!ignored) owned_ = false;
    }
    const fs::path base_{"C:/PROBLEM"};
    bool owned_ = false;
};

struct Observation {
    std::function<void()> afterProjectOpen;
    int openCalls = 0;
    int closeCalls = 0;
    int errorCalls = 0;
    bool projectOpened = false;
};

class DelegatingOutput final : public moproj::detail::ProjectOutputDevice {
public:
    DelegatingOutput(std::unique_ptr<mst::FileDevice> file, Observation& observation)
        : file_(std::move(file)), observation_(observation)
    {
        file_->setAboutToClose([this] {
            if (observation_.afterProjectOpen) observation_.afterProjectOpen();
            notifyAboutToClose();
        });
    }
    ~DelegatingOutput() override
    {
        if (file_->opened()) close();
        file_->setAboutToClose({});
    }
    std::int64_t write(std::string_view bytes) override { return file_->write(bytes); }
    bool flush() override { return file_->flush(); }
    void close() override
    {
        if (!file_->opened()) return;
        file_->close();
        ++observation_.closeCalls;
    }
    bool opened() const override { return file_->opened(); }
    int error() const override
    {
        ++observation_.errorCalls;
        return file_->error();
    }
private:
    std::unique_ptr<mst::FileDevice> file_;
    Observation& observation_;
};

class OutputFactory final : public moproj::detail::ProjectOutputFactory {
public:
    explicit OutputFactory(Observation& observation) : observation_(observation) {}
    std::unique_ptr<moproj::detail::ProjectOutputDevice> open(const qt::String& path) override
    {
        ++observation_.openCalls;
        auto file = std::make_unique<mst::FileDevice>(path);
        observation_.projectOpened = file->opened();
        REQUIRE(observation_.projectOpened);
        return std::make_unique<DelegatingOutput>(std::move(file), observation_);
    }
private:
    Observation& observation_;
};
} // namespace

TEST_CASE(save_cogging_mdt_write_open_failure_keeps_prior_boolean_and_continues_to_mst)
{
    Rig files;
    REQUIRE(fs::exists(files.cogging));
    const std::string priorCogging = read(files.cogging);
    REQUIRE(!priorCogging.empty());
    REQUIRE(!fs::exists(files.bwz));
    REQUIRE(!fs::exists(files.ini));
    REQUIRE(!fs::exists(files.mst));

    Observation observation;
    std::unique_ptr<Handle> denyCoggingWrite;
    observation.afterProjectOpen = [&] {
        // This runs at the project writer's close notification, after conversion Open.
        // Keep the handle until the public Save returns so only Cogging's write-open fails.
        denyCoggingWrite = std::make_unique<Handle>(files.cogging);
    };
    OutputFactory factory(observation);
    detail::ConversionCallServices services{&factory};
    ConversionDetails details;
    const Result result = detail::convertDetailedWithServices(
        files.project.u16string(), files.mst.u16string(),
        M2M_FLAG_WRITE_SAVE, 1252, details, services);
    denyCoggingWrite.reset();

    CHECK_EQ(observation.openCalls, 1);
    CHECK(observation.projectOpened);
    CHECK_EQ(observation.closeCalls, 1);
    CHECK_EQ(observation.errorCalls, 1);
    CHECK_EQ(result.code, M2M_OK); // Candidate adapter result only; no original GUI status claim.
    CHECK_EQ(read(files.cogging), priorCogging);
    // Boolean3D is operation #2 in this fixture; Cogging is operation #3. These
    // are already-written effects, while the main MST write is the later continuation.
    CHECK(read(files.bwz).find("WZD_BOOLEAN_CUTTER_OBJECT_NO") != std::string::npos);
    CHECK(read(files.ini).find("WZD_MACHDIST_OPERATION_BOOLEAN") != std::string::npos);
    CHECK(read(files.mst).find("END OF MASTER FILE") != std::string::npos);
}





int main() { return testkit::run_all(); }


