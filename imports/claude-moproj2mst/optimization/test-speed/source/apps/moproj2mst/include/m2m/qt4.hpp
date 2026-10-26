// m2m/qt4.hpp: the Qt 4.8 text behaviour DEF_GUI_MO relies on (seam; functions owned by P04).
//
// String models QString: UTF-16 text plus the null/empty distinction (QString() is null,
// QString("") is empty but not null). The inline members are the parts every packet needs and that
// follow directly from Qt 4.8's qstring.cpp; everything with a formatting or parsing rule is a P04
// function below. Paths are in qt4path.hpp (P05).
#pragma once

#include <string>
#include <string_view>
#include <vector>

namespace m2m::qt {

class String {
public:
    String() = default;                                   // null
    String(std::u16string s) : s_(std::move(s)), null_(false) {}
    String(const char16_t* s) : s_(s ? s : u""), null_(s == nullptr) {}

    // QLatin1String(s) / QString::fromLatin1: every byte is one code unit. nullptr gives null.
    static String latin1(const char* s);
    static String latin1(std::string_view s);

    bool isNull() const { return null_; }
    bool isEmpty() const { return s_.empty(); }
    int size() const { return static_cast<int>(s_.size()); }
    const std::u16string& u16() const { return s_; }
    char16_t at(int i) const { return s_[static_cast<size_t>(i)]; }

    // QString::append (Qt 4.8): appending a null string changes nothing, appending to a null
    // string copies the other one; the result is null only when both are null.
    String& operator+=(const String& o)
    {
        if (!o.null_) {
            if (null_) *this = o;
            else s_ += o.s_;
        }
        return *this;
    }
    String& operator+=(char16_t c)
    {
        s_ += c;
        null_ = false;
        return *this;
    }
    friend String operator+(String a, const String& b) { return a += b; }
    friend String operator+(String a, char16_t c) { return a += c; }

    // QString::operator== compares content only (null == empty).
    friend bool operator==(const String& a, const String& b) { return a.s_ == b.s_; }
    friend bool operator<(const String& a, const String& b) { return a.s_ < b.s_; }

    // Text for messages (UTF-8, not used for MST bytes).
    std::string utf8() const;

private:
    std::u16string s_;
    bool null_ = true;
};

inline String L(const char* s) { return String::latin1(s); }   // QLatin1String literal

// QVariant as the property reader uses it (s1-01 6.3): double, int or string.
struct Variant {
    enum Kind { Invalid, Double, Int, Text, Bool } kind = Invalid;   // Bool (i = 0/1): moproj-ops getters
    double d = 0.0;
    int i = 0;
    String s;
};

enum class Case { Sensitive, Insensitive };
enum class SplitBehavior { KeepEmptyParts, SkipEmptyParts };

// ---- P04: QString members with rules ------------------------------------------------------------
// Qt 4.8 QString::toInt(bool*, base 10): leading/trailing whitespace allowed, 0 on failure.
int toInt(const String& s, bool* ok = nullptr);
// Qt 4.8 QString::toDouble (C locale).
double toDouble(const String& s, bool* ok = nullptr);
// QString::number(int, 10)
String number(int v);
// QString::arg(int a, int fieldWidth, int base, QChar fill): replaces the lowest %n.
String arg(const String& fmt, int a, int fieldWidth = 0, int base = 10, char16_t fill = u' ');
// QString::arg(const QString& a, int fieldWidth, QChar fill)
String arg(const String& fmt, const String& a, int fieldWidth = 0, char16_t fill = u' ');
// QString::arg(double a, int fieldWidth, char fmt, int prec, QChar fillChar) (Qt 4.8: C locale;
// fill '0' sets ZeroPadded, an upper-case fmt sets CapitalEorX, 'f'/'e'/'g' pick the form).
String arg(const String& fmt, double a, int fieldWidth = 0, char format = 'g', int precision = -1,
           char16_t fill = u' ');
// QString::number(double, char format, int precision)
String number(double v, char format = 'g', int precision = 6);
// QString::split(sep, behavior, Qt::CaseSensitive)
std::vector<String> split(const String& s, const String& sep,
                          SplitBehavior b = SplitBehavior::KeepEmptyParts);
bool contains(const String& s, const String& what, Case cs = Case::Sensitive);
bool startsWith(const String& s, const String& what, Case cs = Case::Sensitive);
bool endsWith(const String& s, const String& what, Case cs = Case::Sensitive);
// QString::toAscii() (Latin-1 in Qt 4 unless setCodecForCStrings; see s1-02 1.2)
std::string toAscii(const String& s);

// ---- P04: number text as QTextStream writes it (s1-02 1.3) ---------------------------------------
// operator<<(int): plain decimal.
std::string formatInt(int v);
// operator<<(double) with the stream defaults (SmartNotation, precision 6, C locale):
// QLocalePrivate::doubleToString(v, 6, DFSignificantDigits, -1, flags 0).
std::string formatDouble(double v);

// ---- P04: bytes of a QString written through the locale codec (s1-02 1.2) ------------------------
// ANSI code page `codepage` (WideCharToMultiByte, unmappable -> '?').
std::string encodeLocale(const String& s, unsigned codepage);

}  // namespace m2m::qt
