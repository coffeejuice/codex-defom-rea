# CP139: remaining pure encoded-null stores before callback initialization

The next source-closed ownership prefix is **eight additional qword stores using the same actual encoded-null result already produced by CP137**, followed by a no-op leaf return. It requires no new Kernel call, error value or FLS/PTD owner. Stop before146ea3d ->147e75c, whose body initializes a nonnull callback instead of storing the passed null encoding. This is the recommended coherent batch; the strict minimum is just the first one-store leaf1481ca8.

Only this report was written. No source/build/test/GUI/original execution or REA database work. Original DEF_GUI_MO_64.exe v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Addresses use the common14 prefix. Bounded DUMPBIN14.44.35228.0 and section-mapped immutable PE bytes qualify the following paths.

## Exact stores and source argument

146e9f4 retains the actual EncodePointer(nullptr) result in RBX at146ea02. After CP137's first setter returns, it reloads RCX from that saved value before each following call. All five setter bodies below only store RCX to their fixed qword roles and return; no allocation, callback, encoding/decoding or TLS work occurs.

| Caller occurrence | Callee source store(s), in order | Original role labels |
| --- | --- | --- |
|146ea0d ->1481ca8|1481ca8|1447006d8|
|146ea15 ->1470438|1470438|1446ffb40|
|146ea1d ->1481ca0|1481ca0|1447006d0|
|146ea25 ->1481ad4|1481ad4|1447006c0|
|146ea2d ->1474954|1474954,147495b,1474962,1474969|1446ffb68,1446ffb70,1446ffb78,1446ffb80|
|146ea35 ->146ff78|No memory effect: RET0 at146ff78|None|

The first four callees return at1481caf,147043f,1481ca7,1481adb; the four-store leaf returns1474970. The last leaf is genuinely a no-op, not a missing role/default. After it returns,146ea3a reloads RCX from RBX and the next call is146ea3d. The eight store targets must remain distinct canonical candidate cells with unavailable knowledge until their actual store event; do not default a zero backing value into a known null token.

Resume must bind the **same captured actual CP137 encoding**, not call EncodePointer again or accept an external token/report. The private canonical runtime has the actual result and first-role state. Preserve a source-frame/saved-value authority or verify/recover that private unmodified issued value under the same owner lock; do not reinterpret an arbitrary report as current handler state. Numeric-zero encodings remain admissible. The original role addresses are source labels, never addresses to write in the candidate process.

## Correction and next blocker

Earlier CP136/138 read-only shorthand describing seven remaining calls as null-role setters was too broad. CP137 implementation is unaffected because it stops before the first such call. Two later calls differ:

-146ff78 returns immediately and stores nothing.
-147e75c **ignores its incoming RCX**:147e760 loads nonnull callback147e714,147e767 calls14813d8 (EncodePointer wrapper), and147e76c stores that encoded callback into1446ffbe8. It must not be replaced with the captured null token.

147e714 in turn calls PTD getter148156c, reads optional PTD+D0 callback, conditionally invokes it, then calls146d94c. A valid candidate counterpart for that callback/termination/PTD owner has not been established in this lane. Using the original executable VA as a candidate callback, a generic host handler, or null would fabricate identity/behavior. Therefore guard before147e75c, even though its own initialization is only an encoding/store. The caller later performs another nonnull callback encoding at146ea49; lock/FLS startup comes after that initializer chain, not after the eight null stores alone.

The pure retained-image mapper does not solve this owner gap or supply LastError. Existing failed-free stages remain before errno provider; no later host error word can be substituted. FLS still needs source callback1481590, actual owned index, locks and PTD lifetime. ContextEnvelope's first-buffer/optional-owner/destructor and accepted Close's main/Project/manager predecessors are unchanged.

## Proposed exclusive private implementation

After root assigns a source cut, reserve existing src/model/feature_crt_runtime.hpp/.cpp, tests/model/test_feature_new_handler_prefix.cpp and tests/model/test_feature_crt_runtime.cpp beneath source/apps/moproj2mst. Optionally add a separate test executable for the completed-first-prefix/late-child refusal profile, since the retained runtime permits one child frame per process. No public model/Project/Session/CMake changes are required.

Suggested explicit method: `resumeEncodedNullRoleTail(canonicalLease)` with a separate fixed-size report/cursor and eight canonical candidate role cells. Require real CP137 completion, the same process/module/control-block owner and no retained child-map frame. Preserve old first-role receipts and stop behavior as history. Do not auto-drive the tail from heap/first-role/map APIs. Restore the actual saved source argument for each callee, execute the eight stores/readbacks in source order, record returns including the no-op, and latch before the first effect. Stop before146ea3d, with no callback encoder, handler invocation, host CRT change, FLS/LastError or complete-startup claim. Repeated calls cannot replay stores or infer completion from equal-looking values.

Tests should derive their golden ledger from the table above: all eight source-role labels/store sites/values/order; one actual initial encoding across both APIs; preserved first-role state; no extra heap/kernel effects; same-owner lease persistence; zero-token admissibility; prerequisite/foreign-lease refusal; child-frame-before-tail refusal; and no automatic/repeated execution. The positive test stays in a fresh executable profile before child preparation. A dedicated negative process can complete the first prefix, prepare a host-only child frame, then show the new tail refuses without any source stores. Keep Testkit Release-active checks and explicit candidate-only role semantics.

## Bounded receipt

| End-exclusive original range | SHA-256 |
| --- | --- |
|14146ea0a..14146ea3d caller|`c6182d7216823990470b19282f5e80397187d4dfe3de2a466be1efcf62ec22d7`|
|141481ca8..141481cb0|`79f19d76b225061b5a85cda175a30c6889c22c4d9b75fc82824aaca3429a3d62`|
|141470438..141470440|`b7b64b177a40336007a903652e97d3b4b83b110e65366b920551a429764817f5`|
|141481ca0..141481ca8|`79f19d76b225061b5a85cda175a30c6889c22c4d9b75fc82824aaca3429a3d62`|
|141481ad4..141481adc|`127a02262bf4851b9d2d7df8c5450d0dc506c4f136a7f96c6003b80b2b331917`|
|141474954..141474971|`a9ceb5652a9abbc860ebb6921a750d7d1e9f23abcf1d91da297175c803d8f762`|
|14146ff78..14146ff7b no-op|`b6cbececf1b07e74f37d57710c68c31f0b92cb08d51093252b321b07542e177b`|
|14147e75c..14147e778 next callback initializer|`fbfcfe635c3852a56b40939652b94dc92088071efdffb54b0ad3bedb6c089a85`|

Identical byte hashes for1481ca8 and1481ca0 do not identify the same role: their different instruction VAs/RIP-relative targets above are part of the qualification. CP137 receipt `analysis/resume137-feature-new-handler-prefix.md` SHA `1655c51fcecb52c14a3a7fac1d9738343d655d54437bae905eb94e10dd2c289d` retains the actual producer/owner contract. This report is not an implementation/build/runtime witness. No reconstructed executable/deployment was inspected or changed.
