# CP146: bounded selected-editor palette REA query

The smallest static query is QPalette's default constructor at QtGui preferred address `6504abb0`, with assembly limited to `[6504abb0,6504ac26)`. It verifies the consumed data and branch, but cannot establish a selected runtime owner or atomic outcome. The smallest occurrence query that can answer CP143 is the **same selected constructor frame** at `QtGui+4abce` and `+4abd4`, with its getter/copy provenance. Do not report another decompilation as that runtime witness.

Only this report was written. No REA tool, session/target change, original execution, GUI/app control, source/test/CMake edit or build occurred. `graph_audit` retains exclusive REA session ownership. This plan is not authorization to launch or attach to an original process; any such acquisition requires its own reviewed capability/lifecycle admission. The root recovery status still records the GUI release-lifecycle blocker. No reconstructed executable/deployment was inspected.

## Verified target identity

Files are under `C:/Users/alext/Projects/codex-deform-rea/imports/claude-moproj2mst/evidence/original-binaries/`. Hashes and PE preferred bases were recomputed locally, without REA:

| Original file | SHA-256 | Preferred base |
| --- | --- | --- |
| DEF_GUI_MO_64.exe, v11.0.2 | `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` | `140000000` |
| QtGui4_64.dll | `c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887` | `65000000` |
| QtCore4_64.dll | `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160` | `67000000` |

Use the exact original DLL identity for the palette query, not whichever target happens to be active. After the session owner releases it, select a verified existing target if available. Otherwise the future owner may use `open_binary` for the exact local QtGui path according to the connected provider's schema. Preserve the target/evidence identity. Do not reopen or replace graph_audit's active target. No provider IDs or unverified tool parameter shapes are prescribed here.

## Minimal static selectors, with a stop rule

Prefer retained evidence when it answers the question. The first new focused function query, if a provider-backed confirmation is required, is `analyze_function` for exact preferred address `0x6504abb0` (RVA `0x4abb0`) and its bounded assembly. Do not request a binary overview, graph-wide search, all callers, recursive callees, or a whole constructor dossier.

| Priority | Exact selector | Question/output bound |
| --- | --- | --- |
| 1 | QtGui `[6504abb0,6504ac26)` | Identify local T, destination P, pointer store, imported deref, AL branch, destruction leg and destination word/ref tail. One complete 118-byte assembly window; focused pseudocode only if it preserves both branches. |
| Conditional | QtGui function `65016f70`, range `[65016f70,65016fd4)` | Only if source selection needs provider evidence: slot RVA9c9690; known-present path to copy at16fc6; lazy-null allocation/initialization kept distinct. |
| Conditional | QtGui function `6504a8b0`, range `[6504a8b0,6504a8fd)` | Only if data passthrough/retain provenance is missing: O→D→T and getter retain once. |
| Conditional | QtCore bytes `[67001ac0,67001ac8)` | Only if deref import semantics need confirmation: raw32 LOCK ADD -1 / SETNE. It has no PE unwind entry; do not invent a function boundary. Resolve QtGui IAT entry6596978 to this exact target. |

The constructor's PE unwind region ends at `6504abd8`; this is **not** its complete logical boundary. A provider returning only that region omits the zero/destruction and tail legs. The explicit118-byte window avoids treating the unwind fragment as the whole function. Getter/copy unwind bounds match the listed byte windows but do not themselves prove logical completeness.

Stop static expansion when these edges agree with CP143. They already establish no detach between getter source D, returned T and private P, and the deref-conditioned destruction branch. Broader xrefs or unrelated application palette consumers cannot provide the selected data value/count. Do not expand into the null-slot initializer `6504bf80` unless the selected occurrence actually establishes null; keep that profile guarded.

## Exact same-occurrence selector

An occurrence query must select the actual Project/Operation/editor/private frame, rather than a global arbitrary QPalette construction. Reuse the retained selected chain: EXE `+eb26f0` editor constructor → `+130740` → `+377950`; QtGui `+70d70` QWidget → `+5e780` private constructor. At `QtGui+4abb0`, require return address `+5e8d9` and P==that selected private+170. Retain process start/generation, loaded module SHA/base, thread and per-invocation frame token, including reentrancy distinctions. Historical editor004 PID27216 is not a live process selector.

With that frame selected, collect only these bounded facts:

