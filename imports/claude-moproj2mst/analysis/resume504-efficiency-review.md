# Efficiency review after CP502–504

Measured successful build step elapsed seconds: CP502r configure12.063, focused build3.390, focused test0.688, full build0.641; CP503r 11.984,3.406,0.735,0.656; CP504 12.000,3.562,0.734,48.890. The short CP502r/503r full builds followed prior passing builds at the same source revision with only focused test changes; they do not imply a general build speedup. Bound launchers took CP502r21.078, CP503r21.406 and CP50421.406 seconds. Token expenditure was not measured.

Keep the focused preflight and one bound full launcher per final candidate. Reviewer-requested tests should be complete before the first build when practical; CP502/503 needed an extra build, but incremental full builds were cheap. Preserve source/evidence binding and logged gap inspection. Continue directly to CP505.
