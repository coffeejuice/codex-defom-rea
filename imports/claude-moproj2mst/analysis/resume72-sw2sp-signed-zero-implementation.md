# CP72 implementation: signed leading-zero SW2SP decimal

Implemented a narrow signed decimal admission for the reachable one-value `SW2SP.DAT` reader. When the one-int SW2SP profile is active and an ASCII `+` or `-` precedes multiple decimal digits starting with zero, the helper now lets the token continue through its existing decimal parser. That matches the original `QTextStream` order: C-locale sign recognition selects decimal before base-0 leading-zero octal selection.

The focused adapter test asserts `-010` → `-10`, `+010` → `10`, `-011` → `-11`, and `+008` → `8`. The last two show that sign-first inputs are decimal rather than octal. Existing unsigned `010` → 8 remains on its earlier explicit octal branch. Tests retain guards for unsigned `011`, a signed suffix token, signed overflow, another file path, and a multi-value SW2SP span. Grouping comma and other parser profiles were not changed.

Static preflight reviewed the one-value SW2SP gate, sign detection before leading-zero validation, the decimal `qt::toInt` range check, branch precedence relative to unsigned octal and exact overflow tokens, and the test guard matrix. No build or CTest was run; root owns integration validation.

Frozen SHA-256 values:

- `src/engine/simcontrol300_native_io.cpp`: `81D33FB963AE00561EE15F87CBAA7061C1DC2AD22BBC9AE9A6E3EB4308A4A9EC`
- `tests/mdt/test_simcontrol300_native_io.cpp`: `C8AFEA9C0354356DC9B86B1F11FE567091C9C4026ABDC1D4EB8992CBEF033321`

No CMake, shared state, GUI, or mutable REA files were changed.
