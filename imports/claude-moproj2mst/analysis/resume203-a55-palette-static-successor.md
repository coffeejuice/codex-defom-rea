# CP203 A55 palette and CG transition static successor

No source-ready selected-editor prefix is admitted. The actual Session still parks at the QWidgetPrivate palette input guard `0x6504abb0`, and the historical A55 two-byte Save difference still lacks a same-occurrence callback witness. This pass adds one precise static condition: the original `QAbstractButton::setChecked(true)` returns without emitting `toggled` when the selected button is already checked (or not checkable). Solver2 and a call to setChecked(true) therefore do not by themselves establish normalization.

Only this report was written. This was converter-only read-only source/retained-evidence inspection and shipped-PE DUMPBIN disassembly. No source/test edit, build, executable, deployment, GUI/app-control session, original process, mutable Ghidra database or REA target/session operation occurred. No reconstructed DEF_PRE instance was inspected.

## Target and current boundary

The locally hashed original `evidence/original-binaries/DEF_GUI_MO_64.exe` is v11.0.2, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Original QtGui4_64.dll SHA-256 is `c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`, preferred base `0x65000000`. DUMPBIN reports version14.44.35228.0; tool path is `C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/VC/Tools/MSVC/14.44.35207/bin/Hostx64/x64/dumpbin.exe`.

`resume148-historical-diff-next.md` retains 93 SAME/five DIFF and qualifies A55 as historical, process-mode-unbound evidence. CP201's complete validation retains that comparison; its TLS effect-description increment supplies no palette owner or selected CG dispatch. The current `loader_l3.cpp::formingSimControl300Open` binds the actual Session/model-manager, calls beginEditor300Factory and unconditionally preserves not_ported6504abb0. Its unbound legacy fixture branch calls normalizeCgAtOpen, which is not evidence that the Session factory completed. `editor_base.cpp` still requires defaultPalette at PrivatePalette and otherwise returns PrivatePaletteUnavailable.

The locally rehashed complete palette constructor range `[0x6504abb0,0x6504ac26)` is unchanged from CP149: SHA-256 `704806e2a08c3327f0cb7f8d0ca7c496af0c69dbd897d456e48713cf1fa6a399`. CP149's source qualifications remain controlling: getter/copied D is stored in P before temporary deref; AL selects cleanup; both legs converge on the destination word0/ref tail. A post-constructor wrapper cannot establish the earlier deref AL or join the selected Session to canonical D/allocation generation. Static reinspection cannot supply that occurrence.

## New narrow transition predicate

The retained EXE iteration-fill assembly has the actual SC pointer at stack+0x118 and solver compare at `0x140ec2191`. For solver2, `0x140ec21af` loads the CG button from UI+0xa90, and `0x140ec21b6` calls EXE IAT141e07298 with DL=1. Retained `wave2-save-diffs-original-ui-imports.json` names that IAT `QAbstractButton::setChecked(bool)`. Fresh DUMPBIN export inspection resolves the original QtGui export to RVA373430. The following bounded instruction facts are from that original DLL:

| Site | Established effect |
|---|---|
| 65373454..61 | Load button private pointer from receiver+8, load word private+204, test bit0; zero branches to355d. |
| 65373467..73 | Extract bit1 as current checked state and compare with requested DL; equal branches to355d. For true this requires a false-to-true transition to continue. |
| 65373479..7b | Requested true skips the uncheck/exclusive-group refusal leg and enters34c5. |
| 653734c5..d5 | Create guarded receiver reference through imported helper, before mutating checked bit. |
| 653734d6..e2 | Clear/set bit1 to requested value in private+204. |
| 653734e8..351d | Conditional virtual state-update and widget/accessibility calls occur before the later signal call. |
| 65373522..2a | Read guarded receiver pointer and skip to3550 if null. |
| 6537352c..39 | For requested true, call65372b00, then reload guarded receiver pointer. This pass does not assign transitive behavior to that helper. |
| 6537353e..4a | Check guarded receiver again; only surviving nonnull receiver calls655d3190 with original receiver and requested bool. |
| 6537355d..68 | Early path can call the virtual state-update method when private+204 bit5 is clear, then returns. It does not join the toggled call. |

