# Real-fixture hash benchmark

Bounded parallel hashing reduces the observed warm snapshot cost, but does not approach a tenfold speedup. Use two workers as a conservative opt-in pending the full-launcher equivalence check. The 2/4/8-worker results are close enough that two repetitions cannot establish a reliable winner.

The benchmark imported the existing serial `admin/run_converter_tests.py:snapshot` and unchanged `admin/fixture_hash_fast.py:snapshot`. It used the exact four fixture roots from `build/test-runs/1792854086489696400/inventory.json`: corpus, oracle B, database fixture, and reference executable. The reference executable was read as bytes only. Every pass covered 16,728 unique files / 1,867,669,827 bytes. The JSON companion records absolute roots, loaded-source SHA-256 identities, Python/platform, individual durations and ordered snapshot digest.

| Engine | First pass, s | Second pass, s | Median, s |
|---|---:|---:|---:|
| Legacy serial | 20.459 | 14.587 | 17.523 |
| 1 worker helper | 15.426 | 14.945 | 15.186 |
| 2 worker helper | 10.872 | 10.789 | 10.831 |
| 4 worker helper | 10.505 | 11.272 | 10.888 |
| 8 worker helper | 10.706 | 10.773 | 10.740 |
| 10 worker helper | 11.398 | 11.426 | 11.412 |

All 12 snapshots matched the initial serial result exactly: path keys, insertion order, byte sizes, nanosecond modification times and SHA-256 values. No persistent cache or sampling was used. The fixture files were read only; no original application, CTest or recovery work ran in this benchmark. The orchestrator reserved this interval from competing assigned heavy fixture I/O.

Run order was serial, 1, 2, 4, 8, 10 workers, followed by 10, 8, 4, 2, 1, serial. OS caches were not cleared; the first pass had uncontrolled cache state. The final warm serial took 14.587 seconds, compared with the two-worker median of 10.831 seconds: approximately 1.35× throughput and 25.8% less elapsed time. The first serial took 20.459 seconds, so comparing only against that pass would exaggerate the likely warm benefit.

Applying the warm difference twice gives an estimated 7.51 seconds saved across before/after snapshots. This is arithmetic from isolated measurements, not a measured full-launcher saving. Eight workers had the lowest pooled median by only 0.091 seconds relative to two workers; ten workers were slower. The extra concurrency has no demonstrated practical advantage here. Keep the serial default until full-run status, diagnostics and fixture manifests are verified. More paired repetitions would be needed for a robust small-difference ranking. No token savings were measured.

The runner integration occurred after this process imported its module. The recorded source hash identifies the loaded version; the integration owner confirmed that the legacy snapshot callable and helper remained unchanged.
