#include "deform/layout.hpp"

#include "deform/key_document.hpp"
#include "deform/numbers.hpp"

#include <algorithm>
#include <cctype>
#include <map>
#include <mutex>

namespace dk {
namespace {

constexpr const char *kLayoutLines[] = {
#include "layout_table.inc"
};

std::string_view trim(std::string_view s) noexcept
{
    while (!s.empty() && std::isspace(static_cast<unsigned char>(s.front()))) {
        s.remove_prefix(1);
    }
    while (!s.empty() && std::isspace(static_cast<unsigned char>(s.back()))) {
        s.remove_suffix(1);
    }
    return s;
}

bool parse_count(std::string_view s, int &literal, std::string &ref)
{
    s = trim(s);
    if (s.empty()) {
        return false;
    }
    if (auto v = parse_int(s)) {
        literal = static_cast<int>(*v);
        ref.clear();
        return true;
    }
    for (char c : s) {
        if (!(std::isalnum(static_cast<unsigned char>(c)) || c == '_')) {
            return false;
        }
    }
    ref = std::string(s);
    return true;
}

// "name:T" or "name:T*K"
bool parse_field(std::string_view s, FieldSpec &f, std::string *error)
{
    const auto colon = s.find(':');
    if (colon == std::string_view::npos || colon == 0 || colon + 1 >= s.size()) {
        if (error) *error = "bad field '" + std::string(s) + "'";
        return false;
    }
    f.name = std::string(s.substr(0, colon));
    const char t = s[colon + 1];
    if (t != 'I' && t != 'R' && t != 'S') {
        if (error) *error = "bad type in '" + std::string(s) + "'";
        return false;
    }
    f.type = t;
    f.repeat = 1;
    f.repeat_ref.clear();
    std::string_view rest = s.substr(colon + 2);
    if (!rest.empty()) {
        if (rest[0] != '*' || !parse_count(rest.substr(1), f.repeat, f.repeat_ref)) {
            if (error) *error = "bad repeat in '" + std::string(s) + "'";
            return false;
        }
    }
    return true;
}

std::vector<std::string_view> split_ws(std::string_view s)
{
    std::vector<std::string_view> out;
    TokenCursor c(s);
    std::string_view t;
    while (c.next(t)) {
        out.push_back(t);
    }
    return out;
}

bool parse_columns(std::string_view s, std::vector<FieldSpec> &cols, std::string *error)
{
    for (auto tok : split_ws(s)) {
        FieldSpec f;
        if (!parse_field(tok, f, error)) {
            return false;
        }
        if (f.type == 'S') {
            if (error) *error = "S columns are not allowed in rows";
            return false;
        }
        cols.push_back(std::move(f));
    }
    if (cols.empty()) {
        if (error) *error = "empty row";
        return false;
    }
    return true;
}

bool parse_body(std::string_view s, std::vector<BodyItem> &items, std::string *error)
{
    s = trim(s);
    if (s == "-" || s.empty()) {
        return true;
    }
    std::size_t i = 0;
    while (i < s.size()) {
        while (i < s.size() && std::isspace(static_cast<unsigned char>(s[i]))) {
            ++i;
        }
        if (i >= s.size()) {
            break;
        }
        // Optional "N*" prefix.
        BodyItem item;
        std::size_t j = i;
        while (j < s.size() && s[j] != '*' && s[j] != '(' && !std::isspace(static_cast<unsigned char>(s[j]))) {
            ++j;
        }
        std::string_view head = s.substr(i, j - i);
        if (head == "...") {
            item.kind = BodyItem::Kind::Rest;
            items.push_back(std::move(item));
            i = j;
            continue;
        }
        if (j < s.size() && s[j] == '*') {
            if (!parse_count(head, item.count, item.count_ref)) {
                if (error) *error = "bad count '" + std::string(head) + "'";
                return false;
            }
            i = j + 1;
        } else if (!head.empty() && head != "TEXT") {
            if (error) *error = "unexpected body item '" + std::string(head) + "'";
            return false;
        }
        if (s.compare(i, 4, "TEXT") == 0) {
            item.kind = BodyItem::Kind::Text;
            i += 4;
        } else if (i < s.size() && s[i] == '(') {
            const auto close = s.find(')', i);
            if (close == std::string_view::npos) {
                if (error) *error = "missing ')'";
                return false;
            }
            item.kind = BodyItem::Kind::Rows;
            if (!parse_columns(s.substr(i + 1, close - i - 1), item.columns, error)) {
                return false;
            }
            i = close + 1;
        } else {
            if (error) *error = "expected TEXT or (...) at '" + std::string(s.substr(i)) + "'";
            return false;
        }
        items.push_back(std::move(item));
    }
    return true;
}

struct LayoutTable {
    std::map<std::string, std::vector<Layout>, std::less<>> byKeyword;
    std::vector<std::string> errors;

