# DOE variable reader: verified B121 native branch

The valid B121 scalar-keyword branch of `0x14090ee20` is now recoverable from the original PE. It creates variable and keyword records before finishing their child data, preserves a per-operation messenger call between those mutations, and returns true on the normal path. B121's `parmType="double"` does **not** take the function-keyword path that calls `0x140916530`; the retained `temp.KWD` is not evidence of a direct file write by this scalar parse branch.

This supersedes the missing-body conclusion in `resume50-doe-variable-body.md` for the new evidence described here. It does not remove that report's historical-fixture or source-guard qualifications. No source edit, build, target execution, GUI input, REA session or Ghidra retry occurred.

## Identity and evidence boundary

- Original: `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, checked again before inspection.
- Native body: `0x14090ee20` through `0x140912a10`; exception-directory end-exclusive `0x140912a11`, 15,345 bytes, file offset `0x90e220`, code SHA-256 `270b855af32d5c5f3fca905489895021d41d376f5e24f05d8941daf161e25f08`.
- Disassembler: Microsoft `dumpbin.exe` version `14.44.35228.0`, installed at the absolute Visual Studio BuildTools path in `resume50-doe-variable-body-pe-receipt.json`. The earlier PATH inventory did not locate this executable. Only bounded `/DISASM /RANGE` selections were used.
- Complete selected body: `resume50-doe-variable-body-pe.asm`; decoded import names and PE-backed literal annotations: `resume50-doe-variable-body-pe-annotated.asm`; readable instruction stream: `resume50-doe-variable-body-pe-compact.asm`.
- Bounded direct-helper captures: `resume50-doe-variable-helpers-pe*.asm`, `resume50-doe-variable-object-pe*.asm`, `resume50-doe-variable-record-pe.asm`, and `resume50-doe-variable-manager-ctor-pe.asm`. Every capture's byte count and SHA-256 are in the receipt.
- Import names come from this PE's import descriptors. String annotations refer to constants at disassembler-confirmed instruction targets, not an unchecked byte-pattern scan. The native instructions were interpreted manually; no decompiler output or REA Evidence ID is claimed.

The controlling input is the historical `121_B_DOE_VAR/.../DoeInputTable.XML`, SHA-256 `18ec0c482730c82ade12e91771ca67e53e4604573032624b99f3f4829a86b069`. It contains one variable, `Transfer Time`, sample count 3, ID 1, dimension 3; one `Keyword` with name `TLOC`, unit 0, type `double`, parameter -1, nominal 0.1; `NominalData/string` text `TLOC    \t0.1`; and `KwOperations/KwOperation id="1"`. This is a study/variable-editing-plus-Save fixture, not a fresh Open–Save oracle. Source qualifications in `wave2-doe-findings.md`, `resume48-b121-variable-output-map.md` and the retained DOE specifications remain attached.

## Dispatch, variable creation and header order

The ABI inputs are a QString argument, the `DoeVariables` QDomElement, and a model/manager pointer. It constructs two deferred-link lists and sets a local Boolean at `[rsp+0x64]` to 1 (`0x14090ee60`). It walks **nodes**, calls `toElement`, skips non-elements, and accepts the exact tag `DoeVariable` (`0x14090eee1`–`0x14090ef15`). Unknown child tags are skipped.

For each matching child the order is:

1. Read `name` as a QString, with empty-string default (`0x14090ef1b`). Read `numSamples`, decimal `toInt` with a null success pointer (`0x14090ef98`–`0x14090f01f`). The reader adds no conversion-success or nonempty-name check.
2. Call manager `vt+0xf8(name, numSamples, false)` at `0x14090f0ac`. Ignore its returned pointer. Read manager `vt+0x178` count, subtract one, then obtain `vt+0x1a0(count-1)` at `0x14090f0ef`. This is the variable pointer used for the following header writes.
3. If `varId` exists, convert it to int and call `0x1409ff590` at `0x14090f225`, which writes variable `+0x1c`.
4. Initialize local dimension to 3. If `dimension` exists, convert it and call `0x1400d8040` at `0x14090f352`, which writes variable `+0x24`.
5. Read `LinkTo` with an empty default. If nonempty, append the current variable pointer and link-name string to parallel deferred lists (`0x14090f3e9`–`0x14090f408`). Resolve links after the complete variable-node traversal, not immediately.
6. Walk each variable's child nodes. A `Description` independently copies nonempty text through `0x14010f200` (`0x14090f543`). A `Keyword` independently enters the branch below. The later `Geometry` discriminator is another independent test. Unknown or non-element children are skipped.

The installed SfMoDoeManager vtable is byte-verified: constructor instruction `0x1400d11b5` installs `0x1421671b8`. Its relevant slots are `+0xf8=0x140919de0`, `+0x118=0x1400d25a0`, `+0x128=0x14091a950`, `+0x138=0x14091aa50`, `+0x178=0x1400d7f00`, `+0x1a0=0x14091b950`, and `+0x1e0=0x14091c7a0`. These agree with the retained vtable inventory; this pass independently checked the qwords.

`0x140919de0` first tests for an existing same-name variable through `vt+0x130`. A new variable is allocated, constructed and appended to model `+0x38` before its name is assigned. Constructor `0x140816900(numSamples)` writes sample count `+0x10`, zero `+0x14`, index `+0x18=-1`, number `+0x1c=0`, `+0x20=100`, dimension `+0x24=3`, empty lists and null link `+0x40`. The reader's false argument prevents that helper's optional initial number assignment. The new-variable path updates selected index `model+0x74` and marks all three sample-invalid bytes `model+0x70..0x72` true. **Only when manager mode `+0x40 == 2`** does it call design acquisition/generation `0x14091d110` at `0x140919fda`. The independently recovered fresh-manager mode-0 prefix skips this call; mode-2 behavior must remain qualified/guarded until its design state is available.

Existing-name handling is not equivalent to a fresh single-variable parse: the create helper returns an existing variable, but the reader ignores that return and still uses the last variable for header writes. Do not generalize the B121 branch to duplicate-name or prepopulated-manager input without preserving this ordering.

## Exact B121 Keyword branch

The branch begins at `0x14090f5bf`. It reads attributes in this order: `name`, `unitType` (decimal int), `parmType` (then `toLower`), `parameter` (decimal int), `parameterName` (empty default), and `nominalValue` (`toDouble`, null success pointer). It sets two local flags by equality with lowercase literals **`function`** and **`enum`** (`0x14090f7db`–`0x14090f835`). It does not require that other text equals `double`; B121 takes the ordinary scalar branch because both flags are false. `funcEntryNo` is read only for `function`.

The reader then initializes enum text/value collections, operation/entity ID collections, and nominal/upper/lower text. It walks Keyword child nodes in document order:

- `NominalData` (`0x14090fe2a`) examines its **first child only**; if that child converts to a non-null element with exact tag `string`, its `.text()` replaces the accumulated nominal text (`0x14090ff66`). UpperData and LowerData follow the corresponding rule. There is no nested sibling loop for these text wrappers in this body.
- `KwOperations` accepts exact `KwOperation` children, decimal-converts their `id` attribute, and appends each result in order to the operations list at local `rsp+0x120` (`0x140910762`). `KwEntities` similarly fills the separate entity list at `rsp+0x138`. These are not deferred exclusively to the separate outer `Operations` reader.
- EnumData has its own branch; geometry/correspondence handling is outside the B121 path and remains unimplemented by this report.

After child collection:

1. Convert keyword name to local 8-bit text and call `0x140921e90` at `0x1409108d0`. This looks up a PE-resident table, returning the record's int ID or sentinel `0x131` when absent. `TLOC` is verified in table `0x1427c06e0` (stride 48) at `0x1427c0860`, ID **8**. Do not confuse its keyword ID with keyword group 6.
2. Call manager `vt+0x118(varName, keywordId, parameterName, unitType, dimension, nominalValue, operations, entities)` at `0x14091095a`.
3. Resolve keyword count for the name with `vt+0x128`, subtract one, then retrieve the keyword with `vt+0x138` (`0x1409109c8`, `0x140910a1b`). The result is used without a null guard.
4. Write parameter selector `keyword+0x80=-1` via `0x14010f1e0`; set function flag `+0x31=false` via `0x140917170`; set enum flag `+0x32=false` via `0x140917190` (`0x140910a38`–`0x140910a62`).
5. If nominal text is nonempty, assign it through `0x140921980` (`0x140910aba`). This helper stores the string at keyword `+0x10`, removes CR characters, and replaces LF with CRLF. The retained B121 text has no newline, so it is preserved. Upper/lower text setters run only for their nonempty text.
6. At `0x140910b79`, false function flag jumps to `0x1409111c4`, bypassing the whole function parsing path and `0x140916530`. False enum flag then skips enum-specific calls. Because B121 has empty upper/lower strings, it skips the bound-parsing/setter calls as well and reaches cleanup at `0x14091142a`.

Keyword constructor `0x1409206e0` allocates/initializes a `0xa8` record through variable helper `0x140816b40`: ID at `+0x2c`, unit type at `+0x28`, nominal double at `+0x48`, two numerical bounds initially zero at `+0x38/+0x40`, function/enum flags initially false, bounds-defined flags `+0x33/+0x34` initially false, and selector initially -1 at `+0x80`. B121 leaves the two bounds and their defined flags at these constructor values. The variable's operations list is extended with missing IDs by `0x140816b40`; B121 yields operation 1. Existing keyword deduplication and nonempty/preexisting collection cases are separate from the single fresh B121 case.

## Reached notification and failure ordering

SfMoDoeManager `vt+0x118` is **the derived `0x1400d25a0`**, not merely base `0x14091a5b0`. The derived helper checks that the variable exists, calls the base keyword-add helper at `0x1400d26e2`, then loops the operations list. For each operation it calls messenger `0x1402cf3c0` at `0x1400d2995` with key **`2signalSetOprHasDoe()`**, typed arguments **bool=true, int=operation ID**, then empty remaining arguments. B121 reaches one delivery for ID 1. Constants `0x142166b34`, `0x142163514`, `0x142166b40` independently decode to `bool`, `int`, and that exact key.

Thus the reached order is:

`append variable / dirty sample state → ID/dimension headers → collect Keyword XML → append keyword / merge Var.ops → signalSetOprHasDoe(true, 1) → set selector/type flags → store nominal text → finish variable children → reindex variables → resolve LinkTo → true`.

The current source's notification helper does not establish a binding for this key. The delivery/receiver effects are **unresolved in this pass**. A faithful adapter must qualify that reachable callback or refuse at its reached boundary; it must not quietly omit it or postpone it until after the remaining scalar setters. If the callback throws/refuses, the preceding variable/keyword/list changes already occurred; there is no parser rollback branch here.

The function's only direct references to its status local are initialization to 1 at `0x14090ee60` and the final read at `0x1409129b5`; it returns that byte at `0x140912a00`. No normal false-return assignment or validation-failure exit appears in the complete selected body. All ordinary child traversal paths converge on the finalization sequence. This does not prove that allocation, helper invocation or messenger delivery always succeeds: unguarded pointers can fault, exceptions can unwind, and unresolved callback effects remain outside coverage.

The finalization call `vt+0x1e0` at `0x1409128b6` maps to `0x14091c7a0`, which writes variable `+0x18=position` for each manager variable. It then resolves queued LinkTo names through `vt+0x180`/`vt+0x1a0`; a found target is attached with `0x140100a60`, while missing pointers are skipped. B121 queues no links and finishes with index 0. The separate outer `0x14090e080` false-return protocol remains a caller contract, but a normally completed B121 invocation of this callee returns true and proceeds to the outer Operations sibling. An unsupported exception in the current port is distinct from that native normal return.

## Smallest supported next implementation boundary

Recover a typed fresh scalar-variable/keyword representation and its lifecycle callbacks before removing the nonempty-DoeVariables guard. The current legacy `Var`/`Item` projection does not expose all newly verified constructor/header/keyword state. Preserve sample count, dimension, numerical nominal/bounds and defined flags, parameter selector, raw keyword text, type flags, operation/entity collections, selected index, sample-invalid flags and deferred link/reindex order. Do not store everything as a name/ID tuple or substitute the function-keyword parser for B121.

For a first bounded branch, require the independently qualified fresh mode-0 manager with no existing variables and accept only the verified B121 scalar shape. Retain explicit guards for mode-2 design acquisition, duplicate/preexisting names/keywords, function/enum, geometry/correspondence and unqualified callback registrations. The notification boundary is after keyword/list commitment. A valid-path test must record the prefix state visible at `signalSetOprHasDoe(true,1)`, then the later selector/flags/text writes and final index; a callback-refusal test must preserve the prefix effects and skip later setters/finalization. A successful parser test must also show that it never calls the function-only `0x140916530`/temp.KWD path.

No fresh original execution, complete callee closure, QObject delivery equivalence, active DOE Save wiring, sample regeneration, or end-to-end B121 parity is established here. Keep the original historical comparator differences and the source guard until those dependent contracts are integrated and tested.
