// List of 0xe8-byte records (defmodel/linked/record_e8_list.hpp).
#include <defmodel/linked/record_e8_list.hpp>

#include <cstddef>

#include <defmodel/port.hpp>

namespace def::model::linked {

// @src 140258580  ctor -> 1402585b0 (ported by kw-g2geo: WPAXIS records)
// @src 1402585b0  id, type = 0; 3-vectors +0x08 and +0x20 = 0; +0x38 = 0; +0x40..+0x58 = 0;
//                 +0x60 = 0; 3-vectors +0x68, +0x80, +0x98 = 0; +0xb0 = 0; +0xb8 = 360.0
//                 (DAT_14051f210); +0xc0 = 0; ints +0xc8, +0xcc = 0; +0xd0, +0xd8, +0xe0 = 0
Cls_140258580::Cls_140258580()
{
    auto* b = reinterpret_cast<std::byte*>(this);
    auto i32 = [b](std::size_t o) -> std::int32_t& { return *reinterpret_cast<std::int32_t*>(b + o); };
    auto f64 = [b](std::size_t o) -> double& { return *reinterpret_cast<double*>(b + o); };
    auto u64 = [b](std::size_t o) -> std::uint64_t& { return *reinterpret_cast<std::uint64_t*>(b + o); };
    i32(0) = 0;
    i32(4) = 0;
    for (int k = 0; k < 3; ++k) {
        u64(0x08 + k * 8) = 0;
        u64(0x20 + k * 8) = 0;
    }
    i32(0x38) = 0;
    u64(0x40) = 0;
    u64(0x48) = 0;
    u64(0x50) = 0;
    u64(0x58) = 0;
    i32(0x60) = 0;
    for (int k = 0; k < 3; ++k) {
        u64(0x68 + k * 8) = 0;
        u64(0x80 + k * 8) = 0;
        u64(0x98 + k * 8) = 0;
    }
    i32(0xb0) = 0;
    f64(0xb8) = 360.0;  // DAT_14051f210 (0x4076800000000000)
    u64(0xc0) = 0;
    i32(0xc8) = 0;
    i32(0xcc) = 0;
    u64(0xd0) = 0;
    u64(0xd8) = 0;
    u64(0xe0) = 0;
}

// @src 140258990
// @src 14025bf40
Cls_140258580::~Cls_140258580() = default;

// @src 1402598a0
Cls_140258580List::~Cls_140258580List() { clear(); }

// @src 140259c70
void Cls_140258580List::clear()
{
    Cls_140258580* node = head;
    while (node != nullptr) {
        Cls_140258580* following = node->next;
        delete node; // 14025bf40(node, 1)
        node = following;
    }
    tail = nullptr;
    head = nullptr;
    count = 0;
}

} // namespace def::model::linked
