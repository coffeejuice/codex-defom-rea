# CP199 symbolic TLS-index CMP: independent source review

**PASS for the bounded six-file source implementation.** No blocking source defect was found. The admitted Session Report frame advances one structural CMP from `0x29614` to `0x2961b`, with a fresh authenticated unknown dword and symbolic subtraction flags. It stops before JNE. This review does not establish native execution, a concrete TLS index, a branch outcome, or an executable test result; root owns compilation, regression and freeze.

Compared the complete six released files against their exact `apps/moproj2mst/...` entries in `build/wave3-checkpoint198-verified/source.zip`, whose archive SHA-256 independently matches its receipt: `e088f9b5063443870b55fd7afa62b627abffa02eb99513d703c23e20498210e0`. The receipt identifies source tree `2ddf0b0ad3d2922f19e555fa9e75385ecebdefc3eff0653ec49274522163015e`. Read the CP199 next-cut, PASS plan review, implementation report and producer/owner audit, the accepted CP198 source review, and the retained static/public-QObject qualifications before interpreting the evidence. The CP198 planning gate is reconciled against that frozen archive, rather than an actively edited predecessor.

The reviewed current file hashes are:

| File under `source/apps/moproj2mst/` | SHA-256 |
| --- | --- |
| `include/m2m/doe_task_factory.hpp` | `ce18860604563f708b4a529fa846e2ff99bd6532df35a9b1ad4ae81c889282db` |
| `include/m2m/doe_task_runtime.hpp` | `bedb3f6e4e98e8f64eb4779d886e1642f37c6e1e7973d0dce541bb1936112c47` |
| `include/m2m/engine/view_manager.hpp` | `0f14e8e53617f473d1112636e80299308f20d32bf2b04522d3a2ed937c5023af` |
| `src/doe/task_factory.cpp` | `5c865d11413fdc50cf137f91e7fb8b82fc044dae111c20a6f15dd5ab83f3cce0` |
| `src/engine/view_manager.cpp` | `4997a9d0688e43f3fffb00c24b3fae15cdd0bae36ea2da0013f4caf6c4bc687f` |
| `tests/mdt/test_doe_task_factory.cpp` | `20541a8dd7a9124d225c4e876161cd42423c8a08dde36865317c08a0a05d4073` |

## Issuer, authority and lifetime

The existing private Session binding remains the admission route. It first checks the pristine Native factory and canonical live allocation/widget pairing, prepares the original private Session capability, and then prepares the symbol/grant on that exact WidgetRuntime root. The symbol, grant and read have private constructors, const metadata and deleted copy constructors. Public identity fields or a matching hash/RVA cannot issue a grant. Standalone roots/factories and parsed projection acquire no production grant.

The root retains its own symbol, grant and exact read handles; the factory separately retains its grant and exact CMP handle. Validation checks pointer and ownership/control-block identity. In particular, a replacement public CMP record cannot appoint itself, even when a privileged test replaces the root read with a matching clone. Read preparation checks the Session frame binding, original root, live allocation scope, ordinal-zero task/private identities, exact frame occurrence, both live call occurrences and instruction RVA. The factory has already validated the generation-2 outer call, generation-3 inner call and immutable issued CP198 predecessor. These authorities are independent of the mutable frame fields.

Root, thread, allocation, capability, symbol and grant links in expression metadata are weak. Raw task/private identities carry no ownership. The existing WidgetThreadState supplies execution context, with no claim that the global slot is thread-local or that a getter returned QThreadData. Factory release revokes the root grant/read and clears its own CMP/grant handles. Allocation release independently fails admission; Session destruction explicitly releases factory and allocation scopes. Retained diagnostic reads therefore neither keep the Session/root alive nor authorize another read. The symbol remains inert root metadata after revocation; no physical DLL unload/load or native lifetime is inferred.

## Instruction and state checks

