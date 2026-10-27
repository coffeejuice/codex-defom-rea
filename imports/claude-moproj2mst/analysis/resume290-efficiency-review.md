# CP288-290 efficiency review

This safe-boundary review follows three frozen or independently reviewed checkpoints. CP288 installed-file acquisition took 52.3 ms and its 11 tests took 31 ms (ten pass, one Windows symlink-privilege skip). CP289's 26 disposable tests took 328 ms (25 pass, one symlink-privilege skip). CP290's five focused tests took 175 ms, including a 172 ms supervised native receipt. These are saved operation durations, not end-to-end checkpoint times.

CP290's first sandboxed child capture completed but could not write the receipt in the checkout; a separately authorized write reran the fixed capture. Before the next disposable capture, preflight the exact receipt destination for write access before launching the child. Preserve the same child ownership, cleanup, receipt and independent validation gates. Do not treat a preflight as evidence that later writes cannot fail.

The bounded 128 MiB session audit has no completed-turn records reliably attributable to these checkpoints. End-to-end wall time and token usage are unavailable; no overall speedup or token saving is claimed. The symlink skips reflect the OS privilege limit and have deterministic reparse-refusal coverage. GUI-control release remains unavailable; original application execution stays HOLD.
