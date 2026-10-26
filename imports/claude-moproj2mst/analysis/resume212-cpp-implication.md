# CP212: C++ implication of the ordered outer-loader returns

**Superseding CLI correction:** the original `-f` interface decodes four hexadecimal digits per UTF-16 QChar. The captured literal path violates that interface. AL=0 is evidence for the malformed CLI invocation only; it is not evidence of valid original B98 Open failure or of a public converter discrepancy. Read the correction appended below before interpreting the earlier boundary analysis. No public source change is justified.

The original observation locates the first failed outer prerequisite at the subordinate reader acceptance result. It does not yet justify making the public converter reject this project, crash, or skip Save. The smallest justified next step is to identify the actual native QString and the suffix predicate that returned false; the existing C++ accepted-suffix/read-status distinction should remain intact meanwhile.

## Evidence boundary

This report read retained evidence and the actual CP209/CP211 source archives. It ran no original executable, GUI, converter, build, capture, or deployment and changes only this report. The original target is DEF_GUI_MO_64 v11.0.2, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. The raw capture retains its historical CP210 directory name; CP212 is the present interpretation/integration checkpoint, not a second runtime experiment.

Principal record: `analysis/resume210-original-outer-loader-frida-2026-10-26T12-41-01-511Z/summary.json`. Qualifications read: `resume209-headless-entry.md`, `resume209-cpp-action.md`, `resume210-loader-null-static.md`, `resume210-loader-cpp-map.md`, and `resume211-typed-gate-implementation.md`. Static reader evidence: `evidence/gui_mo/s1/s1-01/dc/open0.c` plus instruction capture `analysis/wave3-boolean-save-pump-14008a9b0.asm`. Decompiler names remain interpretations.

Frozen sources inspected directly inside `build/wave3-checkpoint209-verified/source.zip` and `build/wave3-checkpoint211-verified/source.zip`:

| Archive | SHA-256 |
|---|---|
| CP209 source.zip | `85f106343c802f05e5e5e69b135c18adee508916c9e35a16799d5a073dfa25c6` |
| CP211 source.zip | `c08fcc946d5db4e36e6a62507a72c8e13128738c30e2f6ed0835df49edf88e11` |

Both freezes have identical `src/moproj/read.cpp` (`da7bb6b87bd70c771c5b0fa25a9eaec2ef6a0139684d00ce503ce0270bc5e145`), `src/core/convert.cpp` (`f3fbc077fc700df96782eef7e621eaf1cba5010712a9b245279b9642c1ec14ce`) and `src/qt4/path.cpp` (`01c428cd6731e23b495efc4ed701e3761095c772b4d08670d65a5494ad4f7039`). CP211 adds the candidate typed-child projection in session construction. These archived bytes, rather than an ancestry commit or mutable working tree, support the source comparison.

## What the runtime record resolves

The fixed owned CLI invocation is `-m open -f C:\PROBLEM\p202a\Gear_blank.moproj`, with unchanged fixture digest `55e2a58cf8155995f942f89ac0ebb91454308b521ae76a228edec0ffdc4cedbd`. One valid invocation, ID 1, has ordered normal returns:

| Sequence | Boundary | Captured result |
|---|---|---|
| 2 | duplicate lookup, caller return RVA 1cf11 | 0: no duplicate refusal |
| 3 | model factory, 1d08a | `0x3853820` |
| 4 | typed-child cast, 1d0bd | `0x3853820`: same child identity |
| 5 | Project getter, 1d0d8 | `0x3855220` |
| 6 | reader acceptance, 1d0fb | AL 0 |
| 7 | outer loader, caller return 1c94 | RAX 0 |

`captureComplete`, `outerEvidenceQualified`, `hookEvidenceQualified`, `commandInvocationQualified` and `cp205FaultPath` are true. Cleanup is verified, process absent, and termination cleanup false. This eliminates duplicate refusal, null factory, failed cast and null Project as the first failed prerequisite in this occurrence. Earlier reports leaving all five alternatives open are superseded for this occurrence. It also supports CP211's kind-only child gate as an internal correspondence; it does not make native and candidate storage layouts identical.

The record explicitly says `runtimePathQualification = raw_qstring_address_only_content_unverified` and restricts the causal scope to the fixed owned CLI Open invocation. `pathQString = 0x7df8e8` is an address, not decoded text. The launch argument and unchanged disk fixture cannot prove what QString contents the reader actually examined after startup, conversions or event processing. Save and accepted Close were not reached; no original full lifecycle acceptance follows.

## Why a public behavior patch would be premature

Frozen `openProject` (`read.cpp:29..65`) sets `openedOk` false, checks suffixes in order, and returns false for an unaccepted suffix. An accepted `.moproj` sets the project path, opens its data-root DB, assigns the XML-reader result to `openedOk`, performs the tail work and returns true independently of XML read success. `.DB` and `.KEY` enter an explicitly unsupported candidate branch. `endsWithSuffix` (`qt4/path.cpp:661..665`) strips an optional dot from the requested extension, adds a dot and compares case insensitively. Thus the literal recorded launch filename is accepted by this C++ suffix predicate; malformed XML alone cannot produce its false acceptance return.

