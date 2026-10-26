# CP146: canonical process codec ownership across the three QString sites

The process field is a borrowed QTextCodec pointer in QtCore, not a Session-owned codec or QString data owner. Static evidence now binds its raw setter and a separate codec registry/cleanup lifetime. The smallest integrated candidate correction is one inert, initially-Unknown process owner outside Project/Session teardown, borrowed by the real Session and shared by SetOprHasDoe, refresh and icon occurrences. That correction must preserve the current guards until a source producer or same-occurrence codec read is qualified; it must not create a Null default or another arbitrary key helper.

Only this new report was written. CP145 source/test freezes were preserved. No source/build/GUI, original execution or REA tool call occurred while the graph lane owns REA. New bounded disassembly reads used immutable QtCore with Microsoft DUMPBIN14.44.35228.0. No new mutable analysis database or raw-capture artifact was created.

## Native field and source producers

The immutable GUI SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; QtCore SHA-256 is `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`.

QtCore exports data `QString::codecForCStrings` at RVA2f3e10, and the accessor/setter separately. The field lies in the module's .data zero tail: section RVA2f0000, raw size3c00, virtual size4a90, symbol offset3e10. The loader image value is therefore null **before module initialization/intervening code**, not a late invocation observation.

Setter export RVA177590 is exactly the8-byte normal body: store RCX to QtCore base+2f3e10, then RET. It performs no retain, delete, ref increment, QObject-parent operation, pointer validation or same-value skip. Replacing its value does not transfer ownership of the old/new QTextCodec. Setter arguments may be null; a source-qualified null argument would establish a current null store at that event. The mere absence of a setter call in one bounded routine does not establish that the prior field was null.

CP83 already closes the GUI producer's order: application constructor0021c0 calls codec setup0024f0 before main-window construction. Setup reads the raw language cache143d79718; indices1/2/3/4/7 select Shift-JIS/Big5/KOI8-R/EUC-KR/GBK and call codecForName at0026c0. Only a nonnull result invokes locale, translation and C-string setters at0026d8/0026e3/0026ee. A skipped language branch or null codecForName result leaves the C-string field unchanged; it does not store null. The language cache, host ACP, locale codec and C-string field remain distinct.

The current retained code does not close all pre-application helpers, QApplication/internal/plugin activity or later writers through the exported field/setter. A bounded direct-RIP candidate scan in QtCore found the setter as the only matching direct store among the selected instruction encodings; confirmed read sites include74126/74184 and other QString helpers. This is not proof against indirect writes, exported-data imports, other modules or alternative encodings. It cannot replace a reached read.

## Codec object lifetime is a separate registry

New bounded immutable bodies show:

- QTextCodec constructor RVA14c1a0 publishes the base vtable, locks the codec-global synchronization helper, ensures registry setup14c840 and inserts `this` into the pointer list rooted at QtCore+2f4430. This is separate from the C-string field.
- Base destructor RVA14c5d0 checks that registry, locks, removes this pointer through2b130, and invalidates its codec lookup cache through14c4f0/14c2b0. No direct write to2f3e10 appears in the captured base body. Its nested/derived destruction must not be assumed to leave all process state unchanged without further closure.
- Registry cleanup14c100 first loads the global list and **sets2f4430 to null at14c115**, then walks nonnull entries and invokes deleting virtual+28 with flag1 at14c150. It later releases list data/container and directly resets locale codec2f4438 at14c186. The captured cleanup body does not directly reset C-string2f3e10 or translation2f4440.
- The global-static helper14c440 registers cleanup callback1a70a0 through178b54 ->178aa4; the latter reaches the MSVCR90 `_onexit` import at IAT1a84c8 on its corresponding path and maintains the CRT exit-list state. Callback1a70a0 calls14c100, frees its holder, clears the holder pointer and marks it destroyed. This is module/global cleanup machinery, not normal Project Close.

The captured global cleanup can therefore own codec destruction independently of the pointer field. A field may remain nonnull while its pointee is dead/unqualified; do not turn a liveness invalidation into an invented native null store. During cleanup, registry-null is already a reached effect before the first deleting virtual; failure/unknown derived destruction cannot be represented as all-codecs-dead completion or rollback. Exact shutdown callback order and every derived destructor are not closed by these bounded bodies.

