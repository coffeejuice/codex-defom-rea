// Tier-3 check of the comparator on real outputs. Every input is copied twice
// into WORKDIR (reference "ref_*" and working copy) and only the copies are read;
// differences are planted in the working copy; both are deleted at the end.
//
//   test_defcmp_corpus WORKDIR [--db FILE.DB]... [--key FILE.KEY]...
//
// Per DB: original vs. copy is equal by digest; a changed date word (SMI4) and a
// changed ISTART are masked; the same date is reported with masks off; a changed
// value in the first stored A region named LLCRZ is reported with that step,
// region and keyword RZ.
// Per KEY: original vs. copy is equal; a changed digit in an RZ data line and a
// changed keyword line are reported with their line and keyword.
#include <algorithm>
#include <chrono>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <string>
#include <vector>

#include "deformdb/database.hpp"
#include "defcmp/defcmp.hpp"

using namespace defcmp;
namespace ddb = deformdb;

static int failures = 0;
#define CHECK(c)                                                                         \
    do {                                                                                 \
        if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } \
    } while (0)
#define CHECK_HAS(s, sub)                                                                                  \
    do {                                                                                                   \
        if ((s).find(sub) == std::string::npos) {                                                          \
            std::printf("FAIL %s:%d \"%s\" does not contain \"%s\"\n", __FILE__, __LINE__, (s).c_str(), std::string(sub).c_str()); \
            ++failures;                                                                                    \
        }                                                                                                  \
    } while (0)

static double seconds_since(std::chrono::steady_clock::time_point t) {
    return std::chrono::duration<double>(std::chrono::steady_clock::now() - t).count();
}

static std::vector<unsigned char> read_at(const fs::path& p, std::uint64_t off, std::size_t n) {
    std::vector<unsigned char> b(n);
    std::ifstream f(p, std::ios::binary);
    f.seekg(static_cast<std::streamoff>(off));
    f.read(reinterpret_cast<char*>(b.data()), static_cast<std::streamsize>(n));
    return b;
}

static void write_at(const fs::path& p, std::uint64_t off, const std::vector<unsigned char>& b) {
    std::fstream f(p, std::ios::binary | std::ios::in | std::ios::out);
    f.seekp(static_cast<std::streamoff>(off));
    f.write(reinterpret_cast<const char*>(b.data()), static_cast<std::streamsize>(b.size()));
}

static void report(const char* what, const Result& r, double secs) {
    std::printf("  %-34s %6.2f s  %s\n", what, secs, r.summary().c_str());
}

static void check_db(const fs::path& original, const fs::path& work) {
    std::printf("DB %s (%llu bytes)\n", original.string().c_str(), static_cast<unsigned long long>(fs::file_size(original)));
    const fs::path src = work / ("ref_" + original.filename().string());
    const fs::path copy = work / original.filename();
    auto t = std::chrono::steady_clock::now();
    fs::copy_file(original, src, fs::copy_options::overwrite_existing);
    fs::copy_file(src, copy, fs::copy_options::overwrite_existing);
    std::printf("  copied twice in %.2f s\n", seconds_since(t));

    t = std::chrono::steady_clock::now();
    auto r = compare_db(src, copy, DbMasks{});
    report("identical copy", r, seconds_since(t));
    CHECK(r.equal && r.by_digest);

    // Cached expected digest: the expected file is not read at all.
    const Digest d = db_digest(src, DbMasks{});
    t = std::chrono::steady_clock::now();
    r = compare_db(work / "not-there.DB", copy, DbMasks{}, d);
    report("identical copy, cached digest", r, seconds_since(t));
    CHECK(r.equal && r.by_digest);

    {   // date word SMI4: masked by default, reported without masks
        const auto old = read_at(copy, 512 + 104, 8);
        write_at(copy, 512 + 104, {'0', '1', '/', '0', '1', '/', '9', '9'});
        t = std::chrono::steady_clock::now();
        r = compare_db(src, copy, DbMasks{});
        report("SMI4 changed, default masks", r, seconds_since(t));
        CHECK(r.equal);
        r = compare_db(src, copy, DbMasks::none());
        report("SMI4 changed, no masks", r, 0);
        CHECK(!r.equal);
        CHECK_HAS(r.where, "record 2 SMI4 (last written)");
        CHECK_HAS(r.actual, "01/01/99");
        write_at(copy, 512 + 104, old);
    }
    {   // ISTART
        const auto old = read_at(copy, 512 + 112, 4);
        auto v = old;
        v[3] ^= 0x5A;
        write_at(copy, 512 + 112, v);
        r = compare_db(src, copy, DbMasks{});
        report("ISTART changed, default masks", r, 0);
        CHECK(r.equal);
        r = compare_db(src, copy, *DbMasks::parse("-istart"));
        report("ISTART changed, -istart", r, 0);
        CHECK_HAS(r.where, "record 2 ISTART");
        write_at(copy, 512 + 112, old);
    }
    {   // value in a known region: lowest byte of the first stored word of LLCRZ.
        // That word is a literal or a run value, never a count or link, so the
        // file still decodes and exactly that coordinate (or run) changes.
        auto db = ddb::Database::open(copy);
        ddb::ReadOptions ro;
        ro.dynamic = false;
        bool planted = false;
        for (const auto& e : db->entries()) {
            if (e.mar_size == 0 || e.a_size == 0 || e.dim != 3 || e.version < 6000) continue;
            const auto s = db->read_step(e, ro);
            for (const auto& g : s.a_regions) {
                if (g.name != "LLCRZ" || g.stored <= 0) continue;
                const std::uint64_t off = (static_cast<std::uint64_t>(g.record) - 1) * 512 + 7;
                const auto old = read_at(copy, off, 1);
                write_at(copy, off, {static_cast<unsigned char>(old[0] ^ 0x01)});
                t = std::chrono::steady_clock::now();
                r = compare_db(src, copy, DbMasks{});
                report("LLCRZ value changed", r, seconds_since(t));
                CHECK(!r.equal);
                CHECK_HAS(r.where, "step " + std::to_string(e.step) + " (slot " + std::to_string(e.slot) + ")");
                CHECK_HAS(r.where, "region LLCRZ");
                CHECK_HAS(r.where, "keyword RZ");
                write_at(copy, off, old);
                planted = true;
                break;
            }
            if (planted) break;
        }
        CHECK(planted);
    }
    r = compare_db(src, copy, DbMasks::none());
    CHECK(r.equal);   // every planted change was undone
    fs::remove(copy);
    fs::remove(src);
}

