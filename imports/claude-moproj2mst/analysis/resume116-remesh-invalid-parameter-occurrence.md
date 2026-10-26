# CP116: original invalid-parameter occurrence and next bounded model

The original PE statically connects an enabled-remesh unchecked fopen-null to its module CRT invalid-parameter dispatcher, context/unwind/filter prelude and a TerminateProcess argument of `0xC0000417`. No retained original runtime capture in this lane establishes that complete occurrence on the constructed B133 overlay. The candidate worker and CLI terminal statuses remain observations of their fixed endpoint/relay, which omit the native prelude.

The useful new static increment is the original default handler's exact runtime-function/unwind metadata. It permits a narrow pure caller-context projection from explicit hypothetical inputs, without calling or claiming native context capture.

## Identity and current model binding

Read-only original `evidence/original-binaries/DEF_GUI_MO_64.exe` independently recomputes SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, preferred image base `0x140000000`. Addresses below use that preferred base; a runtime witness must account for its actual load base.

The current pure model files still match CP112's recorded hashes: header `2783a621840c1a0f92392df1e97b0d04b649be9d3230e5a611194ea3991c226c`, source `4ccd70b1ad233ebc9cdbf8fdf0761b789d072a1ee63d38742f6ae1d61218d01a`, test `a3c8149475bf26411b5a6e97c3e0d6aa50aa406c0b3c52789939e7ab640fc34f`. CP112 report hash is `67b074b348519b27adff6d3e925f78cdbaff3d827eb5591204171e0ee2743a97`. This lane checks those live bytes, not a new archive/test execution binding.

Controlling retained captures: `wave3-boolean-save-io-1400980d0.asm`, `wave3-remesh-save-writer-1400abcc0.asm`, `wave3-remesh-save-writer-14146da0c.asm`, `wave3-remesh-save-invalid-141470568.asm`, `wave3-remesh-default-invalid-141470440.asm`, startup contract `wave3-remesh-invalid-parameter-startup-contract.json`, and CP109/CP112 handler reports. All are under `analysis/`. No GUI, original launch, native capture or mutable REA database was used.

## Exact source occurrence and terminal consequence

1. Save tail obtains the process remesh owner at `14009823e`, reads enabled at `14009824d`, and calls writer `1400abcc0` at `140098265` when enabled. Earlier project/property/MST attempts can already have happened, including returned-false file results. Earlier dialog/guard exits remain distinct. The writer uses relative `LOCAL_REMESH.DAT`; Save-time CWD and enabled state require same-run qualification.
2. Writer `1400abcda` calls fopen, stores its unchecked result at `1400abcdf`, and loads that exact value into the first fprintf's FILE argument at `1400abceb..cf0`. A null result reaches `14146da3b..55`: obtain errno storage, write `0x16` = decimal22/EINVAL, zero all invalid-parameter arguments and call dispatcher `141470568`. This is already a first format attempt, not zero attempts. ERANGE34 is unrelated.
3. Dispatcher reads EXE slot `1446ffb40`, decodes it and branches. Startup installs encoded null before main, but the retained bounded setter search does not rule out every later indirect/foreign replacement. A nonnull custom handler can return. Only still-null selection calls `1414834f0` (clear debug dword `1450540e8`) then default `141470440`.
4. Default handler zeroes a local exception record, captures a local CONTEXT at `14147047f`, looks up the captured RIP at `141470498`, and either invokes RtlVirtualUnwind at `1414704d6` or executes the original no-entry fallback. The virtual operation changes a copied context for reporting; it is not a live Save-stack unwind and does not run C++ destructors.
5. It sets ExceptionCode=`C0000417`, flags1/noncontinuable and ExceptionAddress from its original caller return slot. ExceptionAddress is not replaced by post-unwind RIP. Then `IsDebuggerPresent` at `14147051d`, `SetUnhandledExceptionFilter(nullptr)` at `141470527`, and `UnhandledExceptionFilter(&localExceptionPointers)` at `141470532`. The old filter return is ignored and no restoration appears in this body. If the filter returns exactly0 and the saved debugger result is0, it repeats the debug clear at `141470543`; all other returning filter values skip that repeat.
6. Both ordinary returning filter branches join `GetCurrentProcess` at `141470548`, then call TerminateProcess at `141470556` with EDX=`C0000417`. Successful ordinary termination predicts unsigned status3221226519 / signed-1073740777 and prevents later remesh prints, fclose, Save return/status adaptation and normal Close. Earlier completed artifacts remain. A null FILE alone establishes no arbitrary byte-residue guarantee; the owned fixture's pre-truncation denial supplies that narrower claim.

