#include "../support/test_paths.hpp"
// MDT L6T: the DOE study files (SfMoDoeManager). Oracle B 120_B_DOE: Add DOE Study on COG_P1_B1
// (finished results), then Save, without a variable. Expected bytes come from that case; the
// file-name rules from the decompile of 0x140819600 (hand-derived, shown per case).
// ctest-labels: m2m-mdt mdt-l6
#include <testkit/check.hpp>

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>

#include "m2m/doe_study.hpp"
#include "m2m/mst/writer.hpp"

namespace fs = std::filesystem;
using namespace m2m;

namespace {

std::string readAll(const fs::path& f)
{
    std::ifstream in(f, std::ios::binary);
    return std::string(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
}

fs::path oracleB()
{
    return m2m_test::configuredPath("M2M_ORACLE_B");
}

class InjectedStudyIo : public doe::StudyIo {
public:
    explicit InjectedStudyIo(int failAt) : StudyIo(doe::IoMode::Recording), failAt_(failAt) {}
    bool ensureDirectory(const String& path) override
    {
        const bool success = static_cast<int>(directories().size()) != failAt_;
        return recordDirectory(path, false, true, success);
    }
private:
    int failAt_;
};

fs::path freshScratch(const char* label)
{
    const auto parent = m2m_test::scratchOutput(m2m_test::scratchRoot() / "doe-constructor-io");
    fs::create_directories(parent);
    const auto path = m2m_test::scratchOutput(parent / (std::string(label) + "-" +
        std::to_string(GetCurrentProcessId()) + "-" + std::to_string(GetTickCount64())));
    REQUIRE(!fs::exists(path));
    return path;
}

}  // namespace

TEST_CASE(doe_file_name_of)
{
    // '-' becomes '_' only when the name starts with it; then every listed character
    CHECK(doe::fileNameOf(qt::L("-a-b c.d")).utf8() == "_a_b_c_d");
    CHECK(doe::fileNameOf(qt::L("a-b(1)")).utf8() == "a-b_1_");
    CHECK(doe::fileNameOf(qt::L("x*#!@$'\"\\/<>:;,|?{}[]%^=&y")).utf8() == "x________________________y");
}

TEST_CASE(doe_sample_manager_defaults)
{
    doe::Study s;
    CHECK_EQ(s.samples.mode, 0);
    REQUIRE(s.samples.counts[0].has_value());
    CHECK_EQ(*s.samples.counts[0], 100);
    CHECK(!s.samples.counts[1].has_value());
    CHECK(!s.samples.counts[2].has_value());
    for (int mode = 0; mode < 3; ++mode) {
        CHECK(s.samples.invalid[mode]);
        CHECK(s.samples.tables[mode].empty());
    }
}

TEST_CASE(doe_constructor_stops_on_each_directory_failure_but_keeps_manager)
{
    for (int failAt = 0; failAt < 4; ++failAt) {
        Project p;
        p.path = qt::L("C:/scratch/project.moproj");
        p.doeIo = std::make_shared<InjectedStudyIo>(failAt);
        auto sub = makeSubProject(p, 2, 7);
        REQUIRE(sub->ownedDoeStudy != nullptr);
        CHECK(sub->doe == sub->ownedDoeStudy.get());
        CHECK(sub->ownedDoeStudy->sub == sub.get());
        CHECK(sub->ownedDoeStudy->name == qt::L("DoeStudy7"));
        CHECK(sub->ownedDoeStudy->vars.empty());
        CHECK(sub->ownedDoeStudy->geometryVars(1, 1).empty());
        CHECK(sub->ownedDoeStudy->samples.invalid[0]);
        REQUIRE(p.doeIo->directories().size() == static_cast<std::size_t>(failAt + 1));
        const char* expected[] = {"C:/scratch\\DoeStudies\\DoeStudy7",
            "C:/scratch\\DoeStudies\\DoeStudy7\\Control", "C:/scratch\\DoeStudies\\DoeStudy7\\Solutions",
            "C:/scratch\\DoeStudies\\DoeStudy7\\Output"};
        for (int i = 0; i <= failAt; ++i) {
            CHECK(p.doeIo->directories()[i].path == qt::L(expected[i]));
            CHECK(p.doeIo->directories()[i].success == (i != failAt));
        }
    }
}

TEST_CASE(doe_constructor_owned_model_reuses_vars_and_is_lazy)
{
    Project p;
    p.path = qt::L("C:/scratch/project.moproj");
    p.doeIo = std::make_shared<InjectedStudyIo>(-1);
    auto sub = makeSubProject(p, 2, 1);
    REQUIRE(sub->ownedDoeStudy != nullptr);
    CHECK(!sub->loaded);                                   // no SfDoeStudy/InputTable/sample load
    CHECK(p.doeIo->directories().size() == 4u);
    doe::Var var;
    sub->ownedDoeStudy->vars.push_back(&var);
    CHECK_EQ(sub->doe->count(), 1);                         // single inherited variable store
    bool refused = false;
    try { (void)sub->ownedDoeStudy->geometryVars(0, 0); }
    catch (const Error& e) { refused = e.code == M2M_E_UNSUPPORTED; }
    CHECK(refused);
    auto plain = makeSubProject(p, 0, 0);
    auto optimization = makeSubProject(p, 3, 2);
    CHECK(plain->doe == nullptr);
    CHECK(optimization->doe == nullptr);                    // Type3 lifecycle remains unsupported
    CHECK(p.doeIo->directories().size() == 4u);
}

TEST_CASE(doe_recording_constructor_creates_no_files)
{
    const auto root = freshScratch("recording");
    auto io = doe::makeRecordingStudyIo();
    Project p;
    p.path = String((root / "project.moproj").u16string());
    p.doeIo = io;
    CHECK(io->mode() == doe::IoMode::Recording);
    auto sub = makeSubProject(p, 2, 3);
    REQUIRE(sub->ownedDoeStudy != nullptr);
    CHECK(sub->ownedDoeStudy->name == qt::L("DoeStudy3"));
    CHECK(!fs::exists(root));
    REQUIRE(io->directories().size() == 4u);
    for (const auto& event : io->directories()) {
        CHECK(!event.existed);
        CHECK(event.attempted);
        CHECK(event.success);                              // explicitly simulated
    }
}

TEST_CASE(doe_native_directory_failure_preserves_partial_creation)
{
    // A file occupying Solutions makes its mkdir fail after root and Control succeeded.
    // It must not remove Control or attempt Output, and must not alter the blocking file.
    const auto root = freshScratch("native-partial");
    fs::create_directories(root);
    { std::ofstream blocked(root / "Solutions", std::ios::binary); blocked << "keep"; }
    auto io = doe::makeNativeStudyIo();
    CHECK(io->mode() == doe::IoMode::Native);
    CHECK(!doe::makeStudyDirs(String(root.u16string()), *io));
    CHECK(fs::is_directory(root / "Control"));
    CHECK(readAll(root / "Solutions") == "keep");
    CHECK(!fs::exists(root / "Output"));
    REQUIRE(io->directories().size() == 3u);
    CHECK(io->directories()[0].existed);
    CHECK(!io->directories()[0].attempted);
    CHECK(io->directories()[1].success);
    CHECK(!io->directories()[2].success);
}

TEST_CASE(doe_sample_parser_preserves_rows_and_numeric_failure)
{
    int mode = 2;
    doe::SampleTable rows{{99}};
    doe::parseSampleText(qt::L("1 ignored\r\n99 0.5 -2\r\nlabel\tbad\t1e2\n7 0.5 -2\n8\t\n\n"), mode, rows);
    CHECK_EQ(mode, 1);
    REQUIRE(rows.size() == 3u);
    CHECK((rows[0] == std::vector<double>{0.5, -2}));
    CHECK((rows[1] == std::vector<double>{0, 100}));
    CHECK(rows[2] == rows[0]);                              // duplicates remain, row id is ignored
    doe::parseSampleText(String(u"\n1\u00a00.25\u20032\n"), mode, rows);
    CHECK_EQ(mode, 1);                                     // empty header leaves selected mode alone
    REQUIRE(rows.size() == 1u);
    CHECK((rows[0] == std::vector<double>{0.25, 2}));
    doe::parseSampleText(String(), mode, rows);
    CHECK(rows.empty());
    CHECK_EQ(mode, 1);
    doe::parseSampleText(qt::L("nonnumeric\n"), mode, rows);
    CHECK_EQ(mode, 0);
    doe::parseSampleText(qt::L("42\n"), mode, rows);
    CHECK_EQ(mode, 42);                                    // low-level reader does not range-check
}

TEST_CASE(doe_sample_parser_shipped_regexp_whitespace_boundaries)
{
    int mode = 2;
    doe::SampleTable rows{{99}};
    doe::parseSampleText(String(u"0\n1\u00850.5\n"), mode, rows);
    CHECK_EQ(mode, 0);
    CHECK(rows.empty());                                  // NEL does not split the sole token
    // Put boundary characters in ignored row labels: this tests splitting without
    // assuming additional QString numeric-conversion or default-locale behavior.
    doe::parseSampleText(String(u"0\na\u0084b\u0085c\u0086d\t2\n"
                                u"a\u180e3\u20284\u20295\n"
                                u"a\u200b6\u200c7\u202a8 9\n"
                                u"a\v10\f11\r12\n"), mode, rows);
    REQUIRE(rows.size() == 4u);
    CHECK((rows[0] == std::vector<double>{2}));
    CHECK((rows[1] == std::vector<double>{3, 4, 5}));
    CHECK((rows[2] == std::vector<double>{9}));
    CHECK((rows[3] == std::vector<double>{10, 11, 12}));
}

TEST_CASE(doe_sample_writer_order_and_text_format)
{
    const doe::SampleTable rows{{0.5, -2}, {}, {0.5, -2}, {1.23456789, 1e-8}};
    engine::FileEffects fx;
    CHECK(doe::writeSampleTable(2, rows, qt::L("C:/x/samples.par"), fx));
    REQUIRE(fx.writes.size() == 1u);
    CHECK(fx.writes[0].bytes == "2\r\n1\t0.5\t-2\t\r\n2\t\r\n3\t0.5\t-2\t\r\n4\t1.23457\t1e-08\t\r\n");
    REQUIRE(rows.size() == 4u);                            // foreach does not consume the input
    CHECK(rows[0] == rows[2]);
}

TEST_CASE(doe_sample_historical_120_is_not_a_reload_roundtrip)
{
    const fs::path gui = oracleB() / "120_B_DOE" / "DoeStudies" / "DoeStudy1" / "Control" / "DEF_DOE.PAR";
    if (!fs::exists(gui)) {
        std::printf("SKIP: no oracle B 120_B_DOE\n");
        return;
    }
    const std::string bytes = readAll(gui);
    doe::SampleTable hundredEmptyRows(100);
    engine::FileEffects fx;
    CHECK(doe::writeSampleTable(0, hundredEmptyRows, qt::L("C:/x"), fx));
    REQUIRE(fx.writes.size() == 1u);
    CHECK(fx.writes[0].bytes == bytes);                      // UI-created state, not Open-Save evidence
    doe::Study loaded;
    doe::loadCurrentSampleText(loaded, String::latin1(bytes)); // fixture is ASCII numeric text
    CHECK(loaded.samples.tables[0].empty());                // original parser drops id-only rows
    CHECK(!loaded.samples.invalid[0]);
    REQUIRE(loaded.samples.counts[0].has_value());
    CHECK_EQ(*loaded.samples.counts[0], 0);
}

TEST_CASE(doe_sample_save_gates_and_current_path)
{
    doe::Study s;
    s.name = qt::L("DoeStudy7");
    engine::FileEffects fx;
    CHECK(!doe::saveCurrentSamples(s, qt::L("C:/x/DoeStudies"), fx));
    CHECK(!doe::saveSampleTable(s, -1, qt::L("unused"), fx));
    CHECK(!doe::saveSampleTable(s, 3, qt::L("unused"), fx));
    CHECK(fx.writes.empty());
    s.samples.mode = 1;
    s.samples.invalid[1] = false;
    s.samples.tables[1] = {{0.25}};
    CHECK(doe::saveCurrentSamples(s, qt::L("C:/x/DoeStudies"), fx));
    REQUIRE(fx.writes.size() == 1u);
    CHECK(fx.writes[0].path.utf8() == "C:/x/DoeStudies\\DoeStudy7\\Control\\DEF_DOE.PAR");
    CHECK(fx.writes[0].bytes == "1\r\n1\t0.25\t\r\n");
}

TEST_CASE(doe_sample_load_retains_empty_write_and_wrong_mode_clear)
{
    doe::Study s;
    s.samples.tables[0] = {{1}};
    s.samples.tables[1] = {{2}};
    engine::FileEffects fx;
    CHECK(doe::loadSampleTable(s, 1, qt::L("Samples_FF.par"), fx));
    REQUIRE(fx.writes.size() == 1u);
    CHECK(fx.writes[0].bytes == "1\r\n");                    // load calls writer with empty local table
    CHECK(s.samples.tables[0].empty());                    // clears current, rather than requested
    CHECK((s.samples.tables[1] == doe::SampleTable{{2}}));
    CHECK(!s.samples.invalid[1]);
    CHECK(s.samples.invalid[0]);
    CHECK(doe::loadSampleTable(s, 2, qt::L("Samples_USR.par"), fx));
    CHECK(s.samples.invalid[2]);                           // empty requested table skips flag change
    CHECK(!doe::loadSampleTable(s, 3, qt::L("unused"), fx));
    CHECK(fx.writes.size() == 2u);
}

TEST_CASE(doe_sample_current_load_commits_mode_count_and_refuses_undefined_index)
{
    doe::Study s;
    s.samples.tables[0] = {{9}};
    doe::loadCurrentSampleText(s, qt::L("2\n7 0.5\n8 0.5\n"));
    CHECK_EQ(s.samples.mode, 2);
    REQUIRE(s.samples.counts[2].has_value());
    CHECK_EQ(*s.samples.counts[2], 2);
    CHECK(!s.samples.invalid[2]);
    CHECK((s.samples.tables[0] == doe::SampleTable{{9}}));
    bool refused = false;
    try { doe::loadCurrentSampleText(s, qt::L("3\n1 0.5\n")); }
    catch (const Error& e) { refused = e.code == M2M_E_UNSUPPORTED; }
    CHECK(refused);
    CHECK_EQ(s.samples.mode, 3);                            // parse side effect precedes unsafe index
    CHECK(s.samples.tables[2].size() == 2u);
}

TEST_CASE(doe_input_table_120)
{
    const fs::path ob = oracleB() / "120_B_DOE";
    const fs::path gui = ob / "DoeStudies" / "DoeStudy1" / "Control" / "DoeInputTable.XML";
    if (!fs::exists(gui)) {
        std::printf("SKIP: no oracle B 120_B_DOE\n");
        return;
    }
    // a new study: the manager's defaults (no mesh path, no positioning, ROI type 0, no regions)
    doe::Study s;
    s.name = qt::L("DoeStudy1");
    engine::FileEffects fx;
    CHECK(doe::writeInputTable(qt::L("C:/x/DoeInputTable.XML"), s, fx));
    REQUIRE(fx.writes.size() == 1u);
    CHECK(fx.writes[0].bytes == readAll(gui));
    // the reader takes the same file back
    doe::Study r;
    r.roiType = 7;
    r.doPositioning = true;
    CHECK(doe::readInputTable(readAll(gui), r));
    CHECK_EQ(r.roiType, 0);
    CHECK(!r.doPositioning);
    CHECK(r.commonMeshPath.isEmpty());
}

TEST_CASE(doe_parser_ini_120)
{
    const fs::path gui = oracleB() / "120_B_DOE" / "DoeStudies" / "DoeStudy1" / "Control" / "DEF_DOE_PARSER.INI";
    if (!fs::exists(gui)) {
        std::printf("SKIP: no oracle B 120_B_DOE\n");
        return;
    }
    engine::FileEffects fx;
    CHECK(doe::writeParserIni(qt::L("COG_P1_B1.MST.TEMP"), qt::L("COG_P1_B1.MST"), qt::L("C:/x"), fx));
    REQUIRE(fx.writes.size() == 1u);
    CHECK(fx.writes[0].bytes == readAll(gui));
    CHECK(fx.writes[0].path.utf8() == "C:/x\\DEF_DOE_PARSER.INI");
}

TEST_CASE(doe_def_doe_ini_120)
{
    const fs::path gui = oracleB() / "120_B_DOE" / "DoeStudies" / "DoeStudy1" / "Control" / "DEF_DOE.INI";
    if (!fs::exists(gui)) {
        std::printf("SKIP: no oracle B 120_B_DOE\n");
        return;
    }
    // 120: object 1, operation 5, no positioning, ROI 0; caller supplies 100 run entries.
    doe::Study s;
    s.objNo = 1;
    s.oprNo = 5;
    s.designCounts.assign(100, 1);
    engine::FileEffects fx;
    CHECK(doe::writeDefDoeIni(qt::L("C:/x"), qt::L("COG_P1_B1"), s, false, false, fx));
    REQUIRE(fx.writes.size() == 1u);
    CHECK(fx.writes[0].bytes == readAll(gui));
}

// 121's variable is TLOC (id 8), a group 6 (command) keyword: 0x140916530 dispatches groups 1..5
// only and then reads an uninitialised status, so the port refuses
TEST_CASE(doe_temp_kwd_tloc_refuses)
{
    std::unique_ptr<def::model::DYNARY> out[3];
    engine::FileEffects fx;
    bool refused = false;
    try {
        const qt::String t = qt::L("TLOC      \t0.1\n");
        doe::keywordValues(8, 0, 3, {}, t, t, t, qt::L("C:/x"), fx, out);
    } catch (const Error& e) {
        refused = e.code == M2M_E_UNSUPPORTED;
    }
    CHECK(refused);
    CHECK(fx.writes.empty());
}

TEST_CASE(doe_variables_refuse)
{
    doe::Study s;
    doe::Var v;
    s.vars.push_back(&v);
    engine::FileEffects fx;
    bool refused = false;
    try {
        doe::writeInputTable(qt::L("C:/x/DoeInputTable.XML"), s, fx);
    } catch (const Error& e) {
        refused = e.code == M2M_E_UNSUPPORTED;
    }
    CHECK(refused);
}

int main()
{
    return testkit::run_all();
}
