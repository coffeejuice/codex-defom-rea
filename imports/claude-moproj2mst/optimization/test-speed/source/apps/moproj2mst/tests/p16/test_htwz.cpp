// HeatTreatWizard.HTWZ readers, V3 (0x1411b4ab0) and V1/V2 (0x1411b5e50, engine model only); since
// MDT L6 one body (src/ops/heattreat_htwz.cpp) that keeps media, zone tables and node records.
// The inputs follow the readers' fscanf/fgets/strtok sequence; no sample file exists (s1-05 Q8).
// Engine models are real ones (a fresh DB_STEP_DATA with object 1, no mesh).
#include <testkit/check.hpp>

#include <cstdio>
#include <string>

#include <memory>

#include <defmodel/layout/db_step_data.hpp>

#include "m2m/common.hpp"
#include "m2m/data/heattreat.hpp"
#include "m2m/engine.hpp"
#include "m2m/ht_schedule.hpp"

using m2m::data::HTSchedule;
using m2m::engine::GuiModel;
using m2m::qt::String;

namespace {

const char* kFull =
    "HEAT_TREATMENT_SPECS_FILE_V3\n"
    "NO_MEDIA  1\n"
    "MEDIA   1\n"
    "Water quench\n"
    "NO_ZONES   2\n"
    "ZONE   0\n"
    "CNVCOF 1, 2\n"       // fgets after "%s": type and value share the keyword's line; then
    "0.0 1.0\n"           // 2 rows of (x, value) and the rest of the last row's line
    "10.0 2.0\n"
    "ZONE   1\n"
    "CNVCOF 0 5000.0\n"   // constant: no table
    "RADIATION 1\n"
    "ACVCOF 2 1\n"
    "0.5 0.7\n"
    "NO_ECTMFN  0\n"
    "NO_PROCS  3\n"
    "PROC 1 120.0 1 0 0 2 0 1 1 C:/x/user.KEY 0\n"
    "0 20\n"              // step+0x20 table, 2 rows
    "60 25\n"
    "3 4\n"               // one object pair
    "PROC 2 60.0 2 0 0 0 0 0 0\n"
    "PROC 3 30 1 0 0 0 0 0 0 None 1\n"
    "START_OPTNO  2\n";

const char* kProcs =
    "NO_PROCS  2\n"
    "PROC 1 100 1 0 0 0 0 0 0 D:/k/a.key 0\n"
    "PROC 2 50 1 0 0 0 0 0 0\n"
    "START_OPTNO  2\n";

// A model of dimension dim (with object 1 unless noObject), ECCTMP / ECTMFN widths w1 / w2
std::unique_ptr<GuiModel> model(int dim, int w1 = 1, int w2 = 1, bool noObject = false)
{
    auto m = std::make_unique<GuiModel>(nullptr, dim);
    if (!noObject) m->objectWrap(1, false, false);
    m->step->u15f8[68] = w1;                           // data+0x1708
    m->step->u15f8[69] = w2;                           // data+0x170c
    return m;
}

int parse(HTSchedule& s, const std::string& f, GuiModel* m = nullptr)
{
    return m2m::ht::readBytes(s, f, m);
}

int parseCode(HTSchedule& s, const std::string& f, GuiModel* m)
{
    try {
        parse(s, f, m);
    } catch (const m2m::Error& e) {
        return e.code;
    }
    return 0;
}

}  // namespace

TEST_CASE(full_v3_file)
{
    HTSchedule s;
    parse(s, kFull);
    CHECK_EQ(s.media.size(), 1u);
    CHECK_EQ(s.ectmfn.size(), 0u);
    REQUIRE(s.steps.size() == 3u);
    CHECK_EQ(s.first, 2);
    CHECK(s.steps[0]->userKey == String(u"C:/x/user.KEY"));
    CHECK(s.steps[1]->userKey == String(u"None"));      // token 9 absent
    CHECK(s.steps[2]->userKey == String(u"None"));
    CHECK(s.steps[0]->time == 120.0);
    CHECK_EQ(s.steps[0]->media, 0);
    CHECK_EQ(s.steps[1]->media, 1);
    CHECK_EQ(s.steps[0]->t20.n[0], 2);
    REQUIRE(s.steps[0]->pairs.size() == 1u);
    CHECK_EQ(s.steps[0]->pairs[0].a, 3);
    CHECK_EQ(s.steps[0]->pairs[0].b, 4);
    CHECK(s.steps[2]->flag10);
}

