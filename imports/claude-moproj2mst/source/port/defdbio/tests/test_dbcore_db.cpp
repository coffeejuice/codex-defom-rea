// Tier-1 tests of the DB core on real DEFORM databases, always on copies:
//   * reads (ops 0, 1, 2, 3, 5, region selectors, step search) against deformdb's reader, which
//     was verified against DEFORM's own KEY export of the same steps;
//   * op 4 against DEF_PRE itself: every DB that the oracle cache holds was written by DEF_PRE;
//     rewriting all its steps in order (slots 1..N) through op 4 must give the same bytes.
// Inputs: the COG test DB (C:\PROBLEM\TEST_REA\COG_P1_B1, copied to a temp directory), the
// Inc718 test DB read from a prepared copy (DEF_P112_INC718, default
// C:\Users\alext\Projects\deform-rea-work\p112_dbcopies\Inc718.DB; skipped when absent), and
// the DBs under %USERPROFILE%\.deform-oracle\entries (copied; skipped when absent).
// ctest-labels: p1-12 dbcore corpus
#define _CRT_SECURE_NO_WARNINGS
#include <defdbio/dbcore/dbops.hpp>
#include <defdbio/dbcore/ftn_io.hpp>
#include <defdbio/dbcore/globals_p1_12.hpp>
#include <deformdb/database.hpp>
#include <testkit/check.hpp>

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <random>
#include <string>
#include <vector>

#include "../src/dbcore/internal.hpp"

using namespace def::dbio::dbcore;
namespace fs = std::filesystem;

