# CP204: bounded startup slice for the 37 QListData::size import calls

The static slice adds three useful discriminators to CP203: a loader list whose wrapper is an owner plus `58h`, a recent-path list at a fixed image address, and notifier lists whose wrappers are local stack variables. It also finds a conditional pre-Open constructor chain to a fourth import site. These are proved instruction edges and receiver derivations, not observations that the crashing process traversed them. None selects the faulting caller without retained exception evidence.

## Scope and source qualification

Target: copied shipped `evidence/original-binaries/DEF_GUI_MO_64.exe`, original reference v11.0.2, recomputed SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. The QtCore reference and crash-symbol qualification remain CP203's: QtCore 4.8.4.0, SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`, size export RVA `32c0`, app IAT RVA `1e04210`. No reconstructed DEF_PRE instance is examined or claimed.

All abbreviated addresses below are **app RVAs**, unless marked otherwise; add preferred base `140000000`. Runtime ASLR must use the retained loaded base instead. `S` denotes the current function's post-prologue RSP; its value differs between functions. All ranges are PE exception-directory unwind regions, not guaranteed complete logical functions.

Read-only Python PE parsing and Microsoft DUMPBIN 14.44.35228.0 were used. `tools/doe_static_pe.py` supplied file-backed mappings and exception-directory bounds; Python ran with `-B` to avoid bytecode writes. The existing tool executable was `C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/VC/Tools/MSVC/14.44.35207/bin/Hostx64/x64/dumpbin.exe`. Reproduce each listed region with `/DISASM:BYTES /RANGE:<preferred begin>,<preferred end minus 1>` against the copied executable.

A preliminary executable-section `E8/E9 rel32` byte scan assigned sources to containing unwind regions and retained only targets at unwind-region entries. Its unbounded main-rooted candidate graph reached 6,542 regions, with maximum shortest-path depth 23. This is a **discovery index**, not a validated call graph: embedded opcode bytes, leaf functions without unwind records, interior-entry calls, indirect calls, import transitions, and callbacks limit it. Every edge used below was subsequently confirmed by DUMPBIN from its containing region entry. A missing candidate-graph path is not proof of non-reachability. No complete transitive graph or feasible-path solver is claimed.

Only this report was written. No original execution, GUI acquisition, REA open/database operation, source edit, build, deployment, or fixture mutation occurred.

## Exact fault boundary

CP203 establishes the leaf `mov rax,[rcx]` at QtCore RVA `32c0`, followed by backing-data accesses at `32c3` and `32c6`. Conditional on the reported AV being a data read at this instruction, the invalid operand is the **wrapper address in RCX**, not merely the wrapper's backing-data pointer. WER module/RVA alone does not retain access type, accessed address, or RCX. A readable wrapper containing a bad backing pointer would ordinarily fault later.

## Proved routes with receiver provenance

The ordering below prioritizes useful operand discriminators and short, reviewable paths. It is not a probability ranking of crash causes. Calls and their taken predicates are static; no route is bound to the baseline crashing PID.

### 1. CLI loader: owner-derived wrapper

Validated edges:

```text
main 1410 @1c8f -> loader 1ca30
loader 1ca30 @1d168 -> a7360
a7360 @a7372 -> common size wrapper 4ae0
4ae0 @4aee -> [IAT 1e04210] -> QtCore size
```

`main` supplies its stack main object at `rsp+40h`. Inside loader `1ca30..1d435`, call `1d0d3 -> 44a90` obtains `P`; `1d0d8` stores P at loader `S+38h`, and `1d0dd..1d0e3` rejects null before this later call. `44a90..44bca` returns its incoming owner's field `+19c0h`; when absent it conditionally allocates `1500h` bytes, constructs through `89880`, and stores the result in that field. Thus the selected provenance is the **returned field pointer P**, with a prior null test, not an invented current OPI type or lifetime guarantee.

At `1d163`, loader reloads P into RCX. `a7360..a737c` homes and reloads incoming RCX, adds `58h` at `a736e`, then calls `4ae0`; that wrapper preserves RCX through its import call. At size entry, `RCX=P+58h`. The loader reaches this call only on the corresponding successful earlier branch (`1d11f -> 1d154`); the preceding virtual load result and P's continued lifetime are not runtime-established. A stale/unmapped P can explain an unreadable wrapper address structurally; no evidence establishes that it happened.

Fault-return discriminator: immediate app return `4af4`; wrapper's caller continuation `a7377`. The latter identifies the accessor, but additional callers can use it, so identifying the CLI loader specifically still needs the next valid caller continuation `1d16d`.

### 2. CLI recent-path update: fixed image wrapper

Validated edges:

```text
main @1cbb -> 1dc00 @1dc1a -> cc9ef0
cc9ef0 @cc9f40 -> acf20 @acf44 -> ada30 @ada85 -> size IAT
cc9ef0 @cc9f67 -> 4ae0 @4aee -> size IAT
```

