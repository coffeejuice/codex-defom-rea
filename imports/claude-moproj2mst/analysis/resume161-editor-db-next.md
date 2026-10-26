# CP161 Stopping companion owner and bounded directory query

The next source-ready increment after the frozen CP159 timer prefix is a **nonnull, owned companion directory query whose record version remains >=6000**, followed by the existing `CoordinateSync` refusal. It reads the actual directory record and can retain a real step-version update. It does not complete the loader or the coordinate getter. No additional timer implementation is needed.

This is a converter-only read-only investigation. Only this report was written. No source, test, fixture, build, GUI, live REA session, mutable analysis database, installed program or original project was changed or run. Native reads used the immutable retained **DEF_GUI_MO_64 v11.0.2**, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. This is the original GUI artifact. No reconstructed executable or deployment was inspected. Token measurement is unavailable.

## Checkpoint and evidence boundary

R300 in `analysis/takeover-state.json` remains partial/incomplete: CP159 is frozen and its successor is the DB companion directory owner/body/failure boundary. `analysis/resume159-root-integration.md`, `analysis/resume159-editor-cachemiss-evidence.md` and the repaired-source review bind the already completed timer work. CP159's complete suite passed at its frozen source; this investigation ran no executable tests. Its validation JSON currently hashes to `80d95fecd6b3e1a4e98e0459e018b61256754699297948d7d67252f4b5f8afa5`.

Read and reused qualifications: CP71's native loader/query, CP74's actual Open lifetime and physical-versus-logical extent audit, CP75's real allocation receipt, CP77's query-only retry constraint, and CP159's actual Stopping getter/reader binding. Viewer-specific barriers, frame retirement and positive geometry completion are not transferred to Stopping.

The PE and the following exact file-backed native slices were independently rehashed through the PE section mapping on this pass. Bounds are end-exclusive; subranges are instruction spans within the retained complete normal functions, not separate functions.

| Native interval | Meaning | Native-byte SHA-256 |
|---|---|---|
| `[140828060,1408283d5)` | complete normal loader through RET | `cb9d5ee4388c41ed115d75b236989b9f4cebf0bbe36ed3264d206508e70be796` |
| `[140825440,140825532)` | complete normal query through RET | `4525b4f95fe70043fd1c70d20acced398afb237fa0b8331a12d0da131cd00ac3` |
| `[140825400,140825419)` | complete version-setter leaf | `4fed3c566735c55f1ee0b69e7b5a69ec269243e8cb5aa16df3ca49d81c1beb07` |
| `[14082808b,1408280aa)` | actual companion read/null exit | `d41131ad72431779ef4b26114de96e3dc24aaa2b15490535b93c7f59145ddfa4` |
| `[140825467,14082548c)` | signed actual index, stride, directory, first body read | `90c4bbd4c362c8a71e19cab5cf722a249854722682ad96915493eec8949aad3e` |
| `[140825488,14082552b)` | four record reads then conditional setter | `f80d70e6069e95af73d85a30c7a1abc93dd1a482f70d1631b280d36e6efe0261` |

Decoded text is the existing `resume159-editor-cachemiss-140828060.asm` (text SHA-256 `74c9cfcba3b88ecbabba31c79b57b5efed63c25fb1e9a4594d49ffcf045c580f`) and `resume159-editor-cachemiss-prefix-140825440.asm` (`8e2dbca3f54429e674fb228dc096850b927d64e478e3e307102651615245afb5`). Both were rehashed and agree with their CP159 capture receipts. These are static normal-path statements; full EH funclets and native runtime parity are not established.

## Actual owner and first read

Let `S` be the same live `model.step` already matched to the actual selected main object's node-set owner. Preserve the CP158/159 canonical StateVar, real key0 COW, module-table/index, cache-zero and timer-prefix order. Only after that prefix consider the loader.

1. Loader `14082808b` reads **S+3390**, the actual `Cls_1401dbfe0*` companion. A null value takes `1408280a5 -> 1408283bd`, with no query/version/A/cache mutation. This is a loader return only; the reader still has its arrays checkpoint and assignment loop. Keep null outside this proposed query cut.
2. Nonnull loader calls `140825440(S,D,S.u740,&locals...)` at `14082814e`. The query checks D for null. Its explicit step argument is saved but not used to select a record.
3. `14082546c` sign-extends actual **S+740**; `140825473` multiplies it by **0x28**. `14082547c` loads **D+ad8**. The first record-body dereference is **140825488**, reading `directory + signed_index*0x28 + 8`. There is no native count, null-directory or physical-extent check before this read.
4. The query repeats actual S.u740/directory calculation for offsets **+8,+c,+10,+20**, writing hsize, MAR size, A size and version to caller locals. It compares current S version, then conditionally calls setter `140825400`, writing actual **S+1b64**, and returns AL=1. It does not fill S header lengths, allocate bodies or mark cache.
5. Only afterward does the loader re-run GUI predicate `14082bf10` at `140828167`. A record version below6000 can therefore change the next branch before the loader returns. Never roll this version write back or treat it as preflight of the old version.

