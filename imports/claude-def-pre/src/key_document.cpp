#include "deform/key_document.hpp"

#include "deform/layout.hpp"

#include <fstream>
#include <stdexcept>

namespace dk {
namespace {

std::string_view trim_cr(std::string_view s) noexcept
{
    while (!s.empty() && (s.back() == '\r' || s.back() == '\n')) {
        s.remove_suffix(1);
    }
    return s;
}

std::vector<std::string_view> tokens_of(std::string_view s)
{
    std::vector<std::string_view> out;
    TokenCursor c(s);
    std::string_view t;
    while (c.next(t)) {
        out.push_back(t);
    }
    return out;
}

bool is_keyword_char(char c) noexcept
{
    return (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_';
}

// Splits "KEYWD rest" when the line starts a record.
bool split_keyword_line(std::string_view line, bool keep_unregistered, std::string_view &name,
                        std::string_view &rest, const KeywordInfo *&info)
{
    if (line.empty() || line[0] < 'A' || line[0] > 'Z') {
        return false;
    }
    std::size_t n = 0;
    while (n < line.size() && is_keyword_char(line[n])) {
        ++n;
    }
    if (n > 12 || (n < line.size() && line[n] != ' ' && line[n] != '\t')) {
        return false;
    }
    name = line.substr(0, n);
    rest = n < line.size() ? line.substr(n) : std::string_view{};
    info = find_keyword(name);
    if (info != nullptr) {
        return true;
    }
    if (!keep_unregistered) {
        return false;
    }
    // Unregistered: accept only "NAME number number ..." so plain text lines
    // (object names, titles) are never taken for keywords.
    TokenCursor c(rest);
    std::string_view t;
    std::size_t count = 0;
    while (c.next(t)) {
        if (!is_number_token(t)) {
            return false;
        }
        ++count;
    }
    return count > 0;
}

bool blank_prefix(std::string_view line, std::size_t width) noexcept
{
    if (line.size() <= width) {
        return false;
    }
    for (std::size_t i = 0; i < width; ++i) {
        if (line[i] != ' ') {
            return false;
        }
    }
    return true;
}

} // namespace

// ---------------------------------------------------------------------------
// Record

std::vector<std::string_view> Record::header_tokens() const
{
    return tokens_of(header);
}

std::optional<std::int64_t> Record::header_int(std::size_t index) const
{
    TokenCursor c(header);
    std::string_view t;
    for (std::size_t i = 0; c.next(t); ++i) {
        if (i == index) {
            return parse_int(t);
        }
    }
    return std::nullopt;
}

std::optional<double> Record::header_real(std::size_t index) const
{
    TokenCursor c(header);
    std::string_view t;
    for (std::size_t i = 0; c.next(t); ++i) {
        if (i == index) {
            return parse_real(t);
        }
    }
    return std::nullopt;
}

std::vector<std::string_view> Record::body_lines() const
{
    std::vector<std::string_view> out;
    if (body_line_count == 0) {
        return out;
    }
    out.reserve(body_line_count);
    TokenCursor c(body);
    while (!c.done()) {
        out.push_back(c.take_line());
    }
    // A trailing empty line inside the declared count (e.g. blank names).
    while (out.size() < body_line_count) {
        out.emplace_back();
    }
    return out;
}

std::vector<std::vector<std::string_view>> Record::rows() const
{
    std::vector<std::vector<std::string_view>> out;
    bool prev_numeric = false;
    for (std::string_view line : body_lines()) {
        auto toks = tokens_of(line);
        bool numeric = !toks.empty();
        for (auto t : toks) {
            if (!is_number_token(t)) {
                numeric = false;
                break;
            }
        }
        if (numeric && prev_numeric && !out.empty() && blank_prefix(line, 8)) {
            out.back().insert(out.back().end(), toks.begin(), toks.end());
            continue;
        }
        out.push_back(std::move(toks));
        prev_numeric = numeric;
    }
    return out;
}

std::vector<std::string_view> Record::body_tokens() const
{
    return tokens_of(body);
}

std::optional<int> Record::owner() const
{
    if (info == nullptr) {
        return std::nullopt;
    }
    switch (info->group) {
    case KeywordGroup::Object:
    case KeywordGroup::Material:
    case KeywordGroup::InterObject:
    case KeywordGroup::PhaseTransformation: {
        auto v = header_int(0);
        if (v) {
            return static_cast<int>(*v);
        }
        return std::nullopt;
    }
    default:
        return std::nullopt;
    }
}

// ---------------------------------------------------------------------------
// KeyDocument

KeyDocument KeyDocument::parse(std::string text, const ParseOptions &options)
{
    KeyDocument doc;
    doc.text_ = std::make_shared<const std::string>(std::move(text));
    const std::string &s = *doc.text_;
    const char *const base = s.data();
    const char *const end = base + s.size();

    doc.records_.reserve(1024);

    Record *cur = nullptr;
    bool body_open = false;         // current record still accepts body lines
    std::size_t text_left = 0;      // exact text lines still owed to the record
    const char *body_begin = nullptr;
    const char *body_end = nullptr;

    auto close_body = [&]() {
        if (cur != nullptr && body_begin != nullptr) {
            cur->body = trim_cr(std::string_view(body_begin, static_cast<std::size_t>(body_end - body_begin)));
        }
        body_open = false;
    };

    std::uint32_t lineno = 0;
    const char *p = base;
    while (p < end) {
        ++lineno;
        const char *ls = p;
        while (p < end && *p != '\n') {
            ++p;
        }
        const char *le = p;          // end of line content (before '\n')
        if (p < end) {
            ++p;                     // past '\n'
        }
        std::string_view line(ls, static_cast<std::size_t>(le - ls));
        if (!line.empty() && line.back() == '\r') {
            line.remove_suffix(1);
        }

        if (cur != nullptr && text_left > 0) {
            --text_left;
            ++cur->body_line_count;
            body_end = p;
            if (text_left == 0) {
                close_body();
            }
            continue;
        }

        if (!line.empty() && line[0] == '*') {
            close_body();
            continue;
        }

        std::string_view name, rest;
        const KeywordInfo *info = nullptr;
        if (split_keyword_line(line, options.keep_unregistered, name, rest, info)) {
            close_body();
            Record r;
            r.name = name;
            r.info = info;
            r.header = rest;
            r.line = lineno;
            doc.records_.push_back(r);
            cur = &doc.records_.back();
            body_begin = p;
            body_end = p;
            body_open = true;
            if (info == nullptr) {
                doc.diagnostics_.push_back({Diagnostic::Severity::Warning, lineno,
                                            "unregistered keyword " + std::string(name)});
            }
            const auto htoks = tokens_of(rest);
            if (auto n = text_line_count(name, htoks)) {
                text_left = *n;
                if (text_left == 0) {
                    close_body();
                }
            } else {
                text_left = 0;
            }
            continue;
        }

        if (cur != nullptr && body_open) {
            ++cur->body_line_count;
            body_end = p;
            continue;
        }
        if (line.find_first_not_of(" \t") == std::string_view::npos) {
            continue;
        }
        doc.diagnostics_.push_back({Diagnostic::Severity::Warning, lineno,
                                    "line outside any record: " + std::string(line.substr(0, 40))});
    }
    close_body();
    if (cur != nullptr && text_left > 0) {
        doc.diagnostics_.push_back({Diagnostic::Severity::Warning, cur->line,
                                    "file ended inside text body of " + std::string(cur->name)});
    }
    return doc;
}

KeyDocument KeyDocument::load(const std::filesystem::path &path, const ParseOptions &options)
{
    std::ifstream in(path, std::ios::binary);
    if (!in) {
        throw std::runtime_error("cannot open " + path.string());
    }
    in.seekg(0, std::ios::end);
    const auto size = static_cast<std::size_t>(in.tellg());
    in.seekg(0, std::ios::beg);
    std::string text(size, '\0');
    in.read(text.data(), static_cast<std::streamsize>(size));
    if (static_cast<std::size_t>(in.gcount()) != size) {
        throw std::runtime_error("short read on " + path.string());
    }
    return parse(std::move(text), options);
}

std::vector<const Record *> KeyDocument::find_all(std::string_view keyword) const
{
    std::vector<const Record *> out;
    for (const auto &r : records_) {
        if (r.name == keyword) {
            out.push_back(&r);
        }
    }
    return out;
}

const Record *KeyDocument::find(std::string_view keyword, std::optional<int> owner) const
{
    for (const auto &r : records_) {
        if (r.name == keyword && (!owner || r.owner() == owner)) {
            return &r;
        }
    }
    return nullptr;
}

} // namespace dk
