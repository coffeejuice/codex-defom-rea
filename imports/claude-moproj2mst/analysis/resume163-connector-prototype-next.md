# CP163 First Report prototype: private-allocation cut before shared state

The smallest reachable constructor increment is before TaskInfo: extend the actual pending prototype-one allocation with a resumable QObject/private-construction frame, execute the normal non-null private-allocation prefix, and stop **before QtCore `0x67131185`**, where shared QList identity is first consumed. This cut reaches no TaskInfo storage, cache entry, font, TLS selection or QObject process counter. It addresses the real predecessor identified by CP161 rather than applying a later class's metadata to an absent receiver.

## Current reachable source boundary

Read-only source inspection confirms `TaskFactoryRuntime::createTaskBase` calls `initializeNativePrefix` before requested-key lookup (`source/apps/moproj2mst/src/doe/task_factory.cpp:122`). `initializeNativePrefix:99` commits Factory/Data, retains one pending Report allocation through `TaskAllocationStore`, and throws before entering the QObject base. `PendingTaskPrototype` (`include/m2m/doe_task_runtime.hpp:22`) currently contains only kind, native size `0xc0`, and false `qobjectBaseCompleted`. Its stable `unique_ptr` node is allocated in `src/doe/task_runtime.cpp:37` and survives the refusal. There is no TaskInfo/PIMPL owner there.

`report_qobject_ledger.hpp` is expressly an inert observation ledger. Its successful `observeSharedRef` does not mutate a shared runtime owner or construct a task. `WidgetRuntimeState::commitQObjectConstruction` is a counter operation, not an implementation of the earlier private allocation, container references or thread selection. Neither API closes the currently reachable constructor by itself.

## Original byte order and new narrow cut

Immutable identities:

