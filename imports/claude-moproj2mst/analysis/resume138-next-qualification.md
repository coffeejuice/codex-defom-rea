# CP138 successor wording correction

The CP138 freeze selected a CP139 geometry cut using the shorthand "width3 admission." The source-qualified field is **element metadata `E.u0f8 == 3`**, with packed-XY node metadata `N.u138 == 2`. Element width remains the existing independently checked positive width. The separate native allocation bound is `3 * nodeCount <= INT32_MAX`. `analysis/resume139-geometry-next.md` is the authoritative CP139 source contract. This corrects only the successor wording; CP138 source, build, tests, and frozen comparison receipts are unchanged.
