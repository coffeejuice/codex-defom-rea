#include "../support/test_paths.hpp"
// ctest-labels: m2m-mdt mdt-l6
#include <testkit/check.hpp>

#include <fstream>
#include <iterator>
#include <map>

#include "m2m/doe_activation.hpp"
#include "m2m/mst/writer.hpp"

using namespace m2m;
using qt::L;
namespace fs = std::filesystem;

namespace {
const std::string input = "<sfDoeInputTable><DoeCommonMeshPath path='mesh'/>"
    "<DoeDoPositioning bool='T'/><DoeRoiType type='1'/><DoeVariables/>"
    "<Operations/><RegionsOfInterest/><Constraints/></sfDoeInputTable>";

xml::NodePtr parse(const std::string& text)
{
    xml::Document doc;
    String err;
    int line = 0, col = 0;
    REQUIRE(xml::setContent(doc, text, &err, &line, &col));
    return doc.root;
}

std::string leaf(const String& path)
{
    const auto s = path.utf8();
    return s.substr(s.find_last_of("/\\") + 1);
}

class ScriptIo final : public doe::StudyIo {
public:
    ScriptIo() : StudyIo(doe::IoMode::Recording) {}
    std::map<std::string, std::string> files;
    std::vector<bool> opens{true, true, true};
    std::vector<std::string> order;
    bool ensureDirectory(const String&) override { throw std::runtime_error("unexpected mkdir"); }
    bool readFile(const String& path, std::string& bytes) override
    {
        const auto name = leaf(path);
        order.push_back("read:" + name);
        const auto found = files.find(name);
        bytes = found == files.end() ? "" : found->second;
        return recordRead(path, bytes, found != files.end(), found != files.end());
    }
    bool writeTextFile(const String& path, const std::string& bytes) override
    {
        order.push_back("write:" + leaf(path));
        const bool opened = opens.at(textWrites().size());
        return recordTextWrite(path, bytes, opened, opened, opened);
    }
};

const auto element = [] { return parse("<SfDoeStudy name='Existing'/>"); };
const auto studies = [] { return L("C:\\disposable\\DoeStudies"); };

std::string read(const fs::path& path)
{
    std::ifstream file(path, std::ios::binary);
    return {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
}

void write(const fs::path& path, const std::string& bytes)
{
    std::ofstream file(path, std::ios::binary);
    REQUIRE(file.is_open());
    file.write(bytes.data(), static_cast<std::streamsize>(bytes.size()));
}

fs::path scratch(const char* mode)
{
    static unsigned next = 0;
    auto root = m2m_test::scratchOutput(m2m_test::scratchRoot() /
        (std::string("doe-activation-") + mode + "-" + std::to_string(GetCurrentProcessId()) +
         "-" + std::to_string(GetTickCount64()) + "-" + std::to_string(++next)));
    REQUIRE(!fs::exists(root));
    return root;
}
}  // namespace

TEST_CASE(doe_lazy_study_order_and_zero_column_par_commit)
{
    ScriptIo io;
    io.files["DoeInputTable.XML"] = input;
    io.files["DEF_DOE.PAR"] = "0\r\n1\t\r\n2\t\r\n";
    doe::Study study;
    study.regions = {L("prior")};
    study.constraints = 1;
    doe::readLazyStudy(study, *element(), studies(), io);
    CHECK(io.order == std::vector<std::string>({"read:DoeInputTable.XML", "write:Samples_LHS.par",
        "write:Samples_FF.par", "write:Samples_USR.par", "read:DEF_DOE.PAR"}));
    CHECK(study.name == L("Existing") && study.commonMeshPath == L("mesh"));
    CHECK(study.doPositioning && study.roiType == 1);
    CHECK(study.regions.empty() && study.constraints == 0);
    CHECK(study.samples.tables[0].empty() && !study.samples.invalid[0]);
    CHECK(*study.samples.counts[0] == 0);
    for (int i = 0; i < 3; ++i)
        CHECK(io.textWrites()[i].requestedBytes == qt::formatInt(i) + "\r\n");
}

TEST_CASE(doe_lazy_failed_input_and_sample_open_continue_without_rollback)
{
    ScriptIo io;
    io.opens = {false, true, true};
    doe::Study study;
    study.samples.tables[0] = {{1}};
    study.samples.tables[1] = {{2}};
    study.samples.tables[2] = {{3}};
    doe::readLazyStudy(study, *element(), studies(), io);
    CHECK(io.order.size() == 5);
    CHECK(io.lastError() == L("Failed to open file, ") + studies() +
        L("\\Existing\\Control\\DoeInputTable.XML"));
    CHECK(study.samples.tables[0].empty()); // successful mode1 clears CURRENT mode0
    CHECK(study.samples.tables[1] == doe::SampleTable({{2}}));
    CHECK(study.samples.tables[2] == doe::SampleTable({{3}}));
    CHECK(study.samples.invalid[0] && !study.samples.invalid[1] && !study.samples.invalid[2]);
    CHECK(*study.samples.counts[0] == 100); // failed PAR open does not commit
    REQUIRE(io.reads().size() == 2);
    CHECK(!io.reads()[0].opened && !io.reads()[1].opened);
}

TEST_CASE(doe_input_table_status_is_child_reader_result_not_header_success)
{
    doe::Study study;
    CHECK(!doe::readInputTable("<x><DoeRoiType type='7'/></x>", study));
    CHECK(study.roiType == 7);
    CHECK(doe::readInputTable("<x><DoeVariables/><DoeRoiType type='2'/></x>", study));
    CHECK(study.roiType == 2);
    CHECK(!doe::readInputTable("<x/>", study));
    ScriptIo io;
    io.setLastError(L("prior open error"));
    io.files["DoeInputTable.XML"] = "<malformed";
    io.files["DEF_DOE.PAR"] = "1\n1\t5\n";
    doe::readLazyStudy(study, *element(), studies(), io);
    CHECK(io.lastError() == L("prior open error")); // parse failure does not replace global
    CHECK(study.samples.mode == 1);
    CHECK(study.samples.tables[1] == doe::SampleTable({{5}}));
    CHECK(*study.samples.counts[1] == 1 && !study.samples.invalid[1]);
}

TEST_CASE(doe_lazy_unsupported_branch_preserves_prior_header_and_sample_effects)
{
    ScriptIo io;
    io.files["DoeInputTable.XML"] = "<x><DoeRoiType type='8'/><DoeVariables><Var/></DoeVariables></x>";
    doe::Study study;
    bool refused = false;
    try { doe::readLazyStudy(study, *element(), studies(), io); }
    catch (const Error& e) { refused = e.code == M2M_E_UNSUPPORTED; }
    CHECK(refused && study.roiType == 8 && study.name == L("Existing"));
    CHECK(io.textWrites().empty() && io.order.size() == 1);

    ScriptIo invalidMode;
    invalidMode.files["DoeInputTable.XML"] = input;
    invalidMode.files["DEF_DOE.PAR"] = "9\n";
    refused = false;
    try { doe::readLazyStudy(study, *element(), studies(), invalidMode); }
    catch (const Error& e) { refused = e.code == M2M_E_UNSUPPORTED; }
    CHECK(refused && study.samples.mode == 9);
    CHECK(invalidMode.textWrites().size() == 3);
}

TEST_CASE(doe_lazy_par_codec_guard_occurs_after_sample_writes)
{
    ScriptIo io;
    io.files["DoeInputTable.XML"] = input;
    io.files["DEF_DOE.PAR"] = "\xef\xbb\xbf" "0\n";
    doe::Study study;
    bool refused = false;
    try { doe::readLazyStudy(study, *element(), studies(), io); }
    catch (const Error& e) { refused = e.code == M2M_E_UNSUPPORTED; }
    CHECK(refused && io.textWrites().size() == 3 && io.reads().size() == 2);
    CHECK(study.samples.mode == 0 && study.samples.invalid[0]);
}

TEST_CASE(doe_lazy_native_wiring_and_recording_leave_explicitly_distinct_file_effects)
{
    for (bool native : {false, true}) {
        const auto root = scratch(native ? "native" : "recording");
        const auto control = root / "DoeStudies" / "Existing" / "Control";
        fs::create_directories(control);
        write(control / "DoeInputTable.XML", input);
        write(control / "DEF_DOE.PAR", "0\r\n1\t\r\n");
        for (const auto* name : {"Samples_LHS.par", "Samples_FF.par", "Samples_USR.par"})
            write(control / name, "original bytes");
        Project project;
        project.path = String((root / "Case").u16string());
        project.doeIo = native ? doe::makeNativeStudyIo() : doe::makeRecordingStudyIo();
        project.subs.push_back(makeSubProject(project, 2, 1));
        project.dom = std::make_shared<xml::Document>();
        project.dom->root = parse("<ProjectFile><Sub_Project><SfDoeStudy name='Existing'/></Sub_Project></ProjectFile>");
        Ctx ctx;
        activateSubProject(project, 0, ctx); // internal lazy-parse test, not default C ABI activation
        CHECK(project.subs[0]->loaded && !project.loadingSub);
        const auto& study = *project.subs[0]->ownedDoeStudy;
        CHECK(study.samples.tables[0].empty() && !study.samples.invalid[0]);
        CHECK(*study.samples.counts[0] == 0);
        const char* const names[] = {"Samples_LHS.par", "Samples_FF.par", "Samples_USR.par"};
        for (int mode = 0; mode < 3; ++mode)
            CHECK(read(control / names[mode]) == (native ? qt::formatInt(mode) + "\r\n" : "original bytes"));
        CHECK(read(control / "DEF_DOE.PAR") == "0\r\n1\t\r\n");
        if (!native) {
            std::string virtualBytes;
            CHECK(project.doeIo->readFile(String((control / names[0]).u16string()), virtualBytes));
            CHECK(virtualBytes == "0\r\n");
            CHECK(!fs::exists(root / "DoeStudies" / "DoeStudy1"));
        }
    }
}

int main() { return testkit::run_all(); }
