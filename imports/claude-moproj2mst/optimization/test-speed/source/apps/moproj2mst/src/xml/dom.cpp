// QDomDocument subset for the .moproj reader (packet P01, s1-01 2).
//
// 0x14008afa0 opens the file with QIODevice::ReadOnly | QIODevice::Text and calls
// QDomDocument::setContent(&file, false, &err, &line, &col). What that does in Qt 4.8, as far as
// these files need it:
//   * QIODevice text mode drops every '\r' byte while reading (CRLF -> LF).
//   * QXmlInputSource::fromRawData picks the decoder: UTF-32/UTF-16 by BOM or by the first "<",
//     else UTF-8; an encoding="..." in the XML declaration switches codecs when Qt knows the name.
//   * QDomDocumentPrivate::setContent runs QXmlSimpleReader with namespace processing off and
//     "report-whitespace-only-CharData" off: a run of character data whose trimmed() text is
//     empty creates no text node. CDATA sections always create a node.
//   * Attribute values are taken as written (Qt 4's reader does no attribute-value
//     normalisation); references are expanded. A later duplicate attribute replaces the value.
//   * QDomElement::text() concatenates text and CDATA of all descendants and returns "" (not
//     null) for an element without them.
// Not supported (no corpus file needs them, setContent then fails with "unsupported ..."): entity
// declarations in an internal DTD subset and references to entities other than the five
// predefined ones and character references.
#include <string>

#include "m2m/xml.hpp"

