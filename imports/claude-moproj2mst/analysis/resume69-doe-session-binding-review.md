# DOE Session binding review

The reviewed root integration preserves the intended CP68 boundary: one Session-owned admission holder reaches the actual Type2 XML continuation, retains prior XML effects and refuses before native refresh delivery. No blocking implementation mismatch was found in the reviewed snapshot. This is source review, not compilation, executable testing or original-runtime acceptance.

Only this report was written. No source, build, GUI, mutable REA or original process changed.

## Ownership and destruction

`engine/conversion_session.hpp:91` exposes the one `unique_ptr<DoeTableOwnerState>` declared at line191. In `conversion_session.cpp:148-168`, the actual notifier, WidgetRuntime and `ModelManager(this)` already exist before the holder constructor captures their weak identities. It does not capture Project, MainView or an invented QObject as its table owner.

Construction in the body is safe relative to declaration order: although the holder member is declared before MainView, both the real ModelManager and MainView initializers have completed before the body constructs the holder. There is no `InitializeFields` observation in production startup. Consequently the holder starts with both native fields **Unavailable**, no allocation generations, no receipts and all Qt/timer facts unavailable. The recovered native zero stores are not silently inferred from creating the C++ holder.

`releaseProjectAfterCloseEffects()` (`conversion_session.cpp:410-417`) calls storage release without releasing or resetting the table holder. `releaseProjectStorage()` resets Project/scope and its Mo-model backlink, not the holder, notifier, WidgetRuntime or ModelManager. Holder validation intentionally does not require a live Project. This preserves the native distinction between Project lifetime and main-window/table lifetime; it does not claim original Close effects have run.

At actual Session adapter destruction, Project storage is released first (`:211`); main-view adapter ownership is released at `:214`; table admission is released at `:215`; task adapter scopes follow. The holder's `releaseAdmissionScope()` only sets its adapter flag and cannot throw or invoke a native table destructor. All captured owner members still exist then. Subsequent C++ member destruction releases the holder before ModelManager, WidgetRuntime and notifier. The order introduces no use of an already destroyed captured owner. Constructor unwinding likewise destroys the holder before those prerequisite owner members.

This destructor sequence is explicitly adapter cleanup. It does not purport to replay native `0x140019790` deletion, `+0x458` clear, QObject child destruction or QTimer unregister. The getter returns a non-owning pointer; callers must keep the actual Session alive and cannot use that pointer after Session destruction. No escaped owning table/Qt lease was introduced.

## XML result and reached boundary

The reached production branch is `moproj/write.cpp:144-156` after `appendType2SaveNodePrefix` selects the empty-variable XML boundary:

1. Perform immediate InputTable XML I/O through `writeType2InputTableXmlPrefix`.
2. Require the same StudyIo journal's latest event to be a normal writer return.
3. Record the refresh boundary for that exact returned attempt and XML path.
4. Obtain the same actual Session's canonical holder and require its refresh delivery, which refuses.

There is no `if (xml.writerSucceeded)` gate. An ordinary false XML result therefore reaches the same boundary as true, as native wrapper `0x1400d3200` requires. The false-open branch records its error and normal false return (`type2_xml_prefix.cpp:79-84`); the opened branch closes, queries file error, records the corresponding result/error and normal return (`:89-102`). Stream failure is retained independently from QFile error-derived boolean success. Refusal after these returns preserves XML truncation/accepted bytes/close/error and sticky StudyIo error; it does not transform ordinary false into an early return that skips refresh.

`StudyIo::observeInputTableXmlWriterReturn` (`doe/io.cpp:432-451`) binds the actual file device, attempt ordinal and path. It refuses duplicates or a success/error contradiction. `observeInputTableRefreshBoundary` (`:453-466`) requires the latest exact normal return, path and ordinal and records the phase only once. Thus the writer cannot accidentally select an earlier successful attempt when the latest one failed, and unavailable file output is not misreported as an observed false result.

Exceptions before normal writer return do not reach the boundary marker. Constraints/serialization/device refusal retain their preceding effects and stop earlier. The marker also precedes the table-holder guard, so a default unavailable table field does not erase the already reached XML phase.

With an untouched production holder, the exact next guard is:

```text
DOE table owner: main-window +458 is unavailable
```

If a qualified field-initialization observation is later admitted, the holder still refuses with:

