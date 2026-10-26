// Test doubles for the P15 tier-1 tests: every seam the FormingExpress writer calls, so it is tested
// alone. Emitters write a marker line "[NAME args]"; the stream doubles keep the bytes, with doubles
// printed as %.6g (enough for the values the tests use; P04 owns the real formatting).
#include <cstdio>
#include <string>

#include "m2m/model.hpp"
#include "m2m/mst/emit.hpp"
#include "m2m/mst/simcount.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/props.hpp"
#include "m2m/qt4path.hpp"

#include "doubles.hpp"

namespace p15test {
int flowNumberValue = 4;
int nextSimValue = 0;
int nextSimArg = -1;
bool is2DValue = false;
bool unitOk = true;
std::map<std::u16string, double> unitValues;
}  // namespace p15test

namespace {
std::string ascii(const m2m::qt::String& s)
{
    std::string out;
    for (char16_t c : s.u16()) out += c < 0x100 ? static_cast<char>(c) : '?';
    return out;
}
void mark(m2m::mst::W& w, const std::string& text) { w.ts << ("[" + text + "]").c_str() << m2m::mst::endl; }
std::string num(int v) { return std::to_string(v); }
}  // namespace

namespace m2m::qt {
double toDouble(const String& s, bool* ok)
{
    if (ok) *ok = true;
    return std::stod(s.utf8());
}
int toInt(const String& s, bool* ok)
{
    if (ok) *ok = true;
    return std::stoi(s.utf8());
}
String number(int v)
{
    std::string a = std::to_string(v);
    return String(std::u16string(a.begin(), a.end()));
}
String arg(const String& fmt, int a, int, int, char16_t)
{
    std::u16string s = fmt.u16();
    auto i = s.find(u"%1");
    std::string n = std::to_string(a);
    s.replace(i, 2, std::u16string(n.begin(), n.end()));
    return String(s);
}
// the Express vt+0xc8 body (MDT L4) uses it; none of these tests reaches it
bool contains(const String& s, const String& what, Case)
{
    return s.u16().find(what.u16()) != std::u16string::npos;
}
}  // namespace m2m::qt

namespace m2m::app {
String relToDb(const String&, const String& p) { return qt::L("REL:") + p; }
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
    buf_ += std::to_string(v);
    return *this;
}
Stream& Stream::operator<<(double v)
{
    char b[64];
    std::snprintf(b, sizeof b, "%.6g", v);
    buf_ += b;
    return *this;
}
Stream& Stream::operator<<(const String& s)
{
    buf_ += ascii(s);
    return *this;
}
Stream& Stream::endl()
{
    buf_ += "\n";
    return *this;
}

// OpClass slots that are not P15's
void OpClass::writeMst(W& w, Project&, const String&, int* sim, Operation*, int, int)
{
    *sim = *sim + 1;
    mark(w, "BASE " + num(*sim));
}
void OpClass::writeSetup(W&, Project&, const String&, int, Operation*) {}
bool OpClass::writesDimNstart() const { return false; }
void OpClass::writeExtra(W&, Project&, const String&, int, Operation*) {}
int OpClass::dim() const { return 3; }
void OpClass::simList(int, int*, Operation*, std::vector<SimEntry>&) {}
std::shared_ptr<Props> OpClass::makeProps(int) const { return nullptr; }
String OpClass::displayName() const { return key(); }
Props* OpClass::propsOf(Operation* op) const
{
    Operation* o = op ? op : current;
    return o ? o->props.get() : nullptr;
}

void emitSimHeader(W& w, int n, Operation*) { mark(w, "SIMHDR " + num(n)); }
void emitDimNstart(W& w, const String&, int n, Operation*, int a) { mark(w, "DIMNSTART " + num(n) + " " + num(a)); }
void emitDbread(W& w, const String&, int n, int a) { mark(w, "DBREAD " + num(n) + " " + num(a)); }
void emitKfreadRestart(W& w, const String&, Operation*) { mark(w, "KFREAD_RESTART"); }
void emitKfreadUser(W& w, Operation*) { mark(w, "KFREAD_USER"); }
void emitOprnam(W& w, const String& s) { mark(w, "OPRNAM " + ascii(s)); }
void emitSimnam(W& w, const String& s) { mark(w, "SIMNAM " + ascii(s)); }
void emitCursim(W& w, int n, int a, int b) { mark(w, "CURSIM " + num(n) + " " + num(a) + " " + num(b)); }
void emitGendb(W& w, const String&, int n, int mode, bool f) { mark(w, "GENDB " + num(n) + " " + num(mode) + (f ? " *" : "")); }
void emitSimCtrl(W& w, Project&, const String&, int n, Operation*) { mark(w, "SIMCTRL " + num(n)); }
void emitMaterials(W& w, Project&, const String&, int n, Operation*) { mark(w, "MATERIALS " + num(n)); }
void emitObjects(W& w, Project&, const String&, int n, Operation*, bool s) { mark(w, "OBJECTS " + num(n) + " " + num(s)); }
void emitContact(W& w, Project&, const String&, int n, Operation*) { mark(w, "CNTACT " + num(n)); }
void emitObjpos(W& w, Project&, const String&, int n, Operation*) { mark(w, "OBJPOS " + num(n)); }
void emitMeshSettings(W& w, Project&, const String&, int n, Operation*) { mark(w, "MESHSET " + num(n)); }
void emitObjProperties(W& w, Project&, const String&, int n, Operation*) { mark(w, "OBJPROPS " + num(n)); }
const char* keywordName(int id)
{
    switch (id) {
    case 0x30: return "DSMAX";
    case 0x32: return "DTMAX";
    case 0x33: return "DTTYPE";
    default: return "ERROR: KEYWORD NOT EXISTS!";
    }
}
void simListFormingExpress(OpClass&, int, int* counter, Operation*, std::vector<SimEntry>&)
{
    *counter = *counter + 100;
}
int nextSim(Project&, SubProject&, int ord)
{
    p15test::nextSimArg = ord;
    return p15test::nextSimValue;
}
}  // namespace m2m::mst

namespace m2m {
void Props::postLoad(Project&, Ctx&) {}
void ExpressProps::postLoad(Project&, Ctx&) {}
mst::OpClass* opBase(const Operation& op) { return op.opClass; }
String equipmentKeyPath(const Project& prj, int id)
{
    const Item* it = prj.equipment.find(id);
    return it ? it->keyPath : String();
}
bool is2D(const Operation&) { return p15test::is2DValue; }
int flowNumber(const Operation&) { return p15test::flowNumberValue; }
SubProject* activeSubProject(const Project& p)
{
    return p.subs.empty() ? nullptr : p.subs[static_cast<size_t>(p.activeSub)].get();
}
ObjEntry* objEntry(Operation& op, int slot)
{
    if (slot < 0 || slot >= static_cast<int>(op.objIds.size())) return nullptr;
    while (static_cast<int>(op.objs.size()) <= slot) op.objs.push_back(std::make_unique<ObjEntry>());
    return op.objs[static_cast<size_t>(slot)].get();
}
double evalUnitString(const String& text, int, int, bool* ok)
{
    auto it = p15test::unitValues.find(text.u16());
    if (ok) *ok = p15test::unitOk && it != p15test::unitValues.end();
    return it == p15test::unitValues.end() ? 0.0 : it->second;
}
}  // namespace m2m