namespace m2m::xml {
namespace {

// QChar::isSpace (Qt 4.8): 9..13, 32, 0x85, 0xa0 and the Unicode separators (Zs, Zl, Zp).
bool isSpace(char16_t c)
{
    if ((c >= 9 && c <= 13) || c == 32 || c == 0x85 || c == 0xa0) return true;
    if (c < 0x1680) return false;
    return c == 0x1680 || c == 0x180e || (c >= 0x2000 && c <= 0x200a) || c == 0x2028 ||
           c == 0x2029 || c == 0x202f || c == 0x205f || c == 0x3000;
}

bool trimmedEmpty(const std::u16string& s)
{
    for (char16_t c : s)
        if (!isSpace(c)) return false;
    return true;
}

// ---- decoding (QXmlInputSource::fromRawData) ----------------------------------------------------

void appendUcs4(std::u16string& out, char32_t u)
{
    if (u >= 0x10000) {
        u -= 0x10000;
        out += static_cast<char16_t>(0xd800 + (u >> 10));
        out += static_cast<char16_t>(0xdc00 + (u & 0x3ff));
    } else {
        out += static_cast<char16_t>(u);
    }
}

// Qt 4.8 QUtf8Codec::convertToUnicode: a leading BOM is dropped; an invalid or overlong sequence
// gives U+FFFD and decoding resumes at the byte after the lead byte.
std::u16string decodeUtf8(const std::string& b, size_t start)
{
    std::u16string out;
    out.reserve(b.size());
    size_t i = start;
    if (b.size() - i >= 3 && static_cast<unsigned char>(b[i]) == 0xef &&
        static_cast<unsigned char>(b[i + 1]) == 0xbb && static_cast<unsigned char>(b[i + 2]) == 0xbf)
        i += 3;
    while (i < b.size()) {
        unsigned char c = static_cast<unsigned char>(b[i]);
        if (c < 0x80) {
            out += c;
            ++i;
            continue;
        }
        int need;
        char32_t u, min;
        if ((c & 0xe0) == 0xc0) { need = 1; u = c & 0x1f; min = 0x80; }
        else if ((c & 0xf0) == 0xe0) { need = 2; u = c & 0x0f; min = 0x800; }
        else if ((c & 0xf8) == 0xf0) { need = 3; u = c & 0x07; min = 0x10000; }
        else { out += u'\xfffd'; ++i; continue; }
        bool ok = i + static_cast<size_t>(need) < b.size();
        for (int k = 1; ok && k <= need; ++k) {
            unsigned char cc = static_cast<unsigned char>(b[i + k]);
            if ((cc & 0xc0) != 0x80) ok = false;
            else u = (u << 6) | (cc & 0x3f);
        }
        if (!ok || u < min || u > 0x10ffff || (u >= 0xd800 && u <= 0xdfff)) {
            out += u'\xfffd';
            ++i;
            continue;
        }
        appendUcs4(out, u);
        i += static_cast<size_t>(need) + 1;
    }
    return out;
}

std::u16string decodeLatin1(const std::string& b)
{
    std::u16string out;
    out.reserve(b.size());
    for (char c : b) out += static_cast<unsigned char>(c);
    return out;
}

std::u16string decodeUtf16(const std::string& b, bool bigEndian, size_t start)
{
    std::u16string out;
    for (size_t i = start; i + 1 < b.size(); i += 2) {
        unsigned a = static_cast<unsigned char>(b[i]), c = static_cast<unsigned char>(b[i + 1]);
        out += static_cast<char16_t>(bigEndian ? (a << 8 | c) : (c << 8 | a));
    }
    return out;
}

std::u16string decodeUtf32(const std::string& b, bool bigEndian, size_t start)
{
    std::u16string out;
    for (size_t i = start; i + 3 < b.size(); i += 4) {
        char32_t u = 0;
        for (int k = 0; k < 4; ++k)
            u |= static_cast<char32_t>(static_cast<unsigned char>(b[i + k])) << (bigEndian ? 8 * (3 - k) : 8 * k);
        if (u > 0x10ffff) u = 0xfffd;
        appendUcs4(out, u);
    }
    return out;
}

std::string lower(std::u16string s)
{
    std::string r;
    for (char16_t c : s) r += static_cast<char>(c >= u'A' && c <= u'Z' ? c + 32 : (c < 0x80 ? c : '?'));
    return r;
}

// QXmlInputSource's extractEncodingDecl: encoding="..." of a leading <?xml ...?>.
std::u16string encodingDecl(const std::u16string& text)
{
    if (text.compare(0, 5, u"<?xml") != 0) return {};
    size_t end = text.find(u"?>");
    std::u16string decl = text.substr(0, end);
    size_t p = decl.find(u"encoding");
    if (p == std::u16string::npos) return {};
    p += 8;
    while (p < decl.size() && isSpace(decl[p])) ++p;
    if (p >= decl.size() || decl[p] != u'=') return {};
    ++p;
    while (p < decl.size() && isSpace(decl[p])) ++p;
    if (p >= decl.size() || (decl[p] != u'"' && decl[p] != u'\'')) return {};
    char16_t q = decl[p++];
    size_t e = decl.find(q, p);
    if (e == std::u16string::npos) return {};
    return decl.substr(p, e - p);
}

std::u16string decode(const std::string& b)
{
    auto u8 = [&](size_t i) { return static_cast<unsigned char>(b[i]); };
    if (b.size() >= 4) {
        if (u8(0) == 0 && u8(1) == 0 && u8(2) == 0xfe && u8(3) == 0xff) return decodeUtf32(b, true, 4);
        if (u8(0) == 0xff && u8(1) == 0xfe && u8(2) == 0 && u8(3) == 0) return decodeUtf32(b, false, 4);
        if (u8(0) == 0x3c && u8(1) == 0 && u8(2) == 0 && u8(3) == 0) return decodeUtf32(b, false, 0);
        if (u8(0) == 0 && u8(1) == 0 && u8(2) == 0 && u8(3) == 0x3c) return decodeUtf32(b, true, 0);
    }
    if (b.size() >= 2) {
        if (u8(0) == 0xfe && u8(1) == 0xff) return decodeUtf16(b, true, 2);
        if (u8(0) == 0xff && u8(1) == 0xfe) return decodeUtf16(b, false, 2);
        if (u8(0) == 0x3c && u8(1) == 0) return decodeUtf16(b, false, 0);
        if (u8(0) == 0 && u8(1) == 0x3c) return decodeUtf16(b, true, 0);
    }
    std::u16string text = decodeUtf8(b, 0);
    const std::string enc = lower(encodingDecl(text));
    // Codec names Qt 4 resolves to Latin-1; any other name keeps UTF-8 (only UTF-8 and these
    // are supported here; an unknown name is also UTF-8 in Qt).
    if (enc == "iso-8859-1" || enc == "latin1" || enc == "latin-1" || enc == "l1" || enc == "iso8859-1" ||
        enc == "iso_8859-1" || enc == "cp819" || enc == "ibm819" || enc == "iso-ir-100" || enc == "csisolatin1")
        return decodeLatin1(b);
    return text;
}

// ---- the parser (QXmlSimpleReader as QDomDocument drives it) ------------------------------------

struct ParseError {
    const char* msg;
    size_t pos;
};

class Parser {
public:
    explicit Parser(const std::u16string& t) : s_(t) {}

