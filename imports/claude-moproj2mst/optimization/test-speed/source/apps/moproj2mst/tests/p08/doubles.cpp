// Test doubles for the seams the P08 emitters call but other packets own (P02 accessors, P04
// stream/numbers, P05 paths). They cover only the inputs the P08 tests use (ASCII text, absolute
// paths on one drive, integers) and are linked into test_emit_a only, never into m2m_core.
#include <cwctype>
#include <string>

#include "m2m/model.hpp"
#include "m2m/mst/stream.hpp"
#include "m2m/qt4path.hpp"

namespace m2m::mst {
// This standalone double appends bytes immediately; it has no pending text.
Stream& Stream::flush() { return *this; }

Stream& Stream::operator<<(const char* lit) { buf_ += lit; return *this; }
Stream& Stream::operator<<(int v) { buf_ += std::to_string(v); return *this; }
Stream& Stream::operator<<(double) { throw std::logic_error("double not used by P08"); }
Stream& Stream::operator<<(const String& s)
{
    for (char16_t c : s.u16()) buf_ += static_cast<char>(c);
    return *this;
}
Stream& Stream::endl() { buf_ += '\n'; return *this; }
}  // namespace m2m::mst

namespace m2m::qt {
String number(int v) { return String::latin1(std::to_string(v)); }
int toInt(const String& s, bool* ok)
{
    std::string a = toAscii(s);
    if (ok) *ok = true;
    return std::stoi(a);
}
std::string toAscii(const String& s)
{
    std::string a;
    for (char16_t c : s.u16()) a += static_cast<char>(c);
    return a;
}
// Inputs are already clean except for '\\' separators.
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
    std::u16string u = cleanPath(p).u16();
    auto k = u.rfind(u'/');
    return String(k == std::u16string::npos ? u : u.substr(k + 1));
}
}  // namespace m2m::qt

namespace m2m::app {
// relToDb for a db in the project directory and a file below it (case-insensitive prefix).
String relToDb(const String& dbPath, const String& p)
{
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
static String projDir(const Operation& op)
{
    std::u16string u = op.project->path.u16();
    return String(u.substr(0, u.rfind(u'/')));
}
int opDim(const Operation&) { return 3; }
int startStep(const Operation& op)
{
    if (!op.nstartGiven) return 0;
    int v = op.startStepNo;
    return v < 0 ? -1 - v : v;
}
String simCtrlPath(const Operation& op)
{
    return projDir(op) + String(u"/") + op.taskDir + String(u"/SimCtrl.KEY");
}
String restartKeyPath(const Operation& op, bool)
{
    return projDir(op) + String(u"/") + op.taskDir + String(u"/") + op.restartKey;
}
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
        op.objs.push_back(std::move(e));
    }
    return op.objs[static_cast<size_t>(slot)].get();
}
}  // namespace m2m
