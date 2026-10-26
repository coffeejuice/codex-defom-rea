# CP131: bounded SPRING2 static preflight

No probable compile/link/target issue or narrow admission discrepancy was found in the frozen one-row `0xA0` cut. This report currently covers that reader source only. Editor300's nonfinite successor remains evidence/report-only with no implementation assigned or frozen at this review boundary; it has not been accepted or reviewed as source here.

## Exact admission and regression checks

The native-reader diff against CP130's frozen source.zip adds only eight lines: require `allowSingleRowQualifiedToken`, compare exact four bytes `0xA0`, require EOF or ASCII whitespace after the token, then set cursor=start+4, clear comma-delimiter-pending and return160. The preceding `0xA`/`0XA` branch does not accidentally accept `0xA0` because it too requires its own following delimiter. The caller still supplies the new branch permission only for `count == 1 && i == 0`.

The test diff is +8/-1 lines: a positive one-row160 fixture with both file contents checked unchanged, plus guarded one-row neighbors `0xA1`, `0xA00`, `0xA0G`. The existing count-two refusal set retains `0xA0`. No constructor/access/path/API or numeric-library dependency changes. The test remains two cases containing bounded fixture rows, not a newly registered executable.

The value/cursor agrees with the retained Qt extraction review cited by the implementation and independent review: valid digits A0 yield160 and leave the following CR unread at byte4. This is a source/caller inference. The focused file-level test checks decoded flag and immutability, not a separately exposed cursor/status API or a new original-process differential. Neighbor refusals remain candidate coverage boundaries; they do not assert Qt rejects the corresponding broader valid/partial inputs. No generic hexadecimal grammar, case-folding expansion or overflow claim is added.

## Static build preflight

The existing test resolves its actual quoted includes and uses the established Rig, `qt::String(root.u16string())` path conversion and public Extra3DIo reader. The source is already in the recursive m2m_core glob; the test is already in the packet test glob. Correct target is `m2m_mdt_simcontrol300_spring_reader`, CTest `m2m.mdt.simcontrol300_spring_reader`. No manual registration or new library/source linkage is missing.

`python admin/check_test_source_preflight.py source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp` passed. A separate scan confirmed two unique valid identifier TEST_CASE arguments. This does not prove compiler instantiation or runtime behavior. Root owns builds, focused/full execution and freeze binding; this audit did not run them.

| Frozen file | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp` | `35cd7f6e8b9bad099cde74f07ead2f88ce3d93626df424b07372bf0aaba46035` |
| `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp` | `10c4c26ecdf44d95ba92f3997b94d381dd0fb8ec4df9fe08f5a8375d125bca53` |

These hashes independently match `analysis/resume131-spring-hex.md`. `analysis/resume131-spring-hex-review.md` records the retained Qt/source qualification. No production source, test, CMake, build, fixture, GUI or REA state was edited. Only this report was written. Token usage was unavailable.

## Optional editor successor

The source owner explicitly confirmed its CP131 nonfinite work was evidence-only at this boundary. A future assigned implementation needs a fresh hash-bound source/test review; its proposed MXCSR/bit-classification scope is not a source fact or passing preflight here. No conclusion in this reader audit widens editor nonfinite admission or closes a public editor/native-context gap.
