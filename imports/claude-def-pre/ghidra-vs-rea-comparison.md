# Headless Ghidra vs REA: function recovery comparison (2026-10-21)

Target: `DEF_PRE_64.exe` (copies only). Each arm/target ran as a fresh, blind helper. No arm could read
the repo, docs, notes or prior outputs, and neither arm had a DB file. Arm A used only headless Ghidra
scripts in its own project. Arm B used only REA MCP tools, on its own exe copy in REA's ephemeral
project. The arms ran one after the other, alternating which went first per target (A/B, B/A, A/B).
Scoring: each arm's `arm.py` was run on a copy of COG_P1_B1.DB and compared with deformdb output by
`judge.py`. The judge was first self-tested 140/140 with a reference implementation written from the spec.

## Scorecard

| Target | Arm | DB tests | Wall time | Tool calls | Tokens | Tool problems |
|---|---|---|---|---|---|---|
| Setup | A: import + auto-analysis | – | 77 s | – | – | none |
| Setup | B: open_binary + analysis | – | ~100 s | – | – | none |
| RLE decoder `FUN_14044aeeb` | A | 101/101 | 125 s | 9 | 85 k | FillGaps.java broke script compile; FuncRefs failed |
| | B | 101/101 | 95 s | 10 | 109 k | none |
| Step directory `FUN_1403b92bc` | B | 32/32 | 250 s | 33 | 205 k | none |
| | A | 32/32 | 387 s | 25 | 135 k | StrRefs.java name clash; extra -scriptPath not passed through |
| Step read `FUN_1403b6679` | A | 7/7 | 689 s | 31 | 195 k | one bad script argument |
| | B | 7/7 | 604 s | 38 | 291 k | none |
| **Total** | A | 140/140 | 1278 s | 65 | 415 k | 6 helper scripts written |
| | B | 140/140 | ~1049 s | 81 | 605 k | 0 |

## Spec quality (checked against docs/db-format-spec.md)

Both arms recovered the same facts, with addresses:
- the RLE format (link/length trailer, backwards walk, early exit);
- the 512-byte big-endian records and the always-on swap flag;
- the index chain at record 3, with 127 page pointers plus a next link, and pages of 128 × 13 words;
- the first-match step lookup and errors 201/202;
- the 2D > 3109 / 3D > 999 / ≥ 6000 region variants and the 4-word region table;
- the op ≤ −30 / ≥ 30 single-region selectors.

Neither arm identified directory word 1 (MeshNo), word 10 (dynamic-array record) or the region names.
Both were unsure whether RECL counts bytes or words.

Both arms found one fact that docs/db-format-spec.md does not state: after a full region read, the final
array lengths are set to NINT(header[7]) / NINT(header[8]). Arm B also found that the last real region
may be decoded through a temporary heap buffer.

## Conclusions

- **Accuracy is a tie.** On these three functions, either toolchain plus a model recovers code that is
  byte-exact against real data. The tool is not the bottleneck for correctness.
- **REA was faster on every target**, by 24 %, 35 % and 12 %, with zero tool failures. Its call graph,
  callers, xrefs and batch decompile work out of the box. Arm A lost time to tooling: two broken shared
  scripts, a script-path quirk, a JVM start per query, and six ad-hoc scripts.
- **REA costs about 45 % more tokens.** Its results carry verbose evidence and provenance JSON.
- **REA limits on Windows:** the project is ephemeral and read-only, so renames and comments don't
  persist. It has no bulk scripting either, such as whole-closure dumps, all-function string or immediate
  sweeps, or table dumps to file.
- **Caveats:** there was one run per cell, so model variance is not measured. Arm A's step-read helper
  inherited scripts written by its earlier helpers, which slightly favours A.

## Rule adopted

- Use REA for interactive recovery of one function or one call chain: decompile, callers or callees,
  xrefs and reading bytes.
- Use headless Ghidra (deform-rea-work scripts) for bulk or persistent work: closure decompiles,
  program-wide searches, table dumps, and anything that needs saved names.
- The broken shared scripts are worth fixing: `FillGaps.java` does not compile, and `StrRefs.java`
  clashes with `Listing.java`.

Artifacts (scratchpad): `cmp/armA|armB/work/t1..t3/{spec.md,arm.py}`, `cmp/judge/judge.py`, `cmp/judge/metrics.txt`.
