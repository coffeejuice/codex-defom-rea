// QDomDocument::toString(indent) of Qt 4.8.4 (qdom.cpp: QDomDocumentPrivate::saveDocument and the
// save() of each node class) and the two DOM edits the .moproj writer needs. Packet moproj.
//
//   * The document stream gets the UTF-8 codec (saveDocument with EncodingFromDocument and no XML
//     declaration), so encodeText never needs a character reference for an unencodable char.
//   * The doctype is saved before the first node that is not an <?xml?> declaration.
//   * Attributes are saved in the iteration order of QDomNamedNodeMapPrivate::map, a
//     QHash<QString, QDomNodePrivate*>. setAttribute keeps an existing attribute in place and
//     inserts a new one (insertMulti), so the order is fixed by the sequence of first insertions,
//     which Node::attrs keeps. AttrOrder replays that sequence through Qt 4.8's QHashData.
#include <algorithm>
#include <string>
#include <vector>

#include "m2m/common.hpp"
#include "m2m/xml.hpp"

namespace m2m::xml {
namespace {

// qHash(const QString&) (qhash.cpp, Qt 4.8)
unsigned qtHash(const std::u16string& s)
{
    unsigned h = 0;
    for (char16_t c : s) {
        h = (h << 4) + c;
        h ^= (h & 0xf0000000u) >> 23;
        h &= 0x0fffffffu;
    }
    return h;
}

// QHashData::primeForNumBits (prime_deltas)
int primeForNumBits(int numBits)
{
    static const unsigned char deltas[] = {0, 0, 1, 3, 1, 5, 3, 3, 1, 9, 7, 5, 3, 9, 25, 3,
                                           1, 21, 3, 21, 7, 15, 9, 5, 3, 29, 15, 0, 0, 0, 0, 0};
    return (1 << numBits) + deltas[numBits];
}

// QHashData with only insertions of distinct keys: willGrow (size >= numBuckets -> rehash(numBits
// + 1), MinNumBits 4), createNode at the end of the bucket chain, rehash moving runs of equal
// hashes to the end of their new chain in old bucket order; iteration bucket by bucket.
class AttrOrder {
public:
    void insert(int idx, unsigned h)
    {
        if (size_ >= static_cast<int>(buckets_.size())) rehash(numBits_ + 1);
        buckets_[h % buckets_.size()].push_back({h, idx});
        ++size_;
    }
    std::vector<int> order() const
    {
        std::vector<int> r;
        for (const auto& b : buckets_)
            for (const auto& n : b) r.push_back(n.idx);
        return r;
    }

private:
    struct N {
        unsigned h;
        int idx;
    };
    std::vector<std::vector<N>> buckets_;
    int numBits_ = 0;
    int size_ = 0;

