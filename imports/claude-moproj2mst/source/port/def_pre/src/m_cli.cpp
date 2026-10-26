// The -m utility path of DEF_PRE_64. See m_cli.hpp.
#define _CRT_SECURE_NO_WARNINGS // the original uses the plain CRT string and stdio calls
#include <def_pre/m_cli.hpp>

#include <defdbio/dbcore/dbops.hpp>
#include <defdbio/step/db_info_read.hpp>
#include <defmodel/step/db_step.hpp>

#include <cstdio>
#include <cstdlib>
#include <new>

#include <defmodel/layout/db_step_data.hpp>
#include <defcmd/ctc3d/common.hpp> // owner of DAT_14077a7bc / DAT_14077a7c0 (p67-7's statics image)
#include <cstdlib>
#include <cstring>

#ifdef _WIN32
#include <share.h>
#endif

namespace def::pre {

namespace {

// _fsopen(name, "wt", 0x40) through FUN_14047b934 (str 14050c8a8).
std::FILE* open_wt(const char* name)
{
#ifdef _WIN32
    return _fsopen(name, "wt", _SH_DENYNO);
#else
    return std::fopen(name, "w");
#endif
}

// The header, stepList and lastStepNo modes pass the FILE* of fopen to fprintf unchecked; a
// NULL stream makes the CRT's parameter check end the process (oracle: -o into a missing
// folder exits 0xC0000417 with nothing written).
std::FILE* unchecked(std::FILE* f)
{
    if (f == nullptr) {
        abnormal_exit(kStatusInvalidCrtParameter);
    }
    return f;
}

// The value of an option: the rest of the argument ("-dname") or, when the argument has fewer
// than 3 characters, the next argument (argv[argc] is NULL past the end).
const char* option_value(char** p, int* n, char*** next)
{
    char* arg = p[1];
    if (std::strlen(arg) < 3) {
        *next = p + 2;
        --*n;
        return p[2];
    }
    return arg + 2;
}

// The parser compares each value before anything else; a NULL value faults there
// (oracle: a trailing -o/-d/-m/-t exits 0xC0000005 with nothing written).
char* deref(const char* v)
{
    if (v == nullptr) {
        abnormal_exit(kStatusAccessViolation);
    }
    return const_cast<char*>(v);
}

// Value of -d/-o/-r/-s/-t: quotes stripped unless it is empty.
char* plain_value(char** p, int* n, char*** next)
{
    char* v = deref(option_value(p, n, next));
    if (*v != '\0') {
        strip_quotes(v);
    }
    return v;
}

} // namespace

// @src 14001ce7b  the message unit and NEWEQ.MSG (packet p5-01; listing out/p501/neweq_asm.txt)
void open_neweq_msg()
{
    const std::int32_t p = -1; // main passes -1
    // 14001ceae..14001cebd: unit = |p| (NEG, CMOVS), DAT_14077a7c0 = 0, DAT_14077a7bc = unit
    std::int32_t unit = -p;
    if (-p < 0) unit = p;
    def::cmd::ctc3d::dat_14077a7c0 = 0;
    std::int32_t& msg_unit = def::cmd::ctc3d::dat_14077a7bc;
    msg_unit = unit;
    if (p == 6) return; // 14001cebf
    if (unit == 0) {    // 14001d024
        unit = 6;
        msg_unit = 6;
    }
    // unit 1 (the only value main gives): for a negative p, OPEN(1, 'NEWEQ.MSG', 'UNKNOWN') and
    // CLOSE(1, STATUS='DELETE') remove an old file, then OPEN(1, 'NEWEQ.MSG', 'UNKNOWN') + REWIND
    // leave an empty file open for the whole run. defrt/ftn/fio.hpp writes unit-1 records into
    // it (closed between statements). Notes: docs/re/status/p5-01-neweq.md.
    if (p < 0) std::remove("NEWEQ.MSG");
    if (std::FILE* f = std::fopen("NEWEQ.MSG", "ab")) {
        std::fclose(f);
    }
}

// @src 14001b7b0  command-line parser
std::uint32_t parse_m_args(int argc, char** argv, MArgs& a)
{
    std::uint32_t flags = 0;
    a.mode = 0;
    a.out[0] = '\0';
    a.db[0] = '\0';
    a.db2[0] = '\0';
    int n = argc;
    char** p = argv; // p[1] is the argument in hand
    while (1 < n) {
        char* arg = p[1];
        char** next = p + 1;
        if (arg[0] != '-') {
            break;
        }
        switch (arg[1]) {
        case 'D':
        case 'd': {
            char* v = plain_value(p, &n, &next);
            if ((flags & kFlagDb) == 0) {
                std::strcpy(a.db, v);
                flags |= kFlagDb;
            } else {
                std::strcpy(a.db2, v);
                flags |= kFlagDb2;
            }
            break;
        }
        case 'H':
        case 'h':
            print_help();
            throw ExitRequest{0};
        case 'M':
        case 'm': {
            const char* v = deref(option_value(p, &n, &next));
            if (std::strcmp(v, "header") == 0) {
                a.mode = 1;
            } else if (std::strcmp(v, "steplist") == 0 || std::strcmp(v, "stepList") == 0) {
                a.mode = 2;
            } else if (std::strcmp(v, "lastStepNo") == 0) {
                a.mode = 3;
            } else if (std::strcmp(v, "simsteplist") == 0 || std::strcmp(v, "simStepList") == 0) {
                a.mode = 4;
            } else if (std::strcmp(v, "diff") == 0) {
                a.mode = 5;
            } else if (std::strcmp(v, "reclist") == 0) {
                a.mode = 6;
            } else if (std::strcmp(v, "summary") == 0) {
                a.mode = 7;
            } // anything else leaves the mode as it was; -m sets no flag
            break;
        }
        case 'O':
        case 'o': {
            char* v = plain_value(p, &n, &next);
            std::strcpy(a.out, v);
            flags |= kFlagOut;
            break;
        }
        case 'R':
        case 'r': {
            char* v = plain_value(p, &n, &next);
            a.r = std::atoi(v); // FUN_14047b6c0
            flags |= kFlagR;
            break;
        }
        case 'S':
        case 's': {
            char* v = plain_value(p, &n, &next);
            const std::int32_t value = std::atoi(v);
            if ((flags & kFlagS1) == 0) {
                flags |= kFlagS1;
                a.s1 = value;
            } else {
                flags |= kFlagS2;
                a.s2 = value;
            }
            break;
        }
        case 'T':
        case 't': {
            char* v = plain_value(p, &n, &next);
            a.t = std::atoi(v);
            flags |= kFlagT;
            break;
        }
        default:
            break; // other letters, and "-" alone, are skipped
        }
        --n;
        p = next;
    }
    // @src 14009b170  omitted: licence (network licence check after the loop)
    return flags;
}

// @src 14001c500  run the selected mode
void dispatch_m(std::uint32_t flags, MArgs& a)
{
    // Two Cls_1401dbfe0 (FUN_1401dbfe0) and two DB_STEP_DATA(3) (FUN_1401c8950) are built here
    // and destroyed at the end (FUN_1401cb2c0, FUN_1401dc1c0); the modes use the first info
    // object only, and nothing else of them is visible. The two models are built in zeroed
    // storage: the original keeps them in a fresh (zeroed) stack frame, so members no
    // constructor writes read 0 (p5-01, as main's session object).
    struct ZeroedStep {
        void* p = std::calloc(1, sizeof(def::model::DB_STEP_DATA));
        def::model::DB_STEP_DATA* m = new (p) def::model::DB_STEP_DATA(3);
        ~ZeroedStep()
        {
            m->~DB_STEP_DATA();
            std::free(p);
        }
    };
    ZeroedStep step1;
    DbInfo info;
    ZeroedStep step2;
    std::fprintf(stdout, "\n");
    std::fprintf(stdout, "*** DEF_PRE.EXE ***\n");
    if ((flags & kFlagOut) == 0) {
        std::fprintf(stdout, "   Output file name should be set.\n"
                             "   Please see the help screen (DEF_PRE,EXE -h)\n");
        throw ExitRequest{-1};
    }
    if ((flags & kFlagDb) == 0) {
        std::fprintf(stdout, "   DB name should be set.\n"
                             "   Please see the help screen (DEF_PRE,EXE -h)\n");
        throw ExitRequest{-1};
    }
    switch (a.mode) {
    case 1:
        mode_header(a.db, info, a.out);
        break;
    case 2:
        mode_step_list(a.db, info, a.out);
        break;
    case 3:
        mode_last_step_no(a.db, info, a.out);
        break;
    case 4:
        mode_sim_step_list(a.db, info, a.out, &a.t);
        break;
    default:
        break; // 0 and the parsed-only modes 5..7
    }
}

// @src 14001bc80  -m header
bool mode_header(char* db, DbInfo& info, const char* out)
{
    std::int32_t err = 0;
    if (dbio::dbcore::is_deform_db(db, &err) == 0) {
        std::FILE* f = unchecked(open_wt(out));
        std::fprintf(f, "Not a valid DEFORM database!\n");
        std::fclose(f);
        return false;
    }
    if (dbio::load_db_info(info, db, false) != 0) {
        std::FILE* f = unchecked(open_wt(out));
        std::fprintf(f, "Reading error!\n");
        std::fclose(f);
        return false;
    }
    std::FILE* f = unchecked(open_wt(out));
    write_header(f, info);
    std::fclose(f);
    return true;
}

// part of @src 14001bc80
void write_header(std::FILE* f, const DbInfo& info)
{
    char release[9];
    char initial[9];
    char current[9];
    std::strncpy(release, info.uaa1, 8);
    release[8] = '\0';
    std::strncpy(initial, info.uaa9, 8);
    initial[8] = '\0';
    std::strncpy(current, info.uab1, 8);
    current[8] = '\0';
    std::fprintf(f, "DB %4d\n", info.uabc);
    std::fprintf(f, "*\n");
    std::fprintf(f, "DB Version     : [%8d]\n", info.uabc);
    std::fprintf(f, "ReleaseDate    : [%8s]\n", release);
    std::fprintf(f, "InitialSimDate : [%8s]\n", initial);
    std::fprintf(f, "CurrentSimDate : [%8s]\n", current);
    std::fprintf(f, "SimType        : [%8d]\n", info.uac4);
    std::fprintf(f, "NoRecords      : [%8d]\n", info.uac8);
    std::fprintf(f, "NoStoredSteps  : [%8d]\n", info.uacc);
}

// @src 14001be50  -m stepList
bool mode_step_list(char* db, DbInfo& info, const char* out)
{
    if (dbio::load_db_info(info, db, false) != 0) {
        return false;
    }
    std::FILE* f = unchecked(open_wt(out));
    write_step_list(f, info);
    std::fclose(f);
    return true;
}

// part of @src 14001be50
void write_step_list(std::FILE* f, const DbInfo& info)
{
    if (info.uad8 == nullptr) {
        return;
    }
    std::int32_t count = 0;
    for (std::int32_t i = 0; i < info.uac8; ++i) {
        if (model::db_info_records(info)[i].stored != 0) {
            ++count;
        }
    }
    std::fprintf(f, "%7d\n", count);
    for (std::int32_t i = 0; i < info.uac8; ++i) {
        const model::DbDirRecord& r = model::db_info_records(info)[i];
        if (r.stored != 0) {
            std::fprintf(f, "%7d\n", r.step);
        }
    }
}

// @src 14001bf40  -m lastStepNo
bool mode_last_step_no(char* db, DbInfo& info, const char* out)
{
    if (dbio::load_db_info(info, db, false) != 0) {
        return false;
    }
    std::FILE* f = unchecked(open_wt(out));
    write_last_step_no(f, info);
    std::fclose(f);
    return true;
}

// part of @src 14001bf40
void write_last_step_no(std::FILE* f, const DbInfo& info)
{
    if (info.uad8 == nullptr) {
        return;
    }
    std::int32_t last = 0;
    for (std::int32_t i = 0; i < info.uac8; ++i) {
        const model::DbDirRecord& r = model::db_info_records(info)[i];
        if (r.stored != 0) {
            last = r.step;
        }
    }
    std::fprintf(f, "%7d\n", last);
}

namespace {

} // namespace

// @src 14001c100  -m simStepList
bool mode_sim_step_list(char* db, DbInfo& info, const char* out, const std::int32_t* t)
{
    if (dbio::load_db_info(info, db, false) != 0 || info.uad8 == nullptr) {
        return false;
    }
    // The DB name is cut at '/' into a table nothing reads (it changes main's -d buffer only).
    char* tokens[256];
    std::int32_t ntok = 0;
    rt::split_tokens(db, "/", tokens, &ntok); // 14001c090, str 140508548
    return write_sim_step_list(out, info, *t);
}

// part of @src 14001c100
bool write_sim_step_list(const char* out, const DbInfo& info, std::int32_t t)
{
    // Locals of 14001c100: they live across operations (RSP+0x38 b, +0x3c c, +0x40 a).
    std::int32_t b = 0;
    std::int32_t c = 0;
    std::int32_t a = 0;
    const std::int32_t nrec = info.uac8; // RSP+0x34, EBP
    const std::int32_t last_op = dbio::last_stored_sim_no(info);
    std::int32_t nops = 0;
    for (std::int32_t op = 1; op <= last_op; ++op) {
        if (rt::operation_steps(&op, model::db_info_records(info), &nrec, &a, &b, &c)) {
            ++nops;
        }
    }
    if (nops <= 0) {
        return false;
    }
    std::FILE* f = open_wt(out);
    if (f == nullptr) {
        return false; // 14001c226: this mode checks the stream
    }
    std::fprintf(f, "%7d %7d\n", nops, 4); // str 14050c8d0
    const auto rec = [&](std::int32_t i) -> const model::DbDirRecord& {
        return model::db_info_records(info)[i];
    };
    for (std::int32_t op = 1; op <= last_op; ++op) {
        if (!rt::operation_steps(&op, model::db_info_records(info), &nrec, &a, &b, &c)) {
            continue;
        }
        char name[0x100];
        std::memset(name, 0, sizeof name);
        std::snprintf(name, sizeof name, "Operation %d", op); // str 14050c8c0
        std::fprintf(f, "%7d %7d\n", op, a);
        std::fprintf(f, "%s\n", name);
        std::fprintf(f, "%7d\n", b);
        std::fprintf(f, "%7d\n", c);
        if (t == 1) {
            std::int32_t count = 0;
            for (std::int32_t i = 0; i < nrec; ++i) {
                if (rec(i).stored != 0 && rec(i).sim_no == op) {
                    ++count;
                }
            }
            std::fprintf(f, "%7d\n", count);
            for (std::int32_t i = 0; i < nrec; ++i) {
                if (rec(i).stored != 0 && rec(i).sim_no == op) {
                    std::fprintf(f, "%7d ", rec(i).step); // str 14050c8b4, no newline after
                }
            }
        } else if (t == 2) {
            std::int32_t every = dbio::step_interval(nrec);
            if (nrec < 10) {
                every = 1;
            }
            // The first and last step of the operation always, the others when their position
            // in the whole directory is a multiple of the interval.
            std::int32_t count = 0;
            for (std::int32_t i = 0; i < nrec; ++i) {
                if (rec(i).stored != 0 && rec(i).sim_no == op) {
                    if (rec(i).step == b || rec(i).step == c) {
                        ++count;
                    } else if (i % every == 0) {
                        ++count;
                    }
                }
            }
            std::fprintf(f, "%7d\n", count);
            for (std::int32_t i = 0; i < nrec; ++i) {
                if (rec(i).stored != 0 && rec(i).sim_no == op) {
                    const std::int32_t step = rec(i).step;
                    if (step == b || step == c) {
                        std::fprintf(f, "%7d ", step);
                    } else if (i % every == 0) {
                        std::fprintf(f, "%7d ", step);
                    }
                }
            }
        }
    }
    std::fclose(f);
    return true;
}

// @src 14001c9f0  main, up to the -m exit
int main_m(int argc, char** argv, bool* handled)
{
    *handled = true;
    try {
        // 140224d80 (program info setters) and 1401b8e10 (environment) run before this, in main.cpp
        open_neweq_msg();
        // @src 14009e330  omitted: licence (network-licence client settings)
        // @src 14009e340  omitted: licence
        // @src 14009e350  omitted: licence
        MArgs a;
        const std::uint32_t flags = parse_m_args(argc, argv, a);
        if (flags != 0) {
            dispatch_m(flags, a);
            return 0; // 14001cb39: exit(0)
        }
    } catch (const ExitRequest& e) {
        return e.code;
    }
    *handled = false;
    return 0;
}

} // namespace def::pre
