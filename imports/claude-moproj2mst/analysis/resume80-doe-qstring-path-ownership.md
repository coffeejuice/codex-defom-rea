# Checkpoint 80: QString path leases after QFileInfo ref0

The next native mutation after CP79 ref0 is copying the actual input QString data pointer into a temporary and incrementing that data's32-bit reference count. The fixed DOE path has no backslash, so this is a shared-data lease with unchanged UTF-16 text, not a detach, normalization allocation or byte-array conversion. Candidate qt::String does not expose that native data/ref owner; its text value alone cannot safely advance the current guard.

The subsequent file-entry constructor adds a second filename lease plus the actual QString shared-null native-path lease, initializes its cache words and releases the first temporary. File-engine selection then makes further temporary leases before its actual custom-handler gate. New bounded cleanup evidence establishes the release order and the constructor-failure raw deallocation boundary.

Only uniquely prefixed static evidence and this report changed. No candidate source, build/test, GUI/app or mutable REA action occurred.

## First exact source-closed mutation

CP79 ends after private reference0 at `0x670b8052`, before QDir::fromNativeSeparators call `0x670b8059 -> 0x670ad700`. The helper searches the actual UTF-16 text for backslash. For fixed `:/SfTool/icon/tool/iconDoe_24_24.png`, that search returns-1 and the source path is:

```text
0x670ad7e2 read input QString.data
0x670ad7e5 assign returned QString.data to that same pointer
0x670ad7e8 LOCK ADD DWORD[data.ref], 1
return the temporary QString
```

There is no character buffer rewrite or new QString data allocation on this branch. The pointer assignment precedes the reference increment; they are not interchangeable with a text copy, count observation or C++ shared_ptr use_count. A backslash-containing path enters a distinct shared-copy/detach/replace branch; it remains outside the fixed-resource prefix.

The smallest ready **native operation contract** is this one same-data temporary acquisition. It requires the actual source QString slot/data identity, current reference owner and destination slot. Without them the smallest sound active boundary remains QStringPathOwnership. Source knowledge of the transform is complete; candidate ownership integration is not.

## File-entry construction before engine selection

Retained `0x670b8030`/`0x670eed10` execute in this order:

1. First normalization call above creates a temporary QString lease from the input.
2. Private+8 file-entry constructor calls fromNativeSeparators again. The same fixed no-backslash branch copies that temporary's data to the entry filename and increments its actual data reference once more.
3. File-entry+8 (whole private+0x10) receives QString::shared_null at `0x672f0b30` and increments that object's actual reference. It is a QString native-path slot on this Windows build, not QByteArray::shared_null `0x672f0900`.
4. File-entry signed16-bit cache words+0x10/+0x12 become-2 and+0x14 becomes0 (whole-private offsets+0x18/+0x1a/+0x1c).
5. Private ctor destroys its initial normalization temporary at `0x670b806c–670b8081`, decrementing the input's shared data. Net result is one filename lease and one shared-null native-path lease on this private file entry.
6. Metadata words at private+0x20/+0x24 are set0. Then call `0x670f0080` at `0x670b808e`. The engine+0x50 store occurs only after that returns; outer wrapper publication and private ref1 occur even later, after the whole private constructor returns.

At this point cache-array objects, remaining flags and tail fields have not been initialized. The prefix must not zero them or expose a completed private object merely because the first entry slots are known.

Let P and N denote the actual input data and actual QString shared-null owners, with starting reference words p and n. This table names only leases in the selected constructor scope; initial counts are not supplied by static PE bytes:

| Source stage | P delta | N delta |
| --- | --- | --- |
| First normalization temporary | +1 | 0 |
| File-entry filename copy | +2 | 0 |
| Native-path shared-null slot | +2 | +1 |
| Destroy first temporary | +1 | +1 |
| Copy file entry for engine selection | +2 | +2 |
| Copy path QString for first handler call | +3 | +2 |

The table assumes P is the nonempty resource data, distinct from shared-null N. Do not assign p=1, n=1 or substitute a default-constructed empty counter: other live QStrings share the actual global N. The reference instructions are32-bit atomic arithmetic; they do not create a new global owner from a number.

## The exact first custom file-engine gate

`0x670f0080` copies both entry QString data pointers and increments each, plus copies its three cache words. It sets a local output engine pointer0, then calls `0x670efbc0` with existence-check flagfalse.

That resolver calls file-entry path getter `0x670ef040`, which first checks filename readiness through `0x670eedd0`. New bounded evidence confirms that a nonempty filename returns immediately from readiness; the alternate empty-filename/native-path reconstruction is not entered for this literal. Getter then copies the filename data pointer at `0x670ef060` and increments P at `0x670ef063`.

Resolver next calls `0x670a7dd0` at `0x670efc00`, before testing the colon prefix. The handler gate first reads actual global byte `0x672f3ed0`. Nonzero enters the actual global handler list/lock owner and invokes handlers in order until a nonnull engine. Zero returns null from that helper. No source-bound handler flag/list/current contents have been represented for this reached constructor path.

Therefore there are two distinct future guard positions:

- Current minimum guard: before first QString data/ref acquisition at `0x670b8059`.
- After genuine QString lease ownership is implemented: preserve all above leases/cache/metadata effects and guard before the actual handler global-state decision/invocation at `0x670efc00 -> 0x670a7dd0`.

Do not skip the handler gate because the literal begins `:`. The resource-engine allocation/constructor occurs only after this first custom handler attempt returns null. QFileInfo suffix and icon QFactoryLoader V2/legacy plugin decisions remain still later. A path value `.png` cannot choose an empty handler list, null plugin result or default engine.

## Failure cleanup and lifetime

