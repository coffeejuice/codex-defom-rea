# CP145 independent notifier admission review

## Result

The CP145 change is source-consistent for the actual `parseSubProject` call path. It carries a borrowed `ConversionSession` and expected Project/SUB/Study/StudyIo identities synchronously into the reached TLOC constructor prefix, checks those identities before touching notifier state, then performs suppression before `flag90=false`. The unsuppressed path stops before key construction; the suppressed path preserves `flag90` and stops before named-keyword lookup. Neither path claims a receiver callback or a native return from one.

I found no blocker in the production call path. One boundary qualification should remain explicit: `readLazyStudy(..., StudyIo&, const InputTableNotifierContext*)` does not itself prove that its `StudyIo&` is the same object as `notifierContext->io`. `parseSubProject` currently passes `*p.doeIo` and builds the context from that same `p.doeIo`, so the only production caller I found satisfies the identity relation. If another caller is added, it should either reject a mismatched `io` before reading the table or put the actual `io` identity into the source context and validate it. The focused tests exercise stale context identities but not a mismatch between the `io` used for bytes and the context's `io`.

## Ordering and ownership checked

- `parseSubProject` passes `ctx.session` only when present and constructs context fields from its actual `p`, `sub`, owned Study and `p.doeIo` (`src/model/subproject.cpp:388-392`). It does not recover Session through Study/SUB pointers.
- `readLazyStudy` copies the borrowed context into the local `InputTableSource` after reading the input bytes. It invokes the actual source-aware parser synchronously; a notifier refusal unwinds before the later sample-table reads (`src/doe/activation.cpp:39-52`).
- The source context is explicitly documented as borrowed and synchronous (`src/doe/input_table_reader.hpp:20-36`). No Session pointer is stored on Study or I/O.
- The notifier entry validates the Session's live Project, SUB membership before dereferencing the saved SUB, Study ownership/backlinks, Project I/O, shared I/O owner identity, model-manager binding, and Session notifier identity before reading suppression (`src/doe/variable_notify.cpp:28-58`). The order is sound for stale SUB aliases: membership is established before `const auto& sub = *context.sub`.
- The TLOC path appends the retained constructor prefix first, then enters the actual notifier prefix with `(operationId=1, hasDoe=true)`. Suppressed return has no flag/key/map/list effects; unsuppressed clears the actual `flag90`, then throws at the unresolved key boundary. The source-less route keeps the earlier stop (`src/doe/keyword_reader_prefix.cpp:49-82`). This preserves reached constructor effects without implying callback delivery.

## Test review

`test_keyword_notifier_entry.cpp` has active cases for actual unsuppressed and suppressed parse paths, source-less compatibility, wrong Session, and nine identity/argument refusal profiles. Assertions cover constructor retention, the `A,B,A` selected-header versus named-owner distinction, sample state before later reads, notifier flag/key/attempt preservation, and no save output. Suppression and initial `flag90` values are clearly marked as test inputs, not native producer evidence. Tests do not assert receiver completion or broader parser success. No Release-inactive `assert` checks or zero-case loops are apparent; the identity loop has nine iterations.

The test does not prove the actual reader-I/O/context-I/O match independently. The production call site currently binds them to the same `p.doeIo`, so this is a coverage/contract edge rather than an observed production mismatch.

## Frozen source identities

SHA-256 at review:

| File | SHA-256 |
|---|---|
| `source/apps/moproj2mst/src/model/subproject.cpp` | `9D2EA605EBEA092019405ECB5CBFAAC81C2031AAD1ED034DA6F1FC722D1975D7` |
| `source/apps/moproj2mst/src/doe/activation.cpp` | `FFB76FCFB6C768BCF15AE77480AB781B5DCA0C9594D1F42A6822B6131EB850D1` |
| `source/apps/moproj2mst/src/doe/input_table_reader.hpp` | `886A610A055953E8ACF9F0F3BD45BD85E0D612DACF2A8FE5A6B50641595BDC75` |
| `source/apps/moproj2mst/src/doe/variable_notify.cpp` | `299B520C47576880B619EA3764154EE545B91518EC62F0529D829DF0524A807A` |
| `source/apps/moproj2mst/src/doe/keyword_reader_prefix.cpp` | `561840E9D75E9D99E6F96122D656668E7602EE26322A1C24E9855FD4CC3C6481` |
| `source/apps/moproj2mst/tests/doe/test_keyword_notifier_entry.cpp` | `5B71AF91FF414FA37B50823F3E105036049F3A825574BF69FEBD4A8AFE009192` |

This is a read-only source review. I did not build, run tests, use GUI control, or mutate REA state.
