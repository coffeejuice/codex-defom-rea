# CP205: same-run original crash call chain and null list owner

## Source and confidence boundary

The retained first-exception receipt in `analysis/resume205-original-crash-stack-2026-10-26T08-58-00-325Z/summary.json` is from installed original `DEF_GUI_MO_64.exe` **v11.0.2**, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, PID 31656. `analysis/resume205-original-crash-capture-qualification.md` qualifies its single resumed launch, exception handler, natural AV exit, and unchanged fixture. It is a same-run stack, unlike the earlier WER-only candidate slice in `analysis/resume204-crash-candidate-slice.md`. The copied PE at `evidence/original-binaries/DEF_GUI_MO_64.exe` has the same digest (REA `open_binary` identity response); no reconstructed DEF_PRE instance is examined here.

Static address ranges below come from the copied PE's exception directory, parsed read-only by `tools/doe_static_pe.py` with Python `-B`. Instruction edges were checked with Microsoft DUMPBIN 14.44.35228.0 `/DISASM:BYTES /RANGE:0x<preferred VA>,0x<preferred VA>` on that copied PE. Addresses abbreviated below are **app RVAs**; preferred base is `0x140000000`, whereas the captured app loaded at `0x7ff648e40000`. Exception-directory ranges are exact **unwind regions**, not proof of complete logical function extents. REA `open_binary` bound the SHA-matched PE read-only; its subsequent containing-procedure requests remained pending and were stopped, so this report claims no REA function dossier or Evidence ID. No original execution, live process, GUI control, mutable REA database operation, source edit, build, or deployment was performed in this analysis.

## Captured fault and selected chain

The receipt's `firstAccessViolation` reports QtCore4_64.dll base `0x583e0000`, `QListData::size` RIP `0x583e32c0` (QtCore RVA `32c0`), a **read at `0x58`**, and `RCX=0x58`. CP203 established `mov rax,[rcx]` at that RVA. The accurate same-run backtrace contains app returns `4af4`, `a7377`, `a7510`, `eb1ec`, `eaa7a`, `dafad`, then Qt meta-invocation, app `2cf6cc`, `2a725`, and `1cae` (`summary.json`, `firstAccessViolation.accurateBacktrace`). The symbol text attached to Qt addresses names the app inconsistently; module identity, loaded base and RVA are the reliable fields.

| Stack return RVA | Containing unwind region (end exclusive) | Call immediately before return, from copied PE |
|---|---|---|
| `4af4` | `4ae0..4af9` | `4aee`: indirect size import at app IAT `1e04210` (CP204) |
| `a7377` | `a7360..a737c` | `a7372`: `4ae0` |
| `a7510` | `a74f0..a7549` | `a750b`: `a7360` |
| `eb1ec` | `eaf60..eb5ad` | `eb1e7`: `a74f0` |
| `eaa7a` | `eaa50..eaa96` | `eaa75`: `eaf60` |
| `dafad` | `daf00..db060` | `dafa8`: `eaa50` |
| `2cf6cc` | `2cf3c0..2cf738` | `2cf6c6`: Qt `QMetaObject::invokeMethod` import |
| `2a725` | `2a0e0..2a99f` | `2a720`: `2cf3c0` with notifier address `143d76700` |
| `1cae` | `1410..20b4` | `1ca9`: `2a0e0` postOpen call (CP204) |

The `daf00` dispatch stores incoming `R8D` at entry `daf05`. At `daf31..daf5a` it range-checks that value as a jump-table index; the copied table at preferred `1400db03c` has case index **4** pointing to `dafa3`, whose `dafa8` call selects `eaa50`. This identifies the static callback arm reached by the stack, conditional on intact control flow. The `2cf6c6` Qt invocation uses `R8D=1` at `2cf6bd`; the accurate stack binds this callback to that invocation in this occurrence. It occurred before `postOpen` returned at `2a725`, hence before the enclosing CLI Open returned at `1cae`.

## Receiver provenance to the fault

At `eb1d3..eb1e7`, `eaf60` reloads incoming object `O` from its homed argument (`[rsp+170h]`), obtains `H=[O+30h]`, loads `P=[H+38h]` into `RCX`, and calls `a74f0` at `eb1e7` with loop index in `EDX`. `a74f0` homes this same `RCX` and calls `a7360` at `a750b`; `a7360` homes it, reloads it at `a7369`, adds `58h` at `a736e`, and calls `4ae0` at `a7372`. `4ae0` forwards the wrapper address to the size import (CP204). No instruction on this selected path replaces `P` between the field load and the addition. Therefore the captured `RCX=0x58` at the leaf implies **`P=0` at the `a736e` addition**, assuming ordinary 64-bit arithmetic and the unmodified copied code. The actual invalid wrapper is `P+58h`, a list wrapper within a null owner, not a corrupt backing-data pointer inside a readable list.

This is the **postOpen notifier callback** path. The same `a7360` accessor is also reachable from CP204's CLI loader, but this stack's `a7510 -> eb1ec -> eaa7a -> dafad -> Qt invoke -> 2cf6cc` frames select this different origin. At `eb1e3`, the null owner came from `H+38h`, where `H` is obtained through incoming `O+30h`. The receipt did not retain `O`, `H`, or that field's address/value, so it does not by itself identify a named C++ object or prove its lifetime.

## Producer and missing causal link

Earlier in the same `eaf60` static region, `eaff6..eb00f` calls `1d510` with a pointer at local `[rsp+40h]`, then stores its returned `RAX` into `[O+30h]+38h`. `1d510..1d530` calls `133a0..133de`, passes its result to `45780..457e6`, and returns that helper's result. The `45780` helper explicitly returns zero when its `c78ad0` call returns zero (`4578e..457a2`) or its subsequent lookup at `146f8fc` returns zero (`457c1..457d5`); otherwise it calls `44a90` at `457dc`. CP204 qualifies `44a90` as returning an owner's `+19c0h` field, with conditional allocation and construction. These are **static possible producer outcomes**; there is no same-run return capture for `1d510`, `45780`, or `44a90`.

The field loaded at `eb1e3` may be the value written at `eb00f`, but the retained receipt does not prove that it stayed unchanged through the intervening calls and loop, or that `O+30h` still named the same `H`. The precise causal gap is **why `[O+30h]+38h` was zero at `eb1e3`**: initial producer null, later clear or replacement, or another state transition. The earlier non-null checks at `eafb1` (`[rsp+40h]`) and `eafcd` (`[rsp+48h]`) guard other pointers; they do not check this `+38h` owner. A focused, already-authorized future observation would need the exact `O`, `H`, producer return and field writes in the same occurrence, but this report authorizes no new original run.

The demonstrated failure is an original v11.0.2 pre-Open-completion callback crash on this recorded launch. The public m2m ABI implementation consequence remains undecided until the field's transition and its relationship to the converter's supported behavior are established.
