// KEY file loader 1401b5f00 (packet p8-07).
#define _CRT_SECURE_NO_WARNINGS
#include <defcmd/key_file.hpp>

#include <cstring>
#include <share.h>

#include <defcmd/p8_07_ext.hpp>
#include <defkey/key_reader.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/step/db_step.hpp>
#include <defrt/port.hpp>

#if __has_include(<defcmd/dispatch.hpp>)
#include <defcmd/dispatch.hpp>
#define DEFCMD_HAVE_MODEL_KEY_HOOKS 1
#endif

namespace def::cmd {

namespace {

char* key_file_name(def::model::DB_STEP_DATA* m)
{
    // m+0x2f8a: name of the keyword file being read (char[0x400], in pad_2f8a of the layout)
    return reinterpret_cast<char*>(m) + 0x2f8a;
}

int run_key_loop(def::model::DB_STEP_DATA* m, std::FILE* f)
{
    if (g_key_stream_hook != nullptr) return g_key_stream_hook(m, f);
#ifdef DEFCMD_HAVE_MODEL_KEY_HOOKS
    ModelKeyHooks h(m);
    return def::key::read_key_stream(f, h, stdout);
#else
    (void)m;
    (void)f;
    DEF_NOT_PORTED(0x1401b6520);  // model side of the loop: ModelKeyHooks, owner p5-01
#endif
}

} // namespace

// @src 1401b5f00
int read_keyword_file(def::model::DB_STEP_DATA* m, const char* name, const char* reset,
                      const std::int32_t* flags)
{
    if (*reset != '\0') def::model::step_clear_u2f88(*m);  // 1401d7f10
    std::strcpy(key_file_name(m), name);
    std::FILE* f = _fsopen(name, "r", _SH_DENYNO);  // str 14052553c
    if (f == nullptr) {
        std::fprintf(stdout, "ERROR: Keyword file %s open error!\n", name);  // str 140526110
        return 1;
    }
    m->u1740 = *flags;
    const int rc = run_key_loop(m, f);
    m->u1740 = 0;
    std::fclose(f);
    std::strcpy(key_file_name(m), " ");  // str 140525634
    if (*reset != '\0') ext::fun_1401d7f40(m);
    return rc;
}

} // namespace def::cmd
