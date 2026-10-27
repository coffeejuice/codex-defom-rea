# CP407–412 efficiency review

At the safe CP412 boundary, CP407, CP409 and CP411 each used one new monitored inert occurrence from a clean frozen commit. None was accepted. The sequence identified an extra `conhost.exe` PID in CP407, rejected WORKER-only detachment in CP409, and passed the former strict accounting/ACK boundary with runner-only detachment in CP411. The original occurrences were retained and not repeated. CP412's next diagnostic is focused on the newly reached completion path rather than broadening the test.

The CP412 injected CP3xx suite passed 641 tests from each supported root; unittest execution reported about 0.45 seconds per root, and the surrounding shell command about three seconds. No new public C++ build, original comparison, end-to-end reconstruction speedup or token saving was measured. The public converter remains CP301 with 373 CTests and 93/98 historical comparator matches. The full recovery queue and eight completion gates remain open.

After the CP413 observation, diagnose only the first incomplete completion boundary. Preserve strict success, byte, architecture and cleanup criteria; no repeated uncertain occurrence is justified by this review alone.
