// KEY files: byte comparison; a mismatch is reported as the first differing line
// and the keyword block it belongs to.
#include <algorithm>
#include <string>
#include <unordered_set>

#include "internal.hpp"

namespace defcmp {

namespace {

// Keyword names of DEF_PRE's registry, generated from docs/re/keyword_registry.tsv.
const char* const kRegistry[] = {
#include "keyword_names.inc"
};

const std::unordered_set<std::string_view>& registry() {
    static const std::unordered_set<std::string_view> s(std::begin(kRegistry), std::end(kRegistry));
    return s;
}

std::string_view first_token(std::string_view line) {
    std::size_t e = 0;
    while (e < line.size() && line[e] != ' ' && line[e] != '\t' && line[e] != '\r') ++e;
    return line.substr(0, e);
}

// A keyword line starts at column 0 with a registry name. Data lines start with
// blanks, comments with '*'; text values (OPRNAM, OBJNAM, ...) are not registry names.
bool is_keyword_line(std::string_view line) {
    if (line.empty() || line[0] == ' ' || line[0] == '*' || line[0] == '\t') return false;
    return is_registry_keyword(first_token(line));
}

std::string_view line_at(std::string_view text, std::size_t begin) {
    std::size_t e = text.find('\n', begin);
    if (e == std::string_view::npos) e = text.size();
    auto l = text.substr(begin, e - begin);
    if (!l.empty() && l.back() == '\r') l.remove_suffix(1);
    return l;
}

}  // namespace

bool is_registry_keyword(std::string_view token) { return registry().count(token) != 0; }

Digest key_digest(const fs::path& file) { return bytes_digest(file); }

Result compare_key_text(std::string_view expected, std::string_view actual) {
    Result r;
    r.expected_digest = hash_bytes(expected);
    r.actual_digest = hash_bytes(actual);
    if (r.expected_digest == r.actual_digest && expected == actual) {
        r.equal = r.by_digest = true;
        return r;
    }
    const std::size_t n = std::min(expected.size(), actual.size());
    std::size_t off = 0;
    while (off < n && expected[off] == actual[off]) ++off;
    if (off == n && expected.size() == actual.size()) {   // digest collision, contents equal
        r.equal = true;
        return r;
    }

    // Line containing the first differing byte (same start on both sides).
    const std::size_t line_begin = off == 0 ? 0 : expected.substr(0, off).rfind('\n') + 1;
    const std::size_t line_no = static_cast<std::size_t>(std::count(expected.begin(), expected.begin() + line_begin, '\n')) + 1;
    const auto le = line_at(expected, line_begin);
    const auto la = line_at(actual, line_begin);

    std::string where = "line " + std::to_string(line_no) + " col " + std::to_string(off - line_begin + 1);

    // Keyword block: nearest keyword line at or above the differing line (expected side).
    std::size_t b = line_begin, kw_line = line_no;
    for (;;) {
        const auto l = line_at(expected, b);
        if (is_keyword_line(l)) {
            where += ", keyword " + std::string(first_token(l));
            if (kw_line != line_no) where += " (line " + std::to_string(kw_line) + ": \"" + detail::show(l, 60) + "\")";
            break;
        }
        if (b == 0) {
            where += ", before the first keyword";
            break;
        }
        b = expected.substr(0, b - 1).rfind('\n');
        b = b == std::string_view::npos ? 0 : b + 1;
        --kw_line;
    }

    if (off >= expected.size()) r.expected = "<end of file>";
    else r.expected = "\"" + detail::show(le) + "\"";
    if (off >= actual.size()) r.actual = "<end of file>";
    else r.actual = "\"" + detail::show(la) + "\"";
    if (le == la) where += " (line ending or trailing bytes differ)";
    r.where = where;
    return r;
}

Result compare_key(const fs::path& expected, const fs::path& actual, std::optional<Digest> expected_digest) {
    Result r;
    try {
        r.actual_digest = key_digest(actual);
        if (expected_digest && *expected_digest == r.actual_digest) {
            r.expected_digest = *expected_digest;
            r.equal = r.by_digest = true;
            return r;
        }
        if (expected_digest && !fs::exists(expected)) {
            r.expected_digest = *expected_digest;
            r.where = "digest (expected file not available)";
            return r;
        }
        const auto e = detail::read_file(expected);
        const auto a = detail::read_file(actual);
        r = compare_key_text(e, a);
    } catch (const std::exception& ex) {
        r.error = ex.what();
    }
    return r;
}

}  // namespace defcmp
