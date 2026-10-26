# Checkpoint 80: isolated QString path data/ref owner

Implemented a typed isolated data/ref projection for same-data QString path leases and exact shared-null identity. It keeps represented native reference words separate from qt::String's text and from C++ observer ownership. Unknown counts/shared-null bindings remain explicit refusal points. No QFileInfo or custom-engine continuation is admitted.

Only three approved new source/test files and this report changed. Existing qt::String API/implementation, CP79 partial storage, shared Session/parser/writer, C ABI and CMake remain untouched. No build, executable test/CTest, original app, GUI or mutable REA action occurred. Root owns grouped validation/archive.

## Types and input provenance

New `include/m2m/doe_qstring_path_owner.hpp` provides:

- QStringPathOwner: caller-owned isolated scope, data records and native-ref state.
- QStringPathData: copyable weak scope/data-ordinal observer. Copying observers does not acquire a native QString reference.
- QStringPathLease: noncopyable/nonmovable explicit source-slot state, Empty/Active/Released. C++ destruction is inert; a caller invokes release at the actual reached native destructor phase.
- QStringPathRefObservation: optional32-bit reference snapshot plus explicit profile qualification. Absence remains unknown, never implicit0/1.
- QStringPathSharedNullObservation: exact QtCore SHA/RVA plus the optional ref snapshot.

Each concrete record keeps text value and, only when supplied, a separate atomic32-bit reference word. This is a reconstructed data identity/ref model, not the original QString::Data ABI header, allocation flags, external memory pointer or allocator. It does not coerce qt::String's std::u16string/null flag or C++ use_count into native shared-data references.

Input observations admit only a nonempty path profile with explicit qualification; null/empty input cannot fabricate a new shared-null identity. Known zero counts are refused as unavailable live input/shared-null admission, rather than interpreted as native constructor behavior. Unknown count profiles may be observed but cannot acquire/adopt a lease. These are narrow reconstruction guards, not added original QString validation branches.

Shared-null binding is absent initially and may occur once per isolated owner scope. It requires QtCore hash `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160` and QString symbol RVA0x2f0b30. QByteArray RVA0x2f0900, another module or replacement binding refuses. All shared-null leases in this scope refer to the same data record. Separate declared scopes are not claimed to be one original process; diagnostic qualification strings do not certify original runtime counts.

## Exact operations and mutation order

`observeInput` and `bindSharedNull` admit supplied snapshots into the isolated owner; neither represents a native reference increment or constructor return.

`adoptExistingInput` identifies one already-observed input slot without incrementing its native word. Adoption is adapter admission only and can occur once per Input record. It cannot adopt the shared-null baseline as a disposable root QString. Unknown count or occupied destination refuses before changing either slot or counter.

`acquirePathAlias` implements only the no-backslash source branch. After owner/count/destination checks, it stores the same data identity into the destination and atomically increments the32-bit reference. `copyAlias` repeats that acquisition from an Active source lease. `acquireSharedNull` acquires the exact bound shared-null record. No character buffer rewrite/detach, path normalization, QByteArray allocation or source-independent null object is introduced. An occupied destination is not treated as QString assignment; that separate release/assignment contract remains outside this helper.

Reference operations use unsigned32-bit atomic fetch_add/fetch_sub, preserving source arithmetic wrap rather than saturation. Source-level slot/meta operations require caller serialization; atomic ref words do not make the whole owner/vector/lease API thread-safe. Data scope/control-block/ordinal is checked before mutation, so foreign/expired observers cannot address a new owner's record by ordinal reuse.

`release` requires Active state, decrements exactly once and marks that slot Released. Nonzero result returns normally. Zero-after-decrement marks dataDeallocationPending and throws:

```text
QString path owner: QString data deallocation remains unsupported after reference reached zero
```

The decrement and consumed slot remain after the guard. No Qt data free, header-flag interpretation or native allocator success is fabricated. A second release and further acquisition refuse without changing zero. Retained diagnostic text/count can still be inspected, but not presented as live native storage. This is an explicit next deallocation boundary, not completed QString destruction.

All validation precedes destination changes on unknown-count/shared-null/foreign/occupied/unsupported-path failure. weak slot assignments and atomic mutations introduce no allocation after that admission step.

