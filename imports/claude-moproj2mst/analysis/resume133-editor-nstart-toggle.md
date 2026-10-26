# CP133: bound private NSTART toggle and refill

The recovered private editor owner can now consume an explicitly delivered NSTART checkbox toggle. It stores the actual operation's given flag first, then resolves the current simulation control and refills the cached step-count page. The refill preserves the original signed-wrap calculation and hidden-checkbox history. This is a private page projection on an actual bound Project/Operation/GuiModel; it does not execute native Qt widgets or complete the public Session constructor.

Source and test files are frozen for root integration. No worker build, compiler, CTest, GUI, original execution, mutable REA operation, executable or deployment change occurred. Root supplied the shared declarations. Only the three assigned C++ files and this new receipt were edited.

## Evidence and scope

The implementation follows frozen `analysis/resume132-editor-next.md`, SHA-256 `2d2de720196437cec8041b036940ed5560d5bd9dc63a4136301e3c130abbba22`. Original DEF_GUI_MO_64 v11.0.2 has SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

- Callback `[140ecf910,140ecf977)` SHA-256 `c42e04dbe9cf19df81308a1844308f42e0b1fdb2c35f4c10adad87bfeeed8916`: procType5, live OPI, setter, current SC lookup, complete refill; no same-value early return.
- Setter `[14005a2c0,14005a2d7)` SHA-256 `5c8db3fb96328f5fe89483750946d171aba51735f2a339639c3d628b4cd50439`: delivered byte to OPI+18.
- Refill `[140ebb110,140ebb3dd)` SHA-256 `f7f7cb6e239a1437f6930d385b4534c5f62beeb0ebc181e305efb23c889788c5`: checked-state branches, SC-based ADD/NEG, three numeric controls, primary-die tail.
- Current SC getter `[140130f90,140130fd5)` SHA-256 `f85c714b94efce9d22960b1d2321e0621f827cd1a4b31a59b4f43a08e72b220f`; guarded step provider `[140c7b2d0,140c7b301)` SHA-256 `2f5b78f5174075403916e79492d514c1eb0aab38b00ffd322bcef44b73bc2ef2`.

The primary-die vector remains an explicitly supplied current eligibility-result profile. This increment does not recover native model enumeration, combo labels, physical signal blocking, visibility/stack changes or reentrant widget delivery. The caller must update the cached checkbox to the delivered value before calling the method. Non-MO/null-OPI source no-ops are not inferred from invalid owner metadata.

## Changes

`simcontrol300.cpp` retains the existing two-argument `fillStepCount` as a wrapper with `priorNstartChecked=false`. The new overload initializes the checkbox from its prior state. Non-key operation branches explicitly clear it for missing/nonwriting classes or set it from the operation for writable classes; key/null-operation branches retain history. Fresh existing calls therefore keep their previous results. The original unsigned ADD/NEG and spin clamps are unchanged: positivity can come from operation.startStepNo while negation still uses SC.nstart.

`StagedEditor300::toggleNstart(binding,currentContext,checked)` validates the current Project/OPI generation/GuiModel/remesh, completed private page metadata, same page/context operation and cached delivered checkbox. It does not call `pageState()` during admission because that getter queries SC too early. It then:

1. Stores `binding.operation.nstartGiven = checked ? 1 : 0` and commits `OperationFlagStored`.
2. Looks up the actual bound model's current SC and requires the original session's source-control identity.
3. Refills from current SC/current qualified context, replacing only the step-count page and returning `Complete`/`Completed`.

Null or replaced SC returns `UnsupportedSnapshot` at `OperationFlagStored`, preserving the flag and cached controls. This is a candidate prerequisite guard, not a claim of graceful native null-SC handling. Exceptions from later source access propagate without undoing the flag. Operation.startStepNo and SC fields are not Save-written by this callback. A repeated explicitly delivered boolean still refills.

The public `PrivatePaletteUnavailable`/6504abb0 boundary, native-base construction, lifecycle callsites and `completeForPublicSave=false` remain unchanged. A factory without completed private pages refuses the callback.

## Focused tests and static preflight

New target: `m2m_mdt_simcontrol300_nstart_toggle`; CTest: `m2m.mdt.simcontrol300_nstart_toggle`. Eight Testkit cases cover:

1. Actual bound non-key operation: true/false delivered toggles, all numeric/primary controls refilled, original positive-override/SC-negation bug, unchanged SC/startStepNo/other page/files.
2. Repeated same-value occurrence, current primary eligibility fallback, int32 ADD/NEG wrap and spin clamps.
3. Starts-from-key checkbox history, with fresh two-argument Fill still unchecked.
4. Missing/nonwriting class clears projected checkbox while the preceding operation flag store remains true.
5. Destroyed actual model step: flag commits before missing-SC guard, cached controls retained and no stale dereference.
6. Replaced model step held distinct from original: no substitution or refill after flag commit.
7. Wrong context, missing/replaced context operation, cached-event mismatch and wrong Project binding refuse before store.
8. Unconstructed/partially constructed owner and reused OPI address with a new generation cannot create private pages or advance the public gate.

Static checks verified eight cases, exactly one Testkit main, no C assert or unsupported REQUIRE_EQ, quoted includes resolved against the actual app include root, defmodel layout include exists and its explicit constructor takes dimension3. The source archive comparison against CP132 shows only the planned fill-overload hunks in simcontrol300.cpp and the utility include/new callback in simcontrol300_owner.cpp. CMake's existing CONFIGURE_DEPENDS test glob and naming rule provide the target above; no CMake edit is required. Root must configure/build and execute the focused group and required regression suite; this receipt records no runtime test pass.

## Frozen release hashes

| File | SHA-256 |
| --- | --- |
| source/apps/moproj2mst/src/engine/simcontrol300.cpp | `04b62ac1da494473cb26e1dcb3d1ae93ad087cf3c3fa4409f37e8ee6336f9a80` |
| source/apps/moproj2mst/src/engine/simcontrol300_owner.cpp | `65af624f02b7a322a041d0f114bdf2bfba1aa5d22a9da538c20dd061c4378e8c` |
| source/apps/moproj2mst/tests/mdt/test_simcontrol300_nstart_toggle.cpp | `0d98957da0495ca7782d47292e3e776c37973beb6d176e884cab1d04d5fda0ed` |

Ownership is released to root and these files remain held through source/binary binding, tests and archive freeze. The next dependency is root integration of this private callback target; public constructor palette/control ownership remains independently guarded.