namespace {

const fs::path kCog = R"(C:\PROBLEM\TEST_REA\COG_P1_B1\COG_P1_B1.DB)";

fs::path temp_dir()
{
    static fs::path d = fs::temp_directory_path() /
                        ("p112_db_" + std::to_string(std::random_device{}()));
    fs::create_directories(d);
    return d;
}

fs::path inc718_copy()
{
    const char* e = std::getenv("DEF_P112_INC718");
    return e ? fs::path(e) : fs::path(R"(C:\Users\alext\Projects\deform-rea-work\p112_dbcopies\Inc718.DB)");
}

std::vector<fs::path> oracle_dbs()
{
    std::vector<fs::path> r;
    const char* home = std::getenv("USERPROFILE");
    if (!home) {
        return r;
    }
    const fs::path root = fs::path(home) / ".deform-oracle" / "entries";
    std::error_code ec;
    if (!fs::exists(root, ec)) {
        return r;
    }
    for (auto it = fs::recursive_directory_iterator(root, ec); it != fs::recursive_directory_iterator(); it.increment(ec)) {
        if (ec) {
            break;
        }
        const auto& p = it->path();
        if (it->is_regular_file(ec) && p.extension() == ".DB" && fs::file_size(p, ec) < 40u * 1024 * 1024) {
            r.push_back(p);
        }
    }
    std::sort(r.begin(), r.end());
    return r;
}

fs::path copy_to_temp(const fs::path& src, const std::string& name)
{
    const fs::path dst = temp_dir() / name;
    fs::copy_file(src, dst, fs::copy_options::overwrite_existing);
    fs::permissions(dst, fs::perms::owner_write, fs::perm_options::add);
    return dst;
}

// Opens `p` on `unit` (mode 4 read/write or 5 read-only) and runs op 0.
bool open_db(const fs::path& p, std::int32_t unit, std::int32_t mode)
{
    reset_globals();
    const std::string s = p.string();
    std::int32_t ios = 0;
    fortran_open(&mode, &unit, s.c_str(), "OLD", &ios, static_cast<std::int32_t>(s.size()), 3);
    if (ios != 0) {
        return false;
    }
    std::int32_t op = 0, slot = 0, step = 0, nh = 0, nmar = 0, na = 0, ierr = -1;
    db_operation(&unit, &op, &slot, &step, &nh, nullptr, &nmar, nullptr, &na, nullptr, &ierr);
    return ierr == 0;
}

void close_unit(std::int32_t unit)
{
    const std::int32_t m0 = 0;
    std::int32_t ios = 0;
    fortran_open(&m0, &unit, "", "", &ios, 0, 0);
}

struct Step {
    std::int32_t ierr2 = 0, ierr3 = 0, ierr5 = 0;
    std::vector<double> h;
    std::vector<std::int32_t> mar;
    std::vector<double> a;
    std::int32_t nmar = 0, na = 0;
};

// Op 2, op 3 and op 5 of entry `slot` the way a caller does them (H first).
Step read_step(std::int32_t unit, std::int32_t slot, const deformdb::StepEntry& e)
{
    Step s;
    s.h.assign(static_cast<std::size_t>(std::max(e.header_size, 1)) + 16, 0.0);
    std::int32_t op = 2, sl = slot, step = 0, nh = 0, nmar = 0, na = 0;
    db_operation(&unit, &op, &sl, &step, &nh, s.h.data(), &nmar, nullptr, &na, nullptr, &s.ierr2);
    const std::int32_t full_mar = std::max(detail::nint(s.h[6]), 0);
    const std::int32_t full_a = std::max(detail::nint(s.h[7]), 0);
    s.mar.assign(static_cast<std::size_t>(std::max(full_mar, e.mar_size)) + 16, 0);
    s.a.assign(static_cast<std::size_t>(std::max(full_a, e.a_size)) + 16, 0.0);
    op = 3;
    sl = slot;
    db_operation(&unit, &op, &sl, &step, &nh, s.h.data(), &nmar, s.mar.data(), &na, s.a.data(), &s.ierr3);
    s.nmar = nmar;
    s.na = na;
    std::memset(g_db.dyn_hdr, 0, sizeof g_db.dyn_hdr);
    op = 5;
    sl = slot;
    db_operation(&unit, &op, &sl, &step, &nh, nullptr, &nmar, nullptr, &na, nullptr, &s.ierr5);
    return s;
}

bool same_bits(const std::vector<double>& x, const std::vector<double>& y, std::size_t n)
{
    if (x.size() < n || y.size() < n) {
        return false;
    }
    return std::memcmp(x.data(), y.data(), n * 8) == 0;
}

// Reads with the DB core and with deformdb, entry by entry for the slots in `slots`.
void compare_reads(const fs::path& p, const std::vector<std::int32_t>& slots_in)
{
    auto ref = deformdb::Database::open(p);
    REQUIRE(ref != nullptr);
    const std::int32_t unit = 61;
    REQUIRE(open_db(p, unit, 5));
    const auto& hdr = ref->header();
    CHECK_EQ(g_db.h.nrsets, hdr.entry_count);
    CHECK_EQ(g_db.file_family, static_cast<std::int32_t>(hdr.family));
    CHECK_EQ(g_db.file_version, hdr.version);
    CHECK(std::string(g_db.h.title, 80).rfind(hdr.title, 0) == 0);

    // Op 1: directory words 1..10 of every entry.
    const auto& ents = ref->entries();
    std::vector<std::int32_t> words(ents.size() * 10 + 10);
    std::int32_t op = 1, slot = 0, step = 0, nh = 0, count = 0, na = 0, ierr = -1;
    db_operation(&unit, &op, &slot, &step, &nh, nullptr, &count, words.data(), &na, nullptr, &ierr);
    CHECK_EQ(ierr, 0);
    CHECK_EQ(count, static_cast<std::int32_t>(ents.size()));
    bool dir_ok = true;
    for (std::size_t i = 0; i < ents.size(); ++i) {
        const auto& e = ents[i];
        const std::int32_t* w = &words[i * 10];
        dir_ok = dir_ok && w[0] == e.mesh_no && w[1] == e.step && w[2] == e.header_size &&
                 w[3] == e.mar_size && w[4] == e.a_size && w[5] == e.sim_no && w[6] == e.sim_mode &&
                 w[7] == e.dim && w[8] == e.version && w[9] == e.rec_dynamic;
    }
    CHECK(dir_ok);

    std::vector<std::int32_t> slots = slots_in;
    if (slots.empty()) {
        for (std::size_t i = 1; i <= ents.size(); ++i) {
            slots.push_back(static_cast<std::int32_t>(i));
        }
    }
    for (std::int32_t sl : slots) {
        if (sl < 1 || sl > static_cast<std::int32_t>(ents.size())) {
            continue;
        }
        const auto& e = ents[static_cast<std::size_t>(sl - 1)];
        const deformdb::StepData d = ref->read_step(e);
        const Step s = read_step(unit, sl, e);
        CHECK_EQ(s.ierr2, 0);
        CHECK(same_bits(s.h, d.header, d.header.size()));
        if (d.has_arrays()) {
            CHECK_EQ(s.ierr3, 0);
            CHECK_EQ(s.nmar, static_cast<std::int32_t>(d.mar.size()));
            CHECK_EQ(s.na, static_cast<std::int32_t>(d.a.size()));
            CHECK(std::equal(d.mar.begin(), d.mar.end(), s.mar.begin()));
            CHECK(same_bits(s.a, d.a, d.a.size()));
        } else {
            CHECK_EQ(s.ierr3, 0xca);
        }
        CHECK_EQ(s.ierr5, 0);
        if (e.version >= 11000 && e.rec_dynamic != 0) {
            CHECK_EQ(g_db.dyn_hdr[0], static_cast<std::int32_t>(d.dynamic_ints.size()));
            CHECK_EQ(g_db.dyn_hdr[10], static_cast<std::int32_t>(d.dynamic_reals.size()));
            bool dyn_ok = true;
            for (std::size_t k = 0; k < d.dynamic_ints.size(); ++k) {
                const auto& v = d.dynamic_ints[k].values;
                const std::int32_t* p2 = dyn_int_data(static_cast<std::int32_t>(k + 1));
                dyn_ok = dyn_ok && (v.empty() || (p2 && std::equal(v.begin(), v.end(), p2)));
            }
            for (std::size_t k = 0; k < d.dynamic_reals.size(); ++k) {
                const auto& v = d.dynamic_reals[k].values;
                const double* p2 = dyn_real_data(static_cast<std::int32_t>(k + 1));
                dyn_ok = dyn_ok && (v.empty() || (p2 && std::memcmp(v.data(), p2, v.size() * 8) == 0));
            }
            CHECK(dyn_ok);
        }
        // Step search (slot < 1): the first entry with this step number.
        std::vector<double> h2(s.h.size(), 0.0);
        std::int32_t op2 = 2, sl2 = 0, st2 = e.step, nh2 = 0, nm2 = 0, na2 = 0, ie2 = -1;
        db_operation(&unit, &op2, &sl2, &st2, &nh2, h2.data(), &nm2, nullptr, &na2, nullptr, &ie2);
        CHECK_EQ(ie2, 0);
        CHECK_EQ(sl2, ref->find_step(e.step)->slot);
        // Region selector: one MAR region by its start address (op -30 - start).
        if (!d.mar_regions.empty()) {
            const auto it = std::find_if(d.mar_regions.begin(), d.mar_regions.end(),
                                         [](const deformdb::Region& r) { return r.stored > 0 && r.start > 0; });
            if (it != d.mar_regions.end()) {
                std::vector<std::int32_t> one(static_cast<std::size_t>(std::max(it->stored, it->length)) + 16, 0);
                std::int32_t op3 = -30 - it->start, sl3 = sl, st3 = 0, nh3 = 0, nm3 = 0, na3 = 0, ie3 = -1;
                std::vector<double> h3 = s.h;
                db_operation(&unit, &op3, &sl3, &st3, &nh3, h3.data(), &nm3, one.data(), &na3, nullptr, &ie3);
                CHECK_EQ(ie3, 0);
                CHECK(std::equal(d.mar.begin() + (it->start - 1), d.mar.begin() + (it->start - 1 + it->length), one.begin()));
            }
        }
    }
    close_unit(unit);
}

// Rewrites every entry of `p` in slot order through op 4 and returns whether the file is
// unchanged (DEF_PRE wrote it the same way).
// `last` limits the slots (0: all); with `mask_rec2` record 2 (NRSETS, NXTREC) is not compared.
bool rewrite_all(const fs::path& p, std::string* first_diff, std::int32_t last = 0,
                 bool mask_rec2 = false)
{
    std::vector<char> before;
    {
        std::ifstream f(p, std::ios::binary);
        before.assign(std::istreambuf_iterator<char>(f), {});
    }
    auto ref = deformdb::Database::open(p);
    const auto ents = ref->entries();
    ref.reset();
    const std::int32_t unit = 62;
    if (!open_db(p, unit, 4)) {
        *first_diff = "open failed";
        return false;
    }
    const std::size_t n = last > 0 ? static_cast<std::size_t>(last) : ents.size();
    for (std::size_t i = 0; i < n; ++i) {
        const std::int32_t sl = static_cast<std::int32_t>(i + 1);
        const auto& e = ents[i];
        Step s = read_step(unit, sl, e);
        if (s.ierr2 != 0 || (s.ierr3 != 0 && s.ierr3 != 0xca) || s.ierr5 != 0) {
            *first_diff = "read error at slot " + std::to_string(sl);
            return false;
        }
        if (s.ierr3 == 0xca) {
            s.nmar = 0;
            s.na = 0;
        }
        g_db.put_mesh_no = e.mesh_no;
        g_db.put_step = e.step;
        g_db.put_sim_no = e.sim_no;
        g_db.put_sim_mode = e.sim_mode;
        std::int32_t op = 4, slot = sl, step = e.step, nh = e.header_size, ierr = -1;
        db_operation(&unit, &op, &slot, &step, &nh, s.h.data(), &s.nmar, s.mar.data(), &s.na, s.a.data(), &ierr);
        if (ierr != 0) {
            *first_diff = "op 4 error " + std::to_string(ierr) + " at slot " + std::to_string(sl);
            return false;
        }
    }
    close_unit(unit);
    std::vector<char> after;
    {
        std::ifstream f(p, std::ios::binary);
        after.assign(std::istreambuf_iterator<char>(f), {});
    }
    if (after.size() != before.size()) {
        *first_diff = "size " + std::to_string(before.size()) + " -> " + std::to_string(after.size());
    }
    if (mask_rec2 && before.size() >= 1024 && after.size() >= 1024) {
        std::fill(before.begin() + 512, before.begin() + 1024, '\0');
        std::fill(after.begin() + 512, after.begin() + 1024, '\0');
    }
    const auto mm = std::mismatch(before.begin(), before.end(), after.begin(), after.end());
    if (mm.first != before.end() || mm.second != after.end()) {
        const auto off = static_cast<long long>(mm.first - before.begin());
        *first_diff += " first byte difference at " + std::to_string(off) + " (record " +
                       std::to_string(off / 512 + 1) + ", byte " + std::to_string(off % 512) + ")";
        return false;
    }
    return first_diff->empty();
}

// Rewrites the last entry through the append path of op 4 (slot 0) after taking it off the
// directory (NRSETS - 1, NXTREC = its header record): the file must come out unchanged.
bool append_last(const fs::path& p, std::string* first_diff)
{
    std::vector<char> before;
    {
        std::ifstream f(p, std::ios::binary);
        before.assign(std::istreambuf_iterator<char>(f), {});
    }
    auto ref = deformdb::Database::open(p);
    const auto ents = ref->entries();
    ref.reset();
    if (ents.size() < 2) {
        return true;
    }
    const std::int32_t unit = 63;
    if (!open_db(p, unit, 4)) {
        *first_diff = "open failed";
        return false;
    }
    const auto& e = ents.back();
    const std::int32_t sl = static_cast<std::int32_t>(ents.size());
    Step s = read_step(unit, sl, e);
    if (s.ierr3 == 0xca) {
        s.nmar = 0;
        s.na = 0;
    }
    g_db.h.nrsets = sl - 1;
    g_db.h.nxtrec = e.rec_header;
    g_db.put_interval = 1; // keep every step
    g_db.put_mesh_no = e.mesh_no;
    g_db.put_step = e.step;
    g_db.put_sim_no = e.sim_no;
    g_db.put_sim_mode = e.sim_mode;
    std::int32_t op = 4, slot = 0, step = e.step, nh = e.header_size, ierr = -1;
    db_operation(&unit, &op, &slot, &step, &nh, s.h.data(), &s.nmar, s.mar.data(), &s.na, s.a.data(), &ierr);
    close_unit(unit);
    if (ierr != 0 || slot != sl) {
        *first_diff = "op 4 error " + std::to_string(ierr) + ", slot " + std::to_string(slot);
        return false;
    }
    std::vector<char> after;
    {
        std::ifstream f(p, std::ios::binary);
        after.assign(std::istreambuf_iterator<char>(f), {});
    }
    const auto mm = std::mismatch(before.begin(), before.end(), after.begin(), after.end());
    if (mm.first != before.end() || mm.second != after.end()) {
        const auto off = static_cast<long long>(mm.first - before.begin());
        *first_diff = "first byte difference at record " + std::to_string(off / 512 + 1) + ", byte " +
                      std::to_string(off % 512);
        return false;
    }
    return true;
}

} // namespace

