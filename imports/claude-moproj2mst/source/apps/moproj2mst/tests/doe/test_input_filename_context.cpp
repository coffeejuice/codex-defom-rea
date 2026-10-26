// ctest-labels: m2m-doe
#include <testkit/check.hpp>

#include <string>

#include "m2m/common.hpp"
#include "m2m/doe_activation.hpp"
#include "m2m/doe_io.hpp"
#include "m2m/doe_study.hpp"
#include "m2m/qt4path.hpp"
#include "../../src/doe/input_table_reader.hpp"
#include "../support/test_paths.hpp"

using namespace m2m;
namespace {
const std::string geometry = "<sfDoeInputTable><DoeVariables>"
    "<DoeVariable name=\"A\" numSamples=\"3\" varId=\"7\" dimension=\"3\">"
    "<Description>kept</Description><Geometry/></DoeVariable></DoeVariables></sfDoeInputTable>";
template<class Work> std::string refused(const Work& work)
{
    try { work(); }
    catch (const Error& error) {
        CHECK_EQ(error.code, M2M_E_UNSUPPORTED);
        return error.what();
    }
    CHECK(false);
    return {};
}
String sourcePath()
{ return String((m2m_test::scratchRoot() / "input-filename-context" / "Control" / "DoeInputTable.XML").u16string()); }
}

TEST_CASE(doe_named_input_source_does_not_admit_geometry_or_change_refusal)
{
    doe::Study bytesOnly;
    const auto expected = refused([&] { (void)doe::readInputTable(geometry, bytesOnly); });
    for (const auto& source : {sourcePath(), qt::L("")}) {
        doe::Study named;
        CHECK(refused([&] { (void)doe::readInputTableFromSource(geometry, named, {source}); }) == expected);
        REQUIRE(named.vars.size() == 1 && named.headerVariableOwners.size() == 1);
        CHECK_EQ(named.vars[0]->number, 7);
        CHECK_EQ(named.vars[0]->index, -1);
        CHECK(named.headerVariableOwners[0]->description == qt::L("kept"));
        CHECK(named.vars[0]->files.empty() && !named.vars[0]->geometry);
    }
    CHECK(expected.find("Geometry entry 0x140911532") != std::string::npos);
}

TEST_CASE(doe_named_input_source_preserves_normal_and_malformed_bytes_results)
{
    // Signature of the public bytes-only API stays unchanged and source-less.
    bool (*const bytesReader)(const std::string&, doe::Study&) = &doe::readInputTable;
    doe::Study named, bytesOnly;
    const std::string normal = "<sfDoeInputTable><Operations/></sfDoeInputTable>";
    CHECK(doe::readInputTableFromSource(normal, named, {sourcePath()}));
    CHECK(bytesReader(normal, bytesOnly));
    CHECK(!doe::readInputTableFromSource("<", named, {sourcePath()}));
    CHECK(!bytesReader("<", bytesOnly));
    CHECK(named.vars.empty() && bytesOnly.vars.empty());
}

TEST_CASE(doe_lazy_read_uses_exact_read_path_and_still_stops_before_fileinfo)
{
    auto io = doe::makeRecordingStudyIo();
    doe::Study study;
    const String studies((m2m_test::scratchRoot() / "input-filename-lazy" / "DoeStudies").u16string());
    auto element = xml::createElement(qt::L("SfDoeStudy"));
    xml::setAttribute(*element, qt::L("name"), qt::L("SourceStudy"));
    const String input = studies + qt::kSep + qt::L("SourceStudy") + qt::kSep +
        qt::L("Control") + qt::kSep + qt::L("DoeInputTable.XML");
    CHECK(io->writeTextFile(input, geometry));
    const auto writesBefore = io->textWrites().size();
    CHECK(refused([&] { doe::readLazyStudy(study, *element, studies, *io); })
        .find("Geometry entry 0x140911532") != std::string::npos);
    REQUIRE(io->reads().size() == 1 && study.headerVariableOwners.size() == 1);
    CHECK(io->reads()[0].path == input);
    CHECK(io->textWrites().size() == writesBefore);
    CHECK(study.name == qt::L("SourceStudy"));
    CHECK(study.headerVariableOwners[0]->description == qt::L("kept"));
    CHECK(study.samples.invalid[0] && study.samples.invalid[1] && study.samples.invalid[2]);
}

int main() { return testkit::run_all(); }