`1dc00..1dc2e` explicitly sets RCX to preferred VA `143d768f0` at `1dc13`, independent of its incoming main pointer. `cc9ef0..cc9fc9` adds `c58h` to that same saved input before either call, giving preferred wrapper VA **`143d77548`**, or RVA `3d77548`. The helpers forward that wrapper without adding another offset.

The `cc9f40 -> acf20` call is conditional on the result tested at `cc9f2a..cc9f2d`. `acf20..ad06a` sets `R8D=0` at `acf37` before calling `ada30`. Consequently `ada30`'s negative-start-only size call `ada57` is excluded for this exact helper invocation; its nonnegative path calls **`ada85`**, with continuation `ada8b`. After the conditional helper returns, `cc9f67` uses the common wrapper with the same fixed image address, continuation `cc9f6c`. A later repeated `cc9fa7 -> acf20` also supplies that address; loop entry depends on count exceeding 20.

This is a materially different operand class from P+58h. For the normal same-image mapping, the wrapper itself is in image storage. Corrupt list contents alone would not explain a first-instruction wrapper-read fault here. Image mapping/receiver corruption remains a distinct condition and is not excluded by static bytes. ASLR changes the absolute runtime address.

### 3. postOpen and selected constructor setup: stack snapshot

Validated edges:

```text
main @1ca9 -> postOpen 2a0e0 @2a523 -> notifier 2cf3c0
selected setup 130830 @130ad1 -> notifier 2cf3c0
notifier @2cf47a -> 4ae0 @4aee -> size IAT
notifier @2cf6ec -> 8188f0 @818909 -> size IAT
```

Both notifier callers explicitly supply preferred notifier VA `143d76700`: postOpen at `2a51c`, setup at `130aca`. These prove the respective direct edges, **not** a direct CLI-to-Boolean-constructor edge. CP203's constructor occurrence requirements remain unchanged; virtual/factory dispatch and same-E lifetime joins are unresolved.

`2cf3c0..2cf738` reads notifier `+141h` once at `2cf3f1`; nonzero skips the body. On its unsuppressed path, `2cf454 -> 2d03f0` obtains the selected list, then `2cf45c` supplies `RCX=S+70h` to copy helper `2ef070`. At `2cf475`, `lea rcx,[rsp+70h]` again supplies that local wrapper to the common size wrapper at `2cf47a`. The normal continuation is `2cf47f`.

Later the loop loads a snapshot receiver and calls `c1aef0`. A null result at `2cf4cc..2cf4cf` selects `2cf6e3`; `2cf6e7` supplies the **same notifier S+70h** to `8188f0`, with current index in EDX. That helper rejects negative index, then calls size at `818909` using its saved incoming RCX; continuation `81890f`. This direct-import candidate is newly tied to a notifier path, but it requires a nonempty loop and that null receiver test. It is not a lookup on the notifier map itself.

Both inputs are stack-derived wrapper addresses. Explaining an AV on their first dereference requires more than a corrupt snapshot backing pointer. A complete runtime address/caller remains needed; these local facts do not establish healthy stack memory at a crash.

### 4. conditional pre-Open constructor chain: another local wrapper

The following ten edges were independently confirmed as direct call instructions from the containing unwind entry:

```text
main @1b39 -> 10490 @10598 -> 111c0 @11227 -> 19950
19950 @1a513 -> 196d0 @1970f -> eb6f0 @ebaf8 -> f47e0
f47e0 @f4dd1 -> f4250 @f479c -> ef670 @f0f8e -> ed280
ed280 @ed3e5 -> 4fbe0 @4fc35 -> size IAT
```

`196d0` enters `eb6f0` only when incoming main owner `+458h` is zero and allocation returned nonnull. `f47e0` and `ed280` contain additional branch tests, and `f47e0` makes an intervening virtual call. This report has **not** solved whether all selected predicates can hold together for the recorded launch. It establishes a conditional instruction path from pre-Open setup, not actual or unconditional startup reachability.

At the final edge, `ed3e0` forms `RCX=ed280 S+50h`, after local construction at `ed3aa` and a call with that local as an output argument at `ed3c2`. `ed3d8` sets `R8D=0`; hence only size site **`4fc35`** is selected in `4fbe0`, and negative-start site `4fc07` is excluded for this origin. Fault continuation `4fc3b` identifies that direct site. The receiver is a local wrapper regardless of the earlier heap owners in this longer chain. It should not be relabeled as an OPI field.

## Cleanup route kept outside the pre-show slice

A separately validated path is:

```text
main @1ffc -> 105e0 @106b3 -> c991e0 @c9924f -> c99890
c99890 @c99967 -> ca76e0 @ca7761 -> 2cffd0
2cffd0 @2d0189 -> 1344d50 @1344d6b -> 88be0 @88c35 -> size IAT
```

The main call at `1ffc` follows normal `QApplication::exec` return `1fbc`; it does not prove execution before show or while the event loop runs. At `2d0184`, RCX is the local wrapper `2cffd0 S+20h`; `1344d50` forwards it and sets `R8D=0`. Thus `88c35` can be selected, continuation `88c3b`, while `88c07` is excluded for this particular call. Other entries/callers, including callbacks into cleanup, remain outside this bounded attribution. Process termination or an Open command alone does not prove accepted Close.

