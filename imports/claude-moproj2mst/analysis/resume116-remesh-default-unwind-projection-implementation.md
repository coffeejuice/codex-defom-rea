# CP116: isolated original-handler unwind projection

This increment implements the pure metadata projection proposed in `analysis/resume116-remesh-invalid-parameter-occurrence.md` (SHA-256 `4898714fe1dc046f7d4eef2007b45fd995c0bf608f5a1a380d10ac0fdc567173`). It derives a bounded original-frame hypothesis from explicitly supplied numeric inputs. It does not capture native context, read a process stack, call an unwinder/filter/termination API or activate a public handler route.

Only three new private files were added. All existing source/interfaces/models, CMake, fixtures and build files were left unchanged:

| File under `source/apps/moproj2mst` | SHA-256 |
| --- | --- |
| `src/engine/remesh_default_unwind_projection.hpp` | `bd998b53fdec986da212ff5c5944497dd85c2361075922d509ba6c9c641633f3` |
| `src/engine/remesh_default_unwind_projection.cpp` | `130f911ff0d08f9d22f4f63cf09b2f9df972752ddc5653a8d8e3ecd8124219c5` |
| `tests/mdt/test_remesh_default_unwind_projection.cpp` | `1ef84ae99ebf115dcc9f69e28d2799f8107bdc01a87b0490eddb0106bf99b699` |

## Exact admission and results

`projectOriginalDefaultHandler` requires captured PC=`0x141470485`, explicit `UnmodifiedBody` stack-state hypothesis, and a supplied nonzero16-byte-aligned body RSP. Address arithmetic must accommodate the entire `0x5f0` virtual frame result without uint64 overflow. This is a narrow numeric domain check, not authentication of the stack-state enum or a native memory-access validator.

Runtime-entry admission requires both exact twelve-byte records: `400447016505470198c04703` for RUNTIME_FUNCTION and `010903000901bc0002300000` for aligned UNWIND_INFO, plus saved-RBX and caller-return words. A read-only independent PE mapper compared both source arrays against original `DEF_GUI_MO_64.exe` and confirmed byte equality. Original hash, runtime-entry/unwind hashes and instruction provenance are in the evidence report.

The function decodes begin/end/unwindRVA, allocation0x5e0, version1/flags0/prolog9/codeSlots3/frameReg0/pushed-register3. With body RSP=`B`, supplied words represent saved RBX at `[B+5e0]` and original caller return at `[B+5e8]`. Output restores the supplied RBX, sets caller RIP and exception address to the supplied return, and computes caller RSP=`B+5f0` (reverse allocation, pop saved register and simulate return). Supplied zero words are valid numeric hypotheses and remain distinct from absent inputs.

`ExplicitNoEntry` is a separate hypothetical lookup result. It requires absent runtime metadata to reject contradictory input, preserves original fallback caller RSP=`B+5e8` without +8, and produces no restored-RBX or decoded-metadata claim. The original PE statically has an entry at the qualified PC; this branch does not claim that entry is missing or that an original lookup returned null.

Every refusal contains no projection. Other PCs, unknown/modified stack states, missing/unaligned/overflowing body RSP, unknown lookup enums, absent/changed metadata, contradictory no-entry metadata and missing required words stay explicit guards. Numeric addresses are never dereferenced. The result is not a full CONTEXT or process outcome, and no existing model consumer is changed.

## Focused tests and static verification

Six Release-active cases check:

1. Exact original function/frame/register metadata decoded against literal expectations.
2. Distinct supplied RBX/return words and caller projection, including present zero words.
3. Runtime versus no-entry RSP difference8, original exception address and absence of a fictitious fallback RBX restore.
4. Refusal after changing each of the12 bytes in either record, plus absent records.
5. Unsupported PC/state/lookup, missing words and arithmetic/alignment guards on both routes.
6. Repeated pure evaluation with host errno0/EDOM/ERANGE preserved.

`python admin/check_test_source_preflight.py source/apps/moproj2mst/tests/mdt/test_remesh_default_unwind_projection.cpp` passed with no issues. Main/run_all, supported Testkit assertions, direct quoted includes, masked delimiter balance and whitespace checks passed. No C assert or unsupported assertion macro is used. The source constants were independently compared with PE bytes as described above. No compile, build, C++ test execution, original/candidate launch, GUI or mutable REA operation occurred.

Root integration requires configure to discover the existing source/test globs; no shared CMake change is needed. Target `m2m_mdt_remesh_default_unwind_projection`, CTest `m2m.mdt.remesh_default_unwind_projection`, six cases. Root owns grouped build/focused execution/full source-stable freeze. Source is held at these hashes pending that validation.

This closes only the bounded metadata arithmetic and native branch distinction. Actual captured RIP/RSP, complete unwind context, OS lookup/filter behavior, original enabled-remesh occurrence and native terminal process observation remain unresolved. Candidate endpoint/CLI relay qualifications remain unchanged. Token usage was unavailable.