No original OS exit status is observed here. Exceptions, debugger/filter intervention or a nonreturning filter can prevent reaching TerminateProcess. A termination call that returns reaches the actual epilogue `14147055c..564`; dispatcher returns and fprintf's null branch sets EAX=-1 at `14146da5a`. That adverse native continuation must not be replaced in an original-behavior claim by the candidate service's fail-closed park. Unknown/custom candidate guards and backend parking remain intentional adapter/harness boundaries.

## New PE unwind-table fact

A standard-library read-only PE section/exception-directory walk finds directory RVA `0x5056000`, size2276340 bytes. The default-handler runtime entry is:

| Field | Exact value |
| --- | --- |
| Begin / exclusive end | `141470440` / `141470565` |
| Unwind-info RVA | `0347c098` |
| RUNTIME_FUNCTION 12-byte SHA-256 | `e304ba4f734e9978544add66ee40b5552b921f0d9dc1ace7cdb84526f23c12eb` |
| Aligned unwind bytes | `010903000901bc0002300000` |
| Unwind bytes SHA-256 | `022ee186192acbd397bcba418da75ce9be30f6599b6e8077091d41875e43c28b` |

The metadata describes version1, flags0, prolog length9, three code slots and no frame register. At prolog offset9, `UWOP_ALLOC_LARGE` with info0 uses next value`0xbc`, scaled by8 to `0x5e0`. At offset2, `UWOP_PUSH_NONVOL` info3 describes RBX. This agrees with retained instructions `push rbx; sub rsp,5e0`. Opcode interpretation follows Microsoft's [x64 exception-handling metadata specification](https://learn.microsoft.com/en-us/cpp/build/exception-handling-x64?view=msvc-170).

**Conditional projection:** if an explicitly supplied captured RIP is the original capture-call return site `141470485`, the original image's table covers it after the complete prolog. Let `B` be that supplied captured body RSP. Under ordinary unmodified metadata/stack semantics, virtual reversal restores RBX from `[B+5e0]`, takes caller RIP from `[B+5e8]`, and produces caller RSP=`B+5f0` after the simulated return. These are calculations from supplied words, not reads of an observed original stack.

The original no-entry fallback is different: it writes RIP=`[B+5e8]` and RSP=`B+5e8`, with no +8. Preserve it exactly; do not normalize it to the usual virtual-return rule. The metadata makes the entry-present branch source-predictable when the captured PC falls at the qualified site. It does not supply the actual captured PC/RSP, prove runtime lookup returned this table entry, or recover a full CONTEXT. Candidate compiler frames and candidate image unwind tables are different objects.

## Smallest safe next implementation

Add only new private pure files, for example `src/engine/remesh_default_unwind_projection.hpp/.cpp` and `tests/mdt/test_remesh_default_unwind_projection.cpp`. Do not alter the existing service, wire schema, endpoint, public C ABI or CLI. The function should require the exact original metadata identity, explicit captured PC=`141470485`, supplied body-RSP and two explicitly supplied stack words (saved RBX/caller return). It should return a **modeled original-frame projection**, preserving provenance and a refusal reason. It must never dereference those numeric addresses, call RtlCaptureContext/RtlVirtualUnwind, mutate a context supplied by an embedding app, or claim observation.

Meaningful tests: decode/check exact original metadata fields; derive restored RBX/RIP/RSP from deliberately distinct words; preserve ExceptionAddress as original return value; refuse wrong PC/table/metadata or absent words; reject address-arithmetic overflow; and contrast entry-present RSP=`B+5f0` with original no-entry fallback RSP=`B+5e8`. Use the result as a hypothetical input to CP112's pure model only. These tests close metadata arithmetic and branch relations, not native capture or full lifecycle acceptance.

A later opt-in owned-child native probe could call actual capture/lookup/unwind on its own frame with durable bounded diagnostics. That needs separately reviewed frame/return-slot provenance, owned debug flag, debugger/filter/WER side-effect policy, failure/nonreturn handling and hash/nonce/job/deadline protection. Its successful operations would establish candidate-child behavior only. An original enabled-remesh occurrence still needs one same-run witness binding fixture/CWD/enabled byte/fopen-null, decoded module slot, captured context/lookup branch, filter calls and child OS status; no new model or matching candidate exit code supplies that witness.

Only this report was written. No source, test, CMake or build mutation occurred. No original/candidate executable was launched. Token usage was unavailable. Original terminal behavior remains a qualified static prediction; the pure model and candidate fixed endpoint retain their separate evidence scopes.