TEST_CASE(cog_reads_match_deformdb)
{
    if (!fs::exists(kCog)) {
        std::printf("skip: %s not found\n", kCog.string().c_str());
        return;
    }
    compare_reads(copy_to_temp(kCog, "COG_read.DB"), {});
}

TEST_CASE(inc718_reads_match_deformdb)
{
    const fs::path p = inc718_copy();
    if (!fs::exists(p)) {
        std::printf("skip: %s not found\n", p.string().c_str());
        return;
    }
    auto ref = deformdb::Database::open(p);
    const std::int32_t n = static_cast<std::int32_t>(ref->entries().size());
    ref.reset();
    compare_reads(p, {1, 2, 128, 129, n / 2, n - 1, n});
}

TEST_CASE(oracle_dbs_reads_match_deformdb)
{
    int k = 0;
    for (const auto& src : oracle_dbs()) {
        compare_reads(copy_to_temp(src, "oracle_read_" + std::to_string(k++) + ".DB"), {});
    }
    std::printf("oracle DBs read: %d\n", k);
}

TEST_CASE(cog_rewrite_is_byte_identical)
{
    if (!fs::exists(kCog)) {
        return;
    }
    // The solver wrote COG and later turned steps 4..10 into header-only entries (op 4's
    // step-interval rule), so their headers still hold the array lengths: rewriting those
    // entries cannot give the same bytes. Slots 1..7 come before the first of them; after
    // rewriting them only NRSETS / NXTREC in record 2 differ.
    std::string diff;
    const bool same = rewrite_all(copy_to_temp(kCog, "COG_rewrite.DB"), &diff, 7, true);
    if (!same) {
        std::printf("COG rewrite: %s\n", diff.c_str());
    }
    CHECK(same);
}