    NodePtr parseDocument()
    {
        prolog();
        if (!startsWith(u"<")) fail("unexpected end of file");
        NodePtr root = element();
        misc();
        if (pos_ != s_.size()) fail("unexpected character");
        return root;
    }

    size_t pos() const { return pos_; }

    std::u16string doctypeName_, doctypePublic_, doctypeSystem_;
    bool hasPublic_ = false, hasSystem_ = false, otherTop_ = false;

    void lineCol(size_t at, int& line, int& col) const
    {
        line = 1;
        col = 1;
        for (size_t i = 0; i < at && i < s_.size(); ++i) {
            if (s_[i] == u'\n') { ++line; col = 1; }
            else ++col;
        }
    }

private:
    const std::u16string& s_;
    size_t pos_ = 0;

    [[noreturn]] void fail(const char* msg) { throw ParseError{msg, pos_}; }
    bool eof() const { return pos_ >= s_.size(); }
    char16_t cur() const { return s_[pos_]; }
    bool startsWith(const char16_t* lit) const
    {
        size_t n = std::char_traits<char16_t>::length(lit);
        return s_.compare(pos_, n, lit) == 0;
    }
    void expect(const char16_t* lit, const char* msg)
    {
        if (!startsWith(lit)) fail(msg);
        pos_ += std::char_traits<char16_t>::length(lit);
    }
    bool skipWs()
    {
        size_t p = pos_;
        while (!eof() && (cur() == u' ' || cur() == u'\t' || cur() == u'\n' || cur() == u'\r')) ++pos_;
        return pos_ != p;
    }

    static bool nameStart(char16_t c)
    {
        return (c >= u'a' && c <= u'z') || (c >= u'A' && c <= u'Z') || c == u'_' || c == u':' || c >= 0x80;
    }
    static bool nameChar(char16_t c)
    {
        return nameStart(c) || (c >= u'0' && c <= u'9') || c == u'.' || c == u'-';
    }
    std::u16string name()
    {
        if (eof() || !nameStart(cur())) fail("letter is expected");
        size_t b = pos_;
        while (!eof() && nameChar(cur())) ++pos_;
        return s_.substr(b, pos_ - b);
    }

    // Reference after '&': predefined entity or character reference, appended to out.
    void reference(std::u16string& out)
    {
        ++pos_;   // '&'
        if (!eof() && cur() == u'#') {
            ++pos_;
            int base = 10;
            if (!eof() && cur() == u'x') { base = 16; ++pos_; }
            char32_t v = 0;
            size_t b = pos_;
            while (!eof() && cur() != u';') {
                char16_t c = cur();
                int d;
                if (c >= u'0' && c <= u'9') d = c - u'0';
                else if (base == 16 && c >= u'a' && c <= u'f') d = c - u'a' + 10;
                else if (base == 16 && c >= u'A' && c <= u'F') d = c - u'A' + 10;
                else fail("error occurred while parsing reference");
                v = v * base + d;
                if (v > 0x10ffff) fail("error occurred while parsing reference");
                ++pos_;
            }
            if (eof() || pos_ == b) fail("error occurred while parsing reference");
            ++pos_;   // ';'
            appendUcs4(out, v);
            return;
        }
        std::u16string n = name();
        if (eof() || cur() != u';') fail("error occurred while parsing reference");
        ++pos_;
        if (n == u"amp") out += u'&';
        else if (n == u"lt") out += u'<';
        else if (n == u"gt") out += u'>';
        else if (n == u"quot") out += u'"';
        else if (n == u"apos") out += u'\'';
        else fail("unsupported entity reference");
    }

    void comment(Node* parent)
    {
        pos_ += 4;   // "<!--"
        size_t e = s_.find(u"-->", pos_);
        if (e == std::u16string::npos) { pos_ = s_.size(); fail("error occurred while parsing comment"); }
        if (!parent) otherTop_ = true;
        if (parent) {
            auto n = std::make_shared<Node>();
            n->kind = Node::Comment;
            n->data = String(s_.substr(pos_, e - pos_));
            parent->children.push_back(n);
        }
        pos_ = e + 3;
    }