- `DEF_GUI_MO_64.exe`: SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, image base `0x140000000`.
- `QtCore4_64.dll`: SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`, image base `0x67000000`.

The new complete-unwind-region capture of `0x140137610` starts with `ecx=0xc0`, allocation `0x14146eabc`, the non-null constructor call `0x1407b89e0` at `0x140137644`, and registration `0x1407c8140` only after the constructor returns at `0x140137679`. A null allocation follows a distinct branch that supplies null to registration; no safe-success replacement is inferred for it.

Prototype one calls `0x14157ead0`, which calls TaskBase `0x141403210`. TaskBase passes null parent to public QObject through IAT `0x141e041c0` at `0x141403229`. Only after that call returns does it install its own address point at `0x14140323c` and invoke TaskInfo at `task+0x20` at `0x141403248`.

Public QObject `0x671313d0` performs these first writes/calls:

1. `0x671313f1..0x671313f8`: store QObject address point `QtCore+0x23cac8` to the same task object. This is the active address point during this base stage, not the eventual concrete Report address point.
2. `0x671313fb..0x67131400`: allocate `0x88` private bytes. On the normal non-null branch, pass version `0x40804` and that returned private pointer to private initializer `0x67131160` at `0x67131417`.
3. `0x67131173..0x67131182`: retain the private `this` and write initial private base address point `QtCore+0x1a9a30` to private offset zero.
4. **Proposed stop before `0x67131185`:** this instruction starts loading shared QList address `QtCore+0x2f0a50`; `0x6713118c` writes it to private `+0x18`; `0x67131190` performs the first atomic shared increment. Stopping before the load avoids introducing a factory-local substitute for the missing shared owner.

At that stop the outer public QObject has **not** published `task+8=private`: that occurs at `0x67131425` after private initialization returns. The reciprocal `private+8=task` occurs later at `0x67131431`. Neither backlink may be written early to make the frame look complete. TLS/QThreadData selection and the process counter lie still later. The private initializer's final address point `QtCore+0x1a9d10` is also later, at `0x6713119f`; it must not replace the earlier base address point in this partial frame.

## Source-ready contract for the normal non-null prefix

This is a bounded construction-frame implementation proposal, not a claim that current source already executes it:

- Keep the existing pending Report node and its identity. Add its actual owned optional private-allocation frame in that node, rather than constructing `SessionTask` or another task allocation. A native size descriptor alone is not the private owner; the frame must own the separately allocated private state with a stable identity.
- Represent the task's current base-stage address point and the private state's initial address point with qualified module/RVA identity. Do not turn preferred VAs into host pointers. The native allocation size descriptors remain `0xc0` and `0x88`; uninitialized native fields remain unavailable, with no blanket zero initialization.
- Commit exactly the displayed successful-path effects in order: task QObject address point, private allocation, private base address point. Retain the private object as an unpublished construction local. Set a resumable cursor immediately before QtCore RVA `0x131185`.
- Reentry through `createTaskBase` resumes that same frame and refuses at the cursor without another task/private allocation or address-point reset. A native constructor continuation must eventually return through the original TaskBase frame before any TaskInfo construction.
- At this boundary `qobjectBaseCompleted` remains false; native initialization/prototype completion remain false; both factory maps remain empty; the requested getter returns no fabricated pointer. No QObject counter delta, shared-reference delta or thread-reference delta has occurred.
- Adapter scope release may release its retained partial frame under the already explicit fresh-process adapter boundary. Do not label that cleanup as a recovered native QObject destructor or completed native failure/unwind behavior.

The normal non-null branch is explicitly qualified. Native allocation failure/null/unwind behavior remains outside this small increment; do not translate the captured null branch into invented transactional cleanup or successful construction. This proposal adds real partial allocation/state to the reachable owner, not an observation-only ledger or a preseeded map.

The focused integration checks would verify stable task/private identities across repeated refusals, the exact cursor and address-point phase, absent public/private backlinks, no completed QObject/TaskInfo, unchanged shared/thread/counter state and empty maps. These are ordering/ownership checks; there is no reason to rerun unrelated public DBLIST comparisons for this isolated prefix.

## Why this does not yet expose a TaskInfo initializer

The new TaskBase capture shows TaskInfo is its first member-construction call after public QObject returns. TaskInfo `0x14140e090` installs its own address point, allocates `0x80` PIMPL storage and calls `0x14140e910`; only after that initializer returns does it write `TaskInfo+8`. The private initializer constructs QStrings at PIMPL `+0/+8/+0x10/+0x18/+0x20` before the QFont constructor at `+0x30` (`0x14140e989`). Consequently even a future partial TaskInfo PIMPL allocation must remain unpublished until its initializer returns; manufacturing `TaskInfo+8` early would repeat the same ordering error.

That future call is conditional on the entire preceding QObject constructor. The present cut cannot skip four shared-container increments, the private known/unknown flag-bit update, public/private backlinks, selected QThreadData reference, or process-counter increment to reach it. The existing isolated font helpers likewise do not create the actual TaskInfo owner or perform the native current-font query.

The immediate successor is root-owned shared Qt empty-container state on the same existing runtime, followed by the same pending frame's once-only private-reference effects. The next non-static witness is the actual thread/TLS selection at the later `currentThreadData` call, including missing-index/null-value branch identity if encountered. If advancing all the way to TaskInfo is requested in one increment, the precise remaining evidence is one qualified same-thread constructor chain binding the pending task/private identities, four shared-reference effects, TLS index/value and selected QThreadData/ref increment, process-counter ordering, then TaskInfo/PIMPL allocation and its call-time application-font values. Separate startup-font observations or a completed inert ledger are insufficient.

No new ReportGenerate key work is needed in this pass. No source/build, original runtime, GUI, live REA retry, mutable database or external project change occurred.

## Exact byte and file receipts

End addresses below are exclusive. Raw byte hashes and hex are retained in `analysis/resume163-connector-prototype-bytes.json` (SHA-256 `b38ce1278e3ef032cb2f1a8e125c22c73357bc176dbc3e46f3a411e320f381c3`). Disassembly follows PE exception-directory unwind bounds using existing DUMPBIN 14.44.35228.0; an unwind region is not by itself proof of a complete logical function.

| Binary / byte span | Raw offset | SHA-256 |
|---|---|---|
| GUI `0x140137628..0x14013767e` | `0x136a28` | `deb5a171a1f54b8f2bc54c8de022bd14183258c5dcb99f2a40be44ed423bf816` |
| GUI `0x1407b89f2..0x1407b8a21` | `0x7b7df2` | `fe470583a00d18d5d45c5dc548844460ebe8b52dd4dc3d59cebc7b5b3cfd76e0` |
| GUI `0x141403222..0x14140324d` | `0x1402622` | `87e7643fda70005ddff8b75f1dadd7b026ddb3bf6d1b64184f36e1f778400ffd` |
| GUI `0x14140e0a2..0x14140e0fa` | `0x140d4a2` | `c2aa287e4eaf4c179388ecf4e2615edf913e633f28c996f106916da7864b314f` |
| GUI `0x14140e922..0x14140e98f` | `0x140dd22` | `cbee9e7cf73db16aaa8d3f7c622259b70e5c8950b7f671f385753b4ff2bbaa18` |
| QtCore `0x671313eb..0x6713141c` | `0x1307eb` | `9bfc25427006913f6efe6c2ca93cc3ade27c99d43e42629035f5a62a39691818` |
| QtCore `0x67131173..0x67131185` | `0x130573` | `985f2fac6ff864d0009dcf6988ddd23496811302efc72fa2410fbf978922dbfa` |
| QtCore `0x67131185..0x67131198` | `0x130585` | `110efb63fd2d3c3005f668334e43abf219e8382a25575708272be433a53b4772` |

| Capture/source receipt | SHA-256 |
|---|---|
| `analysis/resume163-connector-prototype-static.json` | `dc6c80d02009f09219da95d6e63d015250dfb3e3e3d901b144407b2adc204349` |
| `analysis/resume163-connector-qobject-static.json` | `9a5a9ae6622a56330ccbbe6dd949d2dc02753d55d7ed59cd5559d834f80b760e` |
| `analysis/resume161-connector-key-plan-review.md` | `580c48b5cfcdc8f603b0b16aee247a52cad1aa74de52895a6d51be0896464271` |
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `88a0a355496a69d515d59affb01e4543b8ac7206cd1764a8302e1d6130830d7d` |
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `3d59535b1771173dfdbdd2bef5ae6637867336544e3487a4d4b683dcab6274ae` |
| `source/apps/moproj2mst/src/doe/task_runtime.cpp` | `ded750f5fb1fd47e279d33283842daeb0271a598c11083cc8eff7c1fa2306762` |
| `source/apps/moproj2mst/include/m2m/engine/report_qobject_ledger.hpp` | `fe0eca40b09767e2673ab1a00d42d282ec8923e5d8d3d639b0e614ca054e9a83` |
| `source/apps/moproj2mst/include/m2m/engine/view_manager.hpp` | `0715b73412eaac42084d3830de6a667d4804777bc76c040137053ca9a4078eff` |

No executable was built or deployed by this converter-only evidence pass.
