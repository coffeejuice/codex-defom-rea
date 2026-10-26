# CP157 negative decimal overflow review

## Conclusion

The proposed one-row negative-overflow increment is source-ready, provided it uses the exact conditional sign rule from the retained Qt audit rather than extending the existing capped `INT64_MAX` accumulator by unconditional negation. No parser edit was made; the current source remains at its CP157 BOM freeze (`simcontrol300_native_io.cpp` `8C1A89B9390E6803E38E66A7BB1B9742A6F87DF7A48433C80CFC90253D92B628`, SPRING reader test `677005EE55152D933F5108DDBB0353F162E0B8E813DE13DA2264A913DC89A079`).

## Arithmetic contract

The byte-qualified Qt getter accumulates decimal digits in a 64-bit register with wrapping multiply/add. For a negative sign, completion tests the wrapped accumulator as signed: it negates only when nonzero and its high bit is clear. When zero or high-bit-set, it leaves the bits unchanged. The int operator then stores the low dword. Therefore the implementation must keep a `uint64_t` accumulator, and conditionally replace it with `0 - accumulator` only if `(accumulator != 0 && (accumulator >> 63) == 0)`, then reinterpret the low 32 bits. The following values discriminate the branches correctly: `-2^63` -> 0; `-(2^63+1)` -> 1; `-(2^64-1)` -> -1; `-2^64` -> 0; `-(2^64+1)` -> -1. The existing `-4294967297` -> -1 remains a useful control.

This transformation is defined in unsigned arithmetic and avoids signed overflow. It matches the corrected static Qt table at `analysis/resume83-sw2sp-parser-independent-review.md`; no new runtime comparison is implied.

## Admission and guards

The proposed grammar `-[1-9][0-9]*` with only EOF/ASCII-whitespace termination is appropriately narrow. The current parser reaches overflow alternatives only after the `from_chars(int)` conversion fails. Its present negative branch requires the count-one gate, canonical nonzero first magnitude digit, and complete-token terminator; it currently caps magnitude at `INT64_MAX`. Replacing only that cap/accumulator path with wrapping `uint64_t` and conditional sign application preserves separation from:

- signed leading-zero tokens handled by the CP153 path;
- the exact `-2147483649` narrowing witness;
- the positive-sign path, which does not conditionally negate;
- malformed attached suffixes, which fail the terminator test and fall through to refusal;
- multi-row calls, which lack the count-one admission flag;
- radix prefixes, non-ASCII bytes, and BOM-less codec cases.

The `-INT64_MIN` edge is not an exceptional arithmetic case in this algorithm: its wrapped magnitude is `0x8000000000000000`, so the high-bit test deliberately skips negation and produces low dword zero.

## Test coverage recommendation

The proposed matrix covers the necessary transition points and byte-preservation behavior. Add an explicit count-two negative-overflow refusal fixture using one of the new tokens (for example `-9223372036854775809`) and a dedicated one-row attached-suffix case (`-18446744073709551617x`) if the implementation changes the former overflow guard. The existing count-two `-4294967297` refusal and general suffix gating are useful but do not directly test the newly admitted magnitude range. These are test-strengthening items, not blockers; the current ownership gate and suffix condition are source-evident.

## Limits

Read-only review only; no build or CTest was run. The claim is bounded to count-one canonical ASCII negative decimal magnitudes with complete termination and the already-qualified decoded-byte path. Qt status/cursor behavior for malformed tokens, arbitrary codecs/Unicode, other filenames, and multi-value retries remain outside scope.