static void check_key(const fs::path& original, const fs::path& work) {
    std::printf("KEY %s (%llu bytes)\n", original.string().c_str(), static_cast<unsigned long long>(fs::file_size(original)));
    const fs::path src = work / ("ref_" + original.filename().string());
    const fs::path copy = work / ("copy_" + original.filename().string());
    fs::copy_file(original, src, fs::copy_options::overwrite_existing);
    fs::copy_file(src, copy, fs::copy_options::overwrite_existing);
    auto t = std::chrono::steady_clock::now();
    auto r = compare_key(src, copy);
    report("identical copy", r, seconds_since(t));
    CHECK(r.equal && r.by_digest);

    std::string text;
    {
        std::ifstream f(src, std::ios::binary);
        text.assign(std::istreambuf_iterator<char>(f), {});
    }
    auto line_of = [&](std::size_t off) {
        return static_cast<std::size_t>(std::count(text.begin(), text.begin() + static_cast<std::ptrdiff_t>(off), '\n')) + 1;
    };
    auto plant = [&](std::size_t off, char c) {
        std::string t2 = text;
        t2[off] = c;
        std::ofstream f(copy, std::ios::binary | std::ios::trunc);
        f.write(t2.data(), static_cast<std::streamsize>(t2.size()));
    };

    // A digit in the first data line after the first RZ keyword line.
    const auto rz = text.find("\nRZ ");
    CHECK(rz != std::string::npos);
    if (rz != std::string::npos) {
        const auto data = text.find('\n', rz + 1) + 1;
        std::size_t p = data;
        while (p < text.size() && !(text[p] >= '1' && text[p] <= '8')) ++p;
        plant(p, static_cast<char>(text[p] + 1));
        t = std::chrono::steady_clock::now();
        r = compare_key(src, copy);
        report("RZ data digit changed", r, seconds_since(t));
        CHECK(!r.equal);
        CHECK_HAS(r.where, "line " + std::to_string(line_of(data)) + " col");
        CHECK_HAS(r.where, "keyword RZ (line " + std::to_string(line_of(rz + 1)));
    }
    // The NSTART keyword line.
    const auto ns = text.find("\nNSTART");
    CHECK(ns != std::string::npos);
    if (ns != std::string::npos) {
        auto last = text.find('\n', ns + 1) - 1;   // last digit, before any '\r'
        if (text[last] == '\r') --last;
        plant(last, text[last] == '9' ? '0' : static_cast<char>(text[last] + 1));
        r = compare_key(src, copy);
        report("NSTART line changed", r, 0);
        CHECK_HAS(r.where, "line " + std::to_string(line_of(ns + 1)) + " col");
        CHECK_HAS(r.where, "keyword NSTART");
    }
    fs::remove(copy);
    fs::remove(src);
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::fprintf(stderr, "usage: test_defcmp_corpus WORKDIR [--db FILE]... [--key FILE]...\n");
        return 2;
    }
    const fs::path work = argv[1];
    fs::create_directories(work);
    for (int i = 2; i + 1 < argc; i += 2) {
        const std::string opt = argv[i];
        const fs::path f = argv[i + 1];
        if (!fs::exists(f)) {
            std::printf("FAIL %s does not exist\n", f.string().c_str());
            ++failures;
            continue;
        }
        if (opt == "--db") check_db(f, work);
        else if (opt == "--key") check_key(f, work);
    }
    std::printf(failures ? "%d failures\n" : "all corpus checks passed\n", failures);
    return failures == 0 ? 0 : 1;
}
