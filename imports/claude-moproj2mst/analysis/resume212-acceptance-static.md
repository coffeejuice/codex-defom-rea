# CP212: acceptance failure follows an encoded CLI filename boundary

The literal `-f C:\PROBLEM\p202a\Gear_blank.moproj` in CP210 is **not** passed unchanged to Open. Main calls RVA `11a0`, which interprets four hexadecimal characters as one QChar, before calling the outer loader. This explains how a pinned literal `.moproj` command can coexist with an acceptance return of zero. The capture does not decode the resulting QString, so its exact malformed contents and the selected zero-return branch remain unobserved. Do not change the candidate's ordinary `.moproj` reader to return false after XML failure on this evidence.

## Evidence boundary

Offline copied original DEFORM v11.0.2 `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256 recomputed as `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. All code addresses below are RVAs unless given as a full VA. No original process, GUI input/control, source edit, build, deployment, fixture mutation or mutable REA database/session was used. The reverse-engineer-anything skill was applied through retained Ghidra interpretation and exact copied-PE instruction/metadata inspection.

Read qualifications: CP209 headless entry, CP210 loader-null-static and outer-Frida review, CP78 suffix admission; retained `s1/s1-05/sub0_s101.c`, `s1/s1-01/dc/open0.c`, `s1/p17/a2.c`, `wave3-editor-owned-open-entry-140001410.asm`, and `wave3-boolean-save-pump-14008a9b0.asm`. New bounded DUMPBIN 14.44.35228.0 views inspected `11a0..1403`, `1ca30` prologue, `1d0e9..1d11f`, and CRT wrapper `146dfd4..146e008`. Partial first/last instructions in wider ranges were excluded from claims. PE import/literal reads used `tools/doe_static_pe.py::PE`; no provider-side analysis mutations occurred.

CP210 capture directory: `analysis/resume210-original-outer-loader-frida-2026-10-26T12-41-01-511Z`. Its valid unique primary-thread invocation records duplicate=0, factory=`0x3853820`, cast=`0x3853820`, Project=`0x3855220`, acceptance AL=0, normal outer return=0. Runtime selected path qualification is explicitly `raw_qstring_address_only_content_unverified`. The command and cwd are pinned; runtime characters are not.

## Actual argument to 8a9b0

Main retrieves key `f` via `2c00` at `1c20`. At `1c4d`, RCX points to main-local QString `[main-rsp+1918]`, RDX points to that lookup result, and it calls `11a0`. Main tests the decoded QString's length at `1c78`; positive length calls `1ca30` at `1c8f` with RDX=`main-rsp+1918` and RCX=`main-rsp+40`.

Outer `1ca30` spills entry RDX at `[entry-rsp+10]`, then subtracts `1d8` from RSP. At `1d0e9` it reloads RDX from `[body-rsp+1e8]`, exactly that same saved input pointer. At `1d0f1` RCX loads Project `[body-rsp+38]`; `1d0f6` calls `8a9b0`. It passes the original **decoded selected QString object**, not the derived `.DB` identity, Project+18, a char pointer or the literal argv string. Static code retains the pointer; mutation/reentrancy of its data has not been excluded by CP210.

## 11a0 decoder contract

Exact import names establish QString::length at IAT VA `141e044b0`, QString::at at `141e04490`, QChar::toLatin1 at `141e04498`, QChar(int) at `141e044a0`, and QString::setUnicode(QChar const*,int) at `141e044a8`.

For nonempty input length L:

1. `1227..1236` computes signed L/4; for valid nonnegative QString length this is floor(L/4). It allocates twice that count in bytes, then default constructs that many QChars.
2. `12c7..12d2` loops while `4*i < L`. `12ed..132a` reads exactly four successive input QChars, converts each to Latin-1, and puts them in four bytes. `132c` appends a NUL byte.
3. `1331..1342` passes the buffer, literal VA `141e151c4` (bytes `25 78 00`, `%x`), and address of stack int `[rsp+54]` to CRT scanf-family wrapper `146dfd4`. The integer conversion return is ignored. Wrapper `146dfd4` forwards buffer/format/varargs through `146df30`; its shape and the `%x` call establish a scanf-style hexadecimal conversion, not base64 or ordinary filename normalization.
4. `1347..1365` constructs QChar from that integer and stores its 16-bit value to array index i. `137b..1395` sets a QString from the array using **floor(L/4)** as its explicit length, deletes the array, then copy constructs the destination at `13c2`.

Each valid four-character hexadecimal group therefore represents one numerical UTF-16 code unit, e.g. `0043` -> U+0043 and `005c` -> backslash. On this x64 Windows image QChar storage is little endian, but the encoded text is the four-digit numeric unit (`0043`), not byte-order text (`4300`). Non-BMP characters need two encoded surrogate units. Empty input constructs an empty QString through the early branch.

