// ctest-labels: p1-02 options
// Tier-1 tests of the static option-label tables (initialisers 0x140501bd0-0x1405036a0).
// The binary checks re-read DEF_PRE_64.exe ($DEFORM_PRE_EXE or the v11.0.2 install; read-only)
// and are skipped when it is absent: every label must be the .rdata string at its recorded
// address, and the recorded instructions must load exactly that length, label and element and
// call FUN_140005c80. The last case runs gen_option_tables.py --check, which re-decodes every
// initialiser and compares the generated files byte for byte.
#include "defmodel/options/option_tables.hpp"
#include "defmodel/options/static_objects.hpp"

#include <testkit/check.hpp>

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <optional>
#include <string>
#include <vector>

using namespace def::model::options;

// ---- compile-time checks ----------------------------------------------------------------------
static_assert(std::size(kOptionTables) == 38);
static_assert(g_heat_transfer_mode[1].text == "Heat transfer ");  // trailing space kept
static_assert(g_temperature_source[1].text == "System ");
static_assert(g_contact_relation[0].text == " None");             // leading space kept
static_assert(g_friction_type[2].text == "Coloumb");              // spelling as in the binary
static_assert(g_object_type[3].text == "Prorus");
static_assert(std::size(g_object_type) == 8 && std::size(g_object_type_b) == 8);
static_assert(g_geometry_type[2].text.size() == 0x29);
static_assert(find_option_table(0x140501BD0) == &kOptionTables[0]);
static_assert(find_option_table(0x1405C0CE0) == &kOptionTables[19]);
static_assert(find_option_table(0x140503710) == nullptr);

namespace {

constexpr std::uint64_t kAssign = 0x140005C80;

std::filesystem::path exe_path()
{
    if (const char* e = std::getenv("DEFORM_PRE_EXE")) return e;
    return R"(C:\Program Files\SFTC\DEFORM\v11.0.2\DEF_PRE_64.exe)";
}

// Minimal PE image reader: virtual address -> file bytes.
struct Image {
    std::vector<unsigned char> d;
    std::uint64_t base = 0;
    struct Sec { std::uint32_t va, vsize, raw, rsize; };
    std::vector<Sec> secs;

    template <class T> T get(std::size_t o) const
    {
        T v;
        std::memcpy(&v, d.data() + o, sizeof v);
        return v;
    }

    bool load(const std::filesystem::path& p)
    {
        std::ifstream f(p, std::ios::binary);
        if (!f) return false;
        d.assign(std::istreambuf_iterator<char>(f), {});
        const auto pe = get<std::uint32_t>(0x3C);
        if (std::memcmp(d.data() + pe, "PE\0\0", 4) != 0) return false;
        const auto nsec = get<std::uint16_t>(pe + 6);
        const auto optsize = get<std::uint16_t>(pe + 20);
        base = get<std::uint64_t>(pe + 24 + 24);
        const std::size_t o = pe + 24 + optsize;
        for (int i = 0; i < nsec; ++i)
            secs.push_back({get<std::uint32_t>(o + 40 * i + 12), get<std::uint32_t>(o + 40 * i + 8),
                            get<std::uint32_t>(o + 40 * i + 20), get<std::uint32_t>(o + 40 * i + 16)});
        return true;
    }

    std::optional<std::size_t> off(std::uint64_t va) const
    {
        const std::uint64_t rva = va - base;
        for (const Sec& s : secs)
            if (rva >= s.va && rva < s.va + s.vsize && rva - s.va < s.rsize) return s.raw + (rva - s.va);
        return std::nullopt;
    }

    bool match(std::size_t o, const char* bytes, std::size_t n) const
    {
        return o + n <= d.size() && std::memcmp(d.data() + o, bytes, n) == 0;
    }

