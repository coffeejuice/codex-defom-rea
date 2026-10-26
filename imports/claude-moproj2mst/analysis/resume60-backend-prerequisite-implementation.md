# Isolated backend prerequisite registry

The new registry supplies the bounded registration/removal driver for `2signalDiagModelCanAddItem()`, bound to the existing actual Session notifier and model generations. It retains allocation records separately from ordered map entries. Map presence/history starts unavailable; no production caller, SetOprHasDoe population, registry backend witness or public B121 path is enabled.

Only these assigned new files and this receipt were written:

- `source/apps/moproj2mst/include/m2m/doe/backend_prerequisite_registry.hpp`
- `source/apps/moproj2mst/src/doe/backend_prerequisite_registry.cpp`
- `source/apps/moproj2mst/tests/doe/test_backend_prerequisite_registry.cpp`

## API and binding

`BackendPrerequisiteRegistry(engine::ConversionSession&)` derives from the existing `BackendSetterPrerequisiteDriver`. Pass it to `BackendCommitPrefix::apply`. `bindInitialMap(BackendPrerequisiteMapProfile)` admits a one-time explicit actual same-key profile with key presence, ordered record identities and preexisting record metadata. Missing profile remains unavailable; a profile cannot replace state already admitted/changed. It may be admitted after a previous unknown-map refusal without retrying that original setter occurrence.

The registry checks the same live Session Project, its actual registry/notifier and the context's actual model/SUB membership. Imported receiver definitions must be exact registry members including shared control-block identity. Live imported receivers also require actual Project/SUB/FlowChart ownership; dead generations remain admissible as historical null guards. The Session must outlive calls. Saved Project pointers are comparison-only until a live Session check succeeds.

Record identities represent native pointer identity and are never dereferenced. Every newly represented registration gets a distinct stable identity. The retained store includes imported, orphaned duplicate and removed records; map entries are ordered identities, so repeated identities model aliases of one record. Native allocation layout, Qt weak-pointer storage, actual heap failure and native Close deletion are not claimed by the modern metadata wrapper. Adapter destruction releases its metadata only.

## Native order and exact guards

Controlling evidence: `analysis/resume60-backend-prerequisite-next.md`, SHA-256 `e48b77d06edd30f00ceac5c35b87264c6f1ba4d56d9da6ac776c1d2f38f6d387`. Source-qualified valid-data behavior is `0x1402cfbe0` registration and `0x1402cffd0` removal before the backend leaf store at `0x14014c404`.

The exact key/slot, flag 0 and null/non-null branch are adapter scope requirements checked before representing native entry effects. Other slots/keys, prepend flag 1, foreign/dead contexts and unqualified owner scope refuse. The selected valid literal's normalized bare method is `slotDiagModelCanAddItem`; this helper does not generalize malformed QString normalization.

Registration records allocation/constructor/guard/method state **before** map admission. With the default unavailable map, it then throws `M2M_E_UNSUPPORTED`: `DOE backend prerequisite: actual same-key map state is unavailable`. The new record survives this refusal. This is a distinct boundary from the older default driver's refusal before entering registration.

With an explicit map profile, registration takes the current ordered local list, finds the first matching live receiver/bare-method pair, and appends the new record only if absent. A duplicate still retains the newly allocated unused record. Writeback occurs even on duplicate; an absent key becomes present at writeback. Receiver guards that are dead do not match and are not pruned.

Removal with unavailable map refuses before operator[] and allocates no record. With qualified absent state, it inserts the present empty key **before** the local-copy/search stage. It selects the first receiver+method match, removes every alias of that exact record identity from the local list, preserves distinct equivalent records and survivor order, then writes back even on miss. Empty keys remain present and removed records remain in the retained store.

Neither path delivers a signal, changes suppression/flag90/unrelated keys, destroys a model or updates a backend itself. It returns to BackendCommitPrefix only after writeback. Existing post-prerequisite generation checks still guard the backend store. Profile admission and diagnostic identity-space/nested-call guards are explicit adapter boundaries rather than new native validation rules.

Diagnostic observers mark RecordReady, KeyInserted, LocalListReady and MapWritten stages. They exist to exercise partial effects and release during an active prerequisite; they do not model a native callback/event pump. Before writeback, loss of the actual generation refuses while retaining allocation/key insertion/local-list diagnostics. After writeback, a diagnostic release leaves that map effect committed; the outer backend prefix observes the dead generation and refuses its store. Exceptions preserve earlier effects and are propagated. No rollback is added.

## Tests and validation

Twelve new focused cases cover allocation before unavailable map, no removal allocation, later explicit profile admission, duplicate attachment allocating twice with one map entry, suppression/flag90 unchanged, absent-key insertion before local copy, pointer-alias versus distinct-record removal, other methods/receivers and survivor order, dead guards retained, foreign owner/control-block/profile replacement guards, invalid exact context/flag guards before allocation, local-list refusal preserving old backend and partial effects, and SetOprHasDoe Population remaining unavailable.

Three cases release the actual candidate Project while the prerequisite is active: after registration allocation, after removal key insertion, and after map writeback. They verify retained effects, dead model generations, refusal before the backend store and no resurrection through diagnostic leases. This invokes the storage-release boundary with no diagram binding; it is not native Close parity.

Static review confirmed current owner/witness interfaces, membership before imported receiver dereference, twelve cases/Testkit main, explicit complete OpClass/utility includes and no trailing whitespace. No build or executable tests ran in this lane. Root should build `m2m_doe_backend_prerequisite_registry` and run `m2m.doe.backend_prerequisite_registry`, retaining backend commit, flow receiver constructor/witness, notifier binding and scalar-prefix regressions. Existing CONFIGURE_DEPENDS globs require no CMake edit.

Actual complete same-key history remains unclosed, including the distinct Copy-constructor registration. The public nonempty DoeVariables guard, isolated scalar delivery guard, SetOprHasDoe Population guard, unavailable FlowReceiverWitness backend and icon/backend boundaries remain unchanged. No shared interfaces, CMake, public B121/Save, original evidence, GUI, mutable REA, build or deployment was changed. Token measurements are unavailable.

## Frozen source hashes

| File | SHA-256 |
| --- | --- |
| `include/m2m/doe/backend_prerequisite_registry.hpp` | `6dcf060acb3651837c36f0e5fff1aa7d8a1a18119ced71be30c8d3c967774f21` |
| `src/doe/backend_prerequisite_registry.cpp` | `1640fc875732c155f537fe7847de14e6765678dc22da86bc302c2a78e0ab0c1c` |
| `tests/doe/test_backend_prerequisite_registry.cpp` | `3f2f82ff7bfe24aae1a3de3333a68b3655a9142b3447c46b0c1f0c0f04d91105` |
