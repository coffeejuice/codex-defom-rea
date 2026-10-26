// .DB files: masked digest first; on a mismatch, the first differing unmasked byte
// is located and explained through deformdb as step / region / keyword.
// Layout references: docs/db-format-spec.md §1-§7.
#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <memory>
#include <set>

#include "deformdb/codec.hpp"
#include "deformdb/database.hpp"
#include "deformdb/file.hpp"
#include "deformdb/model.hpp"
#include "internal.hpp"

namespace defcmp {

namespace {

constexpr std::uint64_t R = deformdb::kRecordBytes;   // 512
constexpr int kEntryWords = 13;
constexpr int kEntriesPerChunk = 128;
constexpr int kChunksPerIndex = 127;

// Record 2 fields (byte offset inside the record, length, name).
struct HeaderField { std::uint64_t off, len; const char* name; bool text; };
constexpr HeaderField kHeaderFields[] = {
    {0, 80, "TITLE", true},   {80, 8, "SMI1 (version)", true},  {88, 8, "SMI2 (release date)", true},
    {96, 8, "SMI3 (first written)", true}, {104, 8, "SMI4 (last written)", true},
    {112, 4, "ISTART", false}, {116, 4, "ISMTYP", false}, {120, 4, "NRSETS", false},
    {124, 4, "NXTREC", false}, {128, 4, "IPTYPE", false},  {132, 4, "word 34", false},
};

constexpr const char* kEntryWordNames[kEntryWords] = {
    "MeshNo", "StepNo", "HSize", "MARSize", "ASize", "SimNo", "SimMode", "DimStep", "VerStep",
    "dynamic descriptor record", "header record", "MAR record", "A record"};

std::string fmt_real(double v) {
    char b[64];
    std::uint64_t bits;
    std::memcpy(&bits, &v, 8);
    std::snprintf(b, sizeof b, "%.17g (0x%016llX)", v, static_cast<unsigned long long>(bits));
    return b;
}

bool same_bits(double a, double b) { return std::memcmp(&a, &b, 8) == 0; }

std::string step_label(const deformdb::StepEntry& e) {
    return "step " + std::to_string(e.step) + " (slot " + std::to_string(e.slot) + ")";
}

// Directory entry locations, read straight from the index chain (deformdb does
// not expose file offsets).
struct RawEntry {
    std::int32_t slot;
    std::uint64_t offset;   // file offset of word 1
    std::array<std::int32_t, kEntryWords> w;
};

struct Layout {
    std::uint64_t size = 0;
    std::set<std::int64_t> index_records;
    std::vector<RawEntry> entries;
    std::int32_t nrsets = 0;
};

Layout read_layout(const fs::path& path) {
    Layout L;
    deformdb::RandomAccessFile f(path);
    L.size = f.size();
    if (L.size < 2 * R) return L;
    std::byte rec2[R];
    f.read_records(2, rec2, R);
    L.nrsets = deformdb::load_be_i32(rec2 + 120);
    if (L.nrsets < 0) L.nrsets = 0;

    std::int64_t index_rec = 3;
    std::vector<std::int32_t> index(128);
    std::int64_t loaded_index = -1;   // which index record (0-based in chain) is loaded
    std::vector<std::byte> chunk(kEntriesPerChunk * kEntryWords * 4);
    std::int64_t loaded_chunk = -1;
    std::int32_t chunk_rec = 0;
    for (std::int32_t e = 1; e <= L.nrsets; ++e) {
        const std::int64_t c = (e - 1) / kEntriesPerChunk;
        const std::int64_t ir = c / kChunksPerIndex;
        while (loaded_index < ir) {
            if (loaded_index >= 0) index_rec = index[127];
            if (index_rec <= 0 || static_cast<std::uint64_t>(index_rec) * R > L.size) return L;
            std::byte buf[R];
            f.read_records(index_rec, buf, R);
            for (int k = 0; k < 128; ++k) index[static_cast<std::size_t>(k)] = deformdb::load_be_i32(buf + 4 * k);
            L.index_records.insert(index_rec);
            ++loaded_index;
        }
        if (loaded_chunk != c) {
            chunk_rec = index[static_cast<std::size_t>(c % kChunksPerIndex)];
            if (chunk_rec <= 0 || (static_cast<std::uint64_t>(chunk_rec) - 1) * R + chunk.size() > L.size) return L;
            f.read_records(chunk_rec, chunk.data(), chunk.size());
            loaded_chunk = c;
        }
        const std::size_t pos = static_cast<std::size_t>((e - 1) % kEntriesPerChunk);
        RawEntry re;
        re.slot = e;
        re.offset = (static_cast<std::uint64_t>(chunk_rec) - 1) * R + pos * kEntryWords * 4;
        for (int k = 0; k < kEntryWords; ++k)
            re.w[static_cast<std::size_t>(k)] = deformdb::load_be_i32(chunk.data() + (pos * kEntryWords + k) * 4);
        L.entries.push_back(re);
    }
    return L;
}

bool header_only(const RawEntry& e) { return e.w[3] == 0 && e.w[4] == 0; }

std::vector<ByteRange> mask_ranges(const Layout& L, const DbMasks& m) {
    std::vector<ByteRange> v;
    if (L.size >= 2 * R) {
        if (m.smi2) v.push_back({R + 88, 8});
        if (m.smi3) v.push_back({R + 96, 8});
        if (m.smi4) v.push_back({R + 104, 8});
        if (m.istart) v.push_back({R + 112, 4});
    }
    for (const auto& e : L.entries) {
        if (!header_only(e)) continue;
        if (m.ho_dir) {
            v.push_back({e.offset + 9 * 4, 4});    // word 10
            v.push_back({e.offset + 12 * 4, 4});   // word 13
        }
        if (m.ho_h7h8 && e.w[2] >= 8 && e.w[10] > 0)
            v.push_back({(static_cast<std::uint64_t>(e.w[10]) - 1) * R + 6 * 8, 16});
    }
    for (const auto& r : m.raw) v.push_back(r);
    std::sort(v.begin(), v.end(), [](const ByteRange& a, const ByteRange& b) { return a.offset < b.offset; });
    std::vector<ByteRange> merged;
    for (const auto& r : v) {
        if (r.length == 0) continue;
        if (!merged.empty() && r.offset <= merged.back().offset + merged.back().length)
            merged.back().length = std::max(merged.back().length, r.offset + r.length - merged.back().offset);
        else merged.push_back(r);
    }
    return merged;
}

// Zeroes the masked bytes of buf, which holds file bytes [pos, pos + n).
void apply_masks(std::vector<char>& buf, std::uint64_t pos, std::size_t n, const std::vector<ByteRange>& masks) {
    auto it = std::upper_bound(masks.begin(), masks.end(), pos,
                               [](std::uint64_t p, const ByteRange& r) { return p < r.offset + r.length; });
    for (; it != masks.end() && it->offset < pos + n; ++it) {
        const std::uint64_t b = std::max(it->offset, pos), e = std::min(it->offset + it->length, pos + n);
        if (e > b) std::memset(buf.data() + (b - pos), 0, static_cast<std::size_t>(e - b));
    }
}

Digest masked_digest(const fs::path& path, const std::vector<ByteRange>& masks) {
    std::ifstream f(path, std::ios::binary);
    if (!f) throw std::runtime_error("cannot open " + path.string());
    std::vector<char> buf(1 << 22);
    Hasher h;
    std::uint64_t pos = 0;
    while (f) {
        f.read(buf.data(), static_cast<std::streamsize>(buf.size()));
        const auto n = static_cast<std::size_t>(f.gcount());
        apply_masks(buf, pos, n, masks);
        h.update(buf.data(), n);
        pos += n;
    }
    return h.finish();
}

// First byte that differs outside the masks of either file; nullopt if none
// (sizes are compared by the caller).
std::optional<std::uint64_t> first_difference(const fs::path& a, const fs::path& b, const std::vector<ByteRange>& ma,
                                              const std::vector<ByteRange>& mb) {
    std::ifstream fa(a, std::ios::binary), fb(b, std::ios::binary);
    std::vector<char> ba(1 << 22), bb(1 << 22);
    std::uint64_t pos = 0;
    for (;;) {
        fa.read(ba.data(), static_cast<std::streamsize>(ba.size()));
        fb.read(bb.data(), static_cast<std::streamsize>(bb.size()));
        const auto na = static_cast<std::size_t>(fa.gcount()), nb = static_cast<std::size_t>(fb.gcount());
        const std::size_t n = std::min(na, nb);
        if (n == 0) return std::nullopt;
        apply_masks(ba, pos, n, ma);
        apply_masks(bb, pos, n, ma);
        apply_masks(ba, pos, n, mb);
        apply_masks(bb, pos, n, mb);
        if (std::memcmp(ba.data(), bb.data(), n) != 0)
            for (std::size_t i = 0; i < n; ++i)
                if (ba[i] != bb[i]) return pos + i;
        pos += n;
        if (na != nb) return std::nullopt;
    }
}

// ---------------------------------------------------------------- explaining a difference

// Where a word of the full MAR or A array lives: region and keyword.
struct RegionHit {
    std::string region;
    std::int32_t start = 0;   // 1-based
    std::int32_t width = 0;
};

std::optional<RegionHit> locate(const deformdb::StepData& s, bool mar, std::size_t index0) {
    const auto pos = static_cast<std::int32_t>(index0 + 1);
    const auto& e = s.entry;
    // Address tables (version >= 11000): IIAD1 = MAR[1..140], IIAD2 follows.
    if (e.version >= 11000 && s.mar.size() >= 2) {
        const auto names = mar ? deformdb::mar_region_names(e.dim, e.version) : deformdb::a_region_names(e.dim, e.version);
        const std::size_t slots = mar ? 70 : 140;
        const std::int64_t i1 = static_cast<std::int64_t>(s.mar[1]) - s.mar[0];   // I1 = MAR[2] - MAR[1]
        const std::size_t base = mar ? 0 : static_cast<std::size_t>(std::max<std::int64_t>(i1, 0));
        if ((mar || i1 > 0) && s.mar.size() >= base + 2 * slots) {
            for (std::size_t k = 0; k + 1 < slots; ++k) {
                const auto b = s.mar[base + k], en = s.mar[base + k + 1];
                if (b > 0 && pos >= b && pos < en) {
                    RegionHit h;
                    h.region = k < names.size() ? names[k] : "#" + std::to_string(k + 1);
                    h.start = b;
                    h.width = s.mar[base + slots + k];
                    return h;
                }
            }
        }
    }
    // Stored region tables (3D, version >= 6000).
    for (const auto& r : mar ? s.mar_regions : s.a_regions)
        if (r.length > 0 && pos >= r.start && pos < r.start + r.length)
            return RegionHit{r.name.empty() ? "#" + std::to_string(r.index) : r.name, r.start, 0};
    return std::nullopt;
}

std::string describe_word(const deformdb::StepData& s, bool mar, std::size_t index0) {
    std::string d = std::string(mar ? "MAR" : "A") + "[" + std::to_string(index0 + 1) + "]";
    const auto hit = locate(s, mar, index0);
    if (!hit) return d;
    const std::size_t off = index0 + 1 - static_cast<std::size_t>(hit->start);
    d += ", region " + hit->region + " word " + std::to_string(off + 1);
    std::string kws;
    const deformdb::KeywordInfo* info = nullptr;
    for (const auto& k : deformdb::keyword_table())
        if (hit->region == k.region) {
            if (!kws.empty()) kws += "/";
            kws += k.keyword;
            if (!info) info = &k;
        }
    if (!kws.empty()) d += ", keyword " + kws;
    if (hit->width > 0) {
        const std::size_t row = off / static_cast<std::size_t>(hit->width);
        d += ", row " + std::to_string(row + 1) + " comp " + std::to_string(off % static_cast<std::size_t>(hit->width) + 1);
        if (info) {
            deformdb::StepModel model(s);
            const bool node = info->kind == deformdb::FieldKind::NodeReal || info->kind == deformdb::FieldKind::NodeInt;
            for (const auto& o : model.objects()) {
                const auto first = node ? o.first_node : o.first_elem;
                const auto count = node ? o.node_count : o.elem_count;
                const auto g = static_cast<std::int64_t>(row) + 1;
                if (count > 0 && g >= first && g < first + count) {
                    d += ", object " + std::to_string(o.number) + (node ? " node " : " element ") +
                         std::to_string(g - first + 1);
                    break;
                }
            }
        }
    }
    return d;
}

std::string header_word_name(const deformdb::StepData& s, std::size_t i0) {
    static const char* global[] = {"objects", "PDIE", "start time", "end time", "sim number", "platform code",
                                   "MAR length", "A length", "H9", "object stride", "operation number",
                                   "process type", "global part length"};
    const std::size_t i = i0 + 1;
    std::string n = "H" + std::to_string(i);
    const auto& h = s.header;
    std::size_t F = 0, S = 0;
    if (s.entry.version >= 11000 && h.size() >= 13) {
        F = static_cast<std::size_t>(h[12]);
        S = static_cast<std::size_t>(h[9]);
    }
    if (i <= 13) return n + " (" + global[i - 1] + ")";
    if (i >= 17 && i <= 24) return n + " (OPRNAM)";
    if (i >= 25 && i <= 34) return n + " (SIMNAM)";
    if (F > 0 && S > 0 && i > F)
        return n + " (object " + std::to_string((i - F - 1) / S + 1) + " block offset " + std::to_string((i - F - 1) % S) + ")";
    return n;
}

struct Diff {
    std::string where, expected, actual;
};

template <class T>
std::optional<std::size_t> first_mismatch(const std::vector<T>& a, const std::vector<T>& b) {
    const std::size_t n = std::min(a.size(), b.size());
    for (std::size_t i = 0; i < n; ++i)
        if (std::memcmp(&a[i], &b[i], sizeof(T)) != 0) return i;
    if (a.size() != b.size()) return n;
    return std::nullopt;
}

// Logical comparison of one directory slot. With `layout`, record numbers
// (directory words 10-13, region-table and dynamic-table records) are compared too.
std::optional<Diff> compare_slot(const deformdb::Database& de, const deformdb::Database& da, const deformdb::StepEntry& ee,
                                 const deformdb::StepEntry& ea, const DbMasks& m, bool layout) {
    const std::int32_t we[] = {ee.mesh_no, ee.step, ee.header_size, ee.mar_size, ee.a_size, ee.sim_no, ee.sim_mode, ee.dim, ee.version};
    const std::int32_t wa[] = {ea.mesh_no, ea.step, ea.header_size, ea.mar_size, ea.a_size, ea.sim_no, ea.sim_mode, ea.dim, ea.version};
    for (int k = 0; k < 9; ++k)
        if (we[k] != wa[k])
            return Diff{step_label(ee) + ", directory word " + std::to_string(k + 1) + " " + kEntryWordNames[k],
                        std::to_string(we[k]), std::to_string(wa[k])};

    const auto se = de.read_step(ee), sa = da.read_step(ea);
    const bool ho = ee.mar_size == 0 && ee.a_size == 0;
    {
        auto he = se.header, ha = sa.header;
        if (ho && m.ho_h7h8 && he.size() >= 8 && ha.size() >= 8) {
            he[6] = ha[6];
            he[7] = ha[7];
        }
        if (auto i = first_mismatch(he, ha))
            return Diff{step_label(ee) + ", step header " + header_word_name(se, *i),
                        *i < he.size() ? fmt_real(he[*i]) : "<none>", *i < ha.size() ? fmt_real(ha[*i]) : "<none>"};
    }
    // Region tables without their record words (4r+2).
    auto table_diff = [&](const std::vector<std::int32_t>& te, const std::vector<std::int32_t>& ta, const char* what,
                          const std::vector<deformdb::Region>& regs) -> std::optional<Diff> {
        const std::size_t n = std::max(te.size(), ta.size());
        for (std::size_t i = 0; i < n; ++i) {
            // 1-based 4r+1 (stored words) and 4r+2 (record) follow from the encoding
            // and placement: compared only after the decoded values.
            const bool record_word = i >= 2 && (i + 1) % 4 == 2;
            const bool stored_word = i >= 2 && (i + 1) % 4 == 1;
            if ((record_word || stored_word) && !layout) continue;
            const auto a = i < te.size() ? te[i] : 0, b = i < ta.size() ? ta[i] : 0;
            if (i < te.size() && i < ta.size() && a == b) continue;
            std::string w = step_label(ee) + ", " + what + " region table word " + std::to_string(i + 1);
            if (i >= 2) {
                const std::size_t r = (i + 1 + 1) / 4;   // word 4r-1..4r+2 -> r
                if (r >= 1 && r <= regs.size()) w += " (region " + regs[r - 1].name + ")";
                if (record_word) w += " (record number)";
                if (stored_word) w += " (stored word count)";
            }
            return Diff{w, i < te.size() ? std::to_string(a) : "<none>", i < ta.size() ? std::to_string(b) : "<none>"};
        }
        return std::nullopt;
    };
    if (auto d = table_diff(se.mar_table, sa.mar_table, "MAR", se.mar_regions)) return d;
    if (auto d = table_diff(se.a_table, sa.a_table, "A", se.a_regions)) return d;
    if (auto i = first_mismatch(se.mar, sa.mar))
        return Diff{step_label(ee) + ", " + describe_word(se, true, *i),
                    *i < se.mar.size() ? std::to_string(se.mar[*i]) : "<none>",
                    *i < sa.mar.size() ? std::to_string(sa.mar[*i]) : "<none>"};
    if (auto i = first_mismatch(se.a, sa.a))
        return Diff{step_label(ee) + ", " + describe_word(se, false, *i),
                    *i < se.a.size() ? fmt_real(se.a[*i]) : "<none>", *i < sa.a.size() ? fmt_real(sa.a[*i]) : "<none>"};

    // Dynamic arrays: descriptor words 3/13 and row words 2/4 are records or handles.
    for (std::size_t k = 0; k < 20; ++k) {
        if (!layout && (k == 2 || k == 12)) continue;
        if (se.dynamic_descriptor[k] != sa.dynamic_descriptor[k])
            return Diff{step_label(ee) + ", dynamic-array descriptor word " + std::to_string(k + 1),
                        std::to_string(se.dynamic_descriptor[k]), std::to_string(sa.dynamic_descriptor[k])};
    }
    auto rows_diff = [&](const auto& re, const auto& ra, const char* what) -> std::optional<Diff> {
        if (re.size() != ra.size())
            return Diff{step_label(ee) + ", " + what + " dynamic table rows", std::to_string(re.size()), std::to_string(ra.size())};
        for (std::size_t r = 0; r < re.size(); ++r)
            for (std::size_t k = 0; k < 10; ++k) {
                if (k == 1 || (!layout && k == 3)) continue;
                if (re[r][k] != ra[r][k])
                    return Diff{step_label(ee) + ", " + what + " dynamic array " + std::to_string(r + 1) + " table word " + std::to_string(k + 1),
                                std::to_string(re[r][k]), std::to_string(ra[r][k])};
            }
        return std::nullopt;
    };
    if (auto d = rows_diff(se.dynamic_int_rows, sa.dynamic_int_rows, "integer")) return d;
    if (auto d = rows_diff(se.dynamic_real_rows, sa.dynamic_real_rows, "real")) return d;
    for (std::size_t k = 0; k < std::min(se.dynamic_ints.size(), sa.dynamic_ints.size()); ++k)
        if (auto i = first_mismatch(se.dynamic_ints[k].values, sa.dynamic_ints[k].values))
            return Diff{step_label(ee) + ", integer dynamic array " + std::to_string(se.dynamic_ints[k].slot) + " word " + std::to_string(*i + 1),
                        *i < se.dynamic_ints[k].values.size() ? std::to_string(se.dynamic_ints[k].values[*i]) : "<none>",
                        *i < sa.dynamic_ints[k].values.size() ? std::to_string(sa.dynamic_ints[k].values[*i]) : "<none>"};
    for (std::size_t k = 0; k < std::min(se.dynamic_reals.size(), sa.dynamic_reals.size()); ++k)
        if (auto i = first_mismatch(se.dynamic_reals[k].values, sa.dynamic_reals[k].values))
            return Diff{step_label(ee) + ", real dynamic array " + std::to_string(se.dynamic_reals[k].slot) + " word " + std::to_string(*i + 1),
                        *i < se.dynamic_reals[k].values.size() ? fmt_real(se.dynamic_reals[k].values[*i]) : "<none>",
                        *i < sa.dynamic_reals[k].values.size() ? fmt_real(sa.dynamic_reals[k].values[*i]) : "<none>"};

    if (layout) {
        const std::int32_t re[] = {ee.rec_dynamic, ee.rec_header, ee.rec_mar, ee.rec_a};
        const std::int32_t ra[] = {ea.rec_dynamic, ea.rec_header, ea.rec_mar, ea.rec_a};
        for (int k = 0; k < 4; ++k) {
            if (ho && m.ho_dir && (k == 0 || k == 3)) continue;
            if (re[k] != ra[k])
                return Diff{step_label(ee) + ", directory word " + std::to_string(k + 10) + " " + kEntryWordNames[k + 9],
                            std::to_string(re[k]), std::to_string(ra[k])};
        }
    }
    return std::nullopt;
}

std::string text_field(const std::byte* p, std::size_t n) {
    return "\"" + detail::show(std::string_view(reinterpret_cast<const char*>(p), n)) + "\"";
}

// Explains the first unmasked byte difference at file offset d.
Diff explain_impl(const fs::path& pe, const fs::path& pa, std::uint64_t d, const DbMasks& m, const std::string& raw) {
    deformdb::RandomAccessFile fe(pe), fa(pa);
    if (d < R) {
        std::byte e[20], a[20];
        fe.read_at(0, e, 20);
        fa.read_at(0, a, 20);
        return {"record 1 signature (" + raw + ")", text_field(e, 20), text_field(a, 20)};
    }
    // A moved NXTREC (next free record) is a consequence; look for its cause first.
    std::optional<Diff> nxtrec;
    if (d < 2 * R) {
        std::byte e[R], a[R];
        fe.read_records(2, e, R);
        fa.read_records(2, a, R);
        for (const auto& f : kHeaderFields)
            if (d - R >= f.off && d - R < f.off + f.len) {
                if (f.text) return {std::string("record 2 ") + f.name, text_field(e + f.off, f.len), text_field(a + f.off, f.len)};
                Diff x{std::string("record 2 ") + f.name, std::to_string(deformdb::load_be_i32(e + f.off)),
                       std::to_string(deformdb::load_be_i32(a + f.off))};
                if (f.off != 124) return x;
                nxtrec = x;
            }
        if (!nxtrec) return {"record 2 padding (" + raw + ")", "", ""};
    }

    std::unique_ptr<deformdb::Database> de, da;
    try {
        de = deformdb::Database::open(pe);
        da = deformdb::Database::open(pa);
    } catch (const std::exception& ex) {
        if (nxtrec) return *nxtrec;
        return {raw + " (cannot decode: " + ex.what() + ")", "", ""};
    }
    const auto& ee = de->entries();
    const auto& ea = da->entries();
    const std::int64_t rec = static_cast<std::int64_t>(d / R) + 1;

    auto slot = [&](std::size_t k, bool layout) -> std::optional<Diff> {
        try {
            return compare_slot(*de, *da, ee[k], ea[k], m, layout);
        } catch (const std::exception& ex) {
            return Diff{step_label(ee[k]) + " cannot be decoded on one side: " + ex.what(), "", ""};
        }
    };
    auto scan = [&](std::size_t from) -> std::optional<Diff> {
        for (std::size_t k = from; k < std::min(ee.size(), ea.size()); ++k)
            if (auto x = slot(k, false)) return x;
        for (std::size_t k = 0; k < std::min(ee.size(), ea.size()); ++k)
            if (auto x = slot(k, true)) return x;
        return std::nullopt;
    };
    if (nxtrec) {
        if (auto x = scan(0)) return *x;
        return *nxtrec;
    }

    const auto L = read_layout(pe);
    if (L.index_records.count(rec)) {
        if (auto x = scan(0)) return *x;
        return {"index record " + std::to_string(rec) + " (" + raw + ")", "", ""};
    }
    for (const auto& r : L.entries)
        if (d >= r.offset && d < r.offset + kEntryWords * 4) {
            const std::size_t w = static_cast<std::size_t>((d - r.offset) / 4);
            const std::size_t k = static_cast<std::size_t>(r.slot - 1);
            if (w < 9 || k >= ea.size()) {
                if (k < ea.size())
                    if (auto x = slot(k, true)) return *x;
                return {"directory slot " + std::to_string(r.slot) + " word " + std::to_string(w + 1) + " " + kEntryWordNames[w], "", ""};
            }
            // A record number moved: look for the content difference that caused it.
            if (auto x = scan(0)) return *x;
            return {"directory slot " + std::to_string(r.slot) + " word " + std::to_string(w + 1) + " " + kEntryWordNames[w], "", ""};
        }

    // Data record: try the step whose header starts last at or before it.
    std::size_t owner = ee.size();
    std::int32_t best = 0;
    for (std::size_t k = 0; k < ee.size(); ++k)
        if (ee[k].rec_header > best && ee[k].rec_header <= rec) {
            best = ee[k].rec_header;
            owner = k;
        }
    if (owner < ee.size() && owner < ea.size())
        if (auto x = slot(owner, false)) return *x;
    if (auto x = scan(0)) return *x;
    std::string w = raw + ", no decoded value differs";
    if (owner < ee.size()) w += " (in the records of " + step_label(ee[owner]) + ")";
    return {w, "", ""};
}

Diff explain(const fs::path& pe, const fs::path& pa, std::uint64_t d, const DbMasks& m) {
    char buf[160];
    std::snprintf(buf, sizeof buf, "byte offset %llu (record %llu, byte %llu)", static_cast<unsigned long long>(d),
                  static_cast<unsigned long long>(d / R + 1), static_cast<unsigned long long>(d % R));
    try {
        return explain_impl(pe, pa, d, m, buf);
    } catch (const std::exception& ex) {
        return {std::string(buf) + " (" + ex.what() + ")", "", ""};
    }
}

}  // namespace

// ---------------------------------------------------------------- masks

DbMasks DbMasks::none() {
    DbMasks m;
    m.smi3 = m.smi4 = m.istart = false;
    return m;
}

std::optional<DbMasks> DbMasks::parse(std::string_view spec, std::string* error) {
    DbMasks m;
    bool first = true;
    auto set_name = [&](std::string_view n, bool on) -> bool {
        if (n == "smi2") m.smi2 = on;
        else if (n == "smi3") m.smi3 = on;
        else if (n == "smi4") m.smi4 = on;
        else if (n == "istart") m.istart = on;
        else if (n == "ho-dir") m.ho_dir = on;
        else if (n == "ho-h7h8") m.ho_h7h8 = on;
        else if (n == "dates") m.smi3 = m.smi4 = on;
        else if (n == "header-only") m.ho_dir = m.ho_h7h8 = on;
        else if (n == "default") { m.smi3 = m.smi4 = m.istart = on; }
        else if (n == "all") m.smi2 = m.smi3 = m.smi4 = m.istart = m.ho_dir = m.ho_h7h8 = on;
        else return false;
        return true;
    };
    while (!spec.empty()) {
        const auto c = spec.find(',');
        std::string_view item = spec.substr(0, c);
        spec = c == std::string_view::npos ? std::string_view{} : spec.substr(c + 1);
        if (item.empty()) continue;
        if (item.substr(0, 6) == "bytes:" || item.substr(0, 7) == "+bytes:") {
            const std::string s(item.substr(item[0] == '+' ? 7 : 6));
            unsigned long long off = 0, len = 0;
            char tail = 0;
            if (std::sscanf(s.c_str(), "%llu:%llu%c", &off, &len, &tail) != 2) {
                if (error) *error = "bad mask item '" + std::string(item) + "' (expected bytes:OFFSET:LEN)";
                return std::nullopt;
            }
            m.raw.push_back({off, len});
            first = false;
            continue;
        }
        bool on = true;
        if (item[0] == '+' || item[0] == '-') {
            on = item[0] == '+';
            item.remove_prefix(1);
        } else if (first) {
            m = none();
            if (item == "none") {
                first = false;
                continue;
            }
        }
        first = false;
        if (item == "none" && !on) continue;
        if (!set_name(item, on)) {
            if (error) *error = "unknown mask '" + std::string(item) + "'";
            return std::nullopt;
        }
    }
    return m;
}

std::string DbMasks::canonical() const {
    std::string s;
    auto add = [&](bool on, const char* n) {
        if (!on) return;
        if (!s.empty()) s += ",";
        s += n;
    };
    add(smi2, "smi2");
    add(smi3, "smi3");
    add(smi4, "smi4");
    add(istart, "istart");
    add(ho_dir, "ho-dir");
    add(ho_h7h8, "ho-h7h8");
    for (const auto& r : raw) {
        if (!s.empty()) s += ",";
        s += "bytes:" + std::to_string(r.offset) + ":" + std::to_string(r.length);
    }
    return s.empty() ? "none" : s;
}

std::vector<ByteRange> db_mask_ranges(const fs::path& file, const DbMasks& m) {
    return mask_ranges(read_layout(file), m);
}

Digest db_digest(const fs::path& file, const DbMasks& m) { return masked_digest(file, db_mask_ranges(file, m)); }

Result compare_db(const fs::path& expected, const fs::path& actual, const DbMasks& m, std::optional<Digest> expected_digest) {
    Result r;
    try {
        const auto ma = db_mask_ranges(actual, m);
        r.actual_digest = masked_digest(actual, ma);
        if (expected_digest) {
            r.expected_digest = *expected_digest;
            if (r.actual_digest == *expected_digest) {
                r.equal = r.by_digest = true;
                return r;
            }
            if (!fs::exists(expected)) {
                r.where = "digest (expected file not available)";
                return r;
            }
        }
        const auto me = db_mask_ranges(expected, m);
        if (!expected_digest) r.expected_digest = masked_digest(expected, me);
        const auto se = fs::file_size(expected), sa = fs::file_size(actual);
        if (r.expected_digest == r.actual_digest && se == sa) {
            r.equal = r.by_digest = true;
            return r;
        }
        auto d = first_difference(expected, actual, me, ma);
        if (!d) {
            if (se == sa) {   // digest collision or masks differ only
                r.equal = true;
                return r;
            }
            d = std::min(se, sa);
        }
        const Diff x = explain(expected, actual, *d, m);
        r.where = x.where;
        r.expected = x.expected;
        r.actual = x.actual;
        if (se != sa) r.where += " [sizes: expected " + std::to_string(se) + ", actual " + std::to_string(sa) + " bytes]";
    } catch (const std::exception& ex) {
        r.error = ex.what();
    }
    return r;
}

}  // namespace defcmp
