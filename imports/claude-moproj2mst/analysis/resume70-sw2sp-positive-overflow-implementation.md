# CP70 implementation: positive SW2SP low-dword overflow

Implemented the exact one-value SW2SP case selected in `analysis/resume70-sw2sp-next.md`:

- Input bytes: `2147483648\r\n`.
- Path/count gate: only `readInts("SW2SP.DAT", span<int> of size 1)` enables the exact branch.
- Result: `INT_MIN` (`(-2147483647 - 1)`).
- Cursor: the token is consumed and the line delimiter remains for the next extraction, matching the retained stream getter's unget delimiter behavior.

The branch shares the existing narrow SW2SP one-int gate with `-2147483649` → `2147483647`. It does not use a general low-dword conversion path. `2147483649`, other out-of-range negative values, suffix-attached tokens, multi-value spans, other filenames, and SPRING counts stay guarded. Existing group-separator and malformed-token behavior is unchanged.

The adapter regression test covers both source-qualified exact overflow tokens, the adjacent in-range negative edge, nearby out-of-range values, non-SW2SP path, SPRING count, suffix, and multi-value guards. Static preflight reviewed the source gate, exact string lengths/delimiters, return values, and unchanged guard order. No build or CTest was run; root owns integration validation.

Frozen SHA-256 values:

- `src/engine/simcontrol300_native_io.cpp`: `1A35C4DA9F38775927714215BABB32F9C078DCDB79296D183044E43058883BE7`
- `tests/mdt/test_simcontrol300_native_io.cpp`: `C12DC7D584400A7E2E6A02783D7FAE7870228C3197D2DCA25DFF9DBC6F8F2272`

No headers, CMake, GUI, or mutable REA state were changed.
