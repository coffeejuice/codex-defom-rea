// Knobs of the P13 test doubles (doubles.cpp).
#pragma once

#include <map>
#include <vector>

namespace p13test {
extern std::map<int, std::vector<int>> members;   // groupMembers(sub, cycleId)
extern std::map<int, int> flowNumbers;            // flowNumber(op) by op id
extern std::map<int, bool> chainHeads;            // isChainHead(op) by op id
extern std::vector<int> subOps;                   // flowOperations(sub) ids
}  // namespace p13test
