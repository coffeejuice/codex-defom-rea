# CP83: actual process codec producer and Session admission

The original application constructor is the source owner of codec setup, before main-window construction and before the later DOE receiver. The language cache and process C-string codec are separate mutable globals. The smallest actual admission must bind the process codec state at the resource QString helper's reached read to the same process/application and actual Session/model occurrence. Startup language, PE zero-fill and CP82's isolated Null profile cannot produce that admission.

This lane changed no source while CP82 validation/archive is pending. It used retained PE/captures, a new bounded candidate receipt and one read-only pre-application capture. No GUI control, running target, build, tests or analysis database mutation occurred.

## Recovered original order

Original GUI SHA-256: `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. QtCore SHA-256: `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. Addresses below are preferred image addresses, not invocation-time loaded addresses.

1. Retained `wave3-editor-owned-open-entry-140001410.asm` shows the application entry routine calling `0x1407fc450` at `0x140001461`, then constructing the application subclass in stack storage `rsp+0x520` at `0x14000147e` via `0x1400021c0`.
2. Retained `wave3-editor-startup-language-install-chain-1400021c0.asm` shows QApplication construction at `0x1400021f1`, subclass vtable publication at `0x140002204`, a call to `0x1407ffc10` at `0x140002210`, and callback registration at `0x14000221c`. Codec configuration `0x1400024f0` is called at `0x140002227`; translator configuration `0x140002290` follows at `0x140002231`.
3. Codec configuration reads the raw language cache at `0x143d79718` at `0x14000251a`, before translator configuration uses the language getter. Values 1/2/3/4/7 choose Shift-JIS/Big5/KOI8-R/EUC-KR/GBK. Other values skip the codec setter section. For a selected name, `codecForName` is called at `0x1400026c0`. Only a non-null return reaches locale, translation and C-string setters at `0x1400026d8/0x1400026e3/0x1400026ee`, in that order.
4. Retained main entry later constructs the main window at `0x140001b39` via `0x140010490` in stack storage `rsp+0x40`, after application construction returned. This is the earlier source lifetime than the Session/ModelManager projection; a Project is not the process-global codec owner.
5. CP77/78's delivered SetOprHasDoe chain enters the matched true flow-model receiver `0x140086ca0` only after ordered registration/lifetime/model/backend/logical-item checks. It allocates QIcon and constructs QSize before the resource QString call `0x140086d68`, destination `rsp+0x58`.
6. CP81's constructor `0x67174bd0` calls `fromAscii_helper` `0x67074120`. The helper reads current C-string codec at `0x67074126`, QtCore RVA `0x2f3e10`. Null selects the recovered fresh 104-byte/ref1 resource branch; non-null selects actual codec virtual+0x18. That reached read, rather than a preceding language choice, selects the branch.

These are source ordering relationships. They do not prove a particular original invocation reached every step or exclude state changes through the intervening calls/events.

## Language initialization and later mutation

The language cache is in the zero-filled virtual tail of GUI `.data` (RVA `0x354c000`, virtual size `0x1b09344`, raw size `0x82a400`; cache RVA `0x3d79718`). This establishes image-load initial 0, without establishing later invocation-time state.

Retained `wave3-editor-startup-language-setting-1403b5520.asm` reads the cache. A negative value calls `0x1403b54f0`, which returns 0, then stores it through `0x1403b5500`; a nonnegative value returns unchanged. The leaf setter at `0x1403b5508` writes its integer argument directly. This getter does not read a settings file in the captured body.

Retained preferences routine `wave3-editor-startup-language-install-chain-1403b5f50.asm` obtains a combo current index at `0x1403b64b2`, compares it with the getter at `0x1403b64bc`, and calls the setter at `0x1403b64cb` only if changed. That store alone is not a codec update. The bounded scan found the application constructor's one direct call to codec setup; it did not establish a later codec-setup invocation from this cache update. A later language index therefore cannot be translated into a current C-string codec pointer.

New `resume83-doe-codec-producer-candidates.json` confirms three direct RIP MOV-pattern candidates to the language cache: setup read `0x14000251a`, setter write `0x1403b5508`, getter read `0x1403b552c`. Existing captures confirm all three instruction boundaries/semantics. Direct relative-call candidates to its setter are the getter and the preferences call above. No full absolute pointer literal to the language cache was found in the scanned file. These scans are useful bounded incoming evidence, not proof against indirect writes, alternative instruction encodings, external modules or plugins.

New `resume83-doe-pre-application-1407fc450.asm` captures the 510-byte unwind region of the entry's pre-application helper. It derives executable-directory/environment strings and calls CRT/environment helpers. No direct language-cache/codec setter is present in that captured region. Its nested callees were not closed here. QApplication construction, `0x1407ffc10`, callback registration and later event/plugin effects are also not closed by this lane; no claim that the initial globals survive them follows.

## Distinguishing the Qt globals

Retained `wave3-editor-translator-codec-observation-contract.json` already identifies locale codec RVA `0x2f4438`, translation codec RVA `0x2f4440`, C-string codec RVA `0x2f3e10`, application pointer RVA `0x2f4220` and application translator-list state. These are not interchangeable.

`wave3-editor-startup-codec-bindings-6706d460.asm` shows the C-string codec getter gated by feature helper `0x671500f0`, whose retained body returns true. The getter reads the same C-string global; it does not lazily select the locale codec. More directly, the resource's `fromAscii_helper` reads the global itself. The locale codec's known lazy-initialization behavior must not be imported into this C-string read, and the host ACP or original `GetACP` is not evidence for C-string codec nullness.

