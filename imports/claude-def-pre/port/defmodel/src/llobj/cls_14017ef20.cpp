// Cls_14017ef20: element state-variable set (defmodel/llobj/cls_14017ef20.hpp).
#include <defmodel/llobj/cls_14017ef20.hpp>

#include <cstdlib>
#include <cstring>

#include <defmodel/llobj/ext.hpp>
#include <defmodel/llobj/realloc_zero_tail.hpp>

namespace def::model {

namespace {
// (std::int64_t)(int32 product) << shift, as the original's size arguments.
std::size_t bytes(std::int32_t a, std::int32_t b, int shift)
{
    const auto p = static_cast<std::int32_t>(static_cast<std::uint32_t>(a) * static_cast<std::uint32_t>(b));
    return static_cast<std::size_t>(static_cast<std::int64_t>(p) << shift);
}
} // namespace

// 14017f2f0: operator new(0x38) and ctor per entry, in this order.
const Cls_14017ef20::Reg Cls_14017ef20::kRegs[28] = {
    {0x14032a990, 0x01, 0x10}, {0x14032a990, 0x02, 0x18}, {0x14032a990, 0x03, 0x20},
    {0x14032a990, 0x04, 0x28}, {0x14032a990, 0x05, 0x30}, {0x14032a990, 0x06, 0x38},
    {0x14032a990, 0x08, 0x40}, {0x14032a990, 0x09, 0x48}, {0x14032a990, 0x0a, 0x50},
    {0x14032a990, 0x0b, 0x58}, {0x14032a990, 0x0c, 0x60}, {0x14032a990, 0x0d, 0x68},
    {0x14032a990, 0x0e, 0x70}, {0x14032a990, 0x0f, 0x78}, {0x14032a990, 0x10, 0x80},
    {0x14032a990, 0x18, 0x88}, {0x14032a990, 0x19, 0x90}, {0x14032a990, 0x1b, 0x98},
    {0x14032a990, 0x1c, 0xa0}, {0x14032a990, 0x1a, 0xa8}, {0x14032a990, 0x1d, 0xb0},
    {0x14032a990, 0x22, 0xb8}, {0x14032a990, 0x23, 0xc0}, {0x14032a990, 0x11, 0xc8},
    {0x14032aa20, 0x1e, 0xd0}, {0x14032aa20, 0x1f, 0xd8}, {0x14032aa20, 0x20, 0xe0},
    {0x14032aa20, 0x21, 0xe8},
};

// @src 14017ef20  ctor
Cls_14017ef20::Cls_14017ef20()
{
    llobj::ext::f140329c70(this);
    u130 = nullptr;
    u138 = 3;
    init();
}

// @src 14017ef80  dtor
Cls_14017ef20::~Cls_14017ef20()
{
    free_arrays();
    llobj::ext::f140329cb0(this);
}

// @src 14017f060  zero the arrays, u128 = 300, register the variables
void Cls_14017ef20::init()
{
    u008 = nullptr;
    for (auto& p : u0f0)
        p = nullptr;
    u128 = 300;
    n = 0;
    u13c = 0;
    register_vars();
}

// @src 14017f130  free the arrays, then reset each variable (140329dc0)
void Cls_14017ef20::free_arrays()
{
    if (u008 != nullptr)
        std::free(u008);
    u008 = nullptr;
    for (auto& p : u0f0) {
        if (p != nullptr)
            std::free(p);
        p = nullptr;
    }
    n = 0;
    u13c = 0;
    llobj::ext::f140329dc0(this);
}

// @src 14017f2f0  delete the old variables (140329d10), create and append the 28 variables
void Cls_14017ef20::register_vars()
{
    llobj::ext::f140329d10(this);
    for (const Reg& r : kRegs) {
        void* field = reinterpret_cast<std::byte*>(this) + r.slot;
        void* sv = llobj::ext::new_statevar(r.ctor, 0x38, r.id, field);
        llobj::ext::f140329e30(this, sv);
    }
}

// @src 140180150  realloc the arrays for n elements, then size each variable (140329e60, -1)
void Cls_14017ef20::alloc(std::int32_t n_, const std::int32_t* dims)
{
    if (0 < n_) {
        u008 = llobj::realloc_zero_tail(u008, bytes(n_, dims[0], 3));  // realloc (zeroed tail)
        for (int k = 0; k < 6; ++k)
            u0f0[k] = llobj::realloc_zero_tail(u0f0[k], bytes(n_, dims[0x12 + k], 2));
        n = n_;
        llobj::ext::f140329e60(this, n_, dims, -1);
    }
}

// @src 140180390  copy n elements from other (state variable 0x11 too unless keep_id17)
void Cls_14017ef20::copy_from(const Cls_14017ef20& other, std::int32_t n_, const std::int32_t* dims,
                              bool keep_id17)
{
    if (n_ == 0) {
        free_arrays();
        return;
    }
    alloc(n_, dims);
    CowList<std::int32_t> skip;  // 14019ac40
    if (!keep_id17) {
        const std::int32_t id = 0x11;
        skip.push_back(id);  // 14019ad10
    }
    llobj::ext::f140329ef0(this, &other, skip);  // copy: 14019acb0
    if (other.u008 != nullptr)
        std::memcpy(u008, other.u008, bytes(n_, dims[0], 3));
    for (int k = 0; k < 6; ++k) {
        if (other.u0f0[k] != nullptr)
            std::memcpy(u0f0[k], other.u0f0[k], bytes(n_, dims[0x12 + k], 2));
    }
}  // skip released: 14019acf0

// @src 14019a370  u138 = v
void Cls_14017ef20::set_u138(std::int32_t v) { u138 = v; }

// @src 140182840  element count (+0x12c)
std::int32_t Cls_14017ef20::count() const { return n; }

} // namespace def::model
