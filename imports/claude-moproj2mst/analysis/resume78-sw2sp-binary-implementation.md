# CP78 SW2SP exact binary token

Added an exact `0b10` -> 2 branch to the SW2SP integer reader. It is gated by the existing one-value `SW2SP.DAT` profile and accepts only EOF or ASCII whitespace after the token. The focused test uses `0b10\r\n` and keeps `0b11`, `0b2`, uppercase `0B10`, and a two-value request guarded. Existing `010` and `02` octal cases remain covered in the same radix test.

The source qualification is documented in `analysis/resume78-sw2sp-next.md` and the retained QtCore capture `analysis/wave2-save-diffs-qt-textstream-getinteger-complete.asm` (original QtCore SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`). The base-zero getter selects lowercase `0b`, accepts binary digits, accumulates `2 * value + bit`, and ungets the CR delimiter after the final bit. This remains static evidence, not an original-process runtime comparison.

Static preflight reviewed the one-value path gate, exact token and delimiter condition, and focused negative boundaries. `git diff --check` completed without diagnostics. No build or test suite was run; root owns integration validation.

Frozen file SHA-256:

- `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp`: `D80CB299D070C096A5E8FD6578DE5975B138C3C9D1C56E6C4501DD88A72903B1`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_native_io.cpp`: `0A89C5A8E2939358A6F12AE95DC3A81F8325D3501F2546473A72A288F662BCA8`
