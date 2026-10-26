// stdout transcripts: compared line by line after masking dates, times and paths.
#include <cctype>

#include "internal.hpp"

namespace defcmp {

namespace {

constexpr auto kIcase = std::regex::ECMAScript | std::regex::icase;
#define MON "(?:Jan|Feb|Mar|Apr|May|Jun|Jul|Aug|Sep|Oct|Nov|Dec)[a-z]*\\.?"
#define DAY "(?:Mon|Tue|Wed|Thu|Fri|Sat|Sun)[a-z]*\\.?"

const std::regex& re_path() {
    // A blank belongs to the path when the next word still contains a separator
    // ("C:\Program Files\SFTC\x.exe"), so words after the path stay unmasked.
    static const std::regex r(R"((?:[A-Za-z]:[\\/]|\\\\)(?:[^\s"'<>|*?]| (?=[^\s"'<>|*?]*[\\/]))*)");
    return r;
}

// Order matters: whole ctime/strftime forms first, then the pieces.
const std::vector<std::regex>& re_dates() {
    static const std::vector<std::regex> r = {
        std::regex("\\b" DAY ",?\\s+" MON "\\s+\\d{1,2}\\s+\\d{1,2}:\\d{2}:\\d{2}\\s+\\d{4}\\b", kIcase),
        std::regex("\\b\\d{4}[-/.]\\d{1,2}[-/.]\\d{1,2}\\b"),
        std::regex("\\b\\d{1,2}[/-]\\d{1,2}[/-]\\d{2,4}\\b"),
        std::regex("\\b\\d{1,2}\\.\\d{1,2}\\.\\d{4}\\b"),
        std::regex("\\b\\d{1,2}[- ]" MON "[- ,]+\\d{2,4}\\b", kIcase),
        std::regex("\\b" MON "\\s+\\d{1,2}(?:,?\\s+\\d{4})?\\b", kIcase),
        std::regex("\\b" DAY "(?=[\\s,])", kIcase),
    };
    return r;
}

const std::regex& re_time() {
    static const std::regex r(R"(\b\d{1,2}:\d{2}(?::\d{2}(?:\.\d+)?)?(?:\s?[AaPp][Mm]\b)?)");
    return r;
}

#undef MON
#undef DAY

bool same_path_char(char a, char b) {
    if ((a == '\\' || a == '/') && (b == '\\' || b == '/')) return true;
    return std::tolower(static_cast<unsigned char>(a)) == std::tolower(static_cast<unsigned char>(b));
}

std::string replace_root(const std::string& s, std::string root, const std::string& name) {
    while (!root.empty() && (root.back() == '\\' || root.back() == '/')) root.pop_back();
    if (root.empty()) return s;
    std::string out;
    std::size_t i = 0;
    while (i < s.size()) {
        bool hit = i + root.size() <= s.size();
        for (std::size_t k = 0; hit && k < root.size(); ++k) hit = same_path_char(s[i + k], root[k]);
        // Must end at a path boundary, so C:\run does not match C:\run2.
        if (hit && i + root.size() < s.size()) {
            const char c = s[i + root.size()];
            hit = c == '\\' || c == '/' || !(std::isalnum(static_cast<unsigned char>(c)) || c == '_' || c == '-' || c == '.');
        }
        if (hit) {
            out += name;
            i += root.size();
        } else {
            out += s[i++];
        }
    }
    return out;
}

}  // namespace

std::string normalize_transcript(std::string_view text, const TranscriptMasks& m) {
    std::string s(text);
    for (const auto& [root, name] : m.roots) s = replace_root(s, root, name);
    if (m.paths) s = std::regex_replace(s, re_path(), "<PATH>");
    if (m.dates)
        for (const auto& r : re_dates()) s = std::regex_replace(s, r, "<DATE>");
    if (m.times) s = std::regex_replace(s, re_time(), "<TIME>");
    for (const auto& r : m.extra) s = std::regex_replace(s, r, "<MASK>");
    return s;
}

Digest transcript_digest(const fs::path& file, const TranscriptMasks& m) {
    return hash_bytes(normalize_transcript(detail::read_file(file), m));
}

Result compare_transcript_text(std::string_view expected, std::string_view actual, const TranscriptMasks& m) {
    Result r;
    const auto ne = normalize_transcript(expected, m);
    const auto na = normalize_transcript(actual, m);
    r.expected_digest = hash_bytes(ne);
    r.actual_digest = hash_bytes(na);
    if (r.expected_digest == r.actual_digest && ne == na) {
        r.equal = r.by_digest = true;
        return r;
    }
    const auto le = detail::split_lines(ne), la = detail::split_lines(na);
    const std::size_t n = std::min(le.size(), la.size());
    for (std::size_t i = 0; i < n; ++i) {
        if (le[i] == la[i]) continue;
        r.where = "line " + std::to_string(i + 1) + " col " + std::to_string(detail::first_diff_column(le[i], la[i])) +
                  " (after masking)";
        r.expected = "\"" + detail::show(le[i]) + "\"";
        r.actual = "\"" + detail::show(la[i]) + "\"";
        return r;
    }
    if (le.size() != la.size()) {
        r.where = "line " + std::to_string(n + 1) + " (line count: expected " + std::to_string(le.size()) +
                  ", actual " + std::to_string(la.size()) + ")";
        r.expected = n < le.size() ? "\"" + detail::show(le[n]) + "\"" : "<end of transcript>";
        r.actual = n < la.size() ? "\"" + detail::show(la[n]) + "\"" : "<end of transcript>";
        return r;
    }
    r.where = "line endings differ";
    return r;
}

Result compare_transcript(const fs::path& expected, const fs::path& actual, const TranscriptMasks& m,
                          std::optional<Digest> expected_digest) {
    Result r;
    try {
        const auto a = detail::read_file(actual);
        if (expected_digest) {
            r.expected_digest = *expected_digest;
            r.actual_digest = hash_bytes(normalize_transcript(a, m));
            if (r.actual_digest == *expected_digest) {
                r.equal = r.by_digest = true;
                return r;
            }
            if (!fs::exists(expected)) {
                r.where = "digest (expected file not available)";
                return r;
            }
        }
        r = compare_transcript_text(detail::read_file(expected), a, m);
    } catch (const std::exception& ex) {
        r.error = ex.what();
    }
    return r;
}

}  // namespace defcmp
