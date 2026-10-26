// ctest-labels: p67-5 commands
// TCYCLE (dispatcher case 0x12): every case of tests/commands/TCYCLE against the oracle, and the
// step-header cache of the DB info object against an independent reader (deformdb).
//
// DBREAD (FUN_1401cc2a0 / FUN_1401d7f40) is not ported (phase 2). The test emulates only what
// TCYCLE reads after it (static analysis of the DBREAD dispatcher case 2 and FUN_1401cc2a0 /
// FUN_1401cc420):
//   * model+0x3390 = a Cls_1401dbfe0 loaded by load_db_info(db, name, 0) (FUN_1401dc3f0, as the
//     case does), on a copy of the DB;
//   * the step: FUN_1401e3b60 (last entry with arrays whose step is not "after" the requested one,
//     FUN_1401e3d40 order), then FUN_1401cc2a0 takes the FIRST directory index with that step and
//     FUN_1401cc420 stores it in model+0x740;
//   * the case's stdout line "DBREAD : Read step %d from database".
// DBREAD also reads the step header of that entry into the cache (FUN_1401dd4d0, db+0x1900 stays
// 0): not emulated, TCYCLE's FUN_1401dd250 re-reads every entry anyway.
//
// Without the real DBREAD the rest of the model is the fresh one, so the KEY records compared are
// only what TCYCLE determines: DTMAX[0] (model+0x920). The DTMAX writer prints it only when the
// time flag model+0x990 is 1 (DTMAX.md); the steps of the DSMAX cases load a model with the flag
// != 1 (their OUT.KEY shows DSMAX), so their value is checked against the formula on TNOWs read
// with deformdb, and for the gendb case through GENDB's warning (see the case notes below).
#include "p67_5_support.hpp"

#include <defcmd/dispatch.hpp>
#include <cmath>

#include <deformdb/database.hpp>
#include <defcmd/p67_5.hpp>
#include <defdbio/dbcore/ftn_io.hpp>
#include <defdbio/step/db_header_cache.hpp>
#include <defdbio/step/db_info_read.hpp>
#include <defdbio/step/globals_p67_5.hpp>
#include <defkey/kw_g1a.hpp>
#include <defmodel/layout/cls_1401dbfe0.hpp>
#include <testkit/check.hpp>

namespace {

using def::model::Cls_1401dbfe0;
using def::model::DbDirRecord;
using def::model::DbInfoEntryE0;
namespace st = def::dbio::step;

void legacy_printf()
{
    unsigned long long& o = *__local_stdio_printf_options();
    o = (o | _CRT_INTERNAL_PRINTF_LEGACY_THREE_DIGIT_EXPONENTS | _CRT_INTERNAL_PRINTF_LEGACY_MSVCRT_COMPATIBILITY) &
        ~static_cast<unsigned long long>(_CRT_INTERNAL_PRINTF_STANDARD_ROUNDING);
}

double& dtmax0(p675::DB_STEP_DATA& m) { return p675::at<double>(m, 0x920); }

// FUN_1401e3d40 order of step numbers (step s < 1 counts as -s - 0.5, i.e. just before |s|):
// written here only for the DBREAD emulation (step resolution of FUN_1401e3b60)
int step_cmp(int a, int b)
{
    if (a == b) return 0;
    double x = a < 1 ? -1.0 * a - 0.5 : a; // DAT_140522fc8 = -1.0, DAT_14051f240 = 0.5
    double y = b < 1 ? -1.0 * b - 0.5 : b;
    return x <= y ? -1 : 1;
}

int resolve_step(const Cls_1401dbfe0& db, int step)
{
    const DbDirRecord* r = def::model::db_info_records(db);
    for (int i = db.uac8 - 1; i >= 0; --i)
        if (!(0 < step_cmp(r[i].step, step)) && r[i].stored != 0) return r[i].step;
    return 0;
}

std::string port_dtmax_record(p675::DB_STEP_DATA& m)
{
    p675::fs::path p = p675::tmp_name("p675_dtmax.key");
    std::FILE* f = std::fopen(p.string().c_str(), "wb");
    def::key::g1a::write_dtmax(f, &m, {});
    std::fclose(f);
    std::string s = p675::slurp(p);
    p675::fs::remove(p);
    return s;
}

// TNOW span of operation `sim` read with deformdb: H[3] of the last entry with arrays minus H[2]
// of the first (TCYCLE.md 4.6-4.7; tLast stays 0 with a single entry)
double deformdb_span(const p675::fs::path& dbfile, int sim)
{
    auto d = deformdb::Database::open(dbfile);
    deformdb::ReadOptions ro;
    ro.arrays = false;
    ro.dynamic = false;
    bool first = true;
    double t0 = 0.0, t1 = 0.0;
    for (const auto& e : d->entries()) {
        if (e.mar_size < 1 || e.sim_no != sim) continue;
        auto s = d->read_step(e, ro);
        if (first) {
            t0 = s.header[2];
            first = false;
        } else {
            t1 = s.header[3];
        }
    }
    double span = t1 - t0;
    return span < 0.0 ? 0.0 : span;
}

struct CaseArgs {
    int k, n;
    double T, d2, d3;
};

} // namespace

