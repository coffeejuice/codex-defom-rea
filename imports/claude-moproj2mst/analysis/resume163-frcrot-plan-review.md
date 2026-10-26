# CP163 FRCROT integration plan review

## Decision

The proposed bounded actual-reader prefix is safe to implement as an explicitly synthetic exploratory profile, provided its admission predicate is exact at the raw XML shape and its stopping point remains before `0x140910b6f` function conversion. This is a narrow source-derived state prefix, not evidence that the original accepted these values or that parser success/temp-file output has been recovered.

## Supported scope

The retained reader evidence in `analysis/resume163-frcrot-integration-evidence.json`, qualified by the source-bound analysis, supports these distinctions:

- `parmType="function"` sets the reader's function bit; the entity list is separately accumulated from `KwEntities`, while `KwOperations` drives the DOE-operation loop. An exact empty operation node therefore bypasses that loop independently of an entity ID.
- After append, the reader reselects the named variable owner and commits parameter/function/enum and text fields before the branch at `0x140910b6f`. Function conversion starts there. The proposed prefix can therefore safely represent the named owner and the three source text values, then stop.
- The subsequent selector comes from entities, not operations. With exactly one entity ID `1`, its described selector shape is `[1,1]`; this is a bound for a future stage, not a reason to enter the converter now.

Use the existing constructor-prefix helper and canonical named record. Keep the current `Study::samples.mode == 0`, reader dimension `3`, fresh-owner/alias, and existing empty Item-list constraints. Admit only registry ID276/FRCROT and the exact supported attributes: `unitType="0"`, `parmType="function"`, `parameter="-1"`, `nominalValue="0"`, `funcEntryNo="0"`, absent `parameterName`. Require the exact expected child order and no extra attributes/children: three data wrappers with one first-child `string` each containing the fixed single-line values, then raw-child-empty `KwOperations`, then one `KwEntity id="1"`. Reject CR/LF, comments/raw text, unexpected null/raw nodes, additional entity children, and any nonempty/unknown operation shape. A generic broad `childElements()` filter would be too permissive if it ignores intervening raw children; preserve the existing first-child discipline.

After append ID276 with empty operations and entities `[1]`, re-resolve by original variable name, ensure the appended record is still the last canonical item, then set `parameter=-1`, `function=true`, `enumeration=false`, and all nominal/upper/lower text fields from the exact admitted strings. Stop with `M2M_E_UNSUPPORTED` at the named `0x140910b6f` boundary. Do not call the notifier, converter, table helper, temp-file path, or downstream finalization.

## Guard and tests

Keep the old TLOC profiles behaviorally unchanged. FRCROT's new exact profile must not broaden into unknown function keywords or imply that entities participate in DOE-operation setup. The guard should fail closed before constructor mutation for malformed profile inputs; after a valid prefix it should preserve the documented partial mutation and boundary error.

Tests should exercise the actual `ConversionSession`/Type2 lazy-read route and a recording `StudyIo`, not only the helper in isolation. Cover A,B,A named-owner selection while the outer header/Description target differs; assert ID276, empty operations, entity `[1]`, parameter/function/enum, and all three exact texts on A's canonical record. Assert B's unrelated outer effects remain as dictated by the established reader order. Verify no notifier event, temp.KWD write, later sample processing, or deferred-link completion. Preserve tests for TLOC and operation-one legacy guard. Add rejection cases for extra/unknown entities, nonempty or raw operation children, malformed data wrappers, newlines/non-ASCII if these are outside the qualified profile, extra attributes, and wrong registry-facing identity. Do not claim tests are run unless explicitly requested by the primary task owner.

## Risks and limits

1. This profile is synthetic and branch-derived. No original parser-success or equality observation is established.
2. Qt string storage, native refcount/codec behavior, and text allocation parity remain unqualified; single-line ASCII reduces but does not eliminate those differences.
3. Function selector handler behavior is not validated by this prefix. The positive pair shape does not license empty/nonpositive selectors or any out-of-bounds handler arm.
4. The actual converter owns relative `temp.KWD`, QFile flags `0x12`, QTextStream output, C scanner reopen, scratch models and nested cleanup. The inactive `keywordValues` helper has a different path/mode/lifetime/parser contract and must not be wired in as a shortcut.
5. Reader setter order and the converter's all-three-nonnull commit gate are distinct from Save reconciliation's gate. The prefix must stop before conversion, avoiding premature assumptions about the converter's commit.
6. Error text should identify the exact reached boundary without asserting parser rejection or file conversion failure.

## Review outcome

Approved in principle for the exact contextual reader owner prefix described above. Full FRCROT table-helper integration remains blocked on qualification of the actual file/current-directory owner, QTextStream bytes/codec, scanner cursor/status producer, group3 pair ownership, scratch allocation and unwind behavior. The next implementation checkpoint should add only the prefix and its contextual tests, then retain evidence and inspect the diff before considering deeper integration.
