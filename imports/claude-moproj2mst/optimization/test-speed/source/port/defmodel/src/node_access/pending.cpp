// defmodel/node_access/pending.cpp: callees of 1401d48d0 that are only reached when 1401d2b70
// returns 0, which it never does (docs/re/model/node_access_1401d48d0.md). Stubs until a packet
// needs them.
#include <defmodel/node_access.hpp>

#include <defrt/port.hpp>

namespace def::model {

// 1401d4400: ReadSingleAArrayFromDB (1211 bytes); unreachable from 1401d48d0.
void fun_1401d4400(DB_STEP_DATA*, const std::int32_t*, const char*) { DEF_NOT_PORTED(0x1401d4400); }

// 1401cf930: rebuild the node name array; unreachable (1401d2b70 returns 1).
void fun_1401cf930(DB_STEP_DATA*) { DEF_NOT_PORTED(0x1401cf930); }

// 1401cfc30: rebuild the element name array; unreachable (1401d2b70 returns 1).
void fun_1401cfc30(DB_STEP_DATA*) { DEF_NOT_PORTED(0x1401cfc30); }

}  // namespace def::model