The reusable source query is `def::dbio::dbread::step_dir_sizes` in `source/port/defdbio/src/dbread/w2_objarrays.cpp:506`, declared by `defdbio/dbread/w2.hpp`. Its four record offsets, ignored explicit step argument and conditional setter match the GUI bytes above. `DbDirRecord` in `defmodel/step/db_step.hpp` has the matching 0x28 stride and a static size assertion. Do not call the whole shared `read_single_a_array_from_file`; its names-up-to-date fold is not the GUI predicate.

## Stopping owner admission

The actual source ownership chain applies to this `GuiModel&` independently of viewer membership. `loader.cpp:newModel(Project&,dim)` constructs the model from `projectDb(p)`; Project owns `dataDb` by shared_ptr; `DbHandle` owns the unique companion reader. `ConversionSession` installs the live thread-local `ProjectScope` and removes it before deleting Project storage. The scope is a borrowed Project pointer, not a separate lifetime token. `GuiModel.db` alone is raw and cannot establish a lease.

Within that existing serialized live managed-Project scope, after CP159's prefix:

1. Read `currentProject()` and copy its existing `dataDb` to a local strong lease. If either is absent, retain `CoordinateSync`. Do not call `projectDb` here to create an owner.
2. Compare `lease.get()` with `model.db` **before dereferencing model.db**. Obtain `lease->reader.get()` through the strong owner; require it nonnull and exactly equal to actual `S.db`. No raw-pointer shared_ptr construction, foreign SubProject.ownDb fallback, reader substitution or copied step is admitted.
3. Request `directory_receipts::current(lease.get(), *reader)`. Require a ready receipt; actual owner, reader, allocation and both generations must remain current. Require actual signed `S.u740>=0` and only then convert to size_t and compare it with `receipt.elementCount`. Recheck `isCurrent` immediately before record admission/query. Hold the strong lease through all reads and the query; no callback, event processing, reload or owner replacement may occur between checks and reads.
4. This receipt observes the actual allocation operand at the two existing full-load/multipart `new[]` publications. It is not derived from logical uac8, stored count uacc, next-record uad0 or `loaded`. CP75 already covers failed/header-only reload, replacement, native reset and retirement. Do not add Stopping instrumentation, inject a test receipt or assume a nonnull directory proves extent.

These are adapter admission checks before a native read that has no such bounds checks. They preserve guarded unsupported states; they do not claim the original safely handles foreign or out-of-range pointers. Existing model parked-barrier checks remain authoritative before Stopping callback work. No viewer frame or barrier is installed or bypassed by this cut.

## Smallest normal/failure cut

After the owner/receipt/index checks, read only the safely bounded record's signed version as an admission preflight. If it is **<6000**, retain `CoordinateSync` without query or version mutation. Otherwise initialize four local output ints as in the loader and invoke `step_dir_sizes(S,reader,S.u740,&hsize,&marSize,&aSize,&recordVersion)`, then return `CoordinateSync` immediately. Preserve its actual version write, including same-version no-write behavior. Do not re-run the getter within the same invocation.

The >=6000 restriction matters: a query changing11000 to5999 followed by a plain failure would let a later fresh invocation take Stopping's existing low-version fast path, bypassing the unfinished native reader. The high-version restriction keeps retries at the guarded branch while admitting a real body read/version effect. This is CP77's query-only rationale applied to the independently matched Stopping model; current viewer source has larger barriers and does not provide Stopping's continuation by proximity.

Expected failures retain allocation/key0 COW/table/timer effects already committed by CP159, then refuse before body access for absent/foreign owner, reader mismatch, unavailable/stale receipt, negative or out-of-extent index. The lower-record-version refusal additionally performs the bounded record-version preflight but does not execute the query. Query normal return, including any version update, still ends in `CoordinateSync`; it is not getter success or loader completion.

Do not add header diagnostics, A allocation, zero-length cache1 publication, nonempty decode, post-loader arrays checkpoint, object assignment or Stop. The query's nonnull AL=true is source-closed. AL=false requires null D and is excluded by the preceding actual nonnull identity checks in this serialized cut; do not invent an extra nonnull native failure branch.

## Required focused implementation checks

Proposed exclusive implementation set is the existing `simcontrol300.cpp` and `test_simcontrol300_stopping_cached.cpp`; no source was reserved or edited by this investigation. Add direct declaring headers after resolving them against the target include paths. Existing target is `m2m_mdt_simcontrol300_stopping_cached`, CTest `m2m.mdt.simcontrol300_stopping_cached`. Use Testkit's Release-active macros and entry point, not C assert.

