// Qt 4.8.4 number text: QLocalePrivate::doubleToString / longLongToString in the C locale,
// QTextStream << int / << double with the stream defaults, QString::number, QString::arg and
// QString::toInt / toDouble (owner: P04).
//
// Addresses are VAs in the copy of QtCore4_64.dll 4.8.4 (image base 0x67000000), checked by static
// reading. QTextStream's defaults are never changed by DEF_GUI_MO (s1-02 1.3): SmartNotation,
// precision 6, field width 0, number flags 0, locale QLocale::c().
#include <charconv>
#include <climits>
#include <limits>
#include <cstdint>
#include <cstring>
#include <string>

#include "m2m/common.hpp"
#include "m2m/qt4num.hpp"

namespace m2m::qt {
namespace {

// C locale characters
constexpr char kZero = '0', kPlus = '+', kMinus = '-', kExp = 'e', kGroup = ',', kDecimal = '.';

// QLocalePrivate::PrecisionMode
enum PrecisionMode { PMDecimalDigits = 1, PMSignificantDigits = 2, PMChopTrailingZeros = 3 };

// @src 0x670547a0 decimalForm(zero, decimal, group, digits, decpt, precision, pm,
//                             always_show_decpt, thousands_group)
std::string& decimalForm(std::string& digits, int decpt, unsigned precision, int pm,
                         bool always_show_decpt, bool thousands_group)
{
    if (decpt < 0) {
        for (int i = 0; i < -decpt; ++i) digits.insert(digits.begin(), kZero);
        decpt = 0;
    } else if (static_cast<int>(digits.size()) < decpt) {
        digits.append(static_cast<size_t>(decpt) - digits.size(), kZero);
    }
    if (pm == PMDecimalDigits) {
        unsigned decimal_digits = static_cast<unsigned>(static_cast<int>(digits.size()) - decpt);
        for (unsigned i = decimal_digits; i < precision; ++i) digits += kZero;
    } else if (pm == PMSignificantDigits) {
        for (unsigned i = static_cast<unsigned>(digits.size()); i < precision; ++i) digits += kZero;
    }
    if (always_show_decpt || decpt < static_cast<int>(digits.size()))
        digits.insert(static_cast<size_t>(decpt), 1, kDecimal);
    if (thousands_group)
        for (int i = decpt - 3; i > 0; i -= 3) digits.insert(static_cast<size_t>(i), 1, kGroup);
    if (decpt == 0) digits.insert(digits.begin(), kZero);
    return digits;
}

// @src 0x670548f0 exponentForm(zero, decimal, exponential, group, plus, minus, digits, decpt,
//                              precision, pm, always_show_decpt)
std::string& exponentForm(std::string& digits, int decpt, unsigned precision, int pm,
                          bool always_show_decpt)
{
    int exp = decpt - 1;
    if (pm == PMDecimalDigits) {
        for (unsigned i = static_cast<unsigned>(digits.size()); i < precision + 1; ++i) digits += kZero;
    } else if (pm == PMSignificantDigits) {
        for (unsigned i = static_cast<unsigned>(digits.size()); i < precision; ++i) digits += kZero;
    }
    if (always_show_decpt || digits.size() > 1) digits.insert(1, 1, kDecimal);
    digits += kExp;
    digits += longLongToString(exp, 2, 10, -1, kAlwaysShowSign);
    return digits;
}

std::string ulltoa(unsigned long long v, int base)
{
    // qulltoa: digits '0'-'9' then 'a'-'z'
    std::string s;
    do {
        int c = static_cast<int>(v % static_cast<unsigned>(base));
        s.insert(s.begin(), static_cast<char>(c < 10 ? '0' + c : 'a' + c - 10));
        v /= static_cast<unsigned>(base);
    } while (v);
    return s;
}

bool isUpperAscii(char c) { return c >= 'A' && c <= 'Z'; }
char toLowerAscii(char c) { return isUpperAscii(c) ? static_cast<char>(c - 'A' + 'a') : c; }

void upperAscii(std::string& s)
{
    for (auto& c : s)
        if (c >= 'a' && c <= 'z') c = static_cast<char>(c - 'a' + 'A');
}

// QChar::digitValue for the characters the formats use.
int digitValue(char16_t c) { return (c >= u'0' && c <= u'9') ? c - u'0' : -1; }

// ---- QString::arg helpers (qstring.cpp findArgEscapes / replaceArgEscapes) ----
struct ArgEscapeData {
    int min_escape = INT_MAX;
    int occurrences = 0;
    int locale_occurrences = 0;
    int escape_len = 0;
};

ArgEscapeData findArgEscapes(const std::u16string& s)
{
    ArgEscapeData d;
    size_t c = 0;
    const size_t end = s.size();
    while (c != end) {
        while (c != end && s[c] != u'%') ++c;
        if (c == end) break;
        const size_t escape_start = c;
        if (++c == end) break;
        bool locale_arg = false;
        if (s[c] == u'L') {
            locale_arg = true;
            if (++c == end) break;
        }
        if (digitValue(s[c]) == -1) continue;
        int escape = digitValue(s[c]);
        ++c;
        if (c != end && digitValue(s[c]) != -1) {
            escape = 10 * escape + digitValue(s[c]);
            ++c;
        }
        if (escape > d.min_escape) continue;
        if (escape < d.min_escape) {
            d.min_escape = escape;
            d.occurrences = 0;
            d.escape_len = 0;
            d.locale_occurrences = 0;
        }
        ++d.occurrences;
        if (locale_arg) ++d.locale_occurrences;
        d.escape_len += static_cast<int>(c - escape_start);
    }
    return d;
}

String replaceArgEscapes(const std::u16string& s, const ArgEscapeData& d, int field_width,
                         const std::u16string& arg, const std::u16string& larg, char16_t fill)
{
    const int abs_field_width = field_width < 0 ? -field_width : field_width;
    std::u16string r;
    size_t c = 0;
    const size_t end = s.size();
    int repl_cnt = 0;
    while (c != end) {
        const size_t text_start = c;
        while (s[c] != u'%') ++c;
        const size_t escape_start = c++;
        bool locale_arg = false;
        if (s[c] == u'L') {
            locale_arg = true;
            ++c;
        }
        int escape = digitValue(s[c]);
        if (escape != -1) {
            if (c + 1 != end && digitValue(s[c + 1]) != -1) {
                escape = 10 * escape + digitValue(s[c + 1]);
                ++c;
            }
        }
        if (escape != d.min_escape) {
            r.append(s, text_start, c - text_start);
        } else {
            ++c;
            r.append(s, text_start, escape_start - text_start);
            const std::u16string& a = locale_arg ? larg : arg;
            const int alen = static_cast<int>(a.size());
            const unsigned pad = static_cast<unsigned>((abs_field_width > alen ? abs_field_width : alen) - alen);
            if (field_width > 0) r.append(pad, fill);
            r += a;
            if (field_width < 0) r.append(pad, fill);
            if (++repl_cnt == d.occurrences) {
                r.append(s, c, end - c);
                c = end;
            }
        }
    }
    return String(std::move(r));
}

std::u16string widen(const std::string& s)
{
    return String::latin1(s).u16();
}

// QLocalePrivate::numberToCLocale (C locale, FailOnGroupSeparators): skip leading and trailing
// white space, map the locale characters to their C form and letters to lower case; any other
// character ends the number and must be followed only by white space.
bool isSpace(char16_t c)
{
    // QChar::isSpace (Qt 4.8, Unicode 5.0 tables): 9-13, 0x20, 0x85, 0xa0 and the Zs/Zl/Zp
    // characters.
    if (c >= 9 && c <= 13) return true;
    if (c < 128) return c == 32;
    switch (c) {
    case 0x85: case 0xa0: case 0x1680: case 0x180e: case 0x2028: case 0x2029: case 0x202f:
    case 0x205f: case 0x3000:
        return true;
    default:
        return c >= 0x2000 && c <= 0x200a;
    }
}

bool numberToCLocale(const String& num, std::string* out)
{
    const std::u16string& u = num.u16();
    size_t idx = 0;
    const size_t l = u.size();
    while (idx < l && isSpace(u[idx])) ++idx;
    if (idx == l) return false;
    while (idx < l) {
        const char16_t in = u[idx];
        char o;
        if (in >= u'0' && in <= u'9') o = static_cast<char>(in);
        else if (in == u'+' || in == u'-' || in == u'.' || in == u',') o = static_cast<char>(in);
        else if (in == u'e' || in == u'E') o = 'e';
        else if (in == u';') o = ';';
        else if (in == u'%') o = '%';
        else if (in >= u'A' && in <= u'Z') o = static_cast<char>(in - u'A' + u'a');
        else if (in >= u'a' && in <= u'z') o = static_cast<char>(in);
        else break;
        *out += o;
        ++idx;
    }
    for (; idx < l; ++idx)
        if (!isSpace(u[idx])) return false;
    return true;
}

// qstrtoll (BSD strtoll) on the C-locale buffer, with QLocalePrivate::bytearrayToLongLong's
// "whole string" rule.
long long bytearrayToLongLong(const std::string& num, int base, bool* ok)
{
    *ok = false;
    if (num.empty()) return 0;
    size_t i = 0;
    while (i < num.size() && (num[i] == ' ' || (num[i] >= 9 && num[i] <= 13))) ++i;
    bool neg = false;
    if (i < num.size() && (num[i] == '-' || num[i] == '+')) neg = num[i++] == '-';
    if ((base == 0 || base == 16) && i + 1 < num.size() && num[i] == '0' && (num[i + 1] == 'x' || num[i + 1] == 'X')) {
        i += 2;
        base = 16;
    }
    if (base == 0) base = (i < num.size() && num[i] == '0') ? 8 : 10;
    const unsigned long long cutoff = neg ? 0x8000000000000000ull : 0x7fffffffffffffffull;
    unsigned long long acc = 0;
    bool any = false, overflow = false;
    for (; i < num.size(); ++i) {
        const char ch = num[i];
        int v;
        if (ch >= '0' && ch <= '9') v = ch - '0';
        else if (ch >= 'a' && ch <= 'z') v = ch - 'a' + 10;
        else if (ch >= 'A' && ch <= 'Z') v = ch - 'A' + 10;
        else break;
        if (v >= base) break;
        any = true;
        if (overflow || acc > (cutoff - static_cast<unsigned>(v)) / static_cast<unsigned>(base)) overflow = true;
        else acc = acc * static_cast<unsigned>(base) + static_cast<unsigned>(v);
    }
    if (!any || overflow) return 0;   // strtoll: no conversion, or ERANGE
    if (i != num.size()) return 0;    // stopped at a non-digit
    *ok = true;
    return neg ? static_cast<long long>(0 - acc) : static_cast<long long>(acc);
}

}  // namespace

// QLocalePrivate::unsLongLongToString(zero, group, plus, l, precision, base, width, flags)
std::string unsLongLongToString(unsigned long long l, int precision, int base, int width, unsigned flags)
{
    bool precision_not_specified = false;
    if (precision == -1) {
        precision_not_specified = true;
        precision = 1;
    }
    std::string num_str = ulltoa(l, base);
    if ((flags & kThousandsGroup) && base == 10)
        for (int i = static_cast<int>(num_str.size()) - 3; i > 0; i -= 3)
            num_str.insert(static_cast<size_t>(i), 1, kGroup);
    for (int i = static_cast<int>(num_str.size()); i < precision; ++i) num_str.insert(num_str.begin(), '0');
    const bool zero_padded = (flags & kZeroPadded) && !(flags & kLeftAdjusted) && precision_not_specified;
    if (zero_padded) {
        int num_pad_chars = width - static_cast<int>(num_str.size());
        if (flags & (kAlwaysShowSign | kBlankBeforePositive)) --num_pad_chars;
        for (int i = 0; i < num_pad_chars; ++i) num_str.insert(num_str.begin(), '0');
    }
    if (flags & kCapitalEorX) upperAscii(num_str);
    if (flags & kAlwaysShowSign) num_str.insert(num_str.begin(), kPlus);
    else if (flags & kBlankBeforePositive) num_str.insert(num_str.begin(), ' ');
    return num_str;
}

// QLocalePrivate::longLongToString(zero, group, plus, minus, l, precision, base, width, flags)
std::string longLongToString(long long l, int precision, int base, int width, unsigned flags)
{
    bool precision_not_specified = false;
    if (precision == -1) {
        precision_not_specified = true;
        precision = 1;
    }
    bool negative = l < 0;
    if (base != 10) {
        flags &= ~(kAlwaysShowSign | kBlankBeforePositive);
        negative = false;
    }
    std::string num_str;
    if (base == 10) num_str = ulltoa(l < 0 ? 0ull - static_cast<unsigned long long>(l) : static_cast<unsigned long long>(l), base);
    else num_str = ulltoa(static_cast<unsigned long long>(l), base);
    if ((flags & kThousandsGroup) && base == 10)
        for (int i = static_cast<int>(num_str.size()) - 3; i > 0; i -= 3)
            num_str.insert(static_cast<size_t>(i), 1, kGroup);
    for (int i = static_cast<int>(num_str.size()); i < precision; ++i) num_str.insert(num_str.begin(), kZero);
    const bool zero_padded = (flags & kZeroPadded) && !(flags & kLeftAdjusted) && precision_not_specified;
    if (zero_padded) {
        int num_pad_chars = width - static_cast<int>(num_str.size());
        if (negative || (flags & (kAlwaysShowSign | kBlankBeforePositive))) --num_pad_chars;
        for (int i = 0; i < num_pad_chars; ++i) num_str.insert(num_str.begin(), kZero);
    }
    if (flags & kCapitalEorX) upperAscii(num_str);
    if (negative) num_str.insert(num_str.begin(), kMinus);
    else if (flags & kAlwaysShowSign) num_str.insert(num_str.begin(), kPlus);
    else if (flags & kBlankBeforePositive) num_str.insert(num_str.begin(), ' ');
    return num_str;
}

// @src 0x6704b550 QLocalePrivate::doubleToString
std::string doubleToString(double d, int precision, DoubleForm form, int width, unsigned flags)
{
    if (precision == -1) precision = 6;
    if (width == -1) width = 0;

    std::uint64_t u;
    std::memcpy(&u, &d, sizeof u);
    const unsigned char b7 = static_cast<unsigned char>(u >> 56), b6 = static_cast<unsigned char>(u >> 48);

    std::string num_str;
    bool negative = false;
    bool special_number = false;
    // qt_is_inf / qt_is_nan look at the two top bytes only.
    if ((b7 & 0x7f) == 0x7f && b6 == 0xf0) {
        num_str = "inf";
        special_number = true;
        negative = !(0.0 <= d);   // also true for a NaN whose payload is only in the low bytes
    } else if ((b7 & 0x7f) == 0x7f && b6 > 0xf0) {
        num_str = "nan";
        special_number = true;
    } else {
        const int pr = form == DoubleForm::Exponent ? precision + 1 : precision;
        Digits q = qdtoa(d, form == DoubleForm::Decimal ? 3 : 2, pr);
        std::string& digits = q.digits;
        const bool alt = (flags & kAlternate) != 0;
        switch (form) {
        case DoubleForm::Exponent:
            num_str = exponentForm(digits, q.decpt, static_cast<unsigned>(precision), PMDecimalDigits, alt);
            break;
        case DoubleForm::Decimal:
            num_str = decimalForm(digits, q.decpt, static_cast<unsigned>(precision), PMDecimalDigits, alt,
                                  (flags & kThousandsGroup) != 0);
            break;
        case DoubleForm::SignificantDigits: {
            const int pm = alt ? PMSignificantDigits : PMChopTrailingZeros;
            if (q.decpt == static_cast<int>(digits.size()) || (-4 < q.decpt && q.decpt <= precision))
                num_str = decimalForm(digits, q.decpt, static_cast<unsigned>(precision), pm, alt,
                                      (flags & kThousandsGroup) != 0);
            else
                num_str = exponentForm(digits, q.decpt, static_cast<unsigned>(precision), pm, alt);
            break;
        }
        }
        // isZero: -0.0 prints without a sign
        negative = q.sign && (u & 0x7fffffffffffffffull) != 0;
    }

    if ((flags & kZeroPadded) && !(flags & kLeftAdjusted) && !special_number) {
        int num_pad_chars = width - static_cast<int>(num_str.size());
        if (negative || (flags & (kAlwaysShowSign | kBlankBeforePositive))) --num_pad_chars;
        for (int i = 0; i < num_pad_chars; ++i) num_str.insert(num_str.begin(), kZero);
    }
    if (negative) num_str.insert(num_str.begin(), kMinus);
    else if (flags & kAlwaysShowSign) num_str.insert(num_str.begin(), kPlus);
    else if (flags & kBlankBeforePositive) num_str.insert(num_str.begin(), ' ');
    if (flags & kCapitalEorX) upperAscii(num_str);
    return num_str;
}

// QTextStream::operator<<(int) -> QTextStreamPrivate::putNumber, C locale (no grouping)
std::string formatInt(int v)
{
    return longLongToString(v);
}

// QTextStream::operator<<(double), SmartNotation, precision 6, flags 0
std::string formatDouble(double v)
{
    return doubleToString(v, 6, DoubleForm::SignificantDigits, -1, 0);
}

// QString::number(qlonglong, 10) -> setNum -> C-locale longLongToString
String number(int v)
{
    return String::latin1(longLongToString(v));
}

// QString::number(double, char f, int prec) -> setNum
String number(double v, char format, int precision)
{
    unsigned flags = 0;
    if (isUpperAscii(format)) flags = kCapitalEorX;
    format = toLowerAscii(format);
    DoubleForm form = DoubleForm::Decimal;
    if (format == 'e') form = DoubleForm::Exponent;
    else if (format == 'g') form = DoubleForm::SignificantDigits;
    return String::latin1(doubleToString(v, precision, form, -1, flags));
}

// QString::arg(qlonglong a, int fieldWidth, int base, QChar fillChar)
// %L<n> uses QLocale() (the system locale) in the original; this port formats it in the C
// locale with ',' grouping, which is what an English system gives. No DEF_GUI_MO format seen
// so far uses %L.
String arg(const String& fmt, int a, int fieldWidth, int base, char16_t fill)
{
    const ArgEscapeData d = findArgEscapes(fmt.u16());
    if (d.occurrences == 0) return fmt;   // qWarning("QString::arg: Argument missing")
    unsigned flags = 0;
    if (fill == u'0') flags = kZeroPadded;
    std::u16string a1, a2;
    if (d.occurrences > d.locale_occurrences) a1 = widen(longLongToString(a, -1, base, fieldWidth, flags));
    if (d.locale_occurrences > 0)
        a2 = widen(longLongToString(a, -1, base, fieldWidth, flags | kThousandsGroup));
    return replaceArgEscapes(fmt.u16(), d, fieldWidth, a1, a2, fill);
}

// QString::arg(const QString& a, int fieldWidth, QChar fillChar)
String arg(const String& fmt, const String& a, int fieldWidth, char16_t fill)
{
    const ArgEscapeData d = findArgEscapes(fmt.u16());
    if (d.occurrences == 0) return fmt;
    return replaceArgEscapes(fmt.u16(), d, fieldWidth, a.u16(), a.u16(), fill);
}

// QString::arg(double a, int fieldWidth, char fmt, int prec, QChar fillChar)
String arg(const String& fmt, double a, int fieldWidth, char format, int precision, char16_t fill)
{
    const ArgEscapeData d = findArgEscapes(fmt.u16());
    if (d.occurrences == 0) return fmt;
    unsigned flags = 0;
    if (fill == u'0') flags = kZeroPadded;
    if (isUpperAscii(format)) flags |= kCapitalEorX;
    format = toLowerAscii(format);
    DoubleForm form = DoubleForm::Decimal;
    if (format == 'e') form = DoubleForm::Exponent;
    else if (format == 'g') form = DoubleForm::SignificantDigits;
    std::u16string a1, a2;
    if (d.occurrences > d.locale_occurrences) a1 = widen(doubleToString(a, precision, form, fieldWidth, flags));
    if (d.locale_occurrences > 0)
        a2 = widen(doubleToString(a, precision, form, fieldWidth, flags | kThousandsGroup));
    return replaceArgEscapes(fmt.u16(), d, fieldWidth, a1, a2, fill);
}

// QString::toInt(bool*, 10) -> toLongLong (default locale, then C; the same for these inputs),
// then the int range check.
int toInt(const String& s, bool* ok)
{
    std::string buf;
    bool my_ok = false;
    long long v = 0;
    if (numberToCLocale(s, &buf)) v = bytearrayToLongLong(buf, 10, &my_ok);
    if (!my_ok || v < INT_MIN || v > INT_MAX) {
        if (ok) *ok = false;
        return 0;
    }
    if (ok) *ok = true;
    return static_cast<int>(v);
}

// QString::toDouble(bool*) -> QLocalePrivate::stringToDouble (C locale) -> bytearrayToDouble ->
// qstrtod (Gay's strtod, correctly rounded). Over- and underflow fail with 0.
double toDouble(const String& s, bool* ok)
{
    auto fail = [&]() {
        if (ok) *ok = false;
        return 0.0;
    };
    std::string num;
    if (!numberToCLocale(s, &num) || num.empty()) return fail();
    if (ok) *ok = true;
    if (num == "nan") return std::numeric_limits<double>::quiet_NaN();
    if (num == "+inf" || num == "inf") return std::numeric_limits<double>::infinity();
    if (num == "-inf") return -std::numeric_limits<double>::infinity();
    // qstrtod syntax: [+-] digits [. digits] [e [+-] digits]; it stops at anything else.
    size_t i = 0;
    bool neg = false;
    if (num[i] == '+' || num[i] == '-') neg = num[i++] == '-';
    const size_t mant = i;
    size_t nd = 0;
    while (i < num.size() && num[i] >= '0' && num[i] <= '9') ++i, ++nd;
    if (i < num.size() && num[i] == '.') {
        ++i;
        while (i < num.size() && num[i] >= '0' && num[i] <= '9') ++i, ++nd;
    }
    if (nd == 0) return fail();
    size_t stop = i;
    if (i < num.size() && num[i] == 'e') {
        size_t e = i + 1;
        if (e < num.size() && (num[e] == '+' || num[e] == '-')) ++e;
        size_t ed = e;
        while (ed < num.size() && num[ed] >= '0' && num[ed] <= '9') ++ed;
        if (ed > e) stop = ed;   // "1e" or "1e+" stops before the 'e'
    }
    if (stop != num.size()) return fail();
    double v = 0.0;
    const auto res = std::from_chars(num.data() + mant, num.data() + stop, v, std::chars_format::general);
    if (res.ec != std::errc() || res.ptr != num.data() + stop) return fail();
    return neg ? -v : v;
}

}  // namespace m2m::qt
