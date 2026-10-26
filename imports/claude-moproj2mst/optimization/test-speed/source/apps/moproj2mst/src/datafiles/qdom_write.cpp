// Qt 4.8 QDomDocument serialisation (owner MDT L6), m2m/datafiles qdom_write.hpp.
#include "qdom_write.hpp"

#include <defrt/stdlib/legacy_printf.hpp>

#include "m2m/common.hpp"

namespace m2m::datafiles {
namespace {

// Attributes are written in the iteration order of QDomNamedNodeMap's QHash<QString, ...>: the
// buckets in order (17 buckets up to 17 attributes: QHashData::rehash(MinNumBits = 4),
// primeForNumBits(4) = 17), each bucket in insertion order; qHash(QString) of Qt 4.8.
std::uint32_t qHash(const std::u16string& s)
{
    std::uint32_t h = 0;
    for (char16_t c : s) {
        h = (h << 4) + c;
        const std::uint32_t g = h & 0xf0000000u;
        if (g) h ^= g >> 23;
        h &= ~g;
    }
    return h;
}

// QDomElementPrivate::save -> encodeText(value, s, encodeQuotes = true, performAVN = true)
std::u16string encodeAttr(const std::u16string& v)
{
    std::u16string r;
    for (std::size_t i = 0; i < v.size(); ++i) {
        const char16_t c = v[i];
        if (c == u'<') r += u"&lt;";
        else if (c == u'"') r += u"&quot;";
        else if (c == u'&') r += u"&amp;";
        else if (c == u'>' && r.size() >= 2 && r[r.size() - 1] == u']' && r[r.size() - 2] == u']') r += u"&gt;";
        else if (c == u'\n') r += u"&#xa;";
        else if (c == u'\r') r += u"&#xd;";
        else if (c == u'\t') r += u"&#x9;";
        else r += c;
    }
    return r;
}

std::vector<const std::pair<std::u16string, std::u16string>*> hashOrder(const DomElement& e)
{
    if (e.attrs.size() > 17)
        throw Error(M2M_E_UNSUPPORTED, "XML element with more than 17 attributes (QHash rehash order)");
    std::vector<const std::pair<std::u16string, std::u16string>*> out;
    for (std::uint32_t b = 0; b < 17; ++b)
        for (const auto& a : e.attrs)
            if (qHash(a.first) % 17 == b) out.push_back(&a);
    return out;
}

void saveElement(const DomElement& e, int depth, int indent, std::u16string& s)
{
    s += std::u16string(static_cast<std::size_t>(indent < 1 ? 0 : depth * indent), u' ');
    s += u'<';
    s += e.name;
    for (const auto* a : hashOrder(e)) {
        s += u' ';
        s += a->first;
        s += u"=\"";
        s += encodeAttr(a->second);
        s += u'"';
    }
    if (!e.children.empty()) {
        s += u'>';
        if (indent != -1) s += u'\n';
        for (const DomElement& c : e.children) saveElement(c, depth + 1, indent, s);
        s += std::u16string(static_cast<std::size_t>(indent < 1 ? 0 : depth * indent), u' ');
        s += u"</";
        s += e.name;
        s += u'>';
    } else {
        s += u"/>";
    }
    if (indent != -1) s += u'\n';
}

}  // namespace

void DomElement::set(const std::u16string& n, const std::u16string& v)
{
    for (auto& a : attrs)
        if (a.first == n) {
            a.second = v;
            return;
        }
    attrs.emplace_back(n, v);
}

void DomElement::setInt(const char16_t* n, int v)
{
    set(n, qt::number(v).u16());
}

// setAttribute(QString, double): qsnprintf("%.16g") (QT_VSNPRINTF = the MSVC CRT)
void DomElement::setDouble(const char16_t* n, double v)
{
    char buf[256];
    const int count = def::rt::crt::snprintf(buf, sizeof buf, "%.16g", v);
    set(n, count > 0 ? qt::String::latin1(std::string(buf, static_cast<std::size_t>(count))).u16()
                     : qt::number(v).u16());
}

void DomElement::setString(const char16_t* n, const qt::String& v)
{
    set(n, v.u16());
}

std::string documentUtf8(const std::u16string& doctype, const DomElement& root)
{
    std::u16string s = u"<!DOCTYPE " + doctype + u">\n";
    saveElement(root, 0, 1, s);
    return qt::String(s).utf8();
}

std::string documentBytes(const std::u16string& doctype, const DomElement& root)
{
    const std::string u8 = documentUtf8(doctype, root);
    std::string out;
    for (char c : u8) {
        if (c == '\n') out += '\r';
        out += c;
    }
    return out;
}

}  // namespace m2m::datafiles