TEST_CASE(oracle_dbs_rewrite_is_byte_identical)
{
    int k = 0, ok = 0;
    for (const auto& src : oracle_dbs()) {
        std::string diff;
        const bool same = rewrite_all(copy_to_temp(src, "oracle_rw_" + std::to_string(k++) + ".DB"), &diff);
        if (!same) {
            std::printf("%s: %s\n", src.filename().string().c_str(), diff.c_str());
        }
        ok += same ? 1 : 0;
        CHECK(same);
    }
    std::printf("oracle DBs rewritten identically: %d of %d\n", ok, k);
}

TEST_CASE(append_path_reproduces_the_last_step)
{
    std::vector<fs::path> dbs = oracle_dbs();
    if (fs::exists(kCog)) {
        dbs.push_back(kCog);
    }
    int k = 0;
    for (const auto& src : dbs) {
        std::string diff;
        const bool same = append_last(copy_to_temp(src, "append_" + std::to_string(k++) + ".DB"), &diff);
        if (!same) {
            std::printf("%s: %s\n", src.filename().string().c_str(), diff.c_str());
        }
        CHECK(same);
    }
}

TEST_CASE(signature_checks_on_copies)
{
    if (!fs::exists(kCog)) {
        return;
    }
    const fs::path p = copy_to_temp(kCog, "COG_sig.DB");
    std::int32_t err = 0;
    CHECK_EQ(is_deform_db(p.string().c_str(), &err), 1);
    CHECK_EQ(err, 0);
    const fs::path junk = temp_dir() / "junk.DB";
    { std::ofstream(junk, std::ios::binary) << std::string(1024, 'x'); }
    err = 0;
    CHECK_EQ(is_deform_db(junk.string().c_str(), &err), 0);
    CHECK_EQ(err, 0);
    err = 0;
    CHECK_EQ(is_deform_db((temp_dir() / "none.DB").string().c_str(), &err), 0);
    CHECK_EQ(err, 1); // OPEN fails (no file): every check reports 1
    CHECK_EQ(is_deform_db(nullptr, &err), 0);
}

int main()
{
    const int r = testkit::run_all();
    close_all_units();
    std::error_code ec;
    fs::remove_all(temp_dir(), ec);
    return r;
}