- Build a live Project/ProjectScope and actual projectDb-owned handle, load a disposable COG DB through the real loader under `M2M_TEST_SCRATCH`, and bind the tested model and S.db to that same owned reader. Obtain extent from the real allocation receipt. Directory edits may be controlled regression inputs, never fabricated allocation receipts or public-reachability evidence.
- Actual first/last physical records (0/31 for the existing32-entry COG case), record versions6000/11000/INT32_MAX and old S version6000/11000: assert the exact surviving version change, cache0, unchanged header lengths/MAR/A pointers and object coordinates, one reached getter/real dispatch, and final CoordinateSync. Reuse the existing real-COW observation to establish that dispatch occurs before table/timer/query effects. No test-only callback may replace canonical native dispatch.
- Repeat using fresh StoppingPage invocations as CP159 does, preserving production's changed-spin suppression. High-version queries stay refused and the second call uses a fresh current receipt; do not force a callback for an unchanged completed page or label a second call a resumed native stack.
- Record versions5999/0/-1/INT32_MIN: retain the old S version across repeated fresh invocations, no cache/body/header changes, same refusal. This specifically prevents a version-changing preflight from exposing the low-version getter route.
- Missing scope/dataDb, foreign handle, opaque model.db, opaque S.db, expired actual owner, directly replaced unregistered reader and mismatched directory pointer must refuse before dereferencing opaque/dead identities. Missing directory, pending/not-ready and retired receipt also refuse.
- Signed index -1/INT32_MIN, index==physical extent and INT32_MAX refuse with version unchanged. Logical33/physical32 rejects32; logical1/physical32 still permits31. Header-only/failed reload preserves the actual old receipt; full/multipart replacement requires a fresh generation (including physical63 after the real64-entry prepend). Reuse receipt tests for producer internals; test Stopping's actual consumer binding.
- Preserve CP158 cached1/-1, foreign node owner, invalid table/index, noncanonical registry, parked model, no-node and CP159 timer controls. Null S.db remains at the existing post-prefix refusal. Assert no arrays-timer/Stop, A allocation, cache1, later coordinate text or public Session/Save admission on the new query path.

Root must run changed targets as one focused group before the full source-stable suite and freeze. Existing DB directory/query tests and viewer tests are supporting regression gates, not proof of Stopping runtime parity. No test/build was run in this evidence lane.

## Exact remaining witness

No new original same-run witness is required to implement this bounded, high-record-version query-only prefix: its native dataflow is complete and its source memory owner can be checked synchronously. **Public Open-to-Save reachability remains unproved.** CP74's examined managed constructor/readDbEntry path sets S.u1b58=1; existing controlled flag0/cache0 fixtures do not prove that public Stopping reaches this branch.

For lower-version records, the missing implementation dependency is a **Stopping-owned durable pending-call barrier or a completely closed same-call reader tail**, tied to the actual model/step/node owner and preserving the committed version across unresolved assignment. A viewer-specific frame is insufficient. For public acceptance, retain one fresh original Open-to-300-InitialFill occurrence that ties the actual Stopping node getter's S to its S+3390 companion, that companion's live directory allocation and signed S+740 record, the selected cache0/header state, query version effect and subsequent reader exit/assignment. Historical owner receipts and separately captured native functions cannot establish that occurrence. Do not retry live capture under the current GUI/release restriction.

For further nonempty loader progress, `140b38d57`'s actual header/MAR/A owner and read/diagnostic/exception effects remain a separate frontier. The native normal-return cache1 write is unconditional after that callee; neither successful read nor full reader completion follows from it.

## Inspected source hashes

| Path | SHA-256 |
|---|---|
| `source/apps/moproj2mst/src/engine/simcontrol300.cpp` | `ef7d756a6b055d17a4abd0b0b1cb025e994b0b408e661192521b6de7f397c447` |
| `source/apps/moproj2mst/tests/mdt/test_simcontrol300_stopping_cached.cpp` | `568a4e64bcf3c910b69f58f550a3d804074967ad69b9691ea69b33184c34968e` |
| `source/apps/moproj2mst/src/engine/loader.cpp` | `746c6855de4831c15d8ad04f01df859d1e48a6ee5a1b16fd327c994d0fad9e7c` |
| `source/apps/moproj2mst/src/engine/db_load.cpp` | `82b2262da2c793d75ecf61af0cca9c59b77038752cfe836330a0ca7586d63920` |
| `source/apps/moproj2mst/src/engine/conversion_session.cpp` | `679f4c2195a4e664709ec946a67253c177e3b3957ef36bf253dce53178eef9a5` |
| `source/port/defmodel/include/defmodel/step/db_directory_receipt.hpp` | `24bf727760f8d21240c7da181662331fe867dbca2c4805eab9dc1b0b1f863c15` |
| `source/port/defdbio/src/dbread/w2_objarrays.cpp` | `c567ec0c3391507f438bfe66f5121aa25dab577f40592d5a84164d4133152359` |

This source audit qualifies a conditional bounded prefix. It makes no claim of positive cache-miss getter, public Save parity, timing equality, full exceptional-path recovery or objective completion.
