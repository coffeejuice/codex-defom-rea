// Shared helpers of menus 5 and 6 (packet p10-02): the int readers 140008a00 / 140009100, the
// string compare 1400030d0, and the DB_STEP_DATA helpers 1401ca9e0..1401cae00 / 14019d5a0 that
// only these menus call. Behaviour: docs/re/menus/MENU5_6.md.
#include <def_pre/menu56.hpp>

#include <cstdio>
#include <cstring>
#include <iostream>
#include <stdexcept>

#include <def_pre/menu7.hpp>
#include <defmodel/layout/cls_1401f46e0.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/step/db_step.hpp>
#include <defmodel/window/inter_object_ops.hpp>

namespace def::model {
bool llobj_not_kind7(const LLOBJ_DATA* o); // 140160f10 (port/defmodel/src/material/state_remap.cpp)
}

namespace def::pre {

using model::Cls_1401f46e0;
using model::DB_STEP_DATA;
using model::field;
using model::LLOBJ_DATA;

namespace {

// 140007460 istream::getline(buf, 256, widen('\n')) on cin (the widen through the stream's
// locale, 140001570 / 1400067a0), then the std::string copy of the buffer
std::string getline256()
{
    char buf[0x100];
    buf[0] = '\0';
    std::cin.getline(buf, 0x100, '\n');
    return std::string(buf);
}

// 1401d6ed0: the n-th object of the step's object list (1-based)
LLOBJ_DATA* nth_object(DB_STEP_DATA* s, std::int32_t n)
{
    return static_cast<LLOBJ_DATA*>(model::step_object_at(*s, n));
}

std::int32_t obj_type(const LLOBJ_DATA* o) { return field<std::int32_t>(o, 0xc); }
std::int32_t obj_id(const LLOBJ_DATA* o) { return field<std::int32_t>(o, 0x8); }

} // namespace

// @src 1400030d0  compare(pos, n, s, len): the substring [pos, pos + min(n, size - pos)) against
// s[0, len); _Xran when pos > size
std::int32_t string_compare(const std::string& a, std::size_t pos, std::size_t n, const char* s,
                            std::size_t len)
{
    if (a.size() < pos) {
        throw std::out_of_range("invalid string position"); // 140477ab4 _Xran
    }
    std::size_t avail = a.size() - pos;
    if (avail < n) {
        n = avail;
    }
    const std::size_t k = n < len ? n : len;
    const int r = std::memcmp(a.c_str() + pos, s, k);
    if (r != 0) {
        return r;
    }
    if (n < len) {
        return -1;
    }
    return n != len ? 1 : 0;
}

// @src 140008a00  read "lo hi" into [*lo, *hi]
void read_int_pair(MenuRec*, std::int32_t* lo, std::int32_t* hi, const std::string& prompt,
                   bool complain)
{
    while (true) {
        std::fprintf(stdout, "%s", prompt.c_str()); // 140002ff0: c_str
        std::fflush(stdout);
        const std::string line = trim(getline256());
        const std::string a = field_of(line, ' ', 0);
        const std::string b = field_of(line, ' ', 1);
        const bool da = is_digits(a, 0);
        const bool db = is_digits(b, 0);
        if (!da || !db) {
            return;
        }
        if (*lo <= to_int(a) && to_int(a) <= *hi && *lo <= to_int(b) && to_int(b) <= *hi) {
            *lo = to_int(a);
            *hi = to_int(b);
            return;
        }
        if (complain) {
            std::fprintf(stdout, " ??? Invalid Data; Valid Data Range = (     %d to      %d)\n", *lo, *hi);
        }
    }
}

// @src 140009100  read n ints from one line
void read_ints(MenuRec*, std::int32_t lo, std::int32_t hi, std::int32_t* out, const std::string& prompt,
               std::int32_t n, bool complain)
{
    while (true) {
        std::fprintf(stdout, "%s", prompt.c_str()); // 140002ff0: c_str
        std::fflush(stdout);
        const std::string line = trim(getline256());
        bool ok = true;
        if (n < 1) {
            return;
        }
        for (std::int32_t i = 0; i < n; i = i + 1) {
            const std::string f = field_of(line, ' ', i);
            if (!is_digits(f, 0)) {
                return;
            }
            // values in range are stored even when another one is not (1400094b7)
            if (to_int(f) < lo || hi < to_int(f)) {
                ok = false;
            }
            else {
                out[i] = to_int(f);
            }
        }
        if (ok) {
            return;
        }
        if (complain) {
            std::fprintf(stdout, " ??? Invalid Data; Valid Data Range = (     %d to      %d)\n", lo, hi);
        }
    }
}

// @src 1401ca9e0  current object id
void step_set_current_object(DB_STEP_DATA* s, std::int32_t id)
{
    field<std::int32_t>(s, 0x1b4c) = id;
}

// @src 1401caa00  current object id, refreshed when asked
std::int32_t step_current_object(DB_STEP_DATA* s, bool refresh)
{
    std::int32_t& cur = field<std::int32_t>(s, 0x1b4c);
    if (refresh) {
        const std::int32_t last = step_last_object(s, 0);
        if (last < cur) {
            cur = 0;
        }
        else if (cur == 0) {
            cur = step_first_deformable(s);
            if (cur == 0) {
                cur = step_first_rigid(s);
            }
        }
    }
    return cur;
}

// @src 1401caae0  current pair
void step_set_current_pair(DB_STEP_DATA* s, Cls_1401f46e0* p)
{
    field<Cls_1401f46e0*>(s, 0x1b50) = p;
}

// @src 1401cab10  current pair
Cls_1401f46e0* step_current_pair(DB_STEP_DATA* s)
{
    return field<Cls_1401f46e0*>(s, 0x1b50);
}

// @src 1401cab30  objects whose type is not 7
std::int32_t step_count_objects(DB_STEP_DATA* s)
{
    std::int32_t n = 0;
    for (std::int32_t i = 1; i <= field<std::int32_t>(s, 0x1378); i = i + 1) {
        LLOBJ_DATA* o = nth_object(s, i);
        if (o != nullptr && model::llobj_not_kind7(o)) { // 140160f10
            n = n + 1;
        }
    }
    return n;
}

// @src 1401cac80  first object that is neither empty (7) nor rigid (0)
std::int32_t step_first_deformable(DB_STEP_DATA* s)
{
    LLOBJ_DATA* o = nullptr;
    std::int32_t i = 1;
    while (true) {
        if (field<std::int32_t>(s, 0x1378) < i) {
            o = nullptr; // the loop's result variable is only set on a match
            break;
        }
        o = nth_object(s, i);
        if (o != nullptr && obj_type(o) != 7 && obj_type(o) != 0) {
            break;
        }
        i = i + 1;
    }
    return o == nullptr ? 0 : obj_id(o);
}

// @src 1401cad40  first rigid object (type 0)
std::int32_t step_first_rigid(DB_STEP_DATA* s)
{
    LLOBJ_DATA* o = nullptr;
    std::int32_t i = 1;
    while (true) {
        if (field<std::int32_t>(s, 0x1378) < i) {
            o = nullptr; // the loop's result variable is only set on a match
            break;
        }
        o = nth_object(s, i);
        if (o != nullptr && obj_type(o) != 7 && obj_type(o) == 0) {
            break;
        }
        i = i + 1;
    }
    return o == nullptr ? 0 : obj_id(o);
}

// @src 1401cae00  last object (type not 7) at or below `from`
std::int32_t step_last_object(DB_STEP_DATA* s, std::int32_t from)
{
    const std::int32_t start = from == 0 ? field<std::int32_t>(s, 0x1378) : from;
    // the result is only set on a match (an exhausted loop leaves it null)
    LLOBJ_DATA* o = nullptr;
    for (std::int32_t i = start; 0 < i; i = i - 1) {
        LLOBJ_DATA* c = nth_object(s, i);
        if (c != nullptr && obj_type(c) != 7) {
            o = c;
            break;
        }
    }
    if (start != field<std::int32_t>(s, 0x1378) && o == nullptr) {
        for (std::int32_t i = 1; i <= field<std::int32_t>(s, 0x1378); i = i + 1) {
            LLOBJ_DATA* c = nth_object(s, i);
            if (c != nullptr && obj_type(c) != 7) {
                o = c;
                break;
            }
        }
    }
    return o == nullptr ? 0 : obj_id(o);
}

// @src 14019d5a0  pair (a, b): 1401facf0 on the list at +0x13c8
Cls_1401f46e0* step_find_pair(DB_STEP_DATA* s, std::int32_t a, std::int32_t b)
{
    return model::pair_find(&s->inter_obj, a, b);
}

} // namespace def::pre
