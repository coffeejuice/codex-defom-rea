// step_names/strtrim.cpp: 140323c50 (packet kw-g1b). Header: defmodel/strtrim.hpp.
#include <defmodel/strtrim.hpp>

#include <cstring>

namespace def::model {

// @src 140323c50  strip trailing ' ', '\r', '\n'
void trim_right_blanks(char* s)
{
    std::size_t n = std::strlen(s);
    char* p = s + (n - 1);
    while (s <= p && (*p == ' ' || *p == '\r' || *p == '\n'))
        p = p - 1;
    p[1] = '\0';
}

}  // namespace def::model
