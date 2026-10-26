// kw-g2bc: the Packet API entry points (defkey/kw_g2bc.hpp), routed to the four families.
#define _CRT_SECURE_NO_WARNINGS
#include "internal.hpp"

#include <defkey/kw_g2obj.hpp>

namespace def::key::g2bc {

LLOBJ_DATA* find_object(DB_STEP_DATA& m, std::int32_t no) { return def::key::g2obj::find_object(m, no); }

bool handles(int id) { return ec::handles(id) || win::handles(id) || nodal::handles(id) || local::handles(id); }

// part of @src 14038b8a0  the kw-g2bc cases of the group 2 parser
int read_case(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj, int id)
{
    if (ec::handles(id)) return ec::read_case(m, f, obj, id);
    if (win::handles(id)) return win::read_case(m, f, obj, id);
    if (nodal::handles(id)) return nodal::read_case(m, f, obj, id);
    if (local::handles(id)) return local::read_case(m, f, obj, id);
    return 2;
}

bool handles_write(int id) { return ec::handles_write(id) || nodal::handles_write(id) || local::handles_write(id); }

int write_keyword(int id, std::FILE* f, DB_STEP_DATA& m, const KeyRef& ref)
{
    if (ec::handles_write(id)) return ec::write_keyword(id, f, m, ref);
    if (nodal::handles_write(id)) return nodal::write_keyword(id, f, m, ref);
    if (local::handles_write(id)) return local::write_keyword(id, f, m, ref);
    return 0;
}

}  // namespace def::key::g2bc
