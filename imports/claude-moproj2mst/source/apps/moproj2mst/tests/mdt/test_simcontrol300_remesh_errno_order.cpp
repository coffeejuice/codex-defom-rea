// ctest-labels: m2m-mdt
#include <testkit/check.hpp>
#include "m2m/engine/simcontrol300_remesh.hpp"
#include "m2m/engine/conversion_session.hpp"
#include <cerrno>
#include <memory>
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
}

TEST_CASE(remesh_finite_scalar_success_preserves_entry_errno_across_records)
{
    for (int previous : {0, EDOM, ERANGE}) {
        MemoryIo io("FILTER_DISTANCE 7.5\nSCALING_FACTOR_EACH_LAYER_INWARD 2.25\n"
            "INTERNAL_ELEMENT_SIZE_CONTROL_METHOD 4\nDONE\n");
        RemeshState state;
        errno = previous;
        const auto report = loadRemeshAtProjectOpen(state, io, NumericLocale::KnownC);
        CHECK(report.outcome == ApplyResult::Completed); REQUIRE(report.loaded); CHECK(*report.loaded);
        CHECK_EQ(state.filterDistance, 7.5); CHECK_EQ(state.scalingFactor, 2.25);
        CHECK_EQ(state.method, 4); CHECK(state.enabled);
        CHECK_EQ(io.errnoAtClose, previous); CHECK_EQ(io.closes, 1);
    }
}

TEST_CASE(remesh_integer_range_errno_survives_later_finite_scalar_stores)
{
    MemoryIo io("INTERNAL_ELEMENT_SIZE_CONTROL_METHOD 2147483648\n"
        "FILTER_DISTANCE 7.5\nSCALING_FACTOR_EACH_LAYER_INWARD 2.25\nDONE\n");
    RemeshState state;
    errno = EDOM;
    const auto report = loadRemeshAtProjectOpen(state, io, NumericLocale::KnownC);
    CHECK(report.outcome == ApplyResult::Completed); REQUIRE(report.loaded); CHECK(*report.loaded);
    CHECK_EQ(state.method, 5); CHECK_EQ(state.filterDistance, 7.5);
    CHECK_EQ(state.scalingFactor, 2.25); CHECK(state.enabled);
    CHECK_EQ(io.errnoAtClose, ERANGE); CHECK_EQ(io.closes, 1);
}

TEST_CASE(remesh_existing_no_digits_zero_success_preserves_prior_errno)
{
    MemoryIo io("FILTER_DISTANCE invalid\nSCALING_FACTOR_EACH_LAYER_INWARD invalid\nDONE\n");
    RemeshState state; state.filterDistance = 9.5;
    errno = EDOM;
    const auto report = loadRemeshAtProjectOpen(state, io, NumericLocale::KnownC);
    CHECK(report.outcome == ApplyResult::Completed); REQUIRE(report.loaded); CHECK(*report.loaded);
    CHECK_EQ(state.filterDistance, 0.0); CHECK_EQ(state.scalingFactor, 1.0); CHECK(state.enabled);
    CHECK_EQ(io.errnoAtClose, EDOM); CHECK_EQ(io.closes, 1);
}

TEST_CASE(remesh_current_conversion_range_error_is_not_hidden_by_restored_prior_errno)
{
    for (const std::string token : {"1.5e9999", "1.5e-9999"}) {
        MemoryIo io("INTERNAL_ELEMENT_SIZE_CONTROL_METHOD 3\nFILTER_DISTANCE " + token + "\nDONE\n");
        RemeshState state; state.filterDistance = 9.5;
        errno = EDOM;
        const auto report = loadRemeshAtProjectOpen(state, io, NumericLocale::KnownC);
        CHECK(report.outcome == ApplyResult::UnsupportedSnapshot); CHECK(!report.loaded);
        CHECK_EQ(state.method, 3); CHECK_EQ(state.filterDistance, 9.5); CHECK(!state.enabled);
        CHECK_EQ(io.errnoAtClose, ERANGE); CHECK_EQ(io.closes, 1);
    }
}

TEST_CASE(actual_session_remesh_dispatcher_preserves_range_errno_through_finite_fields)
{
    engine::DiagramEffectDriver driver; engine::ConversionSession session(driver);
    auto* owner = &session.remeshState();
    MemoryIo io("INTERNAL_ELEMENT_SIZE_CONTROL_METHOD -2147483649\n"
        "FILTER_DISTANCE 7.5\nSCALING_FACTOR_EACH_LAYER_INWARD 2.25\nDONE\n");
    errno = EDOM;
    session.loadProjectRemesh(io);
    const int returnedErrno = errno; // before unrelated operations or outer API work
    CHECK(&session.remeshState() == owner); CHECK(owner->enabled); CHECK_EQ(owner->method, 0);
    CHECK_EQ(owner->filterDistance, 7.5); CHECK_EQ(owner->scalingFactor, 2.25);
    CHECK_EQ(io.errnoAtClose, ERANGE); CHECK_EQ(returnedErrno, ERANGE); CHECK_EQ(io.closes, 1);
    CHECK(session.editor300FactoryAttempt() == nullptr);
}

int main() { return testkit::run_all(); }
