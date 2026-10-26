// defmodel/strtrim.hpp: trailing-blank trim used by the KEY readers (packet kw-g1b; it sits next
// to p1-10's string helpers 1403239f0 / 140323bf0 in the binary).
#pragma once

namespace def::model {

// 140323c50: cut trailing ' ', '\r' and '\n' in place. Like the original it starts at
// s + strlen(s) - 1, so an empty string writes its terminator again at s[0].
void trim_right_blanks(char* s);

}  // namespace def::model
