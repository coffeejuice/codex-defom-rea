# CP131: masked-invalid nonfinite meshing node-count target

The smallest independent source-closed continuation is a nonfinite TARGET node-count value under an explicitly masked SIMD invalid-operation profile. Unlike target interference, this field never enters the native absolute-value helper or QString formatting: native140ecf27f converts it directly with CVTTSD2SI, then passes the signed int to the nonwrapping spin control. With invalid exceptions masked, NaN or either infinity converts to INT_MIN and clamps to spin0. Preserve the raw temporary until a later old-form commit; keep the unmasked path unsupported.

Only this new report was written. No source/test/shared-header/CMake changes, build/test, GUI, original execution or mutable REA occurred. Original DEF_GUI_MO_64 v11.0.2 SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

## Exact source and architectural qualification

Native140ecf261 loads REQUESTED index;f269 multiplies by158;f278 loads the editor+f8 temporary array. At140ecf27f bytes `F2 0F 2C 94 08 C8 00 00 00` execute CVTTSD2SI EDX,[RAX+RCX+C8]. This is a32-bit destination, not a C++ undefined float-to-int cast or a64-bit conversion. Atf297 the caller loads UI+938, thenf29e calls its QSpinBox::setValue. No instruction writes the converted integer back into the temporary on this load path. Stored controller index follows atf2b3.

The already source-qualified plain QSpinBox profile is nonwrapping, minimum0, maximum1000000000. The explicit maximum setter's return is140ef64a1 (setup source line12275). Existing Qt numeric-profile evidence closes signed integer comparison/bounding and setValue's bounded numeric store. Consequently signed INT_MIN becomes0. Native scalar writeback later reads spin value as int and stores a double to OLD temporary+c8 at140ecef7f; only that later occurrence replaces the raw source double with positive zero.

The ISA condition is important. Intel documents that conversion of NaN/infinity/out-of-range input to integer produces the integer-indefinite value on the masked-invalid branch; an unmasked invalid operation invokes exception handling. IM is MXCSR bit7. These are architectural facts, not a selected application's runtime mask observation. See [Intel SDM, SIMD invalid-operation conditions and masked responses](https://cdrdv2-public.intel.com/868137/325462-089-sdm-vol-1-2abcd-3abcd-4.pdf), volume1 section11.5.2.1/table11-1, and [Intel instruction reference, CVTTSD2SI](https://cdrdv2-public.intel.com/671110/325383-sdm-vol-2abcd.pdf).

The source's raw conversion and the existing `integer(double)` adapter both use the SSE2 signed32 truncation instruction. For IM=1, its indefinite result0x80000000 is INT_MIN; applying the existing clamp0..1e9 gives0. No application startup default, selected native MXCSR value, ambient original handler or full post-Qt FP-status equivalence is established by the retained evidence. The public selected editor remains guarded.

## Current exclusion and minimal implementation contract

CP130 currently requires all four target values finite. That is still justified for its selected finite absolute-value/formatting profile, but is unnecessarily broad for this one direct node-count conversion branch.

After the old-form commit, retain finite checks for target.interference, target.sizeRatio and target.tolerance. Classify target.nodeCount by its uint64 representation, using exponent bits0x7ff0000000000000. Do not execute floating comparisons/isfinite on a signaling NaN just to decide admission. If the node bits are nonfinite, require the ACTUAL current thread MXCSR invalid-mask bit7 set before admitting the load. If it is clear, return the existing pending report at PreviousCommitted (or Preflight for initial load without an old form), before any target control or stored-index change. Read the hardware control state; do not install a default or change masks in production.

For an admitted target, reuse existing integer(target.nodeCount) and clampSpin. Do not return a hand-coded0 instead of the original instruction path, mutate raw target.nodeCount during load, serialize the spin prematurely, or change any other target/old-control guards. The hardware conversion naturally has its exception-status effect; do not clear flags or fabricate a no-FP-effect claim. Full native Qt child/event/FP continuation remains outside the private numeric projection.

The existing finite-overflow profile is not redesigned by this change. A broad new policy for every possible SIMD exception, original exception handling, arbitrary2D activation or other nonfinite fields would require separate qualification. This proposal covers the masked nonfinite target-node branch only.

Because Fill already invokes the same InitialLoad helper, a first object's nonfinite node count can be admitted under the same checked mask, displaying spin0 while preserving its raw temporary/model bits until Save. That is source-consistent with the native initial callback; no duplicate Fill-specific conversion is needed.

