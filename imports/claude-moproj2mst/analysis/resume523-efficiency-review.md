# CP520–523 efficiency review

The source-stable full build steps for CP520, CP521, corrected CP522r and
corrected CP523r measured 51.27, 51.58, 32.72 and 52.25 seconds. Their
bound 387-test launchers measured 24.344, 21.750, 22.625 and 21.765
seconds. Focused subproject cases grew from 55 through 58, all passing in
the final candidates. Historical Save remains 93 SAME/five DIFF.

CP522's first focused run found a stale `y=160` refusal fixture; CP523's
first focused run found four stale exclusions after the new positive-ID
direct route. Each initial failed run is retained. The corrected builds
were source-bound and independently run through the full launcher before
freeze. Neither failure established an original behavior mismatch.

The grouped preflight/full launcher kept regression coverage while allowing
read-only PE audits to overlap the build. A second read-only reviewer could
not access this checkout from its isolated visualization workspace; it made
no findings. Token usage is unavailable in local receipts. These timings do
not establish a new tenfold speedup; retain the current gates and continue
parallel read-only audits only when their workspaces can see the converter.
