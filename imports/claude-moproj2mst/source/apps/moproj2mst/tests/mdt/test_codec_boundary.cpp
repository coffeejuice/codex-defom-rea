// ctest-labels: m2m-mdt
#include <testkit/check.hpp>
#include <windows.h>

#include <string>

#include "m2m/qt4.hpp"
#include "../../src/engine/engine_internal.hpp"

using namespace m2m;

TEST_CASE(codec_boundary_keeps_acp_separate_from_default_cstring_latin1)
{
    const qt::String euro(std::u16string(1, u'\u20ac'));

    // This expected ACP byte is qualified to the retained/current Windows
    // host profile (ACP 1252, OEMCP 437); it is deliberately not a CP_OEMCP
    // expectation and must be revisited if the execution host changes.
    REQUIRE(GetACP() == 1252);
    REQUIRE(GetOEMCP() == 437);
    CHECK_EQ(engine::local8(euro), std::string(1, static_cast<char>(0x80)));

    // QString::toAscii under the default null CString codec is Latin-1 here;
    // U+20AC is not representable and is replaced by '?'.
    CHECK_EQ(qt::toAscii(euro), std::string("?"));
}

int main() { return testkit::run_all(); }
