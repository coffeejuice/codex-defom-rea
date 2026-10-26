#pragma once
// The non-table static initialisers of packet p1-02 (0x140503820-0x140503b80). The licence
// initialisers between them (0x140503710-0x140503800) are not ported (docs/re/status/p1-02.tsv).

#include <cstdint>

namespace def::model::options {

// Objects holding one pointer, built by FUN_1400011d0(this, 0) (`*this = arg`); no atexit.
// FUN_1400011e0 reads dat_1406e2e10. Type not recovered yet, so the port keeps the pointer.

// @src 140503820  static initialiser: FUN_1400011d0(&DAT_1406e2e10, 0)
// @data 1406e2e10  one-pointer object
inline void* dat_1406e2e10 = nullptr;

// @src 140503860  static initialiser: FUN_1400011d0(&DAT_1406e2e08, 0)
// @data 1406e2e08  one-pointer object
inline void* dat_1406e2e08 = nullptr;

// Recovered arguments of the initialisers whose object type belongs to another packet. The
// owning packet declares the global and constructs it from this record (status p1-02.tsv: todo).
struct StaticObjectInit {
    std::uint64_t init;   // the initialiser
    std::uint64_t data;   // the global it constructs
    std::uint64_t ctor;   // constructor it calls
    const char* name;     // second argument (copied into the object by FUN_140484f90, strcpy)
    bool flag;            // byte stored at +0x539c (c6 44 24 20 imm8, passed by address)
    std::uint64_t dtor;   // registered with atexit
};

// Step timers: FUN_14019cc60(this, name, &flag) = strcpy(this, name); this[0x539c] = flag;
// FUN_140199f30 (this[0x5398] = 0). Class owned by p1-05 (range 0x1401999c0-0x1401b0fff); used
// as FUN_140199dc0(&timer, "Start") / FUN_140199df0(&timer, "<checkpoint>", sub) in the DB
// readers (e.g. FUN_1401dc3f0 with DAT_1406f2e60 and DAT_1406f8200).
inline constexpr StaticObjectInit kStepTimerInits[] = {
    {0x140503880, 0x1406E2E20, 0x14019CC60, "LLOBJ_DATA::ReadElemIntegerInfoFromDB()", false, 0x140504780},
    {0x140503980, 0x1406F2E60, 0x14019CC60, "_DB_STEP_DATA::ReadRecordFromDB()", false, 0x1405047A0},
    {0x1405039C0, 0x1406F8200, 0x14019CC60, "_DB_STEP_DATA::ReadArraysFromDB", true, 0x1405047C0},
    {0x140503A00, 0x1406EDAC0, 0x14019CC60, "_DB_STEP_DATA::ReadObjStateVariableFromDB", true, 0x1405047E0},
    {0x140503A40, 0x1406E8720, 0x14019CC60, "_DB_STEP_DATA::ReadObjNodeStateVariableFromDB()", false, 0x140504800},
};

// FUN_140275b00(this): *this = new (0x20 bytes) FUN_140275b70(...), or 0 if new fails. Class
// owned by p1-07 (range 0x140275b00-0x140280760).
inline constexpr StaticObjectInit kGeoHolderInit = {0x140503B80, 0x1406FED38, 0x140275B00, nullptr, false,
                                                    0x140504820};

}  // namespace def::model::options
