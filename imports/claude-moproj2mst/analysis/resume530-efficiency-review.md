# CP528–530 efficiency review

The CP528, CP529 and CP530 source-stable full builds measured 52.98,
179.39 and 53.19 seconds; their bound 387-test launchers measured 22.437,
22.563 and 22.313 seconds. Focused subproject cases grew from 63 to 65,
all passing in frozen candidates. Historical Save remained 93 SAME/five
DIFF with no coverage reduction.

CP529's shared model header caused 721 targets to rebuild. CP530's
parser-only cut returned to roughly 53 seconds for the full build. The
independent PE audit supplied a narrower type1 predicate that excludes
lazy default records without another header field. Token usage remains
unavailable. Preserve source-bound focused/full validation; additional
speed claims require measured improvements to the 22-second launcher and
the 53-second ordinary build, not the exceptional header rebuild.
