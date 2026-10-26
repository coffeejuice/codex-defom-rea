// defcmd/uic2c_object.hpp: Uic2CObject (RTTI .?AVUic2CObject@@), base of the 2D CAD file readers
// Uic2CObjectIgesFile (defcmd/iges.hpp, packet p8-03) and Uic2CObjectDxfFile (defcmd/dxf.hpp,
// packet p8-04). Owner: packet p8-03.
//
//   vtable   0x14056aee8 (2 slots, binary order):
//              slot 0  scalar deleting dtor   base 140454b50 (dtor 140454980, then free if bit 0)
//                                             Iges 14040cda0, Dxf 140409890
//              slot 1  class name             base _purecall (1404852f0, pure virtual)
//                                             Iges 14040cd90 "Uic2CObjectIgesFile",
//                                             Dxf 140409880
//   ctor     140454900(this, name): vptr, u08 = 0, name buffer = malloc(0x80) (operator new
//            1404797d8), name[0] = 0, set_name(name) (140454a90). Every caller passes name = 0.
//   dtor     140454980: vptr = base vtable, free(name buffer).
//            These members are the implementation; p8-01's fun_140454900/980/a90 forward here.
//   size     0x18 (derived members start at 0x18: Iges/Dxf ctors write +0x18, +0x1c, +0x20).
//
// The scalar deleting dtors are what the compiler generates from the virtual destructors, so
// slot 0 is the C++ `virtual ~Uic2CObject()`; the class-name slot is `class_name()`.
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/layout_check.hpp>

namespace def::cmd {

class Uic2CObject {
public:
    // @src 140454900  ctor (name may be null: empty name)
    explicit Uic2CObject(const char* name = nullptr);
    Uic2CObject(const Uic2CObject&) = delete;
    Uic2CObject& operator=(const Uic2CObject&) = delete;

    // @src 140454b50  slot 0: scalar deleting dtor (body 140454980)
    virtual ~Uic2CObject();
    // slot 1: class name (pure virtual in the base vtable: _purecall)
    virtual const char* class_name() const = 0;

    // @src 140454a90  copy name into the buffer (reallocated to strlen+1 when longer than
    // 0x7f); null or empty -> empty name
    void set_name(const char* name);

    // vptr at 0x00
    std::int32_t u08;        // 0x08 ctor =0 (no other writer found in the reader classes)
    std::byte pad_0c[4];     // 0x0c
    char* name;              // 0x10 malloc'd, at least 0x80 bytes
};

static_assert(sizeof(Uic2CObject) == 0x18);
DEF_LAYOUT_OFFSET(Uic2CObject, u08, 0x08);
DEF_LAYOUT_OFFSET(Uic2CObject, name, 0x10);

} // namespace def::cmd
