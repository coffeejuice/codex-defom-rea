# CP102 actual Variable LinkTo import

The actual InputTable reader now imports deferred nonempty LinkTo relationships on its existing concrete Study-owned header/Description Variable profiles. Requests capture the reader's last record pointer, wait until the entire current DoeVariables container has reindexed, then bind in encounter order by exact target-name lookup. Missing targets preserve the existing pointer. This advances persistent model relationships without releasing numeric, payload or Save/refresh guards.

Root verified CP101 source/archive freeze and authorized four exclusive files after the evidence plan was corroborated. Frozen `resume102-doe-deferred-link-plan.md`, SHA-256 `5A71A4981309CCD875F90188D5A939C1C2E94AEE0675627F63FE206549AE24EF`, binds the complete actual native reader and new bounded setter/accessor/name-lookup/at-index/destructor captures. Its original hashes, callsites and capture qualifications remain controlling; this receipt does not replace them.

## Source transition and ownership

`include/m2m/doe_variable_record.hpp` adds an internal `DeferredVariableLink` with a borrowed concrete HeaderVariableRecord pointer and owned target String, plus the request-vector alias. The existing helper now requires that queue reference; its sole actual production callsite was updated. No C ABI/shared Session interface changed.

`src/doe/variable_record.cpp` removes the nonempty-LinkTo admission refusal while retaining all existing owner/count/mode, sample, optional-header, retained-prefix and mixed payload conditions. After actual ID/dimension stores and before Description it captures a nonempty LinkTo request whose source is the current reader-last object, independently of the duplicate-name helper's returned match. It does not assign a link at that point. An empty/absent LinkTo adds no request and does not clear an old link.

`src/doe/doe_study.cpp` constructs one local request vector inside each exact DoeVariables branch. A refusal/exception before normal row exhaustion unwinds that vector without resolving it, preserving prior persistent append/header/Description effects. On normal exhaustion it first calls existing reindex, then the new resolver, then sets reader status/continues later top-level children and outer ID finalization. The vector never survives on Study/Session or carries an unresolved target into another container/read.

The resolver uses the same coherent concrete owner/list order already admitted by each row. It skips a null source, chooses the first exact content-equal target name and stores the target's borrowed pointer into source.link. A missing target skips assignment, preserving previous link state. Requests are processed in original order; later successful assignments overwrite earlier ones, and later unresolved requests retain the most recent pointer. No chain flattening, cycle detection, recursive mutation, sample/name/selection/invalidation update or ownership transfer was introduced.

Native setter0x140100a60 performs only the raw +0x40 store; the full record destructor does not follow/delete that link. Candidate self/cycle links likewise add no owning edge: Study continues deleting concrete HeaderVariableRecord objects, and the request vector owns only String values. Existing `unlinkedVars` queries can observe the imported relationship. Geometry/output readiness is not inferred from that pointer.

The native parallel lists append source pointer before target QString. The host uses one scoped typed request pair and does not claim native QList/QString allocation/reference/failure parity. Existing host exception/lifetime behavior remains an adapter. Mixed payload pre-admission refusal is still a reconstruction boundary before the row's native partial effects; it is not represented as original parse rejection or notifier failure.

## Focused validation

New `tests/doe/test_variable_deferred_link.cpp` includes eight cases:

- Forward/backward links, cycles and self links use stable concrete owners with completed indexes.
- A duplicate name queues the last record as source, preserving creator sample counts, selection and invalid flags.
- Ordered requests overwrite on found targets; unresolved/case-mismatched/absent/empty targets retain the previous link.
- Queue lifetime is one DoeVariables invocation: a target created in a later container does not retroactively resolve an earlier request.
- A later unsupported row discards pending links only, keeping reached ID/Description and the unreindexed index phase while retaining old links.
- A later top-level sibling refusal retains completed reindex and resolved pointers.
- Foreign aliases, wrong mode, mixed Keyword/Geometry, malformed samples and fresh textual0 retain their earlier refusal boundaries.
- Actual internal Session/Project/Type2 SUB lazy read imports links on the same owned Study, continues LHS/FF/USR and current-PAR effects, then same-owner Save refuses nonempty-vars reconciliation before XML open with old DOM/link identities preserved and no notifications.

Static Testkit/include preflight passed for the new test. A direct trailing-whitespace/final-newline scan passed for all four exclusive files, including untracked source/test files. Declaration/definition/callsite search found only the updated actual reader callsite; fixture definitions precede test uses and main is outside the anonymous helper namespace. No worker compiler/build/CTest, GUI/native process or mutable analysis database was run. Root owns grouped executable validation and the obsolete old LinkTo guard fixture.

Target: `m2m_doe_variable_deferred_link`; CTest: `m2m.doe.variable_deferred_link`. Existing CMake globs discover the test; no shared build file was edited.

## Source freeze receipt

Hashes were sent to root and independent review immediately after static preflight. All files below are under `source/apps/moproj2mst` and remain held through root validation/archive.

| File | SHA-256 |
| --- | --- |
| src/doe/variable_record.cpp | 0A802CBBE0B46A80F01A3955CB48C7A346F20F2D9F1519E6D41DA2E6D0A78813 |
| include/m2m/doe_variable_record.hpp | 3D65444A19FEF6C05A32753E1D990EB673395D3ED04B9B86DBB9D20258DA45DC |
| src/doe/doe_study.cpp | C6EAA2581E23D4BCE83301190667D038F4BF2045DCA35BC7BC0EC7C0250E83EB |
| tests/doe/test_variable_deferred_link.cpp | C7250C0D37EE6FF4AB06F0E925DEA5C0873894D5420164B62DC4ECE4D85919BA |

Next public Save dependency remains nonempty-vars operation/dimension reconciliation before XML output, then separately the actual refresh receiver/widget/timer lifecycle. Keyword/Geometry payloads still need their actual constructor/append/delivery chain. QLocale initialization/provider/zero-digit state remains unrepresented and fresh textual0 stays guarded. Link admission does not imply any of those dependencies is complete.
