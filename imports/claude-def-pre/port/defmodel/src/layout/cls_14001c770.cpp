// Cls_14001c770: main's session object (packet p5-01 ports the ctor).
// Layout: defmodel/layout/cls_14001c770.hpp, docs/re/model-layout.md.
#include <defmodel/layout/cls_14001c770.hpp>

#include <cstring>

#include <defmodel/phase/phase_ops.hpp>

namespace def::model {

// @src 1401d8000  m->companion = c, then force it to the other dimension (1401d8050)
static void set_companion(DB_STEP_DATA* m, DB_STEP_DATA* c)
{
    m->companion = c;
    if (m->companion != nullptr) sync_companion_dimension(m);
}

// @src 14001c770  ctor: the menu records {lo, hi, ..} and defaults, both steps built with dim 3,
// each step linked to its DB object, step_b set as step_a's companion (1401d8000)
Cls_14001c770::Cls_14001c770() : step_a(3), db_a(), step_b(3), db_b()
{
    // The binary writes the menu part before it constructs the members; nothing in the member
    // constructors reads it, so the order here is the same in effect.
    std::memset(this, 0, 0x190);
    u000 = 0;
    u004 = 0;
    u008 = 1;
    u00c = 10;
    u020 = 0;
    u048 = 1;
    u04c = 5;
    u060 = 1;
    u064 = 0x11;
    u078 = 1;
    u07c = 10;
    u090 = 1;
    u094 = 5;
    u030 = 1;
    u034 = 5;
    u0a8 = 1;
    u0ac = 9;
    u0c0 = 1;
    u0c4 = 4;
    u150 = 1;
    u0d8 = 0;
    u118 = 0;
    u130 = 0;
    u0f0 = 0;
    u0e0 = 0;
    u120 = 0;
    u138 = 0;
    u0f8 = 0;
    u0e8 = 0;
    u128 = 0;
    u140 = 1.0;
    u100 = -1.0;
    u108 = 1;
    u10c = 2;
    std::uint64_t bits = 0x3f1a36e2eb1c432dULL; // 1e-4 as stored
    std::memcpy(&u158, &bits, 8);
    std::memcpy(&u110, &bits, 8);
    u148 = 0;
    u160 = 1;
    u164 = 1000;
    u168 = 0;
    u16c = 0;
    u170 = 1;
    u174 = 9;
    u188 = 1;
    u18c = 10;
    u000 = 1;
    u004 = 9;
    step_a.db = &db_a;
    step_b.db = &db_b;
    set_companion(&step_a, &step_b);
}

// @src 14001c9f0  dtor (no binary function: inlined at the end of main FUN_14001c9f0, members
// destroyed in reverse order: db_b, step_b, db_a, step_a)
Cls_14001c770::~Cls_14001c770() = default;

} // namespace def::model
