# CP280/CP281 static color PE byte gates

## Scope and method

The read-only validator checks four fixed direct `CALL rel32` sites and the separate 16-byte prefix at the 3D helper entry. It reads the frozen `evidence/original-binaries/DEF_GUI_MO_64.exe`, hashes its bytes, parses PE headers and section mappings, and checks exact bytes in raw-backed executable sections. The four calls additionally receive calculated signed-`rel32` targets and return PCs. The prefix check does not calculate a target or return PC. Every mapping requires the complete span to be raw-backed by exactly one section; boundary-crossing and overlapping raw mappings are rejected. The tool never starts the image or invokes GUI, REA, debugger, Frida, or any runtime tooling. Negative fixtures are in memory; the PE file is never written.

## Result

Expected and observed SHA-256: `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` (67,611,136 bytes). The PE is AMD64 PE32+ with image base `0x140000000`. The four five-byte call sequences, targets and return PCs match as previously recorded. The fifth distinct static span is the helper-entry prefix:

| Kind | Address/RVA | Expected bytes | Result |
|---|---|---|---|
| 3D helper entry prefix | VA `0x1411f26e0`, RVA `0x11f26e0`, length 16 | `48 89 4c 24 08 53 48 81 ec c0 00 00 00 48 8b 05` | exact match in executable `.text`; no target/return calculation |

Validation result: `validationPassed=true`; `readyForIndependentReview=false`. Focused tests: `python -m unittest discover -s admin -p test_color_pe_byte_gate.py -v`; 7 tests passed, covering the exact immutable image, negative call and prefix byte mismatches, call and prefix section-boundary rejection, overlapping section mappings, and a truncated DOS header. The CLI reports the same image hash and all five static span checks.

## Review repairs

The independent reviewer found that the original mapper could accept a five-byte call whose tail crossed a raw section boundary or select the first of overlapping mappings. The mapper was generalized to take a span length and now requires exactly one section to back the entire requested span. CP281 adds a separate 16-byte helper-entry prefix check at RVA `0x11f26e0`; it shares only the raw-span mapping rule and remains distinct from the four CALL checks.

## Limits

This establishes only the current immutable image hash, four direct call encodings/targets/return PCs, and one 16-byte helper-entry prefix. It does not decode the prefix into instruction boundaries, establish broader call ordering or reachability, resolve dependencies, prove loaded-module identity/base, cover control flow or observer intervals, establish runtime behavior, or authorize original execution. It makes no runtime claim and is not an original-run readiness result.
