# CP160 ReportGenerate metadata key closed; dispatch occurrence still open

The actual constructor-derived insertion key is now established from immutable PE bytes: `SfTaskReportGenerate`. CP156's metadata-name uncertainty is resolved by following the class-specific virtual metadata provider. Its separate claim that no AppState constructor reset was established is superseded by the already retained CP116 constructor slice. Neither result supplies an observed DBLIST getter return or the gate value at that later occurrence.

## New static chain to TaskInfo.impl+0x10

Target throughout: `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Image base is `0x140000000`. These are preferred-image addresses, not observed loaded addresses.

1. Retained constructor `0x14079f1e0` stores the class address point `0x14269e558`, then calls `0x14079f280` at `0x14079f211`. The new wrapper capture shows `0x14079f28e` calls `0x1414036e0` with the same saved task pointer.
2. `0x1414036e0` first invokes virtual slots `+0x68/+0x70`, stores the child pointers and applies their backlinks when non-null. At `0x14140375f..0x14140376c`, it reloads the task's current vptr and calls virtual `+0x60`. This is ordered after the ReportGenerate address-point store; it is not an inferred metadata call inside the earlier base constructor.
3. Direct PE bytes at `0x14269e5b8` (`0x14269e558+0x60`, raw offset `0x269cdb8`) are `70 f3 79 40 01 00 00 00`, resolving the slot to `0x14079f370` independently of the symbolic vtable inventory. The 8-byte range SHA-256 is `e96ebdbf5661bfd11b9b637d7fa4270a72bf1295728a24ac98370e0074a442ef`.
4. Provider `0x14079f370` sets `rdx=0x14269e278` at `0x14079f3bf`, constructs a temporary through IAT `0x141e04480` at `0x14079f3cb`, and passes the resulting QString to `0x14140e240` with `rcx=task+0x20` at `0x14079f3d6..0x14079f3e7`.
5. The PE import table identifies `0x141e04480` as `QtCore4_64.dll!??0QString@@QEAA@PEBD@Z`, the QString constructor from `const char*`. File-backed bytes at `0x14269e278` (raw offset `0x269ca78`) decode to the NUL-terminated ASCII string `SfTaskReportGenerate`. The retained 32-byte slice SHA-256 is `2e3310166f9e384ac97fe503ba6f0ef1837d56dadd38ac55001f80ac1f53e977` (the hash includes bytes after the NUL).
6. Setter `0x14140e240` loads `TaskInfo+8`, adds `0x10`, and invokes QString assignment at IAT `0x141e04470`. The PE import identifies that entry as `??4QString@@QEAAAEAV0@AEBV0@@Z`. It then destroys the by-value input via `0x141e059e0` (`??1QString@@QEAA@XZ`). Thus the name written to `TaskInfo.impl+0x10` is the exact selector, rather than the translated title passed to the preceding metadata setter.
7. The new byte capture of insertion routine `0x1407c8140` corroborates the retained decompile: `0x1407c8183` reads this name into a temporary, indexes factory-private map `+8` and stores the TaskInfo pointer at `0x1407c81b2`; `0x1407c81ca` reads it again, indexes map `+0x10` and stores the incoming raw task pointer at `0x1407c81f9`. For this successfully completed constructor/registration path, the key is therefore `SfTaskReportGenerate` in both maps.

This establishes the normal static initialization/dataflow path. It does not claim an allocation succeeded, that the constructor ran in a retained original execution, that no later metadata mutation occurs, or that an arbitrary previously populated factory contains this pointer.

## Bounded source-ready cut

The source-ready fact is the ReportGenerate-specific metadata-name assignment on the qualified post-address-point initialization path: write exact `SfTaskReportGenerate` to the existing TaskInfo name storage, preserving the task allocation, child/backlink order, shared initializer order, and two metadata-keyed registration maps. It is suitable for a narrowly scoped metadata initializer or an existing constructor frame once its earlier prerequisites are admitted. It does not authorize skipping unresolved QObject/TaskInfo construction effects, completing unrelated metadata fields, or enabling public DBLIST.

Static factory insertion plus the exact name also supports a conditional identity contract: if this registration has completed in the queried factory and the key's value has not been replaced or removed, its raw map value is the same task pointer whose constructor installed `0x14269e558`. This conditional contract is stronger than selector spelling alone, but remains distinct from an observed getter result.

The newly captured getter `0x1407c7d00` checks key membership in factory-private map `+0x10`, returns the dereferenced stored value on a hit (`0x1407c7d47..0x1407c7d4c`) and null on a miss (`0x1407c7d66`). It performs no vptr validation. No occurrence identity or non-null value is manufactured from that code.

## Gate correction and precise remaining witness

CP116 already binds CRT initializer `0x141ca0bb0` through `0x1402c7ee0` to constructor `0x1402c7f80` on singleton `0x143d766b8`. At `0x1402c8030`, the constructor explicitly stores DWORD zero to `this+0x1c`. A new direct 8-byte PE slice there is `c7 40 1c 00 00 00 00 48`, SHA-256 `02ed3279783c7971a5d2f03e9e4f84627c5d58f71b60f4c27cdee7019e41512a`. Startup zero is established after constructor completion; the missing fact is its later value/write lifecycle, not a missing constructor reset. CP117's negative pointer/caller scans remain bounded and do not prove the setter never runs.

No further static key/provider search is needed for this specific name. The smallest decisive same-run witness now needs:

- Original module hashes/bases, PID/TID and ordered call ordinals binding the selected fresh project and DBLIST handler/gate occurrence.
- At gate call `0x140651eba`, the actual singleton receiver and DWORD at loaded-module RVA `0x3d766d4` (`0x143d766b8+0x1c` at preferred base), plus the gate result. To recover the lifecycle rather than one sample, retain startup constructor completion and all qualified intervening writes to that exact field; setter `0x1402cbf30` entry alone is insufficient without its effective receiver.
- For the admitted handler's MainWindow getter, the actual MainWindow/factory/private-map identities and requested QString; the same-occurrence result pointer or null from map `+0x10`; and, if non-null, a read of its first qword vptr before the handler's `+0x218` call. Match a ReportGenerate vptr as loaded image base plus RVA `0x269e558`, rather than assuming its preferred VA.
- If the goal includes proving the registration occurrence as well as identifying the getter result, retain the same factory generation's constructor pointer, completed TaskInfo name, and map insertion before that lookup; preserve replacement/removal events or explicitly bound the interval without them.

A null return, gate rejection, absent receiver record, read failure or different vptr is an informative outcome, not permission to substitute the expected prototype. The name no longer requires a runtime capture to establish its static initializer; runtime observation remains necessary to claim the selected DBLIST occurrence used that object. Keep the public DBLIST/ReportList guard.

## Receipt and qualification

New disassembly uses the existing `admin/capture_pe_ranges.py` with PE exception-directory unwind bounds and DUMPBIN 14.44.35228.0. An unwind region is a bounded capture, not proof of a complete logical function; claims above use the displayed calls/stores and direct PE operands. The byte/import receipt is `analysis/resume160-connector-key-pe.json`. No source edits, build, original runtime, GUI, live REA retry, mutable database or external project writes occurred.

| Artifact | SHA-256 |
|---|---|
| `analysis/resume160-connector-static.json` | `ff3d7ae324efb62d2e0226f5e18edb7cc1c2ddc9e66e4ecaa831ff37990b95aa` |
| `analysis/resume160-connector-metadata-static.json` | `f4aef1e5707e20c4a369e4b01dcea35c2e517376e780db339b718e5520d1aa3d` |
| `analysis/resume160-connector-key-pe.json` | `42459f9ec74f068c90bbc69605428eff965ef471d70726298246e1c4b9de87db` |
| `analysis/resume160-connector-14079f280.asm` | `06112f10ded91e1a99380c7abe2a0b708fda6f0b709c2cc02280d4edd8146ba9` |
| `analysis/resume160-connector-metadata-1414036e0.asm` | `2330d489ceb576967edc359fb29ef2e6cb46a81f66faeb7390d71899e61f66c0` |
| `analysis/resume160-connector-metadata-14079f370.asm` | `21cb1983af58dd1c854d960cdc02a8867e07c238a1a4ce32bb1a196e4b2f43dd` |
| `analysis/resume160-connector-14140e240.asm` | `c2b5f6ab7a0f794432e8a9945b7b811206971d13aad26874b33067a36e331fc5` |
| `analysis/resume160-connector-metadata-1407c8140.asm` | `97286d7618c719484a44544cc50b713d11affc05a60e697056cb5e7085964900` |
| `analysis/resume160-connector-metadata-1407c7d00.asm` | `ab905c08bb3da7693c0773a53395569aab982aa7f40383bc1215afff122a482e` |
| `analysis/wave2-doe-task-prototype-ctor-14079f1e0.asm` | `0072b6c52a9c19dd54714036fdfceabc7f034a50d2a8df23c4da53590225db4b` |
| `analysis/wave2-doe-task-prototype-taskinfo-name.asm` | `85f0d0060bbd7e230aa0ee179e8bce5e5a085d70385d19c42c85e04466422c78` |
| `analysis/resume116-dblist-setter-slice.md` | `38b5a2c21b327afcc6d7150ea27a6b7d1fbb1366b43730245c92c9b5cd345de1` |
| `analysis/resume117-dblist-setter-receiver.md` | `4dded81d0a3d065424074df7a3853daf231c1de062ea99fd6c5baea0b84e946d` |

No deployed executable belongs to this converter-only evidence pass.