The process model needs three distinct owners/facts:

| Fact | Lifetime/authority |
| --- | --- |
| QtCore C-string field | One process/module generation; stores a borrowed pointer value, independent of Project/Session. |
| Codec object / registry entry | Separately qualified construction, registry membership, decoder identity and liveness; setter does not own it. |
| Decoded QString data/ref cell | Created/retained by the actual constructor/decoder occurrence; aliases share this one cell, not a codec pointer or Session value copy. |

## Reached read versus historical producer

The CString constructor path at174bd0 ->74120 first compares field2f3e10 at74126. Null selects the source-qualified fresh-data branch without rereading that field. Nonnull later loads the actual codec pointer again at74184 before the virtual toUnicode wrapper. A nonnull admission must bind the used pointer/liveness at that load, not assume the earlier comparison permanently captured an unchanged object.

A valid process owner may record the initial image-null fact as history and later source setter events. Its **current** knowledge must become Unknown across an unclosed writer interval; an epoch does not itself prove that all writers were observed. A source setter event advances history even for the same pointer because native performs the store without equality branching. Known nonnull pointer value, live decoder and returned QString data owner must remain separate admissions.

CP130's observed null pointers in another editor startup process/stage are valid for those historical occurrences only. CP145's actual Session/Project/I/O checks qualify notifier ownership, not this module/process field or its writer interval. Neither may be promoted to a late SetOprHasDoe/refresh/icon read. Existing isolated profiles remain explicit modeled assumptions, not native observations.

## One process owner, three occurrence types

All three sites must borrow the same canonical process codec knowledge and data/ref registry:

| Occurrence | Source / exact fixed payload | Null-branch request |
| --- | --- | --- |
| SetOprHasDoe | GUI0d2995 ->dispatcher2cf41f, C string142166b40, destination rsp+98;21 bytes |74 bytes |
| DoeTableRefresh | GUI0d369d ->same dispatcher callsite in a different invocation, C string142166b90, destination rsp+98;24 bytes |80 bytes |
| Icon resource | GUI086d68, C string14213afd8, destination rsp+58;36 bytes |104 bytes |

The source/stack slot/call generation distinguish occurrences even when callsite or text repeats. Each admitted nonempty null-codec constructor creates its own source-initialized ref1 data cell. Do not cache data by key text, share an icon cell with a notification key, acquire shared-null for these nonempty branches or copy a count into a second canonical cell. For nonnull decoding, ownership comes from the actual returned QString, not this null-branch rule.

Current QStringResourceProcessScope already has useful canonical ref/lease mechanics but is isolated and icon-specific. Its constructor allocates a pimpl, its codec defaults Unknown, its isolated setters cannot certify production, and `constructSourceResource` always refuses. It is not currently owned or borrowed by ConversionSession. ApplicationStateProjection handles different process DWORD observations only; it contains no codec owner. CP104's broader ApplicationState insertion report is a design artifact, not a currently wired codec scope.

## Smallest integrated correction

Introduce or refactor one **inert process-scope owner**, then borrow it through the actual Session:

1. The real run driver owns the process model before constructing Session and keeps it alive through Project teardown, Session destruction and catch/details publication. For a selected fresh-original-process conversion profile, one run may own that scope; if multiple Sessions represent one original process, their caller must supply the same explicit owner. Never merge unrelated conversions in an implicit host singleton or reset process state at Project Close.
2. Default construction must be noexcept/no-allocation/Unknown with no native source effects. Current QStringResourceProcessScope allocates its pimpl, so simply placing it before the run try would introduce an uncaught host-allocation path. Use an inert value header/lazy diagnostic storage or perform allocation inside existing error handling. Do not label pimpl allocation as original QString or codec construction.
3. Add an actual Session borrowing constructor/accessor for this owner; preserve the current packet constructor with unavailable process binding. CP145's synchronous context already carries actual Session identity, so it can find that same bound owner without another mutable global/Study back edge or extra C ABI argument.
4. At each reached key/resource source, validate the same process/module generation and source occurrence. Unknown production codec still stops before allocation; NonNull stops at the actual decoder/liveness/data-owner guard. A verified Null read can use the shared canonical fresh constructor mechanics with the correct occurrence descriptor/length.
5. Keep a reached constructor lease/occurrence on that canonical owner or a caller-owned outcome across the reconstruction guard. Host scope cleanup must not pretend a native QString destructor, codec deletion, CRT shutdown or raw-pointer reset ran. Project release revokes Session access but does not reset the process field or free process codec objects.

