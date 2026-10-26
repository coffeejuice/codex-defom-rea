// p8-01: the command keywords EXECMD, GENINT, IMPLUB, IMPEQP, ARMCTL, MORP and GFREAD (cases of
// FUN_140378e00, defcmd/cmd_p8_01.hpp) with their handlers. Specs: docs/re/commands/<CMD>.md.
#define _CRT_SECURE_NO_WARNINGS
#include <defcmd/cmd_p8_01.hpp>
#include <defcmd/gfread.hpp>

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <list>
#include <share.h>
#include <string>
#include <vector>

#include <defkey/kw_g2obj.hpp>
#include <defkey/line_io.hpp>
#include <defmodel/layout/cls_1401f46e0.hpp>
#include <defmodel/layout/cls_1401fa020.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/llobj_ctor/llobj_ctor.hpp>
#include <defmodel/strtrim.hpp>
#include <defmodel/window/inter_object_ops.hpp>
#include <defrt/stdlib/legacy_printf.hpp>

#include "ext_p8_07.hpp"

namespace def::cmd {

namespace {

using model::Cls_1401f46e0;
using model::DB_STEP_DATA;
using model::LLOBJ_DATA;

template <class T>
T& at(void* p, std::size_t off)
{
    return *reinterpret_cast<T*>(static_cast<unsigned char*>(p) + off);
}

// FUN_14047b934: _fsopen(name, mode, _SH_DENYNO)
std::FILE* fsopen(const char* name, const char* mode) { return _fsopen(name, mode, _SH_DENYNO); }

// The MORP argument lists. The original's are copy-on-write handles over a VS2010 std::list
// (a heap representation {refcount, sentinel, size}); they live only on the dispatcher's stack
// and nothing but push_back, size and element i is used, so std::list keeps the behaviour. The
// representation internals (140382ab0 ... 140383080) are not-needed in the status file.
class DoubleList {
public:
    // @src 140382510  ctor (new representation 140382ab0)
    DoubleList() = default;
    // @src 140382580  dtor (release 1403827f0)
    ~DoubleList() = default;
    // @src 1403825a0  push_back (detach 1403826a0, insert before end 140382be0)
    void push_back(const double* v) { items_.push_back(*v); }
    // @src 1401b0790  size (rep+0x10)
    std::uint64_t size() const { return items_.size(); }
    // @src 1401b0760  element i: node walk 1401b0810, value at node+0x10
    // @src 1401b0810  walk i links from the first node
    const double* at(std::uint64_t i) const
    {
        auto it = items_.begin();
        for (std::uint64_t k = 0; k < i; ++k) ++it;
        return &*it;
    }

private:
    std::list<double> items_;
};

// The string list (handle 14032f7d0 / push_back 14032f880, copy-on-write list<std::string>; the
// same stack-only use).
class StringList {
public:
    void push_back(const std::string& s) { items_.push_back(s); }
    // @src 1401b0740  size (rep+0x10)
    std::uint64_t size() const { return items_.size(); }
    // @src 1401b0710  element i (1401b07b0 walk, value at node+0x10)
    const std::string& at(std::uint64_t i) const
    {
        auto it = items_.begin();
        for (std::uint64_t k = 0; k < i; ++k) ++it;
        return *it;
    }

private:
    std::list<std::string> items_;
};

// @src 1401aa750  MORP handler: write DEF_MORP2.INI / DEF_MORP3.INI, launch the morphing program
bool morp_run(DB_STEP_DATA* m, std::uint32_t mval, const DoubleList& doubles, const StringList& strings)
{
    std::FILE* f = nullptr;
    if (!model::model_is_3d(m)) {                // 1400015f0
        f = fsopen("DEF_MORP2.INI", "wt");       // str 140523574
    } else {
        f = fsopen("DEF_MORP3.INI", "wt");       // str 14052347c
    }
    if (f == nullptr) return false;
    rt::crt::fprintf(f, "%d\n", mval);
    const std::uint32_t n = static_cast<std::uint32_t>(doubles.size());
    rt::crt::fprintf(f, "%d\n", n);
    for (int i = 0; i < static_cast<int>(n); i = i + 1) rt::crt::fprintf(f, "%lf\n", *doubles.at(static_cast<std::uint64_t>(i)));
    const int ns = static_cast<int>(strings.size());
    for (int i = 0; i < ns; i = i + 1) rt::crt::fprintf(f, "%s\n", strings.at(static_cast<std::uint64_t>(i)).c_str());  // 140003010
    std::fclose(f);
    char buf[208];
    if (!model::model_is_3d(m)) {
        std::sprintf(buf, "DEF_MORP2.EXE");
        p801ext::launch_kind2(1, buf);  // 140224100
    } else {
        std::sprintf(buf, "DEF_MORP3.EXE morphing DEF_MORP3.INI");
        p801ext::launch_kind3(1, buf);  // 140224130
    }
    return true;
}

// @src 1401d6db0  model+0x1378 (object slots)
std::int32_t object_slots(DB_STEP_DATA* m) { return at<std::int32_t>(m, 0x1378); }

// @src 1401fad60  pair with master m, slave s and flag
Cls_1401f46e0* pair_find_flag(model::Cls_1401fa020* l, std::int32_t master, std::int32_t slave, std::int32_t flag)
{
    Cls_1401f46e0* p = l->head;
    while (p != nullptr && (p->u168 != master || p->u16c != slave || p->u170 != flag)) p = p->next;
    return p;
}

// @src 1401ac020  GENINT handler
bool genint_run(DB_STEP_DATA* m, const char* name)
{
    m->inter_obj.clear();  // 1401fade0
    const std::int32_t n = object_slots(m);
    auto* t = new DB_STEP_DATA(3);  // 1401c8950 (a 0x3458-byte stack object in the original)
    Cls_1401f46e0* tmpl = nullptr;
    const std::int32_t mode = 0;
    const char flag = 1;
    if (p801ext::read_key_file(t, name, &flag, &mode) == 0) tmpl = model::pair_nth(&t->inter_obj, 1);
    for (std::int32_t i = 1; i <= n; i = i + 1) {
        if (model::model_is_3d(m) && i == 1) continue;
        Cls_1401f46e0* a = model::pair_append(&m->inter_obj);
        if (a == nullptr) {
            delete t;  // 1401cb2c0
            return false;
        }
        a->u170 = 2;
        a->u168 = i;
        a->u16c = 1;
        if (model::model_is_3d(m)) a->u174 = 1;
        if (tmpl != nullptr) model::pair_copy_keep_ids(a, tmpl);  // 1401f9e80
        Cls_1401f46e0* b = model::pair_append(&m->inter_obj);
        if (b == nullptr) {
            delete t;
            return false;
        }
        b->u170 = 1;
        b->u168 = 1;
        b->u16c = i;
        if (model::model_is_3d(m)) b->u174 = 1;
        if (tmpl != nullptr) model::pair_copy_keep_ids(b, tmpl);
    }
    delete t;
    return true;
}

// @src 140201bc0  BCCDEF contact codes of object obj for master *a (IMPLUB.md section 3)
void bccdef_lubricant(LLOBJ_DATA* obj, const std::int32_t* a, const char* lub, const std::int32_t* w)
{
    if (*a == 0) return;
    DB_STEP_DATA* owner = at<DB_STEP_DATA*>(obj, 15000);  // obj+0x3a98
    // local_10: set only for a 3D owner; the branches that read it are taken only when the owner is
    // not 2D, so for a 2- or 3-dimensional model it is always set before use.
    std::int32_t lub_code = 0;
    if (model::model_is_3d(owner)) lub_code = -200 - *a;
    const std::int32_t nodes = at<std::int32_t>(obj, 0x880);
    auto slot = [&](std::int32_t r, std::int32_t k) -> std::int32_t& {
        return at<std::int32_t*>(obj, 0x990)[r * *w + k];
    };
    if (*lub == '\0') {
        for (std::int32_t r = 0; r < nodes; r = r + 1) {
            bool seen = false;
            for (std::int32_t k = 0; k < *w; k = k + 1) {
                if (!model::model_is_2d(owner)) {  // 1400015e0
                    if (slot(r, k) == lub_code) {
                        if (seen) {
                            slot(r, k) = 0;
                        } else {
                            slot(r, k) = -*a;
                            seen = true;
                        }
                    } else if (slot(r, k) == -*a) {
                        seen = true;
                    }
                } else if (slot(r, k) == -*a) {
                    if (seen) {
                        slot(r, k) = 0;
                    } else {
                        seen = true;
                    }
                }
            }
        }
        return;
    }
    for (std::int32_t r = 0; r < nodes; r = r + 1) {
        bool seen = false;
        for (std::int32_t k = 0; k < *w; k = k + 1) {
            if (!model::model_is_2d(owner)) {
                if (slot(r, k) == -*a) {
                    if (seen) {
                        slot(r, k) = 0;
                    } else {
                        slot(r, k) = lub_code;
                        seen = true;
                    }
                } else if (slot(r, k) == lub_code) {
                    seen = true;
                }
                continue;
            }
            if (slot(r, k) != -*a) continue;
            for (std::int32_t j = 0; j < *w; j = j + 1)
                if (slot(r, j) == 0) slot(r, j) = -*a;
            break;  // LAB_140201c29: next row
        }
    }
}

// @src 1401ac310  IMPLUB handler
bool implub_run(DB_STEP_DATA* m, const char* name, const std::int32_t* a, const std::int32_t* b, const char* lub)
{
    auto* t = new DB_STEP_DATA(3);  // 1401c8950
    Cls_1401f46e0* tmpl = nullptr;
    const std::int32_t mode = 0;
    const char flag = 1;
    if (p801ext::read_key_file(t, name, &flag, &mode) == 0) tmpl = model::pair_nth(&t->inter_obj, 1);
    Cls_1401f46e0* p = pair_find_flag(&m->inter_obj, *a, *b, 2);
    if (p == nullptr) {
        p = model::pair_append(&m->inter_obj);
        if (p == nullptr) {
            delete t;
            return false;
        }
        p->u170 = 2;
        p->u168 = *a;
        p->u16c = *b;
    }
    if (model::model_is_3d(m)) p->u174 = 1;
    if (tmpl != nullptr) model::pair_copy_keep_ids(p, tmpl);
    Cls_1401f46e0* q = pair_find_flag(&m->inter_obj, *b, *a, 1);
    if (q == nullptr) {
        q = model::pair_append(&m->inter_obj);
        if (q == nullptr) {
            delete t;
            return false;
        }
        q->u170 = 1;
        q->u168 = *b;
        q->u16c = *a;
    }
    if (model::model_is_3d(m)) q->u174 = 1;
    if (tmpl != nullptr) model::pair_copy_keep_ids(q, tmpl);
    LLOBJ_DATA* obj = key::g2obj::find_object(*m, *b);  // 1401d6ea0
    if (obj != nullptr) bccdef_lubricant(obj, a, lub, &at<std::int32_t>(m, 0x1640));
    delete t;
    return true;
}

// @src 1401b82b0  IMPEQP body: KEY file into the live model, then find-or-create object id
LLOBJ_DATA* impeqp_read(DB_STEP_DATA* m, const char* name, std::int32_t id)
{
    const char flag = 0;
    const std::int32_t mode = 0;
    if (p801ext::read_key_file(m, name, &flag, &mode) != 0) return nullptr;
    return model::llobj_ctor::get_or_create(m, id);  // 14019ccc0
}

// @src 1401ac680  IMPEQP handler (returns 1)
bool impeqp_run(DB_STEP_DATA* m, const char* name, const std::int32_t* id)
{
    impeqp_read(m, name, *id);
    return true;
}

} // namespace

// ---- dispatcher cases ----------------------------------------------------------------------------
// Every case starts with the dispatcher's status 0 (local_56d8 = 0 at 1403790..) and ends with
// `status == 0 ? 0 : 2`.

// part of @src 140378e00: case 0xb, EXECMD (the dispatcher itself is p5-01's)
int cmd_execmd(model::DB_STEP_DATA* m, std::FILE* f)
{
    (void)m;
    std::int32_t mode = 0;
    (void)key::read_i(f, &mode, false);  // 140413de0, status not stored
    char line[0x400];
    if (std::fgets(line, 0x400, f) == nullptr) return 2;
    char cmd[0x400];
    std::strcpy(cmd, line);  // 140484f90
    if (mode == 1) {
        p801ext::crt_system(cmd);  // 14047e190
    } else if (mode == 2) {
        p801ext::launch_kind2(1, cmd);  // 140224100
    } else if (mode == 3) {
        p801ext::launch_kind3(1, cmd);  // 140224130
    } else if (mode == 4) {
        p801ext::launch_kind4(1, cmd);  // 140224160
    } else {
        p801ext::launch_kind0(1, cmd);  // 1402240d0
    }
    return 0;
}

// case 0xf, GENINT
int cmd_genint(model::DB_STEP_DATA* m, std::FILE* f)
{
    // local_5b18: uninitialised when the reader hits EOF; the port starts it empty
    char name[0x400] = {};
    (void)key::read_string_next_line(f, name, 0x400);  // 140413ab0, status not stored
    genint_run(m, name);
    return 0;
}

// case 0x10, IMPLUB
int cmd_implub(model::DB_STEP_DATA* m, std::FILE* f)
{
    std::int32_t a = 0, b = 0, c = 0;
    (void)key::read_iii(f, &a, &b, &c, nullptr);  // 140413fb0, status not stored
    char name[0x400];
    if (std::fgets(name, 0x400, f) == nullptr) return 2;
    model::trim_right_blanks(name);  // 140323c50
    const char lub = c != 0 ? 1 : 0;
    implub_run(m, name, &a, &b, &lub);
    return 0;
}

// case 0x11, IMPEQP
int cmd_impeqp(model::DB_STEP_DATA* m, std::FILE* f)
{
    std::int32_t id = 0;  // local_56e8: the IMPLUB case's slot, 0 at entry
    (void)key::read_i(f, &id, false);  // status not stored
    char name[0x400];
    if (std::fgets(name, 0x400, f) == nullptr) return 2;
    model::trim_right_blanks(name);
    impeqp_run(m, name, &id);
    return 0;
}

// case 0x15, ARMCTL
int cmd_armctl(model::DB_STEP_DATA* m, std::FILE* f)
{
    (void)m;
    int count = 0;
    if (key::read_tokens(f, &count, 3) != 0) return 2;  // 140413900
    const std::int32_t v1 = key::token_int(0);         // 14047b6c0(140413a50(0))
    const std::int32_t v2 = key::token_int(1);
    const std::int32_t v3 = key::token_int(2);
    std::remove("DEF_ARM_CTL.DAT");  // 14047e29c (DeleteFileA), result ignored
    // 1401b1880 / 1401b18f0: FILE handle, fopen(name, "w") (str 140554a04); the SfOStream
    // (1401b1bd0) writes each value with operator<<(int) (140159340: decimal, '-' for negatives)
    // and std::endl (140007a50: '\n' + flush); 1401b1940 fclose.
    std::FILE* out = fsopen("DEF_ARM_CTL.DAT", "w");
    if (out != nullptr) {
        std::fprintf(out, "%d\n", v1);
        std::fflush(out);
        std::fprintf(out, "%d\n", v2);
        std::fflush(out);
        std::fprintf(out, "%d\n", v3);
        std::fflush(out);
        std::fclose(out);
    }
    return 0;
}

// case 0x11f + 10, MORP
int cmd_morp(model::DB_STEP_DATA* m, std::FILE* f)
{
    std::int32_t n = 0;
    std::int32_t mval = 0;
    int status = key::read_ii(f, &n, &mval);  // 140413ee0 (stored, not tested)
    DoubleList doubles;
    StringList strings;
    for (std::int32_t i = 0; i < n; i = i + 1) {
        double d = 0.0;  // local_5d18 (the reader zeroes it)
        status = key::read_d(f, &d);  // 140414990
        if (status != 0) return 2;
        doubles.push_back(&d);
    }
    const std::int32_t ns = n * 4 + 2;
    for (std::int32_t i = 0; i < ns; i = i + 1) {
        char line[0x400];
        status = key::read_string_line(f, line, 0x400);  // 140413c60
        if (status != 0) return 2;
        strings.push_back(std::string(line));  // 1400066a0, 14032f880
    }
    morp_run(m, static_cast<std::uint32_t>(mval), doubles, strings);
    return status == 0 ? 0 : 2;
}

// case 0x11f + 0xb, GFREAD
int cmd_gfread(model::DB_STEP_DATA* m, std::FILE* f)
{
    std::int32_t id = 0;
    if (key::read_i(f, &id, false) != 0) return 2;  // 140413de0
    if (model::llobj_ctor::get_or_create(m, id) == nullptr) return 2;  // 14019ccc0
    char name[0x400];
    if (std::fgets(name, 0x400, f) == nullptr) return 2;
    model::trim_right_blanks(name);
    gfread::fun_1401aa9a0(m, &id, name);
    return 0;
}

} // namespace def::cmd
