# CP110 diagnostic qualification correction

The worker ledger now serializes the actual model.qualification instead of hardcoding source-model-only. Its local formatter distinguishes SourceModelOnly, StaticDefaultConditional and DecodedValueWitnessOnly, with Invalid fallback. The admitted FreshDefault expectation now requires StaticDefaultConditional. Thus the explicit conditional startup assumption survives both model and consumed diagnostic; it is still not an observed original handler.

Only `source/apps/moproj2mst/tests/s/test_terminal_remesh_worker.cpp` changed. Frozen SHA-256:5eb9a042204ec78c6e106acd643285c774e23271eb26224ee58eb1d20987b61e. This supersedes worker hashab20892de025924f55433570f97d73325082a728cc3718c21f7cc6e240e7d605 in the original CP110 receipt. The three pure model/test files retain their frozen hashes.

Static checks verified actual enum formatting and literal expected qualification, eight existing cases/one main, supported Testkit macros and whitespace. Endpoint, wire protocol, status/failure handling, native prelude guard, public ABI/CLI and other files are unchanged. No build, test executable, GUI, original execution or mutable REA operation occurred. Source is released/frozen for root validation.
