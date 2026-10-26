// ctest-labels: m2m-mdt
#include <testkit/check.hpp>
#include "../support/test_paths.hpp"
#include "m2m/engine/simcontrol300_remesh.hpp"
#include "m2m/engine/conversion_session.hpp"
#include "m2m/model.hpp"
#include "m2m/qt4path.hpp"
#include <cerrno>
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
            ("r98o-" + std::to_string(_getpid()) + "-" + std::to_string(++sequence)));
        std::filesystem::create_directories(directory);
        project = directory / "project.moproj";
        std::ofstream(project, std::ios::binary) << "<Project><Unit Name=\"SI\"/></Project>";
        std::ofstream(directory / "LOC_DEN_CTRL.DAT", std::ios::binary)
            << "INTERNAL_ELEMENT_SIZE_CONTROL_METHOD " << token
            << "\nFILTER_DISTANCE 7.5\nDONE\n";
    }
    bool open(engine::ConversionSession& session) {
        auto& projectOwner = session.project();
        initProject(projectOwner, qt::String(directory.u16string()), qt::L("project"));
        Ctx context; context.session = &session;
        return openProject(projectOwner, qt::String(project.u16string()), context);
    }
};
}

TEST_CASE(remesh_decimal_overflow_saturates_then_clamps_and_sets_range_errno)
{
    struct Case { const char* token; int method; bool overflow; };
    const Case cases[]{
        {"2147483647", 5, false}, {"-2147483648", 0, false},
        {"2147483648", 5, true}, {"-2147483649", 0, true},
        {"+2147483648tail", 5, true}, {"-2147483649tail", 0, true},
        {"0002147483648", 5, true},
        {"999999999999999999999999999999999999", 5, true},
        {"-999999999999999999999999999999999999", 0, true}};
    CHECK_EQ(ERANGE, 0x22);
    for (const auto& item : cases) {
        MemoryIo io(std::string("INTERNAL_ELEMENT_SIZE_CONTROL_METHOD ") + item.token + "\nDONE\n");
        RemeshState state;
        errno = EDOM;
        const auto report = loadRemeshAtProjectOpen(state, io, NumericLocale::KnownC);
        CHECK(report.outcome == ApplyResult::Completed); REQUIRE(report.loaded);
        CHECK(*report.loaded); CHECK(state.enabled); CHECK_EQ(state.method, item.method);
        CHECK_EQ(io.closes, 1); CHECK_EQ(io.errnoAtClose, item.overflow ? ERANGE : EDOM);
    }
}

TEST_CASE(remesh_saturated_integer_boolean_and_later_scalar_records_continue)
{
    MemoryIo io("INTERNAL_ELEMENT_SIZE_CONTROL_METHOD 2147483648\n"
        "GOOD_INTERNAL_ELEMENT_CHANGEABLE -2147483649\n"
        "FURTHER_IMPROVE_CONTACT_ELEMENTS 999999999999999999999999\n"
        "GOOD_SURFACE_ELEMENT_CHANGEABLE -999999999999999999999999\n"
        "FILTER_DISTANCE 7.5\nSCALING_FACTOR_EACH_LAYER_INWARD 2.25\nDONE\n");
    RemeshState state;
    state.goodInternalChangeable = false; state.improveContact = 0;
    state.goodSurfaceChangeable = 0;
    const auto report = loadRemeshAtProjectOpen(state, io, NumericLocale::KnownC);
    CHECK(report.outcome == ApplyResult::Completed); REQUIRE(report.loaded); CHECK(*report.loaded);
    CHECK(state.enabled); CHECK_EQ(state.method, 5);
    CHECK(state.goodInternalChangeable); CHECK_EQ(state.improveContact, 1);
    CHECK_EQ(state.goodSurfaceChangeable, 1);
    CHECK_EQ(state.filterDistance, 7.5); CHECK_EQ(state.scalingFactor, 2.25);
    CHECK_EQ(io.closes, 1);
}

TEST_CASE(remesh_missing_integer_reuses_saturated_local_instead_of_clamped_method)
{
    for (const std::string token : {"2147483648", "-2147483649"}) {
        MemoryIo io("INTERNAL_ELEMENT_SIZE_CONTROL_METHOD " + token +
            "\nGOOD_INTERNAL_ELEMENT_CHANGEABLE");
        RemeshState state; state.goodInternalChangeable = false;
        const auto report = loadRemeshAtProjectOpen(state, io, NumericLocale::KnownC);
        CHECK(report.outcome == ApplyResult::Completed); REQUIRE(report.loaded); CHECK(*report.loaded);
        CHECK_EQ(state.method, token[0] == '-' ? 0 : 5);
        CHECK(state.goodInternalChangeable); // negative local is INT_MIN, not clamped0
        CHECK(state.enabled); CHECK_EQ(io.closes, 1); CHECK_EQ(io.errnoAtClose, ERANGE);
    }
}

TEST_CASE(remesh_later_unrecovered_real_keeps_saturated_method_prefix)
{
    MemoryIo io("INTERNAL_ELEMENT_SIZE_CONTROL_METHOD 2147483648\nFILTER_DISTANCE NaN\nDONE\n");
    RemeshState state;
    const auto report = loadRemeshAtProjectOpen(state, io, NumericLocale::KnownC);
    CHECK(report.outcome == ApplyResult::UnsupportedSnapshot); CHECK(!report.loaded);
    CHECK_EQ(state.method, 5); CHECK_EQ(state.filterDistance, 0.0); CHECK(!state.enabled);
    CHECK_EQ(io.closes, 1);
}

TEST_CASE(actual_session_open_consumes_overflow_remesh_and_later_field)
{
    for (const std::string token : {"2147483648", "-2147483649"}) {
        OpenScratch disk(token);
        engine::DiagramEffectDriver driver; engine::ConversionSession session(driver);
        auto* owner = &session.remeshState();
        CHECK(disk.open(session)); CHECK(session.project().openedOk);
        CHECK(&session.remeshState() == owner); CHECK(owner->enabled);
        CHECK_EQ(owner->method, token[0] == '-' ? 0 : 5);
        CHECK_EQ(owner->filterDistance, 7.5);
        CHECK(session.editor300FactoryAttempt() == nullptr); // remesh precedes editor creation
    }
}

int main() { return testkit::run_all(); }
