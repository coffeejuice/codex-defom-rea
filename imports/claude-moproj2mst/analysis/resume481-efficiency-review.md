# CP479–481 safe-boundary efficiency review

The source-stable full builds took CP479r 44.687 seconds, CP480 44.625 seconds and CP481r 86.609 seconds. The CP480 and CP481 bound launchers took 21.282 and 21.000 seconds. CP481's first focused run failed on a test-only counter and was repaired before its full launcher. The review lane caught assertion timing errors, so the final full run did not need repetition. These timings have different compile workloads and are not a controlled acceleration measurement. Token-use evidence was unavailable; no token saving is claimed.

Keep focused preflight, one source-bound full launcher, and a frozen candidate per checkpoint. The next action is the private typed Stream/FileDevice bridge; no validation gate is relaxed.
