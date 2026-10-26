# CP81 SW2SP octal prefix before invalid digit

Implemented the exact `018\r\n` one-value SW2SP case as integer 1. The original base-0 octal loop accepts the `1` after the leading-zero prefix, then sees `8` as an invalid octal digit and ungets it. Because an octal digit was already accumulated, the getter succeeds with value 1; the outer reader requests one integer and does not consume a second token. Current candidate's general integer profile rejected this leading-zero token.

The new branch requires exact bytes `018\r\n` through EOF and the existing one-value `SW2SP.DAT` profile. Its focused test asserts value 1 and preserves other-path and multi-value guards. It does not generalize partial octal prefixes, arbitrary suffixes, other invalid octal digits, or subsequent extraction behavior.

Evidence: retained original QtCore SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`; `analysis/wave2-save-diffs-qt-textstream-getinteger-complete.asm` base-8 loop at `0x670C57CC`, digit check and update at `0x670C5832–5852`, unget of invalid character at `0x670C585D–5860`, nonzero-digit success at `0x670C5964–5979`; operator success assignment at `analysis/wave2-save-diffs-qt-textstream-read-int-complete.asm`; one-value SW2SP caller at `analysis/wave2-doe-sim300-extra3d-sidecars-decompile.json` and `analysis/resume67-sw2sp-locale.md`. This is static evidence, not an original-process runtime comparison.

Static preflight reviewed the exact-token scope and tests. `git diff --check` produced no diagnostics. No build or full suite was run; root integrates.

Frozen SHA-256:

- `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp`: `A871E2FD427D6366812AD0C34B46C2CE4C830C2AFDE3FE1067F77301360BEE7E`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_native_io.cpp`: `C73DB2920A705E81AF366BD983CEC247584F8D1F0B5E306B5F791F3190E6ACD1`
