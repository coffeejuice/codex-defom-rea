# Save I/O copy experiment

The isolated fixture `133_B_DBSTEP_BOOL3_before` was copied three times with the current `std::filesystem::copy(..., recursive)` and a manual recursive traversal. Candidate order alternated. Each paired output had identical path/type inventories, SHA-256 file contents and compared metadata.

| Copy method | Warm elapsed samples (ms) | Median (ms) |
| --- | --- | ---: |
| Existing recursive copy | 38.895, 39.629, 37.043 | 38.895 |
| Manual traversal/copy_file | 46.765, 50.393, 45.442 | 46.765 |

The manual candidate was **20.2% slower by median**, so it was not integrated. This scratch-only benchmark did not execute converter tests or establish a cold-cache result. Raw output is in `save-io-copy-benchmark.log`; the script removed only its verified unique scratch subtree.
