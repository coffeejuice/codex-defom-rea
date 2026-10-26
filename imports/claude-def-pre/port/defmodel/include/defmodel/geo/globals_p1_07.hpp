// Globals of the geometry packet (p1-07).
#pragma once

#include <cstdint>

#include <defmodel/sdctl/cow.hpp>

namespace def::model {

// @data 1406fed38  copy-on-write vector handle (element type not recovered; 8-byte assumed).
// Built by the static initialiser 140503b80 (p1-02 records its arguments in
// defmodel/options/static_objects.hpp, kGeoHolderInit) through FUN_140275b00; the atexit
// destructor 140504820 releases it.
// @src 140503b80  static initialiser: FUN_140275b00(&DAT_1406fed38)
// @src 140275b00  CowVector handle ctor: new 0x20, body ctor 140275b70 (0 if new fails)
// @src 140275b70  CowVector body ctor: refs 1 (140001000), three zero pointers
inline CowVector<std::int64_t> dat_1406fed38;

} // namespace def::model
