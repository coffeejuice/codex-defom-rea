// testkit: minimal tier-1 test support for the DEF_PRE port (no external dependencies).
//
//   #include <testkit/check.hpp>
//   TEST_CASE(registry_has_304_entries) { CHECK_EQ(registry().size(), 304u); }
//   int main() { return testkit::run_all(); }
//
// A failing CHECK records the failure and the test case continues; REQUIRE stops the case.
// Exit code is the number of failed cases (0 = pass), which is what ctest looks at.
#pragma once

#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <functional>
#include <sstream>
#include <string>
#include <vector>

namespace testkit {

struct Case {
    const char* name;
    void (*fn)();
};

inline std::vector<Case>& registry()
{
    static std::vector<Case> cases;
    return cases;
}

struct Registrar {
    Registrar(const char* name, void (*fn)()) { registry().push_back({name, fn}); }
};

struct Abort {};

inline int& failures()
{
    static int n = 0;
    return n;
}

inline void fail(const char* file, int line, const std::string& what)
{
    ++failures();
    std::fprintf(stderr, "%s(%d): FAILED %s\n", file, line, what.c_str());
}

template <class A, class B>
std::string describe(const char* ea, const char* eb, const A& a, const B& b)
{
    std::ostringstream s;
    s << ea << " == " << eb << "  (" << a << " vs " << b << ")";
    return s.str();
}

// Bitwise double comparison: the port's FP results must be identical, not close.
inline bool same_bits(double a, double b)
{
    std::uint64_t x, y;
    std::memcpy(&x, &a, 8);
    std::memcpy(&y, &b, 8);
    return x == y;
}

// Root of the repository (set by ctest via DEF_PORT_REPO), for golden files under the tree.
inline std::filesystem::path repo_root()
{
    const char* r = std::getenv("DEF_PORT_REPO");
    return r ? std::filesystem::path(r) : std::filesystem::current_path();
}

inline int run_all()
{
    int failed_cases = 0;
    for (const Case& c : registry()) {
        int before = failures();
        try {
            c.fn();
        } catch (const Abort&) {
        } catch (const std::exception& e) {
            fail(c.name, 0, std::string("exception: ") + e.what());
        }
        bool ok = failures() == before;
        if (!ok)
            ++failed_cases;
        std::printf("[%s] %s\n", ok ? " ok " : "FAIL", c.name);
    }
    std::printf("%d of %zu cases failed\n", failed_cases, registry().size());
    return failed_cases;
}

} // namespace testkit

#define TEST_CASE(name)                                                                        \
    static void name();                                                                        \
    static ::testkit::Registrar name##_registrar(#name, &name);                                \
    static void name()

#define CHECK(cond)                                                                            \
    do {                                                                                       \
        if (!(cond))                                                                           \
            ::testkit::fail(__FILE__, __LINE__, #cond);                                        \
    } while (0)

#define REQUIRE(cond)                                                                          \
    do {                                                                                       \
        if (!(cond)) {                                                                         \
            ::testkit::fail(__FILE__, __LINE__, #cond);                                        \
            throw ::testkit::Abort{};                                                          \
        }                                                                                      \
    } while (0)

#define CHECK_EQ(a, b)                                                                         \
    do {                                                                                       \
        const auto& va_ = (a);                                                                 \
        const auto& vb_ = (b);                                                                 \
        if (!(va_ == vb_))                                                                     \
            ::testkit::fail(__FILE__, __LINE__, ::testkit::describe(#a, #b, va_, vb_));        \
    } while (0)

// Bit-identical doubles (use this, not a tolerance, for ported FP code).
#define CHECK_BITS(a, b)                                                                       \
    do {                                                                                       \
        double va_ = (a), vb_ = (b);                                                           \
        if (!::testkit::same_bits(va_, vb_))                                                   \
            ::testkit::fail(__FILE__, __LINE__, ::testkit::describe(#a, #b, va_, vb_));        \
    } while (0)
