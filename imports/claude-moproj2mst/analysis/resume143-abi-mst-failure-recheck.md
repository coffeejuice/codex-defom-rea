# CP143 denied-MST ABI test path repair recheck

## Result

The prior long-path blocker is repaired in the current test source. Before any `create_directories` or `copy`, it requires `C:\PROBLEM` to exist as a directory, constructs a unique leaf directly beneath it, refuses a pre-existing leaf, enumerates every retained fixture entry and rejects any corresponding generated path of 100 or more native path units. It also explicitly checks the project, default MST, `Boolean.BWZ`, and `DEF_MACHDIST_CTRL2.INI` destinations. The checked test source hash is `1F6EC07F587487168B93D3EFDA8AB3F5AF7823B1F49A23B40B3311920DE94084`.

I independently inspected the current `C:\PROBLEM` directory read-only; it is a directory and is not presently marked as a reparse point. The unique path is not routed through `M2M_TEST_SCRATCH`, avoiding the checkout build path. The `native().size() >= 100` comparison is the appropriate Windows wide-path length test. The test will stop before writes if any fixture-derived path or explicit output exceeds the bound. I did not run the test.

## Failure mechanism unchanged and sound

`DenyWriteOpen` still opens the copied pre-existing MST for `GENERIC_READ` with `FILE_SHARE_READ` and retains the handle across the ABI call. This allows the later read-only assertions and denies the converter's `GENERIC_WRITE` open because the existing handle's share mask omits `FILE_SHARE_WRITE`. Candidate `FileDevice` uses `OPEN_ALWAYS`, so denial occurs before truncation; `convert.cpp` maps that open failure to `M2M_E_WRITE` and `cannot open MST `, which are asserted exactly. The test asserts unchanged copied MST bytes while locked, presence of both prior Boolean side-file markers, changed copied project bytes, and unchanged CWD. The fixture source is recursively copied; no write target is constructed from its source path, and no original application/GUI control is involved.

## Residual note

The code checks that `C:\PROBLEM` is a directory but does not itself reject a reparse-point root. The current inspected directory is not one, so this is not a present blocker; a future environment that replaces that root with a junction would warrant an explicit reparse/canonical-path guard. The previous review's path-policy finding is superseded by this repair. No source, build, GUI, or mutable REA state was changed or run in this review.