TEST_CASE(header_cache_matches_deformdb)
{
    p675::fs::path src = "C:/Users/alext/Projects/DEFORM_corpus/db/COG_P1_B1.DB";
    if (!p675::fs::exists(src)) {
        std::printf("corpus DB missing: skipped\n");
        return;
    }
    p675::fs::path copy = p675::tmp_name("p675_tc_cache.DB");
    p675::fs::copy_file(src, copy, p675::fs::copy_options::overwrite_existing);
    st::reset_globals_p67_5();
    {
        auto* db = new Cls_1401dbfe0();
        CHECK_EQ(def::dbio::load_db_info(*db, copy.string().c_str(), false), 0);
        st::load_header_cache(*db, false);
        CHECK_EQ(static_cast<int>(db->u1900), 1);
        CHECK(!def::dbio::dbcore::unit_is_open(3));
        auto d = deformdb::Database::open(copy);
        deformdb::ReadOptions ro;
        ro.arrays = false;
        ro.dynamic = false;
        CHECK_EQ(static_cast<int>(d->entries().size()), db->uac8);
        int checked = 0;
        DbInfoEntryE0* c = st::cache_entries(*db);
        for (int i = 0; i < db->uac8; ++i) {
            auto s = d->read_step(d->entries()[i], ro);
            CHECK_EQ(st::entry_i32(c[i], 0x00), static_cast<int>(s.header[0]));
            CHECK_EQ(st::entry_i32(c[i], 0x04), static_cast<int>(s.header[1]));
            CHECK_BITS(st::entry_f64(c[i], 0x08), s.header[2]);
            CHECK_BITS(st::entry_f64(c[i], 0x10), s.header[3]);
            const int nobj = st::entry_i32(c[i], 0x00);
            CHECK(nobj > 0 && st::entry_objects(c[i]) != nullptr);
            // the stroke: |x,y,z| at items 3,4,5 of object PDIE-1 (3D, version 11000: block start
            // H[12], H[9] words per object when > 0, else 0x22 / 200; FUN_1401e1de0)
            const int pd = st::entry_i32(c[i], 0x04) - 1;
            const int first = static_cast<int>(s.header[12]) > 0 ? static_cast<int>(s.header[12]) : 0x22;
            const int stride = static_cast<int>(s.header[9]) > 0 ? static_cast<int>(s.header[9]) : 200;
            if (0 <= pd && pd < nobj) {
                const double* b = s.header.data() + first + stride * pd;
                CHECK_BITS(st::entry_f64(c[i], 0x28), std::sqrt((b[3] * b[3] + b[4] * b[4]) + b[5] * b[5]));
            }
            ++checked;
        }
        // first/last of operation 6 (steps -13..24): TNOW of the first and of the last stored entry
        int dim = 0, nobj = 0, pdie = 0, s0 = 0, s1 = 0;
        double t0 = 0, t1 = 0, a0 = 0, a1 = 0;
        CHECK(st::sim_first_last(*db, 6, &dim, &nobj, &pdie, &s0, &s1, &t0, &t1, &a0, &a1));
        std::printf("op 6: dim %d nobj %d steps %d..%d TNOW %.17g..%.17g\n", dim, nobj, s0, s1, t0, t1);
        CHECK_EQ(s0, -13);
        CHECK_EQ(s1, 24);
        std::printf("%d cache records checked against deformdb\n", checked);
        delete db;
    }
    p675::fs::remove(copy);
}