## Source sequence and cleanup use

With explicit input data P and distinct shared-null N snapshots, the isolated operations support the CP80-qualified sequence:

```text
adopt already-observed input slot                  (no native ref change)
acquirePathAlias(P, normalization temporary)        P+1
copyAlias(temporary, filename slot)                P+1
acquireSharedNull(native-path slot)                N+1
release(normalization temporary)                  P-1
release(native-path slot)                          N-1
release(filename slot)                             P-1
```

The caller owns source phase/order; this helper does not invent a file entry or run an implicit destructor graph. Copying engine-selection entries/path temporaries can use the same lease operations when a later actual source binding is assigned. The fixed resource's input data owner still must originate from the actual resource QString construction stage before production use; a supplied profile and matching text are not that binding.

Owner/lease C++ destruction releases adapter storage only and does not decrement represented native counters or replay Qt cleanup. This prevents unrelated C++ diagnostic lifetimes from simulating source destructors. Original constructor-unwind release order/raw0xe0 free remain qualified in `resume80-doe-qstring-path-ownership.md`; no new native cleanup execution is claimed.

## Focused tests and static checks

New `tests/doe/test_qstring_path_owner.cpp` has ten Testkit cases:

- Unknown input count, absent shared-null and unknown shared-null count leave destinations Empty and preserve unknownness.
- Observer copies/adoption do not increment; temporary/filename aliases share one data identity and perform exact increments/releases.
- File-entry-like source order keeps P/N distinct and releases native-path before filename, without QFileInfo admission.
- Exact module/RVA shared-null canonical binding; wrong byte-array symbol/module and replacement reject.
- Occupied destinations, duplicate existing-slot adoption, self-copy and Released source refuse before counter mutation.
- Last existing input reference is consumed at zero before the explicit data-free guard; no retry/deallocation success is invented.
- Unsigned32-bit wrap is preserved; an increment-to-zero does not itself trigger a free branch.
- Foreign scope, backslash profile, null input, known-zero live profile and missing observation qualification refuse.
- Adapter lease destruction leaves native count unchanged until explicit reached release.
- Expired scope observers cannot be used by another owner.

All counts are explicitly declared diagnostic profiles. They are not captured live native state. Tests are authored but unexecuted in this lane.

Static preflight resolved every quoted include against actual/local target roots, checked Testkit main and ten cases, supported active macros and no C assert/REQUIRE_EQ. `git diff --check` reported no assigned-path diagnostics; a direct scan also covered all new files and found no trailing whitespace. Source inspection confirmed no QFileInfo/Session/plugin/engine/public source binding or existing qt::String edit.

Existing glob discovery supplies target `m2m_doe_qstring_path_owner`, CTest `m2m.doe.qstring_path_owner`. Root runs builds/tests; none ran here.

## Frozen sources

Paths are beneath `source/apps/moproj2mst/`.

| New file | SHA-256 |
| --- | --- |
| include/m2m/doe_qstring_path_owner.hpp | A6B453D7E816B19DE243F6B4ACE6678B9E707680D34EE803DFBD242F51039C99 |
| src/doe/qstring_path_owner.cpp | D3E1AC3D88226128A3ED5E26795EEA7C72BCA96B5FEA8D4C3EBA154BBC99422C |
| tests/doe/test_qstring_path_owner.cpp | AD962E87D1C1E778A465A35C6F8F1541D8E99FDDAEEFD43257BE3B57ED592AC3 |

All three source files are held unchanged after this receipt for root's source-stable build/test/archive. Controlling evidence report `analysis/resume80-doe-qstring-path-ownership.md` hashes to `BF4962D2FF23AF9880301DED3B91B26D8B29CBA9FCE5ECC0781EBCD2443D912C` and binds immutable QtCore code/cleanup receipts.

The next production dependency is an actual resource QString data/ref owner and process shared-null binding with current counts/lifecycle, then explicit destination-slot integration into QFileInfo's construction and cleanup. This isolated owner supplies no ready-QFileInfo capability, shared handler registry, plugin engine, receiver delivery, DoeTableRefresh or public Type2 completion.
