# CP104 — review of the proposed shared application-state owner

## Conclusion

CP103 identifies the right **state scope** but not an existing candidate production owner. Native DBLIST admission, current-main publication, and Close busy state belong to one application/main lifetime that outlives individual Project or Session activity. Candidate `run()` instead serializes process-wide engine work and creates a stack `ConversionSession` for each call. Its mutex is not an application-state owner; its Session/MainView/ModelManager are per-call adapters, and Session destruction explicitly is not native main teardown.

Some native state transitions are source-closed **after their actual source entry is reached**: the constructor prefix stores, main publication store, true busy setter stores, and the first two stores of the false busy setter. None has a current candidate application-construction or main-window producer that can establish it was reached. Applying these values at `run()` entry or `ConversionSession` construction would turn known native instruction effects into an invented lifecycle/default. There is no safe production callsite slice to wire now.

## Candidate lifetime mismatch

`core/convert.cpp::run` holds a static mutex because engine/Fortran globals are process-wide and conversions must serialize. It then constructs `ConversionSession` on the stack for the selected call, creates/loads the Project, performs Open/Save, and leaves scope. `ConversionSession` constructs its own Session `ModelManager` and `MainViewState`; its destructor releases those adapter resources and explicitly labels this path as not replaying original normal Close. No source-owned long-lived application-main object spans two `run()` calls, and `run()` has no normal-Close coordinator.

CP103's phrase “one runtime can correspond to one conversion invocation” is valid only as an explicitly selected fresh-application/one-project abstraction. It is not process lifetime in the candidate implementation and cannot also represent an already-running native application's later Open/Close state. Conversely, making a new C++ process-static owner would persist values across API calls but has no source-qualified initialization/clear lifecycle or app-instance identity. Neither scope is a safe implicit default.

## Fields that must stay distinct

- Native application-global `+0x0c` is process type. Its constructor initialization and mode-0 refresh stores are distinct ordered transitions; `5` suppresses only the DBLIST missing-file warning.
- Native `+0x1c` is DBLIST admission. Its zero predicate and `+1` setter body are known, but the setter's caller/`rcx` receiver and same-instance reset/writes are not. It cannot be initialized from `+0x0c`, new Session creation, or fresh-image zero-fill.
- Native current main is the application singleton's published qword pointer. Close's busy read also uses singleton `+9` and the current main's stack/page fields, including short-circuit `main+0x480` / `main+0x270` behavior. Those values can change through source-order setters, saves, Qt/tree callbacks, and modal dispatch; constructor values or a final Save write do not certify a later Close read.
- Candidate `ApplicationStateProjection` is not used by production code. It is a supplied-facts value projection for `+0x0c`, intentionally leaves `+0x1c` unavailable, and owns no main/window/manager/stack state. Treating it as the live app would preserve neither publication lifetime nor callback-sensitive state.

## Source-closed slices versus safe integration

The following leaves can be implemented against a correctly owned internal state object without inventing their local instruction effects, but only after a real same-owner producer reaches each entry:

1. Constructor initializer prefix: ordered stores to `+0x0c`, current-main null, `+0x18`, `+9`, `+0xA`, `+0xB`; the warning counter remains untouched. This is not a constructor-complete marker.
2. Main publication: after the actual prepublication/Qt effects, publish that same canonical window and then initialize its `+0x50` manager field in native order.
3. True busy setter: set `+9=1`, then its known `+0xA/+0xB` stores. Do not require main/counter knowledge earlier than native does. Its later tree-notifier arguments remain a separate boundary.
4. False busy setter: write `+9=0` and warning counter zero before reading main; if main is unavailable, preserve those partial stores and stop before changing `+0xA/+0xB`. A known null main and a nonnull current-main stack are distinct branches.

Creating and unit-testing these methods alone would be another detached helper, not public progress. Wiring them in `run()` or `ConversionSession` construction now would be unsupported: neither location is the native application-constructor entry or a bound current-main producer. A per-conversion owner could host the CP103 one-project profile only after the program explicitly establishes that each call represents a fresh app and supplies its startup phase; current source does neither. A process-static owner is also unsupported until a true app-instance lifecycle is selected.

## Exact blocker and next gate

The minimal missing candidate source is a real application/runtime root with a stable identity and lifetime, an actual primary-window/main+50 manager producer, and ordered Open/Save/Close callsites that borrow that same owner. It must reconcile API-call lifetime (one-shot fresh conversion versus multiple operations in one app) before choosing per-run or process lifetime. Then it needs source writers for the recovered busy/current-main fields and callback-sensitive readers. Separately, DBLIST still requires the original `0x1402cbf30` caller/receiver occurrence and complete enough same-global `+0x1c` write/reset evidence. CP101's Ghidra xref attempts timed out without evidence records; no new REA attempt is authorized here.

Until those gates are met, keep the projection detached, DBLIST/ReportList refused, and public normal Close guarded. The only safe standalone work remains tests of already-closed leaf contracts using explicitly supplied facts; those tests must not be described as live owner integration.

## Read-only evidence bindings

| Artifact | SHA-256 |
|---|---|
| Original `DEF_GUI_MO_64.exe` | `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` |
| `analysis/resume103-application-state-close-design.md` | `75decaa6e8004e99031830ac3a1b8818100ae7c033887ad77023da9b2317c3a7` |
| `analysis/resume101-close-live-owner-chain.md` | `8d893447af81cedd885b112609f7a7de709c1e2ab9b9f54a456746c4e2c7f793` |
| `analysis/resume101-close-pe-writer-chain.json` | `38ae40f0a8d56641940bfd3af1e2b31ab7ef43acb484f420a10b8f930534c9a1` |
| `source/apps/moproj2mst/src/doe/application_state_projection.hpp` | `25d236d51f278ab7e57e3773d8f6e12df68c57900879d150cf3deba7d9cc597c` |
| `source/apps/moproj2mst/src/core/convert.cpp` | `1fd1592c8bd8623e2d4736af077cdef4cd1f530227d87059721f4dbe205f6a30` |
| `source/apps/moproj2mst/src/engine/conversion_session.cpp` | `f1ff5d1c3934b1daa769f58c233b397a48c6eacb7cfdf790dba36edfea820604` |

Read-only source/evidence review. No REA database retry, GUI, original execution, source edit, build, or test was performed.