    void pi(Node* parent)
    {
        pos_ += 2;   // "<?"
        std::u16string target = name();
        size_t e = s_.find(u"?>", pos_);
        if (e == std::u16string::npos) { pos_ = s_.size(); fail("unexpected end of file"); }
        if (!parent) otherTop_ = true;
        if (parent) {
            auto n = std::make_shared<Node>();
            n->kind = Node::ProcessingInstruction;
            n->tag = String(target);
            size_t b = pos_;
            while (b < e && isSpace(s_[b])) ++b;
            n->data = String(s_.substr(b, e - b));
            parent->children.push_back(n);
        }
        pos_ = e + 2;
    }

    void doctype()
    {
        pos_ += 9;   // "<!DOCTYPE"
        if (!skipWs()) fail("error occurred while parsing document type definition");
        doctypeName_ = name();
        skipWs();
        auto literal = [&] {
            skipWs();
            if (eof() || (cur() != u'"' && cur() != u'\'')) fail("error occurred while parsing document type definition");
            char16_t q = cur();
            size_t e = s_.find(q, pos_ + 1);
            if (e == std::u16string::npos) { pos_ = s_.size(); fail("unexpected end of file"); }
            std::u16string v = s_.substr(pos_ + 1, e - pos_ - 1);
            pos_ = e + 1;
            return v;
        };
        if (startsWith(u"SYSTEM")) { pos_ += 6; doctypeSystem_ = literal(); hasSystem_ = true; }
        else if (startsWith(u"PUBLIC")) {
            pos_ += 6;
            doctypePublic_ = literal();
            doctypeSystem_ = literal();
            hasPublic_ = hasSystem_ = true;
        }
        skipWs();
        if (!eof() && cur() == u'[') {
            otherTop_ = true;
            // Internal subset: comments and PIs are skipped; declarations are not supported.
            ++pos_;
            for (;;) {
                skipWs();
                if (eof()) fail("unexpected end of file");
                if (cur() == u']') { ++pos_; break; }
                if (startsWith(u"<!--")) comment(nullptr);
                else if (startsWith(u"<?")) pi(nullptr);
                else fail("unsupported internal DTD subset");
            }
            skipWs();
        }
        expect(u">", "error occurred while parsing document type definition");
    }

    void prolog()
    {
        if (startsWith(u"<?xml") && pos_ + 5 < s_.size() && isSpace(s_[pos_ + 5])) {
            otherTop_ = true;
            size_t e = s_.find(u"?>", pos_);
            if (e == std::u16string::npos) { pos_ = s_.size(); fail("unexpected end of file"); }
            pos_ = e + 2;
        }
        bool sawDoctype = false;
        for (;;) {
            skipWs();
            if (startsWith(u"<!--")) comment(nullptr);
            else if (startsWith(u"<?")) pi(nullptr);
            else if (startsWith(u"<!DOCTYPE")) {
                if (sawDoctype) fail("more than one document type definition");
                sawDoctype = true;
                doctype();
            } else break;
        }
    }

    void misc()
    {
        for (;;) {
            skipWs();
            if (startsWith(u"<!--")) comment(nullptr);
            else if (startsWith(u"<?")) pi(nullptr);
            else break;
        }
    }

    static void setAttr(Node& n, std::u16string an, std::u16string av)
    {
        for (auto& a : n.attrs)
            if (a.name.u16() == an) { a.value = String(std::move(av)); return; }
        n.attrs.push_back(Attr{String(std::move(an)), String(std::move(av))});
    }

