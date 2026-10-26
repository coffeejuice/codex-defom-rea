#include "type3_opt_ascii_file.hpp"

#include <fstream>
#include <iterator>
#include <string>
#include <vector>

#include "m2m/qt4num.hpp"
#include "type3_optimizer_scalar_sequence.hpp"
#include "type3_region_sequence.hpp"

namespace m2m::mst::detail {
namespace {

bool isAsciiLineCount(const std::vector<qt::String>& fields, std::int32_t& count)
{
    if (fields.size() < 2) return false;
    for (char16_t codeUnit : fields[1].u16())
        if (codeUnit > 0x7f) return false;
    bool ok = false;
    count = qt::toInt(fields[1], &ok);
    return ok;
}

qt::String asciiTrimmed(const qt::String& value)
{
    const auto& text = value.u16();
    std::size_t begin = 0;
    std::size_t end = text.size();
    const auto isSpace = [](char16_t c) {
        return (c >= 9 && c <= 13) || c == 32;
    };
    while (begin < end && isSpace(text[begin])) ++begin;
    while (end > begin && isSpace(text[end - 1])) --end;
    return qt::String(text.substr(begin, end - begin));
}

bool splitAdmittedFile(const std::string& bytes, std::vector<qt::String>& lines)
{
    lines.clear();
    if (bytes.empty()) return true;
    if (bytes.back() != '\n') return false;

    std::string line;
    line.reserve(bytes.size());
    for (unsigned char byte : bytes) {
        if (byte == '\n') {
            lines.push_back(qt::String::latin1(line));
            line.clear();
            continue;
        }
        // The finite profile excludes BOMs, locale-dependent bytes, CR, NUL,
        // and C0 controls other than TAB (used as a row-field separator).
        if (byte > 0x7f || byte == '\r' || byte == 0 ||
            (byte < 0x20 && byte != '\t'))
            return false;
        line.push_back(static_cast<char>(byte));
    }
    return line.empty();
}

Type3OptAsciiFileOutcome applyBodyLines(
    Type3OptimizerScalarState& state,
    const std::vector<qt::String>& lines)
{
    std::size_t cursor = 0;
    while (cursor < lines.size()) {
        std::vector<qt::String> fields;
        if (tokenizeType3OptimizerLine(lines[cursor], fields) !=
            Type3LineTokenizeResult::Tokenized)
            return Type3OptAsciiFileOutcome::BodyGuard;

        if (fields.empty()) {
            ++cursor;
            continue;
        }

        if (fields.front() == qt::L("REGIONS")) {
            std::int32_t count = 0;
            if (!isAsciiLineCount(fields, count))
                return Type3OptAsciiFileOutcome::BodyGuard;
            ++cursor; // The REGIONS count line is not a row.
            const auto rows = appendType3RegionRows(
                state, count, std::span<const qt::String>(lines).subspan(cursor));
            cursor += rows.rowsConsumed;
            if (rows.stop == Type3RegionSequenceStop::RowCountUnrepresentable)
                return Type3OptAsciiFileOutcome::BodyGuard;
            if (rows.stop == Type3RegionSequenceStop::InputEnded)
                return Type3OptAsciiFileOutcome::EndOfFile;
            continue;
        }

        if (fields.front() == qt::L("NUM_CON")) {
            std::int32_t count = 0;
            if (!isAsciiLineCount(fields, count))
                return Type3OptAsciiFileOutcome::BodyGuard;
            const std::size_t remaining = lines.size() - cursor - 1;
            const std::size_t requested = count > 0 ? static_cast<std::size_t>(count) : 0;
            const std::size_t supplied = requested < remaining ? requested : remaining;
            const auto block = std::span<const qt::String>(lines).subspan(cursor, supplied + 1);
            const auto sequence = applyType3OptimizerScalarLines(state, block);
            if (sequence.stop != Type3ScalarSequenceStop::AllLinesConsumed)
                return Type3OptAsciiFileOutcome::BodyGuard;
            cursor += supplied + 1;
            if (supplied < requested)
                return Type3OptAsciiFileOutcome::EndOfFile;
            continue;
        }

        const auto lineResult = applyType3OptimizerScalarLine(state, fields);
        if (lineResult == Type3ScalarLineResult::Terminate)
            return Type3OptAsciiFileOutcome::EndData;
        if (lineResult == Type3ScalarLineResult::RowBodyNotImplemented)
            return Type3OptAsciiFileOutcome::BodyGuard;
        ++cursor;
    }
    return Type3OptAsciiFileOutcome::EndOfFile;
}

}  // namespace

Type3OptAsciiFileResult parseType3OptAsciiFile(
    const std::filesystem::path& path,
    Type3OptimizerScalarState& state)
{
    if (!path.is_absolute()) return {Type3OptAsciiFileOutcome::UnsupportedPath};

    std::ifstream input(path, std::ios::binary);
    if (!input.is_open()) return {Type3OptAsciiFileOutcome::OpenFailed};

    // Native parser sets optimizer +0x48 only after QFile::open succeeds.
    state.value48 = true;

    const std::string bytes((std::istreambuf_iterator<char>(input)),
                            std::istreambuf_iterator<char>());
    if (input.bad())
        return {Type3OptAsciiFileOutcome::ReadFailureUnsupported};

    std::vector<qt::String> lines;
    if (!splitAdmittedFile(bytes, lines))
        return {Type3OptAsciiFileOutcome::UnsupportedFileProfile};

    // Native consumes the first readLine separately, calls trimmed() on a
    // discarded temporary, then toInt(base 10) on the original header. This
    // ASCII-profile trim is also discarded before converting the original.
    qt::String header;
    if (!lines.empty()) {
        header = lines.front();
        lines.erase(lines.begin());
    }
    {
        const qt::String trimmedHeader = asciiTrimmed(header);
        (void)trimmedHeader;
    }
    (void)qt::toInt(header);

    return {applyBodyLines(state, lines)};
}

}  // namespace m2m::mst::detail
