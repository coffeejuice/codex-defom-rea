#pragma once

#include <array>
#include <cstddef>
#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace deform::recovered {

// Internal byte-stream component, not a KEY parser or a file-opening API.
// Input must ALREADY have the reference text-mode translation applied. No
// CRLF/Ctrl-Z/encoding conversion is performed here.
//
// Float input items use the recovered decimal-only lexer; C-locale strtod
// converts the bounded accepted decimal substring. Numerical compatibility is
// limited to recorded cases, not all rounding edges of the legacy converter.
// Established scope excludes overflowing integer conversions, floating
// overflow/underflow, nonfinite values, embedded NUL and locale changes during
// a read. No behavior is claimed for those inputs.
class KeywordReader {
public:
    struct LineRead {
        std::size_t offset = 0;
        std::size_t cursor = 0;
        std::string bytes;
        int status = 0; // 1 means fgets would return null at EOF.
    };
    struct IntegerRead {
        std::array<int, 9> values{};
        std::size_t count = 0;
        int status = 1;
        int conversions = -1; // -1 also used when no scan occurs at EOF.
    };
    struct IntegerArrayRead {
        std::array<int, 10> values{};
        int status = 1; // 4147e0: only fgets EOF is failure.
        int conversions = -1; // Only this prefix has defined native values.
    };
    struct CoordinateRead {
        int label = 0;
        std::array<double, 3> values{};
        std::size_t dimensions = 0;
        int status = 1;
        int conversions = -1;
    };
    struct MatrixRowRead {
        int label = 0;
        std::array<double, 4> values{};
        int status = 1;
        int conversions = -1;
    };
    struct FloatRead {
        std::array<double, 2> values{};
        std::size_t count = 0;
        int status = 1;
        int conversions = -1;
    };
    struct RealIntegerRead {
        double real_value = 0.0;
        int integer_value = 0;
        int status = 1;
        int conversions = -1;
    };
    struct MixedIntegerRealRead {
        std::array<int, 2> integers{};
        double real_value = 0.0;
        int status = 1;
        int conversions = -1;
    };
    struct CustomHeaderRead {
        // Native416700 output order: A, B, declared rows, C. The real default
        // occurs between the third and fourth integers in the input format.
        std::array<int, 4> integers{};
        double default_value = 0.0;
        int status = 1;
        int conversions = -1;
    };
    struct ShapeHeaderRead {
        int rows = 0;
        int selector = 0;
        int width = 0;
        double default_value = 0.0;
        int status = 1;
        int conversions = -1;
    };
    struct DynamicHeaderRead {
        std::vector<std::string> tokens; // At most ten; empty header has one empty fallback token.
        std::array<int, 6> integers{}; // Base10 token0..5; omitted token5 defaults to token3.
        double default_value = 0.0; // Token6 when present.
        int token_count = 0;
        int status = 1;
    };
    struct FloatTripleRead {
        std::array<double, 3> values{};
        std::size_t count = 3;
        int status = 1;
        int conversions = -1;
    };
    struct FloatQuadRead {
        std::array<double, 4> values{};
        std::size_t count = 4;
        int status = 1;
        int conversions = -1;
    };
    struct FloatFiveRead {
        std::array<double, 5> values{};
        int status = 1;
        int conversions = -1;
    };
    struct IntegerTripleRealRead {
        std::array<int, 3> integers{};
        double real_value = 0.0;
        int status = 1;
        int conversions = -1;
    };
    struct TextRead {
        std::string bytes;
        int status = 1;
    };
    struct MaterialColumnRead {
        std::vector<double> values; // Resulting destination, including partial row commits.
        std::vector<double> row_buffer; // Reused calloc row; a failed row can partially mutate it.
        std::vector<double> default_values; // Successfully assigned header defaults, in order.
        std::vector<bool> written; // Only actual destination stores are true.
        int status = 1;
        int header_conversions = -1;
        int declared_rows = 0;
        int explicit_width = 0;
        double default_value = 0.0;
        bool default_assigned = false;
        int completed_rows = 0;
        int row_id = 0; // Meaningful only when row_id_initialized is true.
        bool row_id_initialized = false;
    };
    struct IntegerColumnRead {
        std::vector<int> row_buffer;
        std::vector<bool> written; // Required destination extent; only actual stores are true.
        int status = 1;
        int header_conversions = -1;
        int declared_rows = 0;
        int default_value = 0; // Meaningful only when default_assigned is true.
        bool default_assigned = false;
        int completed_rows = 0;
        int row_id = 0;
        bool row_id_initialized = false;
    };