TEST_CASE(crlf_and_ctrl_z)
{
    std::string crlf;
    for (const char* p = kFull; *p; ++p) {
        if (*p == '\n') crlf += '\r';
        crlf += *p;
    }
    HTSchedule s;
    parse(s, crlf + "\x1aSTART_OPTNO 3\n");
    REQUIRE(s.steps.size() == 3u);
    CHECK_EQ(s.first, 2);                              // text after Ctrl-Z is not read
    CHECK(s.steps[0]->userKey == String(u"C:/x/user.KEY"));
}

TEST_CASE(start_optno_out_of_range_keeps_default)
{
    std::string f = kFull;
    f.replace(f.find("START_OPTNO  2"), 14, "START_OPTNO  4");
    HTSchedule s;
    parse(s, f);
    CHECK_EQ(s.first, 1);
}

TEST_CASE(old_format_without_engine_model_reads_nothing)
{
    std::string f = kFull;
    f.replace(0, 28, "HEAT_TREATMENT_SPECS_FILE_3D_V2");
    HTSchedule s;
    parse(s, f);
    CHECK(s.steps.empty());
    CHECK_EQ(s.first, 1);
}

TEST_CASE(v2_3d_with_engine_model)
{
    std::string f = std::string(
        "HEAT_TREATMENT_SPECS_FILE_3D_V2\n"
        "NO_MEDIA  1\n"
        "MEDIA   1\n"
        "Oil\n"
        "NO_ZONES   1\n"
        "ZONE   0\n"
        "CNVCOF 0 1000\n"
        "ECCTMP 1 2 0\n"          // "%s %d", then fgets of the line's rest: 2 rows, default 0
        "1 2 3 4 5 0x10 017\n"    // 3D row: 5 ints, then nEcctmp = 2 values (%i: hex, octal)
        "6 7 8 9 10 1 2\n"
        "ECTMFN 1 0 0\n"          // no rows: no rest-of-line read
        "RADIATION 0\n"
        "ACVCOF 0 0\n") + kProcs;
    HTSchedule s;
    { auto m3 = model(3, 2, 1); parse(s, f, m3.get()); }
    CHECK_EQ(s.media.size(), 1u);
    REQUIRE(s.steps.size() == 2u);
    CHECK_EQ(s.first, 2);
    CHECK(s.steps[0]->userKey == String(u"D:/k/a.key"));
    CHECK(s.steps[1]->userKey == String(u"None"));
}

TEST_CASE(v1_2d_with_engine_model)
{
    std::string f = std::string(
        "HEAT_TREATMENT_SPECS_FILE\n"
        "NO_MEDIA  1\n"
        "MEDIA   1\n"
        "Air\n"
        "NO_ZONES   1\n"
        "ZONE   0\n"
        "NODES    1   5\n"        // 2D V1 region line "%s    %d   %d\n"
        "CNVCOF 2, 1\n"
        "0 10\n") + kProcs;       // V1: no ECC, RADIATION or ACVCOF records
    HTSchedule s;
    auto m2 = model(2);
    // the 2D version-1 region line goes through 0x140a9e8f0 (DEF_PRE 1403056a0, not ported)
    CHECK_EQ(parseCode(s, f, m2.get()), M2M_E_UNSUPPORTED);
    // read with a 3D model the file is version 1 without the region line: it stops at "NODES"
    HTSchedule t;
    { auto m3 = model(3); parse(t, f, m3.get()); }
    CHECK(t.steps.empty());
}

TEST_CASE(v12_needs_object_one)
{
    auto none = model(3, 1, 1, true);
    std::string f = "HEAT_TREATMENT_SPECS_FILE_3D\nNO_MEDIA  0\nNO_PROCS  1\nPROC 1 1 1 0 0 0 0 0 0\n";
    HTSchedule s;
    parse(s, f, none.get());
    CHECK(s.steps.empty());
    { auto m3 = model(3, 2, 1); parse(s, f, m3.get()); }
    CHECK(s.steps.size() == 1u);
}

