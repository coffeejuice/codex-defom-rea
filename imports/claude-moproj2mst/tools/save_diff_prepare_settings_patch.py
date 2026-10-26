"""Prepare, but never apply, the isolated original-settings/300 Open patch.

The coordinator owns builds and will authorize a source edit window separately.
Every replacement requires its exact anchor; concurrent changes are not overwritten.
"""
from pathlib import Path
import difflib
import hashlib
import json

ROOT = Path(__file__).resolve().parents[1]
PREFIX = "source/apps/moproj2mst/"
changes = {}


def replace(path, old, new):
    path = PREFIX + path
    before, current = changes.get(path, (None, None))
    if before is None:
        before = current = (ROOT / path).read_text(encoding="utf-8")
    if current.count(old) != 1:
        raise RuntimeError(f"Expected one anchor in {path}: {old[:100]!r}")
    changes[path] = before, current.replace(old, new, 1)


replace("include/m2m/common.hpp", "// Per-call state that is not part", """// Explicit snapshot of the original settings singleton for one conversion. The original
// constructor 0x1400403c0 initializes +0xc90 to zero; 0x140040810 can load the persisted
// Setting/MoInputMode. Reading a user's installation settings is outside this adapter.
struct OriginalSettings {
    int moInputMode = 0;
};

// Per-call state that is not part""")
replace("include/m2m/common.hpp", "    unsigned flags = 0;", "    OriginalSettings originalSettings;\n    unsigned flags = 0;")
replace("include/m2m/model.hpp", "struct Project {\n", """struct Project {
    // Per-conversion snapshot, not an original Project member/layout field.
    OriginalSettings originalSettings;
""")
replace("include/m2m/convert.hpp", "// A file besides the MST", """// Explicit original settings for this call; old overloads above/below retain mode 0.
// Nonzero mode selects editor 300 for active Forming; its Save is explicitly unsupported
// until its full widget apply is recovered. No environment is read by this C++ entry point.
Result convert(const std::u16string& moproj, const std::u16string& outMst, unsigned flags,
               unsigned codepage, int guiUnit, const std::u16string& deformDir,
               const OriginalSettings& settings);

// A file besides the MST""")
replace("include/m2m/convert.hpp", "\n}  // namespace m2m", """
Result convertToBytes(const std::u16string& moproj, const std::u16string& outMst, unsigned flags,
                      unsigned codepage, std::string& mst, std::u16string& outPath, SideBytes& side,
                      int guiUnit, const std::u16string& deformDir, const OriginalSettings& settings);

}  // namespace m2m""")
replace("src/core/convert.cpp", "           SideBytes* side, bool writeFile)", "           SideBytes* side, bool writeFile, const OriginalSettings& settings)")
replace("src/core/convert.cpp", "    ctx.flags = flags;", "    ctx.originalSettings = settings;\n    ctx.flags = flags;")
replace("src/core/convert.cpp", "        makeRegistry(p);", "        p.originalSettings = ctx.originalSettings;\n        makeRegistry(p);")
replace("src/core/convert.cpp", "    std::string mst;\n    std::u16string outPath;\n    return run(moproj, outMst, flags, codepage, guiUnit, deformDir, mst, outPath, nullptr, true);", """    return convert(moproj, outMst, flags, codepage, guiUnit, deformDir, OriginalSettings{});
}

Result convert(const std::u16string& moproj, const std::u16string& outMst, unsigned flags, unsigned codepage,
               int guiUnit, const std::u16string& deformDir, const OriginalSettings& settings)
{
    std::string mst;
    std::u16string outPath;
    return run(moproj, outMst, flags, codepage, guiUnit, deformDir, mst, outPath, nullptr, true, settings);""")
replace("src/core/convert.cpp", "guiUnit, {}, mst, outPath, nullptr, false);", "guiUnit, {}, mst, outPath, nullptr, false, OriginalSettings{});")
replace("src/core/convert.cpp", "    side.clear();\n    return run(moproj, outMst, flags, codepage, guiUnit, deformDir, mst, outPath, &side, false);", """    return convertToBytes(moproj, outMst, flags, codepage, mst, outPath, side, guiUnit, deformDir,
                          OriginalSettings{});
}

Result convertToBytes(const std::u16string& moproj, const std::u16string& outMst, unsigned flags,
                      unsigned codepage, std::string& mst, std::u16string& outPath, SideBytes& side,
                      int guiUnit, const std::u16string& deformDir, const OriginalSettings& settings)
{
    side.clear();
    return run(moproj, outMst, flags, codepage, guiUnit, deformDir, mst, outPath, &side, false, settings);""")

