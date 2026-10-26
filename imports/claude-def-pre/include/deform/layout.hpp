#pragma once

// Record layouts. Each keyword has zero or more alternative layouts written in
// a small text DSL (data/keyword_layouts.txt, see docs/keyword-layouts.md):
//
//   KEYWORD | scope | provenance | header | body | note
//
// header: `name:T` fields, T in I/R/S, `name:R*3` repeats, `-` empty,
//         trailing `...` = more tokens allowed.
// body:   `-` none, `TEXT`, `N*TEXT`, `N*(cols)`, `(cols)`, `...` (rest is
//         not described). N is a header field name or a literal. Columns are
//         `name:I`, `name:R`, `name:I*K`, `name:R*K` with K literal or a
//         header field name. Rows are a token stream; line wrapping is ignored.

#include <cstdint>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace dk {

class Record;

enum class Scope : std::uint8_t { Global, Object, Material, Pair, Action, Unknown };
enum class Provenance : std::uint8_t { Observed, ObservedDiffers, Manual, Unknown };

struct FieldSpec {
    std::string name;
    char type = 'I';           // 'I', 'R' or 'S'
    int repeat = 1;            // literal repeat count
    std::string repeat_ref;    // header field giving the repeat count (overrides repeat)
};

struct BodyItem {
    enum class Kind : std::uint8_t { Text, Rows, Rest };
    Kind kind = Kind::Rows;
    int count = 1;             // literal count
    std::string count_ref;     // header field giving the count (overrides count)
    std::vector<FieldSpec> columns;
};

struct Layout {
    std::string keyword;
    Scope scope = Scope::Unknown;
    Provenance provenance = Provenance::Unknown;
    std::vector<FieldSpec> header;
    bool header_open = false;  // trailing `...`
    std::vector<BodyItem> body;
    std::string note;
    std::string source;        // the DSL line
};

// Layout alternatives for a keyword, in priority order (empty when unknown).
[[nodiscard]] std::span<const Layout> layouts_for(std::string_view keyword);

// Every keyword that has at least one layout.
[[nodiscard]] std::vector<std::string_view> keywords_with_layouts();

// Syntax errors found in the compiled-in layout table (should be empty).
[[nodiscard]] std::span<const std::string> layout_table_errors();

// Parses one DSL line (exposed for tests and tools). Returns nullopt and sets
// `error` on syntax errors.
[[nodiscard]] std::optional<Layout> parse_layout_line(std::string_view line, std::string *error);

// Number of whole text lines a record of this keyword consumes after its
// header line, given the header tokens. nullopt when the keyword's body is
// not purely text (it then ends at the next keyword or comment line).
[[nodiscard]] std::optional<std::size_t> text_line_count(std::string_view keyword,
                                                         std::span<const std::string_view> header_tokens);

// ---------------------------------------------------------------------------
// Decoding a record with its layout.

struct Value {
    std::string_view name;
    char type = 'I';
    std::vector<std::int64_t> ints;   // type 'I'
    std::vector<double> reals;        // type 'R'
    std::string_view text;            // type 'S'
};

struct Column {
    std::string_view name;
    char type = 'I';
    int width = 1;                    // values per row
    std::vector<std::int32_t> ints;   // rows * width, type 'I'
    std::vector<double> reals;        // rows * width, type 'R'
};

struct Table {
    std::size_t rows = 0;
    std::vector<Column> columns;
    [[nodiscard]] const Column *column(std::string_view name) const noexcept;
};

struct Decoded {
    const Layout *layout = nullptr;
    std::vector<Value> header;
    std::vector<Table> tables;
    std::vector<std::string_view> texts;
    bool complete = true;             // false when the body ended in `...`
    [[nodiscard]] const Value *field(std::string_view name) const noexcept;
    [[nodiscard]] std::optional<std::int64_t> header_int(std::string_view name) const noexcept;
    [[nodiscard]] std::optional<double> header_real(std::string_view name) const noexcept;
};

// Decodes `record` with the first matching layout alternative. When no
// alternative matches, returns nullopt and explains why in `why`.
[[nodiscard]] std::optional<Decoded> decode(const Record &record, std::string *why = nullptr);

[[nodiscard]] std::string_view to_string(Scope s) noexcept;
[[nodiscard]] std::string_view to_string(Provenance p) noexcept;

} // namespace dk