## Value/owner sequence that tests must preserve

The existing actual GuiModel, one MeshingPage.objects vector and supplied RemeshState remain the owners. No new global/module object or shared header field is needed. The node source belongs to the requested vector entry, and firstNodeCount belongs to the current spin projection.

- Activated0->1: commit OLD row0; load row1 node NaN/Inf to spin0; row1 temporary remains its raw NaN/Inf bits. Model remains unchanged until Apply.
- Save after a PROGRAMMATIC move0->1: commit old row0, load target row1 to spin0, then copy ALL temporary records. Row1's raw nonfinite node bits are still copied to the model on THIS Save; do not normalize them during load or double-convert the target. A later same-row callback commits spin0 to row1's temporary; a following copy then writes positive zero.
- InitialLoad on row0: no stale old controls; display0 while raw node bits remain. Save-currentIndex on that loaded row0 then commits positive zero.
- Unmasked unsupported target: keep previousCommitted and old record values; keep target controls/stored index unchanged. This is a reconstruction refusal before an unimplemented hardware-exception continuation, not an original successful return or repaired exception.

These are private explicit call sequences, not evidence that the full public Session reaches the callback or that a complete second native Save can reuse already released temporaries.

## Exclusive files and focused tests

After root's CP130 archive freeze and approval, request only existing `source/apps/moproj2mst/src/engine/simcontrol300.cpp`, existing `tests/mdt/test_simcontrol300_meshing_selection.cpp`, and a new implementation receipt. Root owns build/integration. No shared API, lifecycle, CMake, legacy const primitive or public loader/factory change is needed.

Tests should use an RAII MXCSR fixture that captures/restores its caller's COMPLETE entry control/status word. Mask invalid explicitly for positive cases; when testing the guard, clear pending exception-status bits before clearing IM, invoke only the guarded profile, and restore before assertions/framework formatting. Do not trigger an unmasked CVTT exception merely to test the guard. The production path must never copy this test-only environment mutation.

Focused cases:

1. Positive/negative infinity and distinct quiet/signaling NaN payload bits as a requested node count, with all other target fields finite. Under IM=1, activation completes/reloads spin0; source temporary bits remain exact, then same-row commit replaces them by positive-zero double. Verify both initial Fill and later target selection.
2. Programmatic move plus mutable Save preserves the newly loaded target's raw node bits in the first model copy; a later explicit current-row commit/Save writes0. This detects premature source mutation and duplicate target conversion.
3. With IM=0 and finite old controls, nonfinite target returns PreviousCommitted, retains old scalar edits and leaves target UI/stored index/model/global fields untouched. Confirm after restoring entry MXCSR. Initial first-target guard should return no Fill page without manufacturing a default mask.
4. Existing guards for nonfinite interference/ratio/tolerance and old controls, event index mismatch, negative-index no-op, same-row reload, legacy const separation and disabled remesh order continue unchanged. Retain existing finite2147483648 -> spin0 case as a regression, not a new generalized range policy.

No compiler, test or original instruction execution was performed by this investigation.

## Read-only receipt

- Native requested-node load/convert/spin/stored-index range `[140ecf261,140ecf2ba)` SHA-256 `79e20cb88c3f68216df58c775a5f4b5bbf2c7c917222f7d238ccadb7910917dd`.
- Native previous spin-to-double store range `[140ecef3e,140ecef88)` SHA-256 `712af12cff3d333a6b26e820a270547f53fa6c37d726de41d3145a7556da6254`.
- Full handler `[140eceb60,140ecf2cd)` SHA-256 remains `49b658b9bee06716baddd4df1e69db8d9641b558517df54c3d76bc594bf7c2a9`; instruction order/caller joins are pinned in CP129.
- `analysis/wave3-editor-qt-spin-numeric-profile-contract.json` SHA-256 `cfe8e40a8d6516dbafc88ec12fbb99cc7b66c0f43c9c782e587ee8b962a9ba1a` retains lower setter/bound/comparison evidence and its limitations.
- Read-time simcontrol300.cpp SHA-256 `a657daa78e1a7b9c3618e37422cf1d8e29a7d28f368fcdcbdae16bc166a9bccb`; meshing-selection test SHA-256 `6b7226249296e8e89b70bdcdf5a996dd58fe8ca58b7043241a9c95409e57e6be`.

The external Intel manuals were consulted read-only solely to qualify instruction behavior and mask bits. They add no original application observation. No source/raw capture outside this report was written and no reconstructed executable/deployment was inspected or produced.