replace("src/dll/abi.cpp", "#include <cstring>", "#include <cstring>\n#include <limits>")
replace("src/dll/abi.cpp", "void putMsgW(", """// Adapter contract: snapshot once per C ABI call, never consult the original INI.
// Unset/empty means 0. Otherwise accept optional +/- and ASCII decimal digits only,
// within signed 32-bit range. Invalid input is an adapter argument error.
m2m::OriginalSettings originalSettings()
{
    std::wstring value(16, L'\\0');
    for (;;) {
        SetLastError(ERROR_SUCCESS);
        const DWORD n = GetEnvironmentVariableW(L"M2M_MO_INPUT_MODE", value.data(),
                                                static_cast<DWORD>(value.size()));
        if (n == 0) {
            if (GetLastError() == ERROR_SUCCESS || GetLastError() == ERROR_ENVVAR_NOT_FOUND) return {};
            throw m2m::Error(M2M_E_ARG, "cannot read M2M_MO_INPUT_MODE");
        }
        if (n < value.size()) { value.resize(n); break; }
        value.resize(n); // required size includes NUL; retry if concurrently changed
    }
    std::size_t i = 0;
    const bool negative = value[0] == L'-';
    if (negative || value[0] == L'+') ++i;
    const auto invalid = []() -> void {
        throw m2m::Error(M2M_E_ARG, "M2M_MO_INPUT_MODE must be a signed 32-bit decimal integer");
    };
    if (i == value.size()) invalid();
    const std::uint64_t limit = negative ? 2147483648ull : 2147483647ull;
    std::uint64_t magnitude = 0;
    for (; i < value.size(); ++i) {
        if (value[i] < L'0' || value[i] > L'9') invalid();
        const auto digit = static_cast<unsigned>(value[i] - L'0');
        if (magnitude > (limit - digit) / 10) invalid();
        magnitude = magnitude * 10 + digit;
    }
    m2m::OriginalSettings result;
    result.moInputMode = negative
        ? (magnitude == 2147483648ull ? (std::numeric_limits<int>::min)() : -static_cast<int>(magnitude))
        : static_cast<int>(magnitude);
    return result;
}

void putMsgW(""")
for kind in ("fromWide(moproj_path), fromWide(out_mst_path)", "in, out"):
    replace("src/dll/abi.cpp", f"m2m::convert({kind}, flags, g_codepage.load(), g_guiUnit.load(), deformDir());",
            f"m2m::convert({kind}, flags, g_codepage.load(), g_guiUnit.load(), deformDir(), originalSettings());")
replace("src/dll/abi.cpp", "    } catch (...) {\n        putMsgW(\"internal error\"", "    } catch (const m2m::Error& e) {\n        putMsgW(e.what(), msg, msg_chars);\n        return e.code;\n    } catch (...) {\n        putMsgW(\"internal error\"")
replace("src/dll/abi.cpp", "    } catch (...) {\n        putMsgUtf8(\"internal error\"", "    } catch (const m2m::Error& e) {\n        putMsgUtf8(e.what(), msg, msg_bytes);\n        return e.code;\n    } catch (...) {\n        putMsgUtf8(\"internal error\"")

replace("include/m2m/mst/loader_l3.hpp", "    bool (*formingSimControlApply)", """    // 300 constructor Open fill 0x140eb26f0 -> 0x140eb5630 -> 0x140ec19f0,
    // recovered CG toggled callback 0x140ed01b0 only; other widget behavior is incomplete.
    void (*formingSimControl300Open)(Project&, Operation&) = nullptr;
    bool (*formingSimControlApply)""")
replace("src/ops/family.cpp", "        // the tree's first item, \"Simulation controls\" (SfPreSimControl, vtable 0x1430c3ed8), is the\n        // editor the GUI shows\n        simControlShown_ = &op;", """        // Factory 0x1405ff0b0: mode!=0 constructs SfPreSimControl300; mode0 constructs
        // SfPreSimControl100. Retain the inherited mode0 approximation for now.
        simControl300Shown_ = p.originalSettings.moInputMode != 0;
        if (simControl300Shown_) l3Slot(g_loaderSlotsL3.formingSimControl300Open)(p, op);
        simControlShown_ = &op;""")
