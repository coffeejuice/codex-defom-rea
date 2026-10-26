// Hook table of the command dispatcher (defcmd/dispatch.hpp): every case body and callee another
// packet owns defaults to the owner's function when the owner's header is in the tree, otherwise
// to a body that stops with DEF_NOT_PORTED(<the case's handler address>).
#define _CRT_SECURE_NO_WARNINGS
#include <defcmd/dispatch.hpp>

#include <defkey/kw_g2geo.hpp>

#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defrt/port.hpp>
#include <defmodel/step/db_step.hpp>

#include <cstring>

// ---- owners' headers -----------------------------------------------------------------------
#if __has_include(<defcmd/contact_cmds.hpp>)
#include <defcmd/contact_cmds.hpp>
#define P501_HAVE_P67_6 1
#endif
#if __has_include(<defcmd/cmd_objpos.hpp>)
#include <defcmd/cmd_objpos.hpp>
#define P501_HAVE_P67_1 1
#endif
#if __has_include(<defcmd/cmd_p67_4.hpp>)
#include <defcmd/cmd_p67_4.hpp>
#define P501_HAVE_P67_4 1
#endif
#if __has_include(<defcmd/p67_5.hpp>)
#include <defcmd/p67_5.hpp>
#define P501_HAVE_P67_5 1
#endif
#if __has_include(<defcmd/cmd_p8_01.hpp>)
#include <defcmd/cmd_p8_01.hpp>
#define P501_HAVE_P8_01 1
#endif
#if __has_include(<defcmd/cmd_p8_05.hpp>)
#include <defcmd/cmd_p8_05.hpp>
#define P501_HAVE_P8_05 1
#endif
#if __has_include(<defcmd/cmd_p8_07.hpp>)
#include <defcmd/cmd_p8_07.hpp>
#define P501_HAVE_P8_07 1
#endif
#if __has_include(<defcmd/cmd_p8_08.hpp>)
#include <defcmd/cmd_p8_08.hpp>
#define P501_HAVE_P8_08 1
#endif
#if __has_include(<defcmd/cmd_p9_01.hpp>)
#include <defcmd/cmd_p9_01.hpp>
#define P501_HAVE_P9_01 1
#endif
#if __has_include(<defcmd/key_file.hpp>)
#include <defcmd/key_file.hpp>
#define P501_HAVE_KEY_FILE 1
#endif
// P501_DBREAD_LIVE: the one switch that binds db_read_step to p-dbread's DBREAD core
// (def::dbio::dbread::db_read_step, 1401cc2a0). Live by default; build with
// -DP501_DBREAD_LIVE=0 to fall back to the DEF_NOT_PORTED default.
#ifndef P501_DBREAD_LIVE
#define P501_DBREAD_LIVE 1
#endif
#if P501_DBREAD_LIVE && __has_include(<defdbio/dbread/dbread.hpp>)
#include <defdbio/dbread/dbread.hpp>
#define P501_HAVE_DBREAD 1
#endif
#if __has_include(<defdbio/gendb/gendb.hpp>)
#include <defdbio/gendb/gendb.hpp>
#define P501_HAVE_GENDB 1
#endif
#if __has_include(<defdbio/gendb/pre_steps.hpp>)
#include <defdbio/gendb/pre_steps.hpp>
#define P501_HAVE_PRE_STEPS 1
#endif

// P5 gate markers (harness/gate/p5_gate.py): defined only while the interim copies are compiled
#ifndef P501_HAVE_KEY_FILE
extern "C" const char p501_interim_1401b5f00 = 1;
#endif

