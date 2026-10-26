# CP156: Regions root probe and the smallest remaining occurrence witness

The original constructor and query narrow the next branch, but they do not choose the actual Regions device. The first missing value remains the QDir resolver return stored in private+0x20. The following root QFileInfo has a separate fresh engine resolution and zeroed metadata; a native/null engine there necessarily reaches metadata fill before isDir returns. This report closes that fresh-root constructor/query edge and specifies the minimum correlated witness. It adds no source or arbitrary native-device profile.

Only this report was written. Immutable PE/DUMPBIN inspection and retained reports were used; no GUI, running original, mutable REA, source/test edit, compiler or build was used. CP149's wired directory/model/cleanup guard remains the implemented boundary.

## Evidence reconciliation and stop condition

`resume150-doe-output-device.md` and independent `resume153-doe-device-independent.md` agree on the missing actual QDir/FI/device results. `resume122-doe-handler-state.md` already closed the static handler inventory: constructor/destructor and conditional flag-zero branch are known, while dynamic module/registration history is not. No later same-occurrence Regions return receipt was found in those reviewed artifacts.

A bounded filename/content search for the handler symbol and resolver sites among retained Markdown and observation/probe/runtime JSON families located the controlling static reports, rather than a completed Regions runtime witness. This is a statement about that bounded review, not exhaustive proof that no other artifact could exist. No new import or direct-call absence scan can convert it into a live-state result. The source/observation distinction remains controlling.

## Actual owner and first missing return

After successful mkdir and a nonnull actual operation model, native DoeOutput re-fetches its active SUB at GUI 0x140e2e79d, derives the current Control/Regions path and constructs a QDir at 0x140e2e8bf. This is the selected object, not an arbitrary QDir for an equal pathname and not a copy of the candidate mkdir adapter's attribute result.

Its private path setup calls QtCore 0x670f0080 at **0x670b01a6**. The next instruction/return site is **0x670b01ab**; RAX is the resolver's engine result while RBP identifies that QDir private object. A changed result is published into **private+0x20 at 0x670b01b4**. This is the first narrow, authoritative selection event. The constructor's earlier null initialization is not the completed result.

The same canonical handler byte QtCore+0x2f3ed0 is read inside the reached resolver lookup. Qualified flag0 with the applicable normalized drive/UNC path selects a null engine; nonzero can invoke actual ordered handler callbacks before returning null or nonnull. Absolute syntax and successful mkdir alone do not determine either result. A nonzero flag followed by all-null handlers is also not the no-callback flag-zero profile.

An actual null engine return would qualify this QDir's null branch. It would not establish the independent QFileInfo resolver value, future file removals, or the complete directory snapshot. Nonnull requires the actual returned engine owner/vtable/lifetime and callback results; it cannot be normalized to a native filesystem object.

## New narrowing: the first QFileInfo is a fresh root query

The first cleanup action is helper 0x14081aec0, before entryList. It obtains QDir::absolutePath, constructs **QFileInfo(QString const&)**, asks isDir and destroys its locals. This is distinct from the later per-entry QFileInfo(QDir,name) constructor.

The export/import binding identifies the root constructor as QtCore **0x670b9a00**. Its normal allocation path requests **0xe0 bytes**, calls private constructor **0x670b8030 at 0x670b9a34**, publishes the private pointer at 0x670b9a3d and increments that private refcount at 0x670b9a45. A null private allocation is a different condition from a successfully constructed private object with null engine; do not encode the former as a qualified native-device result or ordinary isDir=false.

Private constructor 0x670b8030 normalizes/builds the root filename entry at +0x08, zeroes both metadata words at +0x20/+0x24, calls **0x670f0080 at 0x670b808e**, and stores that independent engine result into **private+0x50 at return site 0x670b8093**. It clears invalid-state bit0 and sets cache-enable bit1 in byte+0xb4. The fresh object's metadata is not inherited from QDir, its entry cache or the earlier mkdir query.

`QFileInfo::isDir` at **0x670b8fa0** rejects invalid-state bit0, then tests private+0x50. For the completed fresh private construction above, the invalid bit is clear. If engine+0x50 is nonnull, the method goes through **0x670b7950** with requested mask0x40000; its actual engine result remains required.

If engine+0x50 is null, the method's cache-enable test cannot supply a known directory bit: fresh metadata-known word+0x20 is zero. Consequently it calls native metadata fill **0x67102090 at 0x670b8fe3**, requesting mask0x40000. After that call returns, isDir reads bit18 from metadata-result word+0x24. It does not turn the fill helper's Boolean directly into the type result. Thus even the first false result cannot be inferred from a host probe failure or replaced with mkdir's earlier Boolean.

