// defcmd/dxf_uic2c_base.hpp: the Uic2CObject base class as packet p8-04 needs it.
//
// The base class belongs to p8-03 (defcmd/uic2c_object.hpp, origin/claude/p8-03 @389e463). When
// that header is present it is used. Otherwise this file declares a provisional class with the
// same API and layout (vptr; +0x08 int32 = 0; +0x10 char* name, malloc(0x80); size 0x18; slot 0
// the virtual dtor, slot 1 the pure class_name() const), with inline bodies, so that the DXF
// reader builds and links on its own.
#pragma once

#if __has_include(<defcmd/uic2c_object.hpp>)
#include <defcmd/uic2c_object.hpp>
#else
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>

namespace def::cmd {

class Uic2CObject {
public:
    // provisional copy of p8-03's ctor (140454900)
    explicit Uic2CObject(const char* nm = nullptr) : u08(0), pad_0c{}, name(nullptr)
    {
        name = static_cast<char*>(std::malloc(0x80));
        name[0] = '\0';
        set_name(nm);
    }
    Uic2CObject(const Uic2CObject&) = delete;
    Uic2CObject& operator=(const Uic2CObject&) = delete;
    // provisional copy of p8-03's dtor (140454980)
    virtual ~Uic2CObject() { std::free(name); }
    virtual const char* class_name() const = 0;
    // provisional copy of p8-03's set_name (140454a90)
    void set_name(const char* nm)
    {
        if (nm == nullptr || std::strlen(nm) == 0) {
            name[0] = '\0';
            return;
        }
        const std::size_t n = std::strlen(nm);
        if (0x7f < n) {
            std::free(name);
            name = static_cast<char*>(std::malloc(n + 1));
        }
        std::memcpy(name, nm, n + 1);
    }

    std::int32_t u08;
    std::byte pad_0c[4];
    char* name;
};
static_assert(sizeof(Uic2CObject) == 0x18);

} // namespace def::cmd
#define DEF_P804_PROVISIONAL_UIC2C_BASE 1
#endif
