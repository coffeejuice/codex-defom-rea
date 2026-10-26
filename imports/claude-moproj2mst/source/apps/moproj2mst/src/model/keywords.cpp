// ActionKeywords of an operation (owner: P09, taken over from P02): the Keyword children, the
// keyword-text parser and its KEY tokenizer over a QTextStream, and the InterObject block init.
// Spec: s1-01 7, 5.3; decompiles in gui_mo/s1/s1-01/dc/open3.c, open4.c and gui_mo/out_p09/d4.c.
// Literals checked against the exe copy. The field setters the parser calls are P08's
// (model.hpp, src/model/objdata.cpp) and P16's (m2m/setters.hpp); the twin of the tokenizer is DEF_PRE 0x140411ad0 (port/defcmd
// geolib/token.cpp) and the file-based 0x1409068f0 (src/datafiles/token.cpp, P06).
#include <cstdlib>
#include <cstring>
#include <string>

#include "m2m/datafiles.hpp"
#include "m2m/model.hpp"
#include "m2m/qt4.hpp"
#include "m2m/setters.hpp"

namespace m2m {

// ---- 0x140906e40: KEY tokenizer over the keyword text ----------------------------------------------
// The original keeps the line buffer (0x143dd9250, 2000 bytes), the token table (0x143dd8e48 +
// 8k), the token count (0x143dd8e30) and the next index (0x14386f86c) in process globals, shared by
// every tokenizer object; the QTextStream belongs to the caller. Both live in Impl here, so a
// "fresh tokenizer" on the same stream (the SVSCAL array reader 0x1400abab0) is tokWithMode.
struct KeyTokenizer::Impl {
    std::u16string text;           // QTextStream(&copy, ReadWrite) over the text
    size_t pos = 0;
    int mode = 0;                  // the caller's tokenizer object +0x00 (P+0x00 here)
    char line[2000] = {};
    char* tok[1024] = {};          // slot 1 = first token
    int count = 0;
    int next = 1;

    // QTextStream::readLine(2000) (Qt 4.8): the next line without "\n" or "\r\n"; at most 2000
    // characters, the rest of a longer line is the next read. Null at the end of the text.
    qt::String readLine()
    {
        if (pos >= text.size()) return qt::String();
        size_t nl = text.find(u'\n', pos);
        size_t end = nl == std::u16string::npos ? text.size() : nl;
        size_t n = end - pos;
        if (n > 2000) {
            qt::String s(text.substr(pos, 2000));
            pos += 2000;
            return s;
        }
        std::u16string s = text.substr(pos, n);
        // "\r\n" ends a line; a '\r' at the very end of the text is dropped too (Qt 4.8 scan)
        if (!s.empty() && s.back() == u'\r') s.pop_back();
        pos = nl == std::u16string::npos ? text.size() : nl + 1;
        return qt::String(s);
    }

