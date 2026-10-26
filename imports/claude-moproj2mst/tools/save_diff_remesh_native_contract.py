"""Record the bounded native CRT remesh read successor without changing source."""
from pathlib import Path
import hashlib
import json

ROOT = Path(__file__).resolve().parents[1]
SDK = Path("C:/Program Files (x86)/Windows Kits/10/Source/10.0.26100.0/ucrt")
TARGET = ROOT / "analysis/wave2-save-diffs-300-remesh-native-read-contract.json"
if TARGET.exists():
    raise SystemExit("Immutable native read contract already exists")


def record(path):
    path = Path(path)
    absolute = path if path.is_absolute() else ROOT / path
    data = absolute.read_bytes()
    return {"path": path.as_posix(), "bytes": len(data), "sha256": hashlib.sha256(data).hexdigest()}


receipt = {
    "status": "read_only_native_remesh_successor_contract_no_source_implementation",
    "original_evidence": [record(path) for path in [
        "analysis/wave2-save-diffs-300-native-io-repair-contract.json",
        "analysis/wave2-save-diffs-300-native-delete-original.asm",
        "analysis/wave2-save-diffs-300-remesh-reader.asm",
        "analysis/wave2-save-diffs-300-remesh-crt-fgets.asm",
        "analysis/wave2-save-diffs-300-remesh-crt-rewind.asm",
        "analysis/wave2-save-diffs-300-remesh-crt-getbuf.asm",
        "analysis/wave2-save-diffs-300-remesh-memory-increment.json",
    ]],
    "local_current_sdk_reference": [record(SDK / path) for path in [
        "stdio/fopen.cpp", "stdio/openfile.cpp", "lowio/open.cpp",
        "stdio/fgets.cpp", "stdio/rewind.cpp", "stdio/_getbuf.cpp",
        "inc/corecrt_internal_lowio.h",
    ]],
    "reference_qualification": "Installed Microsoft UCRT SDK source is a current-library reference, not original binary evidence or proof of every linked runtime byte. Original instruction selections remain the behavioral oracle; planned focused native tests must validate the chosen regular-file profile. This task's standalone CMake selects static CRT.",
    "original_contract": {
        "open": "Remesh reader calls the same narrow fopen14146fcd8 with literal rt as prior recovered helper. Original open reaches CreateFileA with GENERIC_READ, shareREAD|WRITE3 withoutDELETE, OPEN_EXISTING3, NORMAL80, security size24/null descriptor/inheritTRUE. Fixed relative names inherit the original CWD; adapter needs an explicit ordinary ASCII absolute-directory profile without pretending wide/extended paths are equivalent.",
        "fgets": "1414733ac takes buffer,n,FILE; reads at most n-1 bytes, stops after LF, appends NUL. Immediate EOF returns null without modifying destination; EOF after prefix returns that prefix. TokenReader requests2000. No separate readAll or preread snapshot occurs.",
        "rewind": "141470298 flushes/reset state, clears stream EOF/error and descriptor EOF flag, then seeks file descriptor tooffset0; failed seek sets streamerror. Remesh ignores the return and still follows original scan/fallback branch. Rewind happens even for unmarked primary before close.",
        "buffer": "Original _getbuf14148a7c0 allocates4096; on failure it uses an internal2-byte buffer. Normal profile can pin4096 through setvbuf, but resource-exhaustion/two-byte fallback must remain qualified rather than silently claimed equivalent.",
    },
    "proposed_native_component": {
        "file": "Read analogue of current CoggingFile, not reuse of its write constructor: CreateFileA with exact read flags/inheritance, _open_osfhandle(_O_RDONLY|_O_TEXT), _fdopen(rt), owned4096 buffer, actual fgets/rewind/fclose. Handle/descriptor/FILE cleanup on each failure; open failures still return null to the existing remesh fallback logic.",
        "token_bridge": "Keep shared token.cpp unchanged. Separately coordinate a TextFile borrowed line-reader factory whose fgets calls the live FILE reader. For callback-backed objects, unsupported non-line cursor/data operations must refuse explicitly. RemeshReadFile owns the FILE and bridge lifetime. No full file caching, double CRLF translation, fake rewind, or wide ifstream substitution.",
        "reached_profile_guard": "Each reached fgets chunk can validate its visible C-string prefix and token-count profile before tokenization. Original fgets consumes bytes after an embeddedNUL but TokenReader observes only its C-string prefix; preserve cursor consumption rather than treating strlen as bytes read. Numeric/range and raw-spill guards stay in memory parser. Rejecting a later chunk preserves earlier reader field effects.",
        "test_requirements": "Scratch-only real files: missing/empty/fallback/marked primary; CRLF/loneCR/CtrlZ plus rewind; read sharing allows a concurrent writer but denies delete/rename while held; first-open denial selects fallback; handle closure before fallback; no pre-read of later unsupported chunks; narrow path/MAX_PATH/namespace qualification; native and typed memory outputs agree for supported profile. Parent owns compilation and execution.",
    },
    "limits": "No production/test edits after memory-reader freeze, no build/test execution, original runtime, app control or fixture changes. Shared API change is a proposal awaiting coordinator agreement. Resource failures, ordinary-path qualification and current CRT/reference differences are not represented as complete original behavior.",
}
TARGET.write_text(json.dumps(receipt, indent=2) + "\n", encoding="utf-8")
print(json.dumps({"receipt": TARGET.relative_to(ROOT).as_posix(), "sha256": hashlib.sha256(TARGET.read_bytes()).hexdigest()}))
