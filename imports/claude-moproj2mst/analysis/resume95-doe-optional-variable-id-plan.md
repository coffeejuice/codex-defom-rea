# CP95 absent varId preserves the record until the outer ID pass

The smallest next source-qualified actual Variable branch is an **absent varId attribute**, while retaining all present-value/header/owner/payload bounds. Original reader tests presence before calling the ID setter. A new object's constructor-number0 therefore remains0 until the separate outer finalizer; an existing-name row leaves the reader's last object's current number untouched. This adds a real production default/timing branch without a new owner, callback, parser, allocation contract or shared interface.

This is read-only preparation while CP94 freeze is pending. Only this report changed; no source, compiler/build/CTest, new capture, original process, GUI or mutable database action occurred. Implementation requires root's CP94 frozen receipt/release.

## Independent original evidence

Original GUI PE SHA-256 remains `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. The complete0x14090ee20 reader is qualified by `resume50-doe-variable-body-pe-receipt.json`,15345 bytes/code SHA-256 `270b855af32d5c5f3fca905489895021d41d376f5e24f05d8941daf161e25f08`. Reread compact instruction stream SHA-256 is `1816C5454A6D81C4BADD706592CA854D7FABB548D9B6DC2AEB38B286EC6C5D02`.

At0x14090f0fd, the reader initializes local int0, constructs the varId key and invokes `QDomElement::hasAttribute` at0x14090f12b. If false,0x14090f150 jumps directly to0x14090f22a, bypassing attribute read, conversion and number setter0x14090f225. The local's initial0 is **not** an unconditional assignment to the variable. If present, native reads/converts and sets the number; CP95 can retain canonical positive int32-only admission for that present branch rather than adding generic QString numeric behavior.

New variable constructor0x140816900 writes number0; its create helper receives false assignment flag, so even though it computes a next-number candidate, it does not store one. CP91's complete constructor/create captures and CP93/94 actual typed owner close that provenance. For a duplicate name, no constructor runs and the helper's returned matched object is ignored; the reader selects its last object. With missing varId it skips the setter on that last object as well. Do not overwrite it with0 or an early generated ID.

After skipping the ID setter, normal dimension/LinkTo/Description handling, variable-loop reindex and later top-level child dispatch continue in their existing order. Only normal outer InputTable completion reaches0x14090ed14..0x14090edb3 to repair each nonpositive variable number. A later row/sibling refusal prevents that tail while retaining preceding record/description/index effects.

The finalizer is already source-qualified and implemented by CP84: `resume84-doe-operations-traversal-next.md`, SHA-256 `295CEE7F9F048650188B5521A8319CC6B26AB609327868D37A21945BFC7A698C`; implementation receipt SHA-256 `4396E6ABF6BD53F02A8851200479C6B32131D03E8E2425907AB11DD81395A9C0`. Its ordered scan is not lowest-free-ID or cached max: recompute signed candidate0 for each current nonpositive row, update on number>=candidate with32-bit wrapping add1, preserve source list order and current updates, and do not retry a newly assigned nonpositive result. Existing code must remain unchanged.

## Source-visible consequences and partial effects

An actual new missing-ID record has number0 throughout variable child processing and reindex. For example a later Constraint guard can leave it index0/number0, while a later matched variable guard before reindex leaves index-1/number0. It must not acquire ID1 at allocation or reindex.

On normal completion, numbers `[0,0,5]` become `[6,7,5]`; `[INT_MAX,0]` becomes `[INT_MAX,1]`; `[0,INT_MAX]` becomes `[INT_MIN,INT_MAX]`. Thus admitting a missing ID makes already implemented ordered/wrapping semantics reachable from actual imported records. Do not clamp negative results or add an extra normalization pass. Separate scalar/native Qt allocation/ref ownership remains unclaimed.

An A,B,A duplicate row without varId preserves B's current ID while permitting its bounded Description changes. The duplicate samples argument is still ignored on that native existing-name branch. If the last current number is nonpositive, only the same outer tail may later repair it; the skipped row setter does not cause that value by itself.

## Minimal source cut and exact guards

Remove varId from the mandatory attribute list. Read/parse it only when `hasAttribute(varId)` is true; reject a present unqualified value before current mutation, as now. Guard the actual reader store with presence/parsed optional rather than `value_or(0)` or a generated fallback. No new persistent optional-ID field is necessary because record.number already holds the reached state and the parsed optional is a local branch discriminator.

Keep name and numSamples mandatory, canonical positive samples, nonempty name, mandatory exact dimension3, mode0, empty LinkTo, Description-only/raw children, concrete alias/owner order/count/non-null checks and all scalar/geometry/retained/foreign guards. Present varId0, negative, empty, whitespace, leading-zero and overflow strings remain reconstruction refusals. Missing ID is not permission to accept arbitrary failed conversion. Existing duplicate-last-record behavior, selected state, invalidation and source partial writes remain unchanged.

The helper may throw during ordinary host values/text allocation, preserving reached state as before; do not synthesize native allocator faults or rollback. No later samples execute if the reader throws. Same-owner public Save still refuses nonempty-variable reconciliation before XML open, regardless of whether IDs are positive or an original wrapping result is negative.

## Exclusive files/tests after CP94 freeze

Proposed write set: `src/doe/variable_record.cpp`, profile comment in `include/m2m/doe_variable_record.hpp`, and new `tests/doe/test_variable_optional_id.cpp`. No Study/outer-reader/finalizer/Session/shared writer/CMake/C ABI change is needed. Existing guarded fixtures were inspected: their missing fields also omit name/samples/dimension, or their explicit bad varId remains present. No old assertion is expected to become obsolete.

Expected target `m2m_doe_variable_optional_id`, CTest `m2m.doe.variable_optional_id`. Focused tests should cover a missing ID on an actual fresh record; multiple missing/explicit IDs reaching exact `[6,7,5]`; both list-order wrap examples; number0 retained before same-list and later-sibling guards, with index-1 versus0; duplicate A,B,A without ID preserving B's ID and applying Description; present-empty/zero/noncanonical values remaining pre-append guarded; and actual Session-owned lazy import/sample continuation then unchanged Save reconciliation before XML. These checks distinguish omission from early assignment and from generic conversion.

This is a source-closed production default branch. General integer conversion, absent/default dimension/name/sample headers, deferred links, Keyword/Geometry, actual Qt ref/allocator parity, output and public Save/refresh remain separate dependencies.