Frozen `convert.cpp:241..249` rejects false acceptance; full Save separately permits `openedOk == false` and selects the constructed model before activation. Legacy modes preserve their earlier parse-status mapping. Changing the condition to reject XML failure in full Save would conflate two original results and contradict the retained reader tail. A filename/B98 special case, universal false return, forced null model, or new C ABI error would encode the observation without its cause.

The original reader has more opportunities to inspect the live path than this simplified statement suggests. It pumps events before its first suffix ladder (calls at RVAs 8aa10 and 8aa43), checks `818b50` on the reader path, and has a second suffix ladder after the DB work. `open0.c` shows zero-return arms when all suffix tests fail in either ladder; instructions include the later explicit `xor al,al` at 8af10 and the normal true tail at 8af74 onward. This bounds a useful hypothesis but does not identify which ladder or predicate produced this occurrence's zero. A nonnull Project does not certify the path or predicate results.

The capture therefore establishes a real divergence at the acceptance boundary under the fixed original launch. It does not establish that the native reader rejected the same string value that the public m2m API received. Nor does it establish that a candidate suffix implementation is wrong, that original XML refusal must become outer null, or that the null-arm GUI notification fault is the required public ABI representation.

## Exact missing witness and implementation decision

No public Open/Save behavior change is source-ready from this capture alone. Keep the CP209 resolver and CP211 internal typed-child projection; retain the accepted-suffix/read-status separation and mark this acceptance discrepancy unresolved. A report/comment qualification may state that the native nonnull continuation follows acceptance success, rather than asserting it for every `.moproj` launch.

The smallest discriminating witness is one same-invocation, caller-qualified record of actual suffix-call inputs and result inside `8a9b0`: the bounded decoded UTF-16 path with its QString object/data identity and length, the extension value, and AL from `818b50`, tagged by which first/second ladder call site was reached. Bind the reader's RCX to the returned Project and its RDX to the actual path object; preserve the outer invocation ID, thread, stage ordering, natural completion, first-AV qualification and cleanup receipts. Capture the second ladder too if reached. A decoded outer-entry filename alone remains insufficient because event pumping precedes the first tests and intervening work precedes the second ladder.

If native input differs from the candidate string, recover the transformation/state transition that produced it before choosing any code change. If the same valid UTF-16 value reaches a suffix predicate and the original returns false where C++ returns true, recover `818b50`'s exact callee/locale/argument semantics and add a focused helper regression before integration. If first-ladder acceptance succeeds and a later path changes, recover that actual mutation and its lifecycle owner; do not inject a changed string. Only after that bridge exists can an internal rejection/cleanup transition and the existing ABI's faithful representation be selected. A source-only guard for a native acceptance result is not sufficient while the predicate producing that result is unknown.

The existing complete CP211 candidate test receipt remains candidate validation, not original acceptance for this launch. This report makes no new parity, build, deployment, timing or token-savings claim.


## Superseding CP212 CLI decoder correction

The independent exact-PE analysis in `analysis/resume212-cli-decoder-independent.md` and `analysis/resume212-acceptance-static.md` resolves the previously unqualified launch-to-reader boundary. Main passes option `f` through decoder `11a0` at RVA `1c4d`, then passes its resulting QString to the outer loader. The safe input language is four ASCII hexadecimal digits for each numerical UTF-16 code unit (`0043` means `C`), concatenated without separators. The inspected scanner uses `%x`; it is not a strict canonical-input validator.

The literal `C:\PROBLEM\p202a\Gear_blank.moproj` has 34 characters. Decoder allocation and final QString length use floor(34/4) = 8 QChars, while its loop attempts ceil(34/4) = 9 stores. It also requests input characters beyond the final partial group. Scanner assignment counts are ignored. Therefore the exact malformed output, possible Qt bounds effects and allocator consequences remain unverified; do not invent a deterministic resulting filename or universal crash. A canonical encoding of the intended 34-unit path has 136 hexadecimal characters.

This correction supersedes the earlier sentence describing a real native-versus-candidate acceptance divergence: the measured AL=0 and outer RAX=0 remain valid for the fixed malformed launch, but there is no established same-input comparison to the direct-path public m2m API. The observation is not evidence of valid original B98 Open failure, a parser defect, a candidate model-construction defect, or a public Open/Save discrepancy. Earlier CP205/207 fault-path correspondence remains correspondence among those retained launches, not proof that their reader received the intended project path. Save and accepted Close remain unproven.

Keep the suffix-ladder analysis as qualified static behavior: either ladder can reject, and XML read status remains separate from acceptance. It is no longer the first justification for a new original run before fixing the harness input boundary. The source-ready correction belongs in a separately reviewed original-CLI harness: encode the intended UTF-16 units into the native `-f` transport, pin both encoded argument and intended decoded path, and qualify that new command identity plus a same-run decoded-reader-path witness. This report implements none of those changes and authorizes no capture.

Do not add hexadecimal decoding to `m2m_convert_w` or its UTF-8 counterpart: they take direct paths as their compatibility API. Do not alter `openProject`, force B98 rejection, change the full-Save `openedOk` policy, or add a public failure representation on the basis of malformed-CLI AL=0. No public source change is justified by CP212. The remaining acceptance task is a correctly encoded and independently qualified original Open observation, followed by the agreed lifecycle comparison if Open succeeds.