    LayoutTable()
    {
        for (const char *raw : kLayoutLines) {
            std::string_view line = trim(raw);
            if (line.empty() || line.front() == '#') {
                continue;
            }
            std::string err;
            if (auto l = parse_layout_line(line, &err)) {
                byKeyword[l->keyword].push_back(std::move(*l));
            } else {
                errors.push_back(err + ": " + std::string(line));
            }
        }
    }
};

const LayoutTable &table()
{
    static const LayoutTable t;
    return t;
}

// Resolves a header field reference to its integer value.
std::optional<std::int64_t> ref_value(const std::vector<Value> &header, std::string_view name)
{
    for (const auto &v : header) {
        if (v.name == name && v.type == 'I' && v.ints.size() == 1) {
            return v.ints[0];
        }
    }
    return std::nullopt;
}

bool count_of(int literal, const std::string &ref, const std::vector<Value> &header, std::int64_t &out)
{
    if (ref.empty()) {
        out = literal;
        return true;
    }
    auto v = ref_value(header, ref);
    if (!v || *v < 0) {
        return false;
    }
    out = *v;
    return true;
}

bool match_header(const Layout &l, std::string_view raw, std::vector<Value> &out, std::string &why)
{
    out.clear();
    TokenCursor c(raw);
    std::string_view t;
    for (const auto &f : l.header) {
        Value v;
        v.name = f.name;
        v.type = f.type;
        if (f.type == 'S') {
            // Rest of the header line.
            if (!c.has_more()) {
                v.text = {};
            } else {
                const char *start = c.position();
                v.text = std::string_view(start, static_cast<std::size_t>(raw.data() + raw.size() - start));
                while (c.next(t)) {
                }
            }
            out.push_back(v);
            continue;
        }
        std::int64_t n = 0;
        if (!count_of(f.repeat, f.repeat_ref, out, n)) {
            why = "unresolved repeat " + f.repeat_ref;
            return false;
        }
        for (std::int64_t k = 0; k < n; ++k) {
            if (!c.next(t)) {
                why = "header too short at " + f.name;
                return false;
            }
            if (f.type == 'I') {
                auto iv = is_int_token(t) ? parse_int(t) : std::nullopt;
                if (!iv) {
                    why = "header field " + f.name + " is not an integer: " + std::string(t);
                    return false;
                }
                v.ints.push_back(*iv);
            } else {
                auto rv = parse_real(t);
                if (!rv) {
                    why = "header field " + f.name + " is not a number: " + std::string(t);
                    return false;
                }
                v.reals.push_back(*rv);
            }
        }
        out.push_back(std::move(v));
    }
    if (!l.header_open && c.next(t)) {
        why = "header has extra tokens";
        return false;
    }
    return true;
}

bool decode_body(const Layout &l, const Record &r, Decoded &d, std::string &why)
{
    TokenCursor c(r.body);
    bool mid_line = false;
    std::string_view t;
    for (const auto &item : l.body) {
        if (item.kind == BodyItem::Kind::Rest) {
            d.complete = false;
            return true;
        }
        std::int64_t n = 0;
        if (!count_of(item.count, item.count_ref, d.header, n)) {
            why = "unresolved count " + item.count_ref;
            return false;
        }
        if (item.kind == BodyItem::Kind::Text) {
            if (mid_line) {
                if (!c.at_line_end()) {
                    why = "text expected at line start";
                    return false;
                }
                c.skip_line();
                mid_line = false;
            }
            for (std::int64_t k = 0; k < n; ++k) {
                if (c.done()) {
                    // Blank trailing text lines are trimmed from the body.
                    d.texts.emplace_back();
                    continue;
                }
                d.texts.push_back(c.take_line());
            }
            continue;
        }
        Table tab;
        tab.rows = static_cast<std::size_t>(n);
        std::vector<std::int64_t> widths;
        for (const auto &col : item.columns) {
            std::int64_t w = 0;
            if (!count_of(col.repeat, col.repeat_ref, d.header, w)) {
                why = "unresolved width " + col.repeat_ref;
                return false;
            }
            Column cc;
            cc.name = col.name;
            cc.type = col.type;
            cc.width = static_cast<int>(w);
            if (col.type == 'I') {
                cc.ints.reserve(static_cast<std::size_t>(n * w));
            } else {
                cc.reals.reserve(static_cast<std::size_t>(n * w));
            }
            tab.columns.push_back(std::move(cc));
            widths.push_back(w);
        }
        for (std::int64_t row = 0; row < n; ++row) {
            for (std::size_t ci = 0; ci < tab.columns.size(); ++ci) {
                auto &col = tab.columns[ci];
                for (std::int64_t k = 0; k < widths[ci]; ++k) {
                    if (!c.next(t)) {
                        why = "body ended in row " + std::to_string(row + 1) + " of " + std::to_string(n);
                        return false;
                    }
                    if (col.type == 'I') {
                        auto iv = is_int_token(t) ? parse_int(t) : std::nullopt;
                        if (!iv) {
                            why = "column " + std::string(col.name) + " row " + std::to_string(row + 1) +
                                  " is not an integer: " + std::string(t);
                            return false;
                        }
                        col.ints.push_back(static_cast<std::int32_t>(*iv));
                    } else {
                        auto rv = parse_real(t);
                        if (!rv) {
                            why = "column " + std::string(col.name) + " row " + std::to_string(row + 1) +
                                  " is not a number: " + std::string(t);
                            return false;
                        }
                        col.reals.push_back(*rv);
                    }
                }
            }
            mid_line = true;
        }
        d.tables.push_back(std::move(tab));
    }
    if (c.next(t)) {
        why = "body has extra tokens starting at '" + std::string(t) + "'";
        return false;
    }
    return true;
}

} // namespace

std::optional<Layout> parse_layout_line(std::string_view line, std::string *error)
{
    std::vector<std::string_view> parts;
    std::size_t start = 0;
    for (int i = 0; i < 5; ++i) {
        const auto bar = line.find('|', start);
        if (bar == std::string_view::npos) {
            break;
        }
        parts.push_back(trim(line.substr(start, bar - start)));
        start = bar + 1;
    }
    parts.push_back(trim(line.substr(start)));
    if (parts.size() < 5) {
        if (error) *error = "expected 5 or 6 '|' separated fields";
        return std::nullopt;
    }
    Layout l;
    l.source = std::string(line);
    l.keyword = std::string(parts[0]);
    const auto scope = parts[1];
    if (scope == "global") l.scope = Scope::Global;
    else if (scope == "object") l.scope = Scope::Object;
    else if (scope == "material") l.scope = Scope::Material;
    else if (scope == "pair") l.scope = Scope::Pair;
    else if (scope == "action") l.scope = Scope::Action;
    else l.scope = Scope::Unknown;
    const auto prov = parts[2];
    if (prov == "observed") l.provenance = Provenance::Observed;
    else if (prov == "observed-differs") l.provenance = Provenance::ObservedDiffers;
    else if (prov == "manual") l.provenance = Provenance::Manual;
    else l.provenance = Provenance::Unknown;

    const auto header = parts[3];
    if (header != "-" && !header.empty()) {
        for (auto tok : split_ws(header)) {
            if (tok == "...") {
                l.header_open = true;
                continue;
            }
            if (l.header_open) {
                if (error) *error = "'...' must be last in header";
                return std::nullopt;
            }
            FieldSpec f;
            if (!parse_field(tok, f, error)) {
                return std::nullopt;
            }
            l.header.push_back(std::move(f));
        }
    }
    if (!parse_body(parts[4], l.body, error)) {
        return std::nullopt;
    }
    if (parts.size() > 5) {
        l.note = std::string(parts[5]);
    }
    return l;
}

std::span<const Layout> layouts_for(std::string_view keyword)
{
    const auto &m = table().byKeyword;
    const auto it = m.find(keyword);
    if (it == m.end()) {
        return {};
    }
    return it->second;
}

std::vector<std::string_view> keywords_with_layouts()
{
    std::vector<std::string_view> out;
    for (const auto &[k, v] : table().byKeyword) {
        out.push_back(k);
    }
    return out;
}

std::span<const std::string> layout_table_errors()
{
    return table().errors;
}

std::optional<std::size_t> text_line_count(std::string_view keyword,
                                           std::span<const std::string_view> header_tokens)
{
    for (const auto &l : layouts_for(keyword)) {
        if (l.body.empty()) {
            continue;
        }
        const bool all_text = std::all_of(l.body.begin(), l.body.end(), [](const BodyItem &b) {
            return b.kind == BodyItem::Kind::Text;
        });
        if (!all_text) {
            continue;
        }
        std::size_t total = 0;
        bool ok = true;
        for (const auto &b : l.body) {
            if (b.count_ref.empty()) {
                total += static_cast<std::size_t>(std::max(0, b.count));
                continue;
            }
            // Position of the referenced header field (fields before it are
            // single tokens or literal repeats).
            std::size_t pos = 0;
            bool found = false;
            for (const auto &f : l.header) {
                if (f.name == b.count_ref) {
                    found = true;
                    break;
                }
                if (!f.repeat_ref.empty() || f.type == 'S') {
                    break;
                }
                pos += static_cast<std::size_t>(f.repeat);
            }
            if (!found || pos >= header_tokens.size()) {
                ok = false;
                break;
            }
            auto v = parse_int(header_tokens[pos]);
            if (!v || *v < 0 || *v > 100000) {
                ok = false;
                break;
            }
            total += static_cast<std::size_t>(*v);
        }
        if (ok) {
            return total;
        }
    }
    return std::nullopt;
}

const Column *Table::column(std::string_view name) const noexcept
{
    for (const auto &c : columns) {
        if (c.name == name) {
            return &c;
        }
    }
    return nullptr;
}

const Value *Decoded::field(std::string_view name) const noexcept
{
    for (const auto &v : header) {
        if (v.name == name) {
            return &v;
        }
    }
    return nullptr;
}

std::optional<std::int64_t> Decoded::header_int(std::string_view name) const noexcept
{
    const auto *v = field(name);
    if (v && v->type == 'I' && !v->ints.empty()) {
        return v->ints[0];
    }
    return std::nullopt;
}

std::optional<double> Decoded::header_real(std::string_view name) const noexcept
{
    const auto *v = field(name);
    if (!v) {
        return std::nullopt;
    }
    if (v->type == 'R' && !v->reals.empty()) {
        return v->reals[0];
    }
    if (v->type == 'I' && !v->ints.empty()) {
        return static_cast<double>(v->ints[0]);
    }
    return std::nullopt;
}

std::optional<Decoded> decode(const Record &record, std::string *why)
{
    const auto alts = layouts_for(record.name);
    if (alts.empty()) {
        if (why) *why = "no layout for " + std::string(record.name);
        return std::nullopt;
    }
    std::string reasons;
    for (const auto &l : alts) {
        Decoded d;
        d.layout = &l;
        std::string w;
        if (!match_header(l, record.header, d.header, w) || !decode_body(l, record, d, w)) {
            if (!reasons.empty()) reasons += "; ";
            reasons += "[" + l.source.substr(0, 60) + "] " + w;
            continue;
        }
        return d;
    }
    if (why) *why = reasons;
    return std::nullopt;
}

std::string_view to_string(Scope s) noexcept
{
    switch (s) {
    case Scope::Global: return "global";
    case Scope::Object: return "object";
    case Scope::Material: return "material";
    case Scope::Pair: return "pair";
    case Scope::Action: return "action";
    default: return "unknown";
    }
}

std::string_view to_string(Provenance p) noexcept
{
    switch (p) {
    case Provenance::Observed: return "observed";
    case Provenance::ObservedDiffers: return "observed-differs";
    case Provenance::Manual: return "manual";
    default: return "unknown";
    }
}

} // namespace dk