TEST_CASE(tcycle_cases_match_oracle)
{
    legacy_printf();
    if (p675::repo_root().empty()) {
        std::printf("no repository root: skipped\n");
        return;
    }
    const int id_tcycle = p675::kw_id("TCYCLE");
    const int id_dbread = p675::kw_id("DBREAD");
    int checked = 0;
    for (const p675::Case& c : p675::load_cases("TCYCLE")) {
        p675::fs::path e = p675::oracle_entry(c.label);
        if (e.empty()) {
            std::printf("%s: not in the oracle cache, skipped\n", c.label.c_str());
            continue;
        }
        st::reset_globals_p67_5();
        auto& m = *new p675::DB_STEP_DATA(3);
        auto* db = new Cls_1401dbfe0(); // main always links one (14001c770)
        m.db = db;
        p675::fs::path dbcopy;
        CaseArgs args{};
        bool have_args = false;
        p675::CmdRunner run(m, c);
        run.on_command = [&](int id, std::FILE* f) -> int {
            char buf[1024];
            if (id == id_dbread) {
                int step = 0;
                p675::CmdRunner::line(f, buf, sizeof buf);
                std::sscanf(buf, "%i", &step);
                p675::CmdRunner::line(f, buf, sizeof buf);
                dbcopy = p675::tmp_name(std::string("p675_tc_") + buf);
                p675::fs::copy_file(c.inputs.at(buf), dbcopy, p675::fs::copy_options::overwrite_existing);
                run.temps.push_back(dbcopy);
                if (def::dbio::load_db_info(*db, dbcopy.string().c_str(), false) != 0) return 2;
                const int got = resolve_step(*db, step);
                if (got == step)
                    std::printf("DBREAD : Read step %d from database\n", got);
                else
                    std::printf("DBREAD : Read step %d (<- %d) from database\n", got, step);
                const DbDirRecord* r = def::model::db_info_records(*db);
                for (int i = 0; i < db->uac8; ++i) {
                    if (r[i].step == got) {
                        m.u740 = i;
                        break;
                    }
                }
                return 0;
            }
            if (id == id_tcycle) {
                // peek the arguments for the expected-value check, then rewind the line
                long pos = std::ftell(f);
                if (std::fgets(buf, sizeof buf, f) != nullptr)
                    have_args = std::sscanf(buf, "%i %i %lf %lf %lf", &args.k, &args.n, &args.T, &args.d2, &args.d3) == 5;
                std::fseek(f, pos, SEEK_SET);
                return def::cmd::dispatch(&m, f, id);  // FUN_140378e00
            }
            return -1;
        };
        std::string out;
        {
            p675::StdoutCapture cap;
            run.run();
            out = cap.text();
        }
        CHECK(run.unsupported.empty());
        // stdout: the lines of DBREAD and TCYCLE (the rest of the keyword phase is the seed's)
        auto pick = [](const std::string& s) {
            std::string r;
            for (const std::string& l : p675::lines_of(s))
                if (l.rfind("DBREAD", 0) == 0 || l.rfind("ERROR: Total cycle", 0) == 0) r += l + "\n";
            return r;
        };
        std::string orig_out = p675::keyword_phase_stdout(p675::slurp(e / "stdout.txt"));
        std::string port_out;
        for (const std::string& l : p675::lines_of(out))
            if (!l.empty()) port_out += l + "\n";
        CHECK_EQ(pick(orig_out), pick(port_out));
        if (pick(orig_out) != pick(port_out))
            std::printf("%s stdout\n orig:\n%s port:\n%s", c.label.c_str(), orig_out.c_str(), port_out.c_str());
        if (orig_out != port_out)
            std::printf("%s: note, full keyword-phase stdout differs\n orig:\n%s port:\n%s", c.label.c_str(),
                        orig_out.c_str(), port_out.c_str());

        // DTMAX[0]
        std::string okey = p675::slurp(e / "files" / "OUT.KEY");
        auto orec = p675::records_of(okey);
        std::string what;
        if (orec.count("DTMAX") != 0) {
            if (!dbcopy.empty()) p675::at<std::int32_t>(m, 0x990) = 1; // DBREAD's step had the time flag (see top)
            int bad = p675::compare_records(okey, port_dtmax_record(m), {"DTMAX"}, true, c.label);
            CHECK_EQ(bad, 0);
            what = bad == 0 ? "DTMAX record equal" : "DTMAX record MISMATCH";
        } else if (!dbcopy.empty()) {
            // not visible in OUT.KEY (DSMAX step): the formula on TNOWs read with deformdb
            double expect = 0.0;
            bool changes = false;
            const int sim0 = st::entry_sim_no(*db, m.u740);
            if (have_args && sim0 > 0 && args.k > 0) {
                int sim = sim0 - (args.d2 > 0.0 ? 1 : 0) - (args.d3 > 0.0 ? 1 : 0);
                double span = deformdb_span(dbcopy, sim);
                double total = (span + args.d2) + args.d3;
                expect = ((1.0 + static_cast<double>(args.n)) * args.T - total) / static_cast<double>(args.k);
                changes = true;
            }
            if (changes) {
                CHECK_BITS(dtmax0(m), expect);
            } else {
                CHECK_BITS(dtmax0(m), 0.0); // unchanged from the fresh model
            }
            char b[160];
            std::snprintf(b, sizeof b, "DTMAX[0] = %.10E (%s)", dtmax0(m), changes ? "formula on deformdb TNOWs" : "unchanged");
            what = b;
            if (c.label == "TCYCLE/gendb") {
                // GENDB's check (gendb_ann.c, FUN_1401... sim-control check) prints "DSMAX and
                // DTMAX cannot be defined together" and zeroes S+0x1d0 when both are > 0: the
                // original's DTMAX was > 0 here, and NEW.DB has LLSIM[3] = 0.
                bool warn = p675::slurp(e / "stdout.txt").find("DSMAX and DTMAX cannot be defined together") != std::string::npos;
                CHECK(warn && dtmax0(m) > 0.0);
                what += warn ? "; oracle GENDB warning confirms DTMAX > 0" : "";
            }
        }
        std::printf("%s: %s\n", c.label.c_str(), what.c_str());
        delete db;
        m.db = nullptr;
        ++checked;
    }
    std::printf("%d TCYCLE cases checked\n", checked);
}

int main() { return testkit::run_all(); }
