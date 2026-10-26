# CP147 integrated inert process codec owner

The actual conversion driver now owns one inert process codec holder outside Session/Project teardown and catch/details publication. Its full WRITE_SAVE Session borrows that holder; legacy internal and packet Sessions remain unbound. Default knowledge is Unknown with no module generation, C-string pointer or codec-object generation. No key/resource data construction, source setter/read admission, native cleanup or public output/status behavior was added.

Root assigned the implementation from `analysis/resume146-qstring-process-owner.md`. The independent `analysis/resume147-qstring-process-owner-review.md` was read before implementation; its pre-try allocation, borrowed lifetime, isolated-profile and cleanup constraints are incorporated below. No worker build/CTest, GUI, original process or mutable REA action occurred.

## Frozen write set

Only these six assigned/new files and this unique receipt were written. Paths are relative to `source/apps/moproj2mst`.

| File | SHA-256 |
| --- | --- |
| include/m2m/engine/process_codec_owner.hpp | 5c353b68088024b586e322bcaedd055fbc220777b015e578c012270afcdea680 |
| include/m2m/engine/conversion_session.hpp | bb11c4582745b706feb3f502a65940f96dae327c9e3783960d30ac618c154d93 |
| src/engine/conversion_session.cpp | 679f4c2195a4e664709ec946a67253c177e3b3957ef36bf253dce53178eef9a5 |
| src/core/convert.cpp | f3fbc077fc700df96782eef7e621eaf1cba5010712a9b245279b9642c1ec14ce |
| src/core/process_codec_session.hpp | 911b7aed180410758f69a8a6c28bfed04d93fee629aa15f57528d92c911c818f |
| tests/doe/test_process_codec_owner.cpp | 8110fbb74039a19dceee8616a26b02a8def7e7f3421de101c05315dd9d2772d6 |

The new canonical owner is header-only because its entire implementation is fixed value state with constexpr access; no extra empty .cpp or CMake change is needed. The new private core header is the actual conditional Session-construction seam. Existing isolated QString helper files were not changed. No unassigned CP145 context/notifier, shared Study, emitter, Save, C ABI or details header was edited. Final source was frozen to root after its review correction and before this receipt update; it remains held.

## Ownership and no-allocation default

`engine::ProcessCodecOwner` is noncopyable/nonmovable. Its default constructor is constexpr/noexcept and defaulted; its only member is a snapshot containing the existing codec-knowledge enum and optional scalar identities. It contains no pimpl, allocator, QString/QStringPathOwner/QStringResourceProcessScope instance, dynamic string, shared owner token or native object. Its destructor is trivial and performs no reset/revoke/native cleanup.

Static module hash and RVA2f3e10 identify the retained source symbol only. The default module-generation, pointer and codec-object-generation optionals are absent, so Unknown is not encoded as a zero/native-null pointer. Snapshot returns a value copy; changing that diagnostic copy cannot modify production knowledge. There is no production setter, source read or data-construction API in this increment. `sourceBoundary()` remains CodecKnowledgeUnavailable.

This reuses the existing `QStringCStringCodecKnowledge` type without constructing the allocating icon-specific scope. Placing the old QStringResourceProcessScope before try would have added potentially throwing make_unique/make_shared calls; this implementation avoids that entirely. Constant-evaluation assertions in the new test express the inert default as a compile-time value, alongside noexcept/trivial-destruction/noncopyability checks. They await root compilation; source preflight alone does not compile them.

## Actual driver/Session/context binding

`run` declares the owner after acquiring the existing engine lock, before the try and before the Session inside it. The holder therefore remains alive through Project release, Session exception/final cleanup, catch mapping and caller-owned ConversionDetails publication. Its default cannot introduce a new pre-try allocation-failure path. Result/details/C ABI signatures and output/effect ordering are unchanged.

