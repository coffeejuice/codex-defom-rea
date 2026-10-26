# B121 DOE variable parser body: static evidence status

## Result

The exact valid-data branch and inner failure effects of original `0x14090ee20` cannot be recovered from the currently retained local evidence. The retained decompile of its caller (`evidence/gui_mo/s1/mdt/DOE/d0.c`, `FUN_14090e080`) names the call but does not include the callee body. A repository-wide search found no other retained body, assembly listing, procedure receipt, or independently validated pseudocode for `0x14090ee20`. The case XML identifies one valid-looking B121 `DoeVariable` record, but a single input/output pair cannot establish the callee's accepted variants, type discriminator, field-consumption order, or side effects on malformed input.

The user/task permits inspecting the immutable PE or read-only REA but requires serializing shared REA access with the root first. No such serialization/confirmation was available during this investigation, so no REA operation was started. Local shell inventory also found no `dumpbin`, `objdump`, `rizin`, `radare2`, or `llvm-objdump`, and Python `capstone`, `pefile`, and `lief` modules are unavailable. The repository's `tools/doe_static_pe.py` intentionally produces candidate references and explicitly requires a bounded disassembler confirmation; it cannot decode this function. Thus there is no valid basis here to claim a native body.

## Established enclosing behavior

The retained outer function `0x14090e080` provides these bounded facts:

- It successfully parses the XML document, closes its input file, and visits root child elements in order.
- The exact element tag `DoeVariables` dispatches to `0x14090ee20`; the separate `Operations` tag dispatches to `0x140912a20`.
- B121 document order is four common headers (`DoeCommonMeshPath`, `DoeCommonMeshOrigName`, `DoeDoPositioning`, `DoeRoiType`), then `DoeVariables`, then `Operations`. The header setter effects occur before `0x14090ee20` is entered.
- If `0x14090ee20` returns false, outer code breaks the child loop, performs its post-loop variable collection pass, then returns false. `readLazyStudy` ignores that Boolean and continues its later sample-file operations. This describes the enclosing false-return path; it does not establish that valid B121 causes false or describe any mutations made inside `0x14090ee20` before a false return.

B121's retained XML node has one `DoeVariable` with attributes `numSamples="3"`, `varId="1"`, `dimension="3"`, `name="Transfer Time"`; a nested `Keyword` names `TLOC`, `nominalValue="0.1"`, `unitType="0"`, `parmType="double"`, `parameter="-1"`, and contains `NominalData/string` text plus `KwOperations/KwOperation id="1"`. This is a fixture-specific observation, not a parser contract.

The prior `analysis/wave2-doe-evidence.json` expressly qualifies its imported DOE decompiles: the DOE function-address/body work was not independently byte-validated in that pass. That qualification is particularly material here because the function body is missing entirely. The comments in current source and the DOE notes point to 121 as a future variable case but do not fill the callee gap.

## No exact inner claims supported

The current evidence cannot determine:

- whether the inner discriminator is the child tag `DoeVariable`, a type attribute (`parmType`, geometry node presence, another field), or a sequence of those checks;
- whether name/id/dimension/sample-count attributes are read before nested payloads, or whether defaults/validation occur first;
- whether a `Keyword` record calls `0x140916530`, how `TLOC` group/handler results affect the branch, or how keyword values are attached to variable state;
- whether `KwOperations` is read inside this callee or only by `0x140912a20`;
- when a variable object is allocated/appended/indexed relative to parsing children;
- the return condition for valid B121, and partial list/state/file mutations for malformed or failed records.

In particular, no inference should be made from the 13-byte `temp.KWD`, sample files, or the post-save XML about exactly when/how the parser writes scratch data. Existing notes qualify TLOC's group-6/status behavior as uncertain.

## Source boundary and recommendation

Current `doe_study.cpp::readInputTable` conservatively throws `M2M_E_UNSUPPORTED` when the `DoeVariables` element has child elements. It applies earlier common headers first; because this is an exception, it skips the outer native false-return epilogue and causes `readLazyStudy` to stop before sample-file operations. That is an implementation guard, not evidence of original native failure behavior.

Keep the variable-content guard. The only safe source-qualified helper at present is an outer dispatcher that preserves child order and known parent false-return/finalization behavior for already-understood branches, while keeping “unsupported/unrecovered” distinct from a native Boolean false. It must not parse or partially commit B121 variables. To cross the boundary, a root-serialized, read-only PE decompilation/assembly capture of `0x14090ee20` and its directly called helpers is required; bound the result to this function and preserve full caller/file hashes, address ranges, and tool coverage. Then source-qualify the valid B121 path and each branch's commit/return ordering before implementing it.
