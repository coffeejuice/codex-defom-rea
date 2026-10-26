# CP201 inert paired first-instruction effects

Implemented a static semantic extension to CP200's immutable unresolved JNE record. The CP200 freeze receipt inspected was `build/wave3-checkpoint200-verified/receipt.json`, source fingerprint `c9aaa78c42c45ac35c232b0b1ea031c44e24090f0a23f8733a23e8c52574bf5f`. The implementation follows `resume201-tls-edge-static-next.md` and its PASS plan review. No build or tests were run in this source lane; root owns validation and freeze.

## Representation and boundary

Each existing ordered edge now owns a `PotentialEffect` with explicit `UnevaluatedNotApplied` status. It carries exact bytes (six-byte storage, zero padded for the five-byte taken MOV), instruction length, operand width and symbolic address, prospective destination/result, unchanged flags/RSP, and prospective successor RVA.

- ZF set, `0x29621`: `8b 05 01 a7 2c 00`, width 32, potential module operand `+0x2f3d28`, prospective EAX / zero-extended RAX result, next `0x29627`. No read object, value, issuer or occurrence is created for this operand.
- ZF clear, `0x296c0`: `48 8b 5c 24 40`, width 64, `[RSP+0x40] = [S-0x68]`, prospective RBX result from the existing task spill, next `0x296c5`. The descriptor retains the exact existing slot-7 generation-3 spill handle, authenticated by the unchanged CP198 history validator.

The enclosing issued CP200 record still binds the exact issued CP199 compare/read chain and its existing root/thread/capability/call context. No new issuer or separate factory-local shared-state owner was introduced. CP200's production schema control remains default disabled. The terminal remains unresolved at `0x2961b`; no edge is selected and no operand is read or effect applied. Existing registers, flags, RSP, slots, returns, shared state, read identity, completion and native publication remain unchanged by descriptor preparation. CP200 diagnostic record/phase publication itself retains its existing behavior.

## Validation and atomicity

`validateTlsIndexJneUnresolved` checks every descriptor field against the canonical static pair after validating the issued compare/read and CP198 history. Taken spill validation requires both exact pointer and shared ownership identity. Fallthrough requires an actually empty spill handle, including no concealed owning-null control block. The immutable issued JNE pointer and owner check remains mandatory. Descriptor references are prepared before publication; existing failure injection remains after all preparation and before any publication. Reentry constructs no occurrence or read and leaves the same terminal record intact.

## Test source changes

Added `doe_task_factory_cp201_potential_mov_descriptors_are_exact_and_never_applied`: exact bytes/widths/addresses/results/successors, RIP and stack displacement arithmetic, exact spill ownership/generation/call, same compare/read chain, stable terminal reentry, and full snapshot equality after privileged restoration of the CP200 predecessor.

Added `doe_task_factory_cp201_every_descriptor_field_and_spill_owner_are_validated_atomically`: 44 edge/damage combinations, each retried, covering every descriptor field, byte padding, absent/copied/foreign/wrong-slot/alias-owner/owning-null spill handles and swapped effects. Privileged fixture replacement sets both public and private JNE handles so these tests establish field/provenance validation independently of the final issuance check. Refusals must leave the full snapshot unchanged.

Extended CP200 semantic comparisons to compare descriptor values across independent runs while separately checking each run's exact spill relation. Existing CP200 tests now carry the descriptors through all constructor/callee interruptions, direct/resumed equality, preparation failure and retry, copied/malformed/foreign JNE and predecessor substitution, stale/rewound history, revoked/expired/released authority, allocation/factory release, ordinary ConversionSession lifetime, and no-publication checks. Their names and default-disabled semantics remain unchanged. These are test source claims, not passing results; root must run focused and required regression suites.

## Qualifications and ownership

Read the static plan/review and the original source qualification in `resume163-connector-qobject-static.json`: bounded/unwind observations do not establish a complete logical function or native execution. CP201 descriptors make no branch, read, execution, synchronization or unwind-closure claim. No GUI control, original launch, mutable REA, build, deployed application, parent DMO source or Claude tree was changed in this lane.

Source ownership is released to root for review/build/tests: `doe_task_runtime.hpp`, `doe_task_factory.hpp`, `task_factory.cpp`, and `test_doe_task_factory.cpp`. No worker retains these files for further edits.
