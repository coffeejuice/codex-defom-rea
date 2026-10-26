// The group 6 command dispatcher FUN_140378e00 (packet p5-01). Header: defcmd/dispatch.hpp.
// Decompile deform-rea-work/out/bm_cmd.c, listing out/p501/disp_asm.txt.
#define _CRT_SECURE_NO_WARNINGS
#include <defcmd/dispatch.hpp>

#include <cstring>
#include <string>

#include <defkey/line_io.hpp>
#include <defmodel/layout/cls_1401dbfe0.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/llobj/object_list.hpp>
#include <defmodel/step/db_step.hpp>
#include <defmodel/strtrim.hpp>
#include <defrt/port.hpp>
#include <defrt/stdlib/legacy_printf.hpp>
#include <defrt/stdlib/refcount.hpp>
#include <defrt/stdlib/str_helpers.hpp>

#include <defdbio/step/db_info_read.hpp>

namespace def::cmd {

using model::DB_STEP_DATA;
using model::field;

namespace {

// part of @src 140378e00  1401d6ea0: object `id` of the model's object list (0 = none)
model::LLOBJ_DATA* find_object(DB_STEP_DATA* m, std::int32_t id)
{
    return model::object_list_find(&m->objects, id);
}

// part of @src 140378e00  case 0 PROBID: the next line into +0x340 (or a scratch buffer when
// bit 3 of the KEY reader flags +0x1740 is set)
int case_probid(DB_STEP_DATA* m, std::FILE* f)
{
    char scratch[0x400];
    if ((m->u1740 & 8) == 0) return key::read_string_next_line(f, m->u340, 0x400);
    return key::read_string_next_line(f, scratch, 0x400);
}

// part of @src 140378e00  case 2 DBREAD: "DBREAD n" then the DB name. Read flag bit 0 of +0x1740
// skips it; n == 0 takes the last valid directory entry. The step goes through 1401cc2a0, then
// PROBID (+0x340) becomes the DB file's stem. The read status of the first line is not checked.
int case_dbread(DB_STEP_DATA* m, std::FILE* f)
{
    std::int32_t step = 0;
    char name[0x400];
    (void)key::read_i(f, &step, false);
    if (std::fgets(name, 0x400, f) == nullptr) return 2;
    if ((m->u1740 & 1) != 0) return 0;
    model::trim_right_blanks(name);
    std::strcpy(m->u1748, name);
    model::Cls_1401dbfe0* db = m->db;
    if (db == nullptr) return 2;
    if (dbio::load_db_info(*db, name, false) != 0) return 2;
    if (step == 0) {
        const auto* dir = static_cast<const std::byte*>(db->uad8);
        for (std::int32_t i = db->uac8 - 1; i >= 0; --i) {
            if (field<std::int32_t>(dir, static_cast<std::size_t>(i) * 0x28 + 0x24) != 0) {
                step = field<std::int32_t>(dir, static_cast<std::size_t>(i) * 0x28 + 4);
                break;
            }
        }
        rt::crt::fprintf(stdout, "DBREAD : Read the last step from database\n"); // str 1405551b0
    } else {
        const std::int32_t asked = step;
        step = db_find_step(db, step);
        if (asked == step)
            rt::crt::fprintf(stdout, "DBREAD : Read step %d from database\n", step); // str 1405551e0
        else
            rt::crt::fprintf(stdout, "DBREAD : Read step %d (<- %d) from database\n", step,
                             asked); // str 140555208
    }
    if (dispatch_hooks().db_read_step(m, db, step, 1, 0) != 0) return 2;
    const std::string stem = rt::Cls_1401b19c0(std::string(m->u1748)).stem(); // 1401b19c0, 1401b1a30
    std::strncpy(m->u340, stem.c_str(), 0x400);
    return 0;
}

// part of @src 140378e00  case 3 KFREAD: "KFREAD [n]" then the file name; a file of the same name
// as the one being read (+0x2f8a) is refused with 2
int case_kfread(DB_STEP_DATA* m, std::FILE* f)
{
    std::int32_t ignored = 0;
    char name[0x400];
    (void)key::read_i(f, &ignored, false);
    if (std::fgets(name, 0x400, f) == nullptr) return 2;
    model::trim_right_blanks(name);
    if (std::strcmp(name, reinterpret_cast<const char*>(m) + 0x2f8a) == 0) return 2;
    const char reset = 0;
    const std::int32_t flags = 0;
    if (dispatch_hooks().read_keyword_file(m, name, &reset, &flags) != 0) return 2;
    return 0;
}

// part of @src 140378e00  case 4 GENDB: "GENDB a b c" then the DB name. 1401d7f40 first; with
// the remesh flag (+0x2f89) MESHNO (+0x840) goes up by one; a in 1..2 else 0; b 0 -> 2, 2 -> 1,
// else 2; c in 0..2 else 0, and c 1 or 2 restores NSTART (+0x8b0) as it was before 1401d7f40.
// Direct DB writing (+0x8, 1401c5690) runs 1401bb550, otherwise 1401b9a40 (returns 0, writes
// nothing). The write's result is not used: the case returns the token read status.
int case_gendb(DB_STEP_DATA* m, std::FILE* f)
{
    const std::int32_t nstart = field<std::int32_t>(m, 0x8b0);
    finish_key_read(m);
    int count = 0;
    const int status = key::read_tokens(f, &count, 3);
    if (m->u2f89 != 0) field<std::int32_t>(m, 0x840) += 1;
    std::int32_t v = key::token_int(0);
    const std::int32_t a = (v < 1 || v > 2) ? 0 : v;
    v = key::token_int(1);
    const std::int32_t b = v == 0 ? 2 : (v == 2 ? 1 : 2);
    v = key::token_int(2);
    std::int32_t c = 0;
    if (v >= 0 && v < 3) {
        c = v;
        if (c == 1 || c == 2) field<std::int32_t>(m, 0x8b0) = nstart;
    }
    char name[0x400];
    if (std::fgets(name, 0x400, f) == nullptr) return 2;
    model::trim_right_blanks(name);
    if (field<std::uint8_t>(m, 0x8) != 0) // 1401c5690
        (void)dispatch_hooks().gendb_write(m, name, b, a, c);
    // else 1401b9a40(m, name, b, a, 0, c): returns 0
    return status == 0 ? 0 : 2;
}

// part of @src 140378e00  case 8 TLOC t: 1401a99a0, +0x8c8 = t plus +0xd90 (or +0xd98 when
// +0xda0 != 0)
int case_tloc(DB_STEP_DATA* m, std::FILE* f)
{
    double t = 0.0;
    if (key::read_d(f, &t) != 0) return 2;
    // @src 1401a99a0  (returns 1, unused)
    if (field<std::int32_t>(m, 0xda0) == 0)
        field<double>(m, 0x8c8) = field<double>(m, 0xd90) + t;
    else
        field<double>(m, 0x8c8) = field<double>(m, 0xd98) + t;
    return 0;
}

// part of @src 140378e00  case 10 KFWRIT: the next line is the file name; the writer's result is
// not used (14037c636), and neither is the read status (the test at 14037c5ff reads the never
// written status slot, 0).
int case_kfwrit(DB_STEP_DATA* m, std::FILE* f)
{
    char name[0x400] = {};
    (void)key::read_string_next_line(f, name, 0x400);
    const std::uint32_t flags = 0;
    (void)write_key_file(m, name, &flags);
    return 0;
}

// part of @src 140378e00  case 0x9a DSTROKE obj d: moves the stroke (+0x22d0..) back by d along
// the direction (+0x21b0..) in 3D, by d on X and Y in 2D. A read error or a missing object
// returns the read status.
int case_dstroke(DB_STEP_DATA* m, std::FILE* f)
{
    std::int32_t obj = 0;
    double d = 0.0;
    const int status = key::read_i_d(f, &obj, &d);
    if (status == 0) {
        model::LLOBJ_DATA* o = find_object(m, obj);
        if (o != nullptr) {
            if (!rt::dimension_is_2(m)) {
                field<double>(o, 0x22d0) = field<double>(o, 0x22d0) - field<double>(o, 0x21b0) * d;
                field<double>(o, 0x22d8) = field<double>(o, 0x22d8) - field<double>(o, 0x21b8) * d;
                field<double>(o, 0x22e0) = field<double>(o, 0x22e0) - field<double>(o, 0x21c0) * d;
            } else {
                field<double>(o, 0x22d0) = field<double>(o, 0x22d0) - d;
                field<double>(o, 0x22d8) = field<double>(o, 0x22d8) - d;
            }
        }
    }
    return status == 0 ? 0 : 2;
}

// part of @src 140378e00  id 0x11e ACTOUT n: the next line, '\n' cut, into ACTION_DB_STEP's text
// (140382320, strcpy)
int case_actout(DB_STEP_DATA* m, std::FILE* f)
{
    std::int32_t ignored = 0;
    char line[0x400];
    if (key::read_i(f, &ignored, false) != 0) return 2;
    if (std::fgets(line, 0x400, f) == nullptr) return 2;
    for (unsigned i = 0; i < std::strlen(line); ++i) {
        if (line[i] == '\n') line[i] = '\0';
    }
    std::strcpy(static_cast<model::ACTION_DB_STEP*>(m)->text, line); // @src 140382320
    return 0;
}

} // namespace

// @src 140378e00
int dispatch(DB_STEP_DATA* m, std::FILE* f, int id)
{
    const DispatchHooks& h = dispatch_hooks();
    if (id == kACTOUT) return case_actout(m, f);
    if (id < kACTOUT) {
        switch (id) {
        case kPROBID: return case_probid(m, f) == 0 ? 0 : 2;
        case kRUNTYP: return key::read_i(f, &m->u1738, false) == 0 ? 0 : 2;
        case kDBREAD: return case_dbread(m, f);
        case kKFREAD: return case_kfread(m, f);
        case kGENDB: return case_gendb(m, f);
        case kINICTC: return h.inictc(m, f);
        case kGENCTC: return h.genctc(m, f);
        case kOBJPOS: return h.objpos(m, f);
        case kTLOC: return case_tloc(m, f);
        case kWRTOBJ: return h.wrtobj(m, f);
        case kKFWRIT: return case_kfwrit(m, f);
        case kEXECMD: return h.execmd(m, f);
        case kSVSCAL: return h.svscal(m, f);
        case kCPYOBJ: return h.cpyobj(m, f);
        case kDOESCL: return h.doescl(m, f);
        case kGENINT: return h.genint(m, f);
        case kIMPLUB: return h.implub(m, f);
        case kIMPEQP: return h.impeqp(m, f);
        case kTCYCLE: return h.tcycle(m, f);
        case kGENAXS: return h.genaxs(m, f);
        case kGENSUB: return h.gensub(m, f);
        case kARMCTL: return h.armctl(m, f);
        case kDSTROKE: return case_dstroke(m, f);
        default: return 2; // 14037e1f6
        }
    }
    switch (id - 0x11f) {
    case 0: return h.cnvt3d(m, f);
    case 4: return h.remesh(m, f);
    case 5: return h.brdext(m, f);
    case 6: return h.defamg(m, f);
    case 7: return h.defint(m, f);
    case 8: return h.genbcc(m, f);
    case 9: return h.deffrz(m, f);
    case 10: return h.morp(m, f);
    case 11: return h.gfread(m, f);
    default: return 2; // 14037e1f6
    }
}

} // namespace def::cmd