Read-only PE section mapping independently verified the retained QtCore hash `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`, bytes `83 3d cd 72 2c 00 ff` at RVA `0x29614`, following bytes `0f 85 9f 00 00 00` at `0x2961b`, and the effective address `0x2961b + 0x2c72cd = 0x2f08e8`. The executed range is exactly `[0x29614,0x2961b)`. Opcode 83 sign-extends imm8 `ff` to the 32-bit immediate `ffffffff`.

The PE contains `ff ff ff ff` at the file-backed target. Those bytes are not a running value. Production read construction uses only `FullUnknownDword`, identity and occurrence context; no concrete value, PE-derived sentinel, host TLS, initialized-index receipt, loaded base or successful-startup assumption is introduced.

For the same unknown operand x, the record represents `r = (x - 0xffffffff) mod 2^32` and six Sub32 functions: unsigned borrow, result-low-byte parity, auxiliary borrow, result zero, sign bit 31 and signed overflow. ZF remains the unresolved equality predicate. There is no arithmetic evaluator or concrete flag observation in this increment. Current arithmetic flags select CMP before the retained second SUB, while the first/second SUB and caller TEST/JE records remain history.

CP198 remains ten stages. Its wrapper bounds ordinals to 0..10, and the distinct successor validates retained stage-10 history rather than computing an eleventh displacement/cursor entry. Explicit successor helpers give `0x2961b` and RSP `S-0xa8`. Commit changes only the authenticated root read, factory/frame CMP handles and phase. Registers, eight stack observations, both live return generations, private fields, counts, cache and incomplete state remain as admitted. No JNE evaluation/path token, initialization, return/reload/getter, private+0x40 store, thread/QObject increment, task publication or completion is added.

## Atomicity and test-source assessment

Symbol/grant binding prepares all allocating objects before nonthrowing publication. CMP preparation similarly creates an unpublished read, comparison occurrence and immutable comparison allocation before publishing root read, factory receipt, frame handle and phase through nonthrowing shared-pointer moves/assignments. Injected failure immediately after unpublished read creation leaves the trusted root and complete predecessor unchanged. The normal terminal unsupported error follows a completed instruction; reentry validates the exact issued read/CMP and stops without preparing another occurrence.

The eight added Testkit cases cover actual Session default admission, exact ownership/unknownness/width/immediate/Sub32 schema, current-flag precedence and preserved predecessor, terminal reentry under valid diagnostic budgets, every CP198 split/resume cut, fresh occurrences after coherent rewind and across Sessions, preparation failure, nine copied/foreign/aliased/expired/thread/symbol/grant substitutions, 25 malformed CMP/read/phase/history cases, unqualified read issuance, partial rewind refusal, scope release and root expiration with retained metadata. Privileged coherent rewind restores root/factory/frame trusted state together. Complete refusal snapshots include the new root symbol/grant/read state and factory grant/CMP retention alongside prior frame/private/stack/history/publication fields; root handle control blocks are checked separately. Distinct expression identities do not assert unequal numeric values.

The corruption matrix is bounded, not exhaustive over every field and lifetime interleaving. Some malformed records are rejected by the independent issued-handle check before their changed field becomes decisive, which is consistent with the authority contract. No source mechanism was found that bypasses that check through public diagnostic state. Existing synthetic/standalone/nonnull guards remain present.

Static test preflight confirms the existing Testkit main, supported CHECK/REQUIRE macros and resolvable application includes. No C assert was added. The new quoted include in view_manager.cpp resolves under the existing application include directory. I did not compile or execute tests.

No unrelated edits were found within the six-file increment. A read-only comparison of all 3,554 archived paths found no missing file and exactly these six changed files plus `tests/s/test_save_current.cpp`; root confirmed the seventh as its separately owned RS39 test-only lane, outside this review. `conversion_session.cpp` remains byte-identical to CP198.

Only this report was written. No source/test/build, GUI, native execution, mutable REA database, shared plan/state, parent/DMO/Claude/original artifact or deployment was modified. Concrete JNE selection remains blocked on a separately qualified runtime value at the exact read occurrence.
