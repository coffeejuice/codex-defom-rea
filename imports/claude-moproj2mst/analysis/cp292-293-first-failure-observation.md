# First CP292-293 parallel CTest failure qualification

The initial combined CTest returned exit code 8 after 17.516 seconds. Its captured tool output identified test 242, m2m.mdt.terminal_harness_safety, with an assertion failure at test_terminal_harness_safety.cpp:235: result.transcript.started, inside terminal_deadline_covers_half_frames_and_stalls_after_complete_boundary. The test was unchanged by CP292-293. Its focused rerun passed 1/1 in 2.44 seconds; a later complete parallel CTest passed 389/389. Source fingerprint stayed stable across the validation.

The first run's compact receipt is analysis/cp292-293-first-failure.json. The first raw CTest log was overwritten by the successful rerun, so this note preserves the bounded failure line observed in tool output, not a full raw log. A load-sensitive deadline is plausible but unproven; no production or test source change was made in response.
