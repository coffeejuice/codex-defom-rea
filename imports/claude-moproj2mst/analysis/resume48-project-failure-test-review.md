# Resume 48 project-failure test review

## Conclusion

The `{prior, empty}` final-output envelope cannot be narrowed to one state from the retained original static contract or documented Windows lock contract alone. Keep the two-state assertion for now, or add a same-host preflight that establishes the locked-truncation result on a disposable project copy. Do not assert `empty` or `prior` unconditionally until a candidate run on this host establishes which outcome occurs.

`FileDevice` opens with `OPEN_ALWAYS`, then calls `SetEndOfFile` at offset zero and intentionally ignores its return. The new test's external handle locks byte zero while permitting compatible opens. Windows documentation establishes that an exclusive byte-range lock denies other handles read/write access to the locked range, including a `WriteFile` crossing it; it also permits locking ranges beyond EOF. The cited docs do not specify whether `SetEndOfFile` on the second handle is denied when shrinking across a locked range. Since the production path discards that API result, source analysis alone cannot establish whether the preexisting MOPROJ survives truncation or becomes empty.

After that branch, the large serialized `WriteFile` begins at offset zero and overlaps the lock, so the direct write failure is the useful injected fault. The existing original static/FileDevice contract explains why the stream failure can still map to success: no QFile bytes remain pending; close's initial buffer flush succeeds, successful native close clears QFile.error, and the project writer samples that cleared value. This supports the test's continuation/error expectation, conditional on confirming the successful control really enters the direct-write path (which the test does by requiring serialized output over 16 KiB).

## Narrowing option

To assert one exact target state robustly on this host, use a disposable probe file with the same initial bytes, same filesystem, two handles and sharing modes, and the same byte-zero lock. Attempt `SetEndOfFile` through the second handle while the first owns the lock; record whether it succeeds. Restore/recreate the probe before conversion. The test can then require the post-Save target to equal the probe's resulting state, while still requiring no other bytes, M2M_OK, and later-stage outputs. A simpler candidate build/run may also establish one stable outcome, but retain the observed platform condition in the receipt rather than deriving it from API docs.

The existing `{prior, empty}` assertion is a reasonable bounded interim contract because it rejects any partial or unexpected replacement and reflects the only two source-level branches under discussion. It does not yet report which retained state this host produces. No source/test edits or builds were performed in this review.

Reference for the OS boundary: [LockFileEx documentation](https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-lockfileex) states that other processes are denied access to an exclusively locked byte range; [SetEndOfFile documentation](https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-setendoffile) documents truncation/extension and its return value but does not specify the interaction with an overlapping range lock.