    // toAscii, then strncpy(line, data, 1999) (0x141470020): the buffer's last byte stays 0.
    void load(const qt::String& s)
    {
        std::string a = qt::toAscii(s);
        std::strncpy(line, a.c_str(), 1999);
    }
};

namespace {

constexpr char kSep[] = " ,:=\n\t\r";   // 0x1427b9ec0 / 0x1427b9ed0
constexpr char kEol[] = "\n\r";         // 0x1427b9eb0 / 0x1427b9ec8

void store(void* out, const char* s)
{
    if (out) *static_cast<std::string*>(out) = s;
}

}  // namespace

KeyTokenizer::KeyTokenizer(const Project& p, const String& text) : impl_(std::make_shared<Impl>())
{
    impl_->text = text.u16();
    impl_->mode = p.tokMode;
}

int KeyTokenizer::tok(int type, void* out, int idx) { return tokWithMode(impl_->mode, type, out, idx); }

// @src 0x140906e40
int KeyTokenizer::tokWithMode(int mode, int type, void* out, int idx)
{
    Impl& t = *impl_;
    char* tk;
    int ret;
    if (type == 12) {
        qt::String s = t.readLine();
        if (s.isEmpty()) {
            store(out, "DONE");                                   // 0x1427b9ea8
            return -2;
        }
        t.load(s);
        t.tok[1] = std::strtok(t.line, kEol);
        if (t.tok[1] == nullptr) t.tok[1] = t.line;
        tk = t.tok[1];
        t.next = 2;
        t.count = 1;
        ret = 0;
    } else if (idx == 0 || idx == -5000) {
        if (t.count < t.next || idx == -5000) {
            if (idx == -5000) ret = 0;
            else if (t.count < idx) ret = -1;                     // dead: idx is 0 here
            else ret = 0;
            do {
                qt::String s = t.readLine();
                if (s.isEmpty()) {                                // end of text or a blank line
                    if (type == 2) store(out, "DONE");            // 0x1427b9eb4
                    return -2;
                }
                t.load(s);
                if (mode == 1) t.line[63] = '\0';                 // 0x143dd928f
                if (type == 4) {
                    t.tok[1] = std::strtok(t.line, kEol);
                } else {
                    char* c = std::strstr(t.line, "//");          // 0x1400b22e0, 0x1427b9ebc
                    if (c != nullptr) *c = '\0';
                    t.tok[1] = std::strtok(t.line, kSep);
                }
                t.count = 1;
                for (;;) {
                    // The original's table has no bound; 1999 bytes hold at most 1000 tokens.
                    char* x = std::strtok(nullptr, kSep);
                    if (t.count + 1 >= static_cast<int>(sizeof t.tok / sizeof t.tok[0])) x = nullptr;
                    t.tok[t.count + 1] = x;
                    if (x == nullptr) break;
                    t.count = t.count + 1;
                }
            } while (t.tok[1] == nullptr || *t.tok[1] == '*');
            tk = t.tok[1];
            t.next = 2;
        } else {
            tk = t.tok[t.next];
            t.next = t.next + 1;
            ret = 0;
        }
    } else {
        if (idx < 1 || t.count < idx) return -1;
        tk = t.tok[idx];
        ret = 0;
    }
    switch (type - 1) {
    case 0:
        *static_cast<std::uint8_t*>(out) = std::atoi(tk) != 0;    // 0x1414737ac
        break;
    case 1:
        datafiles::upperInPlace(tk);                              // 0x140906880
        store(out, tk);
        break;
    case 2:
    case 3:
        store(out, tk);
        break;
    case 4:
    case 5:
    case 6:
        *static_cast<std::int32_t*>(out) = std::atoi(tk);         // 0x1414737ac (integer store)
        break;
    case 7: {
        char* end = nullptr;                                      // 0x141475c74 (base 10)
        *static_cast<std::int32_t*>(out) = static_cast<std::int32_t>(std::strtol(tk, &end, 10));
        break;
    }
    case 8:
        datafiles::replaceChar(tk, 'D', 'E', 0);                  // 0x140906790
        *static_cast<float*>(out) = static_cast<float>(std::atof(tk));   // 0x1414737a4
        break;
    case 9: {
        datafiles::replaceChar(tk, 'D', 'E', 0);
        char* end = nullptr;
        *static_cast<double*>(out) = std::strtod(tk, &end);       // 0x1414759b8
        break;
    }
    case 10:
        datafiles::replaceChar(tk, 'D', 'E', 0);
        break;
    case 11:
        store(out, tk);
        break;
    default:
        break;
    }
    if (idx == -5000) ret = t.count;
    return ret;
}

// ---- 0x140095cc0 / 0x140096390 ---------------------------------------------------------------------

// @src 0x140095cc0
void parseActionKeywords(Project& p, const xml::Node& e, Operation& op, Ctx& ctx)
{
    // No reset of the InterObject block: it keeps the 0x1400651f0 ctor defaults (INICTC 1, enable 1,
    // GENCTC on) and the keywords only set. So an operation with INICTC and no GENCTC keyword writes
    // "GENCTC 0", as v11 open+save does (oracle B R11/R19/R46/R49/R52, plan 8).
    for (const xml::Node* c : e.childElements()) {
        if (c->tag == qt::L("Keyword" /*142140018*/)) parseKeywordText(p, c->text(), op, ctx);
    }
}

namespace {

[[noreturn]] void badInput(const char* what)
{
    throw Error(M2M_E_PARSE, std::string("ActionKeywords: ") + what +
                                 " (the original writes outside the record)");
}

// The state variables of object n (1-based) of the operation, or null; the original computes
// OBJ(n)+0x30 without a check and writes through it only when n > 0.
StateVars* svOf(Operation& op, int n)
{
    ObjEntry* o = objEntryN(op, n);                               // 0x1400612d0
    if (n > 0 && !o) badInput("object number beyond the operation's slots");
    return o ? &o->sv : nullptr;                                  // 0x14004df00
}

}  // namespace

// @src 0x140096390
void parseKeywordText(Project& p, const String& text, Operation& op, Ctx& /*ctx*/)
{
    KeyTokenizer ts(p, text);   // QTextStream(&copy, ReadWrite); OPI = item+0x48 (never null here)
    std::string kw;
    do {
        int n = ts.tok(2, &kw, -5000);
        if (kw == "TLOC" /*14214004c*/) {
            double v = 0;
            ts.tok(10, &v);
            setTloc(op, v);                                // 0x1400abc90
        } else if (kw == "OBJPOS" /*142140054*/) {
            int a = 0;
            int m = 0;   // uninitialised in the original when the text ends here
            int k = 0;
            ObjPos* o = newObjpos(op);                     // 0x140061e30
            ts.tok(5, &a);
            o->object = a;
            ts.tok(5, &m);
            o->mode = m;
            switch (o->mode - 1) {
            case 0:
            case 10:
                ts.tok(10, &o->d(0x30));
                ts.tok(10, &o->d(0x38));
                if (is3D(op)) ts.tok(10, &o->d(0x40));            // 0x14005f1c0
                break;
            case 1:
            case 0xb:
            case 0x65:
            case 0x6f:
                if (!is2D(op)) {                                  // 0x14005f150
                    ts.tok(10, &o->d(0x10));
                    ts.tok(10, &o->d(0x18));
                    ts.tok(10, &o->d(0x20));
                    ts.tok(10, &o->d(0x28));
                    ts.tok(5, &o->i08);
                } else {
                    ts.tok(10, &o->d(0x10));
                    ts.tok(5, &o->i08);
                    ts.tok(10, &o->d(0x18));
                    ts.tok(10, &o->d(0x20));
                }
                break;
            case 2:
                ts.tok(10, &o->d(0x48));
                ts.tok(10, &o->d(0x50));
                if (is3D(op)) {
                    ts.tok(10, &o->d(0x58));
                    ts.tok(10, &o->d(0x60));
                    ts.tok(10, &o->d(0x68));
                    ts.tok(10, &o->d(0x70));
                }
                ts.tok(10, &o->d(0x78));
                break;
            case 3:
                if (is2D(op)) {
                    ts.tok(10, &o->d(0x80));
                    ts.tok(10, &o->d(0x88));
                    ts.tok(10, &o->d(0x98));
                    ts.tok(10, &o->d(0xa0));
                }
                break;
            case 5:
                if (is3D(op)) {
                    ts.tok(10, &o->d(0x60));
                    ts.tok(10, &o->d(0x68));
                    ts.tok(10, &o->d(0x70));
                }
                ts.tok(10, &o->d(0x78));
                break;
            case 6:
            case 7:
            case 8:
                if (is3D(op)) {
                    ts.tok(10, &o->d(0x10));
                    ts.tok(10, &o->d(0x18));
                    ts.tok(10, &o->d(0x20));
                    ts.tok(10, &o->d(0x28));
                    if (o->mode == 9) {
                        ts.tok(10, &o->d(0x60));
                        ts.tok(10, &o->d(0x68));
                        ts.tok(10, &o->d(0x70));
                    }
                }
                break;
            default:
                break;
            }
            int lim, at;
            if (!is2D(op)) {
                lim = 10;
                at = 0xb;
            } else {
                lim = 7;
                at = 8;
            }
            if (lim < n) {
                ts.tok(5, &k, at);
                if (0 < k) {
                    int v = 0;
                    objposClearList(*o);                   // 0x1400abc40
                    ts.tok(5, &v, -5000);
                    objposAppend(*o, v);                   // 0x1400abc60
                    for (int j = 1; j < k; ++j) {
                        ts.tok(5, &v);
                        objposAppend(*o, v);
                    }
                }
            }
        } else if (kw == "INICTC" /*14214005c*/) {
            ContactBlock& c = op.icb;                             // 0x140065790
            int i = 0;
            ts.tok(5, &i);
            if (i < 1 || i > 100) badInput("INICTC object number outside 1..100");
            setIcbFlag(c, i, 1);                                  // 0x140065270
            setIcbEnable(c, 1);                            // 0x1400ab7c0
        } else if (kw == "GENCTC" /*142140064*/) {
            ContactBlock& c = op.icb;
            double g = 0;
            setIcbGen(c, 1);                               // 0x1400ab7e0
            ts.tok(10, &g);
            set::genctcValue(c, g);                               // 0x1404ce5b0
        } else if (kw == "NDTMP" /*14214006c*/) {
            int no = 0, skip = 0;
            double v = 0;
            ts.tok(5, &no);
            ts.tok(5, &skip);
            ts.tok(10, &v);
            StateVars* sv = svOf(op, no);
            if (0 < no) {
                svSetNdtmpFlag(*sv, 1);                           // 0x1400ab800
                svSetNdtmp(*sv, v);                               // 0x1400ab820
            }
        } else if (kw == "STRAIN" /*142140074*/) {
            int no = 0, skip = 0;
            double v = 0;
            ts.tok(5, &no);
            ts.tok(5, &skip);
            ts.tok(10, &v);
            StateVars* sv = svOf(op, no);
            if (0 < no) {
                svSetStrainFlag(*sv, 1);                          // 0x1400ab840
                svSetStrain(*sv, v);                              // 0x1400ab860
            }
        } else if (kw == "STRESS" /*14214007c*/) {
            int no = 0, skip = 0;
            double v = 0;
            ts.tok(5, &no);
            ts.tok(5, &skip);
            ts.tok(10, &v);
            StateVars* sv = svOf(op, no);
            if (0 < no) {
                svSetStressFlag(*sv, 1);                          // 0x1400ab880
                svSetStress(*sv, v);                              // 0x1400ab8a0
            }
        } else if (kw == "DAMAGE" /*142140084*/) {
            int no = 0, skip = 0;
            double v = 0;
            ts.tok(5, &no);
            ts.tok(5, &skip);
            ts.tok(10, &v);
            StateVars* sv = svOf(op, no);
            if (0 < no) {
                svSetDamageFlag(*sv, 1);                          // 0x1400ab8c0
                set::damage(*sv, v);                              // 0x141432bc0
            }
        } else if (kw == "URZ" /*14213f57c*/) {
            int no = 0, skip = 0;
            double a = 0, b = 0, c = 0;
            ts.tok(5, &no);
            ts.tok(5, &skip);
            ts.tok(10, &a);
            ts.tok(10, &b);
            StateVars* sv = svOf(op, no);
            if (0 < no) {
                set::urzFlag(*sv, 1);                             // 0x1412e2450
                svSetUrzA(*sv, a);                                // 0x1400ab8e0
                svSetUrzB(*sv, b);                                // 0x1400ab900
            }
            if (is3D(op)) {
                ts.tok(10, &c);
                if (0 < no) set::urz2(*sv, c);                    // 0x1416cab50
            }
        } else if (kw == "DRZ" /*14213f58c*/) {
            int no = 0, skip = 0;
            double a = 0, b = 0, c = 0;
            ts.tok(5, &no);
            ts.tok(5, &skip);
            ts.tok(10, &a);
            ts.tok(10, &b);
            StateVars* sv = svOf(op, no);
            if (0 < no) {
                set::drzFlag(*sv, 1);                             // 0x141632700
                set::drz0(*sv, a);                                // 0x1404ce5b0
                svSetDrzB(*sv, b);                                // 0x1400ab920
            }
            if (is3D(op)) {
                ts.tok(10, &c);
                if (0 < no) set::drz2(*sv, c);                    // 0x141163320
            }
        } else if (kw == "GRAIN" /*14214008c*/) {
            int no = 0, skip = 0;
            double v = 0;
            ts.tok(5, &no);
            for (int j = 0; j < 4; ++j) ts.tok(5, &skip);
            ts.tok(10, &v);
            StateVars* sv = svOf(op, no);
            if (0 < no) {
                svSetGrainFlag(*sv, 1);                           // 0x1400ab940
                set::grain(*sv, v);                               // 0x140bb1000
            }
        } else if (kw == "DENSTY" /*142140094*/) {
            int no = 0, skip = 0;
            double v = 0;
            ts.tok(5, &no);
            ts.tok(5, &skip);
            ts.tok(10, &v);
            StateVars* sv = svOf(op, no);
            if (0 < no) {
                svSetDenstyFlag(*sv, 1);                          // 0x1400ab960
                svSetDensty(*sv, v);                              // 0x1400ab980
            }
        } else if (kw == "SVSCAL" /*14214009c*/) {
            int no = 0, a = 0, b = 0;
            ts.tok(5, &no);
            ts.tok(5, &a);
            ts.tok(5, &b);
            StateVars* sv = svOf(op, no);
            if (0 < no) {
                svSetSvscalFlag(*sv, 1);                          // 0x1400aba00
                svscalSet(*sv, a, b, ts);                         // 0x1400aba20
            }
        } else if (kw == "AVGSTR" /*1421400a4*/) {
            int no = 0;
            double v = 0;
            ts.tok(5, &no);
            ts.tok(10, &v);
            StateVars* sv = svOf(op, no);
            if (0 < no) {
                svSetAvgstrFlag(*sv, 1);                          // 0x1400ab9a0
                svSetAvgstr(*sv, v);                              // 0x1400ab9c0
            }
        } else if (kw == "LMTSTR" /*1421400ac*/) {
            int no = 0;
            double v = 0;
            ts.tok(5, &no);
            ts.tok(10, &v);
            StateVars* sv = svOf(op, no);
            if (0 < no) {
                set::lmtstrFlag(*sv, 1);                          // 0x140414430
                svSetLmtstr(*sv, v);                              // 0x1400ab9e0
            }
        }
    } while (kw != "DONE" /*1421400b4*/);
}

// ---- 0x1400651f0 -------------------------------------------------------------------------------------

// @src 0x1400651f0  InterObject block of the OPI (OPI+0x70): no INICTC flag but object 1, INICTC
// and GENCTC on, GENCTC value 0, and the byte at block+0x70 (OPI+0xe0, lubricant "System") 0
void initInterObjectBlock(Operation& op)
{
    ContactBlock& c = op.icb;
    for (int i = 0; i < 100; ++i) c.flag[static_cast<size_t>(i)] = 0;
    setIcbFlag(c, 1, 1);                                          // 0x140065270
    c.enable = 1;
    c.gen = 1;
    c.genVal = 0.0;
    op.lubSystem = 0;
}

}  // namespace m2m
