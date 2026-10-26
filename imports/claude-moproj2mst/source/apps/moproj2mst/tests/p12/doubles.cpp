// Test doubles for the P12 tier-1 test (ctest-standalone, no m2m_core): every seam the P12 writers
// call, so the writers are tested alone, independent of P02-P05 and P08-P10. Each emitter writes a
// marker line "[<address> args]" to the stream; the stream doubles keep the bytes.
#include <cstdlib>
#include <string>

#include "m2m/model.hpp"
#include "m2m/mst/emit.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/props.hpp"
#include "m2m/qt4path.hpp"

#include "doubles.hpp"

namespace p12test {
int flowNumberValue = 0;
bool startsFromKeyValue = false;
String projectDirValue = String(u"C:\\prj");
}  // namespace p12test

namespace m2m::qt {

std::string toAscii(const String& s)
{
    std::string out;
    for (char16_t c : s.u16()) out += c < 0x100 ? static_cast<char>(c) : '?';
    return out;
}
std::string formatInt(int v) { return std::to_string(v); }
String number(int v) { return String::latin1(std::to_string(v)); }   // for props_base.cpp (P15)
int toInt(const String& s, bool* ok) { if (ok) *ok = true; return std::atoi(s.utf8().c_str()); }
double toDouble(const String& s, bool* ok) { if (ok) *ok = true; return std::atof(s.utf8().c_str()); }
std::string encodeLocale(const String& s, unsigned) { return toAscii(s); }

// Enough of QDir::cleanPath for the paths these tests build (no "." or ".." components).
String cleanPath(const String& p)
{
    if (p.isNull() || p.isEmpty()) return p;
    std::u16string out;
    for (char16_t c : p.u16()) {
        if (c == u'\\') c = u'/';
        if (c == u'/' && !out.empty() && out.back() == u'/') continue;
        out += c;
    }
    if (out.size() > 1 && out.back() == u'/') out.pop_back();
    return String(out);
}

String convertSeparators(const String& p)
{
    std::u16string out = p.u16();
    for (auto& c : out)
        if (c == u'/') c = u'\\';
    return p.isNull() ? p : String(out);
}

}  // namespace m2m::qt

namespace m2m::app {
// REL(p) with the project folder as the current directory: strip "<projectDir>/".
String relToDb(const String&, const String& p)
{
    std::u16string dir = qt::cleanPath(p12test::projectDirValue).u16() + u"/";
    std::u16string s = p.u16();
    if (s.compare(0, dir.size(), dir) == 0) s = s.substr(dir.size());
    return String(u"REL(") + String(s) + String(u")");
}
}  // namespace m2m::app

namespace m2m::mst {

// This standalone double appends bytes immediately; it has no pending text.
Stream& Stream::flush() { return *this; }

Stream& Stream::operator<<(const char* lit)
{
    buf_ += lit;
    return *this;
}
Stream& Stream::operator<<(int v)
{
    buf_ += qt::formatInt(v);
    return *this;
}
Stream& Stream::operator<<(double)
{
    buf_ += "<double>";
    return *this;
}
Stream& Stream::operator<<(const String& s)
{
    buf_ += qt::encodeLocale(s, codepage_);
    return *this;
}
Stream& Stream::endl()
{
    buf_ += "\n";
    return *this;
}

// OpClass base slots (P10 / P02 own the real ones).
void OpClass::writeMst(W&, Project&, const String&, int*, Operation*, int, int) {}
void OpClass::writeSetup(W&, Project&, const String&, int, Operation*) {}
bool OpClass::writesDimNstart() const { return false; }
void OpClass::writeExtra(W&, Project&, const String&, int, Operation*) {}
int OpClass::dim() const { return 2; }
void OpClass::simList(int, int*, Operation*, std::vector<SimEntry>&) {}
std::shared_ptr<Props> OpClass::makeProps(int) const { return std::make_shared<Props>(); }
String OpClass::displayName() const { return key(); }
Props* OpClass::propsOf(Operation* op) const
{
    Operation* o = op ? op : current;
    return o ? o->props.get() : nullptr;
}

static void mark(W& w, const char* addr, std::initializer_list<int> v)
{
    w.ts << "[" << addr;
    for (int x : v) w.ts << " " << x;
    w.ts << "]" << endl;
}

const char* keywordName(int id)
{
    return id == 10 ? "KFWRIT" : id == 3 ? "KFREAD" : "ERROR: KEYWORD NOT EXISTS!";
}
void emitSimHeader(W& w, int n, Operation*) { mark(w, "140131c10", {n}); }
void emitDbread(W& w, const String&, int n, int arg) { mark(w, "1401343e0", {n, arg}); }
void emitKfreadUser(W& w, Operation*) { mark(w, "140134680", {}); }
void emitOprnam(W& w, const String& name)
{
    w.ts << "[140134730 " << name << "]" << endl;
}
void emitSimnam(W& w, const String& name)
{
    w.ts << "[1401347e0 " << name << "]" << endl;
}
void emitCursim(W& w, int n, int a, int b) { mark(w, "140134900", {n, a, b}); }
void emitGendb(W& w, const String&, int n, int mode, bool flag5)
{
    mark(w, "140134a10", {n, mode, flag5 ? 1 : 0});
}
void emitSimCtrl(W& w, Project&, const String&, int n, Operation*) { mark(w, "140134bf0", {n}); }
void emitMaterials(W& w, Project&, const String&, int n, Operation*) { mark(w, "140134db0", {n}); }
void emitObjects(W& w, Project&, const String&, int n, Operation*, bool single)
{
    mark(w, "140135220", {n, single ? 1 : 0});
}
void emitObjpos(W& w, Project&, const String&, int n, Operation*) { mark(w, "140135f10", {n}); }
void emitMeshSettings(W& w, Project&, const String&, int n, Operation*) { mark(w, "1401360e0", {n}); }
// Prints the contact block as the real 0x1400ac010 would, so the ICB changes are visible.
void emitContactFlags(W& w, Project&, const String&, int n, Operation* op)
{
    mark(w, "140136040", {n});
    const ContactBlock& c = op->icb;
    if (c.enable)
        for (int k = 0; k < 100; ++k)
            if (c.flag[static_cast<size_t>(k)]) w.ts << "INICTC    \t" << (k + 1) << endl;
    if (c.gen) w.ts << "GENCTC    \t" << "<genVal>" << endl;
}

}  // namespace m2m::mst

namespace m2m {

std::vector<PropNode> defaultPropTree(const String&) { return {}; }
void Props::postLoad(Project&, Ctx&) {}
void BooleanProps::postLoad(Project&, Ctx&) {}
bool startsFromKey(const Operation&) { return p12test::startsFromKeyValue; }
int flowNumber(const Operation&) { return p12test::flowNumberValue; }
String taskDirAbs(const Operation& op)
{
    if (!op.project) return String();
    return qt::convertSeparators(qt::cleanPath(p12test::projectDirValue + qt::kSep + op.taskDir));
}

}  // namespace m2m
