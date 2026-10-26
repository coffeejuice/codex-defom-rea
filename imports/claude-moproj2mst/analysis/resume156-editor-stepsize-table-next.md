# CP156: owner-preserving type80 StepSize table lifecycle

The next source-ready increment is normal owned1D, one-component DTTYPE tables through the existing private constructor/refill/Apply composition. Current fillStepSize3D/applyStepSize3D still require constantStepCurve. The actual curve owner, deep-copy library, backed refresh and ordered final copy already exist, so this can enable a meaningful complete private page lifecycle without a definition dialog, new native field or numeric-control rewrite. No source change is authorized/performed by this report.

## Native state and callback closure

Original DEF_GUI_MO_64 v11.0.2 SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. The following is tied to complete retained Fill140ebb950, Apply140ebc1f0, callbacks140ecc770/eccaa0/ecc540 and refresh140ecb340, with additional bounded DUMPBIN/PE reads.

Fill gets DTTYPE.u58 through8da6d0 atEBBA34; STPDEF3 selects mode radio2. CallbackECC770 atEBBB9A normalizes live u58 to0 for radio0 or1 for radio1/2. When STPDEF!=3, it gets the curve definition through89b4d0 atEBBBB2 and passes it toECCAA0 atEBBBC5. ECCAA0 selects the corresponding definition combo then callsECC540, which writes the temporary editor+e8 curve definition through89b450. These definition indices are distinct from StepSizePage.definitionRadio, which continues to represent STPDEF1/2.

The definition getter89b4d0 returns0 for ndim0; otherwise it maps var0=1/2 to definition1/2 and defaults other variables to0. Setter89b450 writes ndim0 for definition0; definition1 writes ndim1/var0=1; definition2 writes ndim1/var0=2. It does not allocate, evaluate or rewrite table data. Therefore the normal ndim1/var1-or2 profile is idempotent through this callback. Multidimensional/other-variable tables are not an equivalent extension: the setter can collapse ndim or select constant, retaining counts/pointers with different copy semantics. Keep those guards.

## Actual owner/order through constructor and Save

The owner is existing StepSizePage.curve, a const shared DYNARY allocated by the private Fill/lifecycle, independent of live SC.dttype. Original editor+e8 is a separate object. Constructor initial dispatch precedes the six temporary copies; later140ecb340 backs up actual SC, copies DTTYPE to editor+e8 BEFORE StepSize refill, fills subsequent pages, then restores SC from the backup. Current openInitialEditorInto already does this with its pre-refill stepCurve and actual deep-copy/restore binding.

Do not move the table copy after refill or merely test direct Fill. Initial mode normalization is a real pre-backup model effect: loaded u58=0 with STPDEF3 becomes1 during the initial mode callback. Backup restoration does not undo that earlier normalization. Full constructor restore can replace live backing arrays, so test final values/bits and independent live/page owners rather than assuming allocation addresses remain the original loaded addresses.

Apply keeps its existing scalar order and partial-return rule. It writes selected mode first; writes time/displacement and the mutually cleared scalar; sets STPDEF; then processes strain, penetration, substeps, selected maximum-change branch and sliding error in order. An Invalid field returns with earlier stores retained and WITHOUT copying the remembered curve. Only at the successful final tail does nativeEBD11A save current mode, EBD141 deep-copy editor+e8 into SC+1e8, and EBD153 restore that saved mode. Do not let the remembered table's u58 overwrite the newly reached mode, and do not copy data before a later validation stop. No table interpolation/evaluation or definition-dialog result is needed for this unchanged-table round trip.

## Exact candidate admission and alias guards

Retain the old constant profile and existing3D/KnownC, legal mode/STPDEF, finite scalar-control admission. Add a separate normal table profile:

- type0x80, ndim1, ncomp1, var0 either1 or2;
- positive n0 bounded by a stated resource cap (e.g.4096 points) and safe byte/count arithmetic;
- actual owned axis0 and values, no inactive counts/axis pointers, no u68/borrowed-axis marker;
- no requirement that raw table samples or the uninterpreted constant field are finite: these operations copy them and do not format/evaluate them. Preserve their bits.

Fill's copy must actually have its required arrays before returning a supported table page. Keep scalar formatting/validation as before. In the constructor's final pre-refill StepSize copy, check the required table copy is available before publishing it as the final page owner; if that admission fails, use the existing partial-fill path and still execute SC backup restoration. Do not early-return past backup cleanup or claim native OOM/EH parity. Initial copy/refill order remains unchanged on the admitted successful profile.

For table Apply, the remembered DYNARY must be distinct from SC.dttype and own independent axis/value storage. Require the current destination to have a supported ordinary owned constant/table shape as well. Check cross-array alias/overlap and bounded extents before the initial mode store: self-copy would free the source while copying. The actual normal Fill/lifecycle allocations satisfy this; shared or fabricated array aliases are not a native editor owner. Do not replace/rebind the supplied owner to repair an alias. Do not invent old/new physical array extent from padding or accept unrelated raw-pointer capability as proof of ownership. Actual typed model storage and the existing deep-copy producer are the admitted provenance; arbitrary external mutations remain outside that profile.

Source table u58 need not be reinterpreted as an input constraint at Apply: native saves/restores the reached live mode around the copy. Keep its ordinary Fill mode0/1 admission, but do not add a visibility, new combo field, integer sample or curve-dialog prerequisite. STPDEF1/2/3 can remain admitted for the bounded table because the callback is idempotent for1/2 and skipped for3.

