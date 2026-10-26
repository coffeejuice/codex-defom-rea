"""Freeze original remesh keyword/byte composition with typed memory I/O."""
from pathlib import Path
import hashlib
import json
import re

ROOT = Path(__file__).resolve().parents[1]
TARGET = ROOT / "analysis/wave2-save-diffs-300-remesh-memory-increment.json"
if TARGET.exists():
    raise SystemExit("Immutable remesh memory receipt already exists")


def record(path):
    data = (ROOT / path).read_bytes()
    return {"path": path, "bytes": len(data), "sha256": hashlib.sha256(data).hexdigest()}


sources = [
    "source/apps/moproj2mst/include/m2m/engine/simcontrol300_remesh.hpp",
    "source/apps/moproj2mst/src/engine/simcontrol300_remesh.cpp",
    "source/apps/moproj2mst/tests/mdt/test_simcontrol300_remesh.cpp",
]
header, cpp, tests = [(ROOT / path).read_text() for path in sources]
checks = {
    "declaring_header_used": '#include "m2m/engine/simcontrol300_remesh.hpp"' in cpp and '#include "m2m/engine/simcontrol300_remesh.hpp"' in tests,
    "token_declaring_header": '#include "m2m/datafiles.hpp"' in header,
    "enum_declarations": all(name in (ROOT / "source/apps/moproj2mst/include/m2m/engine/simcontrol300.hpp").read_text() for name in ("enum class ApplyResult", "enum class NumericLocale")),
    "original_parser_composed": "applyRemeshUniformEdgeToken(state, token)" in cpp and "Reader::Raw" in cpp,
    "project_enabled_composed": "if (report.loaded) state.enabled = *report.loaded;" in cpp,
    "test_main": "int main() { return testkit::run_all(); }" in tests,
    "no_trailing_whitespace": all(line.rstrip() == line for value in (header, cpp, tests) for line in value.splitlines()),
}
if not all(checks.values()):
    raise SystemExit(f"Prebuild checks failed: {checks}")
receipt = {
    "status": "bounded_remesh_memory_reader_and_open_assignment_frozen_for_root_build",
    "sources": [record(path) for path in sources],
    "contracts": [record(path) for path in [
        "analysis/wave2-save-diffs-300-remesh-raw-contract.json",
        "analysis/wave2-save-diffs-300-remesh-representation-contract.json",
        "analysis/wave2-save-diffs-300-support-1400ab170.c",
        "analysis/wave2-save-diffs-300-remesh-reader.asm",
        "source/apps/moproj2mst/include/m2m/datafiles.hpp",
        "source/apps/moproj2mst/src/datafiles/token.cpp",
        "source/apps/moproj2mst/src/datafiles/textfile.cpp",
    ]],
    "representation": "Original Raw4 string-copy is represented through9 knownbytes+10..18. Prefix plusNUL overwrites exactdouble bytes preserving suffix, including duplicate-token effects. Length8 terminatingNUL sets contactfalse. Length>=9 or embeddedNUL refuses before helper stores. memcpy avoids floating arithmetic/NaNcanonicalization and avoids arbitrary bool representations or unknownpadding writes.",
    "reader": {
        "interface": "RemeshIo::openRead(fixed narrow literal) ->owned RemeshReadFile with tokens(),rewind(),close(). tokens() is existing TextFile on qualified memory input. File lifecycle calls are observable; native CRT backend not implemented.",
        "profile": "Explicit KnownC plus actualLC_NUMERIC=C; ASCII/noNUL input, physicalchunks<=1999bytes and<=259tokens is an interface precondition of this isolated typedmemory profile. This is not yet a guard accepting arbitrary native files; native fgets integration must validate chunks as reached. Publicmode300 remainsrefused.",
        "primary": "OpenLOCAL_REMESH.DAT, scan firstword of each originalUpper/kNewLine line untilKEYWORD_DRIVEN orDONE/EOF, rewind evenwithoutmarker. Ifnomarker closeprimary and openLOC_DEN_CTRL.DAT; bothopenfailreturnsfalse withoutscalarstores. Markerprimaryreplaysfrombeginning, includingearlierdata lines.",
        "conversion": "Use unchanged TokenReader Text3 to obtain the same selectedtoken/state as Int5/Double10, then apply recovered conversion separately: bounded signeddecimalprefix int32 with originalnonnumeric0; D/d->E thenC-locale strtod. Text3 andtypes5/10 share tokenization; originalDouble mutation of oldtoken is unobserved because nextkeyword forces newphysical line. Raw4 is useddirectly, preserving same-line nexttoken vsfreshRawwholeline distinction.",
        "numeric_limits": "Integer overflow, hexadecimalfloat, nonfinite/range strtod branches explicitlyunsupported ratherthan guessed. EOF integer reuses lastassigned sharedlocal; firstunassigned useunsupported. EOF double keepsdestination, thenoriginalscalingclamp stillruns. Othernumericnonnumeric-prefix andfinite decimal behavior follows existing CRT conversion seam.",
        "fields": "Methodclamp0..5, booleanInt!=0, filterdouble, uniformRawoverwrite, scalingclamp1..10, surfaceboolean; unknownkeywordsignored. Keyword>=208bytesguards originalstack-bufferextent. DONE/EOFterminates; readableemptyreturns true. Earlierstores preserved beforeunsupported; reachedfile closes.",
        "enabled": "readRemesh returns optional originalbool separately; loadRemeshAtProjectOpen assigns state.enabled onlyoncompleted originalreturn, preserving unsupported priorstate. This composes the source-defined assignment at a typedprojectOpen seam; root-owned actual public activation hook is not wired yet.",
    },
    "tests": {"cases": re.findall(r"TEST_CASE\(([^)]+)\)", tests), "count": len(re.findall(r"TEST_CASE\(([^)]+)\)", tests)),
        "method": "MemoryIo with real unchangedTextFile/TokenReader and explicitopen/rewind/closeevents; exactderivedbitconstants; primary/fallback/empty/missing; allscalarkeywords, originalclamps, Rawselectiondifference, sharedintegerEOF, retaineddoubleEOF, partialeffects/refusal andlocaleguard. No realfilesystem writes.",
        "checks": checks, "status": "not_run_worker_root_owns_build_and_execution",
        "registration": "Existing CONFIGURE_DEPENDS source/test globs shouldregisterm2m_mdt_simcontrol300_remesh. Declaringheaders/enums/main and usedhelperreturncontracts reviewed; no workerconfigure/build/tests."},
    "next": "Sourcefrozen for combined rootbuild. Evidence-only native narrowfopen(rt)/fgets/rewind/close successor; coordinate borrowedlineinput support without changing sharedTokenReader source. Do not reusewideifstream orwrite-onlyCoggingFile as nativeproof. ActualrootProjectOpen/Save hooks follow nativeacceptance, not thismemorytestcomponent alone.",
    "limits": "Public incompletepath guard unchanged. No sharedtoken source ororiginalfixtures modified, nooriginalruntime/appcontrol/captures/builds/testexecution. Nativeopen/sharing/readfailure/encoding/rewind semantics remain unimplemented and explicitlyqualified; no fullconvertercompletionclaim.",
}
TARGET.write_text(json.dumps(receipt, indent=2) + "\n", encoding="utf-8")
print(json.dumps({"receipt": TARGET.relative_to(ROOT).as_posix(), "sha256": hashlib.sha256(TARGET.read_bytes()).hexdigest(), "cases": receipt["tests"]["count"], "checks": checks}))