namespace def::cmd {

using model::DB_STEP_DATA;

namespace {

#define P501_NOT_PORTED(name, addr) \
    int np_##name(DB_STEP_DATA*, std::FILE*) { DEF_NOT_PORTED(addr); }

P501_NOT_PORTED(inictc, 0x1402049b0)
P501_NOT_PORTED(genctc, 0x1401a2e90)
P501_NOT_PORTED(objpos, 0x14021d0b0)
P501_NOT_PORTED(wrtobj, 0x14029e250)
P501_NOT_PORTED(svscal, 0x140204bc0)
P501_NOT_PORTED(cpyobj, 0x1401613a0)
P501_NOT_PORTED(doescl, 0x1401ac710)
P501_NOT_PORTED(tcycle, 0x1401dde10)
P501_NOT_PORTED(genaxs, 0x1401af680)
P501_NOT_PORTED(gensub, 0x14037e5c0)
P501_NOT_PORTED(genbcc, 0x1401ac6c0)
P501_NOT_PORTED(execmd, 0x140223790)
P501_NOT_PORTED(genint, 0x1401ac020)
P501_NOT_PORTED(implub, 0x1401ac310)
P501_NOT_PORTED(impeqp, 0x1401ac680)
P501_NOT_PORTED(armctl, 0x140378e00)
P501_NOT_PORTED(morp, 0x1401aa750)
P501_NOT_PORTED(gfread, 0x1401aa9a0)
P501_NOT_PORTED(brdext, 0x1401a9ac0)
P501_NOT_PORTED(defamg, 0x1401a9be0)
P501_NOT_PORTED(remesh, 0x1401a9a10)
P501_NOT_PORTED(defint, 0x1401aa000)
P501_NOT_PORTED(deffrz, 0x1401aa370)
P501_NOT_PORTED(cnvt3d, 0x1401467f0)

#ifndef P501_HAVE_KEY_FILE
#pragma message("P501-INTERIM 1401b5f00 compiled (owner p8-07, defcmd/key_file.hpp absent)")

// 1401b5f00 interim copy (owner p8-07, defcmd/key_file.hpp; no @src tag here): reset flags
// (1401d7f10), current file name at +0x2f8a, open "r", flags into +0x1740, the KEY loop, name
// back to " " (str 140525634), 1401d7f40 when reset. Open error: message on stdout, 1.
int interim_read_keyword_file(DB_STEP_DATA* m, const char* name, const char* reset,
                              const std::int32_t* flags)
{
    if (*reset != 0) model::step_clear_u2f88(*m);
    char* cur = reinterpret_cast<char*>(m) + 0x2f8a;
    std::strcpy(cur, name);
    std::FILE* f = _fsopen(name, "r", 0x40); // str 14052553c
    if (f == nullptr) {
        std::fprintf(stdout, "ERROR: Keyword file %s open error!\n", name);
        return 1;
    }
    m->u1740 = *flags;
    ModelKeyHooks hooks(m);
    const int st = key::read_key_stream(f, hooks, stdout);
    m->u1740 = 0;
    std::fclose(f);
    std::strcpy(cur, " ");
    if (*reset != 0) finish_key_read(m);
    return st;
}
#endif
#ifndef P501_HAVE_DBREAD
int np_db_read_step(DB_STEP_DATA*, model::Cls_1401dbfe0*, std::int32_t, std::uint8_t, std::uint8_t)
{
    DEF_NOT_PORTED(0x1401cc2a0);
}
#endif
std::int32_t np_gendb_write(DB_STEP_DATA*, const char*, std::int32_t, std::int32_t, std::int32_t)
{
    DEF_NOT_PORTED(0x1401bb550);
}
// 1402a1df0 short object writer (packet p5-shortkey, defkey/kw_g2geo.hpp)
int short_object_writer(DB_STEP_DATA* m, std::FILE* f, model::LLOBJ_DATA* obj, std::int32_t opt)
{
    return key::g2geo::write_object_short(*m, f, obj, static_cast<std::uint32_t>(opt));
}
// 1402059a0 does nothing for an object without surface (140160d90: byte +0x3990); the rest is
// not ported yet
void np_object_surface_update(model::LLOBJ_DATA* obj, bool)
{
    if (!model::geo_kw::has_surface_flag(*obj)) return; // 140160d90
    DEF_NOT_PORTED(0x1402059a0);
}

DispatchHooks defaults()
{
    DispatchHooks h{};
#ifdef P501_HAVE_P67_6
    h.inictc = [](DB_STEP_DATA* m, std::FILE* f) { return cmd_inictc(m, f); };
    h.genctc = [](DB_STEP_DATA* m, std::FILE* f) { return cmd_genctc(m, f); };
#else
    h.inictc = np_inictc;
    h.genctc = np_genctc;
#endif
#ifdef P501_HAVE_P67_1
    h.objpos = [](DB_STEP_DATA* m, std::FILE* f) { return cmd_objpos(m, f); };
#else
    h.objpos = np_objpos;
#endif
#ifdef P501_HAVE_P67_4
    h.wrtobj = [](DB_STEP_DATA* m, std::FILE* f) { return cmd_wrtobj(m, f); };
    h.svscal = [](DB_STEP_DATA* m, std::FILE* f) { return cmd_svscal(m, f); };
    h.cpyobj = [](DB_STEP_DATA* m, std::FILE* f) { return cmd_cpyobj(m, f); };
    h.doescl = [](DB_STEP_DATA* m, std::FILE* f) { return cmd_doescl(m, f); };
#else
    h.wrtobj = np_wrtobj;
    h.svscal = np_svscal;
    h.cpyobj = np_cpyobj;
    h.doescl = np_doescl;
#endif
#ifdef P501_HAVE_P67_5
    h.tcycle = [](DB_STEP_DATA* m, std::FILE* f) { return cmd_tcycle(*m, f); };
    h.genaxs = [](DB_STEP_DATA* m, std::FILE* f) { return cmd_genaxs(*m, f); };
    h.gensub = [](DB_STEP_DATA* m, std::FILE* f) { return cmd_gensub(*m, f); };
    h.genbcc = [](DB_STEP_DATA* m, std::FILE* f) { return cmd_genbcc(*m, f); };
#else
    h.tcycle = np_tcycle;
    h.genaxs = np_genaxs;
    h.gensub = np_gensub;
    h.genbcc = np_genbcc;
#endif
#ifdef P501_HAVE_P8_01
    h.execmd = [](DB_STEP_DATA* m, std::FILE* f) { return cmd_execmd(m, f); };
    h.genint = [](DB_STEP_DATA* m, std::FILE* f) { return cmd_genint(m, f); };
    h.implub = [](DB_STEP_DATA* m, std::FILE* f) { return cmd_implub(m, f); };
    h.impeqp = [](DB_STEP_DATA* m, std::FILE* f) { return cmd_impeqp(m, f); };
    h.armctl = [](DB_STEP_DATA* m, std::FILE* f) { return cmd_armctl(m, f); };
    h.morp = [](DB_STEP_DATA* m, std::FILE* f) { return cmd_morp(m, f); };
    h.gfread = [](DB_STEP_DATA* m, std::FILE* f) { return cmd_gfread(m, f); };
#else
    h.execmd = np_execmd;
    h.genint = np_genint;
    h.implub = np_implub;
    h.impeqp = np_impeqp;
    h.armctl = np_armctl;
    h.morp = np_morp;
    h.gfread = np_gfread;
#endif
#ifdef P501_HAVE_P8_05
    h.brdext = [](DB_STEP_DATA* m, std::FILE* f) { return cmd_brdext(m, f); };
#else
    h.brdext = np_brdext;
#endif
#ifdef P501_HAVE_P8_07
    h.defamg = [](DB_STEP_DATA* m, std::FILE* f) { return cmd_defamg(m, f); };
#else
    h.defamg = np_defamg;
#endif
#ifdef P501_HAVE_P8_08
    h.remesh = [](DB_STEP_DATA* m, std::FILE* f) { return cmd_remesh(m, f); };
    h.defint = [](DB_STEP_DATA* m, std::FILE* f) { return cmd_defint(m, f); };
    h.deffrz = [](DB_STEP_DATA* m, std::FILE* f) { return cmd_deffrz(m, f); };
#else
    h.remesh = np_remesh;
    h.defint = np_defint;
    h.deffrz = np_deffrz;
#endif
#ifdef P501_HAVE_P9_01
    h.cnvt3d = [](DB_STEP_DATA* m, std::FILE* f) { return cmd_cnvt3d(m, f); };
#else
    h.cnvt3d = np_cnvt3d;
#endif
#ifdef P501_HAVE_KEY_FILE
    h.read_keyword_file = [](DB_STEP_DATA* m, const char* n, const char* r, const std::int32_t* fl) {
        return static_cast<int>(read_keyword_file(m, n, r, fl));
    };
#else
    h.read_keyword_file = interim_read_keyword_file;
#endif
#ifdef P501_HAVE_DBREAD
    h.db_read_step = def::dbio::dbread::db_read_step; // p-dbread, 1401cc2a0 with the hook's types
#else
    h.db_read_step = np_db_read_step;
#endif
#ifdef P501_HAVE_GENDB
    h.gendb_write = [](DB_STEP_DATA* m, const char* n, std::int32_t a, std::int32_t b, std::int32_t c) {
        return def::dbio::gendb::write_db(*m, n, a, b, c);
    };
#else
    h.gendb_write = np_gendb_write;
#endif
#ifdef P501_HAVE_PRE_STEPS
    h.object_surface_update = [](model::LLOBJ_DATA* obj, bool flag) {
        def::dbio::gendb::object_surface_normals(obj, static_cast<char>(flag));   // 1402059a0 (Phase 4)
    };
#else
    h.object_surface_update = np_object_surface_update;
#endif
    h.write_object_short = short_object_writer;
    return h;
}

DispatchHooks g_hooks = defaults();

} // namespace

DispatchHooks& dispatch_hooks() { return g_hooks; }
void reset_dispatch_hooks() { g_hooks = defaults(); }

} // namespace def::cmd
