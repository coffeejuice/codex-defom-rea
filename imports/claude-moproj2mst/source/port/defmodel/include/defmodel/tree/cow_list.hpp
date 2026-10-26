// defmodel/tree/cow_list.hpp: the 8-byte copy-on-write list handle of packet p1-14.
//
// At the Phase 1 gate p1-14's own template was folded into p1-05's CowList<T>
// (<defmodel/sdctl/cow.hpp>), which has the same handle, holder ({int32 refs; std::list}), release
// and clear semantics. The binary has one instantiation per element type; these are the
// addresses of the std::pair<int,int> one p1-14 owns (Cls_14042a870::u98):
//
// @src 14042c230  handle ctor: new holder (operator new 0x18, 140431240) = CowList()
// @src 14042c2a0  handle dtor: release(*this) = ~CowList
// @src 140431130  clear: in place when not shared (140431930), else detach to a new holder
// @src 140431240  holder ctor: refs = 1 (140001000), empty list
// @src 14042dfc0  release: drop a reference, delete the holder at 0
// @src 14042e0c0  holder deleting dtor (1404312f0, then free)
// @src 1404312f0  holder dtor: free every node, then the sentinel (std::list dtor)
// @src 140431930  list clear: size = 0, free the nodes, relink the sentinel to itself
#pragma once

#include <defmodel/sdctl/cow.hpp>
