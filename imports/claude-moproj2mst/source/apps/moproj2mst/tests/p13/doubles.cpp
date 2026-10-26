// Test doubles for the P13 tier-1 test: the seams the P13 writers call, so the writers are tested
// alone, before (and independent of) P02/P03/P04/P05/P08/P09/P10. Each emitter writes a marker
// line "[<address> args]"; the stream double keeps the bytes. Only this test binary links these
// definitions (not m2m_core).
#include <initializer_list>
#include <map>
#include <cstdlib>
#include <string>
#include <vector>

#include "m2m/model.hpp"
#include "m2m/mst/emit.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/props.hpp"
#include "m2m/qt4path.hpp"

#include "doubles.hpp"
#include "ops/p13.hpp"

namespace p13test {
std::map<int, std::vector<int>> members;
std::map<int, int> flowNumbers;
std::map<int, bool> chainHeads;
std::vector<int> subOps;
}  // namespace p13test

namespace m2m::qt {

std::string toAscii(const String& s)
{
    std::string out;
    for (char16_t c : s.u16()) out += c < 0x100 ? static_cast<char>(c) : '?';
    return out;
}
std::string formatInt(int v) { return std::to_string(v); }
std::string encodeLocale(const String& s, unsigned) { return toAscii(s); }
String number(int v) { return String::latin1(std::to_string(v)); }
int toInt(const String& s, bool* ok) { if (ok) *ok = true; return std::atoi(s.utf8().c_str()); }   // for props_base.cpp (P15)
double toDouble(const String& s, bool* ok) { if (ok) *ok = true; return std::atof(s.utf8().c_str()); }

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

}  // namespace m2m::qt

namespace m2m::app {
String relToDb(const String&, const String& p) { return String(u"REL(") + p + String(u")"); }
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
void OpClass::simList(int, int*, Operation*, std::vector<SimEntry>& out) { out.push_back({-1, -1}); }
std::shared_ptr<Props> OpClass::makeProps(int) const { return std::make_shared<Props>(); }
String OpClass::displayName() const { return key(); }
Props* OpClass::propsOf(Operation* op) const
{
    Operation* o = op ? op : current;
    return o ? o->props.get() : nullptr;
}

const char* keywordName(int id)
{
    return id == 10 ? "KFWRIT" : id == 3 ? "KFREAD" : "ERROR: KEYWORD NOT EXISTS!";
}

static void mark(W& w, const char* addr, std::initializer_list<int> v)
{
    w.ts << "[" << addr;
    for (int x : v) w.ts << " " << x;
    w.ts << "]" << endl;
}

void emitSimHeader(W& w, int n, Operation*) { mark(w, "140131c10", {n}); }
void emitDbread(W& w, const String&, int n, int arg) { mark(w, "1401343e0", {n, arg}); }
void emitKfreadUser(W& w, Operation*) { mark(w, "140134680", {}); }
void emitOprnam(W& w, const String& name) { w.ts << "[140134730 " << name << "]" << endl; }
void emitSimnam(W& w, const String& name) { w.ts << "[1401347e0 " << name << "]" << endl; }
void emitCursim(W& w, int n, int a, int b) { mark(w, "140134900", {n, a, b}); }
void emitGendb(W& w, const String&, int n, int mode, bool flag5) { mark(w, "140134a10", {n, mode, flag5 ? 1 : 0}); }
void emitSimCtrl(W& w, Project&, const String&, int n, Operation*) { mark(w, "140134bf0", {n}); }
void emitMaterials(W& w, Project&, const String&, int n, Operation*) { mark(w, "140134db0", {n}); }
void emitObjects(W& w, Project&, const String&, int n, Operation*, bool single) { mark(w, "140135220", {n, single ? 1 : 0}); }
void emitContact(W& w, Project&, const String&, int n, Operation*) { mark(w, "140135da0", {n}); }
void emitObjpos(W& w, Project&, const String&, int n, Operation*) { mark(w, "140135f10", {n}); }
void emitContactFlags(W& w, Project&, const String&, int n, Operation*) { mark(w, "140136040", {n}); }
void emitStateVars(W& w, Project&, const String&, int n, Operation*) { mark(w, "140136090", {n}); }
void emitMeshSettings(W& w, Project&, const String&, int n, Operation*) { mark(w, "1401360e0", {n}); }
void emitDoeGeometry(W& w, Operation*) { mark(w, "140132a40", {}); }
void emitDoeVariables(W& w, Operation*) { mark(w, "1401322d0", {}); }

#if __has_include("m2m/mst/simcount.hpp")
// P14's sim-list bodies: markers (only the delegation is tested here).
void simListNone(OpClass&, int, int*, Operation*, std::vector<SimEntry>&) {}
void simListCycle(OpClass&, int pos, int*, Operation*, std::vector<SimEntry>& out) { out.push_back({pos, -13}); }
void simListDistortion(OpClass&, int pos, int*, Operation*, std::vector<SimEntry>& out) { out.push_back({pos, -14}); }
#endif

}  // namespace m2m::mst

namespace m2m {

std::vector<PropNode> defaultPropTree(const String&) { return {}; }
void Props::postLoad(Project&, Ctx&) {}
void CycleProps::postLoad(Project&, Ctx&) {}
void DistortionProps::postLoad(Project&, Ctx&) {}

SubProject* activeSubProject(const Project& p)
{
    if (p.activeSub < 0 || p.activeSub >= static_cast<int>(p.subs.size())) return nullptr;
    return p.subs[static_cast<size_t>(p.activeSub)].get();
}
std::vector<int> groupMembers(const SubProject&, int groupId) { return p13test::members[groupId]; }
Operation* operationById(const Project& p, int id)
{
    for (const auto& op : p.operations)
        if (op->id == id) return op.get();
    return nullptr;
}
mst::OpClass* opBase(const Operation& op) { return op.opClass; }
int flowNumber(const Operation& op) { return p13test::flowNumbers[op.id]; }
bool isChainHead(const Operation& op) { return p13test::chainHeads[op.id]; }
bool is2D(const Operation&) { return true; }
bool hasEngineModel(const Operation&) { return false; }
EngineFacts engineFacts(const Operation&) { return {}; }
int opDim(const Operation& op) { return op.id == 2 ? 2 : 4; }
std::vector<Operation*> flowOperations(const SubProject& sub)
{
    std::vector<Operation*> out;
    if (!sub.project) return out;
    for (int id : p13test::subOps) out.push_back(operationById(*sub.project, id));
    return out;
}
int objId(const Operation& op, int slot)
{
    if (slot < 0 || slot >= static_cast<int>(op.objIds.size())) return -99;
    return op.objIds[static_cast<size_t>(slot)];
}
Item* objItem(const Operation& op, int id) { return op.project ? op.project->objects.find(id) : nullptr; }

}  // namespace m2m

