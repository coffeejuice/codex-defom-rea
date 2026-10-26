// Qt 4.8.4 QString members with rules (owner: P04): split, contains, startsWith, endsWith,
// toAscii, and the locale codec used for MST bytes.
#include <windows.h>

#include "m2m/qt4.hpp"

namespace m2m::qt {
namespace {

// QChar::toCaseFolded for the comparisons. ASCII and Latin-1 are folded as Qt does; other
// characters compare as they are (no DEF_GUI_MO comparison seen so far needs more).
char16_t fold(char16_t c)
{
    if (c >= u'A' && c <= u'Z') return static_cast<char16_t>(c + 32);
    if (c >= 0xc0 && c <= 0xde && c != 0xd7) return static_cast<char16_t>(c + 32);
    if (c == 0xb5) return 0x3bc;
    return c;
}

bool eq(char16_t a, char16_t b, Case cs) { return cs == Case::Sensitive ? a == b : fold(a) == fold(b); }

// qFindString(haystack, from, needle, cs) as QString::indexOf(const QString&, int from, cs)
int indexOf(const std::u16string& h, const std::u16string& n, int from, Case cs)
{
    const int l1 = static_cast<int>(h.size()), sl = static_cast<int>(n.size());
    if (from < 0) from += l1;
    if (static_cast<unsigned>(sl + from) > static_cast<unsigned>(l1)) return -1;
    if (!sl) return from;
    if (!l1) return -1;
    for (int i = from; i + sl <= l1; ++i) {
        int j = 0;
        while (j < sl && eq(h[static_cast<size_t>(i + j)], n[static_cast<size_t>(j)], cs)) ++j;
        if (j == sl) return i;
    }
    return -1;
}

// QString::mid(position, n = -1)
String mid(const String& s, int position, int n = -1)
{
    const int size = s.size();
    if (s.isNull() || position > size) return String();
    if (n < 0) n = size - position;
    if (position < 0) {
        n += position;
        position = 0;
    }
    if (n + position > size) n = size - position;
    if (position == 0 && n == size) return s;
    return String(s.u16().substr(static_cast<size_t>(position), static_cast<size_t>(n < 0 ? 0 : n)));
}

}  // namespace

// QString::split(const QString& sep, SplitBehavior, Qt::CaseSensitive)
std::vector<String> split(const String& s, const String& sep, SplitBehavior b)
{
    std::vector<String> list;
    int start = 0, extra = 0, end;
    while ((end = indexOf(s.u16(), sep.u16(), start + extra, Case::Sensitive)) != -1) {
        if (start != end || b == SplitBehavior::KeepEmptyParts) list.push_back(mid(s, start, end - start));
        start = end + sep.size();
        extra = sep.size() == 0 ? 1 : 0;
    }
    if (start != s.size() || b == SplitBehavior::KeepEmptyParts) list.push_back(mid(s, start));
    return list;
}

bool contains(const String& s, const String& what, Case cs)
{
    return indexOf(s.u16(), what.u16(), 0, cs) != -1;
}

// qt_starts_with: a null haystack starts only with a null needle; an empty one only with an
// empty (or null) needle.
bool startsWith(const String& s, const String& what, Case cs)
{
    if (s.isNull()) return what.isNull();
    if (s.size() == 0) return what.size() == 0;
    if (what.size() > s.size()) return false;
    for (int i = 0; i < what.size(); ++i)
        if (!eq(s.at(i), what.at(i), cs)) return false;
    return true;
}

bool endsWith(const String& s, const String& what, Case cs)
{
    if (s.isNull()) return what.isNull();
    if (s.size() == 0) return what.size() == 0;
    const int pos = s.size() - what.size();
    if (pos < 0) return false;
    for (int i = 0; i < what.size(); ++i)
        if (!eq(s.at(pos + i), what.at(i), cs)) return false;
    return true;
}

// QString::toAscii: Latin-1 (no codecForCStrings for the default language), '?' above 0xff.
std::string toAscii(const String& s)
{
    std::string out;
    out.reserve(static_cast<size_t>(s.size()));
    for (char16_t c : s.u16()) out += c > 0xff ? '?' : static_cast<char>(c);
    return out;
}

// QWindowsLocalCodec::convertFromUnicode -> qt_winQString2MB: WideCharToMultiByte(CP_ACP, 0, ...)
// with the default character; here the code page is the caller's (s1-02 1.2, plan section 3).
std::string encodeLocale(const String& s, unsigned codepage)
{
    if (s.isEmpty()) return std::string();
    const auto* w = reinterpret_cast<const wchar_t*>(s.u16().data());
    const int n = s.size();
    BOOL used_def = FALSE;
    const int len = WideCharToMultiByte(codepage, 0, w, n, nullptr, 0, nullptr,
                                        codepage == CP_UTF8 ? nullptr : &used_def);
    if (len <= 0) return std::string();   // qWarning and an empty result
    std::string out(static_cast<size_t>(len), '\0');
    WideCharToMultiByte(codepage, 0, w, n, out.data(), len, nullptr, codepage == CP_UTF8 ? nullptr : &used_def);
    return out;
}

}  // namespace m2m::qt
