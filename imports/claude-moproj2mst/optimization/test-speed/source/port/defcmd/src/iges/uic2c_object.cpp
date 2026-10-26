// Uic2CObject base class and Uic2CObjectIgesFile (packet p8-03). Layout: defcmd/uic2c_object.hpp,
// defcmd/iges.hpp. Ownership (coordinator): these C++ members are the implementation of the
// ctor/dtor bodies 140454900, 140454980, 140454a90, 1404099d0, 140409a50, 140409a90; p8-01's
// fun_ wrappers of the same addresses forward here when this header is present.
#define _CRT_SECURE_NO_WARNINGS
#include <defcmd/iges.hpp>
#include <defcmd/uic2c_object.hpp>

#include <cstdlib>
#include <cstring>

namespace def::cmd {

// @src 140454900  ctor: vptr, u08 = 0, name = malloc(0x80) (1404797d8), name[0] = 0, set_name
Uic2CObject::Uic2CObject(const char* nm) : u08(0), pad_0c{}, name(nullptr)
{
    name = static_cast<char*>(std::malloc(0x80));
    name[0] = '\0';
    set_name(nm);
}

// @src 140454980  dtor body: free(name)
// @src 140454b50  slot 0: scalar deleting dtor (the compiler's, from this virtual dtor)
Uic2CObject::~Uic2CObject() { std::free(name); }

// @src 140454a90  null/empty -> ""; longer than 0x7f -> free + malloc(len+1); strcpy
void Uic2CObject::set_name(const char* nm)
{
    if (nm == nullptr) {
        name[0] = '\0';
        return;
    }
    const int n = static_cast<int>(std::strlen(nm));
    if (n < 1) {
        name[0] = '\0';
        return;
    }
    if (0x7f < n) {
        std::free(name);
        name = static_cast<char*>(std::malloc(static_cast<std::size_t>(n + 1)));
    }
    std::strcpy(name, nm);
}

} // namespace def::cmd

namespace def::cmd::iges {

// @src 1404099d0  ctor: base ctor, vptr, u18 = u1c = 0, u20 = 0, then reset() (140409a50)
Uic2CObjectIgesFile::Uic2CObjectIgesFile(const char* nm) : Uic2CObject(nm), u18(0), u1c(0), u20(0.0) { reset(); }

// @src 140409a90  dtor body (vptr, base dtor 140454980)
// @src 14040cda0  slot 0: scalar deleting dtor (the compiler's, from this virtual dtor)
Uic2CObjectIgesFile::~Uic2CObjectIgesFile() = default;

// @src 14040cd90  slot 1
const char* Uic2CObjectIgesFile::class_name() const { return "Uic2CObjectIgesFile"; }

// @src 140409a50  u18 = u1c = 0, u20 = 0
void Uic2CObjectIgesFile::reset()
{
    u18 = 0;
    u1c = 0;
    u20 = 0.0;
}

} // namespace def::cmd::iges
