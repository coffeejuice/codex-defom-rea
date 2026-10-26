#include "../support/test_paths.hpp"
// ctest-standalone: src/model/keywords.cpp src/model/objdata.cpp src/mst/emit_b.cpp src/mst/emit_b_lines.cpp src/mst/emit_b_doe.cpp src/core/basic.cpp src/xml/dom.cpp tests/p09/doubles.cpp tests/p09/kw_doubles.cpp
// P09 tier 1, standalone against the test doubles in doubles.cpp/kw_doubles.cpp (main build via the
// line above, or tests/p09/CMakeLists.txt): the ActionKeywords KEY tokenizer
// 0x140906e40, the keyword parser 0x140096390, parseActionKeywords 0x140095cc0 and the InterObject
// init 0x1400651f0 (s1-01 7). The corpus group parses every Keyword list of the oracle-A .moproj
// files and checks that each OBJPOS line and each operation's INICTC/GENCTC lines the P09 emitters
// write for it equal the saved MST.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <map>
#include <regex>
#include <set>
#include <sstream>
#include <string>

#include "m2m/model.hpp"
#include "m2m/mst/emit.hpp"

using namespace m2m;

namespace p09test {
bool g_is2D = false;
std::vector<m2m::RecoveryEntry> g_recA;
bool g_recANull = false;
}  // namespace p09test

static int g_fail = 0;
static int g_checks = 0;

static void check(const char* what, const std::string& got, const std::string& want)
{
    ++g_checks;
    if (got == want) return;
    ++g_fail;
    std::printf("FAIL %s\n--- got\n%s\n--- want\n%s\n---\n", what, got.c_str(), want.c_str());
}
static void checkInt(const char* what, int got, int want) { check(what, std::to_string(got), std::to_string(want)); }

static void test_tokenizer()
{
    Project p;
    {
        KeyTokenizer t(p, String(u"* comment\r\n  \t\nTLOC 1.5D-3 // note\r\nOBJPOS,2:2=0.25e1 7\nX\n\nNEVER"));
        std::string s;
        double d = 0;
        int i = 0;
        // "  \t" has no token: skipped like a comment line
        checkInt("count TLOC", t.tok(2, &s, -5000), 2);
        check("kw TLOC", s, "TLOC");
        t.tok(10, &d);
        check("D exponent", std::to_string(d), "0.001500");
        checkInt("count OBJPOS", t.tok(2, &s, -5000), 5);
        check("kw OBJPOS", s, "OBJPOS");
        checkInt("token 5", t.tok(5, &i, 5), 0);
        checkInt("token 5 value", i, 7);
        checkInt("token 6 missing", t.tok(5, &i, 6), -1);
        checkInt("next after positional", t.tok(5, &i), 0);
        checkInt("next = token 2", i, 2);
        t.tok(5, &i);
        t.tok(10, &d);
        check("token 4", std::to_string(d), "2.500000");
        t.tok(5, &i);
        checkInt("token 5 again", i, 7);
        // tokens used up: the next read takes the first token of the next line
        s.clear();
        checkInt("spill to next line", t.tok(3, &s), 0);
        check("spill token", s, "X");
        // a blank line ends the text
        checkInt("blank line", t.tok(2, &s, -5000), -2);
        check("DONE", s, "DONE");
    }
    {
        KeyTokenizer t(p, String(u"abc def\r"));   // a CR at the very end of the text is dropped
        std::string s;
        checkInt("upper", t.tok(2, &s, -5000), 2);
        check("upper text", s, "ABC");
        t.tok(3, &s);
        check("text keeps case", s, "def");
        checkInt("end", t.tok(2, &s, -5000), -2);
        check("end DONE", s, "DONE");
    }
    {
        KeyTokenizer t(p, String(u"A 1\nB 2"));
        int i = 0;
        std::string s;
        t.tok(2, &s, -5000);
        checkInt("int type 8", (t.tok(8, &i), i), 1);
        float f = 0;
        checkInt("next line via -5000 count", t.tok(9, &f, -5000), 2);   // first token "B" as float
        checkInt("float of B", static_cast<int>(f), 0);
        std::uint8_t b = 0;
        t.tok(1, &b);
        checkInt("bool", b, 1);
    }
    {
        Project q;
        q.tokMode = 1;   // cut at column 63
        std::string line(70, 'A');
        line[62] = ' ';
        line += " Z";
        KeyTokenizer t(q, String::latin1(line));
        std::string s;
        checkInt("mode 1 count", t.tok(3, &s, -5000), 1);   // " Z" is beyond column 63
        checkInt("mode 0 count", t.tokWithMode(0, 3, &s, -5000), -2);   // nothing left
    }
}