The old observation contract requested bounded raw reads of the three codec globals after startup. A focused search of retained probe/receipt/binding/observation artifacts did not identify a completed original C-string codec read tied to this DOE resource occurrence. A requested capture/specification is not such a receipt. No new runtime observation was attempted.

## Minimal actual owner and admission

Keep the existing CP82 owner rather than creating another independently mutable codec/ref profile. Extend it only after a real producer is assigned, with a provenance alternative that cannot be set through `setIsolatedCodecProfile`. An explicit external process context must own:

- One process/application generation and the exact QtCore module identity. Its C-string symbol is QtCore base+RVA `0x2f3e10`; its canonical resource data/ref owner is the same owner later used by aliases.
- A source-state epoch and explicit Unknown/Null/NonNull knowledge with production provenance. Unknown remains the initial state without source initialization/read coverage. NonNull stores actual codec owner/vtable identity but stays guarded before decoding.
- A Session binding borrowing that context, with actual live Project, ModelManager ownerSession, notifier and receiver/model generation. The Session must not create or reset a global codec simply because a Project was created or closed.

A sealed production read receipt must be issued at the actual `0x67074126` branch selection for a reached `0x140086d68` occurrence. It must identify the same process/application/module generation, actual receiver source occurrence and logical slot generation, reached source site, codec epoch and pointer knowledge. A raw-pointer field observation must also retain its loaded module base/read stage and unchanged source interval; a post-startup observation alone cannot be stretched across unqualified later writes to a future receiver invocation. Original physical addresses are evidence addresses; reconstructed data/slot identities remain their own typed generations.

For a reconstructed source producer, every covered setter updates the same canonical process field, and any unqualified interval invalidates it before a later read. A source-covered known Null read can then invoke the fresh-resource constructor and initialize that new allocation's ref to1. It needs no observed current count for a not-yet-created data allocation. A NonNull/Unknown read refuses at the decoder/state boundary; there is no Latin-1 fallback by policy or text equality. Once Null is read, later setter activity does not retroactively change the selected native branch.

The smallest useful shared binding is therefore an actual Session lease on this one process context with an **unavailable production read** guard. It must not make CP82's diagnostic state production-ready. A helper-only wrapper that returns another Unknown metadata token adds no native evidence and should not be presented as the next behavioral increment. Before enabling production construction, either close the actual source global/read producer or obtain a bounded occurrence-qualified original read through an independently authorized observation session. This report requests no new GUI/runtime session.

The current earlier integration guards remain material: actual ordered receiver/backend delivery is still incomplete, and a matched live backend currently stops before QIcon allocation in `src/doe/variable_notify.cpp`. A codec admission does not bypass Population, BackendWitness, IconAllocation, addFile, graphic transfer or public Type2 guards. A diagnostic fixed resource request cannot certify that the actual receiver reached the source read.

## Concrete next slice and file ownership

Root must retain shared ownership of `include/m2m/engine/conversion_session.hpp`, `src/engine/conversion_session.cpp`, eventual actual receiver implementation in `src/doe/variable_notify.cpp`, and `src/core/convert.cpp` if an external selected-process context is passed from the run owner. No C ABI change is needed. The run/process owner creates or receives one context; Session borrows it; root revokes its Project/source admission before Project storage release. Retained diagnostics may outlive that admission, but cannot execute through a destroyed Session. Close does not reset process-global codec state or replay QString cleanup.

After root source freeze/assignment, one DOE lane could own existing `include/m2m/doe_qstring_resource_owner.hpp`, `src/doe/qstring_resource_owner.cpp` and its focused test for production provenance/read receipt checks. Private record construction remains in the existing path owner; do not duplicate its counter. The missing producer must be supplied as a source-assigned interface, not a public callable bool/null setter. Root should coordinate that exact interface before assigning further source work.

Focused tests for that later slice should verify: isolated Null cannot satisfy actual source admission; missing process/read identity guards before allocation; foreign/revoked Session/Project/model/module generations refuse; startup-only receipt cannot satisfy a later unclosed occurrence; covered source Null read admits one fresh generation/ref1; NonNull preserves decoder guard; unknown writer invalidates later read; an already selected Null branch keeps its decision; Project Close revokes Session access without resetting process state. Existing CP82 tests already cover isolated codec timing and ref arithmetic; repeating them is not a substitute for these source-owner checks.

## Evidence receipt

- New candidate receipt: `resume83-doe-codec-producer-candidates.json`, SHA-256 `5143ffecfcd696742e45b7bf277cea55e7e8ff2c81b25c826a08264405154f83`.
- New bounded pre-application receipt: `resume83-doe-pre-application-static.json`, SHA-256 `4f39168f7678dc9b82705091356de884e56b700691b31b254a6303b67696bf39`; it records the original binary/capture hashes and DUMPBIN command.
- Startup application capture is retained by `wave3-editor-startup-language-install-chain-static.json`: `1400021c0` capture SHA-256 `ab76d617d3a958e49e1b6b38b69d2c2dc2a230e3a90772c807f50d60d6791531`.
- CP81's resource source report and raw captures remain the constructor/data ownership authority. CP82 source remains untouched and held for root validation/archive.
