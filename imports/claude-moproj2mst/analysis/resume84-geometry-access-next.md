# CP84 private cogging router step-access migration

Select only the private `router` in `src/engine/cogging_save.cpp`. It forwards
one actual model step to a bool-result native call and fits the existing
`GuiModelAccess::withStep` policy without a helper/header/API change. The plan
was prepared read-only while CP83 was frozen. Root has since confirmed its
archive and authorized this bounded source successor after the plan freeze.

## Exclusive source scope

Edit only `source/apps/moproj2mst/src/engine/cogging_save.cpp::router` and this
report. Existing focused test files are reserved if needed, but no test change
is necessary for this forwarding-only slice. No new test API, private-function
export, shared header, native helper, CMake or ABI source change is proposed.

The file already includes `gui_model_access.hpp` following CP83. Preserve its
other raw-state consumers as explicit legacy paths. In particular, the step
size pointers in `cogPlaceBilletAndDies` and `cogPlaceManipulators` live across
many transformations and callbacks. Obtaining those pointers through a tiny
callback and retaining them afterward would escape the synchronous borrow.
Those larger paths are not part of this migration.

## Exact source and native-effect order

The private wrapper is marked `0x1408c0b50 Router = DEF_PRE 14019fa00` and calls
`def::cmd::objpos::fun_14019fa00` with twelve arguments in binary order:
actual step, mover, target, dir, tol, dist, G, flag, nc, list, tchg, one.
The declaration is `source/port/defcmd/include/defcmd/objpos_ifd.hpp:52`.
The qualified implementation at `source/port/defcmd/src/ifd/drv/router.cpp:164`
is marked `@src 14019fa00 spec 4.1` and performs:

1. Write `*tchg = 0`.
2. Probe `ifd::fun_14019ec30(m, a, nc, list)`.
3. False probe: directly return `fun_1401a0300` with the existing arguments,
   including `one`; do not set tchg to1.
4. True probe: write `*tchg = 1`, then return `fun_14019fb50`.

Keep the native call intact and execute it exactly once inside a bool-result
withStep callback. Only replace `w.step.get()` with the supplied nullable
`DB_STEP_DATA* nativeStep`. Preserve every pointer identity/alias, nullable
list, flag value, output buffer and bool result. Do not snapshot arrays, clear
outputs, normalize false results, roll back partial effects, or add a step
default. The native function owns all tchg/branch/output ordering.

withStep performs the actual model's parked-barrier preflight before acquiring
the borrow and before that first tchg write. The callback is synchronous and
neither saves nor returns the native step. Caller behavior remains unchanged:
some billet/die routes discard the bool, while manipulator routes sometimes
conditionally synchronize the viewer and sometimes ignore the bool. Prior
transforms and later repick/synchronization stay outside this small callback.
This is adapter barrier enforcement, not a new claim about native errors.

## Focused validation and explicit coverage limits

Root should build and run existing targets `m2m_mdt_cogging_save` / CTest
`m2m.mdt.cogging_save` (four cases) and `m2m_mdt_viewer_box` / CTest
`m2m.mdt.viewer_box` (39 cases at the CP83 freeze).

`vdie42_cogging_save_geometry` uses independent original-app MDT/Objects
expectations: billet extents, MDT output, manipulator coordinates and CNTRAX.
It traverses the production cogging geometry path rather than reproducing the
private forwarding wrapper in the test. The actual parked-model fixture in
`geometry_low_query_model_barrier_stops_next_view_retries_and_owner_bypass`
already checks the bool withStep seam: nonpending callback runs once on the
actual step; parked callback is not invoked and native state remains unchanged.

No dedicated parked execution of the private router is claimed. The runtime
negative test establishes the common seam, and static inspection must prove
this wrapper unconditionally enters that seam before its one native call.
Exposing a private router or duplicating its forwarding code solely to add a
test would not improve this bounded change. Existing native IFD tests are
separate port coverage; no claim is made that the converter target registers
or executes them.