    // rip-relative target of the disp32 at file offset o (instruction ends at o + 4).
    std::uint64_t rip(std::uint64_t va_of_disp, std::size_t o) const
    {
        return va_of_disp + 4 + static_cast<std::int64_t>(get<std::int32_t>(o));
    }
};

const Image* image()
{
    static Image img;
    static const bool ok = img.load(exe_path());
    return ok ? &img : nullptr;
}

bool skip_without_binary()
{
    if (image()) return false;
    std::printf("  skipped: %s not found\n", exe_path().string().c_str());
    return true;
}

// Checks one label against the binary; returns an empty string or the reason it differs.
std::string check_label(const Image& img, const OptionLabel& l)
{
    const auto so = img.off(l.label_addr);
    if (!so) return "label address not in the file";
    if (!img.match(*so, l.text.data(), l.text.size())) return "label bytes differ";
    if (img.d[*so + l.text.size()] != 0) return "label is not NUL-terminated after its length";

    auto co = img.off(l.code);
    if (!co) return "code address not in the file";
    std::size_t o = *co;
    std::uint64_t va = l.code;
    if (img.match(o, "\x41\xB8", 2)) {  // mov r8d, imm32
        if (img.get<std::uint32_t>(o + 2) != l.text.size()) return "mov r8d length differs";
        o += 6, va += 6;
    } else if (img.match(o, "\x44\x8D\x43", 3)) {  // lea r8d, [rbx+imm8], rbx = 0
        if (img.d[o + 3] != l.text.size()) return "lea r8d length differs";
        o += 4, va += 4;
    } else {
        return "no length load at the code address";
    }
    if (!img.match(o, "\x48\x8D\x15", 3) || img.rip(va + 3, o + 3) != l.label_addr) return "lea rdx differs";
    o += 7, va += 7;
    if (!img.match(o, "\x48\x8D\x0D", 3) || img.rip(va + 3, o + 3) != l.object) return "lea rcx differs";
    o += 7, va += 7;
    if (!img.match(o, "\xE8", 1) || img.rip(va + 1, o + 1) != kAssign) return "call target differs";
    return {};
}

}  // namespace

TEST_CASE(tables_follow_the_binary_layout)
{
    for (std::size_t i = 0; i < std::size(kOptionTables); ++i) {
        const OptionTable& t = kOptionTables[i];
        REQUIRE(!t.labels.empty());
        CHECK_EQ(t.labels.front().object, t.data);
        for (std::size_t k = 0; k < t.labels.size(); ++k)
            CHECK_EQ(t.labels[k].object, t.data + 0x28 * k);  // std::string is 0x28 bytes
        if (i > 0) CHECK(t.init > kOptionTables[i - 1].init);
    }
}

TEST_CASE(both_sets_are_identical)
{
    for (std::size_t i = 0; i < 19; ++i) {
        const OptionTable& a = kOptionTables[i];
        const OptionTable& b = kOptionTables[i + 19];
        CHECK_EQ(std::string(a.name) + "_b", std::string(b.name));
        REQUIRE(a.labels.size() == b.labels.size());
        for (std::size_t k = 0; k < a.labels.size(); ++k) {
            CHECK(a.labels[k].text == b.labels[k].text);
            CHECK_EQ(a.labels[k].label_addr, b.labels[k].label_addr);  // shared literals
        }
    }
}

TEST_CASE(option_strings_copy_every_byte)
{
    const auto v = option_strings(*find_option_table(0x1405BF200));  // geometry_type
    REQUIRE(v.size() == 3);
    CHECK_EQ(v[0], std::string("Axisymmetric (no rotation about Z-axis)"));
    CHECK_EQ(v[1], std::string("Plane Strain"));
    CHECK_EQ(v[2].size(), 0x29u);
    const auto b = option_strings(*find_option_table(0x1405C0CE0));  // heat_transfer_mode_b
    CHECK_EQ(b[1], std::string("Heat transfer "));
}

TEST_CASE(every_label_matches_the_binary)
{
    if (skip_without_binary()) return;
    const Image& img = *image();
    int n = 0;
    for (const OptionTable& t : kOptionTables) {
        const auto io = img.off(t.init);
        REQUIRE(io.has_value());
        // sub rsp,0x38 or push rbx; sub rsp,0x30 -- then mov qword [rsp+0x20],-2
        CHECK(img.match(*io, "\x48\x83\xEC\x38\x48\xC7\x44\x24\x20\xFE\xFF\xFF\xFF", 13) ||
              img.match(*io, "\x40\x53\x48\x83\xEC\x30\x48\xC7\x44\x24\x20\xFE\xFF\xFF\xFF", 15));
        for (const OptionLabel& l : t.labels) {
            const std::string why = check_label(img, l);
            if (!why.empty())
                testkit::fail(__FILE__, __LINE__, std::string(t.name) + " \"" + std::string(l.text) + "\": " + why);
            ++n;
        }
    }
    CHECK_EQ(n, 124);
}