```text
DOE table owner: signalDoeTableRefresh() timer/UI receiver delivery remains unsupported
```

No holder state can return successfully from `requireRefreshDelivery`. Therefore routing cannot continue into Type2 PAR output, DBName completion, pending SUB replacement or final moproj streaming through this branch. Public nonempty B121/Type3 restrictions remain separate.

## Exact native meaning and remaining limit

The source reference is the verified native ordering in CP64-67: XML ordinary return, unconditional no-argument `2signalDoeTableRefresh()`, notifier dispatch/receiver invocation, timer stop, then UI refresh. `RefreshBoundaryReached` means **control reached the pre-dispatch message phase**. It does not mean the native dispatcher ran, suppression was tested, `flag90` was reset, receiver population was resolved, `slotUpdateUI` ran, the timer stopped or QWidget state changed. The current integration does none of those effects and resolves no current Project ahead of them.

The `write.cpp:145-147` comment describes the Session holder as the only receiver owner. Its safe meaning is **the candidate's one canonical admission authority**. It must not be used as evidence that native `+0x458` equals the complete live notifier population. Native table registration at `0x1400ebc5c` precedes constructor return/cache publication at `0x140019738`; partial or later lifecycle states cannot be collapsed into cache presence/absence. CP68 also deliberately retains unavailable native receiver liveness even after constructor/deletion observations.

This wording qualification is nonblocking because the implementation always refuses before dispatch. Before enabling delivery, root must bind actual ordered notifier population, suppression/invoke semantics, QObject/timer/UI state and the recovered lifecycle prerequisites. A cache token or the new boundary marker cannot justify bypassing any of them. The prior generic refresh guard has been replaced by a more specific ownership refusal; this is a bounded diagnostic improvement, not additional native refresh execution.

The new canonical-owner test statically checks field Unavailable and same holder across Project release (`tests/doe/test_table_owner_state.cpp:79`). Shared Type2 Save tests cover successful XML, open false and post-close-error false reaching the boundary and owner guard while preserving output/error records. XML helper tests cover exact device/path/ordinal binding and stale/duplicate rejection. These are declared adapter fixtures. The existing public ABI Type2-selection limitation remains; explicit Type2 Session tests do not establish that public ABI selection is recovered. Tests were read, not executed in this lane.

## Reviewed source hashes

These hashes identify this review snapshot; they are not a source/build freeze or test receipt.

| Source under `source/apps/moproj2mst` | SHA-256 |
| --- | --- |
| `include/m2m/engine/conversion_session.hpp` | `6b05d5d085ff23b42e7a3f1c784ea5e92576945504e471d1dda29d44abb17a99` |
| `src/engine/conversion_session.cpp` | `99731a89662bac4b9162c22f0bbb649da3e65c64a5431c96048060774b4ab8a3` |
| `src/moproj/write.cpp` | `06699e04e92ab7f256771aa3766aac8f10d06735da907ce967623eb0bef62986` |
| `include/m2m/doe_table_owner_state.hpp` | `e3521511037812a260a527647ba63e039938772856dfd7cf1022db9826797034` |
| `src/doe/table_owner_state.cpp` | `d57b8a6ba6f6dac0d7d9efb515b73a6dafbc10ba4213e5681c37744faf5fcfcf` |
| `src/doe/type2_xml_prefix.cpp` | `10bd0e9690f62387e394eb603d7f90619aa81304f9fd8127f7fa58c5f14d4d57` |
| `src/doe/io.cpp` | `5e803ae769b4e5b9a2bac20d2efc34ca74be5a7705128f6f37eccae7596d035d` |
| `include/m2m/doe_io.hpp` | `55e75ac6f3d5c3eaa483690bebc2740fa2789c47111151a5d7d900b843348279` |
| `tests/doe/test_table_owner_state.cpp` | `37b7d477381dcd95367571855b419ee41db4ac660a6395d9bc8a218eee66f018` |
| `tests/doe/test_type2_save_node_prefix.cpp` | `8a29a994c47d376a4f21a501bd9e5c6d47c968b2b95c2ae0ab185c161d1b39ca` |
| `tests/doe/test_type2_xml_prefix.cpp` | `547f1692a295cc1ecd4c6f164f82d7998c133d1330547e0f1791a3b8a1a5ec3f` |

Token measurements are unavailable.