The native fill is not a side-effect-free bit getter. Its entry expands the requested mask with0x0f760000 and clears the corresponding cached result bits. The bounded body has path normalization, then on the attribute-query branch calls **SetErrorMode(0x8001) at 0x671022eb**, retains its previous return value, calls **GetFileAttributesExW at 0x67102357**, and restores the previous mode through SetErrorMode at **0x67102435**. Other failure/fallback code includes 0x670feab0/0x670fe9d0. This report establishes these calls/order, not a complete Windows metadata/error adapter. A future implementation must preserve the reached branch's process error-mode interval and fallback behavior instead of substituting `std::filesystem::is_directory` and claiming parity.

## Minimal correlated witness, without manually invoking a device

The smallest first-stage witness is one **natural Regions QDir resolver return**, not an entire handler registry history:

1. Record process/module generation, verified GUI and QtCore identity, loaded bases, thread and call sequence. Correlate the actual DoeOutput callback, current operation/model and fresh active SUB with the QDir caller at GUI+0xe2e8bf. Record the actual normalized Regions path and object/private identity; equal text in another construction is insufficient.
2. At the return site **QtCore+0xb01ab**, record RAX and the live private object whose +0x20 is about to be updated. Preserve call/return correlation with +0xb01a6 and the resolver's applicable path. This closes only this object's engine selection. It need not reconstruct every earlier registration if the actual natural return itself is captured faithfully.
3. To qualify the first root classification, separately correlate GUI+0x81aec0 -> QFileInfo(QString) -> private constructor, then record the actual engine result at **QtCore+0xb8093** and the live QFileInfo private generation. Null here selects the fresh native metadata query just proved; nonnull retains its actual callback boundary.
4. If execution naturally proceeds, record the normal metadata/isDir return and resulting known/result bits for that same fresh object. A null QDir or QFileInfo engine is not an isDir result. Further entryList, per-entry engine, iterator/sort/removal and later Save returns remain outside this first witness.

A gate-byte observation in the reached handler lookup can additionally distinguish no-handler from all-null callback traversal. Do not invoke a handler, getter, virtual method or QFileInfo query manually to manufacture those results. Do not seed a list or current count. Do not use another process, a standalone Qt harness, a historical InputTable observation or a later file operation as this Regions occurrence. This report initiates no runtime observation and does not authorize bypassing root's GUI/REA serialization.

## Source-ready disposition

The conditional call/owner/ordering prefix is source-qualified. Production device selection remains unbound, so there is no further faithful wired mutation to assign from this report alone. Keep CP149's successful nonnull-model guard before QDir construction/enumeration. Do not add another path-value, supplied-entry or forced-null helper merely to move the guard.

When a real selected-call witness or equivalent source-executed producer exists, root can coordinate a canonical process/file-engine owner and the actual caller together. The first eligible cut would retain the completed QDir resolution and root QFileInfo construction/query on those actual owners, guarding nonnull callbacks and unresolved metadata behavior. Enumeration/cleanup must still preserve CP150's snapshot order, root retention, recursive-false propagation and ignored ordinary removal results. Existing Open DBLIST/task, failed-mkdir modal return, geometry, `.DS`, refresh and public success guards are unchanged.

## Bounded receipt

Original QtCore SHA-256 was rechecked as `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`; GUI identity remains `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Import-table parsing resolves QtCore IAT0x671a8228 as SetErrorMode and0x671a8280 as GetFileAttributesExW. Bounded DUMPBIN inspection used actual entry addresses; only this report is a new artifact.

| QtCore normal byte range | SHA-256 |
| --- | --- |
| 0x670b9a00 /92 bytes, through RET0x670b9a5b | f0546b76197a888574f8f0ea082d37f0b7df982f054ff3a45cf95c8ca4e49c24 |
| 0x670b8030 /272 bytes, through RET0x670b813f | 96cea90fbc376d425a7e43d474d8c7a650dd10254a15fb459b41dbf9aec158e2 |
| 0x670b8fa0 /118 bytes, through RET0x670b9015 | 97e59e2b7df22a586631b9c9d06da8540690d94b91850f66a996a457cf052c4d |
| 0x67102090 /1343-byte bounded unwind region | aad260c02b5ec510a61e0f619f0b8a95f65a9fabb3b37cfe0cc53047006e5e3f |

The metadata range is a bounded code-region receipt; no claim of complete fallback/exception closure follows from it. Prior report hashes checked: CP150 device `0f80e1a356e589a012ccd3208ad58558805859b7642ac88d7d82f0af6afcc030`; CP153 independent audit `adcc830f56f5d4f7e7ddc3da9be24a3ea7ff4e0c0a7e9bd84524bd3d8e561823`; CP122 handler state `cef1da376df5bdec9ccd4c049091115531fb358e41877f852aa65ae9b4b2aed5`. No current runtime value is inferred from these static hashes.
