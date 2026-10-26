# CP148 independent review: fixed three-double geometry copy

## Conclusion

The source copy path is narrowly admitted and its ownership checks are consistent with the CP148 evidence: current initial frame, current step/DB/directory receipt, actual produced A allocation and extent, actual same-step main object, freshly produced three-double destination, and completed ordered receiver prefix are revalidated before the 24-byte `memcpy`. The receipt is written only after the copy. For the admitted profile, the helper continues through the same object loop, reaches the single Stop, and retires only the exact initial high frame; the populated path then takes its existing cached second getter.

I found one actionable **test discrimination gap**, not a source defect: the updated normal-copy test places 11/12/13 in the destination but leaves the calloc-produced source at 0/0/0, then expects 0/0/0 in the destination. That does not distinguish copying the actual source from zero-filling the destination. Seed distinct values in the actual produced A allocation at the final-sizer test observation and verify those values overwrite the destination sentinel. The implementation visibly uses `memcpy` from the recorded source, so this gap does not invalidate the source review, but the regression should prove that exact data-flow contract.

## Source contract checked

- `matchesInitialHighProducer` requires the installed frame identity, current model/step, high version, id11/flag1/which0/dimension3 profile, current A pointer equal to the frame's actual `producedA`, and source count equal to guarded `u2b24 + 2` with the overflow bound. It also binds the live DB reader and current directory receipt.
- `geometryMainDescriptorMatches` pins same-step ownership, positive object width, first-node1, record n1, node width3 equal to D0=3, global dimension3, and selectors. The mismatch fast path is not used for this equal-width branch.
- Main coordinates are published by the actual `calloc_doubles(3)` and recorded as three doubles. Six BC attempts and their exact owner/count state must complete. The 28-variable registry body, identities, order and effects are checked before and during dispatch; `MainVariablesSizedCommitted` requires all 28 reached leaves.
- `copyGeometryMainRow` rechecks the complete sizing state, unique copy receipt absence, current assignment ordinal and loaded MAR index bounds. At the fixed n1/d3/first1 profile source offset is zero and copy size is statically 24 bytes. It copies from `frame->producedA` to the actual `nodes->u008`; only afterward does it record source/destination/size/ordinal and advance stage. Distinct still-live `calloc` producers establish disjoint storage; no arbitrary span or overlap case is admitted.
- A later valid zero/skip object reloads MAR before the skip and continues the helper. A later malformed MAR or second positive recipient returns before Stop and does not roll back the first copied bytes. The normal loop reaches exactly one node Stop. High retirement requires the exact installed frame and, for a produced main object, the 28-variable and 24-byte copy receipts. Low retirement is separate and unchanged.
- After retirement, the existing readiness path performs the first predicate getter and, for populated geometry, the `BuilderHighCached` second getter. I found no third getter or replay of reader/array/Stop work introduced by this cut.

## Test review

The test source retains active assertions for the 28 sizing receivers, one copy and one Stop on normal completion, exact cached refresh behavior, second-positive refusal, later invalid-MAR refusal, retained primary bytes, no rollback/replay, and existing incompatible profiles. The test hooks used to mutate destination/MAR are explicit candidate-only instrumentation; they do not claim original runtime capture.

In `geometry_main_zero_sizing_copies_actual_source_once_into_owned_destination`, the hook at the final receiver writes a sentinel into `nodes->u008`, but the test never writes a discriminating value into `step.u1b78`. Since that source came from `calloc_doubles`, the assertion that both source and destination contain zeros is insufficient to prove the copy source. A focused update should set, for example, source `{31,32,33}` and destination `{11,12,13}` at that hook, then assert destination equals `{31,32,33}` after the run; the subsequent cached-refresh sentinel assertion can remain.

The later-guard test does prove committed destination bytes remain after a later refusal, but its source is likewise all zeros, so it currently proves retention of the copied-or-zeroed result, not source selection. Seeding the source in the same test hook would make that preservation check stronger as well.

## Reviewed frozen files

| File | SHA-256 |
|---|---|
| `source/apps/moproj2mst/src/engine/viewer.cpp` | `154245702A04FDB3C16E01F9EA8A6EAA9AEBE14DE841D141FC231B295A23EADA` |
| `source/apps/moproj2mst/tests/mdt/test_geometry_a_producer.cpp` | `6AE335A9294889DF38035E632593D5CDB17383A9185A1F64C096F59ACEBA8897` |
| `analysis/resume148-geometry-equal-copy-implementation.md` | `A318C4F6876B8D0C326F92E89B49833720CFBD25834F64B4402ADDA71787BF5D` |

This is a read-only source/test review. No build or tests were run; no GUI, original application, or REA state was used.
