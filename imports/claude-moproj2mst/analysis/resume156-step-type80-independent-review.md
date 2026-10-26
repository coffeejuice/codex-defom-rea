# CP156: type80 StepSize table readiness review

The frozen detailed contract is source-ready for a bounded **private** Fill/constructor/Apply extension. No missing native decision blocks its proposed ordinary1D var1/2 profile. Approval depends on the specified alias, destination-shape and successful-copy guards; replacing `constantStepCurve` with a permissive table test alone would be incorrect. This is a design/source-readiness review, not a review of an implemented patch or test result.

Only this report was written. No source/test/CMake edit, build/test, GUI/original execution or REA operation occurred. Scope remains the private typed-page lifecycle; public palette/Save guards remain.

## Exact admission and native meaning

Independently read retained Fill140ebb950, Apply140ebc1f0, ECC770/ECCAA0/ECC540 and backed-refresh ECB340, and disassembled89b450/89b4d0 from original DEF_GUI_MO_64 v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. The definition getter returns0 for ndim0; for nonconstant state var0=1/2 returns1/2. Setter1/2 only writes ndim1 and the matching var0; it neither reallocates nor evaluates samples. Thus the proposed1D var1/2 case survives the Fill callback unchanged. STPDEF3 skips that definition callback; it does not invalidate an otherwise admitted table.

Keep the legacy constant profile unchanged. The new table branch should require:

- type80, ndim1, ncomp1, var0=1 or2, positive n0<=4096 (explicit candidate cap);
- valid owned axis0 and values with bounded n0*8 extents, no inactive counts/axis pointers and no u68/borrowed-array marker;
- unchanged existing3D/KnownC, legal mode/STPDEF and finite scalar-control Fill guards.

Unused var1/2 and opaque scalar fields are copied, not interpreted; no new zero requirement is justified. Samples and the constant field need no finite, sorted, distinct-axis or interpolation constraint: this path copies them. Test qNaN/signed-zero bits to catch accidental formatting or evaluation. `StepSizePage.definitionRadio` remains STPDEF1/2, distinct from the native curve-definition combo's var1/2. No shared field or definition-dialog implementation is needed.

## Alias and copy guards are necessary

Current `def::model::dynary_copy` has no self-copy guard. It copies metadata, calls dynary_alloc (which frees/reallocates the destination), then copies source buffers. Object self-alias loses data; distinct objects with shared/overlapping payload can read freed storage. The existing const shared_ptr type alone cannot establish independent native storage.

Before the initial Apply mode/scalar store, reject object identity with live SC.dttype, source/destination payload sharing or overlap, and unsupported live destination ownership/shape. Check both source axis/value extents against both destination extents, as well as ordinary independent arrays within each admitted table. Use actual typed-model/deep-copy provenance; do not infer raw allocation lengths from unchecked counts, repair aliases by silently cloning, or seed arbitrary fake addresses in tests. Guard refusal must retain mode/scalars/table contents. A valid destination may be the old ordinary constant or ordinary table; successful Apply must be able to restore the remembered profile over a later live profile.

Fill must not publish a supported table page until its actual clone has required arrays and valid profile. Initial mode normalization occurs before its copy, so a copy-unavailable refusal must retain that reached model effect rather than claim rollback. At the final Apply copy, restore the selected live mode in native order, then check required destination arrays before reporting Completed. If allocation is unavailable, preserve reached scalar/mode/partial-copy state and return a candidate refusal; do not claim original OOM/EH behavior or fabricate an allocation failure hook. This final post-copy check is an implementation clarification of the contract's positive-copy completeness requirement.

## Constructor and ordered Apply

Current lifecycle already makes the needed independent DYNARY owners. Initial dispatch precedes temporary copies. The backed refresh deep-copies SC DTTYPE to the remembered temporary **before** StepSize refill and restores SC from the common backup afterward. Do not move that copy behind Fill. For loaded u58=0/STPDEF3, the first mode callback normalizes live u58=1 before backup; later restoration retains that normalization.