    explicit KeywordReader(std::string_view translated_bytes, std::size_t offset = 0);

    // fscanf("%39s"): skip the six C-locale whitespace bytes, then consume
    // at most 39 non-whitespace bytes, leaving the following byte unread.
    [[nodiscard]] TextRead token();
    // Bounded fgets with size 1024, including LF if encountered.
    [[nodiscard]] LineRead line();
    [[nodiscard]] LineRead bounded_line(std::size_t capacity); //fgets capacity.

    // Stream fscanf("%i") used for object selection: consumes only the numeric
    // prefix, including whitespace/sign on a failed conversion, not a line.
    [[nodiscard]] IntegerRead selector_integer();
    // Stream fscanf("%i %i"), without line consumption or comma normalization.
    [[nodiscard]] IntegerRead selector_pair();
    [[nodiscard]] IntegerRead integer(bool skip_one_empty_line = false);
    [[nodiscard]] IntegerRead integer_pair();
    // 413fb0: three zeroed integer destinations, optional native scan-count
    // output. DIEGEO uses conversions to distinguish two-field headers.
    [[nodiscard]] IntegerRead integer_triple();
    [[nodiscard]] IntegerRead integer_quad(); //414100, four zeroed int32 outputs.
    [[nodiscard]] IntegerRead connectivity3(); // 414100: label plus three integers.
    [[nodiscard]] IntegerRead connectivity4(bool skip_empty_lines = false);
    [[nodiscard]] IntegerRead connectivity8(bool skip_empty_lines = false);
    // 414990/414a40: one bounded line, comma replacement, zeroed outputs.
    // Input failure before any assignment returns status1, including a sign
    // or decimal point consumed at physical EOF. Matching failure with input
    // remaining, and partial successful conversions, return status0.
    [[nodiscard]] FloatRead real();
    [[nodiscard]] FloatRead real_pair();
    [[nodiscard]] FloatTripleRead real_triple(); // 414c20, three real destinations.
    [[nodiscard]] FloatTripleRead real_triple_clear_single(); // 414d10.
    [[nodiscard]] FloatFiveRead real_five(); // 414f50, five zeroed destinations.
    [[nodiscard]] IntegerTripleRealRead integer_triple_real(); // 4164e0.
    [[nodiscard]] FloatQuadRead real_quad(); // 414e40, four zeroed destinations.
    [[nodiscard]] IntegerArrayRead integer_array10(); // 4147e0 temporary scan.
    // 416aa0: one line, comma replacement, then legacy %Flf %i. Both
    // destinations start at zero; successful partial assignments survive.
    [[nodiscard]] RealIntegerRead real_integer();
    [[nodiscard]] RealIntegerRead integer_real(); //4154c0; reverse input order.
    // 415590: one fgets, commas become spaces, zeroed %i %Flf %i destinations.
    // integers[0] is rows and integers[1] is width; real_value is the default.
    [[nodiscard]] MixedIntegerRealRead integer_pair_real();
    // 416700: zeroed %i %i %i %Flf %i, one fgets and comma replacement.
    [[nodiscard]] CustomHeaderRead custom_variable_header();
    // 417c80 mode4 header only: zeroed %i %i %i %Flf, one raw fgets,
    // no comma replacement. Width<1 fails after parsing. The caller owns
    // width mutation, selector validation, allocation and default assignment.
    [[nodiscard]] ShapeHeaderRead shape_header();
    // DYNARY's413900(limit10)/413a50 token header, then base10 integer and
    // finite legacy atof conversions. Delimiters are space, comma, LF, TAB, CR.
    [[nodiscard]] DynamicHeaderRead dynamic_array_header();
    // Stream fscanf("%Flf"): no comma replacement or line boundary. Failed
    // assignment preserves previous_value. Cursor/EOF behavior is restricted
    // to the recovered decimal scanner; nonfinite values remain unsupported.
    [[nodiscard]] FloatRead stream_real(double previous_value = 0.0);
    // 416c90 mode1: one header line, no comma replacement, then419b90/4199a0.
    // Returns owned scratch only; the caller commits it only on status0.
    // Width0 uses input_width and requires trailing fgets after positive rows;
    // positive explicit width consumes stream records without that final read.
    // Nonnegative bounded allocation extents and initialized row IDs only.
    [[nodiscard]] MaterialColumnRead material_columns(int element_count, int input_width);
    // 416c90 modes0/1/6: mutate defaults and completed rows immediately, even
    // on later failure. Mode0 reads count/default; mode1 count/width/default;
    // mode6 count/up to ten defaults (exact width gives per-column defaults,
    // otherwise the first default broadcasts). No default preserves prior data.
    // Mode6 width>10 returns1 before fgets. Other modes, invalid extents and
    // first-row uninitialized IDs are excluded. Destination must be initialized.
    [[nodiscard]] MaterialColumnRead real_columns(std::span<double> destination,
        int row_count, int width, int mode);
    // Raw4199a0: no header/default; reuse a zeroed row buffer, commit completed
    // valid rows immediately, then require one fgets when declared_rows>0.
    [[nodiscard]] MaterialColumnRead real_rows(std::span<double> destination,
        int declared_rows, int row_count, int width);
    // Raw419b90 with positive explicit width equal to destination width:
    // same sparse row commits, without a final fgets or header/default fill.
    [[nodiscard]] MaterialColumnRead real_rows_explicit(std::span<double> destination,
        int declared_rows, int row_count, int width);
    // 416c90 mode2: initial width>10 rejects before input. A read header assigns
    // width from its zero-initialized second integer before checking conversions
    // or destination extent. No allocation is inferred or resized. Effective
    // width/row product must fit the supplied initialized span before any store.
    [[nodiscard]] MaterialColumnRead real_columns_mode2(std::span<double> destination,
        int row_count, int& width);
    // 419d40/419ea0: default and completed rows mutate destination immediately,
    // even when a later read fails. The untouched destination is never read.
    // Validates extent before consuming input. Header conversion0 and the first
    // row-ID conversion0 leave native locals uninitialized and are excluded.
    [[nodiscard]] IntegerColumnRead integer_columns(std::span<int> destination,
        int element_count, int width);
    [[nodiscard]] CoordinateRead coordinates2();
    [[nodiscard]] CoordinateRead coordinates3();
    // 415990: one bounded line, comma replacement, zeroed label and four
    // real destinations. Partial conversions succeed and retain zero tails.
    [[nodiscard]] MatrixRowRead matrix_row();
    // 413ab0 at the OBJNAM call's capacity 81. On either line-read failure,
    // preserve the supplied previous name; no reset or object mutation implied.
    [[nodiscard]] TextRead name(std::string_view previous_name = {});
    // Same 413ab0 helper with MTNAME capacity65: at most64 name bytes.
    [[nodiscard]] TextRead material_name(std::string_view previous_name = {});
    // FRNNAM uses413ab0 with capacity64: at most63 stored name bytes.
    [[nodiscard]] TextRead root_name(std::string_view previous_name = {});
    //413c60: one line only; capacity64 with the same bounded trim semantics.
    [[nodiscard]] TextRead furnace_text_line(std::string_view previous_name = {});

    [[nodiscard]] std::size_t cursor() const noexcept { return cursor_; }
    [[nodiscard]] const std::vector<LineRead>& line_reads() const noexcept { return line_reads_; }
    [[nodiscard]] std::string_view input() const noexcept { return input_; }

private:
    enum class BlankRule { none, once, repeated };
    IntegerRead integers(std::size_t count, BlankRule rule);
    CoordinateRead coordinates(std::size_t dimensions);
    FloatRead reals(std::size_t count);
    TextRead name_with_capacity(std::string_view previous_name, std::size_t capacity);
    TextRead single_name_line(std::string_view previous_name, std::size_t capacity);
    MaterialColumnRead real_rows_impl(std::span<double> destination,
        int declared_rows, int row_count, int width, bool trailing_line);

    std::string input_;
    std::size_t cursor_ = 0;
    std::vector<LineRead> line_reads_;
};

} // namespace deform::recovered
