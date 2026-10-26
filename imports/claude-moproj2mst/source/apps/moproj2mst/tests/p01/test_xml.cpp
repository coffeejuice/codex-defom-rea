// P01: the QDomDocument subset (setContent, attribute, text, child elements) on snippets and on
// every corpus .moproj (element count equals the number of start tags in the file).
#include <testkit/check.hpp>

#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>

#include "m2m/xml.hpp"

using m2m::qt::String;
using m2m::xml::Document;
using m2m::xml::Node;

namespace {

String S(const char16_t* s) { return String(s); }

Document parse(const std::string& bytes, bool expectOk = true)
{
    Document d;
    String err;
    int line = 0, col = 0;
    const bool ok = m2m::xml::setContent(d, bytes, &err, &line, &col);
    if (ok != expectOk)
        std::fprintf(stderr, "setContent %s: %s (%d:%d)\n", ok ? "succeeded" : "failed", err.utf8().c_str(), line, col);
    CHECK(ok == expectOk);
    return d;
}

int countElements(const Node& n)
{
    int k = n.kind == Node::Element ? 1 : 0;
    for (const auto& c : n.children) k += countElements(*c);
    return k;
}

}  // namespace

TEST_CASE(basic_tree_and_doctype)
{
    Document d = parse("<!DOCTYPE ProjectFile>\r\n<Project>\r\n <Title Name=\"\"/>\r\n <Unit Name=\"SI\"/>\r\n"
                       " <Sub_Project Active=\"0\" Number=\"5\" Type=\"0\" Id=\"0\" >\r\n </Sub_Project>\r\n</Project>\r\n");
    REQUIRE(d.root != nullptr);
    CHECK(d.root->tag == S(u"Project"));
    auto kids = d.root->childElements();
    REQUIRE(kids.size() == 3u);
    CHECK(d.root->children.size() == 3u);                 // whitespace-only text is not kept
    CHECK(kids[0]->tag == S(u"Title"));
    CHECK(kids[0]->text().isEmpty());
    CHECK(!kids[0]->text().isNull());                     // QDomElement::text() of an empty element is ""
    CHECK(kids[0]->hasAttribute(S(u"Name")));
    CHECK(kids[0]->attribute(S(u"Name"), S(u"x")).isEmpty());
    CHECK(kids[1]->attribute(S(u"Name")) == S(u"SI"));
    CHECK(kids[1]->attribute(S(u"Missing")).isNull());
    CHECK(kids[1]->attribute(S(u"Missing"), S(u"-1")) == S(u"-1"));
    CHECK(kids[2]->attribute(S(u"Number")) == S(u"5"));
    CHECK(d.root->childElements(S(u"Unit")).size() == 1u);
}

TEST_CASE(keyword_text_keeps_tabs_and_newlines)
{
    // CRLF becomes LF; leading/trailing whitespace of a non-blank text run is kept.
    Document d = parse("<A>\r\n <Keyword>TLOC      \t0</Keyword>\r\n <Keyword>OBJPOS\t1\r\nOBJPOS\t2\r\n\r\n</Keyword>"
                       "<Keyword></Keyword><Keyword>  \r\n </Keyword></A>");
    auto k = d.root->childElements(S(u"Keyword"));
    REQUIRE(k.size() == 4u);
    CHECK(k[0]->text() == S(u"TLOC      \t0"));
    CHECK(k[1]->text() == S(u"OBJPOS\t1\nOBJPOS\t2\n\n"));
    CHECK(k[2]->text().isEmpty());
    CHECK(k[3]->text().isEmpty());                        // whitespace-only run: no text node
    CHECK(k[3]->children.empty());
}

TEST_CASE(text_is_concatenated_descendant_text)
{
    Document d = parse("<A>x<B>y<!-- c --><C>z</C></B><?pi data?><![CDATA[ <w> ]]>&amp;&lt;&gt;&quot;&apos;&#65;&#x42;</A>");
    CHECK(d.root->text() == S(u"xyz <w> &<>\"'AB"));
    // comment and PI nodes exist but do not contribute
    bool sawComment = false, sawPi = false;
    for (const auto& c : d.root->childElements()[0]->children) sawComment |= c->kind == Node::Comment;
    for (const auto& c : d.root->children) sawPi |= c->kind == Node::ProcessingInstruction;
    CHECK(sawComment);
    CHECK(sawPi);
}

TEST_CASE(attributes)
{
    Document d = parse("<A a='1' b = \"x&amp;y\" c=\"t\tab\" a=\"2\"/>");
    CHECK(d.root->attribute(S(u"a")) == S(u"2"));          // a later duplicate replaces the value
    CHECK(d.root->attrs.size() == 3u);
    CHECK(d.root->attribute(S(u"b")) == S(u"x&y"));
    CHECK(d.root->attribute(S(u"c")) == S(u"t\tab"));      // no attribute-value normalisation
    CHECK(d.root->text().isEmpty());
}

TEST_CASE(encodings)
{
    // UTF-8 (default), BOM dropped, invalid byte -> U+FFFD
    Document a = parse("\xef\xbb\xbf<A>\xc3\xa4\xff</A>");
    CHECK(a.root->text() == S(u"ä�"));
    // declared Latin-1
    Document b = parse("<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?><A>\xe4</A>");
    CHECK(b.root->text() == S(u"ä"));
    // UTF-16LE with BOM
    Document c = parse(std::string("\xff\xfe<\0A\0>\0x\0<\0/\0A\0>\0", 18));
    CHECK(c.root->text() == S(u"x"));
}

TEST_CASE(errors)
{
    parse("", false);
    parse("<A>", false);
    parse("<A></B>", false);
    parse("<A x=1/>", false);
    parse("<A>&unknown;</A>", false);
    parse("<A/><B/>", false);
    parse("<!DOCTYPE A><!DOCTYPE B><A/>", false);
    Document d;
    String err;
    int line = 0, col = 0;
    CHECK(!m2m::xml::setContent(d, "<A>\n<B>\n</C>", &err, &line, &col));
    CHECK(err == S(u"tag mismatch"));
    CHECK_EQ(line, 3);
}

TEST_CASE(corpus_files_parse)
{
    const char* corpus = std::getenv("M2M_CORPUS");
    REQUIRE(corpus != nullptr);
    int files = 0;
    for (const auto& dir : std::filesystem::directory_iterator(corpus)) {
        if (!dir.is_directory()) continue;
        for (const auto& f : std::filesystem::directory_iterator(dir.path())) {
            if (f.path().extension() != ".moproj") continue;
            ++files;
            std::ifstream in(f.path(), std::ios::binary);
            const std::string s((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
            Document d = parse(s);
            REQUIRE(d.root != nullptr);
            CHECK(d.root->tag == S(u"Project"));
            int tags = 0;   // start tags: '<' followed by a name character
            for (size_t i = 0; i + 1 < s.size(); ++i)
                if (s[i] == '<' && (std::isalpha(static_cast<unsigned char>(s[i + 1])) || s[i + 1] == '_')) ++tags;
            const int els = countElements(*d.root);
            if (els != tags) std::fprintf(stderr, "%s: %d elements, %d start tags\n", f.path().string().c_str(), els, tags);
            CHECK_EQ(els, tags);
        }
    }
    CHECK_EQ(files, 56);
}

int main() { return testkit::run_all(); }
