# CP201 inert edge-effect source review

Decision: **PASS for root build and validation.** No source correction is required. This is a read-only source review, not a passing build/test receipt or permission to execute either successor. The boundary remains the unresolved JNE at QtCore RVA `0x2961b`.

## Baseline and reviewed scope

Compared the four released task-factory source/test files against `build/wave3-checkpoint200-verified/source.zip`, authenticated through its receipt. Independently recomputed archive SHA-256 `74084cde99d622b367aba896a9ecc0ec8826b1a446a15b9aeb2df602ec0c65e3`; receipt source fingerprint is `c9aaa78c42c45ac35c232b0b1ea031c44e24090f0a23f8733a23e8c52574bf5f`. Compared decoded lines to avoid treating archive/current newline encoding as program changes.

Reviewed current file SHA-256 values:

| File under `source/apps/moproj2mst/` | SHA-256 |
|---|---|
| `include/m2m/doe_task_runtime.hpp` | `c3bb5e89f1770333d2eed55ed137c2537f99a5ce9824c91471168a7cb1dc1ab8` |
| `include/m2m/doe_task_factory.hpp` | `860f9732a99cf7358ce1ed340ea392e928cdf28b32b5ef7d0e8d34969c5da527` |
| `src/doe/task_factory.cpp` | `fe1ab28c3e1ee846972f9762d35dbf79a103f510b0ad199a7110596d9973ca48` |
| `tests/mdt/test_doe_task_factory.cpp` | `da0783842862bcb4398de6608148faa9701437d31696acac97d87553ca58e383` |

Read the static-next report, plan review, implementation report, CP163 source qualification and public QObject ownership contract. Their bounded/unwind evidence does not establish a complete logical function or native execution. Existing root TLS grant/read validators were inspected for the inherited authority chain.

## Original instructions and inert semantics

Independently mapped the copied original PE sections in memory and verified original `QtCore4_64.dll` SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. Exact bytes match the schema and static contract:

| RVA | Original bytes | Meaning represented |
|---|---|---|
| `2961b` | `0f 85 9f 00 00 00` | Unexecuted JNE; ZF-set fallthrough `29621`, ZF-clear target `296c0` |
| `29621` | `8b 05 01 a7 2c 00` | Potential 32-bit module read at `29627+2ca701=2f3d28`; prospective EAX and zero-extended RAX, next `29627` |
| `296c0` | `48 8b 5c 24 40` | Potential 64-bit stack read at `RSP+40=S-68`; prospective task RBX restore, next `296c5` |
| `2960f` | `48 89 5c 24 40` | Existing generation-3 RBX spill producer |

The five-byte taken instruction's sixth storage byte is zero padding, not an instruction byte. Both descriptors explicitly use `UnevaluatedNotApplied`, preserve flags/RSP, and contain no read value, new operand occurrence, selected edge or applied effect. The new module operand is distinct from the issued CP199 TLS-index read at module RVA `2f08e8`.

The actual implementation delta adds descriptors and attaches the existing spill handle before preparing the immutable record. It introduces no memory-read issuer, execution path, register assignment, stack mutation, global state owner, branch evaluation or native-publication change. The existing CP200 diagnostic phase publication remains; the instruction cursor remains `2961b`. The factory flag remains private and default false. Already-issued terminal reentry validates and returns the same unsupported boundary even if the diagnostic flag is later disabled.

## Authority, lifetime and atomicity

`advanceTlsIndexJneUnresolved` validates the Session caller and exact CP199 compare/history before construction. The retained immutable record links the exact issued compare; that compare links the exact issued read, existing root/thread/symbol/grant and frame/call context. `validateTlsIndexJneUnresolved` requires pointer and control-block identity for both compare predecessors and the issued JNE. It compares every descriptor field to the canonical pair.

The taken descriptor must retain the exact pointer and control block of `calleePrefix->slots[7]`. The inherited CP198 history validator authenticates that slot's producer `2960f`, generation 3, width 64, offset `-68`, TaskRbx token, caller occurrence and nested call occurrence, then requires the exact issued callee-prefix owner. The fallthrough descriptor requires a truly empty spill handle; an owning-null alias fails the explicit owner check. Equal-address handles with different control blocks cannot pass.

The existing Session/root/read checks still reject foreign or expired roots/threads, copied or absent capability/grant/read state, revoked grants, released factory/allocation scope, changed frame/call occurrences and rewound phase/history. A retained descriptor is diagnostic storage, not replacement authority. The new strong spill reference contains weak occurrence relationships and does not establish a new root owner or restore released scope authority.

All descriptor preparation, record allocation and terminal-error preparation precede publication and the existing failure injection. A preparation exception leaves existing state intact. The subsequent shared-pointer assignments, phase assignment and terminal throw introduce no new allocating operation between publications. Validator construction copies static fields and existing shared handles; it issues no occurrence/read. There is no new rollback path or partially published descriptor.

## Test review and remaining gate

The two added tests are meaningful: one independently states instruction bytes, widths, displacement arithmetic, destinations, successors and spill provenance; the other exercises 44 edge/damage combinations, twice each, including every descriptor field, padding, copied/foreign/wrong-slot/absent spill, different-owner same-address alias, owning-null alias and swapped effects. The latter deliberately replaces both public/private JNE handles, so rejection must come from field/provenance validation rather than merely the final issuance check.

Inherited CP200 tests now carry descriptors through all eleven callee interruption positions, earlier constructor cuts, repeated preparation failure/retry, direct/resumed semantic comparison, copied/foreign/malformed JNE and predecessor substitutions, rewinds, authority revocation/expiry and scope release. Snapshot helpers include descriptor edges, read/root state, register/flags/stack/history and completion/publication observations. Restoring only the privileged JNE checkpoint to its predecessor permits a full pre/post snapshot comparison of all other state. Stable terminal reentry is checked repeatedly. Independent-run semantic comparison correctly excludes differing opaque spill tokens while checking each run's spill relationship; the new exact-owner test and validator establish ownership separately.

No tests or builds were run in this lane. Root must retain focused tests, required regression/full-launcher validation, source-stability verification and freeze before claiming checkpoint validation. No additional behavioral progress, branch witness, synchronization guarantee or unwind closure follows from this PASS.

Only this report was written. No source, build/test outputs, GUI session, REA state, original process/application, parent DMO or Claude tree was changed.
