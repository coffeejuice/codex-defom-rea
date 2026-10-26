// ctest-labels: m2m-mdt
#include "../support/test_paths.hpp"
#include <testkit/check.hpp>
#include <cmath>
#include <fstream>
#include <iterator>
#include "m2m/common.hpp"
#include "m2m/engine/simcontrol300_io.hpp"

using namespace m2m;
using namespace m2m::engine::simcontrol300;
namespace fs = std::filesystem;
namespace {
struct Rig {
    fs::path root;
    int warnings = 0;
    std::unique_ptr<Extra3DIo> io;
    Rig()
    {
        static unsigned serial = 0;
        root = m2m_test::scratchOutput(m2m_test::scratchRoot() /
            ("sim300-native-" + std::to_string(GetCurrentProcessId()) + "-" +
             std::to_string(GetTickCount64()) + "-" + std::to_string(++serial)));
        REQUIRE(!fs::exists(root)); fs::create_directory(root);
        io = makeNativeExtra3DIo(qt::String(root.u16string()), 1252, [this] { ++warnings; });
    }
    void write(const char* name, const std::string& bytes)
    {
        std::ofstream file(m2m_test::scratchOutput(root / name), std::ios::binary);
        REQUIRE(file.is_open()); file.write(bytes.data(), static_cast<std::streamsize>(bytes.size()));
    }
    std::string read(const char* name)
    {
        std::ifstream file(root / name, std::ios::binary);
        return {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
    }
};
struct Handle {
    HANDLE value;
    Handle(const fs::path& path, DWORD share, DWORD access = GENERIC_READ)
        : value(CreateFileW(path.c_str(), access, share, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr))
    {
        REQUIRE(value != INVALID_HANDLE_VALUE);
    }
    ~Handle() { if (value != INVALID_HANDLE_VALUE) CloseHandle(value); }
    Handle(const Handle&) = delete;
};
template <class F> bool unsupported(F action)
{
    try { action(); } catch (const Error& error) { return error.code == M2M_E_UNSUPPORTED; }
    return false;
}
}

TEST_CASE(native_extra3d_empty_files_enable_and_zero_count_probe_does_not_decode)
{
    Rig rig;
    for (const char* name : {"NBC.DAT", "SW2SP.DAT", "STRETCH.DAT", "B_WEIGHT.DAT"}) rig.write(name, "");
    rig.write("STRAIN_DST.DAT", std::string("\xff\xfe\0\x80", 4));
    auto form = fillExtra3D(*rig.io, NumericLocale::KnownC); REQUIRE(form.has_value());
    CHECK(form->nbcEnabled && form->switchToSparseEnabled && form->stretchEnabled);
    CHECK(form->weightEnabled && form->strainDistributionEnabled);
    CHECK(form->nbc[0] == qt::L("0")); CHECK(form->switchToSparse == qt::L("0"));
    constructorRefreshExtra3D(*form);
    CHECK(applyExtra3D(*rig.io, *form) == ApplyResult::Completed);
    CHECK(rig.read("NBC.DAT") == "75\t\t\t105\t\r\n");
    CHECK(rig.read("SW2SP.DAT") == "0\t\r\n");
    CHECK(rig.read("STRAIN_DST.DAT") == "\r\n"); CHECK_EQ(rig.warnings, 0);
}

TEST_CASE(native_extra3d_real_reader_preserves_prefix_pushback_and_failure_continuation)
{
    Rig rig;
    rig.write("real", " \t1.25-2.5 3e+1x 99");
    std::array<double, 5> values{};
    REQUIRE(rig.io->readDoubles(qt::L("real"), values));
    CHECK(values[0] == 1.25 && values[1] == -2.5 && values[2] == 30.0);
    CHECK(values[3] == 0.0 && values[4] == 0.0); // bad x is repeatedly ungotten
    rig.write("real", "1e+ 4");
    REQUIRE(rig.io->readDoubles(qt::L("real"), values));
    CHECK(values[0] == 0.0 && values[1] == 4.0); // sticky status does not block later extraction
    CHECK(values[2] == 0.0);
    rig.write("real", "-0 .5 1.");
    REQUIRE(rig.io->readDoubles(qt::L("real"), values));
    CHECK(values[0] == 0.0 && std::signbit(values[0])); CHECK(values[1] == 0.5 && values[2] == 1.0);
}

TEST_CASE(native_extra3d_integer_and_decoder_profiles_are_explicit)
{
    Rig rig;
    rig.write("integer", "-2147483648 2147483647 +7 0");
    std::array<int, 4> values{};
    REQUIRE(rig.io->readInts(qt::L("integer"), values));
    CHECK_EQ(values[0], (-2147483647 - 1)); CHECK_EQ(values[1], 2147483647);
    CHECK_EQ(values[2], 7); CHECK_EQ(values[3], 0);
    for (const char* outside : {"010", "0x10", "0b10", "2147483648", "2x"}) {
        rig.write("integer", outside);
        CHECK(unsupported([&] { rig.io->readInts(qt::L("integer"), values); }));
    }
    rig.write("real", std::string("\xef\xbb\xbf" "1.5", 6));
    std::array<double, 1> real{};
    CHECK(unsupported([&] { rig.io->readDoubles(qt::L("real"), real); }));
    rig.write("SPRING.INI", "0\r\n");
    std::vector<SpringRecord> springs;
    CHECK(!rig.io->readSprings(springs));
    rig.write("SPRING.INI", "1\r\n");
    CHECK(unsupported([&] { rig.io->readSprings(springs); }));
}

TEST_CASE(native_extra3d_write_denial_keeps_existing_prefix_and_continues_other_files)
{
    Rig rig;
    auto form = fillExtra3D(*rig.io, NumericLocale::KnownC); REQUIRE(form.has_value());
    form->nbcEnabled = form->switchToSparseEnabled = true;
    rig.write("NBC.DAT", "existing NBC"); rig.write("SW2SP.DAT", "old");
    {
        Handle held(rig.root / "NBC.DAT", FILE_SHARE_READ);
        CHECK(applyExtra3D(*rig.io, *form) == ApplyResult::Completed);
        CHECK(rig.read("NBC.DAT") == "existing NBC");
        CHECK(rig.read("SW2SP.DAT") == "140000\t\r\n");
        CHECK_EQ(rig.warnings, 0);
    }
}

TEST_CASE(native_extra3d_readable_delete_failure_warns_only_nbc_and_continues)
{
    Rig rig;
    auto form = fillExtra3D(*rig.io, NumericLocale::KnownC); REQUIRE(form.has_value());
    rig.write("NBC.DAT", "keep"); rig.write("SW2SP.DAT", "delete");
    {
        Handle held(rig.root / "NBC.DAT", FILE_SHARE_READ | FILE_SHARE_WRITE);
        CHECK(applyExtra3D(*rig.io, *form) == ApplyResult::Completed);
        CHECK(rig.read("NBC.DAT") == "keep"); CHECK(!fs::exists(rig.root / "SW2SP.DAT"));
        CHECK_EQ(rig.warnings, 1);
    }
    CHECK(applyExtra3D(*rig.io, *form) == ApplyResult::Completed);
    CHECK(!fs::exists(rig.root / "NBC.DAT")); CHECK_EQ(rig.warnings, 1);
}

TEST_CASE(native_extra3d_read_open_denial_is_remembered_as_disabled)
{
    Rig rig; rig.write("NBC.DAT", "75 105");
    std::optional<Extra3DPage> form;
    {
        Handle held(rig.root / "NBC.DAT", FILE_SHARE_WRITE, GENERIC_WRITE);
        form = fillExtra3D(*rig.io, NumericLocale::KnownC); REQUIRE(form.has_value());
        CHECK(!form->nbcEnabled); CHECK(form->nbc[0] == qt::L("75.0"));
    }
    // The control state is remembered even though the file is readable now.
    CHECK(applyExtra3D(*rig.io, *form) == ApplyResult::Completed);
    CHECK(!fs::exists(rig.root / "NBC.DAT"));
}

TEST_CASE(native_extra3d_missing_parents_are_not_created_and_directory_is_explicit)
{
    Rig rig;
    std::array<double, 1> value{99.0};
    CHECK(!rig.io->readDoubles(qt::L("missing/data"), value)); CHECK(value[0] == 0.0);
    CHECK(!rig.io->writeStringList(qt::L("missing/data"), {qt::L("text")}));
    CHECK(rig.io->removeIfReadable(qt::L("missing/data")));
    CHECK(!fs::exists(rig.root / "missing"));
    for (const char* relative : {"relative", "C:relative", "/drive-relative"}) {
        bool argument = false;
        try { makeNativeExtra3DIo(qt::L(relative), 1252, [] {}); }
        catch (const Error& error) { argument = error.code == M2M_E_ARG; }
        CHECK(argument);
    }
}

TEST_CASE(native_extra3d_directory_and_delete_path_profiles_are_explicit)
{
    Rig rig;
    const auto directory = rig.root.u16string();
    CHECK(unsupported([&] {
        makeNativeExtra3DIo(qt::String(directory + u"/\u4e2d"), 1252, [] {});
    }));
    CHECK(unsupported([&] {
        makeNativeExtra3DIo(qt::String(directory + u"/" + std::u16string(MAX_PATH, u'x')),
            1252, [] {});
    }));
    CHECK(unsupported([&] {
        makeNativeExtra3DIo(qt::String(u"\\\\?\\" + directory), 1252, [] {});
    }));
    CHECK(unsupported([&] { rig.io->removeIfReadable(qt::String(u"\u4e2d")); }));
    // A guarded path never becomes a successful missing-file delete probe.
    CHECK(!fs::exists(rig.root / fs::path(u"\u4e2d")));
}

int main() { return testkit::run_all(); }
