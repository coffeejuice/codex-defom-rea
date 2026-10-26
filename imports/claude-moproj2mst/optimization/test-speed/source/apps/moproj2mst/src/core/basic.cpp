// Basic pieces every packet links (owner: G0): errors, String helpers without Qt rules.
#include <cstdio>

#include "m2m/common.hpp"
#include "m2m/model.hpp"
#include "m2m/qt4.hpp"

namespace m2m {

static std::string describe(std::uint64_t addr, const char* name)
{
    char buf[32];
    std::snprintf(buf, sizeof buf, "0x%llx", static_cast<unsigned long long>(addr));
    return std::string("not ported yet: ") + buf + " " + (name ? name : "");
}

NotPorted::NotPorted(std::uint64_t addr, const char* name)
    : Error(M2M_E_UNSUPPORTED, describe(addr, name)), src(addr)
{
}

void not_ported(std::uint64_t addr, const char* name)
{
    throw NotPorted(addr, name);
}

Item* ItemManager::find(int id) const
{
    for (const auto& it : rows)
        if (it && it->id == id) return it.get();
    return nullptr;
}

}  // namespace m2m

namespace m2m::qt {

String String::latin1(const char* s)
{
    if (!s) return String();
    return latin1(std::string_view(s));
}

String String::latin1(std::string_view s)
{
    std::u16string u;
    u.reserve(s.size());
    for (unsigned char c : s) u += static_cast<char16_t>(c);
    return String(std::move(u));
}

std::string String::utf8() const
{
    std::string out;
    for (size_t i = 0; i < s_.size(); ++i) {
        std::uint32_t c = s_[i];
        if (c >= 0xd800 && c < 0xdc00 && i + 1 < s_.size() && s_[i + 1] >= 0xdc00 && s_[i + 1] < 0xe000) {
            c = 0x10000 + ((c - 0xd800) << 10) + (s_[i + 1] - 0xdc00);
            ++i;
        }
        if (c < 0x80) {
            out += static_cast<char>(c);
        } else if (c < 0x800) {
            out += static_cast<char>(0xc0 | (c >> 6));
            out += static_cast<char>(0x80 | (c & 0x3f));
        } else if (c < 0x10000) {
            out += static_cast<char>(0xe0 | (c >> 12));
            out += static_cast<char>(0x80 | ((c >> 6) & 0x3f));
            out += static_cast<char>(0x80 | (c & 0x3f));
        } else {
            out += static_cast<char>(0xf0 | (c >> 18));
            out += static_cast<char>(0x80 | ((c >> 12) & 0x3f));
            out += static_cast<char>(0x80 | ((c >> 6) & 0x3f));
            out += static_cast<char>(0x80 | (c & 0x3f));
        }
    }
    return out;
}

}  // namespace m2m::qt
