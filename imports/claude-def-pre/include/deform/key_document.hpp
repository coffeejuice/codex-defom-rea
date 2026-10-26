#pragma once

// KEY file parsing. A KEY file is a sequence of records:
//
//   KEYWD   h1  h2 ...        <- keyword at column 1, then header fields
//   body lines ...            <- until the next keyword or '*' comment line
//
// Text bodies (TITLE, OBJNAM, UNNAME ...) are consumed by exact line count so
// a name that looks like a keyword cannot split a record. Numeric rows longer
// than one line continue on lines whose first 8 columns are blank.

#include "deform/keywords.hpp"
#include "deform/numbers.hpp"

#include <cstdint>
#include <filesystem>
#include <memory>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace dk {

struct Diagnostic {
    enum class Severity : std::uint8_t { Info, Warning, Error };
    Severity severity = Severity::Warning;
    std::uint32_t line = 0;
    std::string message;
};

class Record {
public:
    std::string_view name;            // keyword as written
    const KeywordInfo *info = nullptr; // nullptr: not in the registry
    std::string_view header;          // rest of the keyword line
    std::string_view body;            // body lines (raw, comments excluded)
    std::uint32_t line = 0;           // 1-based line of the keyword
    std::uint32_t body_line_count = 0;

    [[nodiscard]] std::vector<std::string_view> header_tokens() const;
    [[nodiscard]] std::optional<std::int64_t> header_int(std::size_t index) const;
    [[nodiscard]] std::optional<double> header_real(std::size_t index) const;
    [[nodiscard]] std::vector<std::string_view> body_lines() const;

    // Logical numeric rows: wrapped lines merged into the preceding row.
    [[nodiscard]] std::vector<std::vector<std::string_view>> rows() const;

    // All body tokens in order (line structure ignored).
    [[nodiscard]] std::vector<std::string_view> body_tokens() const;

    // Object (or material / first object of a pair) number: the first header
    // integer for object, material and inter-object keywords; nullopt otherwise.
    [[nodiscard]] std::optional<int> owner() const;
};

struct ParseOptions {
    bool keep_unregistered = true; // unregistered keyword-looking lines become records
};

class KeyDocument {
public:
    KeyDocument() = default;

    [[nodiscard]] static KeyDocument parse(std::string text, const ParseOptions &options = {});
    [[nodiscard]] static KeyDocument load(const std::filesystem::path &path,
                                          const ParseOptions &options = {});

    [[nodiscard]] std::span<const Record> records() const noexcept { return records_; }
    [[nodiscard]] const std::vector<Diagnostic> &diagnostics() const noexcept { return diagnostics_; }
    [[nodiscard]] std::string_view text() const noexcept
    {
        return text_ ? std::string_view(*text_) : std::string_view{};
    }

    // Records of one keyword, in file order.
    [[nodiscard]] std::vector<const Record *> find_all(std::string_view keyword) const;
    // First record of the keyword, optionally owned by `owner`.
    [[nodiscard]] const Record *find(std::string_view keyword,
                                     std::optional<int> owner = std::nullopt) const;

private:
    std::shared_ptr<const std::string> text_;
    std::vector<Record> records_;
    std::vector<Diagnostic> diagnostics_;
};

} // namespace dk
