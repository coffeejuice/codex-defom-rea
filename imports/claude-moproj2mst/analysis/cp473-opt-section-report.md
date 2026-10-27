# CP473 Type3 `.OPT` count and region insertion literals

This receipt binds seven fixed literals used by the original Type3 `.OPT` writer to the retained executable and confirms their reference order in the saved CP465 writer disassembly.

The original executable is `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. The bounded CP465 assembly is `analysis/cp465-type3-opt-save-140907a10.asm`, SHA-256 `c188feb9a5cf880507f6b1fa7b4e78f833e547cc6b8f6540ad613f4b19e9e866`; CP465's full writer byte comparison reports 3,095 bytes, 577 instructions and zero mismatches. The verifier maps literals through PE sections, checks every byte including NUL and checks the individual SHA-256 values. All seven values matched (77 bytes total).

The count path calls helper `0x140004ae0`, inserts the resulting count with the QTextStream integer insertion, then iterates from 1 through that count. For each index it inserts the numeric index and then the fixed literal at `0x1427ba038`. That literal is a spaced fixed string (`   0.0  1.0  0.5  0.01   0`), not a collection element.

The region path obtains the row count through slot `+0xc0`. For a positive count it inserts `REGIONS    \t\t`, then iterates rows through slot `+0xd8`; the visible construction references `.GEO`, `/`, and `Opt` in that order before emitting the constructed row. For a nonpositive count it inserts `REGIONS\t\t0`. These are static call/reference order findings; the receipt does not claim the helper calls produce any particular runtime string.

The receipt does not establish QTextStream codec or numeric formatting, buffering, flush success, QFile text translation, write/close failures, or physical file bytes. The concatenation helpers' complete behavior is not included. No original process, GUI, build, test, or mutable REA session was used. Existing CP470 files were not modified.

Reproduce with `python analysis/cp473-opt-section-verify.py`. The script writes `analysis/cp473-opt-section-byte-verification.json`.