replace("src/ops/family.cpp", "        if (simControlShown_ != &op) return false;\n        return l3Slot", """        if (simControlShown_ != &op) return false;
        // 300 vt+0x1c0 applies all pages, unlike the inherited small-editor helper.
        // Do not silently run that different editor for newly exposed settings state.
        if (simControl300Shown_) not_ported(0x140eb5180, "SfPreSimControl300 Save apply");
        return l3Slot""")
replace("src/ops/family.cpp", "    const Operation* simControlShown_ = nullptr;", "    bool simControl300Shown_ = false;\n    const Operation* simControlShown_ = nullptr;")
replace("src/engine/loader_l3.cpp", "#include <cmath>", "#include <cmath>\n#include <emmintrin.h>")
replace("src/engine/loader_l3.cpp", "int spinClamp(int v,", """// @src 0x140ed01b0  SfPreSimControl300::on_m_rbtnDefSolCG_toggled(true).
// Reached on construction/Open only for the selected active Forming editor300:
// 0x140eb26f0 -> 0x140eb5630 -> 0x140eb80c0(page0) -> 0x140ec19f0.
// 0x140ec21b6 checks the CG radio only for SOLMTD==2. False toggles are display-only.
void formingSimControl300Open(Project& p, Operation& op)
{
    GuiModel* m = opModel(p, op);
    Cls_1401e7b10* sc = m ? m->simCtl() : nullptr;
    if (!sc || sc->solmtd != 2) return;
    if (sc->u2e8 != 4) {
        sc->u2e8 = 4;
        sc->solmtd_p[0] = 2.0;
        sc->solmtd_p[1] = 4.0;
        sc->solmtd_p[2] = 1.0;
        sc->solmtd_p[3] = 1.0;
    }
    // Preserve the original CVTTSD2SI semantics, including indefinite INT_MIN for
    // NaN/infinity/out-of-range. A C++ double-to-int cast would be undefined there.
    const auto integer = [](double v) { return _mm_cvttsd_si32(_mm_set_sd(v)); };
    const int first = integer(sc->solmtd_p[0]);
    if (first < 0 || first > 2) sc->solmtd_p[0] = 2.0;
    if (integer(sc->solmtd_p[1]) <= 0) sc->solmtd_p[1] = 1.0;
    const int fourth = integer(sc->solmtd_p[3]);
    if (fourth != 1 && fourth != 5) sc->solmtd_p[3] = 1.0;
}

int spinClamp(int v,""")
replace("src/engine/loader_l3.cpp", "    mst::g_loaderSlotsL3.formingSimControlApply", "    mst::g_loaderSlotsL3.formingSimControl300Open = &formingSimControl300Open;\n    mst::g_loaderSlotsL3.formingSimControlApply")
replace("include/moproj2mst.h", "/* out_mst_path NULL:", """/* Adapter settings contract (not an original environment interface): each conversion reads
   M2M_MO_INPUT_MODE once. Unset or empty = 0; otherwise optional +/- and decimal digits in
   signed 32-bit range; invalid values return M2M_E_ARG. It supplies Setting/MoInputMode
   explicitly without reading the original installation's INI. Nonzero selects the active
   Forming editor300; its unrecovered Save returns M2M_E_UNSUPPORTED. Mode0 retains existing
   behavior. No ABI signatures, flags or API version change. */

/* out_mst_path NULL:""")