Static preflight must use a function-bounded body: exactly one withStep, one
unchanged native call with twelve arguments and zero `.step`/`->step` accesses
within `router`. Check native call argument order and no borrow escape. The
whole file still has deferred raw accesses, so a whole-file zero claim is false.
Keep CP81 pointer/reference-result rejection and void/bool compile assertions,
native DB_STEP_DATA size0x3408, GuiModel step type/nonpolymorphic checks, API
version1 and six public C signatures active. No member/layout/access label or
public signature changes are needed. Root retains its normal source-stable
build, export/ABI comparison and archive gates.

## Read-only plan source binding

SHA-256 before the CP84 source edit:

| File | SHA-256 |
|---|---|
| source/apps/moproj2mst/src/engine/cogging_save.cpp | 58918c5f9ac62d21a6a637ed5af0784cd1c932fc96b9de66de44a268f5aceb49 |
| source/port/defcmd/src/ifd/drv/router.cpp | 1c6fb3c09b806bfec38432f101fbb33169d163abdd12031338fee1f96bcbc281 |
| source/port/defcmd/include/defcmd/objpos_ifd.hpp | 564d45a364be13ea47c845aa058f3d31134bf575243101f80db07b1f0a586b34 |

The worker performs static preflight only; no build, CTest, GUI, mutable REA,
reader continuation, pending-frame retirement or public-field encapsulation is
enabled by this migration. Public raw fields and previously held pointers
remain explicit legacy limitations and never authorize pending continuation.

## Completed source and static release

Read-only plan freeze SHA-256:
`04f107aabfdb360c47b8f1258ada5d92e159a20478d1176b71d32812f4f54cf2`.
After that freeze, the authorized private wrapper now returns an explicit
bool-result withStep callback containing the same native call. Its twelve
arguments are unchanged except the scoped actual nativeStep replacing the raw
getter. No test/helper/header/native-source edits were necessary.

Static preflight passed: balanced router-body extraction found one withStep,
one native call, exactly the twelve ordered arguments above, explicit bool
result, no raw step member accesses and the preflight wrapper before the native
call. Reversing only this exact body edit reconstructs the complete CP83
cogging_save.cpp SHA-256, proving no other source changes in that file. Existing
test counts remain four cogging-save / 39 viewer-box. These are static results;
root must perform the focused builds/runtime tests and broader acceptance.

Final SHA-256 receipt:

| File | SHA-256 |
|---|---|
| source/apps/moproj2mst/src/engine/cogging_save.cpp | b5661ce0251e0c6e262977e0bd17edd8ae8ad96a4704fd05d39c78ed0939a58f |
| source/apps/moproj2mst/tests/mdt/test_cogging_save.cpp (unchanged) | 7904836d3ec38843a283e4c8e2fa0d36cf87c2785afb8181f98eb0922c0b6db8 |
| source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp (unchanged) | f6f12fb475836c482fdde3e74ab7d92dcd357ad6479419091352289705022d0d |
| source/apps/moproj2mst/src/engine/gui_model_access.hpp (unchanged) | e05944babf29b0477452ca497d7df836ac57fa822056088e4c693a079465fa26 |
| source/apps/moproj2mst/include/m2m/engine.hpp (unchanged) | 77153701684b2644d0b470c23ba110c9ce51ad8cbabbc82ab008b8b9c174128f |
| source/apps/moproj2mst/include/moproj2mst.h (unchanged) | 34e55cdd58280b056d21e94986c11ac32ccea6f9a6c956d5dad32dbb9864fc40 |
| source/apps/moproj2mst/src/dll/abi.cpp (unchanged) | 6bf5e8bb306461440b7ba8019233449a3b2b133081f73a02430e0f06d3fca848 |

Source is frozen for root validation. No worker build, CTest, GUI or mutable REA
operation was performed.