The initial source review found that unconditional borrowing in run would also bind legacy internal Sessions, even with ctx.session null. Root requested correction before build. The final driver uses fresh stack optional storage at the original Session point inside try; `detail::emplaceProcessCodecSession` selects the borrowing constructor only for M2M_FLAG_WRITE_SAVE and otherwise uses the original unbound constructor. It adds no Session heap allocation/move or lifetime outside that try. The same private seam is tested for flags0, WRITE_MDT, STRICT, WRITE_SAVE and combined full flags; it is an internal implementation seam, not a native observable or public observation hook.

New `ConversionSession(driver, ProcessCodecOwner&)` delegates to the common existing constructor body with one borrowed pointer. Existing `ConversionSession(driver)` delegates with nullptr and remains unavailable; it creates no implicit per-Session process owner. The const `processCodecOwner()` accessor returns that explicit identity. Project release and Session destruction do not modify it or its external holder. All original Session construction/body/teardown effects stay on the same common path.

CP145's synchronous context already borrows the actual Session and validates it at the notifier encounter. The same new owner is reachable through that Session; no cloned codec facts, mutable global current-reader variable, Study back edge or extra context field is introduced. Context/Session lifetime preconditions remain synchronous: this raw borrowed holder is not a token usable after its owning run scope or Session dies.

Sharing is explicit. Multiple Sessions supplied one owner share that process-profile identity while retaining distinct Session notifiers/managers. Different owner objects represent separate profiles even if their default Unknown facts agree. The actual driver selects a fresh reconstructed-process profile per run; no host singleton silently merges independent conversions. A future multi-Session process driver can keep and pass the same owner. Normal Project storage release does not reset it.

## Focused tests and static verification

New target `m2m_doe_process_codec_owner`, CTest `m2m.doe.process_codec_owner`, contains seven cases:

1. Constant-evaluable/noexcept/trivially-destructible Unknown default and static symbol metadata; a mutated diagnostic copy does not become a producer.
2. Explicit same-owner sharing, separate-profile identities and unbound legacy Sessions, with the existing synchronous context resolving the same Session-borrowed owner.
3. The exact run-construction seam keeps legacy0/MDT/STRICT modes unbound and binds only WRITE_SAVE/full profiles, with stack in-place construction and unchanged Unknown facts.
4. Explicit adapter Project storage release and Session destruction leave the outer owner alive/unchanged; a later Session can borrow it again. This is not an assertion that original normal Close callbacks ran.
5. An outer owner encloses Session exception unwinding, catch and caller-owned details publication; no Session pointer is put into ConversionDetails.
6. Real convertDetailed empty-input handling retains M2M_E_ARG/no-project-path and the existing details/output surface after the new inert pre-try declaration.
7. An isolated Null resource profile cannot change a bound actual owner from Unknown. A real bound-Session Type2 lazy TLOC read still stops at key construction after flag90, with no key/map/attempt or further I/O effect. The isolated source-resource entry still refuses without publishing data.

`python admin/check_test_source_preflight.py source/apps/moproj2mst/tests/doe/test_process_codec_owner.cpp` returned `ok: true`, no issues after the final edit. Direct UTF-8/final-newline/trailing-whitespace checks passed on all six files. The actual run declaration was inspected before try, its conditional Session construction inside try, and details publication after catch. No compiler/executable/CTest was run by the worker; root's grouped Release compile and runtime tests remain authoritative.

## Guard and cleanup limits

Possession of this holder supplies no actual QtCore module load/generation, source codec pointer, codec registry/decoder liveness or QString data/ref cell. No initial image-null or historical observation is promoted into a late value. Isolated profile labels remain non-admitting. Existing key/refresh/icon, population/backend/callback, named-keyword re-fetch, reconciliation and public Save guards remain unchanged.

No QString deallocation, QTextCodec deletion, registry cleanup, CRT callback, field-null store or Project Close reset is synthesized by holder construction/destruction or Session borrowing. The next implementation still requires a qualified source producer/read and same-occurrence data publication. This release corrects the ownership location and access path only.
