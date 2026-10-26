# CP120: smallest unnormalized resolution for the B98 path DIFF

## Choice and current status

This review follows the B:98_B_DS3F Diestress path row. CP110 still records it as one historical raw `Gear_blank.MST` difference with zero missing/extra files and no comparator adjustments. The difference is the absolute KFREAD path; the retained B98 MST is 2,756 bytes with SHA-256 `e69980305fc2ad704a1f44c57b723592af7494a5e97ab61826867a193efa27c1`. CP95 provides a candidate-only run whose raw MST is 2,721 bytes (SHA-256 `42a48184d865f1465a5d4a2f23ab935743cada14b9a8a16cf87a5170b416f947`); replacing exactly its one project-local key path in a separate comparison vector yields the retained historical bytes. The raw difference remains real. The transformed comparison is a diagnostic result, not raw parity.

The current candidate source remains aligned with the source-qualified original writer: Diestress derives a full DB path from the active subproject's project, changes `.DB` to `.KEY`, and emits that QString. Thus the candidate is expected to emit a different raw path when its project lives at a different root. No writer adjustment is supported.

## Smallest exact-byte comparison

To compare raw bytes without normalization, both executions must see the same project-path string. The smallest decisive acceptance is one fresh original Save and one candidate Save from equivalent pristine B98 copies located at the same disposable absolute path, with both raw MSTs retained and compared byte-for-byte. Reuse the same frozen input copy for sequential runs, restore it from a retained hash-verified pristine snapshot before the second run, capture the full output manifests, and keep outputs/logs outside the original retained evidence tree. This controls the path value without changing the comparator or transforming either output.

The old absolute root in the retained MST exists on disk, but running candidate WRITE_SAVE directly on that retained folder is not a safe substitute: the conversion path may write other Save side files into the input project tree. A candidate-only test under a different scratch root or the existing exact-path-substitution test cannot resolve raw equality by itself. Recreating the old root as an isolated disposable environment would permit a candidate-only byte check against the old retained MST, but it would still not bind the original historical action/session and is not equivalent to the matched-root fresh-original pair.

## Gate and evidence boundary

The matched-root pair requires launching the original application and recording its Save. Do not do so under this read-only task. The repository's independent recovery status currently records the GUI release lifecycle as unavailable (`blocked_external_current_validation` / `blocked_external_gui_release`), so there is no authorized, verifiably releasable GUI-control session for the needed original capture. The next actionable step is therefore to wait for a supported control-release operation plus a verifiable release receipt/status. After that gate changes, run the single matched-root pair above, preserving the raw outputs and original identity/action record. Until then, keep the B98 row as raw DIFF with the source-backed path explanation, not as fixed or parity-confirmed.

This read-only review used retained CP110/CP95 receipts and current candidate source. No source, test, build, fixture, original executable, GUI, or mutable REA state was changed or run.
