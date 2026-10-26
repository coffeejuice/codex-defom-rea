# CP66 actual node-owner old-version fast path

The public3D geometry getter now admits an existing actual N owner with u1b58=0
when its signed step_version is below6000, with preallocated coordinates and the
same exact canonical registry. Native synchronization then takes its proven
fast return, while actual coordinate getter dispatch preserves reached COW
effects. No owner, flag, version, dimensions or arrays are synthesized.

Exclusive edits are `source/apps/moproj2mst/src/engine/viewer.cpp`,
`source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp` and this report. No shared
model/CMake edit, build, CTest, GUI or mutable REA action was performed. Source
is frozen for root validation. Token measurement is unavailable.

## Qualification and change

`analysis/resume66-geometry-next.md` retains bounded dependency reads, byte/
stdout hashes, exact native gate order and fixture implications. Original
DEF_GUI_MO_64.exe SHA-256 is
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
Gate `14082bf10` returns true for a nonzero actual owner+1b58; on zero it calls
`14035b400` (owner+1c==3), then `140825420` (owner+1b64 step_version) when3D.
Its signed JL at `14082bf49` returns true below6000. Node sync fast-returns via
`14082e9d8` on that true result.

`qualifiedGeometryReadiness` still verifies actual N-step identity before reading
model fields. The zero-flag rejection is now conditional on actual model not3D
or signed version>=6000. Thus only the separately qualified public3D old-version
case expands admission. Nonzero flags keep their existing branch independent of
version. CP65's null/actual E-owner check remains independent; arbitrary E/N
pointers, missing coordinates and noncanonical registry stay Unqualified.
CP64 refusal before raw arithmetic and all builder checks remain intact.

On an admitted path, the existing real coordinate getter still calls key0
collection dispatch before model synchronization, so a shared list detaches at
the native point and the second getter sees a unique body. Preflight reads are
ordinary actual-model qualification reads, not a replacement dispatch. Actual
zero-flag/version>=6000 remains adapter refusal with no dispatch/detach. That
does not claim native failure-order equivalence: native would reach its COW
prefix before the unresolved synchronization body. No such incomplete prefix
is fabricated or executed through the port's unconditional counterpart.

## Focused tests and fixture migration

Three new Testkit cases bring the file to24 unique cases:

- Signed threshold matrix covers INT32_MIN,-1,0,5999,6000,6001,INT32_MAX with E
  owner null/actual and populated/null connectivity (28 combinations). Below6000
  observes two/one real n4/key0 dispatches with actual dimensions, first shared
  detach and stable second body, and Ymax1/0 respectively. At/above6000 observes
  no dispatch/detach and unsupported selected rectangle. Every combination
  checks variable identities/IDs/current/previous/value bodies, list refs,
  source arrays, owner fields, bytes and version remain unchanged by refresh.
- Nonzero node flag0x80 at version6000 and INT32_MAX remains qualified, with two
  actual dispatches, stable unique body/arrays/owners, unchanged bytes/version
  and populated bounds. This protects the native early nonzero-byte branch.
- Old-version actual N/E with missing coordinates or id0 registry still refuses
  without dispatch or shared-list detach, preserving the prior admission guards.

The intended zero-flag refusal fixtures now explicitly set step_version=6000
instead of relying on the constructor's0: guard-matrix modes0 and6 (the latter
also has actual E owner and zero E byte), and restoration-test mode0's refused
phase. Assertions check those versions; restoration resets the captured prior
version before its known phase. Prior CP63/64/65 reports/receipts are unchanged
historical records, not rewritten claims that their earlier default0 fixture
already exercised the native gate-false route.

All previous qualified ownerless/actual-owner/E-byte/COW tests, opaque/foreign
owner refusals, custom/null/noncanonical registry checks, malformed builder
guards, zero-count skips and restoration cases remain. Hooks forward the real
dispatcher and end before cleanup; leases own only list storage and fixture
owner fields restore before mesh destruction.

Static preflight confirmed24 unique case names, existing Testkit entry point,
Release-active CHECK/REQUIRE macros, no C assert or unsupported REQUIRE_EQ,
unchanged resolvable includes, no trailing whitespace and successful
`git diff --check`. No compilation or test execution is claimed.

Root target **`m2m_mdt_viewer_box`**; CTest **`m2m.mdt.viewer_box`**. Root owns
focused/full validation. This is a conditional internal fidelity expansion;
real-project optional-mesh owner provenance and native runtime parity remain
outside the synthetic test evidence.

## Frozen source SHA-256

| File | SHA-256 |
|---|---|
| `source/apps/moproj2mst/src/engine/viewer.cpp` | `18fc6c1913ec6142bb00e0f7278a3f4ff954ee01f853ce662a593d646d20042b` |
| `source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp` | `00d470e743c8333089b7e4c89e4d47307580840f5c96cbccc023fd7616ea2223` |
