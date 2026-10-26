# CP127 Type3 CLI disassembly follow-up

## Result

An already-installed Microsoft `dumpbin.exe` provides bounded address-based disassembly without reopening the timed-out Ghidra analysis. The verified original `evidence/original-binaries/DEF_GUI_MO_64.exe` still hashes to `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`. The tool was Visual Studio Build Tools MSVC 14.44.35207 `Hostx64\x64\dumpbin.exe`.

This recovers the complete instruction bodies for the optimizer constructor and `.OPT` parser, plus the directly called optimizer initializer helper and destructor thunk. It narrows—but does not fully close—the owner contract: virtual/indirect callees, helper callees of the parser, and their dynamic ownership effects remain unresolved. No source or public Type3 guard change follows from these exports alone.

## Bounded captures

The raw files are direct `dumpbin /DISASM:NOBYTES /RANGE:<VA-start>,<VA-end>` outputs; each range was stopped immediately after the observed `ret` boundary:

| Raw file | Range | Recovered boundary |
|---|---|---|
| `analysis/resume127-type3-optimizer-constructor.asm` | `0x140917ff0–0x14091804d` | Constructor returns at `0x14091804c` |
| `analysis/resume127-type3-optimizer-init.asm` | `0x1409180b0–0x140918159` | Initializer returns at `0x140918158` |
| `analysis/resume127-type3-optimizer-destructor.asm` | `0x140918060–0x1409180b0` | Destructor thunk returns at `0x14091809f` |
| `analysis/resume127-type3-opt-parser.asm` | `0x140908630–0x140909467` | Parser returns at `0x140909466` |

The constructor body at `0x140917ff0` first calls `0x14091dfc0`, writes vtable address `0x1427bceb8`, calls an indirect target with `this+0x28`, stores `2` at `this+8`, and calls initializer helper `0x1409180b0`. The helper sets fields at `+0x0c`, `+0x10`, `+0x30`, `+0x34`, `+0x38`, `+0x3c`, `+0x40`, and `+0x48`; it also calls indirect code with `this+0x28` and direct helpers `0x140919000`/`0x140919050` with `this+0x18`/`this+0x20`. The `+0x40` value is loaded from `0x1421b5050`. This establishes literal initialization stores and calls, but not semantic field names, the targets/contracts of indirect calls, or nested-field ownership.

The destructor thunk at `0x140918060` restores vtable address `0x1427bceb8`, calls an indirect target with `this+0x28`, then calls `0x14091e040`. This confirms teardown code exists, but the indirect target and `0x14091e040` body are not captured here, so it does not establish the full disposal order for every subobject.

The parser body is fully disassembled from entry through its return. Its entry checks the second pointer argument and has an early false-return/cleanup branch when that pointer is null. The nonnull body contains a large sequence of indirect calls, direct helper calls, conditional exits, and a common cleanup/return path; the bounded export is 679 lines. The disassembly alone does not reliably name these calls as QFile operations or assign high-level record/list meaning to their effects. In particular, the existing caller-side evidence that stores the parser return without an aborting test is unchanged; this does not show whether the parser itself leaves partial mutations on false.

## Remaining boundary

The disassembly is sufficient to stop claiming the constructor/parser *bodies are absent*. It is not enough to implement a source-equivalent owner/parser or populate `listF0_` safely. The indirect constructor/destructor calls, direct helper bodies at `0x140919000`, `0x140919050`, and `0x14091e040`, and the parser's helper/data targets require separate bounded captures and symbol/data qualification. The exact vtable getter/setter targets consumed by `FUN_14013d740` also remain required before claiming parser-produced list equivalence.

The earlier Type3 load sequence remains: original `FUN_14008f530` requires both SUB `+0x20` and `+0x28`, processes the study XML and InputTable, then calls `FUN_140908630(optPath, optimizer)`. The later `FUN_14013d740` producer transforms optimizer getters into ordered ShapeOptim `listF0_` names. Neither newly recovered body binds a live active operation, QFile outcome, or generated GEO state. Keep the public Type3 activation/GEO guards.

No Ghidra import or analysis database was retried or modified. Only bounded read-only CLI disassembly and report/raw-output creation were performed; no source edit, build, test, or GUI action occurred.
