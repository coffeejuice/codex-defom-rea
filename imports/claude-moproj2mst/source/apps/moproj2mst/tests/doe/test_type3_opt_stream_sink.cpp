#include <testkit/check.hpp>
#include "../../src/ops/type3_opt_stream_sink.hpp"
#include <m2m/mst/doe_model.hpp>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iterator>

using m2m::mst::detail::Type3OptStreamSink;
namespace {
std::string pathUtf8(const std::filesystem::path& path)
{
    const auto value = path.u8string();
    return std::string(reinterpret_cast<const char*>(value.data()), value.size());
}
}

TEST_CASE(type3_opt_stream_sink_is_typed_and_refuses_opaque_fragments)
{
    const char* scratch = std::getenv("M2M_TEST_SCRATCH");
    REQUIRE(scratch != nullptr);
    const auto path = std::filesystem::path(scratch) / u"cp482-stream-sink-\u00e9.OPT";
    std::error_code ec;
    std::filesystem::create_directories(path.parent_path(), ec);

    Type3OptStreamSink sink(1252);
    REQUIRE(sink.open(pathUtf8(path)));
    CHECK(!sink.insert(std::string("untyped opaque fragment")));
    CHECK(sink.insertLiteral("I="));
    CHECK(sink.insertInt(12));
    CHECK(sink.insertLiteral(" D="));
    CHECK(sink.insertDouble(1.25));
    CHECK(sink.insertLiteral(" S="));
    CHECK(sink.insertString(m2m::qt::String(u"caf\u00e9")));
    CHECK(sink.insertEndl());
    CHECK(sink.insertLiteral("close-tail")); // intentionally pending until QFile aboutToClose
    CHECK(sink.close());
    CHECK(!sink.opened());
    CHECK(sink.close()); // idempotent after the device has closed
    std::ifstream input(path, std::ios::binary);
    const std::string bytes((std::istreambuf_iterator<char>(input)), {});
    CHECK(bytes == std::string("I=12 D=1.25 S=caf") + char(0xe9) + "\r\nclose-tail");
}

TEST_CASE(type3_opt_stream_sink_failed_open_records_path_without_stream)
{
    const char* scratch = std::getenv("M2M_TEST_SCRATCH");
    REQUIRE(scratch != nullptr);
    const auto path = std::filesystem::path(scratch) / u"cp482-missing-parent" / u"fail.OPT";
    Type3OptStreamSink sink(1252);
    CHECK(!sink.open(pathUtf8(path)));
    CHECK(!sink.opened());
    CHECK(sink.diagnostic() == pathUtf8(path));
    CHECK(!sink.insertLiteral("ignored"));
}

TEST_CASE(type3_opt_stream_sink_profile_refusal_does_not_truncate_destination)
{
    namespace fs = std::filesystem;
    const char* scratch = std::getenv("M2M_TEST_SCRATCH");
    REQUIRE(scratch != nullptr);
    const auto path = fs::path(scratch) / "cp482-refusal.OPT";
    { std::ofstream out(path, std::ios::binary | std::ios::trunc); out << "sentinel"; }
    m2m::mst::detail::Type3OptSavePrepared prepared;
    prepared.path = pathUtf8(path);
    m2m::mst::detail::Type3OptimizerScalarState state;
    struct EmptyManager : m2m::doe::Model {
        std::vector<m2m::doe::Var*> geometryVars(int, int) const override { return {}; }
        std::vector<m2m::doe::Var*> unlinkedVars() override { return {}; }
    } manager;
    m2m::mst::detail::Type3OptGroupReceipt receipt;
    Type3OptStreamSink sink(1252);
    const auto result = m2m::mst::detail::runType3OptStreamProfile(
        m2m::mst::detail::Type3OptStreamProfile::OpaquePrepared,
        prepared, state, manager, sink, receipt);
    CHECK(!result.admitted);
    CHECK(!sink.opened());
    std::ifstream input(path, std::ios::binary);
    const std::string bytes((std::istreambuf_iterator<char>(input)), {});
    CHECK(bytes == "sentinel");
}

TEST_CASE(type3_opt_stream_sink_typed_manager_wrapper_admits_and_skips_opaque_fragments)
{
    namespace fs = std::filesystem;
    const char* scratch = std::getenv("M2M_TEST_SCRATCH");
    REQUIRE(scratch != nullptr);
    const auto path = fs::path(scratch) / "cp482-typed-profile.OPT";
    m2m::mst::detail::Type3OptSavePrepared prepared;
    prepared.path = pathUtf8(path);
    prepared.krgMax = 2;
    prepared.oprNo = 3;
    prepared.scalarPrefix = {"opaque-prefix-must-not-appear"};
    prepared.scalarSuffix = {"opaque-suffix-must-not-appear"};
    prepared.rows = {{"opaque-row-must-not-appear"}};
    prepared.managerCountAndNumbers = {"opaque-manager-must-not-appear"};
    prepared.emptySection = {"opaque-empty-must-not-appear"};
    prepared.finalLines = {"opaque-final-must-not-appear"};
    m2m::mst::detail::Type3OptimizerScalarState state;
    struct EmptyManager : m2m::doe::Model {
        std::vector<m2m::doe::Var*> geometryVars(int, int) const override { return {}; }
        std::vector<m2m::doe::Var*> unlinkedVars() override { return {}; }
    } manager;
    m2m::mst::detail::Type3OptGroupReceipt receipt;
    Type3OptStreamSink sink(1252);
    const auto result = m2m::mst::detail::runType3OptStreamProfile(
        m2m::mst::detail::Type3OptStreamProfile::TypedManager,
        prepared, state, manager, sink, receipt);
    CHECK(result.admitted);
    CHECK(!result.writerResult); // source-bound writer return remains false
    CHECK(!sink.opened());
    const auto& bytes = sink.bytes();
    CHECK(bytes.find("opaque-prefix-must-not-appear") == std::string::npos);
    CHECK(bytes.find("opaque-suffix-must-not-appear") == std::string::npos);
    CHECK(bytes.find("opaque-row-must-not-appear") == std::string::npos);
    CHECK(bytes.find("opaque-manager-must-not-appear") == std::string::npos);
    CHECK(bytes.find("opaque-empty-must-not-appear") == std::string::npos);
    CHECK(bytes.find("opaque-final-must-not-appear") == std::string::npos);
    CHECK(bytes.find("ENDDATA") != std::string::npos);
}

int main() { return testkit::run_all(); }