// ---- parser ---------------------------------------------------------------------------------------
static std::unique_ptr<Operation> newOp(Project& p, int slots)
{
    auto o = std::make_unique<Operation>();
    o->project = &p;
    o->objIds.assign(static_cast<size_t>(slots), 1);
    return o;
}

static void test_parse()
{
    Project p;
    auto op = newOp(p, 3);
    p09test::g_is2D = false;
    Ctx ctx;
    parseKeywordText(p, String(u"TLOC      \t2.5"), *op, ctx);
    check("tloc", std::to_string(op->tloc), "2.500000");
    initInterObjectBlock(*op);
    op->icb.genVal = 9;
    initInterObjectBlock(*op);
    checkInt("init flag 1", op->icb.flag[0], 1);
    checkInt("init genVal", static_cast<int>(op->icb.genVal), 0);
    parseKeywordText(p, String(u"INICTC    \t3\nGENCTC    \t0.5\nINICTC 100"), *op, ctx);
    checkInt("inictc 3", op->icb.flag[2], 1);
    checkInt("inictc 100", op->icb.flag[99], 1);
    check("genctc", std::to_string(op->icb.genVal), "0.500000");
    bool threw = false;
    try {
        parseKeywordText(p, String(u"INICTC 0"), *op, ctx);
    } catch (const Error& e) {
        threw = e.code == M2M_E_PARSE;
    }
    checkInt("inictc 0 rejected", threw, 1);

    // ActionKeywords only set: 11_THEXP EXIT (INICTC 1, no GENCTC) keeps GENCTC on, as v11 open+save
    {
        auto o3 = newOp(p, 1);
        initInterObjectBlock(*o3);
        xml::Node ak;
        for (const char16_t* s : {u"TLOC      	0", u"", u"INICTC    	1", u""}) {
            auto kw = std::make_shared<xml::Node>();
            kw->tag = String(u"Keyword");
            auto tx = std::make_shared<xml::Node>();
            tx->kind = xml::Node::Text;
            tx->data = String(s);
            kw->children.push_back(tx);
            ak.children.push_back(kw);
        }
        parseActionKeywords(p, ak, *o3, ctx);
        check("THEXP EXIT block", std::to_string(o3->icb.flag[0]) + std::to_string(o3->icb.enable) +
                                      std::to_string(o3->icb.gen), "111");
        xml::Node none;
        parseActionKeywords(p, none, *o3, ctx);
        check("empty ActionKeywords", std::to_string(o3->icb.flag[0]) + std::to_string(o3->icb.enable) +
                                          std::to_string(o3->icb.gen), "111");
    }

    // OBJPOS, 3D: VDIE's record with an id list on the next line
    parseKeywordText(p, String(u"OBJPOS\t2\t2\t0.0001\t1\t0\t0\t6\t0\t0\t1\n-3\nOBJPOS\t1\t3\t0\t0\t0\t1\t0\t0\t0"),
                     *op, ctx);
    checkInt("objpos count", static_cast<int>(op->objpos.size()), 2);
    const ObjPos& a = *op->objpos[0];
    check("objpos a", std::to_string(a.object) + " " + std::to_string(a.mode) + " " + std::to_string(a.d(0x10)) +
              " " + std::to_string(a.d(0x18)) + " " + std::to_string(a.i08) + " list " +
              std::to_string(a.list.size()) + ":" + std::to_string(a.list.empty() ? 0 : a.list[0]),
          "2 2 0.000100 1.000000 6 list 1:-3");
    const ObjPos& b = *op->objpos[1];
    check("objpos b", std::to_string(b.d(0x60)) + " " + std::to_string(b.d(0x78)), "1.000000 0.000000");
    // 2D: type 2 reads D10, I, D18, D20 and the list count is token 8
    p09test::g_is2D = true;
    auto op2 = newOp(p, 2);
    parseKeywordText(p, String(u"OBJPOS 3 2 0.0001 1 0 -1 2\n4 5"), *op2, ctx);
    const ObjPos& c = *op2->objpos[0];
    check("objpos 2D", std::to_string(c.i08) + " " + std::to_string(c.d(0x20)) + " list " +
              std::to_string(c.list.size()), "1 -1.000000 list 2");
    p09test::g_is2D = false;

    // per-object keywords
    parseKeywordText(p, String(u"NDTMP 2 0 20\nURZ 1 0 1 2 3\nGRAIN 3 1 2 3 4 55\nAVGSTR 1 0.2\nNDTMP 0 0 9"),
                     *op, ctx);
    check("ndtmp obj 2", std::to_string(op->objs[1]->sv.ndtmp.set) + " " + std::to_string(op->objs[1]->sv.ndtmp.v),
          "1 20.000000");
    check("urz 3D", std::to_string(op->objs[0]->sv.urz[2]), "3.000000");
    check("grain", std::to_string(op->objs[2]->sv.grain.v), "55.000000");
    check("avgstr", std::to_string(op->objs[0]->sv.avgstr.v), "0.200000");
    threw = false;
    try {
        parseKeywordText(p, String(u"STRAIN 9 0 1"), *op, ctx);
    } catch (const Error& e) {
        threw = e.code == M2M_E_PARSE;
    }
    checkInt("object beyond slots rejected", threw, 1);
    // unknown keywords and comment lines are skipped
    parseKeywordText(p, String(u"* x\nFOO 1 2\nTLOC 7"), *op, ctx);
    check("after unknown", std::to_string(op->tloc), "7.000000");
}

