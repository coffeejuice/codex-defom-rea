# CP151 leading-space behavior addendum

The CP151 negative family also accepts ordinary leading ASCII spaces because original `QTextStream::getInteger` skips leading whitespace before it reads a sign or digit. The byte-matched Qt capture `analysis/wave2-save-diffs-qt-textstream-getinteger-complete.asm` shows the leading scan at `0x670c5280–52b9`: it calls `QChar::isSpace()` at `0x670c52a0`, continues over whitespace, and exits the scan on the first nonspace character before sign/radix parsing. The same Qt getter is used by the SPRING2 reader. This is static PE/source evidence, not an original-process test.

The candidate `readSpringFlagInt` independently skips its supported ASCII whitespace before computing the token start, so the assigned negative branch naturally handles this source-qualified spelling. Added the count-one regression ` -4294967297\r\n` -> `-1`, with exact input-file byte-preservation checks. This confirms the effective contract is optional supported leading ASCII whitespace followed by the canonical negative token; non-ASCII whitespace remains outside the candidate's ASCII decoder profile.

No reader code changed. `git diff --check` passed for the focused test; no build or CTest was run.

SHA-256:

- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp`: `EC11894DD863BA9E53E237ABC5EB5CA1108FB3337994FBD7DAA63D2B422F2524`
- `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp` (unchanged): `3D39D2234423D326139EBE3B8FBD2D8782ED8F93B6D6FD247589BB8C21A26FB3`