TEST_CASE(static_objects_match_the_binary)
{
    if (skip_without_binary()) return;
    const Image& img = *image();
    // 140503820 / 140503860: sub rsp,0x28; xor edx,edx; lea rcx,[obj]; call FUN_1400011d0
    const std::uint64_t ptr_inits[][2] = {{0x140503820, 0x1406E2E10}, {0x140503860, 0x1406E2E08}};
    for (const auto& p : ptr_inits) {
        const std::size_t o = *img.off(p[0]);
        CHECK(img.match(o, "\x48\x83\xEC\x28\x33\xD2\x48\x8D\x0D", 9));
        CHECK_EQ(img.rip(p[0] + 9, o + 9), p[1]);
        CHECK(img.match(o + 13, "\xE8", 1));
        CHECK_EQ(img.rip(p[0] + 14, o + 14), 0x1400011D0u);
    }
    // Timers: sub rsp,0x38; mov byte [rsp+0x20],flag; lea r8,[rsp+0x20]; lea rdx,[name];
    // lea rcx,[obj]; call ctor; lea rcx,[dtor]; call atexit
    for (const StaticObjectInit& t : kStepTimerInits) {
        const std::size_t o = *img.off(t.init);
        const std::uint64_t va = t.init;
        CHECK(img.match(o, "\x48\x83\xEC\x38\xC6\x44\x24\x20", 8));
        CHECK_EQ(int(img.d[o + 8]), int(t.flag));
        CHECK(img.match(o + 9, "\x4C\x8D\x44\x24\x20\x48\x8D\x15", 8));
        const auto so = img.off(img.rip(va + 17, o + 17));
        REQUIRE(so.has_value());
        CHECK_EQ(std::string(reinterpret_cast<const char*>(img.d.data() + *so)), std::string(t.name));
        CHECK(img.match(o + 21, "\x48\x8D\x0D", 3));
        CHECK_EQ(img.rip(va + 24, o + 24), t.data);
        CHECK(img.match(o + 28, "\xE8", 1));
        CHECK_EQ(img.rip(va + 29, o + 29), t.ctor);
        CHECK(img.match(o + 33, "\x48\x8D\x0D", 3));
        CHECK_EQ(img.rip(va + 36, o + 36), t.dtor);
    }
    // 140503b80: sub rsp,0x28; lea rcx,[obj]; call ctor; lea rcx,[dtor]; call atexit
    {
        const StaticObjectInit& t = kGeoHolderInit;
        const std::size_t o = *img.off(t.init);
        CHECK(img.match(o, "\x48\x83\xEC\x28\x48\x8D\x0D", 7));
        CHECK_EQ(img.rip(t.init + 7, o + 7), t.data);
        CHECK_EQ(img.rip(t.init + 12, o + 12), t.ctor);
        CHECK_EQ(img.rip(t.init + 19, o + 19), t.dtor);
    }
}

TEST_CASE(generator_check_passes)
{
    if (skip_without_binary()) return;
    const auto script = testkit::repo_root() / "port/defmodel/scripts/gen_option_tables.py";
    REQUIRE(std::filesystem::exists(script));
    const char* py = std::getenv("PYTHON");
    const std::string cmd = std::string("\"\"") + (py ? py : "python") + "\" -I \"" + script.string() +
                            "\" --check --exe \"" + exe_path().string() + "\"\"";
    const int rc = std::system(cmd.c_str());
    if (rc == 77) return;
    if (rc != 0 && rc != 1) {  // 9009 / 127: no Python on PATH
        std::printf("  skipped: python not runnable (exit %d)\n", rc);
        return;
    }
    CHECK_EQ(rc, 0);
}

int main() { return testkit::run_all(); }