changes[PREFIX + "tests/g0/test_abi_original_settings.cpp"] = ("", r'''// ctest-labels: abi
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cstring>
#include <initializer_list>
#include <string>
#include <testkit/check.hpp>
#include "moproj2mst.h"

namespace {
struct EnvironmentMode {
    bool present = false;
    std::wstring prior;
    EnvironmentMode() {
        wchar_t buffer[32768];
        SetLastError(ERROR_SUCCESS);
        DWORD n = GetEnvironmentVariableW(L"M2M_MO_INPUT_MODE", buffer, 32768);
        present = n != 0 || GetLastError() == ERROR_SUCCESS;
        if (n) prior.assign(buffer, n);
    }
    ~EnvironmentMode() { SetEnvironmentVariableW(L"M2M_MO_INPUT_MODE", present ? prior.c_str() : nullptr); }
    void set(const wchar_t* value) { REQUIRE(SetEnvironmentVariableW(L"M2M_MO_INPUT_MODE", value) != FALSE); }
};
}

TEST_CASE(adapter_settings_parse_each_call)
{
    EnvironmentMode env;
    // A nonexistent input reaches the existing Open error only after successful adapter parsing.
    // These calls never create output or consult installation settings.
    for (const wchar_t* value : std::initializer_list<const wchar_t*>{nullptr, L"", L"0", L"1", L"-1", L"+1", L"00001",
                                L"2147483647", L"-2147483648"}) {
        env.set(value);
        CHECK_EQ(m2m_convert_w(L"C:/no/such/m2m-settings.moproj", nullptr, 0, nullptr, 0), M2M_E_OPEN);
        CHECK_EQ(m2m_convert_utf8("C:/no/such/m2m-settings.moproj", nullptr, 0, nullptr, 0), M2M_E_OPEN);
    }
    for (const wchar_t* value : {L" ", L" 1", L"1 ", L"0x1", L"+", L"-", L"1.0",
                                L"2147483648", L"-2147483649", L"99999999999999999"}) {
        env.set(value);
        char message[256] = {};
        CHECK_EQ(m2m_convert_w(L"C:/no/such/m2m-settings.moproj", nullptr, 0, nullptr, 0), M2M_E_ARG);
        CHECK_EQ(m2m_convert_utf8("C:/no/such/m2m-settings.moproj", nullptr, 0, message, sizeof message), M2M_E_ARG);
        CHECK(std::strstr(message, "M2M_MO_INPUT_MODE") != nullptr);
    }
    env.set(L"0");
    CHECK_EQ(m2m_convert_w(L"C:/no/such/m2m-settings.moproj", nullptr, 0, nullptr, 0), M2M_E_OPEN);
}

int main() { return testkit::run_all(); }
''')