## Coverage of all 37 original direct-import sites

Every CP203 region was disassembled again from its unwind entry. All 37 import calls retain their expected boundaries. The table accounts for each call without treating unconnected incoming receivers as known owners.

| Size-call RVA(s) | RCX at that import call | Slice status |
|---|---|---|
| `4aee` | incoming RCX of wrapper `4ae0` | Loader P+58h, recent-image list and notifier snapshot origins proved above; many other origins remain. |
| `4fc07`, `4fc35` | incoming RCX of `4fbe0` | Conditional constructor-chain origin is local `ed280 S+50h`; only second site on R8D=0. |
| `88c07`, `88c35` | incoming RCX of `88be0` | Selected after-exec cleanup origin is local `2cffd0 S+20h`; only second site on R8D=0. |
| `ada57`, `ada85` | incoming RCX of `ada30` | Selected recent-path origin is image RVA `3d77548`; only second site on R8D=0. |
| `d1004`, `d1011` | respectively incoming RCX and incoming RDX of `d0ff0` | No validated startup origin in this slice. |
| `15d919`, `188539`, `1f4679`, `376709`, `410129`, `6192e9`, `619349`, `6194c9`, `619689`, `91fc59`, `91fdd9`, `f34bd9`, `11c33a9`, `1232c19`, `16bf509` | saved incoming RCX of each respective region | Each has a negative-index guard; startup owner/call route unresolved. |
| `376777`, `3767a5`; `550e17`, `550e45`; `1272077`, `12720a5` | saved incoming RCX of each respective region | First site is negative-start branch; second is subsequent count check. Startup origin unresolved. |
| `410197` | saved incoming RCX of `410170` | Guarded index path; startup origin unresolved. |
| `818909` | saved incoming RCX of `8188f0` | Notifier snapshot removal origin proved above; other origins unresolved. |
| `d4b8ef`, `d4b924` | saved incoming RCX of `d4b8c0` | Negative-start/next-count pair; startup origin unresolved. |
| `13c52a4`, `13c52b1` | respectively incoming RCX and incoming RDX of `13c5290` | No validated startup origin in this slice. |
| `1582ac9` | saved incoming RCX of `1582ab0` | Guarded index path; startup origin unresolved. |

Four individual sites have proved pre-show **conditional paths** with end-to-end receiver derivations: common wrapper `4aee`, `ada85`, `818909`, and `4fc35`. The cleanup path adds `88c35` after exec return. This is neither an exclusion of the other 32 sites nor proof that the crash used one of these five. In particular, incoming RCX provenance within an isolated helper says nothing about its original owner until its caller is resolved.

`QSplashScreen::finish`, `QWidget::showMaximized`, timer delivery and `QApplication::exec` cross into Qt and/or indirect dispatch. This app-only slice does not enumerate QtGui callers, receiver virtual targets, signal/slot connections, queued events, exception handlers or every leaf thunk. The Boolean constructor `130830` seed has the two notifier paths, but its same-invocation connection from CLI startup remains unknown. The raw candidate graph cannot close these gaps.

## Smallest useful evidence alternative to a full fault dump

No further original run is needed to analyze an **already retained** matching exception receipt, if one exists. A full user-mode dump is sufficient but not the minimum: the exception record (including access type and accessed address), loaded module bases/identities, `RIP`, `RCX`, `RSP`, and a readable **`40h`-byte prefix of the fault stack** can already separate the direct-import sites from the common wrapper. This is a specification for examining retained evidence, not an instruction or authorization to execute the original.

At the exact size leaf entry, with unmodified expected call frames:

```text
[fault RSP]       immediate caller continuation
                 4af4 => common app wrapper
                 ada8b / 81890f / 4fc3b / 88c3b => respective direct site
if continuation is 4af4:
[fault RSP+30h]   common wrapper's caller continuation
[fault RSP+38h]   wrapper's homed incoming RCX
```

The offsets follow directly from `4ae0` homing RCX at entry `[rsp+8]`, subtracting `28h`, then the call pushing another eight-byte return address. Validate the instruction/loaded mappings, stack readability and unwind metadata before treating these words as frames; corrupted stacks and redirected imports invalidate the shortcut. A return in QtGui selects an unenumerated cross-module route. For `a7377`, another valid unwound caller is needed to bind the loader origin; for `2cf47f`, the next caller distinguishes postOpen `2a528` from setup `130ad6` or other notifier callers.

The current retained WER summaries and trace receipts do not contain that prefix or fault context. A fixed-image versus stack-local versus owner-derived RCX would be useful even without a complete stack; a wrapper return plus its caller continuation would narrow it much further. Static instructions cannot manufacture those runtime values. No source behavior change follows from this triage.

Sources: `resume203-qtcore-fault-callers.md`, `resume203-cli-open-static-semantics.md`, `resume203-boolean-open-constructor-static.md`, and the copied PE's bounded DUMPBIN views described above. CP203 supplies the matching-crash limitations; this report adds static edges and operand provenance only.
