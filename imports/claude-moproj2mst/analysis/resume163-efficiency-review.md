# CP163 safe-boundary efficiency review

CP161–CP163 retained full source/build/test/freeze gates; CP163's measured accepted work totaled 110.675 seconds of build envelopes plus its selected complete launcher. This is recorded command work, not full checkpoint wall time. The most actionable improvement is to close the test contract and compile the exact newly added test targets before broad linking, then repair the concrete test defects before the accepted full build. Preserve every source binding, focused check, full regression and frozen candidate gate.

## Measured work

| Checkpoint | Recorded build envelopes | Selected complete launcher | Build plus launcher |
| --- | ---: | ---: | ---: |
| CP161 | 2 accepted envelopes: 15.226s and 50.817s (66.043s) | 30.094s | 96.137s |
| CP162 | 3 envelopes: 32.516s, 16.734s and 16.875s (66.125s) | 32.047s | 98.172s |
| CP163 | 3 envelopes: 11.140s, 50.130s and 15.623s (76.893s) | 33.782s | 110.675s |

The CP161 and CP162 values come from their build/test timing receipts and root integration records; CP163 values are in `resume163-grouped-build.json`, `resume163-repair-build.json`, `resume163-session-repair-build.json`, and `resume163-session-repair-tests.json` plus its referenced timing receipt. Build envelopes include fingerprint work and may contain reusable compiler outputs; do not add overlapping phase fields. The CP163 full launcher phases sum to 33.766s, with 17.016s in CTest; 11.687s in fixture hashing and 3.875s in source/binary binding. Launcher timing excludes timing-receipt writing and process exit. Checkpoint subtotals exclude untimed reviews, evidence collection, freeze/archive work and idle/parallel worker time; they are not end-to-end elapsed time or normalized throughput. Different code and action sets preclude a speedup conclusion.

## Retries and reviewer-preflight effect

CP161 and CP162 each had three build envelopes, including focused repair and/or source-stable rerun phases; CP163 also had three. CP163's first grouped envelope (11.140s) passed compilation but its focused test build failed. The broad repair envelope (50.130s) followed independent review and incorporated test/source repairs; the later source-stable accepted envelope was 15.623s. The reports identify a timer expectation and void-pointer indexing issue, a connector compile/private-owner test mismatch, and a stale DOE Session boundary expectation found by the first full run. The first broad build found the test compile error; the full run then found the stale expectation. These are real rework events, but the full 50.130s envelope is not an avoidable-time estimate because compilation could be reused and the repair also incorporated legitimate changes.

The CP163 plan/source reviews produced concrete constraints before integration (editor A allocation timing, FRCROT owner-prefix commit boundary, and connector QObject ownership/reentry boundary). They narrowed acceptable implementations and caught owner/test alignment issues; receipts do not isolate reviewer-preflight elapsed time or provide a counterfactual, so its time benefit is unquantified. Likewise, CP161–162 review benefit cannot be converted to saved seconds from the available receipts.

## Recommendation

For the next similarly bounded integration, have each plan/source reviewer enumerate the exact focused tests and boundary controls implied by the accepted contract. Before broad linking, run a Release compile of only those changed targets and correct compile/test-contract defects. Then retain the existing focused runtime, complete optimized launcher, raw diagnostic/fidelity checks and freeze. This directly targets CP163's observed early test compile issue and subsequent stale assertion without weakening coverage. Do not infer a fixed time saving; compare the next matched sequence and record its actual retries.

Token measurements are explicitly unavailable in the test receipts. Per-worker start/finish, occupancy, review duration, worker-hours and complete checkpoint wall time are also unavailable; do not estimate them from timestamps or document counts. No token, throughput or multi-agent savings claim is supported.

Evidence: `resume161-root-integration.md`, `resume162-root-integration.md`, `resume163-root-integration.md`, CP161–163 build/test/validation receipts, `resume160-efficiency-review.md`, and the referenced CP163 launcher timing receipt. This report is the only write; no source, tests, shared plan/state, build, GUI or live REA operation was performed.