changes[PREFIX + "tests/mdt/test_simcontrol300.cpp"] = ("", r'''// ctest-labels: m2m-mdt
#include "../support/test_paths.hpp"
#include <testkit/check.hpp>
#include <defmodel/layout/cls_1401e7b10.hpp>
#include <limits>
#include "m2m/convert.hpp"
#include "m2m/engine.hpp"
#include "m2m/model.hpp"
#include "m2m/mst/loader_l3.hpp"
#include "m2m/mst/writer.hpp"
#include "../../src/engine/engine_internal.hpp"

using namespace m2m;
namespace {
std::filesystem::path input() {
    return m2m_test::configuredPath("M2M_ORACLE_B") / "RS_55_Multiple_Operations_Example1_before" /
           "Multiple_Operations_Example1.moproj";
}
struct Fixture {
    Project p;
    Ctx ctx;
    ProjectScope scope;
    engine::FileEffects effects;
    Fixture(int mode) : scope(p) {
        const qt::String file{input().u16string()};
        const auto db = app::changeSuffix(file, qt::L(".DB"));
        initProject(p, qt::absolutePath(db), qt::completeBaseName(db));
        p.originalSettings.moInputMode = mode;
        ctx.originalSettings = p.originalSettings;
        makeRegistry(p);
        REQUIRE(openProject(p, file, ctx));
        activateSubProject(p, 0, ctx);
        engine::openActiveOperation(p, ctx, effects);
    }
    Operation& op() { auto* op = operationById(p, 2); REQUIRE(op != nullptr); return *op; }
    def::model::Cls_1401e7b10& sc() {
        auto* model = engine::opModel(p, op()); REQUIRE(model != nullptr);
        auto* sc = model->simCtl(); REQUIRE(sc != nullptr); return *sc;
    }
};
}

TEST_CASE(default_mode_keeps_inherited_open_values)
{
    Fixture f(0);
    CHECK_EQ(f.sc().solmtd, 2);
    CHECK_EQ(f.sc().u2e8, 4);
    for (double v : f.sc().solmtd_p) CHECK(v == 0.0);
}

TEST_CASE(editor300_open_corrects_only_active_model)
{
    Fixture f(1);
    // Independent expectation: original callback0x140ed01b0 and retained RS55 Save row.
    CHECK_EQ(f.sc().solmtd, 2);
    CHECK(f.sc().solmtd_p[0] == 0.0);
    CHECK(f.sc().solmtd_p[1] == 1.0);
    CHECK(f.sc().solmtd_p[2] == 0.0);
    CHECK(f.sc().solmtd_p[3] == 1.0);
    auto* inactive = operationById(f.p, 1); REQUIRE(inactive != nullptr);
    auto* model = engine::opModel(f.p, *inactive); REQUIRE(model != nullptr);
    auto* other = model->simCtl(); REQUIRE(other != nullptr);
    other->solmtd = 2; other->u2e8 = 4;
    for (double& v : other->solmtd_p) v = 0.0;
    mst::g_loaderSlotsL3.formingSimControl300Open(f.p, f.op());
    for (double v : other->solmtd_p) CHECK(v == 0.0);
    bool refused = false;
    try { engine::SaveEffects saved; engine::saveCurrent(f.p, f.ctx, saved); }
    catch (const NotPorted& e) { refused = e.src == 0x140eb5180; }
    CHECK(refused);
}

TEST_CASE(original_cg_parameter_boundaries)
{
    Fixture f(1);
    auto& sc = f.sc();
    auto fill = [&] { mst::g_loaderSlotsL3.formingSimControl300Open(f.p, f.op()); };
    sc.u2e8 = 0;
    fill();
    CHECK_EQ(sc.u2e8, 4);
    CHECK(sc.solmtd_p[0] == 2.0); CHECK(sc.solmtd_p[1] == 4.0);
    CHECK(sc.solmtd_p[2] == 1.0); CHECK(sc.solmtd_p[3] == 1.0);
    sc.solmtd_p[0] = -0.9; sc.solmtd_p[1] = 1.9; sc.solmtd_p[2] = -77.0; sc.solmtd_p[3] = 5.9;
    fill(); // original tests truncated integers but preserves accepted source doubles
    CHECK(sc.solmtd_p[0] == -0.9); CHECK(sc.solmtd_p[1] == 1.9);
    CHECK(sc.solmtd_p[2] == -77.0); CHECK(sc.solmtd_p[3] == 5.9);
    sc.solmtd_p[0] = 3.0; sc.solmtd_p[1] = 0.9; sc.solmtd_p[3] = 2.0;
    fill();
    CHECK(sc.solmtd_p[0] == 2.0); CHECK(sc.solmtd_p[1] == 1.0); CHECK(sc.solmtd_p[3] == 1.0);
    sc.solmtd_p[0] = (std::numeric_limits<double>::quiet_NaN)();
    sc.solmtd_p[1] = (std::numeric_limits<double>::infinity)();
    sc.solmtd_p[3] = 2147483648.0;
    fill();
    CHECK(sc.solmtd_p[0] == 2.0); CHECK(sc.solmtd_p[1] == 1.0); CHECK(sc.solmtd_p[3] == 1.0);
    sc.solmtd = 1; sc.u2e8 = 0;
    for (double& v : sc.solmtd_p) v = 0.0;
    fill();
    CHECK_EQ(sc.u2e8, 0);
    for (double v : sc.solmtd_p) CHECK(v == 0.0);
}

TEST_CASE(explicit_settings_adapter_does_not_approximate300_save)
{
    std::string mst;
    std::u16string output;
    SideBytes side;
    OriginalSettings settings; settings.moInputMode = 1;
    const auto result = convertToBytes(input().u16string(), {}, 0, 1252, mst, output, side, 0, {}, settings);
    CHECK_EQ(result.code, M2M_E_UNSUPPORTED);
    CHECK(result.message.find("0x140eb5180") != std::string::npos);
    CHECK(mst.empty()); CHECK(side.empty());
}

int main() { return testkit::run_all(); }
''')

patch = "".join("".join(difflib.unified_diff(before.splitlines(True), after.splitlines(True),
                    fromfile="a/" + path, tofile="b/" + path))
                for path, (before, after) in changes.items())
destination = ROOT / "analysis/wave2-save-diffs-settings.patch"
destination.write_text(patch, encoding="utf-8", newline="\n")
receipt = {
    "status": "prepared_not_applied_not_built",
    "patch": str(destination.relative_to(ROOT)),
    "sha256": hashlib.sha256(destination.read_bytes()).hexdigest(),
    "files": [{"path": p, "before_sha256": hashlib.sha256((ROOT/p).read_bytes()).hexdigest() if (ROOT/p).exists() else None}
              for p in changes],
    "scope": "Explicit per-call original settings plus established active Forming300 CG Open callback; newly selected300 Save refuses rather than using inherited small-editor approximation.",
    "validation": "Exact source anchors checked; C++ compilation/runtime deferred to coordinator. Focused ABI parser and editor300 Open/Save refusal tests included.",
}
(ROOT / "analysis/wave2-save-diffs-settings-patch.json").write_text(json.dumps(receipt, indent=2)+"\n", encoding="utf-8")
print(json.dumps({"patch": receipt["patch"], "sha256": receipt["sha256"], "files": len(changes)}))
