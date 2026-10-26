// std::basic_string<char> member functions as instantiated in DEF_PRE_64 (VS2008 STL), over the
// 0x28-byte layout of defrt/stdlib/msvc9_string.hpp. Packet p1-01.
#include <defrt/stdlib/msvc9_string.hpp>

#include <cstring>
#include <new>
#include <stdexcept>

namespace def::rt {

namespace {

// FUN_140477ab4: std::_Xran()
[[noreturn]] void xran() { throw std::out_of_range("invalid string position"); }
// FUN_140477a4c: std::_Xlen()
[[noreturn]] void xlen() { throw std::length_error("string too long"); }

// FUN_1404797d8 (operator new) / free: the pair the instantiations use.
char* allocate(std::size_t n) { return static_cast<char*>(::operator new(n)); }
void deallocate(char* p) { ::operator delete(p); }

} // namespace

// @src 140005040  basic_string() (_Tidy(false) inlined)
Msvc9String::Msvc9String() noexcept
{
    res_ = 15;
    size_ = 0;
    bx_.buf[0] = '\0';
}

// @src 1400066a0  basic_string(const char*)
Msvc9String::Msvc9String(const char* s)
{
    res_ = 15;
    size_ = 0;
    bx_.buf[0] = '\0';
    assign(s, std::strlen(s));
}

// @src 1401b3730  basic_string(size_type count, char ch)
Msvc9String::Msvc9String(std::size_t count, char ch)
{
    tidy(false, 0); // 140004df0(this, 0, 0)
    assign(count, ch);
}

Msvc9String::Msvc9String(const Msvc9String& other) : Msvc9String()
{
    assign(other);
}

// @src 140005060  ~basic_string() (_Tidy(true))
Msvc9String::~Msvc9String()
{
    if (15 < res_) {
        deallocate(bx_.ptr);
    }
    res_ = 15;
    size_ = 0;
    bx_.buf[0] = '\0';
}

// @src 140002990  _Myptr()
char* Msvc9String::data() noexcept
{
    return 15 < res_ ? bx_.ptr : bx_.buf;
}

// @src 140002990  _Myptr() const
const char* Msvc9String::data() const noexcept
{
    return 15 < res_ ? bx_.ptr : bx_.buf;
}

// @src 140003240  _Chassign(off, count, ch)
void Msvc9String::chassign(std::size_t off, std::size_t count, char ch)
{
    if (count == 1) {
        data()[off] = ch;
    } else {
        std::memset(data() + off, ch, count); // FUN_140478d30
    }
}

// @src 140003290  _Eos(newsize)
void Msvc9String::eos(std::size_t newsize)
{
    size_ = newsize;
    data()[newsize] = '\0';
}

// @src 140004df0  _Tidy(built, newsize)
void Msvc9String::tidy(bool built, std::size_t newsize)
{
    if (built && 15 < res_) {
        char* src = bx_.ptr;
        if (newsize != 0) {
            std::memcpy(bx_.buf, src, newsize); // memcpy_s(bx, 0x10, src, newsize)
        }
        deallocate(src);
    }
    res_ = 15;
    size_ = newsize;
    bx_.buf[newsize] = '\0';
}

// @src 140004eb0  _Copy(newsize, oldlen): reallocate, keeping the first oldlen characters
void Msvc9String::copy(std::size_t newsize, std::size_t oldlen)
{
    std::size_t newres = newsize | 0xf;
    if (newres != npos) {                     // max_size() (npos - 1) >= newres
        const std::size_t half = res_ >> 1;
        if (newres / 3 < half && res_ <= (npos - 1) - half) {
            newres = half + res_;             // grow by half the old capacity
        }
    } else {
        newres = newsize;
    }
    if (newres + 1 == 0) {
        throw std::bad_alloc(); // 140004f63: the allocator's count check
    }
    char* dst = allocate(newres + 1);
    if (oldlen != 0) {
        std::memcpy(dst, data(), oldlen); // memcpy_s(dst, newres + 1, src, oldlen)
    }
    if (15 < res_) {
        deallocate(bx_.ptr);
    }
    bx_.buf[0] = '\0';
    bx_.ptr = dst;
    res_ = newres;
    eos(oldlen);
}

// @src 1400051f0  _Grow(newsize, trim): true when the string is to hold characters
bool Msvc9String::grow(std::size_t newsize, bool trim)
{
    if (newsize == npos) {
        xlen();
    }
    const std::size_t res = res_;
    if (res < newsize) {
        copy(newsize, size_);
    } else if (!trim || 15 < newsize) {
        if (newsize == 0) {
            size_ = 0;
            data()[0] = '\0';
        }
    } else {
        // trim to the inline buffer
        const std::size_t n = newsize < size_ ? newsize : size_;
        if (15 < res) {
            char* src = bx_.ptr;
            if (n != 0) {
                std::memcpy(bx_.buf, src, n);
            }
            deallocate(src);
        }
        res_ = 15;
        size_ = n;
        bx_.buf[n] = '\0';
    }
    return newsize != 0;
}

// @src 140004d40  erase(off, count)
Msvc9String& Msvc9String::erase(std::size_t off, std::size_t count)
{
    if (size_ < off) {
        xran();
    }
    const std::size_t tail = size_ - off;
    if (tail < count) {
        count = tail;
    }
    if (count != 0) {
        char* p = data();
        std::memmove(p + off, p + off + count, tail - count);
        eos(size_ - count);
    }
    return *this;
}

// @src 1400066f0  assign(const basic_string&)
Msvc9String& Msvc9String::assign(const Msvc9String& s)
{
    return assign(s, 0, npos);
}

// @src 1400058b0  assign(const basic_string&, off, count)
Msvc9String& Msvc9String::assign(const Msvc9String& s, std::size_t off, std::size_t count)
{
    if (s.size_ < off) {
        xran();
    }
    std::size_t n = s.size_ - off;
    if (count < n) {
        n = count;
    }
    if (this == &s) {
        erase(off + n, npos);
        erase(0, off);
        return *this;
    }
    // _Grow(n) inlined
    if (n == npos) {
        xlen();
    }
    if (res_ < n) {
        copy(n, size_);
        if (n == 0) {
            return *this;
        }
    } else if (n == 0) {
        size_ = 0;
        data()[0] = '\0';
        return *this;
    }
    std::memcpy(data(), s.data() + off, n);
    eos(n);
    return *this;
}

// @src 140005c80  assign(const char*, count)
Msvc9String& Msvc9String::assign(const char* p, std::size_t count)
{
    if (p != nullptr) {
        const char* mine = data();
        if (mine <= p && p < mine + size_) {
            // _Inside(p): assign from our own characters
            return assign(*this, static_cast<std::size_t>(p - mine), count);
        }
    }
    // _Grow(count) inlined
    if (count == npos) {
        xlen();
    }
    if (res_ < count) {
        copy(count, size_);
    } else if (count == 0) {
        size_ = 0;
        data()[0] = '\0';
        return *this;
    }
    if (count != 0) {
        std::memcpy(data(), p, count);
        eos(count);
    }
    return *this;
}

// @src 1400059d0  assign(count, ch)
Msvc9String& Msvc9String::assign(std::size_t count, char ch)
{
    if (count == npos) {
        xlen();
    }
    if (res_ < count) {
        copy(count, size_);
    } else if (count == 0) {
        size_ = 0;
        data()[0] = '\0';
        return *this;
    }
    if (count != 0) {
        chassign(0, count, ch); // inlined: one store for count 1, memset otherwise
        eos(count);
    }
    return *this;
}

} // namespace def::rt
