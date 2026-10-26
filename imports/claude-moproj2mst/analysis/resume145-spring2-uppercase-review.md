# CP145 independent review — uppercase `1X\r\n` SPRING2 case

## Review result

No blocking defect found in the frozen implementation or focused test for the bounded count-two profile. The source and test hashes match the CP145 freeze: `simcontrol300_native_io.cpp` is `35B209095822E0FCC4692AC9D7B8E484813311473790B451FD439106777C2536`; `test_simcontrol300_spring_reader.cpp` is `A00336A8608D5325EE8893E78657810950BBDD182ED16CA15101AAB41066B0B5`.

## Source and byte-order check

The `SPRING2.INI` exception is admitted only when primary count is 2 and the complete companion bytes equal `1X\r\n` (`simcontrol300_native_io.cpp`, companion profile setup around lines 513–523). For that call profile, the first integer extraction consumes the decimal prefix `1`, recognizes `X` as a permitted unread suffix, leaves the cursor at `X`, and returns 1. The second requested flag sees the same `X`, returns 0 without advancing, and the count-two loop ends. The public row flags are therefore `[1,0]`; no internal QTextStream status is exposed or claimed.

The uppercase allowance is threaded to the one `readSpringFlagInt` callsite. It is gated both at profile creation and in the token branch. Other integer parsing remains unchanged. The original source qualification in `resume145-format-failure-edge.md` supports the ASCII non-digit behavior and ignored stream status; it does not depend on uppercase hex-prefix semantics.

## Focused test and nearby negatives

The test uses the existing complete two-row primary fixture, checks returned row count and flags for `1X\r\n`, and checks that both `SPRING.INI` and `SPRING2.INI` retain their original bytes. It also preserves the lowercase `1x\r\n` regression. The negative cases are effective: `1X 2\r\n` and `1XX\r\n` refuse with the two-row primary; exact `1X\r\n` refuses with a complete one-row primary and with a complete three-row primary. The companion bytes are checked unchanged after these refusals. These cover the exact-byte, wrong-suffix, and wrong-count edges called out by the proposal.

No implementation defect requires a patch. The contract is intentionally narrow: no inference is made for whitespace variants, other uppercase suffixes, status reporting, or a native-process differential. Existing test stays a private reader test. No build/CTest was run because root owns grouped validation; no source edit, GUI control, original process, or mutable REA state was used.
