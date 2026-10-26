# CTest folder scheduling metadata

The comparator permits one directional inventory delta: checkpoint-45 has no `COST` property on `m2m.s.save_folder`, and the candidate adds exactly `{"name":"COST","value":25}`. Numeric JSON `25.0` is equivalent to `25`. A string value, different cost, duplicate property, additional property fields, cost removal, or a cost on another test remains a blocker.

This property changes the scheduler's estimated test cost so the long folder sweep can start earlier. It may change scheduling order, contention and measured elapsed time. It does not waive comparison of test membership, commands, environments, working directories, labels, other properties, fixture manifests, statuses, gaps, folder results or raw semantic case output.

When the permitted addition occurs, the report explicitly lists it in `permitted_differences` and adds a scheduling qualification under `qualifications`. Only the candidate's exact approved `COST` property is removed from the temporary semantic inventory view. The original inventory file is untouched. Existing `raw_differences` retains paths and SHA-256 hashes for the raw baseline/candidate inventory and every other changed artifact.

Focused Python checks loaded the preserved 150-test checkpoint-45 baseline and the existing isolated serial run `1792859270669970100`. Their prior comparison remained equivalent. An in-memory `COST=25.0` addition was accepted and reported with the qualification. Eleven counter-mutations were rejected: costs 24, 26, string `25`, and 0; duplicate COST; unknown COST object field; COST on another test; TIMEOUT addition; environment change; test removal; and reversal of the approved addition. Both raw inventory SHA-256 values were independently checked against the preserved files. In-memory mutation checks do not create a new run receipt or claim that a newly scheduled candidate has been executed.

No full suite, build, original executable or fixture mutation was performed for this comparator change. Equivalence of a future scheduled run remains subject to the complete normal comparator invocation and retained evidence.
