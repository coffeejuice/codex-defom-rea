# CP209: original CLI Open and the conditional RefreshMainView arm

The inspected original executable provides a command-line **GUI startup Open**, not an established headless Open-to-Save interface corresponding to `m2m_convert_w`. That route calls `postOpen` on the actual main window. However, the particular `signalRefreshMainView()` call implicated by CP205/CP207 is **conditional on a null loader return**, not mandatory on every successful Open. This distinction materially sharpens CP208's mapping: omitting that callback does not by itself explain why the candidate and original reached different loader outcomes.

## Source and activity boundary

Target is the copied original DEFORM v11.0.2 `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256 recomputed here as `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. All abbreviated code addresses below are RVAs, preferred image base `0x140000000`. Retained DUMPBIN captures are instruction evidence, while retained Ghidra pseudocode is supporting interpretation; unwind extents are not automatically entire logical functions. The copied target's identity matches the CP207 installed-original capture identity.

This investigation read retained evidence and parsed/disassembled the copied PE offline. No original process, GUI/control session, REA session/database, build, fixture mutation or deployment was used. No reconstructed executable was inspected. Only this report was written. It applies the reverse-engineer-anything skill's shipped-artifact evidence discipline through static binary evidence; no live REA claim is made.

## CLI argument path

`analysis/wave3-editor-owned-open-entry-140001410.asm` covers native main `1410..20b4`. Startup constructs `SfApplication` through `21c0` at `147e` and the main window at stack `rsp+40h` through `10490` at `1b39`. Only afterward, at `1b56`, it calls `2900` with mutable argc/argv and its argument-map object at `rsp+538h`.

The retained `wave3-editor-owned-open-argv-140002900.asm` shows iteration from argv index 1 (`2930`), testing the current argument's first byte against `'-'` (`296a`) and requiring a following value which does not begin with `'-'` (`2973..29a3`). It stores key/value information through `2df0` at `2b4f`. `wave3-editor-owned-open-argv-140002c00.asm` and `2d50` provide keyed value lookup, with an empty/default result when absent. This generic parser accepting a key is not evidence that main implements that key as an operation.

Main's explicit action selection is:

| Code | Bound meaning |
|---|---|
| `1b5c..1bec` | Read key `m`, compare value with ASCII `open`; false branches to `1cd4`. |
| `1bf2..1c80` | Read key `f`, convert it through `11a0`, test positive resulting length. |
| `1c82..1c94` | Pass path to loader `1ca30` on the actual stack main; save its returned pointer. |
| `1c9c..1ca9` | Pass that exact pointer as RDX to `2a0e0` on the same main, with **no nonnull gate**. |
| `1cae..1cbb` | After `postOpen`, update recent paths through `1dc00`. |
| `1cd4..1e48` | Alternative `m == new`, optional `prob_dir`, then timer delivery of `1slotActionFileNew()`. |
| `1e4e..1f79` | Other explicit lookups include `troubleshoot` and `language`. |
| `1f80..1fb6` | Splash finish/main display, delayed `slotRelaxDockSizeLimit()`, then QApplication event loop. |

The literals were checked directly in the copied PE: `m` at VA `141e151f4`, `open` at `1420edd4c`, `f` at `141e15224`, alternative `m` at `141e15384`, `new` at `141e1559c`, and the later literal addresses shown in that retained main capture. Main has no corresponding explicit `m == save`, convert-and-exit, or headless Open/Save branch in this inspected body. The recorded `-m open -f ...` launch therefore selects the ordinary main-window Open route before display/event-loop entry; absence of GUI input does not make its implementation headless.

## The refresh is in the null-result arm

`analysis/wave3-boolean-external-callback-14002a0e0.asm` establishes the branch precisely:

```text
2a120: cmp qword ptr [rsp+4a8h],0   ; saved incoming RDX
2a129: je 2a1a6                     ; null arm
...
2a19c: call 2a9b0                   ; nonnull arm
2a1a1: jmp 2a94e                   ; bypass null-arm notifications
2a1a6: ...                         ; null arm starts
...
2a515: lea rdx,[142100158]           ; 2signalProjectChanged()
2a523: call 2cf3c0
...
2a712: lea rdx,[142100170]           ; 2signalRefreshMainView()
2a720: call 2cf3c0
...
2a94e: ...                         ; common virtual-method tail
```

