// Extra test doubles for the standalone keyword test (P12 objEntryN, P06 upperInPlace/replaceChar);
// linked with doubles.cpp, never into m2m_core.
#include "m2m/datafiles.hpp"
#include "m2m/model.hpp"

namespace m2m {
ObjEntry* objEntryN(Operation& op, int n) { return objEntry(op, n - 1); }
}  // namespace m2m

namespace m2m::datafiles {
char* upperInPlace(char* s)
{
    for (char* c = s; *c; ++c)
        if (*c >= 'a' && *c <= 'z') *c = static_cast<char>(*c - 'a' + 'A');
    return s;
}
char* replaceChar(char* s, char from, char to, int)
{
    for (char* c = s; *c; ++c)
        if (*c == from || *c == from - 'A' + 'a') *c = to;
    return s;
}
}  // namespace m2m::datafiles
