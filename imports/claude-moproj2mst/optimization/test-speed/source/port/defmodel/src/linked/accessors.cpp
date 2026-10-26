// Raw-offset accessors of p1-06's range (defmodel/linked/accessors.hpp).
#include <defmodel/linked/accessors.hpp>

#include <cstring>

#include <defmodel/layout/cls_1401e7b10.hpp>

namespace def::model::linked {

namespace {

template <class T>
T load(const void* obj, std::size_t off)
{
    T v;
    std::memcpy(&v, static_cast<const unsigned char*>(obj) + off, sizeof v);
    return v;
}

template <class T>
void store(void* obj, std::size_t off, T v)
{
    std::memcpy(static_cast<unsigned char*>(obj) + off, &v, sizeof v);
}

} // namespace

// @src 140256710
std::int32_t get_i32_44(const void* obj) { return load<std::int32_t>(obj, 0x44); }

// @src 140256720
std::int32_t get_i32_18(const void* obj) { return load<std::int32_t>(obj, 0x18); }

// @src 140256730
void set_u64_18(void* obj, std::uint64_t v) { store(obj, 0x18, v); }

// @src 140256750
void set_u64_20(void* obj, std::uint64_t v) { store(obj, 0x20, v); }

// @src 140256770
std::int32_t get_i32_24(const void* obj) { return load<std::int32_t>(obj, 0x24); }

// @src 1402568e0
std::uint64_t deref_u64_10(void* const* handle) { return load<std::uint64_t>(*handle, 0x10); }

// @src 140256780
std::uint64_t member10_deref_u64_10(const void* obj)
{
    void* h = load<void*>(obj, 0x10);
    return deref_u64_10(&h); // 140159c20 (p1-04): same body
}

// @src 1402567a0
std::uint64_t member20_deref_u64_10(const void* obj)
{
    void* h = load<void*>(obj, 0x20);
    return deref_u64_10(&h);
}

// @src 14026a3c0
double* member10_array_first(const void* obj)
{
    void* rep = load<void*>(obj, 0x10);         // the handle at +0x10
    double* first = load<double*>(rep, 0x08);   // 14026a4d0: (*handle)+8
    return first + 0;                           // 14026a470(h, 0): first + 0 * 8
}

// @src 1401f4390
void set_dsmax(Cls_1401e7b10* sim, double v) { sim->dsmax = v; }

// @src 1401fc040
std::int32_t fun_1401fc040(const void* obj)
{
    std::int32_t r;
    if (load<std::int32_t>(obj, 0x87c) == 0) {
        r = 0;
    } else if (load<const std::int32_t*>(obj, 0x9f8) == nullptr) {
        r = 0;
    } else {
        r = *load<const std::int32_t*>(obj, 0x9f8);
    }
    return r;
}

} // namespace def::model::linked