Direct byte-string reads verify the two signal literals, plus `2signalDiagEditorFitAll()` at VA `142100190`. Retained `wave3-boolean-open-registration-14002a0e0.c` independently has the same null/non-null split. The nonnull arm calls `a5890`, `133a0`, `c79ee0` and `2a9b0`; it skips the three direct notifier calls in the null arm. This report does not prove that all deeper nonnull-arm callees or common virtual targets cannot emit other refreshes.

Thus the exact notifier at `2a720` is mandatory along the selected null arm if earlier calls return normally, but is not an unconditional Open-success step. Notifier internals also have suppression/receiver predicates (CP204); reaching its call site is distinct from successful callback delivery.

CP205's qualified same-run stack contains return `2a725`, then `1cae`, binding its fault to this call. CP207 reports `cp205FaultPath=true` and independently captures the `1d510` producer returning zero in the selected callback occurrence, with `CAPTURE_COMPLETE`, qualified hook evidence and verified natural AV cleanup. Combining the static branch with that preserved path implies a **null `1ca30` loader result supplied to postOpen** in the selected occurrence, conditional on the captured code and ordinary control flow. This is a derived upstream fact, not a newly measured loader-return register. It does not identify which loader condition returned null or establish that all B98 launches do so.

`evidence/gui_mo/s1/s1-05/sub0_s101.c` supports that loader `1ca30` can return null after model/factory/owner prerequisites or load refusal, whereas its subordinate project reader is `8a9b0`. Candidate `src/moproj/read.cpp` models `8a9b0`; its comment about an omitted refresh is not, by itself, evidence about the distinct outer `2a0e0` branch. Do not equate accepted suffix/parser completion with a nonnull outer native loader result.

The other retained application Open caller, `wave3-editor-owned-open-entry-140025b60.asm`, calls `1ca30` at `26f7c`, saves RAX at `[rsp+48h]`, and later passes that same saved pointer to `2a0e0` at `27095`. Its intervening operations remain part of that caller. It offers no shown alternate direct headless Save interface.

## Interface conclusion and remaining uncertainty

The copied PE export name table has exactly five named exports: `DllCanUnloadNow`, `DllGetClassObject`, `DllMain`, `DllRegisterServer`, `DllUnregisterServer` (export directory RVA `0x354b710`, size 188). No named `m2m`/Open/Save C export exists there. This does **not** exclude COM-mediated behavior: those interfaces were not recovered in this bounded investigation. Nor does it exclude internal indirect entry paths, other companion programs, unnamed/publicly undocumented interfaces, or command handling inside unexpanded callees. A global proof of absence is not claimed.

Established: the inspected CLI path is a GUI application Open, it unconditionally invokes postOpen after its loader for a nonempty selected filename, and the captured RefreshMainView call belongs to its **null-result arm**. Not established: a shipped headless Open/Save API equivalent to m2m, successful original Save for B98, or why original loader returned null while the candidate continued. The scientifically useful next comparison is the original `1ca30` result and its first differing prerequisite versus the candidate's model preparation/selection, using static evidence first. Relabeling the crash as harmless GUI refresh, or adding it to every successful headless Open, would both outrun the evidence.

## Retained evidence hashes

| Artifact under analysis/ | SHA-256 |
|---|---|
| wave3-editor-owned-open-entry-140001410.asm | a18e1a6b48e042f2795bf9536d7bcc299d4e9ba8532a9b189912a719a161778e |
| wave3-editor-owned-open-argv-140002900.asm | c8da45daea038ffd0965eb19b3614bacba643c266fca48a7bbcf93bdb7fac379 |
| wave3-editor-owned-open-argv-140002c00.asm | 1100348457311cee3e1e90d1ba8cf43059205d294fdfd8fd9c97568c5ec522dd |
| wave3-boolean-external-callback-14002a0e0.asm | e3e74166115892e04fe23160955278cf3101abacb6d818cd41746bca94c43f63 |
| wave3-editor-owned-open-entry-140025b60.asm | 95fd6657d13f9f677bc87d391a810909acfe9e85bb7fdf96391942de2e2e2d8b |
| wave3-boolean-open-registration-14002a0e0.c | e3aaadc9161f9982385071a7671ec2ba7dac1bf692d844f04f96424e3223788e |

PE reads used `python -B` and `tools/doe_static_pe.py`; optional-header data-directory 0 supplies export RVA/size, IMAGE_EXPORT_DIRECTORY `NumberOfNames`/`AddressOfNames` supply the exact named-export list. Additional offline helper disassembly used DUMPBIN 14.44.35228.0 on bounded unwind ranges `140002d50..140002dec` and `14002a9b0..14002aad5`; no claim here requires treating those as complete transitive closures. No new runtime observation was obtained.
