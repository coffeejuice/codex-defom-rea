# CP118 Spring general reader/page review

## Result

No functional blocker found in the frozen bounded reader/page increment. The parser clears its output, preserves the exact historical `x\r\n` and nonpositive-count branches, parses a positive count before enforcing candidate cap 256, requires absent `SPRING2.INI`, builds rows in temporary storage, and publishes only after all required fields parse (`simcontrol300_native_io.cpp:285-329`). `readSpringInt`/`readSpringReal` tokenize on ASCII whitespace, validate the complete token against the intentionally narrow signed-decimal/finite-C-locale grammar, and use locale-independent `from_chars` conversion (`:159-218`). The per-row extraction order matches the retained original, the loop stops after the declared count, and ASCII suffix tokens are left unread; `asciiBytes` still rejects non-ASCII bytes. The cap is correctly described as candidate resource policy, not an original format maximum.

The focused native reader test exercises retained one/two-row spellings, a changed numeric row, three distinct ordered rows, mixed whitespace/sign/exponent syntax, ignored trailing ASCII tokens, cap 256/cap+1, present companion, malformed/partial/non-finite/non-ASCII cases, and zero/`x` behavior (`tests/mdt/test_simcontrol300_spring_reader.cpp`). It asserts parsed output remains empty on guarded failures. It does not claim malformed-token Qt cursor equivalence or original allocator failure behavior.

The typed page supports 1..256 rows only when all records have finite doubles and flag zero (`simcontrol300_io.cpp:31-40`). Fill looks up combo IDs for row0 only, the initially selected/no-click row; it preserves unselected later IDs/payloads without inventing row-change callbacks. Apply requires checked page/current row0, selected IDs matching the first payload, flag zero and unchanged visible fields; it copies the vector, reparses only row0's five visible numbers, preserves all later payloads, then writes count and all rows in source order (`simcontrol300_io.cpp:159-174,213-265`). This aligns the CP117 callback-order report for an Open→Save with no row click. Exact writer tabs/LFs/final CRLF, primary-first behavior and flag-zero SPRING2 readable-delete after the main attempt remain unchanged. The test verifies exact literal bytes and repeat roundtrip, unselected payload preservation, missing selected ID, edited/reselected/swap/flag guards, cap refusal and post-open failure close-before-delete order. Its scope remains helper/page projection with explicit test combo IDs, not public Session/palette/native widget parity.

## Documentation item for root

One stale shared-header comment remains at `include/m2m/engine/simcontrol300_io.hpp:92-95`: it says the first native profile admits “no positive spring records,” which is now false. The new typed-page comments above it are updated, but this lower `makeNativeExtra3DIo` comment should be corrected before the final freeze. I notified root. This is a documentation blocker only; no parser/page source defect was found.

## Qualification

The original PE/Qt receipts establish stream read order, C locale, complete-row field order, absent-companion zero flags, no trailing extraction, list item ownership, initial row-zero fill, same-row Save writeback, all-row snapshot and writer order. The new 1..256 grammar, ASCII-only finite-token subset, cap, typed-page value policy and exact disk behavior remain candidate-only until differential original runtime validation. The public selected Session/palette refusal remains in force. Root reports CP118 full suite 306/306 with stable source; I did not independently build or execute tests.

Read-only review. No source/test edits, build/test execution, GUI control, original launch, or mutable REA operation.

Root has since corrected the stale “no positive spring records” header statement. The corrected text now says “canonical positive SPRING rows,” which I flagged as mildly ambiguous because “positive” should refer to the record count, not the signed row values; the parser explicitly accepts signed IDs and real values. This is wording only and does not change the no-blocker conclusion.