    NodePtr element()
    {
        ++pos_;   // '<'
        auto n = std::make_shared<Node>();
        n->kind = Node::Element;
        std::u16string tag = name();
        n->tag = String(tag);
        for (;;) {
            bool ws = skipWs();
            if (eof()) fail("unexpected end of file");
            if (startsWith(u"/>")) { pos_ += 2; return n; }
            if (cur() == u'>') { ++pos_; break; }
            if (!ws) fail("error occurred while parsing element");
            std::u16string an = name();
            skipWs();
            expect(u"=", "error occurred while parsing element");
            skipWs();
            if (eof() || (cur() != u'"' && cur() != u'\'')) fail("error occurred while parsing element");
            char16_t q = cur();
            ++pos_;
            std::u16string av;
            for (;;) {
                if (eof()) fail("unexpected end of file");
                char16_t c = cur();
                if (c == q) { ++pos_; break; }
                if (c == u'<') fail("unexpected character");
                if (c == u'&') reference(av);
                else { av += c; ++pos_; }
            }
            setAttr(*n, std::move(an), std::move(av));
        }
        content(*n);
        // end tag
        pos_ += 2;   // "</"
        std::u16string et = name();
        if (et != tag) fail("tag mismatch");
        skipWs();
        expect(u">", "error occurred while parsing element");
        return n;
    }

    void flushText(Node& n, std::u16string& text)
    {
        if (!text.empty() && !trimmedEmpty(text)) {
            auto t = std::make_shared<Node>();
            t->kind = Node::Text;
            t->data = String(text);
            n.children.push_back(t);
        }
        text.clear();
    }

    void content(Node& n)
    {
        std::u16string text;
        for (;;) {
            if (eof()) fail("unexpected end of file");
            char16_t c = cur();
            if (c == u'<') {
                if (startsWith(u"</")) { flushText(n, text); return; }
                flushText(n, text);
                if (startsWith(u"<!--")) comment(&n);
                else if (startsWith(u"<![CDATA[")) {
                    pos_ += 9;
                    size_t e = s_.find(u"]]>", pos_);
                    if (e == std::u16string::npos) { pos_ = s_.size(); fail("unexpected end of file"); }
                    auto t = std::make_shared<Node>();
                    t->kind = Node::CData;
                    t->data = String(s_.substr(pos_, e - pos_));
                    n.children.push_back(t);
                    pos_ = e + 3;
                } else if (startsWith(u"<?")) pi(&n);
                else if (startsWith(u"<!")) fail("error occurred while parsing content");
                else n.children.push_back(element());
            } else if (c == u'&') {
                reference(text);
            } else {
                text += c;
                ++pos_;
            }
        }
    }
};

void collectText(const Node& n, std::u16string& out)
{
    for (const auto& c : n.children) {
        if (c->kind == Node::Text || c->kind == Node::CData) out += c->data.u16();
        else if (c->kind == Node::Element) collectText(*c, out);
    }
}

}  // namespace

String Node::attribute(const String& name, const String& defValue) const
{
    for (const auto& a : attrs)
        if (a.name == name) return a.value;
    return defValue;
}

bool Node::hasAttribute(const String& name) const
{
    for (const auto& a : attrs)
        if (a.name == name) return true;
    return false;
}

String Node::text() const
{
    std::u16string t;
    collectText(*this, t);
    return String(std::move(t));   // QLatin1String("") + ...: never null
}

std::vector<const Node*> Node::childElements() const
{
    std::vector<const Node*> r;
    for (const auto& c : children)
        if (c->kind == Element) r.push_back(c.get());
    return r;
}

std::vector<const Node*> Node::childElements(const String& name) const
{
    std::vector<const Node*> r;
    for (const auto& c : children)
        if (c->kind == Element && c->tag == name) r.push_back(c.get());
    return r;
}

bool setContent(Document& doc, const std::string& fileBytes, String* err, int* line, int* col)
{
    // QIODevice::Text: every '\r' is dropped while reading.
    std::string bytes;
    bytes.reserve(fileBytes.size());
    for (char c : fileBytes)
        if (c != '\r') bytes += c;
    const std::u16string text = decode(bytes);
    Parser parser(text);
    try {
        doc.root = parser.parseDocument();
        if (!parser.doctypeName_.empty()) doc.doctypeName = String(parser.doctypeName_);
        if (parser.hasPublic_) doc.doctypePublic = String(parser.doctypePublic_);
        if (parser.hasSystem_) doc.doctypeSystem = String(parser.doctypeSystem_);
        doc.otherTopLevel = parser.otherTop_;
    } catch (const ParseError& e) {
        doc.root.reset();
        int l = 0, c = 0;
        parser.lineCol(e.pos, l, c);
        if (err) *err = String::latin1(e.msg);
        if (line) *line = l;
        if (col) *col = c;
        return false;
    }
    return true;
}

}  // namespace m2m::xml
