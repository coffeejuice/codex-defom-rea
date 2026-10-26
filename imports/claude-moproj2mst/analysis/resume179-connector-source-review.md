# CP179 connector independent source review

## Verdict

**Accepted for build and executed validation; no blocking source defect found.** Released source implements only the retained private `+0x08` zero observation and phase advance before private `+0x10`. Source hashes match the implementation receipt. This is static review, not compilation, executed tests, freeze or original-comparison acceptance.

## Implementation check

The exact `BeforePrivateZero08` branch at `task_factory.cpp:433` rechecks retained Report/private/root qualification, canonical committed QVector with optional RVA, all six predecessor zeros, second canonical QList binding/control block and true marker, retained canonical caller provenance and unknown private08. The common qualifier supplies Report ordinal/pointee, allocation-time private identity, task/private sizes and written address points, store/root control block and pointee pairing, first QList/QString qualified committed bindings, and incomplete base/backlinks. Phase alone cannot bypass missing/wrong version evidence: digest, argument `0x40804`, load RVA `0x13140f` and call RVA `0x131417` are explicitly checked again.

All admission precedes the only writes: `private08 = uint64_t{0}` and `BeforePrivateZero10`. Both already observed zero and nonzero refuse through `has_value()`. No root reference event, observation rewrite, rebinding, allocation/cache/map change, backlink or completion occurs. The header supplies initially disengaged optional<uint64_t> private08 and no private10 observation. Terminal reentry stops at `0x13120f` without replay. This private08 zero is distinct from the later task receiver's private-pointer backlink.

Native evidence independently verified in this review sequence: QtCore digest `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`, raw `0x13060b`, bytes `48897b08`, SHA-256 `0ff59178935f4e42602274d216887da3888a7055509d5c9caff6c34bf579ed7e`. Retained disassembly establishes RBX private and RDI zero through the qualified equality branch, skipping the mismatch call. Next bytes `48897b10` are excluded. Static proof qualifies the selected non-null/equal path, not generic callers, live occurrence, mismatch-call semantics, cleanup or complete QObject construction.

## Authored tests and migration

Independently counted **59** retained corruption variants at `test_doe_task_factory.cpp:3567`. Coverage includes private08 observed zero/nonzero, missing/wrong caller provenance and version, missing/nonzero six predecessor zeros, owner/binding/expiry/control-block/marker faults, Report/private identity/size/address/written flag/phase errors and completion/backlinks. Snapshots preserve private08 and all earlier fields, caller evidence, weak ownership identities, markers, deltas/observations, private identity, sizes/address points and counts/cache state through refusal. Four owner-pair cases cover absence, foreign pairing and both aliasing directions. Replacement-private snapshot handling remains safe.

Actual ConversionSession coverage at line 3826 starts with canonical caller evidence, reconstructs CP178 by clearing private08 and rewinding its phase, supplies independent zero or `0xffffffff` histories and optional list/vector observations, then proves only private08/cursor publication. Same-key and two different-key calls preserve all other fields/bindings/provenance, three deltas and observation metadata, counts and incomplete/backlink flags. Separate sparse zero-history/no-absolute-count coverage confirms no invented count prerequisite. Fresh/type checks establish unknown optional<uint64_t>.

Ordinary stops use `0x13120f` / `BeforePrivateZero10`; zero old `RVA 0x13120b` terminal strings remain. The bounded audit of earlier explicit phase rewinds found private08 reset within their setup; CP178's phase-only equality replay now clears the new observation as required. Prior delta arithmetic remains unchanged. No private10, version mismatch-call effect or later task backlink is materialized.

## Exact SHA-256 receipts

| File | SHA-256 |
|---|---|
| `analysis/resume179-connector-next.md` | `10e55a3a08500371bb824c095928ff58db3a2f5917f30f14ba7a0c887efa6036` |
| `analysis/resume179-connector-plan-review.md` | `0a34d7fac9fab167ce127a21b9351bd4084a3bdc54e05d3e574bb8cf181f84c7` |
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `ea7b2db9ac979e51b6f196360fc98d5ec406d34c52b439ed3a886c44fa0534b0` |
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `0dda6382a161822eaedd510c08e91b2a7231bc3a31a83c6bfeee19489065128e` |
| `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` | `8ea82fda1aa489da24361d172e6b230fa362d35531d48ca0aeea8871b06ef6de` |

Only this report was written. No source/test edits, build, executed tests, GUI control, live REA, deployment or shared recovery-state changes occurred. Required executed gates remain with the parent.
