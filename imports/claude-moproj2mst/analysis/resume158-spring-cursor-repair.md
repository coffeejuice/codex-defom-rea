# CP158 SPRING2 q/question cursor repair

## Review repair

The first CP158 version tied the cursor model to a whole-file whitelist. After independent review in `analysis/resume158-spring-cursor-source-review.md`, the reader now gates only on the primary row count (one or two) and on the actual current marker byte `q` or `?`. After a valid numeric prefix, either marker is returned as the first unread byte regardless of later tail bytes. If another extraction starts there, it writes zero and leaves the marker in place; further requested reads repeat that result. This models why bytes after the marker are unreachable for the bounded one/two-row extraction count.

The repair preserves other malformed handling: `1z`, `1z?`, and `1x?` remain refused. Count three remains outside the q/question cursor profile. CP157 BOM decoding and negative overflow code were not changed.

## Focused regressions

Accepted inputs now include the prior one/two-row fixtures plus the review-identified tail cases:

- count one: `1q\r\n`, `1?\r\n` -> `[1]`; `q 2\r\n` -> `[0]`;
- count two: `1q\r\n`, `1?\r\n`, `1q 2\r\n`, `1? 2\r\n`, `1??\r\n` -> `[1,0]`; `q 2\r\n` -> `[0,0]`.

Every accepted case checks both input files remain byte-identical. Negative controls cover other marker `z`, an x/question combination not admitted by this branch, and the same q-leading bytes with a three-row primary.

## Static preflight and evidence

- Followed the cursor-based contract in `analysis/resume158-spring-cursor-source-review.md`; the decision now depends on the actual unread byte, not the entire file's spelling.
- Inspected the two callsites in the count-limited loop: the no-digit path returns zero without moving the cursor; the parsed-prefix path sets the cursor to the first unread marker.
- Checked owned source/test files for trailing whitespace. No build or CTest was run; no GUI or mutable REA state was used.
- This repair supersedes the exact-whole-file whitelist wording in `analysis/resume158-spring-cursor-implementation.md`. Status bits and arbitrary invalid characters remain outside the bounded q/question profile.

## Frozen hashes

- `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp`: `657685968B5296D519D87A7014735FF2BC76368C846FC25DEE9A4FEBA21DD7DA`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp`: `A84E6C758688782352ADCFE5954B7EE038119411B513FD98CAAA9AC7181AF752`
