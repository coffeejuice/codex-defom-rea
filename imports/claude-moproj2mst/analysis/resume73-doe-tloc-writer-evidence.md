# Checkpoint 73: B121 TLOC direct writer evidence

The retained one-variable B121 record is sufficient for a direct isolated model-to-XML fixture writer. It is not sufficient to enable public Save. The nominal XML text is a copy from the existing keyword record; it does not itself write `temp.KWD`. Exact agreement with the historical 707-byte XML can establish this bounded candidate format, without proving keyword reconstruction, earlier reconciliation, native I/O failure order, refresh delivery or the later DOE files.

This report is read only apart from its own creation. Findings were coordinated with `/root/oracle_audit`, owner of the isolated writer source/test changes. No worker source changes or test execution occurred here.

## Exact target and historical qualification

Target: `evidence/gui_mo/oracle_b/121_B_DOE_VAR/DoeStudies/DoeStudy1/Control/DoeInputTable.XML`, 707 bytes, SHA-256 `18EC0C482730C82ADE12E91771CA67E53E4604573032624B99F3F4829A86B069`. This is the historical post-action state after creating a Study/Transfer Time variable, not output of plain Open-Save on the no-study `_before` input.

The retained shape is:

1. DoeInputTableFile doctype, sfDoeInputTable Version1.
2. Empty DoeCommonMeshPath; empty DoeCommonMeshOrigName; DoeDoPositioning false; DoeRoiType0. No RegionsOfInterest.
3. DoeVariables containing one DoeVariable: numSamples3, varId1, dimension3, name Transfer Time.
4. Its Keyword: nominalValue0.1, unitType0, name TLOC, parmType double, parameter-1.
5. NominalData/string, then KwOperations/KwOperation id1, inside that Keyword.
6. Root Operations/Operation id1/DoeOprVariable name Transfer Time, after DoeVariables. No constraints.

NominalData text is exactly `TLOC` + **four spaces** + tab + `0.1`. Some earlier prose used five spaces; CP72's implementation note and the retained bytes correct that. The root `temp.KWD` is 13 bytes with the same 12-byte content followed by LF, SHA-256 `15EEFCF3997F7C89507AF9D42DB2E38A37E38F4F20D7F3B0467BD992455DC404`. Do not add its LF inside the XML string node or copy the XML's file line endings into that keyword payload.

XML numSamples3 is a variable attribute, not proof of the sample manager's row count. The separate B121 PAR files contain 100 rows. Nested keyword operation id1 and root operation binding id1 are separate relationships; neither equals the downstream DEF_DOE.INI operation numbering layer (OPR_NO5).

## Native serialization order

Retained `evidence/gui_mo/s1/mdt/DOE/d0.c`, function `0x14090a550` beginning at line1145, supplies the writer's structural/control order. Original image SHA-256 remains `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

- At d0.c:1567, `0x14091c820` removes variables with no keyword bindings and no geometry entries. This precedes QFile construction/open. The single admitted TLOC record is nonempty; a const isolated one-record writer does not implement general pruning.
- Failed WriteOnly|Text open sets native global error and returns false. Document creation and all serialized variable work follow successful open. The existing XML device prefix separately qualifies close and post-close QFile.error semantics; a FileEffects planner does not implement this device sequence.
- Common root/header nodes are appended first, followed by optional nonempty RegionsOfInterest, then DoeVariables. Native enumerates manager variables in its stored order. For each variable it obtains name/id, its model, sample-count/dimension values and optional linked metadata, then its keyword records.
- At d0.c:1765–1780, manager virtual+0x128 counts keyword records for the variable and virtual+0x138 obtains each actual keyword. `0x140921930` supplies the keyword name. Unit/type/parameter and nominal value follow. B121 exercises the scalar double branch; linked/string/other parameter branches remain excluded.
- At d0.c:1890–1911, obtain nominal text and, when nonempty, append NominalData/string with a DOM text node. Later upper/lower branches are conditional and absent in B121.
- At d0.c:1959–1989, obtain keyword operation IDs via `0x1400d0e20`; a nonempty list appends KwOperations and ordered KwOperation id children. This is part of the keyword, not the root operation list.
- After variable/keyword serialization, d0.c:2300 onward derives the root operation-to-variable map from variable model operation membership, appends Operations, then Operation id and DoeOprVariable name children (roughly2340–2370). The one-record case agrees with the typed separate keyword IDs and variable.ops model.
- Constraints follow only when their count is positive. QTextStream emits QDomDocument.toString(1), QFile closes, and post-close QFile.error controls ordinary XML writer true/false. The enclosing `0x1400d3200` subsequently dispatches refresh regardless of that Boolean.

The native **attribute setter call order is not the observed serialized attribute order**. For example the native variable loop sets name before later numeric attributes; the retained output prints numSamples, varId, dimension, name. The keyword loop obtains/sets name before later nominal value, while the retained output prints nominalValue first. QDom attribute serialization order must not be described as native insertion order or as a general portable guarantee. The isolated candidate's explicit attribute order can reproduce the exact frozen artifact; that is a fixture-specific byte regression, not a recovered general Qt attribute-map algorithm.

