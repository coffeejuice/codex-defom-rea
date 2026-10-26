# Resume 203: converter format, encoding, numeric, and path contract

## Scope and result

Read-only, converter-only review of the retained original evidence and current tree. No production changes, build, original execution, GUI input, or mutable REA operation was performed. The narrow conclusion is that the current evidence does **not** support a source-ready parity fix for RFORMAT parsing/encoding/numeric conversion. The existing ledger correctly leaves the external converter path unimplemented and uncharacterized.

## What the evidence establishes

`analysis/recovery-feature-ledger.json` entry `converter_inputs` lists six command choices (`IGE`, `ANS`, `STL`, `PDA`, `STB`, `UG2`) and says the DEF_PRE dispatcher invokes `FUN_1402240d0`, then tests for `SCRATCH.KEY` and reads that file if present. It explicitly qualifies this as a converter-command boundary, not six parsers inside DEF_PRE. `analysis/keyword-dispatch.json` is tied to the retained original `source/DEF_PRE_64.exe` SHA-256 `f78a2a5c3ebb3c1affca3d2d0ea31f0ed802f6fe1e9083168681502ac6cea794`; its referenced strings bind the six command literals and `SCRATCH.KEY` to the dispatcher. This is enough to establish the command names and the downstream existence/read gate.

The current source has no `RFORMAT` implementation or converter parser: a scoped search of `src` and `tests` finds no RFORMAT symbol (the sole unrelated textual hit is a geometry fixture phrase containing “format”). No retained evidence inspected here identifies RFORMAT's record grammar, character encoding, numeric lexical rules/rounding, path resolution, child exit handling, or output lifetime. In particular, the DEF_PRE call site cannot determine what `DEF_CVT3` does internally.

## Gap and acceptance target

The actionable gap is **missing evidence at the converter boundary**, rather than a justified numeric or encoding behavior to implement. The exact next acceptance should be a bounded static contract for the authorized `DEF_CVT3` artifact (identity and SHA-256 first), covering one selected input format end-to-end: invocation arguments and working directory; path quoting/resolution; relevant text encoding; numeric grammar and conversion; `SCRATCH.KEY` create/truncate/retain behavior; and child failure/missing-output behavior. Bind each claim to the converter's own code/evidence. Then add fixtures for the observed encoding and numeric edge cases and compare exact generated KEY bytes plus the caller's observed status. Until that artifact and evidence exist, inventing RFORMAT rules or mapping converter formats into DEF_PRE would exceed the established contract.

## Evidence limits

The dispatcher evidence is static and proves only its local call and subsequent file check/read. It does not prove child success, blocking semantics, an absolute executable path, active-code-page behavior, locale, numeric rounding, or whether a converter is installed/authorized. No runtime observation or companion converter identity was available in this review. These remain explicit unresolved items rather than inferred parity requirements.
