# CP210: outer Open loader null predicates

The next original observation should identify the first failed outer-loader prerequisite. CP207's callback fault and CP209's postOpen branch imply a null outer result, but do not identify its producer. In particular, an XML read failure is not sufficient: the native project reader returns suffix acceptance separately from Project+55 read status.

## Boundary and identity

This is offline inspection of the copied original DEFORM v11.0.2 `evidence/original-binaries/DEF_GUI_MO_64.exe`. Recomputed SHA-256: `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Addresses below are RVAs at preferred image base `0x140000000`. The reverse-engineer-anything skill was applied through retained decompilation and exact-PE DUMPBIN instruction evidence. No original process, GUI control, REA database/session, build, deployment, source edit or fixture mutation occurred. Only this report was created.

Read qualifications: CP209; CP207 original capture summary; CP62 DBLIST owner; CP63 constructor; CP78 suffix admission; retained `s1/s1-05/sub0_s101.c`, `s1/s1-01/dc/path1.c`, `open0.c`, and `open3.c`. Exact PE was disassembled with DUMPBIN 14.44.35228.0 over `1ca30..1d2d0`, `1d3e7..1d435`, and `c79390..c79470`. The first range's final partial instruction is not used. Retained `wave3-closure-next-140044a90.asm` and `wave3-boolean-save-pump-14008a9b0.asm` supply additional instruction evidence. Decompiler names are interpretations, not source provenance.

## Ordered branch sequence

Main calls `1ca30` at `1c8f` and returns at `1c94`, then passes its returned pointer to `2a0e0` without a nonnull check (CP209). The outer loader first tests legacy import suffixes in order: `dprj`, `d2prj`, `d3prj`, `d2mo`, `d3mo`, `d2md`, `dmd`, `mdt`. Any match calls `1c2f0` at `1cdf9` and returns that delegated result. Those delegated null routes are outside this bounded .moproj analysis.

For B98's `.moproj` path, ordinary suffix behavior selects the other branch at `1ce03`. It initializes Project result `[rsp+38]` to zero, derives the `.DB` identity at `[rsp+40]`, obtains the main-owned manager through `c9b7c0`, then executes:

| Order | Exact instruction evidence | Effect on outer result |
|---|---|---|
| 1 | `1cf0c -> c78c10`; return at `1cf11`; compare `1cf16`; `je 1d070` at `1cf1c` | Nonzero means an existing matching model: show duplicate-ID information, then explicit zero result at `1d04c..1d06b`. Zero continues to creation. |
| 2 | `1d085 -> c79390`; return `1d08a`; compare `1d08f`; `je 1d3e7` at `1d095` | Zero leaves initialized Project result zero. This is an outer syntactic route; safe reachability through the factory's unguarded uses is not established. |
| 3 | `1d0b8 -> 146f8fc`, arguments child, 0, `143569448`, `143569470`, 0; return `1d0bd`; `je 1d3e7` at `1d0c8` | Failed RTTI conversion leaves Project result zero. |
| 4 | `1d0d3 -> 44a90`; return `1d0d8` stored at `[rsp+38]`; `je 1d3e7` at `1d0e3` | Project getter/construction returns zero. |
| 5 | `1d0f6 -> 8a9b0`; return `AL` at `1d0fb` saved to `[rsp+50]`; main virtual+548 at `1d112`; test saved byte at `1d118..1d11f` | False calls `44bf0` on the cast child (`1d126`), then explicit zero return at `1d12b..1d14a`. True continues to license notices. |

The common tail at `1d3e7` optionally calls `cf6210` with `[main+478]`, then returns exactly `[rsp+38]` through `1d40d..1d425`. License-notice paths converge there without clearing the Project pointer. A missing DOE/Optimization license notice is therefore not itself a null-return predicate in this body. A null manager is also not a separately tested safe return here: helper calls dereference their receiver.

`c79390` calls manager virtual+68 at `c793b9`, saves its return, and immediately passes it to `cdcdc0(path,false)` at `c793ce`, before ID assignment, list append and signal connections. Its stored child becomes its return. CP63 identifies the concrete factory as `45490` and qualifies the missing allocation-null safety path. Do not infer that native factory allocation failure normally survives these unguarded accesses to reach outer `1d095`.

`44a90` first tests child+19c0. Existing nonzero is returned. Otherwise it requests 0x1500 storage at `44ac8`; explicit allocation zero stores zero at child+19c0, while nonzero calls Project constructor `89880` with the child and its directory/base data. This identifies the local construction-zero mechanism without proving its occurrence in B98.

## B98 and candidate implications

CP207 launched the original with `-m open -f C:\PROBLEM\p202a\Gear_blank.moproj`, working directory `C:\PROBLEM\p202a`, and the image hash above. Its summary records `cp205FaultPath=true` and `causalClassification=same_fault_path_producer_returned_zero`; that producer is the later `1d510`, not a newly measured `1ca30` return. CP209's static split makes the upstream null result a derived fact. CP207 did not capture the five predicates above.

The retained B98 fixture has `Gear_blank.moproj` plus asset directories and an oracle MST; there is no top-level `Gear_blank.DB`. Its XML is SHA-256 `e769faf514b335d56ffbb22f2c76490d029f2714ada9b26a75496fbd62ade7fd`. This does not establish a missing-file cause: the outer `.DB` path here is model identity and factory input; no explicit QFile existence predicate is in the inspected outer sequence. `c78c10` tests existing manager children, not existence of that file. A fresh process is not itself proof that startup left the manager empty.

Retained `open0.c` and the `8a9b0` instruction tail show that `.moproj` invokes `8afa0`, stores its result in Project+55, and ultimately returns 1 (`8af74..8af95`). Unsupported suffixes return zero earlier. Thus parser/file-read failure alone cannot explain outer route 5 for an unchanged admitted B98 filename, assuming the relevant calls return normally. Initial event pumps and later processing mean this is a qualified static expectation, not a runtime observation of path immutability or reentrancy.

Candidate `src/core/convert.cpp:209..248` derives DB identity, calls `prepareProjectModel`, constructs Project, commits its binding, calls `openProject`, then commits selection. `src/engine/conversion_session.cpp:277..323` explicitly requires an empty candidate manager, allocates a Mo `DbHandle`, sets its path, appends/registers it and binds `p.dataDb`; its comment disclaims evidence for native factory `0133f0`, class installation `045449`, Qt construction or native main readiness. It does not execute an observed native `c78c10` duplicate outcome or failed RTTI outcome. The port's Project binding is not evidence that the original reached one. Candidate correctly distinguishes suffix acceptance from openedOk in full Save; changing this to treat XML failure as native outer null would contradict the inspected reader contract.

## Smallest discriminating same-run record

One original invocation can distinguish all five .moproj routes using an invocation-scoped entry/exit record plus five conditional observations of post-call return values:

1. At `1ca30` entry bind thread, invocation/depth, return address (`1c94` for this main call), main identity and decoded selected QString. Record outer return value and normal/abnormal completion.
2. `1cf11`: RAX duplicate lookup result.
3. If lookup is zero, `1d08a`: RAX created child.
4. If child is nonzero, `1d0bd`: RAX RTTI result.
5. If cast is nonzero, `1d0d8`: RAX Project result.
6. If Project is nonzero, `1d0fb`: AL acceptance result, before any intervening callback can change volatile registers.

Every observation must retain the same invocation token; nested calls caused by event pumping must not overwrite it. Associate final outer zero with its first failing prerequisite. The last reached site alone is insufficient if capture drops events or execution faults before the next site. Complete capture receipts, hook readiness/qualification, fixed caps, fixture/image identity and exact-process cleanup remain mandatory. This is a static proposal, not a newly qualified probe.

The smallest extra operand set useful for explaining the identified route is: main's manager pointer and lookup DB QString at `1cf0c`; returned child's vtable for RTTI failure; child+19c0 and allocator result for construction failure; actual QString at `8a9b0` entry for acceptance failure. These can be captured in the same bounded occurrence if authorized. Full call tracing, GUI input, or a Save attempt is unnecessary to answer which outer-null predicate fired. Capturing only `1ca30` return confirms the derived null but cannot answer the causal question.
