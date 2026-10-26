// ctest-labels: m2m-mdt
#include <testkit/check.hpp>
#include "../support/test_paths.hpp"
#include "m2m/engine/simcontrol300_remesh.hpp"
#include "m2m/engine/conversion_session.hpp"
#include "m2m/model.hpp"
#include "m2m/qt4path.hpp"
#include <bit>
#include <cerrno>
#include <cmath>
#include <fstream>
#include <memory>
#include <process.h>
#include <string>
#include <utility>

using namespace m2m;
using namespace m2m::engine::simcontrol300;
namespace {
struct MemoryIo final : RemeshIo {
    std::string bytes;
    int closes = 0, errnoAtClose = -1;
    explicit MemoryIo(std::string input) : bytes(std::move(input)) {}
    struct File final : RemeshReadFile {
        MemoryIo& io;
        datafiles::TextFile text;
        bool closed = false;
        explicit File(MemoryIo& owner) : io(owner), text(owner.bytes) {}
        datafiles::TextFile& tokens() override { return text; }
        void rewind() override { text = datafiles::TextFile(io.bytes); }
        void close() override {
            const int error = errno;
            CHECK(!closed); closed = true;
            io.errnoAtClose = error; ++io.closes;
        }
        ~File() override { CHECK(closed); }
    };
    std::unique_ptr<RemeshReadFile> openRead(std::string_view name) override {
        if (name == "LOCAL_REMESH.DAT") return {};
        REQUIRE(name == "LOC_DEN_CTRL.DAT");
        return std::make_unique<File>(*this);
    }
};
struct OpenScratch {
    qt::CurrentDirScope restoreDirectory{qt::currentPath()};
    std::filesystem::path directory, project;
    explicit OpenScratch(const std::string& token) {
        static unsigned sequence = 0;
        directory = m2m_test::scratchOutput(m2m_test::scratchRoot() /
            ("r99r-" + std::to_string(_getpid()) + "-" + std::to_string(++sequence)));
        std::filesystem::create_directories(directory);
        project = directory / "project.moproj";
        std::ofstream(project, std::ios::binary) << "<Project><Unit Name=\"SI\"/></Project>";
        std::ofstream(directory / "LOC_DEN_CTRL.DAT", std::ios::binary)
            << "FILTER_DISTANCE " << token
            << "\nINTERNAL_ELEMENT_SIZE_CONTROL_METHOD 4\nDONE\n";
    }
    bool open(engine::ConversionSession& session) {
        auto& projectOwner = session.project();
        initProject(projectOwner, qt::String(directory.u16string()), qt::L("project"));
        Ctx context; context.session = &session;
        return openProject(projectOwner, qt::String(project.u16string()), context);
    }
};
}

TEST_CASE(remesh_canonical_huge_positive_exponents_return_signed_infinity_and_range_errno)
{
    for (const std::string token : {"1e5201", "-1e5201", "+9E9999", "-9e+9999",
        "2e00005201", "+3D9999", "-4d+05201",
        "1e99999999999999999999999999999999999999999999999999"}) {
        MemoryIo io("FILTER_DISTANCE " + token + "\nDONE\n");
        RemeshState state;
        errno = EDOM;
        const auto report = loadRemeshAtProjectOpen(state, io, NumericLocale::KnownC);
        CHECK(report.outcome == ApplyResult::Completed); REQUIRE(report.loaded); CHECK(*report.loaded);
        CHECK(state.enabled);
        const std::uint64_t expected = token[0] == '-' ? 0xfff0000000000000ULL : 0x7ff0000000000000ULL;
        CHECK_EQ(std::bit_cast<std::uint64_t>(state.filterDistance), expected);
        CHECK_EQ(io.closes, 1); CHECK_EQ(io.errnoAtClose, 0x22);
    }
}

TEST_CASE(remesh_canonical_real_overflow_reaches_later_records_and_scaling_clamps)
{
    for (const std::string token : {"1e9999", "-1e9999"}) {
        MemoryIo io("FILTER_DISTANCE " + token +
            "\nSCALING_FACTOR_EACH_LAYER_INWARD " + token +
            "\nINTERNAL_ELEMENT_SIZE_CONTROL_METHOD 4\nGOOD_INTERNAL_ELEMENT_CHANGEABLE 0\nDONE\n");
        RemeshState state;
        const auto report = loadRemeshAtProjectOpen(state, io, NumericLocale::KnownC);
        CHECK(report.outcome == ApplyResult::Completed); REQUIRE(report.loaded); CHECK(*report.loaded);
        CHECK(std::isinf(state.filterDistance));
        CHECK_EQ(state.scalingFactor, token[0] == '-' ? 1.0 : 10.0);
        CHECK_EQ(state.method, 4); CHECK(!state.goodInternalChangeable); CHECK(state.enabled);
        CHECK_EQ(io.closes, 1); CHECK_EQ(io.errnoAtClose, ERANGE);
    }
}

TEST_CASE(remesh_real_overflow_admission_keeps_other_range_and_spelling_guards)
{
    for (const std::string token : {"1e5200", "1.5e9999", "10e9999", "1e9999x",
        "1.5e-9999", "-1.5e-9999", "INF", "-INF", "NaN", "1.5e9999"}) {
        MemoryIo io("INTERNAL_ELEMENT_SIZE_CONTROL_METHOD 3\nFILTER_DISTANCE " + token + "\nDONE\n");
        RemeshState state;
        const auto report = loadRemeshAtProjectOpen(state, io, NumericLocale::KnownC);
        CHECK(report.outcome == ApplyResult::UnsupportedSnapshot); CHECK(!report.loaded);
        CHECK_EQ(state.method, 3); CHECK_EQ(state.filterDistance, 0.0); CHECK(!state.enabled);
        CHECK_EQ(io.closes, 1);
    }
}

TEST_CASE(remesh_real_overflow_does_not_replace_existing_finite_decimal_conversion)
{
    for (const std::string token : {"1e2", "-2E+1", "3D1", "0e9999"}) {
        MemoryIo io("FILTER_DISTANCE " + token + "\nDONE\n");
        RemeshState state;
        const auto report = loadRemeshAtProjectOpen(state, io, NumericLocale::KnownC);
        CHECK(report.outcome == ApplyResult::Completed); REQUIRE(report.loaded); CHECK(*report.loaded);
        const double expected = token == "1e2" ? 100.0 : token == "-2E+1" ? -20.0 : token == "3D1" ? 30.0 : 0.0;
        CHECK_EQ(state.filterDistance, expected); CHECK(state.enabled); CHECK_EQ(io.closes, 1);
    }
}

TEST_CASE(actual_session_open_consumes_canonical_real_overflow_and_later_setting)
{
    for (const std::string token : {"1e9999", "-1e9999"}) {
        OpenScratch disk(token);
        engine::DiagramEffectDriver driver; engine::ConversionSession session(driver);
        auto* owner = &session.remeshState();
        CHECK(disk.open(session)); CHECK(session.project().openedOk);
        CHECK(&session.remeshState() == owner); CHECK(owner->enabled); CHECK_EQ(owner->method, 4);
        const std::uint64_t expected = token[0] == '-' ? 0xfff0000000000000ULL : 0x7ff0000000000000ULL;
        CHECK_EQ(std::bit_cast<std::uint64_t>(owner->filterDistance), expected);
        CHECK(session.editor300FactoryAttempt() == nullptr);
    }
}

int main() { return testkit::run_all(); }
