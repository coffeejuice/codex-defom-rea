// defrt/stdlib/msvc9_string.hpp: std::string as DEF_PRE_64 (VS2008 STL, _SECURE_SCL on) laid it
// out, for model classes that embed a string at a fixed offset (packet p1-01).
//
// Layout (0x28 bytes, x64):
//   0x00  aux    _Container_base_aux::_Myownedaux (checked-iterator bookkeeping, always null here)
//   0x08  bx     16-byte buffer, or the heap pointer when res > 15 (small-string optimisation)
//   0x18  size   _Mysize
//   0x20  res    _Myres (capacity without the terminating NUL; 15 when the buffer is inline)
//
// Code that does not need the binary's layout uses std::string: no output of DEF_PRE depends on
// the string's capacity, only on its contents. The growth rule (_Copy) is kept anyway, so that
// a ported class holds exactly the bytes the original held.
#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>

namespace def::rt {

class Msvc9String {
public:
    static constexpr std::size_t npos = static_cast<std::size_t>(-1);
    static constexpr std::size_t kBufSize = 16;

    Msvc9String() noexcept;                          // 140005040
    explicit Msvc9String(const char* s);             // 1400066a0
    Msvc9String(std::size_t count, char ch);         // 1401b3730
    Msvc9String(const Msvc9String& other);           // init + 1400066f0
    Msvc9String& operator=(const Msvc9String& other) { return assign(other); }
    ~Msvc9String();                                  // 140005060

    // @src 140002810  size() (_Mysize)
    std::size_t size() const noexcept { return size_; }
    std::size_t capacity() const noexcept { return res_; }
    char* data() noexcept;                                   // 140002990
    const char* data() const noexcept;                       // 140002990
    const char* c_str() const noexcept { return data(); }
    std::string_view view() const noexcept { return {data(), size_}; }
    std::string str() const { return std::string(view()); }

    Msvc9String& assign(const Msvc9String& s);                                 // 1400066f0
    Msvc9String& assign(const Msvc9String& s, std::size_t off, std::size_t n); // 1400058b0
    Msvc9String& assign(const char* p, std::size_t n);                         // 140005c80
    Msvc9String& assign(std::size_t n, char ch);                               // 1400059d0
    Msvc9String& erase(std::size_t off = 0, std::size_t n = npos);             // 140004d40

    // The private helpers of std::basic_string, public so that ported callers that use them
    // (append 14015ccb0 in p1-04 calls grow/chassign/eos) can follow the binary call for call.
    void chassign(std::size_t off, std::size_t count, char ch);         // 140003240 _Chassign
    void eos(std::size_t newsize);                                      // 140003290 _Eos
    void tidy(bool built = false, std::size_t newsize = 0);             // 140004df0 _Tidy
    void copy(std::size_t newsize, std::size_t oldlen);                 // 140004eb0 _Copy
    bool grow(std::size_t newsize, bool trim = false);                  // 1400051f0 _Grow

private:
    void* aux_ = nullptr; // 0x00
    union {
        char buf[kBufSize];
        char* ptr;
    } bx_;                // 0x08
    std::size_t size_;    // 0x18
    std::size_t res_;     // 0x20

    friend struct Msvc9StringLayout;
};

struct Msvc9StringLayout {
    static constexpr std::size_t aux = offsetof(Msvc9String, aux_);
    static constexpr std::size_t bx = offsetof(Msvc9String, bx_);
    static constexpr std::size_t size = offsetof(Msvc9String, size_);
    static constexpr std::size_t res = offsetof(Msvc9String, res_);
};

static_assert(sizeof(Msvc9String) == 0x28);

} // namespace def::rt
