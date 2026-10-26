# CP215 candidate and original parser-state audit

## Scope and result

This is a source/test audit only. It uses the converter's current source, the CP214 B98 candidate Open-only report, the CP214 original postOpen report, the CP215 static parser map, and the source-qualified s1-01 specification. No source, fixture, original executable, GUI state, Frida state, or shared state was changed.

The candidate exposes two distinct outcomes:

* `acceptedProject` is the return value of `openProject`. In the candidate it reports that the input suffix selected a supported Open route and the Open tail completed. It can be true when the project file is unreadable or its XML read fails.
* `Project::openedOk` is set from `readProjectFile` for `.moproj` input. It reports the file/XML/top-level-walk result. It is the candidate's representation of original `Project+0x55` on the `.moproj` route.

CP214's B98 test observed both values true on the pinned valid fixture. That candidate result is consistent with the original static contract, but it does not report or establish the original B98 `P+0x55` runtime byte. CP214's original capture did not read that field. The original byte remains unknown absent the separate same-occurrence witness described in `resume215-xml-parser-map.md`.

## Candidate return and store order

In `source/apps/moproj2mst/src/moproj/read.cpp`, `openProject` first clears `p.openedOk` (line 31), then checks suffixes and returns false immediately for an unsupported suffix (lines 34–41). For accepted routes it sets the modeled flags and opens the data DB (lines 42–48). For `.moproj`, it assigns `p.openedOk = readProjectFile(p, ctx)` (line 50); it then performs current-directory/remesh tail work and returns `true` at line 63. Thus suffix acceptance and parser status are separate even when the parser returns false.

`readProjectFile` opens the project path and returns false if opening fails (lines 72–73), reads bytes and closes the file, stores the parsed DOM, returns false when `xml::setContent` fails (lines 77–83), and otherwise returns `walkProject`'s Boolean result (line 84). `openedOk` therefore reflects candidate file-open, XML parse, and top-level walk success. It does not by itself establish completion of all nested/lazy parsing or full GUI Open.

The B98 test in `source/apps/moproj2mst/tests/s/test_b98_open_only.cpp` calls `openProject` at line 94, stores its return in `acceptedProject`, then reads `project.openedOk` at line 95, then captures the scratch-tree manifest at line 96. Its output and assertions preserve both independently (lines 98–103). It checks the fixture hash and pre/post tree but is candidate-only. CP214 reports Release build and focused CTest passed, with `acceptedProject=true`, `openedOk=true`, and `treeUnchanged=true` for the 27-file fixture.

## Original `Project+0x55` contract

The copied-original static chain is specified in `source/docs/re/moproj2mst/s1-01/spec.md` sections 1–3 and mapped instruction-by-instruction in `analysis/resume215-xml-parser-map.md`:

1. `OpenProject` (`0x14008a9b0`, RVA `8a9b0`) initializes `P+0x55` to zero. On the `.moproj` branch it calls `ReadProjectFile` (`0x14008afa0`, RVA `8afa0`) and stores its returned AL into `[P+0x55]` (`8ae11`–`8ae1d`).
2. The common OpenProject tail independently returns literal AL=1 on the accepted path (`8af87`–`8af95`). Therefore a normal OpenProject return of true does not imply `P+0x55 == 1`.
3. Static `ReadProjectFile` control flow requires QFile open success, `QDomDocument::setContent` success, and `WalkProject` (`0x14008b410`, RVA `8b410`) true for a true result. The walk's true return is not a guarantee that ignored nested return values or lazy main-diagram parsing all succeeded.

The original contract is static evidence, not a runtime sample for CP214's B98 occurrence. CP214 observed suffix acceptance AL=1, a nonzero Project and synchronous postOpen activation, but no `P+0x55` read. Its report also records one empty `Lubricants` directory appearing in the clone; that side effect is not attributed to a completed XML read. The candidate's true `openedOk` must not be substituted for the original byte.

## Existing failure-path coverage and safe bounded increment

The candidate already tests the two outcomes independently on B98. Existing source tests also exercise the distinction: `tests/mdt/test_conversion_session.cpp` has a malformed-XML case that expects accepted `openProject` with `openedOk == false` (around lines 545–578), and a missing-file case that expects accepted Open with `openedOk == false` (around lines 579–588). `tests/p01/test_walk.cpp` includes suffix rejection and missing-path checks. Those cases support the candidate API contract without changing source behavior.

No candidate source change is justified by current evidence. If a small follow-up test is needed, the safe boundary is a candidate-only paired assertion using a fresh malformed or absent `.moproj` under `M2M_TEST_SCRATCH`: preserve `acceptedProject=true` and `openedOk=false`, and assert source fixture manifests remain unchanged. The existing conversion-session tests already cover these failure states, so this would mainly make the B98-specific output easier to compare; it would add no evidence about original runtime `P+0x55`.

The smallest original runtime increment remains the map's guarded one-byte read of the same `P+0x55` in the qualified `8a9b0` acceptance-return callback, after the `8ae1d` store and before outer return. It must record the raw byte and match the outer-return Project pointer. Until that independently reviewed capture exists, report original B98 `P+0x55` as unknown. Do not add hooks or infer the value from candidate state.

## Evidence order

The candidate test's local observation order is: initialize Project and registry; call `openProject`; capture its Boolean return; read `project.openedOk`; capture the copied project tree; print and assert the three independent results. The original static store order is: zero `P+0x55`; call the XML reader on the `.moproj` route; store its Boolean at `P+0x55`; continue OpenProject tail; return AL=1 for accepted suffix. CP214's original runtime order stops at the qualified postOpen endpoint and contains no field observation, so it cannot fill the missing value.
