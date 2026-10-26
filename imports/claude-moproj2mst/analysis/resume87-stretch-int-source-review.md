# CP87 independent review: STRETCH count-one integer route

No source blocker was found. The change extends the already reviewed private Qt integer helper to exactly one value from STRETCH.DAT, matching the retained shared native reader/caller evidence. Filename/count/ASCII/open gates remain bounded, and the later double-read path is unchanged. Root still owns build, execution and frozen acceptance.

Only this report was written. No source/test edit, build, executable test, GUI, original execution or mutable REA action occurred. Paths below are relative to `C:/Users/alext/Projects/codex-deform-rea/imports/claude-moproj2mst`.

## Bound source and plan

| Reviewed file | SHA-256 |
|---|---|
| analysis/resume87-stretch-int-successor.md | d0d958a8d40a4bede2fc3eab8bb1e4e0bbb875fb95ecfa0ea2df222c9dfb9438 |
| analysis/resume87-stretch-int-implementation.md | 836f01d41fb676f9bc1d49d1b61a183c14ffacfc96c75b09ea152261e4e11d1c |
| source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp | c60b3291b6306b256699ba671afc8bd466242e8dd51d5bce7f44226f802a96e5 |
| source/apps/moproj2mst/tests/mdt/test_simcontrol300_native_io.cpp | 08eebc85a6de0684fdd9a89ec292bcd44071845f56d6e58d2ef72db1023e7e46 |

The active source hashes match the implementation receipt. Comparison used the actual CP86 source archive: recorded source tree `828b26e6b5aff32844ded203ea7c38152be2bb795a252aee1aae91466a5bff36`, archive SHA-256 `b53fce602d69889ede80467a56497a8e260e4c4896009609e324447a280a1096`.

The Qt helper remains byte-identical to CP86, SHA-256 `7784b177f2df89c2ea8ba413a40b51b5a985a2d40d7f5cae0e21a976ed05a8d7`. simcontrol300_io.cpp also remains byte-identical, SHA-256 `3e796cc212cc02767485848eb67a6f8c44dd049161061ca01bd01a6eed45b7d4`.

## Native caller qualification

The retained original caller FUN_140ec8110 uses the same FUN_1410da690 integer reader with count 1 for the string at 0x142b14e90. The keyword receipt binds that VA to STRETCH.DAT and original executable SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. On successful open, the integer destination is formatted in decimal into the stretch-kind field. The shared reader initializes the destination, opens QFile ReadOnly, constructs the default QTextStream, extracts while not atEnd and below count, closes and returns open success without inspecting stream status.

Retained source bindings:

- wave2-save-diffs-300-support-140ec8110.c: `e531263876d6a9c2809f605f2ebee92ff04a526f29e7d97391560353a57ef97f`.
- wave2-save-diffs-300-support-1410da690.c: `c973d7d1db35ee26d84f5e8d24759c558b3d8463d4bb95e45e25ac5d62ee6352`.
- wave2-save-diffs-300-extra3d-keywords.json: `d6f9b6468cf0c6ffcbdead738371c0b1ae7378f6b2a97a81e9a081340bd660f2`.

The shared QtCore parser/default-C/one-extraction facts remain those independently checked in the CP83 correction and CP84 source review. This task adds no new PE execution or original-process comparison. It is a qualified additional caller to the same conversion contract, not a new conversion implementation.

## Exact source change and guards

The adapter diff contains only the one-value predicate extension/rename and a generalized unsupported diagnostic label. It now requires `values.size()==1` and exact path spelling either SW2SP.DAT or STRETCH.DAT before calling parseQt4Base0Int. It retains sourceQualified enforcement, fresh int mapping and caller-level open success. No basename/case-insensitive/substring match or global parser substitution was introduced.

Zeroing and file-open checks still precede the route. A zero-count request still returns after open without decoding. Nonempty requests still pass asciiBytes before dispatch, so high bytes/BOMs/undecoded input cannot enter the new helper. Other filenames, alternate path spellings and multiple-value spans remain on the legacy parser. SPRING.INI cannot match the new predicate; its count and populated-spring guard are unchanged.

The error text for an unqualified SW2SP shape changes from `editor300 SW2SP integer input outside recovered Qt profile` to `editor300 sidecar integer input outside recovered Qt profile`, reflecting the shared route. The unsupported code and refusal predicate are unchanged. Therefore the change is not byte-identical for that diagnostic, even though no broader decoding/open/count behavior changed. This is visible in the source diff and is not an original diagnostic claim.

## Later-double preservation

Native caller evidence separately invokes FUN_1410da510 for count 3 at the STRETCH.DAT literal 0x142b14ea0 after the integer/open result permits it. The candidate fillExtra3D retains the same shape: a count-one readInts for stretchKind, followed by a separate count-three readDoubles when stretchEnabled is true.

readDoubles and readReal are unchanged. Each readDoubles call opens its own stream, obtains its own ASCII bytes and begins its own cursor at zero. It neither inherits the integer helper's nextByte nor uses the integer value as a double. Consequently 0X10 producing integer 16 must not be described as making the later double parser parse hexadecimal 16. Existing finite-value/default-field handling in fillExtra3D also remains unchanged.

The focused added test invokes readInts directly; it does not execute the full form fill or assert later double values/UI state. Source identity proves the double code was preserved, while runtime page-state fidelity remains outside this new test's claim. No new later-double implementation change or additional test is needed solely to mirror this filename predicate change; root's existing full regression remains required.

## Focused test adequacy

The test delta is exactly one new Testkit case: native_extra3d_stretch_one_integer_uses_qt4_base0_profile. Removing that case yields the CP86 test source exactly after newline normalization. Case count increases from 19 to 20.

The new case writes exact 0X10 CRLF bytes into disposable scratch and requires successful one-value STRETCH read with value 16. It separately requires another filename to retain legacy value zero and a two-value STRETCH request to remain unsupported. Those negative-scope assertions are adapter isolation checks, not original multi-value equivalence. No skip or OK-or-unsupported allowance masks the selected one-value result.

All inspected quoted includes resolve against source-relative or actual component include directories. The test uses Release-active Testkit assertions, keeps existing main and introduces no production hook, public-header/C ABI change, fixture mutation or original app launch. The existing independent 20-row helper oracle remains intact; this route reuses that byte-identical helper.

Root can proceed with the focused native-IO build/test and exact full source-stable validation/archive. The helper's known conservative initial-text/sign-text refusals, fresh-only status model, ASCII/offset preconditions and original-runtime limitations remain qualified from CP84. No runtime pass, stronger page acceptance or completion is claimed here. Token measurements are unavailable.