// ---- corpus round trip -------------------------------------------------------------------------------
static std::string xmlText(std::string s)
{
    const std::pair<const char*, const char*> ents[] = {{"&lt;", "<"}, {"&gt;", ">"}, {"&quot;", "\""},
                                                        {"&apos;", "'"}, {"&amp;", "&"}};
    for (auto [e, r] : ents)
        for (size_t k; (k = s.find(e)) != std::string::npos;) s.replace(k, std::strlen(e), r);
    for (size_t k; (k = s.find("\r\n")) != std::string::npos;) s.replace(k, 2, "\n");
    return s;
}

static void test_corpus()
{
    std::filesystem::path root = m2m_test::configuredPath("M2M_CORPUS");
    if (!std::filesystem::is_directory(root)) {
        std::printf("SKIP corpus: %s missing\n", root.string().c_str());
        return;
    }
    std::ifstream cases(root / "cases.tsv");
    std::string row;
    std::getline(cases, row);
    int files = 0, lines = 0, contacts = 0;
    const std::regex opRe("<Operation>([\\s\\S]*?)</Operation>");
    const std::regex typeRe("<Type>([^<]*)</Type>");
    const std::regex kwRe("<Keyword>([\\s\\S]*?)</Keyword>");
    while (std::getline(cases, row)) {
        std::vector<std::string> f;
        std::stringstream ss(row);
        for (std::string x; std::getline(ss, x, '\t');) f.push_back(x);
        if (f.size() < 7 || f[6] != "A") continue;
        std::ifstream mp(root / f[0] / f[2], std::ios::binary), ms(root / f[0] / f[3], std::ios::binary);
        if (!mp || !ms) continue;
        std::string proj((std::istreambuf_iterator<char>(mp)), {}), mst((std::istreambuf_iterator<char>(ms)), {});
        std::set<std::string> mstLines;
        std::map<std::string, std::string> contactOf;   // OPRNAM -> INICTC/GENCTC lines of its block
        {
            std::stringstream ml(mst);
            std::string blk, prev;
            for (std::string l; std::getline(ml, l);) {
                mstLines.insert(l);
                if (l.rfind("SIMULATION", 0) == 0) blk.clear();
                if (l.rfind("INICTC", 0) == 0 || l.rfind("GENCTC", 0) == 0) blk += l + "\n";
                if (prev == "OPRNAM" && !contactOf.count(l)) contactOf[l] = blk;
                prev = l;
            }
        }
        ++files;
        const std::regex nameRe("<Name>([^<]*)</Name>");
        for (auto it = std::sregex_iterator(proj.begin(), proj.end(), opRe); it != std::sregex_iterator(); ++it) {
            std::string body = (*it)[1], type, name;
            std::smatch tm;
            if (std::regex_search(body, tm, typeRe)) type = tm[1];
            if (std::regex_search(body, tm, nameRe)) name = xmlText(tm[1]);
            if (type.find("Cogging") != std::string::npos) continue;   // Cogging writes its own OBJPOS
            p09test::g_is2D = type.find("2D") != std::string::npos;
            Project p;
            Ctx ctx;
            auto op = newOp(p, 10);
            initInterObjectBlock(*op);
            xml::Node ak;
            for (auto k = std::sregex_iterator(body.begin(), body.end(), kwRe); k != std::sregex_iterator(); ++k) {
                auto kw = std::make_shared<xml::Node>();
                kw->tag = String(u"Keyword");
                auto tx = std::make_shared<xml::Node>();
                tx->kind = xml::Node::Text;
                tx->data = String::latin1(xmlText((*k)[1]));
                kw->children.push_back(tx);
                ak.children.push_back(kw);
            }
            parseActionKeywords(p, ak, *op, ctx);
            mst::Stream ts(1252);
            mst::State st;
            mst::W w{ts, st, ctx};
            for (const auto& o : op->objpos)
                p09test::g_is2D ? mst::emitObjposLine2D(w, *o) : mst::emitObjposLine3D(w, *o);
            std::stringstream out(ts.bytes());
            for (std::string l; std::getline(out, l);) {
                ++lines;
                if (!mstLines.count(l)) {
                    ++g_fail;
                    std::printf("FAIL corpus %s: %s line not in the MST: %s\n", f[0].c_str(), type.c_str(), l.c_str());
                }
            }
            // Boolean's writer puts its INICTC lines after OPRNAM (s1-04 1), outside this capture
            if (contactOf.count(name) && type.find("Boolean") == std::string::npos) {
                mst::Stream cs(1252);
                mst::W cw{cs, st, ctx};
                mst::emitContactBlock(cw, op->icb);
                ++contacts;
                // waiver unopened-genctc (run_corpus.py): v11 open+save adds GENCTC 0 after INICTC
                std::string want = contactOf[name];
                if (f.size() > 23 && f[23].find("unopened-genctc") != std::string::npos &&
                    want.find("INICTC") != std::string::npos && want.find("GENCTC") == std::string::npos)
                    want += "GENCTC    \t0\n";
                if (cs.bytes() != want) {
                    ++g_fail;
                    std::printf("FAIL corpus %s %s: INICTC/GENCTC\n--- got\n%s--- MST\n%s", f[0].c_str(),
                                name.c_str(), cs.bytes().c_str(), want.c_str());
                }
            }
        }
    }
    std::printf("corpus: %d oracle-A projects, %d OBJPOS lines rebuilt from ActionKeywords\n", files, lines);
    ++g_checks;
}

int main()
{
    struct T {
        const char* name;
        void (*fn)();
    } tests[] = {{"tokenizer", test_tokenizer}, {"parse", test_parse}, {"corpus", test_corpus}};
    int skipped = 0;
    for (const T& t : tests) {
        try {
            t.fn();
        } catch (const NotPorted& e) {
            ++skipped;
            std::printf("SKIP %s: %s\n", t.name, e.what());
        }
    }
    std::printf("p09 keywords: %d checks, %d failed, %d groups skipped\n", g_checks, g_fail, skipped);
    return g_fail ? 1 : 0;
}