New complete selected QString destructor `0x67020c70` decrements its actual data ref atomically; it calls QString data deallocation `0x670700d0` only when the decrement reaches0. A copied QString lease is not automatically a deep-copy owner, and ref decrement is not unconditional data free.

New file-entry destructor `0x670a75d0` releases **native-path slot+8 first**, then filename slot+0, each with the same decrement/free-on-zero rule. This order applies both to the temporary engine-selection entry and the private entry already constructed. It is not interchangeable with byte-array cleanup.

For an exception from the first custom handler route, selected MSVC FuncInfo/IP-state maps and newly disassembled cleanup funclets establish:

1. `0x670efbc0` state0 destroys its local path QString via funclet `0x67192e70 -> 0x67020c70`.
2. `0x670f0080` state0 destroys its temporary copied file entry via `0x6718b8d0 -> 0x670a75d0`: native-path QString, then filename.
3. Private QFileInfo constructor state2 destroys its installed entry via retained `0x6718a060 -> 0x670a75d0`, in that same slot order.
4. Outer QFileInfo constructor state0 invokes retained `0x6717b5e0` to free its raw0xe0 allocation. Private constructor never returned, outer wrapper was not published and ref1 was not assigned.

For these selected file-entry/path leases, the count deltas unwind back to their constructor-entry baseline, subject to any independent effects a handler actually performed. This is not rollback of handler side effects. The enclosing QIcon shell is already constructed; CP78's receiver-level QString cleanup can release its resource argument after addFile unwinds, but there is no completed-icon automatic delete in that caller. Keep those enclosing scopes separate.

Earlier failures have narrower cleanup: if the first normalization call never returns, no normalization temporary/entry has been constructed and outer raw construction cleanup remains. If entry construction refuses/throws after a temporary has returned, use its actually reached construction states instead of releasing every planned slot. The report closes the selected normal fixed-path/first-handler exception sequence, not arbitrary plugin/resource engine failure or all MSVC unwind descendants.

An adapter guard may retain diagnostic prefix bytes in its own arena; that is not proof that the original raw allocation remains live after a real native constructor exception. No new destructor/guard semantics should be inferred from the candidate arena's lifetime.

## Missing candidate owner/interface and smallest implementation proposal

`include/m2m/qt4.hpp:15–63` implements qt::String as std::u16string plus a null flag. It has no native QString data pointer,32-bit ref word, shared-null identity, lease-construction state or deallocation event. Copying its text or using C++ use_count would fabricate the missing effect. Existing ReportQObjectSharedRefObservation is an inert Report-specific ledger, not a reusable mutable actual QString/global owner; do not relabel it as the resource string's live ref state.

Before production advancement, bind a typed source-owned input QString data/slot and an actual Session/process QString shared-null owner. Their owner identity and native-ref lifecycle must be distinct from adapter smart-pointer lifetime. The fixed input handle must originate from the actual resource QString construction stage, not be reconstructed with an invented starting refcount from the descriptor's text. N must be the shared owner for this module SHA and RVA0x2f0b30; a per-call null data block or QByteArray null owner is invalid.

The smallest isolated implementation after those interfaces exist is `acquireNoSeparatorPathTemporary(sourceHandle,destinationSlot)`: validate the admitted exact no-backslash input, assign the same data identity, perform its one acquired reference, and preserve that lease through later source stages/cleanup. It must not eagerly initialize the filename, native path, caches or engine. A broader file-entry helper can then repeat acquisition, initialize the distinct shared-null lease/cache words and release the initial temporary in order, ending before custom handler state. This is a future ownership seam, not source work authorized/performed by this report.

Useful tests must use actual typed owner/counter objects with provenance and no fake text-only coercion; verify same data identity and one increment, distinct P/N owners, entry slot offsets/cache sentinels, temporary release order, native-path-before-filename cleanup/free-on-zero and no publication/ref1/engine. Negative tests should reject wrong module/null symbol, unavailable input owner, nonfixed/backslash profile and missing cleanup ownership before mutation. Supplying diagnostic counter values can test an isolated transform only; it does not prove a fresh public owner state.

## Evidence receipt

Original QtCore SHA-256 remains `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. CP79 path/private/handler/FuncInfo receipts remain controlling. New bounded evidence is six dumpbin selections totaling9,398 output bytes, plus the selected early unwind-state receipt. No full Qt runtime inventory was captured.

| Artifact | SHA-256 |
| --- | --- |
| analysis/resume80-doe-qstring-cleanup-static.json | DA3004848011B7F1CFA37A5171F1C9312149B18512DF0F9E9E27C85F796B285C |
| analysis/resume80-doe-fileentry-path-static.json | F9100EDAE79040C989CBE1575F86827F186DA849B3DF5D4CB8D056FA154A70BD |
| analysis/resume80-doe-fileentry-path-ready-static.json | 25F5F0711508174D980FE261F4216F52CDCA510C3B0CAB116DD71D630B5592F1 |
| analysis/resume80-doe-path-unwind.json | CD9353A4989AE76D84ED0DB1092C2B67E26123385E116F1910AA310568E513E3 |
| analysis/resume80-doe-path-unwind-actions-static.json | B0EDDDAFE3036612B8F62B09E80835F3BCFBE2ED3E4F87FA50E6E10DE54C63E2 |
| source/apps/moproj2mst/include/m2m/qt4.hpp (inspection) | 72AD25FD2AE6412C2C7043DD73A43F53398A9DE9373A7834AD1FAAB2061E6261 |
| source/apps/moproj2mst/include/m2m/doe_fileinfo_private_prefix.hpp (inspection) | 1FAF67C98CDE0623EF18D1961275839013C78115FDCE385CAD3220BCA243A5DC |

Public Type2/SetOprHasDoe/addFile/DoeTableRefresh remain guarded. No runtime native reference values, handler population, resource/plugin result or full-feature completion is claimed.