TEST_CASE(v3_with_engine_model_uses_v3_reader)
{
    HTSchedule s;
    { auto m3 = model(3); parse(s, kFull, m3.get()); }
    CHECK(s.steps.size() == 3u);
}

TEST_CASE(bad_section_stops_before_procs)
{
    std::string f = kFull;
    f.replace(f.find("RADIATION"), 9, "RADIATIOX");
    HTSchedule s;
    parse(s, f);
    CHECK_EQ(s.media.size(), 1u);
    CHECK(s.steps.empty());
    CHECK_EQ(s.first, 1);
}

TEST_CASE(truncated_after_procs_keeps_steps)
{
    std::string f = kFull;
    f.erase(f.find("START_OPTNO"));
    HTSchedule s;
    parse(s, f);
    CHECK(s.steps.size() == 3u);
    CHECK_EQ(s.first, 1);
}

TEST_CASE(table_row_count_shifts_input)
{
    // One row too many in the zone table: the reader takes "ZONE" as data and stops at the next
    // check, as the original does.
    std::string f = kFull;
    f.replace(f.find("1, 2\n"), 5, "1, 3\n");
    HTSchedule s;
    parse(s, f);
    CHECK(s.steps.empty());
}

namespace {

std::string v3(const std::string& media, const std::string& ect, const std::string& procs)
{
    return "HEAT_TREATMENT_SPECS_FILE_V3\n" + media + ect + procs;
}

const char* kNoMedia = "NO_MEDIA  0\n";
const char* kNoEct = "NO_ECTMFN  0\n";

}  // namespace

TEST_CASE(procs_past_end_of_file_repeat_the_last_record)
{
    // NO_PROCS says 3 but only one record follows. At the end of input "%s" fails and leaves
    // "PROC" in the buffer, and fgets fails and leaves the last tokens: the original builds two
    // more steps from the same tokens.
    HTSchedule s;
    parse(s, v3(kNoMedia, kNoEct, "NO_PROCS  3\nPROC 1 10 1 0 0 0 0 0 0 K.key 0\n"));
    REQUIRE(s.steps.size() == 3u);
    CHECK(s.steps[2]->userKey == String(u"K.key"));
    CHECK_EQ(s.first, 1);
}

TEST_CASE(proc_tables_and_pairs_consume_their_lines)
{
    HTSchedule s;
    parse(s, v3(kNoMedia, kNoEct,
                              "NO_PROCS  2\n"
                              "PROC 1 10 1 0 0 1 2 0 2 A.key 0\n"
                              "0 1\n"            // step+0x20: 1 row
                              "0 1\n"            // step+0x98: 2 rows on two lines
                              "5 6\n"
                              "1 2\n"            // 2 pairs
                              "3 4\n"
                              "PROC 2 10 1 0 0 0 0 0 0\n"
                              "START_OPTNO  2\n"),
                        {});
    REQUIRE(s.steps.size() == 2u);
    CHECK_EQ(s.steps[0]->t20.n[0], 1);
    CHECK_EQ(s.steps[0]->t98.n[0], 2);
    REQUIRE(s.steps[0]->pairs.size() == 2u);
    CHECK_EQ(s.steps[0]->pairs[1].a, 3);
    CHECK_EQ(s.first, 2);
}