    void rehash(int hint)
    {
        if (hint < 4) hint = 4;                  // MinNumBits
        if (numBits_ == hint) return;
        std::vector<std::vector<N>> old;
        old.swap(buckets_);
        buckets_.assign(static_cast<std::size_t>(primeForNumBits(hint)), {});
        numBits_ = hint;
        for (const auto& chain : old) {
            std::size_t i = 0;
            while (i < chain.size()) {
                std::size_t j = i + 1;           // the run of nodes with the same hash
                while (j < chain.size() && chain[j].h == chain[i].h) ++j;
                auto& dst = buckets_[chain[i].h % buckets_.size()];
                dst.insert(dst.end(), chain.begin() + static_cast<std::ptrdiff_t>(i),
                           chain.begin() + static_cast<std::ptrdiff_t>(j));
                i = j;
            }
        }
    }
};

bool isTextNode(const Node* n)                   // QDomNodePrivate::isText(): Text and CDATA
{
    return n && (n->kind == Node::Text || n->kind == Node::CData);
}

std::u16string hex(unsigned v)                   // QString::number(v, 16)
{
    static const char16_t d[] = u"0123456789abcdef";
    std::u16string r;
    do {
        r.insert(r.begin(), d[v & 15]);
        v >>= 4;
    } while (v);
    return r;
}

// encodeText (qdom.cpp) with a UTF-8 stream: every character can be encoded
std::u16string encodeText(const std::u16string& str, bool encodeQuotes, bool performAVN)
{
    std::u16string r;
    r.reserve(str.size());
    for (std::size_t i = 0; i < str.size(); ++i) {
        const char16_t c = str[i];
        if (c == u'<') r += u"&lt;";
        else if (encodeQuotes && c == u'"') r += u"&quot;";
        else if (c == u'&') r += u"&amp;";
        // the test reads the partly replaced string: two ']' just before in the output
        else if (c == u'>' && r.size() >= 2 && r[r.size() - 1] == u']' && r[r.size() - 2] == u']') r += u"&gt;";
        else if (performAVN && (c == 0xa || c == 0xd || c == 0x9)) r += u"&#x" + hex(c) + u";";
        else r += c;
    }
    return r;
}

std::u16string spaces(int depth, int indent)
{
    return std::u16string(static_cast<std::size_t>(indent < 1 ? 0 : depth * indent), u' ');
}

// quotedValue (qdom.cpp): single quotes when the value contains a double quote
std::u16string quoted(const std::u16string& v)
{
    const char16_t q = v.find(u'"') != std::u16string::npos ? u'\'' : u'"';
    return std::u16string(1, q) + v + q;
}

void saveNode(std::u16string& s, const Node* n, const Node* prev, const Node* next, int depth, int indent);

void saveChildren(std::u16string& s, const Node& p, int depth, int indent)
{
    const auto& c = p.children;
    for (std::size_t i = 0; i < c.size(); ++i)
        saveNode(s, c[i].get(), i ? c[i - 1].get() : nullptr, i + 1 < c.size() ? c[i + 1].get() : nullptr, depth,
                 indent);
}

void saveNode(std::u16string& s, const Node* n, const Node* prev, const Node* next, int depth, int indent)
{
    switch (n->kind) {
    case Node::Element: {
        // QDomElementPrivate::save
        if (!isTextNode(prev)) s += spaces(depth, indent);
        s += u'<';
        s += n->tag.u16();
        AttrOrder ord;
        for (std::size_t i = 0; i < n->attrs.size(); ++i) ord.insert(static_cast<int>(i), qtHash(n->attrs[i].name.u16()));
        for (int i : ord.order()) {
            const Attr& a = n->attrs[static_cast<std::size_t>(i)];
            s += u' ';
            s += a.name.u16();
            s += u"=\"";
            s += encodeText(a.value.u16(), true, true);
            s += u'"';
        }
        if (!n->children.empty()) {
            s += u'>';
            if (!isTextNode(n->children.front().get()) && indent != -1) s += u'\n';
            saveChildren(s, *n, depth + 1, indent);
            if (!isTextNode(n->children.back().get())) s += spaces(depth, indent);
            s += u"</";
            s += n->tag.u16();
            s += u'>';
        } else {
            s += u"/>";
        }
        if (!isTextNode(next) && indent != -1) s += u'\n';
        break;
    }
    case Node::Text:
        s += encodeText(n->data.u16(), true, false);   // QDomTextPrivate::save: encodeText(value, s)
        break;
    case Node::CData: {
        // QDomCDATASectionPrivate::save: "]]>" is split over two sections
        std::u16string v = n->data.u16();
        std::size_t p = 0;
        while ((p = v.find(u"]]>", p)) != std::u16string::npos) {
            v.replace(p, 3, u"]]]]><![CDATA[>");
            p += 15;
        }
        s += u"<![CDATA[" + v + u"]]>";
        break;
    }
    case Node::Comment: {
        // QDomCommentPrivate::save
        if (!isTextNode(prev)) s += spaces(depth, indent);
        s += u"<!--" + n->data.u16();
        if (!n->data.u16().empty() && n->data.u16().back() == u'-') s += u' ';
        s += u"-->";
        if (!isTextNode(next)) s += u'\n';
        break;
    }
    case Node::ProcessingInstruction:
        // QDomProcessingInstructionPrivate::save
        s += u"<?" + n->tag.u16() + u' ' + n->data.u16() + u"?>\n";
        break;
    }
}

}  // namespace

std::u16string toString(const Document& doc, int indent)
{
    if (doc.otherTopLevel)
        throw Error(M2M_E_UNSUPPORTED, "project file: a top-level node other than the doctype and the root (an XML "
                                       "declaration, comment or internal DTD subset) is not reproduced on save");
    std::u16string s;
    // QDomDocumentTypePrivate::save, before the first node that is not an XML declaration
    if (!doc.doctypeName.isNull() && !doc.doctypeName.isEmpty()) {
        s += u"<!DOCTYPE " + doc.doctypeName.u16();
        if (!doc.doctypePublic.isNull()) {
            s += u" PUBLIC " + quoted(doc.doctypePublic.u16());
            if (!doc.doctypeSystem.isNull()) s += u' ' + quoted(doc.doctypeSystem.u16());
        } else if (!doc.doctypeSystem.isNull()) {
            s += u" SYSTEM " + quoted(doc.doctypeSystem.u16());
        }
        s += u">\n";
    }
    if (doc.root) saveNode(s, doc.root.get(), nullptr, nullptr, 0, indent);
    return s;
}

void setAttribute(Node& e, const String& name, const String& value)
{
    for (Attr& a : e.attrs)
        if (a.name == name) {
            a.value = value;
            return;
        }
    e.attrs.push_back(Attr{name, value});
}

NodePtr createElement(const String& tag)
{
    auto n = std::make_shared<Node>();
    n->kind = Node::Element;
    n->tag = tag;
    return n;
}

NodePtr createTextNode(const String& text)
{
    auto n = std::make_shared<Node>();
    n->kind = Node::Text;
    n->data = text;
    return n;
}

}  // namespace m2m::xml
