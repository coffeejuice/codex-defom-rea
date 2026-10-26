// Tier-1 helper for the p1-04 tests: records the calls p1-04 code makes into other packets
// (defmodel/llobj/ext.hpp) and supplies their results.
#pragma once

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <new>
#include <vector>

#include <defmodel/llobj/ext.hpp>

namespace llobj_test {

struct Call {
    std::uint64_t addr;
    std::uint64_t a[4];
};

struct Recorder {
    std::vector<Call> calls;
    // Optional result per call; default 0.
    std::function<std::uint64_t(const Call&)> result;

    static Recorder*& current()
    {
        static Recorder* r = nullptr;
        return r;
    }
    static std::uint64_t hook(std::uint64_t addr, const std::uint64_t* args, int nargs)
    {
        Call c{addr, {0, 0, 0, 0}};
        for (int i = 0; i < nargs && i < 4; ++i) c.a[i] = args[i];
        Recorder* r = current();
        r->calls.push_back(c);
        return r->result ? r->result(c) : 0;
    }

    Recorder()
    {
        current() = this;
        def::model::llobj::ext::g_hook = &Recorder::hook;
    }
    ~Recorder()
    {
        def::model::llobj::ext::g_hook = nullptr;
        current() = nullptr;
    }
    Recorder(const Recorder&) = delete;
    Recorder& operator=(const Recorder&) = delete;

    std::vector<Call> only(std::uint64_t addr) const
    {
        std::vector<Call> v;
        for (const Call& c : calls)
            if (c.addr == addr) v.push_back(c);
        return v;
    }
    std::vector<std::uint64_t> addrs() const
    {
        std::vector<std::uint64_t> v;
        for (const Call& c : calls) v.push_back(c.addr);
        return v;
    }
};

inline std::uint64_t u(const void* p) { return reinterpret_cast<std::uintptr_t>(p); }

// Zeroed storage for an object whose ctor belongs to another packet (only the members the test
// sets are meaningful).
template <class T>
T* raw_object()
{
    void* p = ::operator new(sizeof(T));
    std::memset(p, 0, sizeof(T));
    return static_cast<T*>(p);
}

} // namespace llobj_test
