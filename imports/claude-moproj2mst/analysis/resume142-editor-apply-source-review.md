# CP142 independent solver6 Apply source review

No blocking discrepancy found in the released implementation, nine new Apply cases or migrated Fill guard. The code follows the CP141 bounded admission and sequential model-write contract; it does not admit the public editor constructor or unobserved edit history. Root also corrected the shared header's old blanket solver6-Apply exclusion comment before this review freeze.

## Exact native3D binding and writes

Reviewed against `analysis/resume141-solver6-apply-next.md` SHA-256 `2894bbf8e743a72423431c12dabb32f181697d3918ed7ff3487899f3bba37849`, including retained ASM/PE qualifications. Native130ea0 means dimension==2, so3D follows the later Apply leg35ec. The new source comment correctly names explicit count4 at38c9 and toDouble calls3919/3983/39ed/3a57. Their native stores3927/3991/39fb/3a65 precede solver/method/thermal3be8/3bfa/3c0c and convergence validation3c7f/3d42. These are the3D addresses, not the earlier2D block.

The existing count0 assignment remains before the explicit count4 assignment. A for-loop then calls qt::toDouble once per index0..3 and stores that result immediately. It neither preconverts an array nor substitutes original raw doubles for formatted texts. Last-two Apply values remain doubles rather than being constrained by integer Fill formatting. The selection/validation tail follows those four stores and remains in native order: Invalid convergence0 preserves the complete parameter/selection prefix; Invalid1 additionally preserves cvgerr0; bandwidth/iteration stores follow only after both avoid Invalid. Intermediate retains the existing non-Invalid path and Invalid iteration fields default independently to200. No lifecycle/global-Save continuation policy was changed.

## Preflight and boundary

The new lexical helper checks characters/counters only. It accepts exactly the proposed signed ASCII decimal/scientific grammar: one optional point,1..15 total mantissa digits, optional e/E and signed1..2-digit exponent of magnitude<=32, maximum length21. Empty/null, sign/point-only, missing exponent digits, duplicate point, extra signs, whitespace, special/radix/non-ASCII/NUL characters and excessive lengths/exponents refuse. Its control/index operations are bounded, including sign-only inputs. The admitted nonzero magnitudes avoid overflow/underflow; no floating conversion is executed to decide admission.

All four optionals/profile checks occur before count or any other model write and only for solver6. These are explicitly candidate profile guards, not native parameter validation. Native conversions pass a null success pointer and have no parameter-validation stop. Unknown non6→6 texts and wider numerical domains remain refused. No visibility predicate, default-text fill, model/context lookup or API widening is added. Existing non6 branches ignore the new text profile. Native widget/QString/validator allocation/EH, locale-global and FP/TLS behavior remain unclaimed by this synchronous private value projection.

## Test assertions and prior fixture

The new actual bound-page Fill→Apply case verifies count0/77 becomes4 and source-g6 rounded text values, rather than pre-Fill raw doubles, are applied. Other cases prove fractional last-two doubles, hidden/unknown visibility independence, accepted lexical boundaries/signed-zero bits, every missing/excluded text position guarding before writes, both convergence partial returns, Intermediate/default200 behavior, unchanged non6/CG behavior and existing radio preflight. All are Release-active Testkit assertions with one main.

The old Fill case was migrated to explicit missing text2 and excluded text3=`1e33`, retaining strong unchanged count/parameter/public-owner assertions; qualified success is tested separately. No valid assertion was merely removed to accommodate the change. The old toggle refusal remains appropriate with its unknown explicit texts. Root's shared comment now accurately says solver6 Apply needs four known bounded texts. Palette6504abb0 remains the actual Session-constructor guard and completeForPublicSave remains false. No required correction or regression addition identified.

## Frozen hashes

All three source/test hashes match `analysis/resume142-editor-explicit-apply.md`, SHA-256 `9622e38b772b326f6f9d17128338552929a288b7769c32827d4d9d5924e02e4e`:

- simcontrol300.cpp: `5029294067d02890a01726d518664dbb9cb65504ee54cc3c4ead46fdeade30d0`.
- New explicit_apply test: `d48b4ca9aeb992b483727df9ec13e363595f96b269cb73d4efae1f9e1609c307`.
- Migrated explicit_fill test: `e5ca2a5b5b778da5fe25fc257d9502ed6be17d49f7aaae7dd3db3fae2f10674e`.

Root-owned simcontrol300.hpp observed after documentation correction: `6cc6ac1d0109c542c345ced6a493ea22ee3306fb9c7ed4f96f12e16cd518dbf9`.

Root owns compilation and focused/full execution. Only this report was written; no source/test edits, build/test run, GUI/original execution or mutable REA action occurred.