The final pre-refill StepSize copy needs an explicit completeness guard. On refusal, take the existing partial-fill path and still reach common SC backup restoration. An early return past restore is not acceptable. Do not broadly change unrelated environment/view-factor copy behavior under this assignment. Intermediate incomplete pages must not be presented as pageStateComplete. On successful construction, test values/bits and independent final page/live arrays, not preservation of original physical array addresses across backup restoration.

Apply order remains selected mode; time/displacement and opposite scalar clear; STPDEF; strain/penetration/substeps; selected maximum-change path; sliding error; only then remembered-curve copy and selected-mode restore. Invalid at any scalar returns with its prior stores and **without** table copy. The remembered table's u58 cannot overwrite the selected reached mode. No visibility/widget-enable, integer-sample, curve-dialog or interpolation dependency should be added. Existing later-page ValidationStopped continuation remains.

## File split and decisive tests

The proposed existing simcontrol300.cpp + narrowly scoped lifecycle.cpp + NEW test_simcontrol300_stepsize_table.cpp ownership is coherent. Root retains any shared comment/header changes and build registration. No API/C ABI or new page field is required. Target is `m2m_mdt_simcontrol300_stepsize_table` / `m2m.mdt.simcontrol300_stepsize_table`.

Require direct Fill var1/2 across modes/STPDEF, independent allocated samples and raw-bit preservation; direct Apply remembered-data restoration with selected-mode survival; early/late Invalid retaining live table pointer/data; and negative shape/count/variable/alias profiles before effects. Alias fixtures must restore real ownership before cleanup. Keep all old constant tests, including their malformed ndim1/no-storage refusal, unchanged.

The key lifecycle positive test must use the actual private Project/Operation/GuiModel/StagedEditor300 binding and its ordinary explicit feature observations, reach initial/temporary/backed-refresh/restore/pageStateComplete phases, mutate live table samples after Open, then call CP155's owner-bound private page Apply. Verify remembered samples are restored at StepSize's successful tail, later pages/recovery order remains, and completeness stays false for public Save. A direct helper-only success would not close the constructor path. The initial/final copies and failure cleanup deserve source inspection even when real allocator-failure injection is intentionally excluded.

Current StagedEditor300 private construction fixtures do not establish a native QWidget constructor or actual Session Save delivery. Keep6504abb0/140eb5180 and `completeForPublicSave=false`. This extension changes the admitted private table lifecycle, not those public boundaries.

## Evidence binding

Detailed contract `resume156-editor-stepsize-table-next.md` SHA-256: `ecb28b90422c8a3a5865208c1aa57cf03b399e45638a9b90e49a513961853e43`. All nine published PE hashes were independently recomputed and match: Fill, Apply, mode/definition callbacks, getter/setter, backed refresh and GUI deep copy. In particular getter89b4d0..89b528=`36724836d304046460c3a0cc41057524f6f3802a65a3cedcc4dc05681c240e70`; setter89b450..89b4c1=`d0d6036532cb76c8539f643c6548382347ebf7324718ccc5218ab2231e199811`; backed-refreshECB340..ECB6e6=`e2c4d4336d87e8059f8446c46c2f46ff0502fd4d067491a58e3520dac19643f3`.

Read-time source SHA-256: simcontrol300.cpp `bd41db8ef7c06523b7c4f97f99bfbabbb939a35ebf78e03e4a9cc56f682f9eb2`; lifecycle.cpp `10299266be63d0fc85c0474776492bc30946b8f5fa52d541a00ea4f6341c49dc`; defmodel/window/inter_object_ops.cpp `7743de56231c2c0626fc78a2f7dc824a34606fa1563a919b003ccb641ba7721b`. CP154's original alternative is refined by the detailed contract above. No reconstructed executable/deployment was inspected.
