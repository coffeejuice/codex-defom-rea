// Warnings of the 2D GENCTC kernels: PathScale formatted WRITEs (FUN_1404b5ef4) on the unit held
// in DAT_140b847a4. That variable is 0 in the image and no store to it was found (GENCTC.md
// section 9 item 2), and unit 0 is the Fortran runtime's stderr, so the records go to stderr.
// Each WRITE statement is one or more records ending in a newline; '/' starts a new record.
// The edit descriptors used here are emulated: Iw right-justified (w asterisks when the value
// does not fit), Fw.d as printf "%w.df" through the VS2008 CRT formatter (w asterisks when too
// wide).
#include <defcmd/ctc2d_msg.hpp>

#include <cstdio>
#include <cstring>
#include <string>

#include <defrt/stdlib/legacy_printf.hpp>

namespace def::cmd::ftn {

namespace {

// 140b847a4 (owner: ifd::d2::g_msg_unit, never assigned: 0)  Fortran unit of the 2D contact
// warnings: unit 0 is stderr
std::FILE* unit_file() { return stderr; }

std::string ftn_i(std::int32_t v, int w)
{
    char buf[32];
    std::snprintf(buf, sizeof buf, "%*d", w, v);
    if (static_cast<int>(std::strlen(buf)) > w) return std::string(static_cast<std::size_t>(w), '*');
    return buf;
}

std::string ftn_f(double v, int w, int d)
{
    std::string s = def::rt::crt::format("%*.*f", w, d, v);
    if (static_cast<int>(s.size()) > w) return std::string(static_cast<std::size_t>(w), '*');
    return s;
}

void put(const std::string& s)
{
    std::FILE* f = unit_file();
    std::fputs(s.c_str(), f);
    std::fflush(f);
}

std::string moved_records(const char* tag_end, std::int32_t node, std::int32_t obj,
                          std::int32_t entity, double dist)
{
    // str 140604c50 / 1406049f0
    std::string s = "    WARNING: Node " + ftn_i(node, 4) + " pulled to object " + ftn_i(obj, 3) +
                    " surface.\n";
    s += "             Moved to entity," + ftn_i(entity, 6) + ", distance " + ftn_f(dist, 15, 7) + "\n";
    s += tag_end;
    return s;
}

} // namespace

// str 140604e00 / 140603fd0: 1001(4X,'WARNING: Unable to change BCC for Node ',I4,';',
//                                ' BCCDEF = (',2I4,')')
void ctc2d_warn_1001(std::int32_t node, std::int32_t c0, std::int32_t c1)
{
    put("    WARNING: Unable to change BCC for Node " + ftn_i(node, 4) + "; BCCDEF = (" + ftn_i(c0, 4) +
        ftn_i(c1, 4) + ")\n");
}

// str 140604c50: 1002(4X,'WARNING: Node ',I4,' pulled to object ',I3,' surface.',
//                     /,4X,'         Moved to entity,',I6,', distance ',F15.7)
void ctc2d_warn_1002(std::int32_t node, std::int32_t obj, std::int32_t entity, double dist)
{
    put(moved_records("", node, obj, entity, dist));
}

// str 1406049f0: 1004(... as 1002 ...,/,4X,'         Invalid contact boundary condition on mesh')
void ctc2d_warn_1004(std::int32_t node, std::int32_t obj, std::int32_t entity, double dist)
{
    put(moved_records("             Invalid contact boundary condition on mesh\n", node, obj, entity, dist));
}

} // namespace def::cmd::ftn