The source-ready correction is **ownership wiring with unchanged admission**, not a native codec default or key helper. It gives all three consumers one authoritative process field and prevents per-key/per-Session clones. No actual key allocation, map/list operation or receiver return becomes supported merely because that holder exists.

Minimum coordinated root scope: `src/core/convert.cpp`, `include/m2m/engine/conversion_session.hpp` and `src/engine/conversion_session.cpp`, plus one canonical process-owner declaration/implementation if no existing application-global holder is adopted. A worker can then refactor existing `doe_qstring_resource_owner.hpp/.cpp` and `doe_qstring_path_owner.hpp/.cpp` under root assignment, preserving their isolated profile tests and separating source provenance from diagnostics. Actual key/refresh/icon consumers must share that owner rather than each adding a new arbitrary-key runtime. There is no source assignment in this report.

An actual source setter/read producer is still required after this inert wiring. Qualified application setup may publish a borrowed codec identity when its codecForName return and registry/decoder lifetime are closed. Image load history can record initial null but cannot be used at the later read through unclosed startup calls. A runtime read can qualify a same-occurrence branch only with process/module/TID/source-slot identity; it does not retroactively prove all intervening setters or a callback return.

## Focused validation for the correction

Tests should establish that one bound owner survives normal Project release, can be shared by explicitly same-process Sessions, stays distinct across separate profiles and is unavailable to legacy packet Sessions. Default binding must allocate no native key/data and leave current public outcomes unchanged. Actual and isolated codec knowledge must not cross-admit; same-pointer setter events, unknown-writer invalidation, stale pre-read events and null-selected no-reread behavior remain explicit. Nonnull pointer death invalidates decoder admission without inventing field=null.21/24/36-byte occurrences share codec history but create distinct data/lease identities and retain their own allocation/cleanup boundaries.

Root should test that the owner exists through catch/details and that no destructor or Project Close silently resets it. A source-qualified CRT registry cleanup prefix may separately retain registry=null before a deleting-virtual guard; host RAII disposal is not that event. No GUI/REA observation is needed for inert ownership tests, and none would prove a production codec value without the missing source input.

## Compact new static receipt

Byte intervals below were decoded from their actual entries. Several Qt logical routines have split unwind regions;14c100 and14c440 ranges deliberately include their forward normal branches rather than treating one unwind subrange as a whole function. Bytes after the last stated RET are excluded.

| QtCore preferred VA / bytes | SHA-256 |
| --- | --- |
| 67177590 /8 | e0fd2f5b42afae743dcd067fd5e2870239bccee69a03501de4cabcc033ea76f1 |
| 6714c1a0 /221 | 3672ecbcc1e9a758db2e151144d2e048c27d8a687e78371927ddd96a60f1796a |
| 6714c5d0 /232 | e45f9eb685c5e8c729104bdc125346d59208ea100014b52a95e85066f7ce5724 |
| 6714c100 /151 | 37ba199a4ce2ff31685cdc69413054a44a9cf7d7d07b025da7491485ee38b33d |
| 6714c440 /173 | daffac0271e07bfe9f68145cbabd21e5254cd9450b868966eb495cd0c049be79 |
| 671a70a0 /68 | 537790b0b27b815247fb90f79ad7714d79178e4704749f53589b3045246d484e |
| 67074120 /181 | 582c5dacfd3f6edb8acfe48a648f52369b8fa88e8a45de00700dd9a4ab71f3de |

The next native value dependency remains the reached process codec/decoder/data publication at the selected source. This report adds a source-backed lifetime correction and new registry/cleanup evidence, not another scan-based claim that an unobserved codec must be null.
