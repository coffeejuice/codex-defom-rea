# CP164: nonempty Type2 DOE key needs one selected-occurrence witness

## Decision

The smallest independent nonempty DOE production branch is the unsuppressed `SetOprHasDoe` key for a Type2 `KwOperation` ID 1. Its call order and conditional null-codec allocation are retained, but the actual selected invocation's codec branch and resulting QString data are not. **No further production mutation is source-ready from the reviewed evidence.** Keep the current key guard. The next input is one natural branch-to-publication observation, defined below. This selection is independent of CP163 FRCROT and connector changes.

This is a converter-only read-only review. No source, tests, shared state, build, GUI control, original process, live REA or analysis database was changed or run. The original is DEF_GUI_MO_64 from DEFORM v11.0.2, not a reconstructed DEF_PRE instance.

## Why this is the first unclosed nonempty branch

The current actual reader (`source/apps/moproj2mst/src/doe/keyword_reader_prefix.cpp:95-157`) appends the canonical Keyword on the named Study owner. For operation ID 1 it enters the contextual notifier, then throws at unresolved key construction. The empty-operations path commits a scalar text prefix and bypasses the notifier entirely; CP161 already tested that distinct path through an actual Session. It cannot establish this nonempty key. The bound `ProcessCodecOwner` still reports `Unknown`; its snapshot holds identity without a native read, Qt startup or produced QString (`include/m2m/engine/process_codec_owner.hpp:10-39`). The public nonempty InputTable Save remains independently guarded in `src/doe/doe_study.cpp:411,452`, so a key projection would not complete Open-to-Save.

Original GUI `0x1400d2995` calls dispatcher `0x1402cf3c0` with the exact NUL-terminated 21-byte `2signalSetOprHasDoe()` source. The unsuppressed dispatcher clears flag90 at `0x1402cf408`, calls `QString(char const*)` at `0x1402cf41f`, and only after constructor return at `0x1402cf425` proceeds to map/list lookup at `0x1402cf454`. QtCore's helper compares its C-string codec field at `0x67074126`. A null read takes the source-qualified fresh 21-unit, 74-byte-request branch; a nonnull read invokes a codec decoder whose result and owner are not established by the ASCII source spelling. The null branch is a conditional contract, not evidence that this invocation selected it. CP148 further shows that application setup has an earlier fromAscii read before its codec lookup/setter, so a language choice does not bootstrap this field. These claims are bounded by the CP146, CP148 and CP160 retained source qualifications; static addresses are preferred image addresses, not observed loaded bases.

The other independent nonempty candidates are later or need different values. DoeOutput Save has already wired the immediate `Control/Regions` mkdir attempt and operation-model lookup, then guards before cleanup (`src/engine/save_current.cpp:760-770`). CP156 identifies that branch's first missing natural QDir resolver return at QtCore `0x670b01ab`, followed by a separate QFileInfo engine result. Type3 optimization still needs its actual owner/lifetime and getter-to-Save effects; private scalar and region helpers do not admit its public route. An isolated forced-null QString helper, supplied Regions listing or private Type3 projection would repeat conditional behavior without advancing an actual production owner. I therefore select the nonempty Type2 key witness as the smallest next evidence acquisition, not a source patch.

## Exact witness and stop

After the project's supported GUI-control release operation and verifiable receipt/status exist, root may schedule one owned fresh original process and a disposable nonempty Type2 input under `C:\PROBLEM` with all generated paths below 100 characters. Freeze and hash the input, and record run/process generation, thread, module paths/hashes and loaded bases. The current app-control gate remains in force; this report does not authorize acquisition before it is satisfied.

1. Correlate the actual reader/manager call at GUI `+0x0d2995` to dispatcher and constructor at GUI `+0x2cf41f`. Record the constructor's C-string bytes including NUL, destination QString slot, notifier identity and stack base `S` immediately before the constructor call.
2. At the same natural QtCore helper invocation, record flags immediately after the comparison at QtCore `+0x74126` (for example `+0x7412e`), especially ZF and the taken null/nonnull edge. A separately sampled codec global is not the branch operand. If nonnull, record the actually reloaded pointer at `+0x74184`, codec/vtable target at `+0x7247e`, and the natural decoder result. Do not call or seed the decoder.
3. Follow the natural helper return to QtCore `+0x174be7` and constructor return to GUI `+0x2cf425`. At the latter, `RAX` is the QString object pointer; read its destination slot's **data pointer**, bounded header/ref/length and payload, or retain the actual exception/failure. Do not mistake the unexecuted spill slot for the return. Correlate the same `S`, process generation and thread throughout. Stop before map lookup at GUI `+0x2cf454`.

A null edge plus normal publication can qualify one actual key data owner. A nonnull edge requires its natural decoder output and ownership, not a guessed Latin1 conversion. A missed return, unreadable data, timeout or fault remains an incomplete occurrence. The subsequent map/list/receiver state and public Save still need their own evidence. No new GUI session was opened or released by this review.

## Verified identities and limits

Hashes below were rechecked from the imported converter tree for this review. CP160 additionally records the exact PE byte ranges and disassembly qualifications; this report does not turn a static capture into runtime reachability.

| Artifact | SHA-256 |
| --- | --- |
| `evidence/original-binaries/DEF_GUI_MO_64.exe` | `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` |
| `evidence/original-binaries/QtCore4_64.dll` | `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160` |
| `analysis/resume160-doe-codec-next.md` | `877b7287f1b5637f78cbf4e8e68b06b48dc447fd0248c4707b24e95bbd989488` |
| `analysis/resume148-process-codec-producer.md` | `18c9510ead42d1f5a0f120f79cbf19dd39207dda575465baf720f93c57b9aaf0` |
| `analysis/resume156-doe-regions-device-next.md` | `9286c0ca16d55fd0aecda68c55607a8a1d9a56519fdda96489d0fdef04c68e49` |
| `analysis/resume81-doe-qstring-source-67074120.asm` | `0dcd3d48f97c3f157d4df4bc9fb5d65bb99cf0690d8bf04d8e667a3296174f53` |
| `analysis/resume81-doe-qstring-source-67174bd0.asm` | `39562aa6405de1900dd8e7d5263aa6828b3f457d98cade572b6d5fcd070149e2` |
| `source/apps/moproj2mst/src/doe/keyword_reader_prefix.cpp` | `df1135df9b0d4410d551281845fb7e7c599b89d058b47056203242552d1a8a31` |
| `source/apps/moproj2mst/include/m2m/engine/process_codec_owner.hpp` | `5c353b68088024b586e322bcaedd055fbc220777b015e578c012270afcdea680` |
| `source/apps/moproj2mst/src/engine/save_current.cpp` | `bc77d02830ac0c75ea5fa669f303b818178610291fdbda837f1a1abb08185129` |

The current continuation check returned `CONTINUE`, 15 remaining items and eight pending gates. Historical B121 remains a difference with one differing `.moproj` and eight missing files; neither this review nor the earlier empty-operation tests change that result. Source file hashes are a snapshot and must be rechecked before implementation if another worker edits those files.
