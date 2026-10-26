# CP76: next exact SW2SP base-0 token

## Selected input: unsigned octal `02`

The next narrow single-value witness is exact bytes `02\r\n` in `SW2SP.DAT`. The retained original getter parses it as base-8 value 2. The current candidate has only the exact `010` -> 8 case; it rejects `02`, so this is a concrete reachable Open-path divergence.

The relevant parser is shipped QtCore `QTextStream::getInteger`, not a CRT or Fortran formatted-input routine. The original Extra3D sidecar reader constructs a QTextStream and extracts an `int`; this route is statically bound in `analysis/wave2-doe-sim300-extra3d-sidecars-decompile.json` and discussed in `analysis/resume67-sw2sp-locale.md`. No CRT/Fortran numeric rule is needed or asserted for this field.

## Source qualification

- Retained QtCore identity: SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`.
- In `analysis/wave2-save-diffs-qt-textstream-getinteger-complete.asm`, base-zero parsing dispatches a leading `0` to the octal path at `0x670C57CC`. That path reads the next character, accepts decimal digit values 0 through 7 (`0x670C5832–5848`), and accumulates `value = digit + 8 * value` at `0x670C5848`. For `02`, the accumulator is zero for the initial prefix and then computes `2 + 8*0 = 2`; CR ends the digit sequence and is ungot as delimiter. At least one octal digit was accepted, so the getter completes via its parsed-number path. The `int&` operator stores the resulting value on the success branch.
- `analysis/resume56-extra3d-octal-increment.md` records that `010` -> 8 was implemented from this same base-8 multiply/add path and explicitly lists `02` among the still-guarded adjacent octal examples.
- `analysis/wave2-doe-sim300-extra3d-sidecars-decompile.json` shows the sidecar reader initializes the output, reads one requested integer, and returns true after successful QFile open. `analysis/resume69-sidecar-int-next.md` binds the one-value SW2SP call to the reachable Extra3D Open flow.

## Minimal candidate-only test proposal

Write exact bytes `02\r\n` to `SW2SP.DAT`, call `readInts` with a one-element destination, and assert success with value 2. Retain the existing path/count gates and leave other octal values, radix forms, overflow, malformed-token status, and repeated extraction outside this exact witness. No original-app runtime comparison is claimed; this is static QtCore evidence.

No source, test, build, GUI, or mutable REA state was changed for this audit.
