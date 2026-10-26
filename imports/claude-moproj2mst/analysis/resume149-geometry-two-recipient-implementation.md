# CP149: two separately owned fresh main recipients

The initial high-version geometry reader now supports exactly two distinct fresh same-step n1/d3/first1 positive main recipients, each with its own real allocation/sizing/copy record. It retains the first completed copy when a second recipient refuses, keeps a third positive recipient guarded, and reaches the existing one Stop/exact retirement/cached getter only after every started recipient completes. This is the bounded private profile from `analysis/resume149-geometry-next.md` (SHA-256 `b525781a4c4c99f469b1e7e025e57675c7733f8b72806a347817f55a7bf24d58`). No public activation gate was changed.

## Actual state and ordered effects

Viewer now has fixed `GeometryMainRecipientProgress mainRecipients[2]` inside the original frame. Each entry records its actual object/ordinal, stage, three-double primary allocation, six BC attempt pointers/counts, initial registry body/28 receiver identities and dimensions, completed leaf count, and committed source/destination/24-byte copy receipt. The records observe actual storage; they do not own/free it or copy CowList bodies. No receipt vector/allocation was introduced.

The global reader stage moves from ArraysTimerCommitted to MainRecipientsInProgress once. Per-recipient reset/publication/BC/sizing/copy stages advance independently. Starting recipient two never rewinds the global arrays stage or overwrites recipient one's receipt. The same actual step A allocation and exact installed frame/current initial borrow remain required. Object identity and caller ordinal are distinct/strictly increasing; capacity2 is checked before third-positive effects.

Each recipient uses the existing real empty free/reset, diagnostic calloc3/publication, six ordered BC calloc/publications (including null returns), node n1, actual28 mutable lookups/virtual zero-size leaves, and fixed24-byte copy. Existing descriptor, canonical selectors, source extent/current maximum, typed destination, BC, registry/body/receiver/slot and dimension guards remain. The ordinary widths remain0. Wider rows, offsets, preexisting span provenance and nonempty variable allocation stay guarded. The previously loaded current-recipient MAR input is used; later iterations retain actual ordered MAR preload/bounds before skips.

`geometryCompletedMainRecipientsMatch` validates each prior receipt after callback-sensitive work, before the next allocation/leaf/copy and before Stop/retirement. It first recovers the actual current object with `step_object_at(currentStep, recordedOrdinal+1)` and compares identity; only then dereferences that object. It never dereferences a saved historical first-object pointer as lifetime authority. Active-recipient validation likewise recovers its actual current ordinal before reading its node state. This addresses `analysis/resume149-geometry-two-recipient-design-review.md` and the reviewer message. The current borrowed step/model/DB/receipt checks precede these lookups.

Prior primary/BC pointers, registry/body/receiver/binding/dimension receipts must still match. Prior bytes are historical copy effects and are not compared to the current mutable source row: source changes during a later declared test observation do not replay earlier copies. A later refusal preserves every reached actual allocation/publication/leaf/copy with the frame parked. Raw repair cannot resume or recopy on a later refresh.

After actual loop exhaustion, all started records must be CopyCommitted before the single node Stop. The high completer checks every record again and clears only this exact installed frame within its initial borrow. Zero-main high and low completers are preserved. Optional populated geometry uses the existing BuilderHighCached second getter; null connectivity retains its one first getter/edge behavior. Neither path reissues query/arrays/Stop or a third getter.

## Native qualification

No new REA or runtime evidence was acquired. The frozen plan reverified original EXE SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` and bounded intervals. Native node call `14082dbe9` returns through `dbee -> dc66 -> d9b4` to the next actual object; only loop exhaustion reaches `dc6b..dc81` Stop/AL1. Each iteration reloads A and independently executes already-qualified `140837080` reset/allocation, `140854200` coordinate/BC/zero-sizing and `1408422e5..842354` fixed-row copy. CP145–148 leaf qualifications are unchanged; nonempty resize failure/unwind is still outside this cut.

## Focused acceptance cases

The existing observation wrapper still forwards every actual mutable lookup and cannot replace the result. Its test-only receiver recording can now distinguish two actual node receivers. All injected source/table/owner/width mutations below are explicitly adversarial tests, not original runtime observations.

- `geometry_two_fresh_recipients_copy_independent_rows_and_complete_once`:8 profiles (versions6000/11000, populated/null connectivity, zero/mixed BC widths). Captures separate registry bodies and ordered receiver identities. Verifies56 actual lookups in per-recipient native order, two distinct actual primary allocations and independent positive BC buffers, unchanged empty values/null slots/seventh slots, each node n1/reset marker0, one arrays checkpoint/Stop, correct getter count/bounds and no cached producer replay. Seeds actual A with nonzero rowA then a different nonzero rowB at the respective final lookup, retaining destination sentinels; byte comparisons require first destination remain rowA while second/source are rowB.
- `geometry_second_recipient_partial_lookup_preserves_first_and_rechecks_live_table`:3 profiles interrupt second lookup5 by ordinary width, second owner, or replacing the first ordinal's actual table identity while leaving second ordinal3 valid. The linked-list mutation swaps the first with a real spare object and is restored through RAII before cleanup. First copy remains byte-exact, second retains its allocations/five completed leaves and sentinel bytes, node timer remains2, scene/frame stay guarded and raw repair cannot replay. This exercises the current-table identity check before any retained first-pointer dereference.
- `geometry_two_recipient_copies_remain_before_third_positive_or_later_MAR_guard`:2 profiles preserve both copied rows when a third fully qualified positive object or later malformed MAR load refuses. Third primary/reset marker remain untouched; no Stop, retirement or replay after raw repair.
- `geometry_second_recipient_fresh_admission_guards_preserve_first_copy`:3 profiles reject a second preexisting primary, first-node2 or foreign owner before its reset. First copy remains; second primary/count/reset marker are unchanged. Existing CP148 later-positive refusal still has logical n0 and remains intentionally guarded.

Existing CP148 successful single-recipient/nonzero-source assertions and earlier guarded BC/variable/allocation cohorts remain. No prior normal assertion was changed in this CP149 update; the new qualified second descriptor has its own tests.

## Static preflight and release

Static preflight passed: balanced delimiters, no trailing whitespace, resolved project/Testkit includes, valid CHECK/REQUIRE outer macro arity, unique21 cases,293 CHECK/107 REQUIRE calls, single Testkit main, matching internal helper call arities, no old frame-global main-progress field references. The retained-recipient validator declaration and definition use the same anonymous namespace. Source declarations for actual intrusive list links, node slots and lookup were checked. No compilation, build or test execution was performed in this lane.

Build target `m2m_mdt_geometry_a_producer`; exact focused CTest `m2m.mdt.geometry_a_producer`. Root owns changed-test binding preflight, focused/full regression and any broader Viewer target validation. Runtime results are pending that validation.

Frozen exclusive source:
- `source/apps/moproj2mst/src/engine/viewer.cpp` SHA-256 `ac7a0603adfa0c397441223d61a44aabdefd170c8023a658c13e589c0327d716`
- `source/apps/moproj2mst/tests/mdt/test_geometry_a_producer.cpp` SHA-256 `2f288b00a17567a7e1e4326583297ce43902f7126fb8ca72eb1bc6942c34e33b`
- This new implementation receipt.

Viewer/test are frozen and released to root. No shared header, CMake, CTest registration, original/evidence binary, GUI, mutable REA database or other source file was edited.