Malformed handling is unsafe and must remain qualified. There is no length-multiple-of-four gate or scanf-success gate in this routine. `[rsp+54]` is not initialized before the loop; failed conversion can reuse an earlier successful value or uninitialized stack contents. `%x` can accept a leading hexadecimal prefix without consuming the whole group. The literal B98 filename has L=34, so the allocation/output count is 8, while the loop attempts 9 groups; the final group reads beyond the input's logical length and writes a ninth QChar beyond the allocated logical array. Qt indexing diagnostics, allocation behavior and malformed conversion mean a deterministic resulting filename must not be invented from a clean Python hex decoder. The first literal group `C:\P` can already parse hexadecimal C (12); this is not U+0043.

## Which branches produce AL=0

`818b50` calls `818e20` to remove an optional leading dot from the extension, prefixes a dot, and calls QString::endsWith(..., case-sensitivity 0). Accepted endings are thus case-insensitive `.moproj`, `.DB`, `.KEY`.

| Explicit zero site | Required normal-flow branch |
|---|---|
| `8acf3: xor al,al; jmp 8af8e` | First suffix chain all false: moproj call/return `8aa6f/8aa74`; DB `8ab30/8ab35`; KEY `8ac26/8ac2b`, then branch `8ac4c` to `8acf3`. |
| `8af10: xor al,al; jmp 8af8e` | Initial chain admitted a suffix, later chain all false: moproj `8addc/8ade1`; DB `8ae4c/8ae51`; KEY `8aec1/8aec6`, then branch `8aee7` to `8af10`. |

The reader pumps events twice before its initial suffix chain; admitted processing runs FOR003 and DB/UI helpers before checking the selected QString again. First-chain failure and later mutation/behavioral failure remain distinct possibilities. XML read failure at `8afa0` is stored at Project+55; its path joins `8af14`, and normal tail `8af74..8af95` returns 1 independently. The raw CP210 AL=0 cannot distinguish the two zero sites.

## Smallest further same-run witness

Keep the qualified owned-process/thread/invocation/caller, relocation, bounded event, exception and cleanup gates. Add a guarded bounded QString content observation at outer entry and `8a9b0` entry: object address, data/header identity, explicit length, UTF-16 units/decoded string, termination/read-error/truncation status. Use a layout reader independently qualified against the exact QtCore DLL; do not insert unreviewed hard-coded offsets or call target helpers just to print a path. Never decode only until NUL because malformed units can include embedded zero.

To distinguish first versus later rejection with minimal additional function-level instrumentation, hook `818b50` only while the selected `8a9b0` invocation is active and only for its six exact return addresses listed above. Record same-run/thread/depth token, return address, input QString contents/identity and AL, with cap six for each selected reader invocation and explicit nested/overflow rejection. Sequence `moproj=false, DB=false, KEY=false` at the first three sites identifies `8acf3`; a later all-false chain identifies `8af10`. A separately reviewed instruction hook at either zero site can establish the branch with fewer fields, but requires its own relocation qualification; do not treat tiny interior instructions as already safe Interceptor entries.

For corrected successful-path experimentation, change the owned launcher `-f` value to the valid four-hex-digit encoding below, pin both encoded argument and intended decoded path, and update exact command identity/fixture preflight accordingly. This is a materially new capture argument and requires updated runner, review and same-run decoded-path proof; reusing CP210's command receipt would be false. Existing controls are not waived. No new original run was performed here.

```text
0043003a005c00500052004f0042004c0045004d005c00700032003000320061005c0047006500610072005f0062006c0061006e006b002e006d006f00700072006f006a
```

`analysis/resume212-acceptance-codec-fixture.py` independently models only the deterministic valid input domain. Its offline check passes five roundtrips (B98, ASCII, non-ASCII BMP, surrogate pair, empty) and rejects the literal B98 argument as outside that domain. This checks the encoding arithmetic against the inspected routine; it is not native runtime confirmation.

## Candidate source implications

Candidate `moproj/read.cpp::openProject` correctly distinguishes suffix admission from Project+55. Preserve this split. Its public `m2m_*` filename is a direct path adapter and should not silently acquire the shipped GUI's hexadecimal CLI transport. A separate native-CLI harness/adapter can encode code units; it must remain distinct from candidate reader semantics. Candidate event-pump omissions and later suffix recheck still warrant separate fidelity treatment if a decoded-path witness reaches them. CP210's first mismatch is presently explained by a harness input-boundary error, not established as a candidate model construction, XML parser or Open-to-Save defect. Original successful Open/Save for B98 remains unproven.
