#include <algorithm>
#include <cctype>
#include <cstdio>
#include <fstream>
#include <map>
#include <stdexcept>

#include "internal.hpp"
#include "json.hpp"

namespace defcmp {

namespace detail {

std::string read_file(const fs::path& p) {
    std::ifstream f(p, std::ios::binary);
    if (!f) throw std::runtime_error("cannot open " + p.string());
    std::string s;
    f.seekg(0, std::ios::end);
    s.resize(static_cast<std::size_t>(f.tellg()));
    f.seekg(0);
    f.read(s.data(), static_cast<std::streamsize>(s.size()));
    if (!f) throw std::runtime_error("cannot read " + p.string());
    return s;
}

std::vector<std::string_view> split_lines(std::string_view text) {
    std::vector<std::string_view> lines;
    std::size_t b = 0;
    while (b < text.size()) {
        std::size_t e = text.find('\n', b);
        if (e == std::string_view::npos) {
            lines.push_back(text.substr(b));
            break;
        }
        std::size_t end = e;
        if (end > b && text[end - 1] == '\r') --end;
        lines.push_back(text.substr(b, end - b));
        b = e + 1;
    }
    return lines;
}

std::string show(std::string_view s, std::size_t max) {
    std::string out;
    for (std::size_t i = 0; i < s.size() && out.size() < max; ++i) {
        const unsigned char c = static_cast<unsigned char>(s[i]);
        if (c == '\r') out += "\\r";
        else if (c == '\t') out += "\\t";
        else if (c < 32 || c == 127) {
            char b[8];
            std::snprintf(b, sizeof b, "\\x%02X", c);
            out += b;
        } else out += static_cast<char>(c);
    }
    if (out.size() >= max) out += "...";
    return out;
}

std::size_t first_diff_column(std::string_view a, std::string_view b) {
    const std::size_t n = std::min(a.size(), b.size());
    for (std::size_t i = 0; i < n; ++i)
        if (a[i] != b[i]) return i + 1;
    return a.size() == b.size() ? 0 : n + 1;
}

}  // namespace detail

const char* kind_name(Kind k) {
    switch (k) {
    case Kind::Key: return "key";
    case Kind::Db: return "db";
    case Kind::Transcript: return "transcript";
    case Kind::Bytes: return "bytes";
    }
    return "?";
}

std::optional<Kind> parse_kind(std::string_view s) {
    if (s == "key") return Kind::Key;
    if (s == "db") return Kind::Db;
    if (s == "transcript" || s == "stdout") return Kind::Transcript;
    if (s == "bytes") return Kind::Bytes;
    return std::nullopt;
}

Kind kind_from_path(const fs::path& p) {
    auto upper = [](std::string x) {
        for (auto& c : x) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
        return x;
    };
    const std::string name = upper(p.filename().string());
    if (name == "STDOUT.TXT" || name == "STDERR.TXT") return Kind::Transcript;
    const std::string ext = upper(p.extension().string());
    if (ext == ".KEY") return Kind::Key;
    if (ext == ".DB") return Kind::Db;
    return Kind::Bytes;
}

std::string Result::summary() const {
    if (!error.empty()) return "ERROR: " + error;
    if (equal) return by_digest ? "equal (digest " + digest_hex(actual_digest) + ")" : "equal";
    std::string s = "DIFFERENT";
    if (!where.empty()) s += " at " + where;
    if (!expected.empty() || !actual.empty()) {
        s += "\n  expected: " + expected;
        s += "\n  actual:   " + actual;
    }
    return s;
}

// ------------------------------------------------------------------ bytes

Digest bytes_digest(const fs::path& file) {
    std::ifstream f(file, std::ios::binary);
    if (!f) throw std::runtime_error("cannot open " + file.string());
    std::vector<char> buf(1 << 22);
    Hasher h;
    while (f) {
        f.read(buf.data(), static_cast<std::streamsize>(buf.size()));
        h.update(buf.data(), static_cast<std::size_t>(f.gcount()));
    }
    return h.finish();
}

Result compare_bytes(const fs::path& expected, const fs::path& actual, std::optional<Digest> expected_digest) {
    Result r;
    try {
        r.actual_digest = bytes_digest(actual);
        r.expected_digest = expected_digest ? *expected_digest : bytes_digest(expected);
        if (r.actual_digest == r.expected_digest) {
            r.equal = r.by_digest = true;
            return r;
        }
        if (!fs::exists(expected)) {
            r.where = "digest (expected file not available)";
            return r;
        }
        std::ifstream a(expected, std::ios::binary), b(actual, std::ios::binary);
        std::vector<char> ba(1 << 20), bb(1 << 20);
        std::uint64_t off = 0;
        for (;;) {
            a.read(ba.data(), static_cast<std::streamsize>(ba.size()));
            b.read(bb.data(), static_cast<std::streamsize>(bb.size()));
            const auto na = static_cast<std::size_t>(a.gcount()), nb = static_cast<std::size_t>(b.gcount());
            const std::size_t n = std::min(na, nb);
            for (std::size_t i = 0; i < n; ++i)
                if (ba[i] != bb[i]) {
                    char t[96];
                    std::snprintf(t, sizeof t, "byte offset %llu", static_cast<unsigned long long>(off + i));
                    r.where = t;
                    std::snprintf(t, sizeof t, "0x%02X", static_cast<unsigned char>(ba[i]));
                    r.expected = t;
                    std::snprintf(t, sizeof t, "0x%02X", static_cast<unsigned char>(bb[i]));
                    r.actual = t;
                    return r;
                }
            if (na != nb) {
                r.where = "size: expected " + std::to_string(fs::file_size(expected)) + " bytes, actual " +
                          std::to_string(fs::file_size(actual));
                return r;
            }
            if (na == 0) break;
            off += na;
        }
        r.where = "digest only (contents equal on re-read)";
    } catch (const std::exception& e) {
        r.error = e.what();
    }
    return r;
}

// ------------------------------------------------------------------ dispatch

Digest digest_file(Kind k, const fs::path& file, const Options& o) {
    switch (k) {
    case Kind::Key: return key_digest(file);
    case Kind::Db: return db_digest(file, o.db);
    case Kind::Transcript: return transcript_digest(file, o.transcript);
    case Kind::Bytes: break;
    }
    return bytes_digest(file);
}

Result compare_files(Kind k, const fs::path& expected, const fs::path& actual, const Options& o,
                     std::optional<Digest> expected_digest) {
    switch (k) {
    case Kind::Key: return compare_key(expected, actual, expected_digest);
    case Kind::Db: return compare_db(expected, actual, o.db, expected_digest);
    case Kind::Transcript: return compare_transcript(expected, actual, o.transcript, expected_digest);
    case Kind::Bytes: break;
    }
    return compare_bytes(expected, actual, expected_digest);
}

namespace {

std::string upper(std::string s) {
    for (auto& c : s) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    return s;
}

// Regular files under dir by upper-cased relative path; top-level names in
// `skip_top` (upper case) are left out.
std::map<std::string, fs::path> list_files(const fs::path& dir, const std::vector<std::string>& skip_top = {}) {
    std::map<std::string, fs::path> m;
    if (!fs::is_directory(dir)) return m;
    for (const auto& e : fs::recursive_directory_iterator(dir)) {
        if (!e.is_regular_file()) continue;
        const auto rel = fs::relative(e.path(), dir);
        const auto key = upper(rel.generic_string());
        if (std::find(skip_top.begin(), skip_top.end(), key) != skip_top.end()) continue;
        m[key] = e.path();
    }
    return m;
}

void compare_maps(const std::map<std::string, fs::path>& me, const fs::path& root_e,
                  const std::map<std::string, fs::path>& ma, const fs::path& root_a, const Options& o,
                  std::vector<DirEntryResult>& out) {
    for (const auto& [name, pe] : me) {
        DirEntryResult d;
        d.name = fs::relative(pe, root_e).generic_string();
        d.kind = kind_from_path(pe);
        auto it = ma.find(name);
        if (it == ma.end()) {
            d.missing_actual = true;
            d.result.where = "missing in actual";
        } else {
            d.result = compare_files(d.kind, pe, it->second, o);
        }
        out.push_back(std::move(d));
    }
    for (const auto& [name, pa] : ma) {
        if (me.count(name)) continue;
        DirEntryResult d;
        d.name = fs::relative(pa, root_a).generic_string();
        d.kind = kind_from_path(pa);
        d.missing_expected = true;
        d.result.where = "not written by the original";
        out.push_back(std::move(d));
    }
}

std::optional<int> read_exit_code(const fs::path& dir) {
    if (fs::exists(dir / "result.json")) {
        const auto v = json::parse(detail::read_file(dir / "result.json"));
        if (const auto* c = v.get("exit_code"); c && c->type == json::Value::Number) return static_cast<int>(c->num);
        return std::nullopt;
    }
    if (fs::exists(dir / "exit_code")) {
        const auto t = detail::read_file(dir / "exit_code");
        try {
            return std::stoi(t);
        } catch (...) {
            return std::nullopt;
        }
    }
    return std::nullopt;
}

}  // namespace

std::vector<DirEntryResult> compare_dirs(const fs::path& expected, const fs::path& actual, const Options& o) {
    std::vector<DirEntryResult> out;
    compare_maps(list_files(expected), expected, list_files(actual), actual, o, out);
    return out;
}

std::vector<DirEntryResult> compare_oracle_entry(const fs::path& entry, const fs::path& actual, const Options& o) {
    std::vector<DirEntryResult> out;
    const auto result = json::parse(detail::read_file(entry / "result.json"));

    {   // exit code: a non-zero exit is a cached result like any other
        DirEntryResult d;
        d.name = "exit code";
        const auto* c = result.get("exit_code");
        const std::optional<int> ce = c && c->type == json::Value::Number ? std::optional<int>(static_cast<int>(c->num)) : std::nullopt;
        std::optional<int> ca;
        try {
            ca = read_exit_code(actual);
        } catch (const std::exception& e) {
            d.result.error = e.what();
        }
        if (d.result.error.empty()) {
            if (!ca) {
                d.skipped = true;
                d.result.where = "actual run recorded no exit code";
            } else {
                d.result.equal = ce == ca;
                if (!d.result.equal) {
                    d.result.where = "process exit code";
                    d.result.expected = ce ? std::to_string(*ce) : "<none>";
                    d.result.actual = std::to_string(*ca);
                }
            }
        }
        out.push_back(std::move(d));
    }

    for (const char* t : {"stdout.txt", "stderr.txt"}) {
        DirEntryResult d;
        d.name = t;
        d.kind = Kind::Transcript;
        if (!fs::exists(actual / t)) {
            d.missing_actual = true;
            d.result.where = "missing in actual";
        } else {
            d.result = compare_transcript(entry / t, actual / t, o.transcript);
        }
        out.push_back(std::move(d));
    }

    const fs::path ea = fs::is_directory(actual / "files") ? actual / "files" : actual;
    const std::vector<std::string> skip =
        ea == actual ? std::vector<std::string>{"STDOUT.TXT", "STDERR.TXT", "RESULT.JSON", "EXIT_CODE"} : std::vector<std::string>{};
    auto ma = list_files(ea, skip);
    auto me = list_files(entry / "files");

    if (const auto* files = result.get("files"); files && files->type == json::Value::Object) {
        for (const auto& [name, info] : files->obj) {
            const auto key = upper(fs::path(name).generic_string());
            const auto* status = info.get("status");
            const auto* stored = info.get("stored");
            if (status && status->str == "deleted") {
                DirEntryResult d;
                d.name = name;
                d.kind = kind_from_path(name);
                d.result.equal = ma.erase(key) == 0;
                if (!d.result.equal) d.result.where = "the original deleted this file, the port left it";
                me.erase(key);
                out.push_back(std::move(d));
            } else if (stored && stored->type == json::Value::Bool && !stored->b) {
                DirEntryResult d;
                d.name = name;
                d.kind = kind_from_path(name);
                d.skipped = true;
                const auto* size = info.get("size");
                d.result.where = "the oracle kept only size and sha256 (" +
                                 std::to_string(size ? static_cast<long long>(size->num) : -1) + " bytes)" +
                                 (ma.count(key) ? "" : "; missing in actual");
                me.erase(key);
                ma.erase(key);
                out.push_back(std::move(d));
            }
        }
    }
    compare_maps(me, entry / "files", ma, ea, o, out);
    return out;
}

}  // namespace defcmp
