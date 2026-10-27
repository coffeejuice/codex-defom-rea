# CP494–496 safe-boundary efficiency review

The corrected CP494 full build took 45.968 seconds and bound launcher 22.516 seconds; CP495 took 46.063 and 26.344 seconds; CP496 took 30.844 and 24.422 seconds. Different changed-source sets and cache state prevent a controlled speedup claim. The earlier CP494 build was correctly left unfrozen after a PE audit found a graph-fidelity defect, and CP494r repeated validation only after correction. Token-use measurement is unavailable.

The CP495 source audit converted a literal x=1 leaf into a finite collision interval before the first build. CP497's audit grouped five free-column intervals into one planned implementation cut. Continue focused preflight, one source-bound full launcher and freeze per stable increment; the reduction in number of candidate checkpoints is methodological, not a measured tenfold wall-time result. Keep native notification and fresh-original qualifications visible.