## Nominal keyword side effects

The actual nominal getter is closed by retained `evidence/gui_mo/s1/mdt/L1/B/b12.c:92–99`:

```text
0x1400654f0: construct returned QString by copying keyword+0x10; return it.
```

The XML writer calls it at d0.c:1890. It is a value copy, with no temp file open, KEY parse, sampler action, nominal-value recomputation or notification. Upper/lower getters are likewise QString copies from keyword+0x18/+0x20 in retained `out_p16/p17slice.c:113–131`. This direct writer can consume the already-loaded nominalDataText; it must not invent a temp.KWD write merely because that historical file exists.

The relevant temp.KWD producer is `0x140916530` in DOE/d9.c. Its first successful QFile open writes supplied nominal text, closes, constructs a scratch dimensioned model and dispatches the keyword reader. Upper/lower phases follow in that function when admitted. It can be called **before** the InputTable writer from wrapper reconciliation `0x1400d2b60`: DOE/d1.c:405–484 resolves actual global current Project/operation, compares the variable's old dimension with the operation dimension, updates on difference, obtains its first keyword's stored nominal/upper/lower text and calls `0x140916530`.

Therefore Save may reach a nominal temp-keyword effect on that conditional reconciliation branch, but direct `0x14090a550` serialization does not establish it. The B121 after-state file also includes earlier interactive Add/Edit effects, so it cannot establish that the Save invocation changed dimension or rewrote temp.KWD. TLOC/group6's native uninitialized reader status remains a separately guarded uncertainty. A pure isolated writer should produce only its requested XML effect and leave this branch unclaimed.

## Candidate isolated writer review

The inspected in-flight API is `writeB121TlocModelPrefix(path, const Study&, FileEffects&)`, `src/doe/doe_study.cpp:550` onward. It is not called by public writeInputTable/saveInputTable. It validates the exact one-record scalar/name/id/dimension/sample/nominal text and both operation mappings; builds DOM elements and text from that typed payload; uses local8/toString(1), Windows newline translation and one deferred write. It does not read retained artifact bytes to populate its output. Its model fixture can therefore support an exact 707-byte comparator without bypassing lifecycle guards.

One bounded scope defect was sent to oracle_audit/root: the inspected helper hardcodes empty common mesh fields, positioning false and ROI0, and omits regions/constraints without validating the actual Study values. Native serializes those Study values or branches on them. Minimal safe fix is to reject any Study outside the retained common-header/empty-region/constraint0 profile before appending an effect, or source-bind and serialize actual headers. Do not silently serialize a nonfixture Study as B121. The frozen source hash for the implemented/fixed helper belongs in its owner's report; this lane only reviews the in-flight implementation.

The direct writer's true result currently means successful **planning**, not native QFile open/write/close success. It must remain outside public error-aware XML execution. It neither mutates the actual keyword's scalar state nor proves normal native getter/owner calls were executed. Exact bytes establish the bounded projection only.

Focused owner tests should compare model-generated output to the complete retained 707-byte artifact, retain separate round-trip model assertions, verify exactly one requested XML write/no temp.KWD or other effects, reject nonfixture common headers before output, and assert both public nonempty-variable reader/writer guards remain. A direct native I/O/refresh test is outside this helper's claim. This lane authored or executed no tests.

## Next dependency and receipt

Smallest ready source slice is the isolated fixed-schema model writer plus precise profile validation/tests. Public binding still requires variable ownership/keyword construction and conditional dimension reconciliation, pruning/index finalization, real immediate error-aware XML device serialization, actual refresh population/timer/UI continuation, and downstream outputs in native order. Matching this fixture does not authorize B121, B127 or full DOE Save release.

| Retained input | SHA-256 |
| --- | --- |
| B121 DoeInputTable.XML | 18EC0C482730C82ADE12E91771CA67E53E4604573032624B99F3F4829A86B069 |
| B121 temp.KWD | 15EEFCF3997F7C89507AF9D42DB2E38A37E38F4F20D7F3B0467BD992455DC404 |
| evidence/gui_mo/s1/mdt/DOE/d0.c | 1EC4144D1BCDC367EEDDF8ADD4F2368ACFAC4103D15F3D582564F5FFF3A5E2F3 |
| evidence/gui_mo/s1/mdt/DOE/d1.c | DC1662AB5BDE9B414407337B95A4D9F770EFDBDF4D4F48B197C4D036575684B0 |
| evidence/gui_mo/s1/mdt/L1/B/b12.c | C36EB5A9AF8AAA19208D986E269DD8B5A333F5DE385D76296F3CDE8A1633D8B0 |

No source/build/test/GUI/application/mutable REA operation was performed. This report is the only written file. No native runtime equivalence or public Save completion is claimed.
