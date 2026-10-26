// Group 1 KEY parser and writer shells (FUN_14039aa90, FUN_1403d4990): dispatch to kw-g1a and
// kw-g1b. kw-g1b's header is optional so that each packet builds on its own.
#define _CRT_SECURE_NO_WARNINGS  // the original's plain CRT calls (strncpy, sscanf, fscanf)
#include <defkey/group1.hpp>

#include <defkey/kw_g1a.hpp>

#if __has_include(<defkey/kw_g1b.hpp>)
#include <defkey/kw_g1b.hpp>
#define DEF_KEY_HAVE_G1B 1
#else
#define DEF_KEY_HAVE_G1B 0
#endif

#include <defmodel/layout/db_step_data.hpp>

namespace def::key {

// @src 14039aa90  group 1 parser: switch (id - 0x16); cases 0x00..0x26 are kw-g1a's,
// 0x27..0x45 kw-g1b's; any other id is the default case, return 2
int read_group1(model::DB_STEP_DATA* m, std::FILE* f, int id)
{
    int c = id - 0x16;
    if (0 <= c && c <= 0x26) return g1a::read_keyword(*m, f, id);
#if DEF_KEY_HAVE_G1B
    if (0x27 <= c && c <= 0x45) return g1b::read_keyword(*m, f, id);
#endif
    return 2;
}

// @src 1403d4990  group 1 KEY writer: Process Definition and Stopping & Step Controls (kw-g1a),
// then Iteration Controls, Processing Conditions and User Defined Variables (kw-g1b)
int write_group1(model::DB_STEP_DATA* m, std::FILE* f)
{
    int r = g1a::write_sections(f, *m);
    if (r != 0) return r;
#if DEF_KEY_HAVE_G1B
    r = g1b::write_sections(f, *m);
#endif
    return r;
}

bool group1_has_g1b() { return DEF_KEY_HAVE_G1B != 0; }

} // namespace def::key