TEST_CASE(index_checks)
{
    HTSchedule a;                                      // MEDIA must count from 1
    parse(a, v3("NO_MEDIA  1\nMEDIA   2\nX\nNO_ZONES   0\nRADIATION 0\nACVCOF 0 0\n",
                              kNoEct, "NO_PROCS  1\nPROC 1 1 1 0 0 0 0 0 0\n"));
    CHECK(a.steps.empty());
    CHECK_EQ(a.media.size(), 0u);
    HTSchedule b;                                      // ZONE counts from 0
    parse(b, v3("NO_MEDIA  1\nMEDIA   1\nX\nNO_ZONES   1\nZONE   1\nCNVCOF 0 1\n"
                              "RADIATION 0\nACVCOF 0 0\n",
                              kNoEct, "NO_PROCS  1\nPROC 1 1 1 0 0 0 0 0 0\n"));
    CHECK(b.steps.empty());
    CHECK_EQ(b.media.size(), 1u);
    HTSchedule c;                                      // a good media block
    parse(c, v3("NO_MEDIA  1\nMEDIA   1\nX\nNO_ZONES   1\nZONE   0\nCNVCOF 0 1\n"
                              "RADIATION 0\nACVCOF 0 0\n",
                              kNoEct, "NO_PROCS  1\nPROC 1 1 1 0 0 0 0 0 0\n"));
    CHECK(c.steps.size() == 1u);
}

TEST_CASE(other_coefficient_types_read_no_table)
{
    // type 3 keeps +4 = 0 after 0x14089b9e0, so no table follows; the type is %i (0x1 = 1), the
    // value %lf (010 = 10)
    HTSchedule s;
    parse(s, v3("NO_MEDIA  1\nMEDIA   1\nX\nNO_ZONES   1\nZONE   0\nCNVCOF 3 5\n"
                              "RADIATION 0\nACVCOF 0x1 010\n"     // "%i %Flf": type 1, 10 rows
                              "0 0\n0 0\n0 0\n0 0\n0 0\n0 0\n0 0\n0 0\n0 0\n0 0\n",
                              kNoEct, "NO_PROCS  1\nPROC 1 1 1 0 0 0 0 0 0\n"));
    CHECK(s.steps.size() == 1u);
}

TEST_CASE(ectmfn_records)
{
    const std::string rec = v3(kNoMedia,
                               "NO_ECTMFN  1\nECTMFN_NO   1\nOBJECT_NO   2\nMEDIA_NO   1\n"
                               "ECTMFN   2   2   0\n1  2  3\n4  5  6\n",
                               "NO_PROCS  1\nPROC 1 1 1 0 0 0 0 0 0\n");
    auto m3 = model(3);
    HTSchedule s;
    parse(s, rec, m3.get());
    REQUIRE(s.ectmfn.size() == 1u);
    CHECK_EQ(s.ectmfn[0]->object, 2);
    CHECK_EQ(s.ectmfn[0]->media, 1);
    // object 2 does not exist, so no face count: every (node > 0, value != 0) row is kept
    REQUIRE(s.ectmfn[0]->values.size() == 2u);
    CHECK_EQ(s.ectmfn[0]->values[1].first, 5);
    CHECK_EQ(s.ectmfn[0]->values[1].second, 6);
    CHECK(s.steps.size() == 1u);
    HTSchedule t;                                      // object number must match OBJECT_NO
    parse(t, v3(kNoMedia,
                "NO_ECTMFN  1\nECTMFN_NO   1\nOBJECT_NO   2\nMEDIA_NO   1\n"
                "ECTMFN   3   0   0\n",
                "NO_PROCS  1\nPROC 1 1 1 0 0 0 0 0 0\n"), m3.get());
    CHECK(t.steps.empty());
    HTSchedule u;                                      // without a model the original reads a null model
    CHECK_EQ(parseCode(u, rec, nullptr), M2M_E_UNSUPPORTED);
}

TEST_CASE(media_and_zone_tables_are_kept)
{
    HTSchedule s;
    parse(s, kFull);
    REQUIRE(s.media.size() == 1u);
    CHECK(s.media[0]->name == String(u"Water quench"));
    REQUIRE(s.media[0]->zones.size() == 2u);
    CHECK_EQ(s.media[0]->zones[0]->ndim, 1);           // CNVCOF type 1: var 3, 2 rows
    CHECK_EQ(s.media[0]->zones[0]->n[0], 2);
    CHECK(s.media[0]->zones[0]->axis[0][1] == 10.0);
    CHECK(s.media[0]->zones[0]->values[1] == 2.0);
    CHECK_EQ(s.media[0]->zones[1]->ndim, 0);           // constant
    CHECK(s.media[0]->zones[1]->value == 5000.0);
    CHECK(s.media[0]->radiation);
}

int main()
{
    return testkit::run_all();
}