## Focused implementation reservation

After root releases the current freeze, reserve:

- `source/apps/moproj2mst/src/engine/simcontrol300.cpp`: private table/profile/alias checks, new Fill/Apply admission and positive-copy completeness guard; unchanged scalar bodies.
- `source/apps/moproj2mst/src/engine/simcontrol300_lifecycle.cpp`: only the final StepSize pre-refill copy availability/owner guard, preserving the existing backup-restore path and order.
- NEW `source/apps/moproj2mst/tests/mdt/test_simcontrol300_stepsize_table.cpp`: one meaningful constructor→retained table→Save suite. Existing test glob supplies `m2m_mdt_simcontrol300_stepsize_table` / `m2m.mdt.simcontrol300_stepsize_table`.

Root may update the header's constant-only profile comments if needed; no field/signature/C ABI or CMake change is required. Preserve the old constant-only helper name if used as the old branch predicate; do not turn it into a misleading generic check.

## Required focused tests

1. Direct Fill for var1/2 across u58 modes and STPDEF1/2/3, using real dynary_alloc-backed type80 table with at least two points. Check mode normalization, scalar g6 text, exact table metadata/data and distinct live/page arrays. Include a payload quiet-NaN and signed-zero bits to detect accidental evaluation/formatting. Direct Fill leaves live arrays intact.
2. Actual Project/Operation/GuiModel/StagedEditor300 construction with ordinary existing feature observations. Verify initial fill, temporary copy, backed StepSize refresh, backup restore and pageStateComplete all reach their existing phases. Final live/page curves remain independent and contain the exact original table; the table profile must not be accepted only by an isolated helper while constructor still refuses it.
3. Change live table samples after Open, leaving the remembered page table unchanged; then call the existing owner-bound private applyInitialEditorPageState overload from CP155. Verify StepSize's remembered table is copied only at its successful tail, selected mode survives the copy, later pages/recovery tail retain their original order and caller completeness remains qualified.
4. Put Invalid at an early and a late StepSize scalar. Verify prior selected-mode/scalar/STPDEF writes survive but live destination table pointers/data and remembered table remain untouched; later private pages retain existing per-page ValidationStopped behavior.
5. Reject multidimensional, ncomp!=1, variable0/3, zero-count borrowed-marker, missing arrays/u68, out-of-cap and wrong-type tables before new effects. Include direct object self-alias and actual shared-array alias attempts restored safely for test cleanup; no raw invalid address or allocator fault injection.
6. Constant profiles and all old scalar tests remain unchanged. Backed refresh failure must still restore SC; actual allocation-failure injection/native EH need their own qualification and must not be fabricated for this test.

## Bounded evidence receipt

Direct PE hashes, end-exclusive:

| Native range | SHA-256 |
| --- | --- |
|140ebb950..140ebc1e2 Fill|`35bc6a14af33cda574aaf2e78a659c91ed1a582ea4cd7bb6a456040a69df50bc`|
|140ebc1f0..140ebd178 Apply|`8f1892ffe350b5044e64c04dfa76fde9ea053a1120f2ff88bdffa8f02307c733`|
|140ecc770..140ecca92 mode callback|`64ae7b2de24b8883c3e43782895fc00cba2ffdfeea78bc696f0fb54fb7b498a7`|
|140eccaa0..140eccb41 definition dispatch|`31222f2b5bb42f9b4a0cd85b066c526d8158658fbfaa6710ad55e2d47ff50a9e`|
|140ecc540..140ecc75d definition callback|`51359d74caa995160d99444582b73683b27f3958d65bd2f797096ea9d38a8759`|
|14089b450..14089b4c1 definition setter|`d0d6036532cb76c8539f643c6548382347ebf7324718ccc5218ab2231e199811`|
|14089b4d0..14089b528 definition getter|`36724836d304046460c3a0cc41057524f6f3802a65a3cedcc4dc05681c240e70`|
|140ecb340..140ecb6e6 backed refresh|`e2c4d4336d87e8059f8446c46c2f46ff0502fd4d067491a58e3520dac19643f3`|
|14089ad60..14089af17 existing GUI deep copy|`43651a213052f75cb536e53f21faafe653326ddd1a89765905303b91b1e4df78`|

Retained sources are wave2-save-diffs-300-fill-140ebb950.c, apply-140ebc1f0.c, save-140ebc1f0.asm and support-140ecc770/eccaa0/ecc540/ecb340.c, plus actual defmodel DYNARY/layout/copy/allocator source. No new raw capture file was created. The old CP47 constant-only contract supplies scalar/partial ordering but is deliberately narrower than this now-qualified idempotent table profile.

Read-time current source hashes: simcontrol300.cpp `bd41db8ef7c06523b7c4f97f99bfbabbb939a35ebf78e03e4a9cc56f682f9eb2`; lifecycle.cpp `10299266be63d0fc85c0474776492bc30946b8f5fa52d541a00ea4f6341c49dc`; simcontrol300.hpp `6cc6ac1d0109c542c345ced6a493ea22ee3306fb9c7ed4f96f12e16cd518dbf9`. Root owns their current freeze/build; this report does not acquire source ownership or claim runtime acceptance. Full Qt control/definition dialog, allocation/refcount/EH and outer lifecycle completion remain separate.
