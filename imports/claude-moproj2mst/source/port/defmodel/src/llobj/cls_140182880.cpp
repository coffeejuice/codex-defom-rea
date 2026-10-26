// Cls_140182880: node state-variable set (defmodel/llobj/cls_140182880.hpp).
#include <defmodel/llobj/cls_140182880.hpp>

#include <cstdlib>
#include <cstring>

#include <defmodel/llobj/ext.hpp>
#include <defmodel/llobj/realloc_uninitialized.hpp>

namespace def::model {

namespace {
std::size_t bytes(std::int32_t a, std::int32_t b, int shift)
{
    const auto p = static_cast<std::int32_t>(static_cast<std::uint32_t>(a) * static_cast<std::uint32_t>(b));
    return static_cast<std::size_t>(static_cast<std::int64_t>(p) << shift);
}
std::int32_t int_at(const void* p, std::size_t off)
{
    std::int32_t v;
    std::memcpy(&v, static_cast<const std::byte*>(p) + off, sizeof v);
    return v;
}
} // namespace

// 140182b10: operator new(size) and ctor per entry, in this order.
const Cls_140182880::Reg Cls_140182880::kRegs[25] = {
    {0x14032c950, 0x40, 0x26, 0x20, true},  {0x14032c8c0, 0x38, 0x27, 0x28, false},
    {0x14032c8c0, 0x38, 0x28, 0x30, false}, {0x14032c8c0, 0x38, 0x29, 0x38, false},
    {0x14032c8c0, 0x38, 0x2a, 0x40, false}, {0x14032ca80, 0x40, 0x2b, 0x48, true},
    {0x14032c8c0, 0x38, 0x2c, 0x50, false}, {0x14032cbb0, 0x40, 0x2d, 0x58, true},
    {0x14032ccd0, 0x38, 0x2e, 0x60, false}, {0x14032c8c0, 0x38, 0x2f, 0x68, false},
    {0x14032c8c0, 0x38, 0x30, 0x70, false}, {0x14032c8c0, 0x38, 0x31, 0x78, false},
    {0x14032c8c0, 0x38, 0x32, 0x80, false}, {0x14032c8c0, 0x38, 0x33, 0x88, false},
    {0x14032c8c0, 0x38, 0x34, 0x90, false}, {0x14032c8c0, 0x38, 0x35, 0x98, false},
    {0x14032c8c0, 0x38, 0x36, 0xa0, false}, {0x14032c8c0, 0x38, 0x37, 0xa8, false},
    {0x14032c8c0, 0x38, 0x38, 0xb0, false}, {0x14032c8c0, 0x38, 0x39, 0xb8, false},
    {0x14032c8c0, 0x38, 0x3a, 0xc0, false}, {0x14032c8c0, 0x38, 0x3b, 0xc8, false},
    {0x14032ceb0, 0x40, 0x3e, 0xd0, true},  {0x14032c8c0, 0x38, 0x3f, 0xd8, false},
    {0x14032c8c0, 0x38, 0x3c, 0xe0, false},
};

// @src 140182880  ctor
Cls_140182880::Cls_140182880()
{
    llobj::ext::f140329c70(this);
    u0f0 = nullptr;
    u0f8 = 3;
    init();
}

// @src 1401828e0  dtor
Cls_140182880::~Cls_140182880()
{
    free_arrays();
    llobj::ext::f140329cb0(this);
}

// @src 1401829c0  zero the arrays and counts, register the variables
void Cls_140182880::init()
{
    u008 = nullptr;
    u018 = nullptr;
    n = 0;
    m = 0;
    u0fc = 0;
    u100 = 0;
    u010 = nullptr;
    register_vars();
}

// @src 140182a50  free the arrays, then reset each variable (140329dc0)
void Cls_140182880::free_arrays()
{
    if (u008 != nullptr)
        std::free(u008);
    u008 = nullptr;
    if (u018 != nullptr)
        std::free(u018);
    u018 = nullptr;
    n = 0;
    m = 0;
    if (u010 != nullptr)
        std::free(u010);
    u010 = nullptr;
    llobj::ext::f140329dc0(this);
}

// @src 140182b10  delete the old variables (140329d10), create and append the 25 variables
void Cls_140182880::register_vars()
{
    llobj::ext::f140329d10(this);
    for (const Reg& r : kRegs) {
        void* field = reinterpret_cast<std::byte*>(this) + r.slot;
        void* sv = llobj::ext::new_statevar(r.ctor, r.size, r.id, field, r.pass_owner ? this : nullptr);
        llobj::ext::f140329e30(this, sv);
    }
}

// @src 140183770  realloc for n nodes of m values (u018: int at src+0xf4 per node), size the variables
void Cls_140182880::alloc(std::int32_t n_, const void* src, std::int32_t m_)
{
    if (0 < n_) {
        u008 = llobj::realloc_uninitialized(u008, bytes(n_, m_, 2));
        u018 = llobj::realloc_uninitialized(u018, bytes(n_, int_at(src, 0xf4), 2));
        n = n_;
        m = m_;
        llobj::ext::f140329e60(this, n_, src, -1);
    }
}

// @src 140183900  copy n nodes from other; a false keep flag leaves those variables out of the copy
void Cls_140182880::copy_from(const Cls_140182880& other, std::int32_t n_, std::int32_t m_, const void* src,
                              bool keep_3c, bool keep_31_34, bool keep_28)
{
    if (n_ == 0)
        free_arrays();  // and go on, as the original does
    alloc(n_, src, m_);
    CowList<std::int32_t> skip;  // 14019ac40
    if (!keep_3c)
        skip.push_back(0x3c);  // 14019ad10
    if (!keep_28)
        skip.push_back(0x28);
    if (!keep_31_34) {
        skip.push_back(0x31);
        skip.push_back(0x32);
        skip.push_back(0x33);
        skip.push_back(0x34);
    }
    llobj::ext::f140329ef0(this, &other, skip);  // copy of the handle: 14019acb0
    if (other.u008 != nullptr)
        std::memcpy(u008, other.u008, bytes(n_, m_, 2));
    if (other.u018 != nullptr)
        std::memcpy(u018, other.u018, bytes(n_, int_at(src, 0xf4), 2));
}  // skip released: 14019acf0

// @src 140183c20  1; or 4 / 8 / 1 when the owner's int at 0x888 + 4i is 3 (by u0f8 == 3, m == 8)
std::int32_t Cls_140182880::fun_140183c20(std::int32_t i) const
{
    std::int32_t r = 1;
    if (u0f0 == nullptr) {
        r = 1;
    } else if (int_at(u0f0, static_cast<std::size_t>(0x888 + static_cast<std::int64_t>(i) * 4)) == 3) {
        if (!u0f8_is_3())  // 14019a450
            r = 4;
        else if (m == 8)
            r = 8;
        else
            r = 1;
    }
    return r;
}

// @src 14019a450  u0f8 == 3 (absorbed into 140183c20; also called by 140184ac0)
bool Cls_140182880::u0f8_is_3() const { return u0f8 == 3; }

// @src 14019a3f0  u0f8 = v
void Cls_140182880::set_u0f8(std::int32_t v) { u0f8 = v; }

// @src 1401854f0  node count (+0xe8)
std::int32_t Cls_140182880::count() const { return n; }

// @src 140185530  values per node (+0xec)
std::int32_t Cls_140182880::width() const { return m; }

} // namespace def::model
