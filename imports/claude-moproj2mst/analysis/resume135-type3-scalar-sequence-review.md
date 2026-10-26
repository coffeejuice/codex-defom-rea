# CP135 independent Type3 scalar sequence review

No blocking discrepancy found in the released private in-memory sequence. The implementation preserves dispatch order and earlier scalar mutations, stops on ENDDATA before the next line, and keeps unsupported input behind explicit candidate guards. It does not admit public Type3/ShapeOptim loading or claim file-parser success.

## Evidence and control order

Reviewed the new sequence header, implementation and all four tests against the existing scalar dispatcher/tokenizer, CP130 termination qualification, CP134 parser review and CP135 parser-boundary report. Retained parser assembly `resume127-type3-opt-parser.asm` SHA-256 `7c77136a27a1514d94d02a07263a082a9c4c2fddf2f323cb26335df57a5172d9` is tied to original DEF_GUI_MO_64 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

The native loop tests atEnd at `14090886d`, reads at `140908893`, splits at `14090890a`, applies a recognized scalar before jumping through `1409093ee` back to the loop, and takes the ENDDATA branch `1409093b9..1409093ec` to common cleanup `140909400`. The new driver dispatches one line immediately before advancing its index. It performs no whole-input preflight or rollback. ENDDATA returns the following index; surrogate/row refusal returns the unapplied index. The existing dispatcher returns row/short-scalar guards before writes, and the tokenizer clears its temporary fields before checking surrogates, so previous tokens cannot be dispatched after refusal.

These are candidate guard outcomes, not original malformed-input behavior. The separate first-line read/trim/conversion, successful-open purge mutation, stream decoding, read/close failures, native false return, and row allocations remain outside the helper. The receipt states those limits accurately. No native exception or cleanup equivalence is established by this synchronous value-only driver.

## Ownership, tests and public boundary

The span and state reference are borrowed only during the call. The only temporary ownership is the local token vector and its QString values; no pointer/span escapes, native optimizer owner is invented, or row storage is appended. Earlier state writes survive either a later guard or ordinary C++ unwinding; no native allocation-failure contract is claimed.

The four tests meaningfully distinguish encounter-order overwrite, unknown/skipped-key continuation, terminal index and suppression of later writes, prior-effect retention at NUM_CON, and surrogate refusal after a write. Existing scalar-prefix tests cover empty tokens, token-output clearing, unchanged ENDDATA state, both row keys and short-scalar guards. A whitespace-only line between two sequence mutations would be a useful additional integrated case, but its absence is nonblocking given the reviewed clear/empty-dispatch path and existing helper tests.

Repository references to the new driver are limited to its declaration/definition and focused test. `ShapeOptimClass::loadClassData` in `src/ops/report.cpp` still refuses the type-3 optimizer owner; no public route calls this sequence. CMake retains C++20 and recursive core/test discovery. Build and runtime validation belong to root; none were run in this review.

## Frozen binding

All three source/test hashes match the implementation receipt:

- Header: `b2a1605aa4851167e33e6e9f059f294fc73f06d433e9c6b6c9cd66402bcbd7ad`.
- Implementation: `ab3251e3f8534ad65c0d25919e9415457fe18c7e91224f8ae1af7156835610a6`.
- Focused test: `970c31d372cc0a86aead0231557db9baeeab548119266cbdcabc303c3f9f5b25`.
- Implementation receipt `analysis/resume135-type3-scalar-sequence.md`: `68a93118bb0078263b90b84a1e58ea6e9a01d34ce11d5ecb394d297256290326`.

Only this review report was written. No production/test edits, build/test execution, GUI, original process execution or mutable REA actions were performed.
