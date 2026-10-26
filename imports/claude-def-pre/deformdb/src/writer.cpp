#include "deformdb/writer.hpp"

#include <algorithm>
#include <bit>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <span>
#include <stdexcept>

#include "deformdb/codec.hpp"

namespace deformdb {

namespace {

constexpr int kWordsPerRecord = 128;
constexpr int kEntryWords = 13;
constexpr int kEntriesPerChunk = 128;
constexpr int kChunkRecords = 13;         // 128 entries x 13 words
constexpr int kChunksPerIndex = 127;
constexpr int kMaxIndexRecords = 200;     // "ERROR: Step limit exceeded!!" beyond this
constexpr std::int32_t kFirstIndexRecord = 3;
constexpr std::int32_t kFirstDataRecord = 17;  // after the first index record and chunk
constexpr int kPlatformCode = 2;          // swap mode of the writer, stored in the step header

std::int32_t records_for_bytes(std::size_t bytes) {
    return static_cast<std::int32_t>((bytes + kRecordBytes - 1) / kRecordBytes);
}

template <class T>
void store_be(std::byte* p, T v) {
    std::memcpy(p, &v, sizeof(T));
    if constexpr (std::endian::native == std::endian::little)
        std::reverse(p, p + sizeof(T));
}

// Text field padded with blanks to `n` characters.
void put_text(std::byte* p, const std::string& s, std::size_t n) {
    for (std::size_t k = 0; k < n; ++k) p[k] = static_cast<std::byte>(k < s.size() ? s[k] : ' ');
}

std::string current_date() {
    std::time_t t = std::time(nullptr);
    std::tm tm{};
#ifdef _WIN32
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif
    char buf[16];
    std::snprintf(buf, sizeof buf, "%02d/%02d/%02d", tm.tm_mon + 1, tm.tm_mday, tm.tm_year % 100);
    return buf;
}

bool compressed_step(int dim, int version) { return dim == 2 ? version > 3109 : version > 999; }

}  // namespace

WriterHeader writer_header_from(const FileHeader& source) {
    WriterHeader h;
    h.title = source.title;
    h.release_date = source.dates[0];
    h.ismtyp = source.words[1];
    return h;
}

Writer::Writer(std::unique_ptr<WritableFile> f, WriterHeader h) : file_(std::move(f)), hdr_(std::move(h)) {
    if (hdr_.today.empty()) hdr_.today = current_date();
}

Writer::~Writer() = default;

std::unique_ptr<Writer> Writer::create(const std::filesystem::path& path, WriterHeader header) {
    return std::unique_ptr<Writer>(new Writer(std::make_unique<WritableFile>(path), std::move(header)));
}

void Writer::flush() { file_->flush(); }

// Writes `words` from `record` on, padding the last record with zeros; returns the
// first record after the block.
std::int32_t Writer::write_i32(std::int32_t record, const std::vector<std::int32_t>& words) {
    if (words.empty()) return record;
    const std::int32_t n = records_for_bytes(words.size() * 4);
    std::vector<std::byte> raw(static_cast<std::size_t>(n) * kRecordBytes);
    for (std::size_t k = 0; k < words.size(); ++k) store_be(raw.data() + 4 * k, words[k]);
    file_->write_records(record, raw.data(), raw.size());
    return record + n;
}

std::int32_t Writer::write_f64(std::int32_t record, const std::vector<double>& words) {
    if (words.empty()) return record;
    const std::int32_t n = records_for_bytes(words.size() * 8);
    std::vector<std::byte> raw(static_cast<std::size_t>(n) * kRecordBytes);
    for (std::size_t k = 0; k < words.size(); ++k) store_be(raw.data() + 8 * k, words[k]);
    file_->write_records(record, raw.data(), raw.size());
    return record + n;
}

void Writer::write_signature(int dim) {
    std::byte rec[kRecordBytes]{};
    const char* sig = dim == 2 ? "SFTC-DEF-2D-DB100-00" : "SFTC-DEF-3D-DB100-00";
    std::memcpy(rec, sig, 20);
    file_->write_records(1, rec, kRecordBytes);
}

// Region-wise MAR storage (3D, version >= 6000): table first, then every region
// with a non-zero length on its own records, then the table again with the
// stored sizes and records filled in.
void Writer::write_region_array_i32(const StepData& s, std::array<std::int32_t, 13>& e) {
    std::vector<std::int32_t> t = s.mar_table;
    if (t.empty()) throw std::runtime_error("step " + std::to_string(s.entry.step) + ": MAR region table missing");
    const bool packed = compressed_step(e[7], e[8]);
    const std::int32_t table_rec = nxtrec_;
    e[3] = static_cast<std::int32_t>(t.size());
    nxtrec_ = write_i32(table_rec, t);
    const int r_count = t[0];
    for (int r = 1; r <= r_count && static_cast<std::size_t>(4 * r + 1) < t.size(); ++r) {
        const std::int32_t len = t[4 * r - 2];
        const std::int32_t start = t[4 * r - 1];
        if (len == 0) {
            t[4 * r] = 0;
            t[4 * r + 1] = nxtrec_;
            continue;
        }
        if (start < 1 || static_cast<std::size_t>(start - 1 + len) > s.mar.size())
            throw std::runtime_error("MAR region " + std::to_string(r) + " outside the array");
        std::span<const std::int32_t> part(s.mar.data() + start - 1, static_cast<std::size_t>(len));
        std::vector<std::int32_t> data = packed ? rle_encode(part) : std::vector<std::int32_t>(part.begin(), part.end());
        t[4 * r] = static_cast<std::int32_t>(data.size());
        t[4 * r + 1] = nxtrec_;
        nxtrec_ = write_i32(nxtrec_, data);
    }
    write_i32(table_rec, t);
}

void Writer::write_region_array_f64(const StepData& s, std::array<std::int32_t, 13>& e) {
    std::vector<std::int32_t> t = s.a_table;
    if (t.empty()) throw std::runtime_error("step " + std::to_string(s.entry.step) + ": A region table missing");
    const bool packed = compressed_step(e[7], e[8]);
    const std::int32_t table_rec = nxtrec_;
    e[4] = static_cast<std::int32_t>(t.size());
    nxtrec_ = write_i32(table_rec, t);
    const int r_count = t[0];
    for (int r = 1; r <= r_count && static_cast<std::size_t>(4 * r + 1) < t.size(); ++r) {
        const std::int32_t len = t[4 * r - 2];
        const std::int32_t start = t[4 * r - 1];
        if (len == 0) {
            t[4 * r] = 0;
            t[4 * r + 1] = nxtrec_;
            continue;
        }
        if (start < 1 || static_cast<std::size_t>(start - 1 + len) > s.a.size())
            throw std::runtime_error("A region " + std::to_string(r) + " outside the array");
        std::span<const double> part(s.a.data() + start - 1, static_cast<std::size_t>(len));
        std::vector<double> data = packed ? rle_encode(part) : std::vector<double>(part.begin(), part.end());
        t[4 * r] = static_cast<std::int32_t>(data.size());
        t[4 * r + 1] = nxtrec_;
        nxtrec_ = write_f64(nxtrec_, data);
    }
    write_i32(table_rec, t);
}

// Dynamic arrays (version >= 11000): descriptor, integer table, real table, then
// every array RLE-compressed; tables and descriptor are rewritten at the end.
void Writer::write_dynamic(const StepData& s, std::array<std::int32_t, 13>& e) {
    auto d = s.dynamic_descriptor;
    const int ni = d[0], nr = d[10];
    if (ni < 1 && nr < 1) {
        e[9] = 0;
        return;
    }
    const std::int32_t desc_rec = nxtrec_;
    e[9] = desc_rec;
    std::vector<std::int32_t> desc(d.begin(), d.end());
    nxtrec_ = write_i32(desc_rec, desc);

    auto make_table = [](int n, int len, const std::vector<std::array<std::int32_t, 10>>& rows) {
        std::vector<std::int32_t> t(static_cast<std::size_t>(std::max(len, 10 * n)), 0);
        for (int k = 0; k < n && static_cast<std::size_t>(k) < rows.size(); ++k)
            std::copy(rows[static_cast<std::size_t>(k)].begin(), rows[static_cast<std::size_t>(k)].end(),
                      t.begin() + 10 * k);
        return t;
    };
    std::vector<std::int32_t> ti, tr;
    if (ni > 0) {
        d[2] = nxtrec_;
        ti = make_table(ni, d[3], s.dynamic_int_rows);
        d[3] = static_cast<std::int32_t>(ti.size());
        nxtrec_ = write_i32(d[2], ti);
    } else {
        d[2] = 0;
    }
    if (nr > 0) {
        d[12] = nxtrec_;
        tr = make_table(nr, d[13], s.dynamic_real_rows);
        d[13] = static_cast<std::int32_t>(tr.size());
        nxtrec_ = write_i32(d[12], tr);
    } else {
        d[12] = 0;
    }

    for (int k = 0; k < ni; ++k) {
        std::int32_t* row = ti.data() + 10 * k;
        if (row[0] == 0) {
            row[2] = 0;
            row[3] = 0;
            continue;
        }
        std::vector<std::int32_t> values(static_cast<std::size_t>(row[0]), 0);
        for (const auto& arr : s.dynamic_ints)
            if (arr.slot == k + 1) values = arr.values;
        auto data = rle_encode(std::span<const std::int32_t>(values));
        row[3] = nxtrec_;
        row[2] = static_cast<std::int32_t>(data.size());
        nxtrec_ = write_i32(nxtrec_, data);
    }
    if (ni > 0) write_i32(d[2], ti);

    for (int k = 0; k < nr; ++k) {
        std::int32_t* row = tr.data() + 10 * k;
        if (row[0] == 0) {
            row[2] = 0;
            row[3] = 0;
            continue;
        }
        std::vector<double> values(static_cast<std::size_t>(row[0]), 0.0);
        for (const auto& arr : s.dynamic_reals)
            if (arr.slot == k + 1) values = arr.values;
        auto data = rle_encode(std::span<const double>(values));
        row[3] = nxtrec_;
        row[2] = static_cast<std::int32_t>(data.size());
        nxtrec_ = write_f64(nxtrec_, data);
    }
    if (nr > 0) write_i32(d[12], tr);

    desc.assign(d.begin(), d.end());
    write_i32(desc_rec, desc);
}

// A new directory chunk (13 records) starts with every 128th entry; every 127th
// chunk also needs a new index record, linked from word 128 of the previous one.
void Writer::allocate_directory_chunk(std::int32_t slot) {
    if (slot == 1) {
        index_.assign(1, {});
        index_[0][0] = kFirstIndexRecord + 1;
        nxtrec_ = kFirstDataRecord;
        return;
    }
    if ((slot - 1) % kEntriesPerChunk != 0) return;
    const int chunk = (slot - 1) / kEntriesPerChunk;   // 0-based
    const int j = chunk / kChunksPerIndex;
    const int pos = chunk % kChunksPerIndex;
    if (pos == 0) {
        if (j >= kMaxIndexRecords) throw std::runtime_error("step limit exceeded");
        index_[static_cast<std::size_t>(j - 1)][kWordsPerRecord - 1] = nxtrec_;
        ++nxtrec_;
        index_.emplace_back();
    }
    index_[static_cast<std::size_t>(j)][static_cast<std::size_t>(pos)] = nxtrec_;
    nxtrec_ += kChunkRecords;
}

void Writer::write_file_header(const StepEntry& last) {
    std::byte rec[kRecordBytes]{};
    char smi1[16];
    std::snprintf(smi1, sizeof smi1, "%5.2f   ", last.version / 1000.0);
    put_text(rec, hdr_.title, 80);
    put_text(rec + 80, smi1, 8);
    put_text(rec + 88, hdr_.release_date, 8);
    if (created_.empty() || last.step == -1) created_ = hdr_.today;
    put_text(rec + 96, created_, 8);
    put_text(rec + 104, hdr_.today, 8);
    const std::int32_t words[6] = {
        last.slot,                              // ISTART: entry just written
        last.dim == 3 ? 1 : hdr_.ismtyp,        // ISMTYP
        entry_count(),                          // NRSETS
        nxtrec_,                                // NXTREC
        1,                                      // IPTYPE
        0};
    for (int k = 0; k < 6; ++k) store_be(rec + 112 + 4 * k, words[k]);
    file_->write_records(2, rec, kRecordBytes);
}

void Writer::write_index_records() {
    for (std::size_t j = 0; j < index_.size(); ++j) {
        if (static_cast<std::int64_t>(j) * kEntriesPerChunk * kChunksPerIndex >= entry_count()) break;
        const std::int32_t rec = j == 0 ? kFirstIndexRecord : index_[j - 1][kWordsPerRecord - 1];
        std::vector<std::int32_t> words(index_[j].begin(), index_[j].end());
        write_i32(rec, words);
    }
}

void Writer::write_directory_chunk(std::int32_t slot) {
    const int chunk = (slot - 1) / kEntriesPerChunk;
    const std::int32_t rec =
        index_[static_cast<std::size_t>(chunk / kChunksPerIndex)][static_cast<std::size_t>(chunk % kChunksPerIndex)];
    std::vector<std::int32_t> words(static_cast<std::size_t>(kEntriesPerChunk) * kEntryWords, 0);
    const std::size_t first = static_cast<std::size_t>(chunk) * kEntriesPerChunk;
    for (std::size_t k = first; k < dir_.size() && k < first + kEntriesPerChunk; ++k)
        std::copy(dir_[k].begin(), dir_[k].end(), words.begin() + static_cast<std::ptrdiff_t>((k - first) * kEntryWords));
    write_i32(rec, words);
}

StepEntry Writer::append_step(const StepData& s, bool header_only) {
    const auto& src = s.entry;
    if (src.dim != 2 && src.dim != 3) throw std::runtime_error("step dimension must be 2 or 3");
    if (s.header.empty()) throw std::runtime_error("step " + std::to_string(src.step) + ": header missing");
    const bool arrays = !header_only && !s.mar.empty() && !s.a.empty();
    const std::int32_t slot = entry_count() + 1;

    write_signature(src.dim);
    allocate_directory_chunk(slot);

    std::array<std::int32_t, 13> e{};
    e[0] = src.mesh_no;
    e[1] = src.step;
    e[2] = static_cast<std::int32_t>(s.header.size());
    e[3] = arrays ? static_cast<std::int32_t>(s.mar.size()) : 0;
    e[4] = arrays ? static_cast<std::int32_t>(s.a.size()) : 0;
    e[5] = src.sim_no;
    e[6] = src.sim_mode;
    e[7] = src.dim;
    e[8] = src.version;

    // The writer records the platform code and the full MAR/A lengths in the header.
    std::vector<double> header = s.header;
    header.resize(std::max<std::size_t>(header.size(), 9), 0.0);
    if (src.dim == 3) {
        header[5] = kPlatformCode;
        header[6] = e[3];
        header[7] = e[4];
    } else {
        header[6] = e[3];
        header[7] = e[4];
        header[8] = kPlatformCode;
    }
    header.resize(s.header.size());
    e[10] = nxtrec_;
    nxtrec_ = write_f64(nxtrec_, header);

    const bool regions = src.dim == 3 && src.version >= 6000;
    const bool packed = compressed_step(src.dim, src.version);
    e[11] = nxtrec_;
    if (e[3] > 0) {
        if (regions) {
            write_region_array_i32(s, e);
        } else {
            auto data = packed ? rle_encode(std::span<const std::int32_t>(s.mar)) : s.mar;
            e[3] = static_cast<std::int32_t>(data.size());
            nxtrec_ = write_i32(nxtrec_, data);
        }
    }
    if (e[4] > 0) {
        e[12] = nxtrec_;
        if (regions) {
            write_region_array_f64(s, e);
        } else {
            auto data = packed ? rle_encode(std::span<const double>(s.a)) : s.a;
            e[4] = static_cast<std::int32_t>(data.size());
            nxtrec_ = write_f64(nxtrec_, data);
        }
        write_dynamic(s, e);
    }

    dir_.push_back(e);
    StepEntry out = src;
    out.slot = slot;
    out.header_size = e[2];
    out.mar_size = e[3];
    out.a_size = e[4];
    out.rec_dynamic = e[9];
    out.rec_header = e[10];
    out.rec_mar = e[11];
    out.rec_a = e[12];

    write_file_header(out);
    write_index_records();
    write_directory_chunk(slot);
    return out;
}

}  // namespace deformdb
