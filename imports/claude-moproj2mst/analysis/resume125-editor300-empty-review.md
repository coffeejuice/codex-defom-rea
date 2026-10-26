# CP125 independent review: Editor300 unchecked and empty Spring Save

## Result

For an explicit Spring page whose checkbox is off, or whose checked list has zero rows, the candidate preserves the native branch's meaningful behavior: earlier scalar sidecar operations remain first; it does not inspect row payloads or invoke the item callback; and it attempts readable deletion of `SPRING.INI` followed by `SPRING2.INI`, ignoring the first result and continuing. The new tests exercise those effects through the private `Extra3DPage` seam. They do not establish the real GUI signal/state path, and they were not built or run by the source owner.

One extra candidate state is worth keeping distinct: `form.spring == null` also falls into the deletion tail. Native evidence describes an existing page checkbox and list, not an absent optional value. The explicit off and zero-count cases are source-qualified; deletion for an absent optional page is a compatibility/candidate behavior unless its callers are separately shown to mean the native empty page.

## Native branch and ordering

The retained decompile `analysis/wave2-save-diffs-300-support-140ec9040.c` is tied to original `DEF_GUI_MO_64.exe` SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. It places the Spring decision after NBC, SW2SP, STRETCH, B_WEIGHT and STRAIN_DST processing. It reads list count, then reads the page checkbox. If unchecked or signed count is less than or equal to zero, it jumps directly to the cleanup branch. Only checked positive count obtains `currentItem`, invokes `0x140ecdee0` (which commits the currently displayed item), allocates/copies row payloads and reaches the primary writer.

At the cleanup branch the code calls `0x1407fbc10` for literal `SPRING.INI` and then for `SPRING2.INI`; the first result is not tested before the second call. The helper opens with `"rt"`; absent or unreadable files return without removal, while readable files are closed and passed to remove. The Spring caller ignores the deletion results and reaches its common return. This is an ordered, per-file helper path, not an atomic cleanup batch.

Candidate `applyExtra3D` keeps scalar handling before the final Spring branch. It creates `springSnapshot` only when the optional page exists, is checked, and has nonempty items. An explicitly unchecked or empty page leaves the snapshot absent. That branch invokes `removeIfReadable("SPRING.INI")` and then `removeIfReadable("SPRING2.INI")` without branching on the first result. It does not validate record finiteness, row indexes, combo labels, visible numeric text or flags, and it does not mutate the const page. Thus invalid ignored controls on the off/empty branch do not suppress earlier scalar effects or later Spring deletions.

The candidate's pure-struct gate evaluates page presence/check state before list emptiness, while native reads count before checkbox. Since these are inert stored fields in this seam, the difference has no side effect in the admitted profile. Do not generalize it to lazy getters or real widget event timing.

## Failure cases and evidence boundary

The new primary-delete-failure case uses deterministic `Extra3DIo` injection: main deletion reports failure, companion deletion still runs, the original main bytes remain, and Apply completes without an NBC warning. This demonstrates candidate continuation and avoids host-dependent sharing semantics. It does not establish a fresh native occurrence or physical deletion-failure residue. The earlier NBC write-failure case likewise verifies the adapter's prior action order followed by the two Spring removals; it makes no claim about native durability.

The cases that recheck a poisoned populated page and require the existing refusal preserve the checked-positive guard. That is correctly separate from the off/empty branch. Its earlier preflight still occurs before scalar side effects; the receipt explicitly does not claim native partial effects for that excluded checked-positive profile.

I found no discrepancy in the explicit unchecked or zero-row source path. The absent-optional-page behavior should stay labeled separately. The test receipt reports 19 Release-active cases and static preflight only; neither proves compilation nor a test pass. No build, test, GUI, original-process or mutable REA action was performed in this review.

## Frozen files

Reviewed against `analysis/resume125-editor300-empty-delete.md` (source-owner receipt SHA-256 `08d44a0337f8a8a88a86134889aa6e9e552118eb5c35d5eb064a33d46c7158da`):

- `source/apps/moproj2mst/src/engine/simcontrol300_io.cpp`: `720828fd1a77cef0e3b6fbb7201c40be86fb847fe603f2e2f638ef8075045ee0`
- `source/apps/moproj2mst/tests/s/test_spring_page_io_roundtrip.cpp`: `7e652d40e4e9463391a469fb61e0552d6110764cc087a1866ab08b7bc24a2df2`
