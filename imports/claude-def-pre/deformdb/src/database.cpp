#include "deformdb/database.hpp"

#include <algorithm>
#include <atomic>
#include <cmath>
#include <cstring>
#include <exception>
#include <mutex>
#include <stdexcept>
#include <thread>

#include "deformdb/codec.hpp"

namespace deformdb {

namespace {

constexpr int kWordsPerRecord = 128;     // int32 words per record
constexpr int kEntryWords = 13;          // directory entry size
constexpr int kEntriesPerChunk = 128;    // entries per directory chunk (13 records)
constexpr int kChunksPerIndex = 127;     // chunk pointers per index record (+1 link word)
constexpr std::int32_t kFirstIndexRecord = 3;

std::string trim(std::string s) {
    while (!s.empty() && (s.back() == ' ' || s.back() == '\0')) s.pop_back();
    std::size_t b = 0;
    while (b < s.size() && s[b] == ' ') ++b;
    return s.substr(b);
}

std::string chars(const std::byte* p, std::size_t n) {
    return trim(std::string(reinterpret_cast<const char*>(p), n));
}

bool starts_with(std::string_view s, std::string_view p) { return s.substr(0, p.size()) == p; }

}  // namespace

// ---------------------------------------------------------------- versions

int version_from_text(Family family, std::string_view text) {
    std::string t(text);
    while (!t.empty() && t.back() == ' ') t.pop_back();
    struct Pair { const char* s; int v; };
    if (family == Family::Legacy2D) {
        static const Pair tbl[] = {{"5.0", 5000}, {"5.0 CNV", 5000}, {"5.1", 5100}, {"5.1 CNV", 5100},
                                   {"5.2", 5200}, {"5.3", 5300},    {"6.0", 6000}, {"7.0", 7000},
                                   {"7.10", 7100}, {"7.20", 7200},  {"8.00", 8000}, {"8.10", 8100},
                                   {"8.20", 8200}, {"9.00", 9000},  {"9.01", 9010}};
        for (auto& p : tbl) if (t == p.s) return p.v;
        return 9100;
    }
    if (family == Family::Legacy3D) {
        static const Pair tbl[] = {{"2.3", 2300}, {"2.4", 3000}, {"3.0", 3000}, {"3.1.1", 3100},
                                   {"3.2", 3200}, {"3.3", 3300}, {"4.0", 4000}, {"5.0", 5000},
                                   {"5.1", 5100}, {"6.0", 6000}, {"6.01", 6010}};
        for (auto& p : tbl) if (t == p.s) return p.v;
        return 6100;
    }
    if (family == Family::Combined) {
        for (int major = 10; major <= 20; ++major)
            if (t == std::to_string(major) + ".00") return major * 1000;
        return 10000;
    }
    return 0;
}

// ---------------------------------------------------------------- StepData

std::span<const std::int32_t> StepData::mar_region(std::string_view name) const {
    for (const auto& r : mar_regions)
        if (r.name == name && r.start >= 1 && r.length > 0 &&
            static_cast<std::size_t>(r.start - 1 + r.length) <= mar.size())
            return {mar.data() + r.start - 1, static_cast<std::size_t>(r.length)};
    return {};
}

std::span<const double> StepData::a_region(std::string_view name) const {
    for (const auto& r : a_regions)
        if (r.name == name && r.start >= 1 && r.length > 0 &&
            static_cast<std::size_t>(r.start - 1 + r.length) <= a.size())
            return {a.data() + r.start - 1, static_cast<std::size_t>(r.length)};
    return {};
}

// ---------------------------------------------------------------- Database

Database::Database(std::unique_ptr<RandomAccessFile> f) : file_(std::move(f)) {}

std::unique_ptr<Database> Database::open(const std::filesystem::path& path) {
    std::unique_ptr<Database> db(new Database(std::make_unique<RandomAccessFile>(path)));
    db->refresh();
    return db;
}

void Database::refresh() {
    read_file_header();
    read_directory();
}

std::vector<std::int32_t> Database::read_i32(std::int32_t record, std::size_t count) const {
    std::vector<std::byte> raw(count * 4);
    if (count) file_->read_records(record, raw.data(), raw.size());
    std::vector<std::int32_t> v(count);
    decode_be_i32(raw.data(), count, v.data());
    return v;
}

std::vector<double> Database::read_f64(std::int32_t record, std::size_t count) const {
    std::vector<std::byte> raw(count * 8);
    if (count) file_->read_records(record, raw.data(), raw.size());
    std::vector<double> v(count);
    decode_be_f64(raw.data(), count, v.data());
    return v;
}

void Database::read_file_header() {
    std::byte rec1[kRecordBytes];
    std::byte rec2[kRecordBytes];
    file_->read_records(1, rec1, kRecordBytes);
    file_->read_records(2, rec2, kRecordBytes);

    FileHeader h;
    h.signature = chars(rec1, 20);
    if (h.signature == "SFTC-DEFORM-2D-DB450" || h.signature == "SFTC-DEFORM-2D-DB720")
        h.family = Family::Legacy2D;
    else if (h.signature == "SFTC-DEFORM-3D-DB230")
        h.family = Family::Legacy3D;
    else if (h.signature == "SFTC-DEFORM-23-DB100" || h.signature == "SFTC-DEF-2D-DB100-00" ||
             h.signature == "SFTC-DEF-3D-DB100-00")
        h.family = Family::Combined;
    else
        throw std::runtime_error("not a DEFORM database (signature '" + h.signature + "')");

    // Record 2: 28 words of text, then 6 int32 words.
    h.title = chars(rec2, 80);
    h.version_text = chars(rec2 + 80, 8);
    for (int k = 0; k < 3; ++k) h.dates[k] = chars(rec2 + 88 + 8 * k, 8);
    decode_be_i32(rec2 + 112, 6, h.words.data());
    h.entry_count = h.words[2];
    h.version = version_from_text(h.family, h.version_text);
    header_ = std::move(h);
}

void Database::read_directory() {
    std::vector<StepEntry> out;
    const std::int32_t count = header_.entry_count;
    if (count <= 0) {
        entries_.clear();
        return;
    }
    const int chunks = (count + kEntriesPerChunk - 1) / kEntriesPerChunk;

    // Index records form a chain: words 1..127 point at directory chunks,
    // word 128 points at the next index record.
    std::vector<std::int32_t> chunk_records;
    std::int32_t index_record = kFirstIndexRecord;
    while (static_cast<int>(chunk_records.size()) < chunks) {
        if (index_record <= 0) throw std::runtime_error("broken directory index chain");
        auto idx = read_i32(index_record, kWordsPerRecord);
        for (int k = 0; k < kChunksPerIndex && static_cast<int>(chunk_records.size()) < chunks; ++k)
            chunk_records.push_back(idx[k]);
        index_record = idx[kWordsPerRecord - 1];
    }

    out.reserve(static_cast<std::size_t>(count));
    for (int c = 0; c < chunks; ++c) {
        const int in_chunk = std::min(kEntriesPerChunk, count - c * kEntriesPerChunk);
        auto words = read_i32(chunk_records[c], static_cast<std::size_t>(in_chunk) * kEntryWords);
        for (int e = 0; e < in_chunk; ++e) {
            const std::int32_t* w = words.data() + e * kEntryWords;
            StepEntry s;
            s.slot = c * kEntriesPerChunk + e + 1;
            s.mesh_no = w[0];
            s.step = w[1];
            s.header_size = w[2];
            s.mar_size = w[3];
            s.a_size = w[4];
            s.sim_no = w[5];
            s.sim_mode = w[6];
            s.dim = w[7];
            s.version = w[8];
            s.rec_dynamic = w[9];
            s.rec_header = w[10];
            s.rec_mar = w[11];
            s.rec_a = w[12];
            if (header_.family != Family::Combined) {
                // Legacy files: dimension and version are global.
                s.dim = static_cast<int>(header_.family);
                s.version = header_.version;
            }
            out.push_back(s);
        }
    }
    entries_ = std::move(out);
}

std::optional<StepEntry> Database::find_step(std::int32_t step) const {
    for (const auto& e : entries_)
        if (e.step == step) return e;
    return std::nullopt;
}

namespace {

// Region table: T[1] = R, region r at T[4r-1..4r+2] = (length, start, stored, record).
std::vector<Region> parse_region_table(const std::vector<std::int32_t>& t, std::span<const char* const> names) {
    std::vector<Region> regions;
    if (t.empty()) return regions;
    const int r_count = t[0];
    for (int r = 1; r <= r_count; ++r) {
        const std::size_t base = static_cast<std::size_t>(4 * r - 1);  // 1-based index of the length word
        if (base + 3 > t.size()) break;
        Region g;
        g.index = r;
        g.length = t[base - 1];
        g.start = t[base];
        g.stored = t[base + 1];
        g.record = t[base + 2];
        if (static_cast<std::size_t>(r - 1) < names.size()) g.name = names[static_cast<std::size_t>(r - 1)];
        regions.push_back(std::move(g));
    }
    return regions;
}

}  // namespace

void Database::read_region_array_i32(StepData& s) const {
    const auto& e = s.entry;
    auto table = read_i32(e.rec_mar, static_cast<std::size_t>(e.mar_size));
    s.mar_regions = parse_region_table(table, mar_region_names(e.dim, e.version));
    s.mar_table = table;
    const auto total = static_cast<std::size_t>(std::llround(s.header.size() >= 7 ? s.header[6] : 0.0));
    s.mar.assign(total, 0);
    for (const auto& g : s.mar_regions) {
        if (g.stored <= 0) continue;
        auto raw = read_i32(g.record, static_cast<std::size_t>(g.stored));
        std::vector<std::int32_t> data = s.compressed ? rle_decode(raw) : std::move(raw);
        const std::size_t first = static_cast<std::size_t>(g.start - 1);
        if (g.start < 1 || first + data.size() > s.mar.size())
            s.mar.resize(std::max(s.mar.size(), first + data.size()));
        std::copy(data.begin(), data.end(), s.mar.begin() + static_cast<std::ptrdiff_t>(first));
    }
}

void Database::read_region_array_f64(StepData& s) const {
    const auto& e = s.entry;
    auto table = read_i32(e.rec_a, static_cast<std::size_t>(e.a_size));
    s.a_regions = parse_region_table(table, a_region_names(e.dim, e.version));
    s.a_table = table;
    const auto total = static_cast<std::size_t>(std::llround(s.header.size() >= 8 ? s.header[7] : 0.0));
    s.a.assign(total, 0.0);
    for (const auto& g : s.a_regions) {
        if (g.stored <= 0) continue;
        auto raw = read_f64(g.record, static_cast<std::size_t>(g.stored));
        std::vector<double> data = s.compressed ? rle_decode(raw) : std::move(raw);
        const std::size_t first = static_cast<std::size_t>(g.start - 1);
        if (g.start < 1 || first + data.size() > s.a.size())
            s.a.resize(std::max(s.a.size(), first + data.size()));
        std::copy(data.begin(), data.end(), s.a.begin() + static_cast<std::ptrdiff_t>(first));
    }
}

void Database::read_dynamic(StepData& s) const {
    if (s.entry.version < 11000 || s.entry.rec_dynamic <= 0) return;
    auto d = read_i32(s.entry.rec_dynamic, 20);
    std::copy(d.begin(), d.end(), s.dynamic_descriptor.begin());

    auto read_table = [&](int n, int len, int rec) {
        std::vector<std::array<std::int32_t, 10>> rows;
        if (n <= 0 || len <= 0) return rows;
        auto t = read_i32(rec, static_cast<std::size_t>(len));
        for (int k = 0; k < n && static_cast<std::size_t>(10 * k + 10) <= t.size(); ++k) {
            std::array<std::int32_t, 10> row{};
            std::copy_n(t.begin() + 10 * k, 10, row.begin());
            rows.push_back(row);
        }
        return rows;
    };

    const auto ints = read_table(d[0], d[3], d[2]);
    s.dynamic_int_rows = ints;
    for (std::size_t k = 0; k < ints.size(); ++k) {
        const auto& row = ints[k];
        if (row[0] <= 0 || row[2] <= 0) continue;
        DynamicIntArray arr;
        arr.slot = static_cast<int>(k + 1);
        arr.descriptor = row;
        arr.values = rle_decode(read_i32(row[3], static_cast<std::size_t>(row[2])));
        s.dynamic_ints.push_back(std::move(arr));
    }
    const auto reals = read_table(d[10], d[13], d[12]);
    s.dynamic_real_rows = reals;
    for (std::size_t k = 0; k < reals.size(); ++k) {
        const auto& row = reals[k];
        if (row[0] <= 0 || row[2] <= 0) continue;
        DynamicRealArray arr;
        arr.slot = static_cast<int>(k + 1);
        arr.descriptor = row;
        arr.values = rle_decode(read_f64(row[3], static_cast<std::size_t>(row[2])));
        s.dynamic_reals.push_back(std::move(arr));
    }
}

StepData Database::read_step(const StepEntry& e, const ReadOptions& opt) const {
    StepData s;
    s.entry = e;
    s.compressed = (e.dim == 2) ? e.version > 3109 : e.version > 999;

    const bool region_tables = (e.dim == 3 && e.version >= 6000);
    if ((opt.header || (opt.arrays && region_tables)) && e.header_size > 0)
        s.header = read_f64(e.rec_header, static_cast<std::size_t>(e.header_size));

    if (opt.arrays && e.mar_size > 0 && e.a_size > 0) {
        if (region_tables) {
            read_region_array_i32(s);
            read_region_array_f64(s);
        } else {
            auto m = read_i32(e.rec_mar, static_cast<std::size_t>(e.mar_size));
            s.mar = s.compressed ? rle_decode(m) : std::move(m);
            auto a = read_f64(e.rec_a, static_cast<std::size_t>(e.a_size));
            s.a = s.compressed ? rle_decode(a) : std::move(a);
        }
    }
    if (opt.dynamic) read_dynamic(s);
    return s;
}

void Database::read_steps(std::span<const StepEntry> list, const ReadOptions& opt, unsigned threads,
                          const std::function<void(StepData&&)>& sink) const {
    if (threads == 0) threads = std::max(1u, std::thread::hardware_concurrency());
    threads = std::min<unsigned>(threads, static_cast<unsigned>(std::max<std::size_t>(1, list.size())));
    std::atomic<std::size_t> next{0};
    std::exception_ptr failure;
    std::mutex failure_mutex;
    auto worker = [&] {
        for (;;) {
            const std::size_t k = next.fetch_add(1);
            if (k >= list.size()) return;
            try {
                sink(read_step(list[k], opt));
            } catch (...) {
                std::lock_guard lock(failure_mutex);
                if (!failure) failure = std::current_exception();
                next = list.size();
                return;
            }
        }
    };
    std::vector<std::thread> pool;
    for (unsigned t = 1; t < threads; ++t) pool.emplace_back(worker);
    worker();
    for (auto& t : pool) t.join();
    if (failure) std::rethrow_exception(failure);
}

}  // namespace deformdb
