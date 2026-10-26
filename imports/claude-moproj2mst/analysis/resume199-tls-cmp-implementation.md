# CP199 symbolic TLS-index CMP implementation

The actual Session Report route now models exactly CMP at QtCore `[0x29614,0x2961b)` and stops before JNE at `0x2961b`. Its dword operand is a privately issued full-domain unknown read from the adapter QtCore symbol namespace, not a concrete TLS index or native observation. No branch, initialization path, return, getter, QThreadData/result owner, private+0x40 store, reference increment, publication or completion is implemented.

## Frozen predecessor and evidence

The seven prospective source/test files (including the two announced WidgetRuntime files) matched their exact entries in `build/wave3-checkpoint198-verified/source.zip` before editing. Receipt source SHA-256 is `2ddf0b0ad3d2922f19e555fa9e75385ecebdefc3eff0653ec49274522163015e`. The owner supplied CP198 PASS source review/full 361-test regression and freeze before this assignment. ConversionSession source was compared and left unchanged.

Read the CP199 next-cut/PASS review and retained original qualifications. Qualified bytes `83 3d cd 72 2c 00 ff` at RVA `0x29614` give next RIP `0x2961b`, memory RVA `0x2961b+0x2c72cd=0x2f08e8`, operand width 32, and sign-extended imm8 `ff` as `0xffffffff`. The QtCore identity remains `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`; the bounded initializer listing identity is `c06368be2e4a56cb27af21608f344fa868c383eca125e3ffd1838cd1373014b3`. These are retained static qualifications, not native execution, loader, initialized-index or same-thread TLS-value evidence. PE sentinel bytes and static producer/reset candidates are never used as the read value.

## Private root issuer and lifetime

The existing private Session factory binding now prepares a noncopyable immutable QtCore symbol and Session grant on its exact WidgetRuntime root. Their fields bind weak root/thread/allocation/capability identities, receiver, fixed hash/evidence/RVA/width and private occurrences. No value, physical module base/process/load state or initialized-index bit is present. The grant is published only after the existing pristine Native Session checks and canonical allocation/widget pairing. Ordinary standalone roots/factories and parsed projection gain no grant.

WidgetRuntime privately prepares an unpublished `TlsIndexReadExpression` only for that grant and the exact source-bound frame/context. It records a fresh unknown-dword occurrence, symbol, root, existing thread context, private Session grant/frame binding, ordinal-zero task/private identities, generation-2 and generation-3 call occurrences and read producer `0x29614`. All ownership links in retained expression metadata are weak. The index cell remains module-global; thread binding identifies execution context only.

WidgetRuntime independently retains/authenticates the exact issued read handle and grant/symbol control blocks. The factory independently retains/authenticates the exact immutable CMP handle. Matching fields, copied records, aliased control blocks and coherent lookalike substitutions cannot appoint these authorities. Factory release revokes the root grant/read even when the root and diagnostic metadata survive; allocation-scope release independently prevents consumption. Retained reads do not keep a root alive or authorize a new Session's read.

## CMP state, flags and failure atomicity

For the one unknown x, the result expression is `(x-0xffffffff) mod 2^32`. The record contains six Sub32 functions: unsigned borrow, parity of the result low byte, auxiliary borrow, zero of result, sign bit 31 and signed overflow. ZF remains the unresolved x==0xffffffff predicate. No concrete flag or dword is supplied. Width, imm8 sign extension, immediate, producer and exact read/predecessor occurrences are validated. Current flags select CMP ahead of retained second-SUB history; both SUBs and TEST/JE remain unchanged history.

CP198 stays a ten-stage validator. Its bounded wrapper computes only ordinals 0..10; a separate retained-history validator checks stage 10 for the distinct CMP successor. Cursor and RSP helpers handle pre-JNE explicitly rather than indexing an eleventh stack instruction. RSP remains S-0xa8, both live return generations and all eight stack slots are unchanged.

Read and comparison allocations are prepared before one nonthrowing commit of root read retention, factory receipt, frame flags selector and phase. An injected allocation failure immediately after unpublished read preparation verifies that root trusted state, factory/frame handles and complete predecessor state/counts remain unchanged. Terminal reentry validates the existing read/CMP and stops without allocating a new occurrence. Coherent diagnostic rewind restores root/factory/frame trusted handles with the full CP198 predecessor; partial rewinds refuse.

## Focused tests and static checks

Complete snapshots include root symbol/grant/read, fixed source fields, weak owner/control blocks, factory grant/CMP receipt, expression/context/comparison occurrences, current/historical flags and every prior frame/slot/private/count/cache field. Eight CP199 cases cover exact private ownership/unknownness/Sub32 effects, all valid reentry budgets with no replay, every interrupted CP198 prefix, fresh identities after coherent rewind and across Sessions, injected preparation failure, nine copied/aliased/foreign/expired/thread/root/symbol/grant substitutions with specific errors, 25 malformed CMP/read/phase/history/source cases, unqualified issuance, partial flags/phase rewinds, scope release, retained metadata and actual root expiration. Distinct read identities imply neither equal nor unequal numeric values.

Static preflight passed: private root issuer, noncopyable symbol/grant/read records, independent trusted handles, prepare-before-commit plus failure injection, exact Sub32 schema/pre-JNE cursor, retained CP198 history without ordinal 11, eight Testkit cases with the existing main and supported CHECK/REQUIRE macros, and no C assert. New quoted include `m2m/doe_task_runtime.hpp` in view_manager.cpp resolves under the existing application include root. No executable test or compile was run in this lane.

Released files: `source/apps/moproj2mst/src/doe/task_factory.cpp`, `include/m2m/doe_task_runtime.hpp`, `include/m2m/doe_task_factory.hpp`, `tests/mdt/test_doe_task_factory.cpp`, `include/m2m/engine/view_manager.hpp`, and `src/engine/view_manager.cpp`, all under the converter application root, plus this report. No extra Session file was edited.

No build, full suite, GUI, native execution, mutable REA database action, deployment, shared plan/state/build edit, parent/DMO/Claude/original-evidence modification was performed. Root owns compilation, execution, source review and freeze. A concrete JNE decision still requires separately qualified runtime input.
