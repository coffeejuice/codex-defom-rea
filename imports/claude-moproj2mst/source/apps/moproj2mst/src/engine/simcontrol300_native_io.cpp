// Native scalar sidecars: bounded KnownC/ASCII input profile. See
// analysis/wave2-save-diffs-300-native-reader-contract.json for qualifications.
#include "m2m/engine/simcontrol300_io.hpp"
#include "m2m/common.hpp"
#include "m2m/doe_io.hpp"
#include "m2m/mst/file_device.hpp"
#include "m2m/qt4path.hpp"
#include "engine/simcontrol300_qt_int_parser.hpp"
#include <algorithm>
#include <bit>
#include <charconv>
#include <cmath>
#include <cstdio>
#include <cstdint>
#include <functional>
#include <limits>
#include <windows.h>

namespace m2m::engine::simcontrol300 {
namespace {
bool asciiSpace(unsigned char c) { return (c >= 9 && c <= 13) || c == 32; }

int qtBmpDecimalDigit(std::uint32_t c)
{
    static constexpr std::uint16_t starts[] = {
        0x0030,0x0660,0x06f0,0x07c0,0x0966,0x09e6,0x0a66,0x0ae6,
        0x0b66,0x0be6,0x0c66,0x0ce6,0x0d66,0x0e50,0x0ed0,0x0f20,
        0x1040,0x17e0,0x1810,0x1946,0x19d0,0x1b50,0xff10};
    for (const auto start : starts)
        if (c >= start && c < start + 10) return static_cast<int>(c - start);
    return -1;
}

std::string decodeSpringUnicodeDigits(const std::string& bytes, bool& hasNonAscii)
{
    const auto at = [&bytes](std::size_t i) { return static_cast<unsigned char>(bytes[i]); };
    std::size_t offset = 0, width = 0;
    bool be = false;
    if (bytes.size() >= 4 && at(0)==0 && at(1)==0 && at(2)==0xfe && at(3)==0xff) { offset=4;width=4;be=true; }
    else if (bytes.size() >= 4 && at(0)==0xff && at(1)==0xfe && at(2)==0 && at(3)==0) { offset=4;width=4; }
    else if (bytes.size() >= 2 && at(0)==0xfe && at(1)==0xff) { offset=2;width=2;be=true; }
    else if (bytes.size() >= 2 && at(0)==0xff && at(1)==0xfe) { offset=2;width=2; }
    else throw Error(M2M_E_UNSUPPORTED, "SPRING2 Unicode decimal profile requires UTF-16/32 BOM");
    if ((bytes.size()-offset)%width) throw Error(M2M_E_UNSUPPORTED, "SPRING2 incomplete Unicode unit");
    std::string normalized;
    hasNonAscii = false;
    for (auto p=offset; p<bytes.size(); p+=width) {
        std::uint32_t cp=0;
        for (std::size_t i=0;i<width;++i) cp |= std::uint32_t(at(p+i)) << (8*(be?width-i-1:i));
        if (cp>0xffff || (cp>=0xd800 && cp<=0xdfff)) throw Error(M2M_E_UNSUPPORTED,"SPRING2 non-BMP/surrogate excluded");
        if (cp<128) normalized.push_back(static_cast<char>(cp));
        else {
            const int d=qtBmpDecimalDigit(cp);
            if (d<0) throw Error(M2M_E_UNSUPPORTED,"SPRING2 Unicode category outside recovered BMP Nd profile");
            normalized.push_back(static_cast<char>(0x80+d));
            hasNonAscii = true;
        }
    }
    return normalized;
}

int readUnicodeDecimalFlag(const std::string& bytes, std::size_t& cursor)
{
    while(cursor<bytes.size() && asciiSpace(static_cast<unsigned char>(bytes[cursor]))) ++cursor;
    auto start=cursor;
    if(cursor<bytes.size() && (bytes[cursor]=='+' || bytes[cursor]=='-')) ++cursor;
    auto digits=cursor;
    while(cursor<bytes.size() && ((bytes[cursor]>='0' && bytes[cursor]<='9') ||
          (static_cast<unsigned char>(bytes[cursor])>=0x80 && static_cast<unsigned char>(bytes[cursor])<=0x89))) {
        ++cursor;
    }
    if(digits==cursor || (bytes[digits]=='0' && cursor-digits>1) ||
       (cursor<bytes.size() && !asciiSpace(static_cast<unsigned char>(bytes[cursor]))))
        throw Error(M2M_E_UNSUPPORTED,"SPRING2 Unicode decimal token outside bounded profile");
    std::int64_t value=0;
    for(auto i=digits;i<cursor;++i) {
        const auto ch=static_cast<unsigned char>(bytes[i]);
        const auto d=ch>=0x80 ? ch-0x80 : ch-'0';
        value=value*10+d;
        if(value>2147483648ll) throw Error(M2M_E_UNSUPPORTED,"SPRING2 Unicode decimal outside int32 range");
    }
    const bool negative=start<digits && bytes[start]=='-';
    if(negative) value=-value;
    if(value<std::numeric_limits<std::int32_t>::min() || value>std::numeric_limits<std::int32_t>::max())
        throw Error(M2M_E_UNSUPPORTED,"SPRING2 Unicode decimal outside int32 range");
    return static_cast<int>(value);
}

// Original1407fbc10 takes narrow literal names, calls fopen("rt"), fclose,
// then remove -> DeleteFileA. Absolute Unicode/extended-path substitution is
// not established by that source. This adapter's first directory profile is
// explicitly narrower; original ASCII relative names under a Unicode CWD
// remain a recovery dependency, not a claim that the original rejects them.
std::string ordinaryNarrowPath(const qt::String& path, std::size_t reserve = 0)
{
    const auto& text = path.u16();
    if (text.size() + reserve >= MAX_PATH)
        throw Error(M2M_E_UNSUPPORTED, "editor300 sidecar path outside ordinary MAX_PATH profile");
    std::string narrow;
    for (const auto c : text) {
        if (c < 32 || c >= 128)
            throw Error(M2M_E_UNSUPPORTED, "editor300 sidecar path outside ASCII profile");
        narrow.push_back(c == u'\\' ? '/' : static_cast<char>(c));
    }
    if (narrow.starts_with("//?/") || narrow.starts_with("//./"))
        throw Error(M2M_E_UNSUPPORTED, "editor300 extended/device path remains unsupported");
    return narrow;
}

// QFile-backed Extra3D reads/writes accept a QString path. Keep the ordinary
// path length/control/namespace profile without converting it through ACP or
// rejecting Unicode. The distinct readable-delete consumer still uses the
// narrow helper above.
void validateQFilePath(const qt::String& path, std::size_t reserve = 0)
{
    const auto& text = path.u16();
    if (text.size() + reserve >= MAX_PATH)
        throw Error(M2M_E_UNSUPPORTED, "editor300 sidecar path outside ordinary MAX_PATH profile");
    std::u16string normalized = text;
    for (auto& c : normalized) {
        if (c < 32) throw Error(M2M_E_UNSUPPORTED, "editor300 sidecar path outside ordinary path profile");
        if (c == u'\\') c = u'/';
    }
    if (normalized.starts_with(u"//?/") || normalized.starts_with(u"//./"))
        throw Error(M2M_E_UNSUPPORTED, "editor300 extended/device path remains unsupported");
}

// Original getReal's C-locale ASCII classes and transition table,6722c880.
int realClass(unsigned char c)
{
    if (c >= '0' && c <= '9') return 2;
    if (c == '+' || c == '-') return 1;
    if (c == '.') return 3;
    if (c == 'e' || c == 'E') return 4;
    if (c == 'i' || c == 'I') return 5;
    if (c == 'n' || c == 'N') return 6;
    if (c == 'f' || c == 'F') return 7;
    if (c == 'a' || c == 'A') return 8;
    if (c == 't' || c == 'T') return 9;
    return 0;
}

double readReal(const std::string& bytes, std::size_t& cursor)
{
    static constexpr unsigned char transitions[13][10] = {
        {0,1,2,3,0,10,8,0,0,0}, {0,0,2,3,0,10,8,0,0,0},
        {13,13,2,3,5,0,0,0,0,0}, {0,0,4,0,0,0,0,0,0,0},
        {13,13,4,13,5,0,0,0,0,0}, {0,6,7,0,0,0,0,0,0,0},
        {0,0,7,0,0,0,0,0,0,0}, {13,13,7,13,13,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,9,0}, {0,0,0,0,0,0,12,0,0,0},
        {0,0,0,0,0,0,11,0,0,0}, {0,0,0,0,0,0,0,12,0,0},
        {13,0,0,0,0,0,0,0,0,0}};
    while (cursor < bytes.size() && asciiSpace(static_cast<unsigned char>(bytes[cursor]))) ++cursor;
    int state = 0;
    std::string numeric;
    while (cursor < bytes.size()) {
        const auto c = static_cast<unsigned char>(bytes[cursor++]);
        state = transitions[state][realClass(c)];
        if (state == 0 || state == 13 || numeric.size() > 123) {
            --cursor; // original ungetChar
            if (numeric.size() > 123)
                while (cursor < bytes.size() && bytes[cursor] >= '0' && bytes[cursor] <= '9') ++cursor;
            break;
        }
        numeric.push_back(static_cast<char>(c));
    }
    return numeric.empty() ? 0.0 : qt::toDouble(qt::String::latin1(numeric));
}

int readDecimalInt(const std::string& bytes, std::size_t& cursor)
{
    while (cursor < bytes.size() && asciiSpace(static_cast<unsigned char>(bytes[cursor]))) ++cursor;
    if (cursor == bytes.size()) return 0;
    // Preserve the generic legacy profile for callers outside the qualified
    // one-value SW2SP route: lowercase 0x prefix and lowercase hex digits.
    if (cursor + 2 <= bytes.size() && bytes.compare(cursor, 2, "0x") == 0) {
        std::size_t digitCursor = cursor + 2;
        std::uint64_t value = 0;
        while (digitCursor < bytes.size()) {
            const auto ch = static_cast<unsigned char>(bytes[digitCursor]);
            unsigned digit = 0;
            if (ch >= '0' && ch <= '9') digit = ch - '0';
            else if (ch >= 'a' && ch <= 'f') digit = ch - 'a' + 10;
            else break;
            value = (value << 4) | digit;
            ++digitCursor;
        }
        if (digitCursor == cursor + 2)
            throw Error(M2M_E_UNSUPPORTED, "editor300 hexadecimal input requires a supported hex digit");
        cursor = digitCursor; // First non-hex character remains for the next extraction.
        const auto low = static_cast<std::uint32_t>(value);
        return static_cast<int>(std::bit_cast<std::int32_t>(low));
    }
    // Preserve the generic legacy zero-prefix fallback; it is not promoted to
    // the one-value SW2SP stream model.
    std::size_t decimalPrefix = cursor;
    if (bytes[decimalPrefix] == '+' || bytes[decimalPrefix] == '-') ++decimalPrefix;
    if (decimalPrefix + 1 < bytes.size() && bytes[decimalPrefix] == '0' &&
        (bytes[decimalPrefix + 1] == 'x' || bytes[decimalPrefix + 1] == 'X') &&
        (decimalPrefix != cursor || bytes[decimalPrefix + 1] == 'X')) {
        cursor = decimalPrefix + 1; // Leave x/X unread, as the legacy branch does.
        return 0;
    }
    // This legacy exact token was not behind the SW2SP-only flag, so preserve
    // its behavior for the generic reader while cleaning the dead ladder.
    if (cursor + 3 <= bytes.size() && bytes.compare(cursor, 3, "010") == 0 &&
        (cursor + 3 == bytes.size() || asciiSpace(static_cast<unsigned char>(bytes[cursor + 3])))) {
        cursor += 3;
        return 8;
    }
    const auto start = cursor;
    const bool hasSign = bytes[cursor] == '+' || bytes[cursor] == '-';
    if (hasSign) ++cursor;
    const auto digits = cursor;
    while (cursor < bytes.size() && bytes[cursor] >= '0' && bytes[cursor] <= '9') ++cursor;
    if (cursor == digits ||
        (bytes[digits] == '0' && cursor - digits > 1) ||
        (cursor < bytes.size() && !asciiSpace(static_cast<unsigned char>(bytes[cursor]))))
        throw Error(M2M_E_UNSUPPORTED, "editor300 integer input outside recovered decimal profile");
    bool valid = false;
    const int value = qt::toInt(qt::String::latin1(std::string_view(bytes).substr(start, cursor - start)), &valid);
    if (!valid) throw Error(M2M_E_UNSUPPORTED, "editor300 integer input outside recovered int32 range");
    return value;
}

std::string_view nextSpringToken(const std::string& bytes, std::size_t& cursor)
{
    while (cursor < bytes.size() && asciiSpace(static_cast<unsigned char>(bytes[cursor]))) ++cursor;
    const auto start = cursor;
    while (cursor < bytes.size() && !asciiSpace(static_cast<unsigned char>(bytes[cursor]))) ++cursor;
    return std::string_view(bytes).substr(start, cursor - start);
}

[[noreturn]] void unsupportedSpringNumber()
{
    throw Error(M2M_E_UNSUPPORTED, "editor300 SPRING numeric token outside bounded ASCII profile");
}

int readSpringInt(const std::string& bytes, std::size_t& cursor)
{
    const auto token = nextSpringToken(bytes, cursor);
    if (token.empty()) unsupportedSpringNumber();
    std::size_t digit = (token.front() == '+' || token.front() == '-') ? 1 : 0;
    if (digit == token.size()) unsupportedSpringNumber();
    for (std::size_t i = digit; i < token.size(); ++i)
        if (token[i] < '0' || token[i] > '9') unsupportedSpringNumber();
    if (token[digit] == '0' && token.size() - digit != 1) unsupportedSpringNumber();
    auto numeric = token;
    if (numeric.front() == '+') numeric.remove_prefix(1);
    int value = 0;
    const auto converted = std::from_chars(numeric.data(), numeric.data() + numeric.size(), value, 10);
    if (converted.ec != std::errc{} || converted.ptr != numeric.data() + numeric.size())
        unsupportedSpringNumber();
    return value;
}

int readSpringFlagInt(const std::string& bytes, std::size_t& cursor, bool& commaDelimiterPending,
                      bool allowSingleRowQualifiedToken, bool allowExactTwoRowBinaryRetry,
                      bool allowExactTwoRowInvalidHexRetry,
                      bool allowExactTwoRowOctalContinuation,
                      bool allowExactTwoRowBinaryContinuation,
                      bool allowExactTwoRowDecimal09EofProfile,
                      bool allowExactTwoRowUpperXRetry,
                      bool allowQQuestionCursor)
{
    while (cursor < bytes.size() && asciiSpace(static_cast<unsigned char>(bytes[cursor]))) ++cursor;
    if (cursor == bytes.size()) throw Error(M2M_E_UNSUPPORTED, "SPRING2 extraction attempted at EOF");

    const auto start = cursor;
    const bool bareHexPrefix = bytes.compare(start, 2, "0x") == 0 &&
        (start + 2 == bytes.size() || asciiSpace(static_cast<unsigned char>(bytes[start + 2])));
    const bool exactBareHexCrLfZero = allowSingleRowQualifiedToken && bytes == "0x\r\n";
    const bool exactUpperBareHexCrLfZero = allowSingleRowQualifiedToken && bytes == "0X\r\n";
    const bool exactBareHexEofZero = allowSingleRowQualifiedToken && bytes == "0x";
    const bool exactUpperBareHexEofZero = allowSingleRowQualifiedToken && bytes == "0X";
    if (bareHexPrefix && !exactBareHexEofZero && !exactBareHexCrLfZero)
        throw Error(M2M_E_UNSUPPORTED, "SPRING2 bare hexadecimal prefix remains unsupported");
    if (exactBareHexEofZero || exactUpperBareHexEofZero ||
        exactBareHexCrLfZero || exactUpperBareHexCrLfZero) {
        // Qt consumes the prefix and the int operator stores zero. For CRLF,
        // the candidate leaves CR unread; internal Qt status is not modeled.
        cursor = start + 2;
        commaDelimiterPending = false;
        return 0;
    }
    const bool exactHexA =
        (bytes.compare(start, 3, "0xA") == 0 || bytes.compare(start, 3, "0XA") == 0) &&
        (start + 3 == bytes.size() || asciiSpace(static_cast<unsigned char>(bytes[start + 3])));
    if (exactHexA) {
        cursor = start + 3;
        commaDelimiterPending = false;
        return 10;
    }
    const bool exactHexA0 = allowSingleRowQualifiedToken &&
        bytes.compare(start, 4, "0xA0") == 0 &&
        (start + 4 == bytes.size() || asciiSpace(static_cast<unsigned char>(bytes[start + 4])));
    if (exactHexA0) {
        cursor = start + 4;
        commaDelimiterPending = false;
        return 160;
    }
    const bool exactInvalidHexG = (allowSingleRowQualifiedToken || allowExactTwoRowInvalidHexRetry) &&
        bytes.compare(start, 3, "0xG") == 0 &&
        (start + 3 == bytes.size() || asciiSpace(static_cast<unsigned char>(bytes[start + 3])));
    const bool guardedInvalidHexG = !(allowSingleRowQualifiedToken || allowExactTwoRowInvalidHexRetry) &&
        bytes.compare(start, 3, "0xG") == 0 &&
        (start + 3 == bytes.size() || asciiSpace(static_cast<unsigned char>(bytes[start + 3])));
    if (guardedInvalidHexG)
        throw Error(M2M_E_UNSUPPORTED, "SPRING2 multi-value invalid-hex retry remains unsupported");
    if (exactInvalidHexG) {
        // Qt consumes the 0x prefix, ungets G and reports failure. The int
        // extraction stores zero; this caller ignores status. Keep this exact
        // approximation to one requested flag so a retry at G stays guarded.
        cursor = start + 2;
        commaDelimiterPending = false;
        return 0;
    }
    const bool exactHexGRetryAtUnreadDigit = allowExactTwoRowInvalidHexRetry &&
        bytes == "0xG\r\n" && start == 2;
    if (exactHexGRetryAtUnreadDigit) {
        // The first Qt failure leaves G unread after consuming 0x. The exact
        // two-row profile retries there once; it fails again and leaves G.
        cursor = start;
        commaDelimiterPending = false;
        return 0;
    }
    const bool oneRowCompleteHexPrefix = allowSingleRowQualifiedToken &&
        start + 2 < bytes.size() && bytes[start] == '0' &&
        (bytes[start + 1] == 'x' || bytes[start + 1] == 'X');
    if (oneRowCompleteHexPrefix) {
        std::size_t end = start + 2;
        std::uint32_t low = 0;
        std::size_t digitsRead = 0;
        while (end < bytes.size()) {
            const auto ch = static_cast<unsigned char>(bytes[end]);
            std::uint32_t digitValue = 0;
            if (ch >= '0' && ch <= '9') digitValue = ch - '0';
            else if (ch >= 'a' && ch <= 'f') digitValue = ch - 'a' + 10u;
            else if (ch >= 'A' && ch <= 'F') digitValue = ch - 'A' + 10u;
            else break;
            low = (low << 4) + digitValue;
            ++digitsRead;
            ++end;
        }
        if (digitsRead == 0 || (end < bytes.size() && !asciiSpace(static_cast<unsigned char>(bytes[end]))))
            throw Error(M2M_E_UNSUPPORTED, "SPRING2 hexadecimal suffix remains outside complete-token profile");
        cursor = end;
        commaDelimiterPending = false;
        return std::bit_cast<std::int32_t>(low);
    }
    const bool exactOctal018 = (allowSingleRowQualifiedToken || allowExactTwoRowOctalContinuation) &&
        bytes.compare(start, 3, "018") == 0 &&
        (start + 3 == bytes.size() || asciiSpace(static_cast<unsigned char>(bytes[start + 3])));
    if (exactOctal018) {
        // Base zero selects octal from the second digit 1. The following 8 is
        // not an octal digit, so Qt leaves it unread after successful value 1.
        // One-row admission returns here; the separate exact two-row 018 file
        // profile allows its next extraction to parse the unread 8 as decimal.
        cursor = start + 2;
        commaDelimiterPending = false;
        return 1;
    }
    const bool exactOctal019 = allowSingleRowQualifiedToken && bytes == "019\r\n";
    if (exactOctal019) {
        // The octal prefix 01 succeeds; 9 is the first unread invalid octal
        // digit. Keep this exact one-row cursor witness bounded.
        cursor = start + 2;
        commaDelimiterPending = false;
        return 1;
    }
    const bool oneRowCompleteOctal = allowSingleRowQualifiedToken && bytes.size() >= 2 &&
        bytes[0] == '0' && bytes[1] >= '0' && bytes[1] <= '7';
    if (oneRowCompleteOctal) {
        std::size_t end = 1;
        std::uint32_t octal = 0;
        while (end < bytes.size() && bytes[end] >= '0' && bytes[end] <= '7') {
            const auto digit = static_cast<std::uint32_t>(bytes[end] - '0');
            if (octal > (0x7fffffffU - digit) / 8u)
                throw Error(M2M_E_UNSUPPORTED, "SPRING2 octal value outside bounded int32 profile");
            octal = octal * 8u + digit;
            ++end;
        }
        if (end < bytes.size() && !asciiSpace(static_cast<unsigned char>(bytes[end])))
            throw Error(M2M_E_UNSUPPORTED, "SPRING2 octal suffix outside bounded complete-token profile");
        cursor = end;
        commaDelimiterPending = false;
        return static_cast<int>(octal);
    }
    const bool exactDecimal09 = (allowSingleRowQualifiedToken || allowExactTwoRowDecimal09EofProfile) &&
        bytes.compare(start, 2, "09") == 0 &&
        (start + 2 == bytes.size() || asciiSpace(static_cast<unsigned char>(bytes[start + 2])));
    if (exactDecimal09) {
        // Qt selects decimal when the second digit after leading zero is 8/9.
        cursor = start + 2;
        commaDelimiterPending = false;
        return 9;
    }
    const bool oneRowLeadingZeroDecimalFallback = allowSingleRowQualifiedToken &&
        start + 1 < bytes.size() && bytes[start] == '0' &&
        (bytes[start + 1] == '8' || bytes[start + 1] == '9');
    if (oneRowLeadingZeroDecimalFallback) {
        std::size_t end = start;
        std::uint32_t low = 0;
        while (end < bytes.size() && bytes[end] >= '0' && bytes[end] <= '9') {
            low = low * 10u + static_cast<std::uint32_t>(bytes[end] - '0');
            ++end;
        }
        if (end < bytes.size() && !asciiSpace(static_cast<unsigned char>(bytes[end])))
            throw Error(M2M_E_UNSUPPORTED, "SPRING2 leading-zero decimal suffix remains unsupported");
        cursor = end;
        commaDelimiterPending = false;
        return std::bit_cast<std::int32_t>(low);
    }
    const bool exactBinary0B10 = allowSingleRowQualifiedToken &&
        bytes.compare(start, 4, "0B10") == 0 &&
        (start + 4 == bytes.size() || asciiSpace(static_cast<unsigned char>(bytes[start + 4])));
    if (exactBinary0B10) {
        // Qt case-folds the base prefix, consumes binary digits 10, then
        // leaves the following delimiter unread. Keep the witness count-one.
        cursor = start + 4;
        commaDelimiterPending = false;
        return 2;
    }
    const bool exactInvalidBinary0B2 = (allowSingleRowQualifiedToken || allowExactTwoRowBinaryRetry) &&
        bytes.compare(start, 3, "0B2") == 0 &&
        (start + 3 == bytes.size() || asciiSpace(static_cast<unsigned char>(bytes[start + 3])));
    if (exactInvalidBinary0B2) {
        // Qt rolls back the zero-plus-binary-prefix when the first binary
        // digit is invalid. The int operator stores zero. One- and exact
        // two-row profiles model the caller's bounded extraction count.
        cursor = start;
        commaDelimiterPending = false;
        return 0;
    }
    const bool exactPartialBinary0b12 = (allowSingleRowQualifiedToken || allowExactTwoRowBinaryContinuation) &&
        bytes.compare(start, 4, "0b12") == 0 &&
        (start + 4 == bytes.size() || asciiSpace(static_cast<unsigned char>(bytes[start + 4])));
    if (exactPartialBinary0b12) {
        // Qt accepts binary digit 1, then ungets the following invalid digit
        // 2 and returns the accumulated value. A separately gated two-row
        // profile lets the next extraction parse that unread 2 as decimal.
        cursor = start + 3;
        commaDelimiterPending = false;
        return 1;
    }
    const bool oneRowCompleteBinaryPrefix = allowSingleRowQualifiedToken &&
        start + 2 < bytes.size() && bytes[start] == '0' &&
        (bytes[start + 1] == 'b' || bytes[start + 1] == 'B');
    if (oneRowCompleteBinaryPrefix) {
        std::size_t end = start + 2;
        std::uint32_t low = 0;
        std::size_t digitsRead = 0;
        while (end < bytes.size() && (bytes[end] == '0' || bytes[end] == '1')) {
            low = (low << 1) + static_cast<std::uint32_t>(bytes[end] - '0');
            ++digitsRead;
            ++end;
        }
        if (digitsRead == 0 || (end < bytes.size() && !asciiSpace(static_cast<unsigned char>(bytes[end]))))
            throw Error(M2M_E_UNSUPPORTED, "SPRING2 binary suffix remains outside complete-token profile");
        cursor = end;
        commaDelimiterPending = false;
        return std::bit_cast<std::int32_t>(low);
    }

    std::size_t digit = (bytes[cursor] == '+' || bytes[cursor] == '-') ? cursor + 1 : cursor;
    const auto digits = digit;
    while (digit < bytes.size() && bytes[digit] >= '0' && bytes[digit] <= '9') ++digit;
    if (digit == digits) {
        // A recognized C-locale sign is consumed even when no decimal digit
        // follows. QTextStream then ungets the next nondigit (except the
        // separately handled C-locale grouping separator) and stores zero.
        // Keep this cursor transition within the admitted one/two-row slice.
        if (allowQQuestionCursor && digits == start + 1 &&
            (bytes[start] == '+' || bytes[start] == '-') &&
            (digit == bytes.size() ||
             (bytes[digit] != ',' && (bytes[digit] < '0' || bytes[digit] > '9')))) {
            cursor = start + 1;
            commaDelimiterPending = false;
            return 0;
        }
        if (allowQQuestionCursor && (bytes[cursor] == 'q' || bytes[cursor] == '?')) {
            // Qt ungets a first nonnumeric character and writes zero on each
            // failed extraction. The bounded SPRING2 loop retries it without
            // advancing while atEnd() remains false.
            commaDelimiterPending = false;
            return 0;
        }
        // Qt's getter ungets this first invalid character and reports failure;
        // operator>>(int&) writes zero. The companion loop ignores status and
        // retries while atEnd() remains false, so the bounded x witness yields
        // zero for every remaining requested slot without moving the cursor.
        if ((bytes[cursor] == 'x' || (allowExactTwoRowUpperXRetry && bytes[cursor] == 'X')) &&
            (cursor + 1 == bytes.size() || asciiSpace(static_cast<unsigned char>(bytes[cursor + 1]))) )
            return 0;
        if (bytes[cursor] == ',' && commaDelimiterPending && cursor + 1 < bytes.size() &&
            bytes[cursor + 1] >= '0' && bytes[cursor + 1] <= '9')
            return 0;
        throw Error(M2M_E_UNSUPPORTED, "SPRING2 integer outside bounded malformed-prefix profile");
    }

    const auto prefix = std::string_view(bytes).substr(start, digit - start);
    const auto digitStart = (prefix.front() == '+' || prefix.front() == '-') ? 1u : 0u;
    const bool completeSingleRowSignedLeadingZero = allowSingleRowQualifiedToken &&
        digitStart == 1 && prefix.size() - digitStart > 1 && prefix[digitStart] == '0' &&
        (digit == bytes.size() || asciiSpace(static_cast<unsigned char>(bytes[digit])));
    if (prefix[digitStart] == '0' && prefix.size() - digitStart != 1 &&
        !completeSingleRowSignedLeadingZero)
        throw Error(M2M_E_UNSUPPORTED, "SPRING2 leading-zero/radix integer remains unsupported");
    auto numeric = prefix;
    if (numeric.front() == '+') numeric.remove_prefix(1);
    int value = 0;
    const auto converted = std::from_chars(numeric.data(), numeric.data() + numeric.size(), value, 10);
    if (converted.ec != std::errc{} || converted.ptr != numeric.data() + numeric.size()) {
        const bool exactNegativeNarrowing = prefix == "-2147483649" &&
            (digit == bytes.size() || asciiSpace(static_cast<unsigned char>(bytes[digit])));
        if (exactNegativeNarrowing) {
            // The retained QTextStream getter completes this 64-bit negative
            // value successfully; operator>>(int&) stores its low dword.
            value = 2147483647;
        } else if (allowSingleRowQualifiedToken && prefix.size() > 1 &&
                   prefix.front() == '-' && prefix[1] >= '1' && prefix[1] <= '9' &&
                   (digit == bytes.size() || asciiSpace(static_cast<unsigned char>(bytes[digit])))) {
            // Qt accumulates decimal digits modulo 2^64. For a negative sign,
            // it negates only a nonzero signed-positive accumulator; int& then
            // stores the low dword. Model the sign test without signed overflow.
            std::uint64_t magnitude = 0;
            for (std::size_t i = 1; i < prefix.size(); ++i) {
                const auto next = static_cast<std::uint64_t>(prefix[i] - '0');
                magnitude = magnitude * 10u + next;
            }
            if (magnitude != 0 && (magnitude & 0x8000000000000000ULL) == 0)
                magnitude = 0u - magnitude;
            auto low = static_cast<std::uint32_t>(magnitude);
            value = std::bit_cast<std::int32_t>(low);
        } else if (allowSingleRowQualifiedToken && prefix.size() > 1 &&
                   prefix.front() == '+' && prefix[1] >= '1' && prefix[1] <= '9' &&
                   (digit == bytes.size() || asciiSpace(static_cast<unsigned char>(bytes[digit]))) ) {
            // A recognized positive sign selects decimal. Qt's 64-bit digit
            // accumulator wraps without rejection and int& stores its low
            // dword; direct uint32 accumulation preserves that observable
            // result. Keep leading zeroes and attached suffixes guarded.
            std::uint32_t low = 0;
            for (std::size_t i = 1; i < prefix.size(); ++i) {
                low = low * 10u + static_cast<std::uint32_t>(prefix[i] - '0');
            }
            value = std::bit_cast<std::int32_t>(low);
        } else {
            // For this bounded unsigned decimal class, Qt's getter accumulates
            // modulo 2^64 and the int operator stores the low dword. Direct
            // uint32 accumulation gives the same result; signs, leading zeroes,
            // and non-whitespace delimiters remain on their guarded branches.
            if (prefix.front() < '1' || prefix.front() > '9' ||
                (digit < bytes.size() && !asciiSpace(static_cast<unsigned char>(bytes[digit]))))
                throw Error(M2M_E_UNSUPPORTED, "SPRING2 integer outside bounded int32 profile");
            std::uint32_t low = 0;
            for (char ch : prefix)
                low = low * 10u + static_cast<std::uint32_t>(ch - '0');
            value = std::bit_cast<std::int32_t>(low);
        }
    }

    if (digit < bytes.size() && !asciiSpace(static_cast<unsigned char>(bytes[digit]))) {
        if ((bytes[digit] == 'x' || (allowExactTwoRowUpperXRetry && bytes[digit] == 'X')) &&
            (digit + 1 == bytes.size() || asciiSpace(static_cast<unsigned char>(bytes[digit + 1]))) ) {
            // getInteger ungets the first nonnumeric ASCII QChar. The helper may
            // request another flag, which sees this same x (or exact gated X).
            commaDelimiterPending = false;
        } else if (allowQQuestionCursor && (bytes[digit] == 'q' || bytes[digit] == '?')) {
            // The valid decimal prefix is returned and the marker is left at
            // the cursor for a later extraction, if the row count requests it.
            commaDelimiterPending = false;
        } else if (bytes[digit] == ',' && digit + 1 < bytes.size() &&
                   bytes[digit + 1] >= '0' && bytes[digit + 1] <= '9') {
            // C-locale comma is not grouped with default stream options. It is
            // ungot after the valid prefix and remains for each later request.
            commaDelimiterPending = true;
        } else {
            throw Error(M2M_E_UNSUPPORTED, "SPRING2 suffix outside bounded malformed-prefix profile");
        }
        cursor = digit;
    } else {
        commaDelimiterPending = false;
        cursor = digit;
    }
    return value;
}

double readSpringReal(const std::string& bytes, std::size_t& cursor)
{
    const auto token = nextSpringToken(bytes, cursor);
    if (token.empty()) unsupportedSpringNumber();
    std::size_t i = (token.front() == '+' || token.front() == '-') ? 1 : 0;
    std::size_t digits = 0;
    while (i < token.size() && token[i] >= '0' && token[i] <= '9') ++i, ++digits;
    if (i < token.size() && token[i] == '.') {
        ++i;
        while (i < token.size() && token[i] >= '0' && token[i] <= '9') ++i, ++digits;
    }
    if (digits == 0) unsupportedSpringNumber();
    if (i < token.size() && (token[i] == 'e' || token[i] == 'E')) {
        ++i;
        if (i < token.size() && (token[i] == '+' || token[i] == '-')) ++i;
        const auto exponentStart = i;
        while (i < token.size() && token[i] >= '0' && token[i] <= '9') ++i;
        if (i == exponentStart) unsupportedSpringNumber();
    }
    if (i != token.size()) unsupportedSpringNumber();
    auto numeric = token;
    if (numeric.front() == '+') numeric.remove_prefix(1);
    double value = 0.0;
    const auto converted = std::from_chars(numeric.data(), numeric.data() + numeric.size(), value,
                                           std::chars_format::general);
    if (converted.ec != std::errc{} || converted.ptr != numeric.data() + numeric.size() ||
        !std::isfinite(value))
        unsupportedSpringNumber();
    return value;
}

class NativeExtra3DIo final : public Extra3DIo {
public:
    NativeExtra3DIo(qt::String directory, unsigned codepage, std::function<void()> warning)
        : Extra3DIo(codepage), directory_(qt::cleanPath(directory)), warning_(std::move(warning)),
          reader_(doe::makeNativeStudyIo())
    {
        if (directory_.isEmpty() || qt::isRelativePath(directory_))
            throw Error(M2M_E_ARG, "editor300 sidecar directory must be absolute");
        validateQFilePath(directory); // Check namespace before cleanPath too.
        validateQFilePath(directory_, 1 + 14); // Slash plus STRAIN_DST.DAT.
        const auto& ordinary = directory_.u16();
        const bool driveAbsolute = ordinary.size() >= 3 &&
            ((ordinary[0] >= u'A' && ordinary[0] <= u'Z') || (ordinary[0] >= u'a' && ordinary[0] <= u'z')) &&
            ordinary[1] == u':' && (ordinary[2] == u'/' || ordinary[2] == u'\\');
        const auto serverEnd = ordinary.find_first_of(u"/\\", 2);
        const bool uncAbsolute = ordinary.size() >= 2 &&
            (ordinary[0] == u'/' || ordinary[0] == u'\\') &&
            (ordinary[1] == u'/' || ordinary[1] == u'\\') && serverEnd != std::u16string::npos &&
            serverEnd > 2 && serverEnd + 1 < ordinary.size();
        if (!driveAbsolute && !uncAbsolute)
            throw Error(M2M_E_ARG, "editor300 sidecar directory must be fully absolute");
    }
    bool readDoubles(const qt::String& path, std::span<double> values) override
    {
        std::fill(values.begin(), values.end(), 0.0);
        auto file = openRead(path);
        if (!file) return false;
        if (values.empty()) return true; // original count0 opens but does not read
        const bool oneValueBWeight = values.size() == 1 && path.u16() == u"B_WEIGHT.DAT";
        const auto bytes = asciiBytes(*file, oneValueBWeight);
        std::size_t cursor = 0;
        for (double& value : values) {
            if (cursor == bytes.size()) break;
            value = readReal(bytes, cursor);
        }
        return true;
    }
    bool readInts(const qt::String& path, std::span<int> values) override
    {
        std::fill(values.begin(), values.end(), 0);
        auto file = openRead(path);
        if (!file) return false;
        if (values.empty()) return true;
        const auto bytes = asciiBytes(*file);
        std::size_t cursor = 0;
        const auto& pathName = path.u16();
        const bool oneValueQtIntSidecar = values.size() == 1 &&
            (pathName == u"SW2SP.DAT" || pathName == u"STRETCH.DAT");
        for (int& value : values) {
            if (cursor == bytes.size()) break;
            if (oneValueQtIntSidecar) {
                const auto parsed = detail::parseQt4Base0Int(
                    bytes, cursor, pathName == u"SW2SP.DAT");
                if (!parsed.sourceQualified)
                    throw Error(M2M_E_UNSUPPORTED,
                                "editor300 sidecar integer input outside recovered Qt profile");
                const auto extracted = detail::applyQt4IntOperatorToFreshStream(parsed, bytes.size());
                value = extracted.value; // The original SW2SP reader ignores stream status.
                cursor = parsed.nextByte;
                continue;
            }
            value = readDecimalInt(bytes, cursor);
        }
        return true;
    }
    bool readSprings(std::vector<SpringRecord>& output) override
    {
        output.clear();
        auto file = openRead(qt::L("SPRING.INI"));
        if (!file) return false;
        const auto bytes = asciiBytes(*file);
        // FUN_1410da800 reports success for a nonempty stream even when the
        // first QTextStream integer extraction fails. Qt writes zero to the
        // destination, so this exact invalid-count fixture skips allocation.
        if (bytes == "x\r\n") return false;
        int count = 0;
        if (!bytes.empty()) {
            std::size_t cursor = 0;
            count = readSpringInt(bytes, cursor);
        }
        if (count <= 0) return false;
        // Candidate resource ceiling, not an original-format limit. The native
        // reader's count-driven allocations are retained, but allocator
        // overflow/failure behavior is not source-qualified.
        constexpr int maxSupportedSpringRows = 256;
        if (count > maxSupportedSpringRows)
            throw Error(M2M_E_UNSUPPORTED, "editor300 SPRING count exceeds bounded candidate profile");

        std::vector<int> flags(static_cast<std::size_t>(count), 0);
        if (auto companion = openRead(qt::L("SPRING2.INI"))) {
            // The original default QTextStream detects a Unicode BOM before
            // extracting flags. Keep this explicit to SPRING2 so BOM-less
            // high bytes still use the existing guarded ASCII profile.
            std::string flagBytes;
            bool unicodeDecimalProfile = false;
            const auto rawFlags = companion->readAll();
            const bool explicit16or32 = rawFlags.size() >= 2 &&
                ((static_cast<unsigned char>(rawFlags[0])==0xfe && static_cast<unsigned char>(rawFlags[1])==0xff) ||
                 (static_cast<unsigned char>(rawFlags[0])==0xff && static_cast<unsigned char>(rawFlags[1])==0xfe) ||
                 (rawFlags.size()>=4 && static_cast<unsigned char>(rawFlags[0])==0 && static_cast<unsigned char>(rawFlags[1])==0 && static_cast<unsigned char>(rawFlags[2])==0xfe && static_cast<unsigned char>(rawFlags[3])==0xff));
            (void)rawFlags;
            // The profile is selected only for explicit UTF-16/32 companions
            // containing non-ASCII payload. ASCII BOM behavior stays on the
            // established Qt-compatible bounded routes.
            if (explicit16or32) {
                bool hasNonAscii=false;
                auto decoded = decodeSpringUnicodeDigits(rawFlags, hasNonAscii);
                if (hasNonAscii) {
                    if (count != 1 && count != 2) throw Error(M2M_E_UNSUPPORTED,"SPRING2 Unicode decimal profile requires one/two rows");
                    flagBytes=std::move(decoded);
                    unicodeDecimalProfile=true;
                } else flagBytes=std::move(decoded);
            } else {
                const std::size_t begin = rawFlags.size()>=3 && static_cast<unsigned char>(rawFlags[0])==0xef &&
                    static_cast<unsigned char>(rawFlags[1])==0xbb && static_cast<unsigned char>(rawFlags[2])==0xbf ? 3 : 0;
                if (std::any_of(rawFlags.begin()+static_cast<std::ptrdiff_t>(begin),rawFlags.end(),
                    [](unsigned char c){ return c>=128; }))
                    throw Error(M2M_E_UNSUPPORTED,"editor300 sidecar decoder outside recovered ASCII profile");
                flagBytes.assign(rawFlags.begin()+static_cast<std::ptrdiff_t>(begin),rawFlags.end());
            }
            std::size_t flagCursor = 0;
            bool commaDelimiterPending = false;
            const bool exactTwoRowBinaryRetryProfile = count == 2 && flagBytes == "0B2\r\n";
            const bool exactTwoRowInvalidHexRetryProfile = count == 2 && flagBytes == "0xG\r\n";
            const bool exactTwoRowOctalContinuationProfile = count == 2 && flagBytes == "018\r\n";
            const bool exactTwoRowBinaryContinuationProfile = count == 2 && flagBytes == "0b12\r\n";
            const bool exactTwoRowDecimal09EofProfile = count == 2 && flagBytes == "09\r\n";
            const bool exactTwoRowUpperXRetryProfile = count == 2 && flagBytes == "1X\r\n";
            const bool allowQQuestionCursor = count == 1 || count == 2;
            for (int i = 0; i < count; ++i) {
                while (flagCursor < flagBytes.size() &&
                       asciiSpace(static_cast<unsigned char>(flagBytes[flagCursor]))) ++flagCursor;
                if (flagCursor == flagBytes.size()) break;
                bool tokenHasUnicodeDigit = false;
                for (auto p=flagCursor;p<flagBytes.size() && !asciiSpace(static_cast<unsigned char>(flagBytes[p]));++p)
                    tokenHasUnicodeDigit |= static_cast<unsigned char>(flagBytes[p])>=0x80 &&
                                            static_cast<unsigned char>(flagBytes[p])<=0x89;
                flags[static_cast<std::size_t>(i)] = unicodeDecimalProfile && tokenHasUnicodeDigit
                    ? readUnicodeDecimalFlag(flagBytes, flagCursor)
                    : readSpringFlagInt(flagBytes, flagCursor, commaDelimiterPending,
                                      count == 1 && i == 0, exactTwoRowBinaryRetryProfile,
                                      exactTwoRowInvalidHexRetryProfile,
                                      exactTwoRowOctalContinuationProfile,
                                      exactTwoRowBinaryContinuationProfile,
                                      exactTwoRowDecimal09EofProfile,
                                      exactTwoRowUpperXRetryProfile,
                                      allowQQuestionCursor);
            }
            // The original helper stops at count and ignores any later tokens.
        }

        std::size_t cursor = 0;
        (void)readSpringInt(bytes, cursor); // count
        std::vector<SpringRecord> parsed;
        parsed.reserve(static_cast<std::size_t>(count));
        for (int i = 0; i < count; ++i) {
            SpringRecord row{};
            row.firstObject = readSpringInt(bytes, cursor);
            row.firstValue = readSpringReal(bytes, cursor);
            row.secondValue = readSpringReal(bytes, cursor);
            row.secondObject = readSpringInt(bytes, cursor);
            for (double& component : row.direction) component = readSpringReal(bytes, cursor);
            row.flag = flags[static_cast<std::size_t>(i)];
            parsed.push_back(row);
        }
        // The original count-controlled loop does not extract or validate a
        // suffix after the final declared record. asciiBytes still ensures any
        // ignored suffix remains within the admitted ASCII byte profile.
        output = std::move(parsed);
        return true;
    }
    bool removeIfReadable(const qt::String& path) override
    {
        const auto native = ordinaryNarrowPath(resolve(path));
        std::FILE* file = std::fopen(native.c_str(), "rt");
        if (!file) return true;
        std::fclose(file);
        return std::remove(native.c_str()) == 0;
    }
    void nbcRemoveWarning() override { if (warning_) warning_(); }
    std::unique_ptr<mst::OutputDevice> openWrite(const qt::String& path) override
    {
        return std::make_unique<mst::FileDevice>(resolve(path));
    }
private:
    qt::String resolve(const qt::String& name) const
    {
        return qt::cleanPath(directory_ + qt::kSep + name);
    }
    std::unique_ptr<doe::StudyReadFile> openRead(const qt::String& name)
    {
        const auto path = resolve(name);
        return qt::fileExists(path) ? reader_->openReadFile(path) : nullptr;
    }
    std::string asciiBytes(doe::StudyReadFile& file, bool allowUnicodeBom = false)
    {
        auto bytes = file.readAll();
        const auto decodeAsciiUnits = [&bytes](std::size_t offset, std::size_t width,
                                                bool bigEndian) {
            if ((bytes.size() - offset) % width != 0)
                throw Error(M2M_E_UNSUPPORTED,
                            "editor300 Unicode BOM sidecar has an incomplete code unit");
            std::string decoded;
            decoded.reserve((bytes.size() - offset) / width);
            for (auto position = offset; position < bytes.size(); position += width) {
                std::uint32_t codePoint = 0;
                for (std::size_t i = 0; i < width; ++i) {
                    const auto byte = static_cast<unsigned char>(bytes[position + i]);
                    const auto shiftIndex = bigEndian ? width - i - 1 : i;
                    codePoint |= static_cast<std::uint32_t>(byte) << (8 * shiftIndex);
                }
                if (codePoint > 0x7f)
                    throw Error(M2M_E_UNSUPPORTED,
                                "editor300 Unicode BOM sidecar outside ASCII payload profile");
                decoded.push_back(static_cast<char>(codePoint));
            }
            return decoded;
        };
        const auto byteAt = [&bytes](std::size_t index) {
            return static_cast<unsigned char>(bytes[index]);
        };
        if (allowUnicodeBom && bytes.size() >= 4 &&
            byteAt(0) == 0x00 && byteAt(1) == 0x00 &&
            byteAt(2) == 0xfe && byteAt(3) == 0xff) {
            bytes = decodeAsciiUnits(4, 4, true); // UTF-32BE, MIB 0x3FA.
        } else if (allowUnicodeBom && bytes.size() >= 4 &&
                   byteAt(0) == 0xff && byteAt(1) == 0xfe &&
                   byteAt(2) == 0x00 && byteAt(3) == 0x00) {
            bytes = decodeAsciiUnits(4, 4, false); // UTF-32LE, MIB 0x3FB.
        } else if (allowUnicodeBom && bytes.size() >= 2 &&
                   byteAt(0) == 0xfe && byteAt(1) == 0xff) {
            bytes = decodeAsciiUnits(2, 2, true); // UTF-16BE, MIB 0x3F5.
        } else if (allowUnicodeBom && bytes.size() >= 2 &&
                   byteAt(0) == 0xff && byteAt(1) == 0xfe) {
            bytes = decodeAsciiUnits(2, 2, false); // UTF-16LE, MIB 0x3F6.
        } else if (allowUnicodeBom && bytes.size() >= 3 &&
                   byteAt(0) == 0xef && byteAt(1) == 0xbb && byteAt(2) == 0xbf) {
            bytes.erase(bytes.begin(), bytes.begin() + 3); // UTF-8, MIB 0x6A.
        }
        if (std::any_of(bytes.begin(), bytes.end(), [](unsigned char c) { return c >= 128; }))
            throw Error(M2M_E_UNSUPPORTED, "editor300 sidecar decoder outside recovered ASCII profile");
        return bytes;
    }
    qt::String directory_;
    std::function<void()> warning_;
    std::shared_ptr<doe::StudyIo> reader_;
};
} // namespace

std::unique_ptr<Extra3DIo> makeNativeExtra3DIo(const qt::String& directory,
    unsigned codepage, std::function<void()> warning)
{
    return std::make_unique<NativeExtra3DIo>(directory, codepage, std::move(warning));
}
} // namespace m2m::engine::simcontrol300
