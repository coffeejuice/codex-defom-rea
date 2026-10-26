# CP63 actual-owner geometry getter fast path

The viewer now executes native getter/COW effects for a preallocated canonical
geometry node set whose existing owner is exactly the view's actual model step
and whose actual `u1b58` byte is nonzero. It writes no owner, synchronization flag,
dimension table or default coordinate array.

Exclusive implementation files are `source/apps/moproj2mst/src/engine/viewer.cpp`,
`source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp` and this report. No shared
model/CMake edits, build, CTest, GUI or mutable REA action was performed. Files
are frozen for root validation; token measurement is unavailable.

## Qualification and implementation

`analysis/resume63-geometry-owner.md` retains the original target/tool identities,
five bounded native captures, byte/capture hashes and native counterpart limits.
Original GUI coordinate getter `140854af0` obtains dimensions from N+130's
owner+15f8, invokes key0 allocation/collection dispatch, then calls
`14082dca0(owner,&which0)` when the owner exists. Native gate `14082bf10` reads
owner+1b58; any nonzero byte returns true and sync exits through `14082e9d8`.
Its zero-byte route can return false and is not justified by the port's
unconditional `names_up_to_date` counterpart. Missing-coordinate allocation and
owner-backed element synchronization remain outside this increment.

`qualifiedGeometryReadiness` replaces the ownerless-specific name. It first
requires preallocated coordinates, E+f0 null and exact canonical28 registry
IDs/types. For nonnull N+130 it resolves the existing guarded
`view.ownerModel.get()`, requires a live actual step, compares N+130 to that
actual `step.get()` identity, then reads only the actual step's `u1b58`. A
mismatching raw owner is never dereferenced. Nonzero admits the existing real
coordinate/connectivity getters; zero retains Unqualified. Ownerless CP62
behavior is unchanged.

The true readiness path still performs count/count/width and the second real
coordinate/connectivity getter round before the builder. The real coordinate
getter receives the actual model dimension table and preserves collection COW
effects. The existing missing-coordinate, E-owner and canonical-registry checks
remain before any newly admitted getter call; no arbitrary model sync is added.

The legacy populated raw-builder fallback for Unqualified profiles remains
unchanged, as explicitly reported to root before implementation. Thus the guard
assertions below mean no newly admitted native getter, not a new universal
unsupported result for every populated unqualified mesh. Null connectivity and
missing coordinates still refuse outside the proven readiness branches. Closing
that raw fallback is a separately scoped admission change, not an invented
native known-false predicate.

## Focused regression additions

Three new Testkit cases bring the file to17 unique cases:

- Actual-owner populated path explicitly binds a fixture to its own actual step,
  tests flag1 and flag0x80, and observes two n4/key0 dispatches with the actual
  step.u15f8 address. A shared list detaches on the first getter and stays stable
  on the second. A repeated unique refresh observes two further dispatches
  without another detach. Variable identities, IDs, current/previous dimensions,
  value bodies, source arrays, owner fields, flag byte and bounds are checked.
- Actual-owner null-connectivity path observes exactly one dispatch with the
  actual dimensions pointer, detaches its shared list once and retains edge-only
  bounds without changing source arrays, owner fields or flag.
- Rejected-profile matrix uses populated and null-connectivity fixtures for zero
  flag, absent coordinates, opaque E owner, a different valid model's step owner,
  and id0 registry. All observe zero getter dispatches and unchanged collection
  body/flag. Existing populated raw fallback results are asserted explicitly;
  null-connectivity/missing-coordinate cases remain unsupported.

The existing CP60/62 ownerless, custom/null/noncanonical registry, malformed
builder, shared/unique COW and populated-to-skip-to-populated cases remain. Hooks
forward to the actual dispatcher rather than supplying success and are released
before fixture cleanup. Existing owner-field RAII restores mesh-owned storage
before destruction; COW leases own list storage only, never StateVar pointees.

Static preflight verified the new DB_STEP_DATA include exists in the target's
defmodel include tree, the Testkit entry point remains, all17 case names are
unique, CHECK/REQUIRE assertions remain active in Release, no C assert or
unsupported REQUIRE_EQ was added, and both files have no trailing whitespace.
`git diff --check` returned0. No compilation or test execution is claimed.

Root target: **`m2m_mdt_viewer_box`**; CTest: **`m2m.mdt.viewer_box`**. Root owns
focused and complete validation. Synthetic owner bindings prove the internal
conditional contract only; no real-project owner writer/population receipt or
original runtime parity is claimed.

## Frozen source SHA-256

| File | SHA-256 |
|---|---|
| `source/apps/moproj2mst/src/engine/viewer.cpp` | `ed6cec89c659c52dc2e3de4fd99ffe4aefac6fbd2199d2f5e42dd98726be5f83` |
| `source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp` | `c06f25b6a0ec89a0b0f04f74c4db1a3a28ffdecd7b96ef1bb8f2d662cde4b720` |
