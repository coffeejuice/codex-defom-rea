// Test doubles for the seams the P09 emitters call but other packets own (P02 accessors, P04
// stream/numbers/QString helpers, P05 paths, P08 itemKeyPathClean, P17 equipmentKeyPath). They cover only the inputs the
// P09 tests use (ASCII, absolute paths below the project directory, ordinary doubles) and are
// linked into the standalone P09 test only (tests/p09/CMakeLists.txt), never into m2m_core.
#include <cstdio>
#include <cwctype>
#include <string>

#include "m2m/model.hpp"
#include "m2m/mst/doe_model.hpp"
#include "m2m/mst/emit.hpp"
#include "m2m/mst/stream.hpp"
#include "m2m/qt4path.hpp"

namespace p09test {                                    // defined in test_emit_b.cpp
extern bool g_is2D;                                    // what is2D()/is3D() answer
extern std::vector<m2m::RecoveryEntry> g_recA;         // recovery list A
extern bool g_recANull;
}  // namespace p09test

namespace m2m::mst {
// This standalone double appends bytes immediately; it has no pending text.
Stream& Stream::flush() { return *this; }

Stream& Stream::operator<<(const char* lit) { buf_ += lit; return *this; }
Stream& Stream::operator<<(int v) { buf_ += std::to_string(v); return *this; }
// %.6g: equals Qt 4 SmartNotation precision 6 for the finite values used here (MSVC prints a
// two-digit exponent, as Qt does).
Stream& Stream::operator<<(double v)
{
    char b[64];
    std::snprintf(b, sizeof b, "%.6g", v);
    buf_ += b;
    return *this;
}
Stream& Stream::operator<<(const String& s)
{
    for (char16_t c : s.u16()) buf_ += static_cast<char>(c);
    return *this;
}
Stream& Stream::endl() { buf_ += '\n'; return *this; }

String itemKeyPathClean(const ItemManager& mgr, int id)
{
    Item* it = mgr.find(id);
    return it ? qt::cleanPath(it->keyPath) : String();
}
}  // namespace m2m::mst

namespace m2m::qt {
String number(int v) { return String::latin1(std::to_string(v)); }
std::string toAscii(const String& s)
{
    std::string a;
    for (char16_t c : s.u16()) a += static_cast<char>(c);
    return a;
}
String cleanPath(const String& p)
{
    if (p.isNull() || p.isEmpty()) return p;
    std::u16string u = p.u16();
    for (auto& c : u)
        if (c == u'\\') c = u'/';
    return String(u);
}
String fileName(const String& p)
{
    if (p.isNull()) return String(u"");
    std::u16string u = cleanPath(p).u16();
    auto k = u.rfind(u'/');
    return String(k == std::u16string::npos ? u : u.substr(k + 1));
}
// Replaces the lowest %n (n = 1..9) with the decimal a (width 0).
String arg(const String& fmt, int a, int, int, char16_t)
{
    std::u16string u = fmt.u16();
    size_t best = std::u16string::npos;
    char16_t low = u'9' + 1;
    for (size_t i = 0; i + 1 < u.size(); ++i)
        if (u[i] == u'%' && u[i + 1] >= u'1' && u[i + 1] < low) low = u[i + 1], best = i;
    if (best == std::u16string::npos) return fmt;
    return String(u.substr(0, best) + number(a).u16() + u.substr(best + 2));
}
std::vector<String> split(const String& s, const String& sep, SplitBehavior b)
{
    std::vector<String> out;
    std::u16string u = s.u16(), d = sep.u16();
    size_t start = 0;
    for (;;) {
        size_t k = u.find(d, start);
        std::u16string part = u.substr(start, k == std::u16string::npos ? std::u16string::npos : k - start);
        if (!(b == SplitBehavior::SkipEmptyParts && part.empty())) out.push_back(String(part));
        if (k == std::u16string::npos) break;
        start = k + d.size();
    }
    return out;
}
}  // namespace m2m::qt

namespace m2m::app {
// relToDb for a db in the project directory and a file below it (case-insensitive prefix); null
// stays null as in Qt 4 (relativeFilePath of a relative path returns it).
String relToDb(const String& dbPath, const String& p)
{
    if (p.isNull()) return p;
    std::u16string db = qt::cleanPath(dbPath).u16(), f = qt::cleanPath(p).u16();
    std::u16string dir = db.substr(0, db.rfind(u'/') + 1);
    auto lower = [](std::u16string s) {
        for (auto& c : s) c = static_cast<char16_t>(std::towlower(c));
        return s;
    };
    if (lower(f.substr(0, dir.size())) != lower(dir)) throw std::logic_error("relToDb double: not below");
    return String(f.substr(dir.size()));
}
}  // namespace m2m::app

namespace m2m {
static String taskDir(const Operation& op)
{
    std::u16string u = op.project->path.u16();
    return String(u.substr(0, u.rfind(u'/'))) + String(u"/") + op.taskDir;
}
static String numbered(const Operation& op, const char* prefix, int slot)
{
    char b[16];
    std::snprintf(b, sizeof b, "%05d", slot + 1);
    return taskDir(op) + String(u"/") + String::latin1(prefix) + String::latin1(b) + String(u".KEY");
}
String equipmentKeyPath(const Project& prj, int id) { return mst::itemKeyPathClean(prj.equipment, id); }
bool is2D(const Operation&) { return p09test::g_is2D; }
bool is3D(const Operation&) { return !p09test::g_is2D; }
int objSlots(const Operation& op) { return static_cast<int>(op.objIds.size()); }
int objId(const Operation& op, int slot)
{
    if (slot < 0 || slot >= objSlots(op)) return -99;
    return op.objIds[static_cast<size_t>(slot)];
}
ObjEntry* objEntry(Operation& op, int slot)
{
    if (slot < 0 || slot >= objSlots(op)) return nullptr;
    while (static_cast<int>(op.objs.size()) <= slot) {
        auto e = std::make_unique<ObjEntry>();
        e->number = static_cast<int>(op.objs.size()) + 1;
        e->sv.objNumber = e->number;
        op.objs.push_back(std::move(e));
    }
    return op.objs[static_cast<size_t>(slot)].get();
}
Item* objItem(const Operation& op, int id) { return op.project->objects.find(id); }
String interObjectPath(const Operation& op) { return taskDir(op) + String(u"/InterObject.KEY"); }
String meshSettingsPath(const Operation& op, int slot) { return numbered(op, "MeshSettings", slot); }
String bccDefPath(const Operation& op, int slot) { return numbered(op, "BccDef", slot); }
String objPropertiesPath(const Operation& op, int slot) { return numbered(op, "ObjProperties", slot); }

std::vector<RecoveryEntry>* recoveryListA(Operation&)
{
    return p09test::g_recANull ? nullptr : &p09test::g_recA;
}
int slotNumber(const Operation& op, int slot) { return slot >= 0 && slot < objSlots(op) ? slot + 1 : 0; }
int slotOfObject(const Operation& op, int id)
{
    for (int i = 0; i < objSlots(op); ++i)
        if (op.objIds[static_cast<size_t>(i)] == id) return i;
    return -1;
}
ObjEntry* objEntryById(Operation& op, int id)
{
    int s = slotOfObject(op, id);
    return s < 0 ? nullptr : objEntry(op, s);
}
String objDisplayName(Operation& op, int id)
{
    if (id == -2) return String(u"Read From DB");
    ObjEntry* o = objEntryById(op, id);
    return o ? o->name : String();
}
}  // namespace m2m
