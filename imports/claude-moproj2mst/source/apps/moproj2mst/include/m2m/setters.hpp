// m2m/setters.hpp: small shared setters of the operation model (owner P16, rows moved from P17).
//
// The state-variable setters are folded functions (one body stores at one offset, whoever the
// object is); each keeps its address and gets one typed overload per struct it is called on.
// Callers: ParseKeywordText (s1-01 7.1, dc/open4.c 1290-1500) and the readers of s1-01 5-7.
#pragma once

#include <cstdint>

#include "m2m/model.hpp"

namespace m2m::set {

// @src 0x1412e2450  byte at +0x40: URZ flag (StateVars+0x40)
inline void urzFlag(StateVars& s, std::uint8_t v) { s.urzSet = v; }
// @src 0x1416cab50  double at +0x58: URZ third value, 3D only (StateVars+0x58)
inline void urz2(StateVars& s, double v) { s.urz[2] = v; }
// @src 0x141632700  byte at +0x60: DRZ flag (StateVars+0x60)
inline void drzFlag(StateVars& s, std::uint8_t v) { s.drzSet = v; }
// @src 0x1404ce5b0  double at +0x68: DRZ first value (StateVars+0x68) ...
inline void drz0(StateVars& s, double v) { s.drz[0] = v; }
// ... and the GENCTC value of the contact block (ContactBlock+0x68 = OPI+0xd8)
inline void genctcValue(ContactBlock& c, double v) { c.genVal = v; }
// @src 0x141432bc0  double at +0x38: DAMAGE value (StateVars+0x38)
inline void damage(StateVars& s, double v) { s.damage.v = v; }
// @src 0x141163320  double at +0x78: DRZ third value, 3D only (StateVars+0x78)
inline void drz2(StateVars& s, double v) { s.drz[2] = v; }
// @src 0x140bb1000  double at +0x88: GRAIN value (StateVars+0x88)
inline void grain(StateVars& s, double v) { s.grain.v = v; }
// @src 0x140414430  byte at +0xd0: LMTSTR flag (StateVars+0xd0)
inline void lmtstrFlag(StateVars& s, std::uint8_t v) { s.lmtstr.set = v; }
// @src 0x1407b5170  int at +0x48: lubricant id of the operation (OPI+0x48, <Lubricant Id>)
inline void lubricantId(Operation& op, int id) { op.lubricantId = id; }

// @src 0x140050c80  setClass: OPI+0x120 = cls when it differs; then the operation's engine model
// (0x140054f40(op, 0), a QStandardItemModel) is cleared. The port's engine model is built on
// demand from the class (P15 engineFacts), so only the cached class lookup is dropped.
inline void classKey(Operation& op, const String& cls)
{
    if (op.classKey == cls) return;
    op.classKey = cls;
    op.opClass = nullptr;
}

// @src 0x140070920  registry contains(key): QMap::find(key) != end()
inline bool registryContains(const Project& p, const String& key)
{
    return p.registry.find(key) != p.registry.end();
}

}  // namespace m2m::set
