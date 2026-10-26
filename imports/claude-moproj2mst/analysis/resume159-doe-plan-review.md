# CP159 independent DOE empty-operation plan review

## Disposition

The proposed empty-`KwOperations` scalar prefix is source-ready as a **separate, narrowly admitted candidate branch**, and it does not depend on the unresolved Regions directory resolver or the nonempty-operation notifier codec/receiver path. The original `InputTable` reader constructs an empty local list for a truly childless `KwOperations` node; the derived manager still runs its base keyword append, then its counted operation loop exits at count zero. Native continuation reaches the original-name Keyword re-fetch and the `NominalData` text setter.

There is one necessary implementation condition: the current candidate helper `readBoundedKeywordConstructorPrefix` calls `enterSetOprHasDoeNotifierPrefix` whenever `source->notifier` exists, using a hard-coded operation id 1. That is correct only for the admitted existing one-operation profile. For the new empty profile, the implementation must branch **before that call** and must not inspect or mutate notifier state. Merely decoding `operations = {}` into the current helper would still invoke the notifier and would diverge from the native zero-iteration path. With that control-flow change, there is no blocker to the bounded value-prefix integration; the notifier and Regions resolvers remain guarded on the paths that reach them.

## Independent native path check

Proposal identity rechecked: `analysis/resume159-doe-next.md`, SHA-256 `BEA5EAAEA22BFB5D3456DF982B802A2849DA57D8BD0CAC7AE9560EBF60F9827B`. Original image is `DEF_GUI_MO_64.exe`, SHA-256 `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`.

Bounded DUMPBIN inspection of the retained original supports the key branch claims in the proposal:

- At `0x1409105c7..0x1409105f1`, the reader obtains `KwOperations.firstChild`; the loop enters only if that node exists. A strict self-closing `<KwOperations/>` has no raw child and yields no operation id. The proposal correctly forbids accepting “zero element children” when raw comments/text children exist.
- At `0x14091090d` the reader invokes installed manager vtable `+0x118` (`0x1400d25a0`) with its actual operation list. The derived body calls the base append at `0x1400d26e2`, reads list size at `0x1400d26ef`, sets index zero, and at `0x1400d2715` jumps to `0x1400d299f` when the index is already at count. For count zero, the body at `0x1400d271b..0x1400d2995` is skipped, so no SetOprHasDoe argument/key/suppression or dispatch state is touched.
- The reader resumes through the original-name variable lookup and keyword-list accessors (`0x1409109c8` calls `0x14091a950`; `0x140910a1b` calls `0x14091aa50`). For the exact nonempty scalar string, it reaches nominal-text setter `0x140921980` at `0x140910aba`. The next native boundary is `0x140910abf`, before upper-text probing; guarding there preserves the supported prefix and avoids later reads.

The exact retained byte intervals and hashes are in the proposal’s Bounded identity receipt. The local source identities also match its recorded current versions: `keyword_reader_prefix.cpp` SHA-256 `561840E9D75E9D99E6F96122D656668E7602EE26322A1C24E9855FD4CC3C6481`; `keyword_constructor_prefix.cpp` `799F0B0FB17226091828F125810838BEA2A1F27E1F562FBFB5DF3D28E28D1C3A`; `variable_record.cpp` `829CFBCA780A0F95AD0313C75580998D8EBC31FAFB461B0CCE601A8B338F3ABF`.

## Candidate source review

Current `keyword_reader_prefix.cpp` admits only the exact operation-1 case through `retainedProfile`, constructs a `KeywordConstructorPrefixInput` with operations `{1}`, appends the fresh record, then calls the notifier path if source context exists. `appendFreshKeywordConstructorPrefix` already accepts an empty operation vector: it creates/appends the typed Keyword and unions only supplied operation IDs, so an empty vector leaves a preexisting `HeaderVariableRecord::ops` unchanged. It also commits the same canonical Item owner before later reader work.

A correct extension should keep the operation-1 profile and all its behavior unchanged; distinguish the new raw-childless/no-attribute empty profile explicitly; pass a true empty operation vector only for that profile; apply the recovered selector/type/nominal-text stores to the named owner; re-resolve the original variable after append as the native reader does; then throw at the `0x140910abf` boundary. In that branch, it must not call `enterSetOprHasDoeNotifierPrefix`, `dispatchSetOprHasDoePrefix`, touch notification flags/keys/maps, or claim parser success. The implementation should also update the existing comment that currently says all profiles stop before notifier, since an empty profile can now advance through the scalar setter first.

The proposal’s A,B,A distinction is sound: header/Description setters in the outer reader use its last-row target, while the Keyword path selects by the original row name. Tests should exercise both profiles through `readInputTableFromSource` in the actual `ConversionSession`/Type2 path, verify the empty branch's nominal text on the named owner, and verify that operation-1 retains the current notifier-entry stop and null nominal text. Any empty input fixture must be called synthetic/exploratory, not an original observation or held-out acceptance case.

## Dependency separation and remaining scope

The empty-list branch is a source-derived InputTable value path. It does not construct DoeOutput’s Regions `QDir`, call the unresolved QtCore file-engine resolver, enumerate/remove files, or claim an output Save result. CP156’s missing Regions return remains a separate guard for the DoeOutput Save callback. Similarly, the nonempty-operation branch retains CP146’s notifier codec/data owner and dispatch dependencies. The proposed empty branch skips those reached sites because native control flow skips the operation loop; it does not synthesize their return values or resolve them for operation-1.

The branch is not an original observed empty-operation case, not successful study parse/Open, and not active Open-to-Save parity. Existing public Save and later parser guards remain. No source/test/build/GUI/runtime/mutable REA state was changed or executed during this review.
