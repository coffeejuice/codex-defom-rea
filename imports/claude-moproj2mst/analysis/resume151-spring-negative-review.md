# CP151 independent review — canonical negative SPRING2 narrowing

## Result

No arithmetic or signed-range defect found within the count-one `-[1-9][0-9]*`, magnitude-through-`INT64_MAX` profile. Current reader/test match the implementation receipt `analysis/resume151-spring-negative-implementation.md`: reader SHA-256 `3D39D2234423D326139EBE3B8FBD2D8782ED8F93B6D6FD247589BB8C21A26FB3`; test SHA-256 `35736A964ABA90CBFF2346B1F390ADB994894097426BABED872F530221D04D28`.

## Value and overflow check

The existing exact `-2147483649` case runs first. The new branch is reached only after `from_chars(int)` range failure, only when the one-row profile flag is true, the prefix begins `-` followed by `1..9`, and the first delimiter is EOF or ASCII whitespace. It accumulates the magnitude in `uint64_t` with the checked condition `magnitude <= (INT64_MAX - digit) / 10`; this admits `INT64_MAX` and rejects larger magnitudes before multiplication/addition. It then computes `0u - uint32_t(magnitude)` and bit-casts the low word to `int32_t`. For the admitted interval, that equals the low 32 bits of Qt's successful 64-bit negation followed by the `int&` low-dword store.

The tested mappings are consistent: `-2147483650` → `2147483646`, `-4294967297` → `-1`, and `-9223372036854775807` at EOF → `1`. The exact prior `-2147483649` result remains `INT32_MAX`. CRLF/EOF acceptance and byte preservation are tested; the common cursor update leaves the delimiter unread (or cursor at EOF), as in the ordinary parser path.

## Guard and prior-branch preservation

The general multi-digit-leading-zero rejection runs before this conversion branch, so `-010` remains refused. The positive low-dword branch below it still requires a first digit `1..9`, preventing `+4294967297` or negative tokens from falling through. The new negative overflow arm is one-row gated; the two-row refusal table retains `-4294967297`. Tests also reject attached `x`, `INT64_MIN` magnitude, the next larger magnitude, and the separate 64-bit accumulation-wrap case `-18446744073709551615`. Exact `018`/`019` and one-row octal cases run earlier in the reader and remain covered; signed/radix and other malformed cases remain in the refusal matrix.

One scope point to keep explicit: `readSpringFlagInt` skips leading ASCII whitespace before setting `start`, so a one-row input with whitespace before an otherwise canonical negative token reaches this branch. That matches ordinary formatted-stream whitespace skipping, but the CP151 contract describes the token grammar without specifying whether leading whitespace is admitted, and the new test matrix does not cover it. This is not a demonstrated value bug; either preserve the existing skip behavior and add a test if it is intended, or tighten the new branch if the contract means byte-zero canonical spelling.

No source edits, build/CTest, GUI, original process, or mutable REA state was used.