| Site | Required facts |
| --- | --- |
| Getter `QtGui+16f70` | Caller is default constructor return+4abc3; output address is T. Record the actual global slot at loaded QtGui base+9c9690 and its branch, without calling the getter from instrumentation. |
| Copy `QtGui+4a8b0` | Caller return+16fcb, destination RCX==T, source O=RDX, D=*O. If the existing full-value registry API is used, record O+8 too; that word is not consumed by the private data-only slice. |
| Before imported deref `QtGui+4abce` | RBX==P; T is this default-constructor frame's RSP+20; require RCX==*T==*P==D. Record raw uint32[D], allocation/data generation and relevant mutation sequence. The getter+1 and destination pointer store already occurred. |
| Returned deref `QtGui+4abd4` | Record AL for this same frame/D and actual branch. Preserve supported execution-exclusion or relevant atomic/interleaving evidence. A nearby unsynchronized memory read cannot prove the atomic input/outcome relation. |

For this profile, two final edge observations plus the selected getter/copy provenance are sufficient; a global ref-hook transcript is unnecessary if supported exclusion establishes the same atomic sequence. If exclusion is unavailable and concurrent mutations are not retained, preserve that limitation and refuse an exact counter-state admission. Actual AL establishes the executed branch; it does not retrospectively establish a sampled raw word as the atomic input.

Do not change memory, invoke Qt methods, force a branch, seed a count or issue retain/release operations to obtain a convenient outcome. Do not infer nonzero from normal constructor return. Raw32 input1 derefs to zero; wrap/negative-bit patterns retain native arithmetic rather than a fabricated positive-count guard.

## Acceptance and candidate admission

Accept a **static edge confirmation** only with the exact file identity, selected bytes/import target and complete branch coverage. Mark O, D, current native count and occurrence outcome unknown unless an actual selected receipt supplies them.

Accept a **selected palette witness** only when:

- The Project/Operation/editor/private selection, thread and invocation chain match; the same P/T/O/D equalities hold at the relevant edges.
- The raw word observation is qualified at the consumed sequence, already including getter retain, and its relation to the native atomic outcome is established or explicitly left pending.
- The source D/allocation generation is joined to the same actual Session/runtime canonical PaletteDataState owner. Neither a historical pointer nor startup alias1 is that join. Reject another runtime, another receiver or reused Operation identity.
- Absent/null/present slot cases remain distinct. The retained present-slot path does not silently qualify lazy initialization.
- Host token copies/destruction have no native effect; late observation/refinement does not replay getter+1. Unknown deref preserves the already-stored partial pointer and held token. Zero commits destruction-pending; nonzero authorizes only its separately recovered tail. No early PaletteConstructed/word0/font/counter/setup completion is inferred.

The first useful integrated implementation after admission is CP143's retained EditorBase getter-token/partial-slot cursor, with the narrow Session pending-stage expectation updated coherently. It is not public constructor or Save completion. Keep `PrivatePaletteUnavailable`, loader6504abb0 and Save140eb5180 until the respective prerequisites are actually closed. Do not route this through a SplitView placeholder receiver or enable private pages as an owner substitute.

## Receipt and stopping point

Locally recomputed half-open ranges:

| QtGui range | SHA-256 |
| --- | --- |
| 6504abb0..6504ac26 | `704806e2a08c3327f0cb7f8d0ca7c496af0c69dbd897d456e48713cf1fa6a399` |
| 65016f70..65016fd4 | `c25e3f1aa850624ddd48c4f9523c292a873f9573fc1c338462fc6c86a8035b25` |
| 6504a8b0..6504a8fd | `a0c82e2e8037dc55d2034c9b82ad3ae911f3c4af2e740438ecfbe4e9970cc03b` |

Hook-byte anchors rechecked:4abb0=`40534883ec30488bd9`;4abce=`ff15a4bd6400`;4abd4=`84c07535`;16fc6=`e8e5380300`. Use loaded module base+RVA, verify bytes before any later observation, and preserve complete Evidence records plus compact selector/hash/qualification receipts. Query and release operations belong to the future sole REA owner; this lane made none and claims no release receipt.

CP143 report SHA-256 is `1642bda232143fc3cf3b3bae9a5d3fe41c503e6f34213535620a81c1b60b2ccd`. Editor004 remains wrapper-only at selected D: it cannot satisfy the new acceptance criteria. If the next admitted REA surface offers static analysis only, perform at most the needed priority1 confirmation, report the occurrence still missing, and stop this query lane without broadening the search.
