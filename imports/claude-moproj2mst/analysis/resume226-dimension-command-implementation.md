# CP226 dimension owner command implementation

`GuiModel::setDimension(int)` now owns the two application dimension setters.
It first calls `requireGeometrySyncAvailable()`, converts the copied integer to
`std::int32_t`, then invokes `step->set_dimension(&d)` through the existing
virtual interface. It has no equality shortcut, reset, GUI initialization,
viewer rebuild, dimension validation or null-step fallback. The supported-domain
precondition remains an existing native step; unparked null-step behavior remains
unchecked, and no defined crash outcome is claimed.

The loader hook delegates immediately to the command. DieStress replaces only
its actual-model setter callback, retaining scratch creation, actual reset,
guarded scratch dimension read, actual setter and `objIds.clear()` order. All
other aliases and callbacks in that function remain. The now-unused native step
layout and generic access headers were removed from `class_slots.cpp`; the
focused compile confirms that cleanup. The public loader hook signature and
indirect ShapeOptim/DOEOutput users are unchanged.

The command retains the existing native setter's base dimension write, contents
cleanup and initialization order. No new StepIdentity invalidation was added;
the declaration explicitly limits this staged migration. The unguarded scalar
`dimension()` query is unchanged. This follows
`analysis/resume226-dimension-owner-command-contract.md` and the CP225 freeze at
`build/wave3-checkpoint225-verified/receipt.json` (frozen source tree
`c381d3784b6932715a7afa8dc3bad9b1eea6315078f89ee8b7c439c36112f9dc`).

## Focused tests and preflight

The new `tests/mdt/test_gui_model_dimension.cpp` has two Testkit cases. Equal
dimension is tested for 2D and 3D, preserving the native step address, existing
object, seeded control, `state100` and `viewerMirrorPlanes`. Changed dimension
is tested in both directions, retaining the step address while clearing objects
and restoring the native control default. A separate ordinary native step
transition establishes the expected `NSTEP=1`, distinguishing this operation
from `GuiModel::reset()`'s GUI `NSTEP=100`. The changed test queries current
object absence and count without retaining/comparing a destroyed object pointer.
Both cases explicitly preserve the CP222 StepIdentity observation and explain
that it grants no storage access.

The existing viewer geometry barrier case now checks direct
`model.setDimension(2)` refusal next to the unchanged loader-hook refusal. Its
existing state-byte/effect comparisons and parked scalar reads remain. Existing
loader-hook success coverage remains. `test_l6_slots.cpp` is unchanged, including
the qualified no-DB DieStress case.

The read-only `admin/check_new_cpp_bindings.py` preflight passed for both test
files. The new test has a Testkit `main`, resolvable quoted owner header and
native/Testkit includes, supported `CHECK`/`CHECK_EQ`/`REQUIRE` macros, no
`REQUIRE_EQ` or C `assert`, and no file probes. `git diff --check` passed for the
assigned files. CMake's existing test glob discovered the new target and test;
no CMake edit was required.

## Validation receipts

Focused build command:

```text
python admin/build_candidate.py --label cp226-dimension-focused --target m2m_mdt_gui_model_dimension --target m2m_mdt_viewer_box --target m2m_mdt_l6_slots --target m2m_mdt_step_identity
```

Configure and build passed. `analysis/cp226-dimension-focused-build.json` records
26.631 seconds including configure, build and receipt work, with 3562 source
files and matching before/after source tree SHA-256:
`42289755be44995145e84f8a519fa8313891c681d211b2166a41a518e16c568f`.
Configure took 1.719 seconds and the grouped focused build took 22.235 seconds.
This is one observed build, not a comparative speed or token measurement.

Focused CTest command:

```text
ctest --test-dir build/candidate -R '^m2m\.mdt\.(gui_model_dimension|viewer_box|l6_slots|step_identity)$' --output-on-failure --verbose
```

All four tests passed in 0.76 seconds: direct dimension command 2 cases,
viewer box 56 cases, L6 slots 12 cases (including `diestress_without_db`) and
StepIdentity 5 cases. The full output is retained in
`build/logs/cp226-dimension-focused-ctest.log`, SHA-256
`1093e3077028e485260331c921ce4c4ff0c7aa6c326dea8b1e679cb9be0b810e`.
This focused CTest invocation is not the root's source/binary-bound full launcher
or a new frozen candidate. Root owns integration, independent review, the full
suite and freeze.

## Changed source ownership and limits

All source/test edits are confined to these assigned paths beneath
`source/apps/moproj2mst/`:

| File | SHA-256 at source-stable handoff |
|---|---|
| `include/m2m/engine.hpp` | `dd0bc1fcba2466a509f2f1d30c0466e7a032177d067fad974603dbb1a2c7cb63` |
| `src/engine/gui_model.cpp` | `e46c914b5bc112f51eb7b91faf28b93a67b3da6767d53b7e94307a00cc156822` |
| `src/engine/class_slots.cpp` | `ad4da028baeb17531dc33af75457cf727263df741f3c176741a79346c422e71a` |
| `src/engine/l6_slots.cpp` | `a08a559ab7e75f425df7cc71ba8275a5f7eafadf79ee9766be894cc826f4d906` |
| `tests/mdt/test_gui_model_dimension.cpp` | `1ba60c887bd33d67cedb4dedb002dc743b56f357db2dc083d1b0c4e4500b5ece` |
| `tests/mdt/test_viewer_box.cpp` | `fbce8dfac2aca2f28fd48d3672f2c3c21ee95af0a4f81713911478293d6c0a6d` |

The tests exercise reconstruction behavior; no fresh original comparison,
fault-injected native cleanup failure, null-step runtime test or derived tracing
fixture was added. Source inspection establishes the unchanged virtual call
expression and cleanup ordering. Live ObjectKey issuance, identity invalidation
coverage, native KEY/import/DB bypasses, retained viewer/interior aliases and
cache1 original-witness gates remain open. No original application execution,
GUI/REA session, deployment or shared recovery-state edit occurred in this lane.
Source ownership remains on hold for root's complete run and freeze.