Fresh export lookup resolves655d3190 as `QAbstractButton::toggled(bool)`. Its bounded assembly forwards the boolean argument, Qt metaobject address65755d18 and signal index4 to an imported activation helper at IAT656968b0. This pass did not inspect that import's transitive activation/connection dispatch and does not claim callback delivery from a signal call alone.

This closes a static ambiguity in the older shorthand that solver2's setChecked synchronously reaches the slot. The precise statement is conditional on a checkable initially-unchecked selected radio, survival through pre-emission calls, applicable signal/connection semantics, and delivery to the selected editor. A retained screenshot showing a checked CG radio after fill cannot distinguish an already-checked no-emission path from a new checked transition. Repeated fill can also take the no-emission path.

## Exact missing same-occurrence witness

Two separately necessary joins remain. First, the selected Project/Operation/editor/private constructor must supply getter source O, temporary T, destination P and shared D/allocation generation; its actual temporary-deref AL or correctly ordered atomic input must qualify the selected branch, with canonical Session owner provenance. A numeric current ref state is additionally required by the existing numeric ref primitive; AL=true alone does not justify inventing that state. Palette return does not close later font/thread/setup dependencies.

Second, the actual A55 occurrence must bind its process start/module identities and settings path/hash/parsed MoInputMode; selected active Task00002 and actual editor/SC owner; completed setup/auto-connection; solver2 fill and the actual CG button; and either the requested-true transition predicates plus qualified dispatch, or direct actual entry into `0x140ed01b0` with true on the same owner. Preserve pre/post SC+2f8 and SC+308 and the matching Task00002 SimCtrl.KEY Save bytes/hash. A direct slot-entry witness is stronger and smaller than trying to infer dispatch from every internal Qt predicate; it still needs owner and occurrence binding. A slot from another editor or process is insufficient. Inactive Task00001 must remain separately compared.

The historical reader preserves SOLMTD fields; there is no basis for a reader clamp, writer substitution, unconditional/all-task normalizer or a public callback occurrence. Keep the historical raw DIFF and both public palette/Save refusals. Further static expansion cannot establish which radio state or signal outcome that historical process consumed. This lane requests no implementation write set.

## Compact receipt

DUMPBIN used `/exports` filtered to the exact setChecked/toggled exports, `/disasm:bytes /range:0x65373430,0x65373650`, and `/disasm:bytes /range:0x655d3190,0x655d31c4`. Only `[65373430,65373583)` is treated as the setChecked body through its observed RET; the extra output is not used as a function claim. SHA-256 of that339-byte range is `9d13a3605a5a6855bf64d31afb1e41693389e27ae3ad5c9eeb2ba1ffbbe48a93`. The 56-byte signal wrapper window `[655d3190,655d31c8)` hashes to `8e2609445afedb1ca84853ccef9d2b0ef9087c4b5c70769efecdb5e52bb28f71`; disassembly in this pass ends at31c4, so no completeness claim is based on its final four bytes.

Read-time source SHA-256: `editor_base.cpp` `20063efa3b650fec2dce5274894c10d5448ad8566b26bd4dad956512e06f3817`; `loader_l3.cpp` `c634eba9bdffbb6386938fe442bbd129682511bc40ba3e929792ada24a0ab766`.

Evidence reviewed: resume148-historical-diff-next, resume149-editor-palette-static-next, resume146-editor-palette-rea-query, resume48-a55-cg-timing, resume201-root-integration, resume203-editor300-cache1-static, current editor_base/loader_l3 source, and retained iteration-fill/UI import evidence. The cache1 raw-step closure audit is a separate dependency; it does not discharge palette ownership or CG event occurrence.
