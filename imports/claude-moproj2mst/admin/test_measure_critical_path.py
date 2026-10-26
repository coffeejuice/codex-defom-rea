"""Focused tests for receipt-only timing summaries and worker DAGs."""

import unittest

from measure_critical_path import EVENT_SCHEMA, make_report, validate_event_file


HASH = "a" * 64


def events():
    return {
        "schema": EVENT_SCHEMA,
        "checkpoint": "CPX",
        "clock_domain": "host_monotonic_seconds",
        "source_tree_sha256": HASH,
        "events": [
            {"id": "worker-a", "role": "worker", "start_seconds": 0, "end_seconds": 4},
            {"id": "worker-b", "role": "worker", "start_seconds": 1, "end_seconds": 5},
            {"id": "integrate", "role": "orchestrator", "start_seconds": 5,
             "end_seconds": 7, "depends_on": ["worker-a", "worker-b"]},
        ],
    }


class CriticalPathTests(unittest.TestCase):
    def test_measures_overlap_and_dependency_critical_path(self):
        result = validate_event_file(events(), checkpoint="CPX", source_sha256=HASH)
        self.assertEqual(result["observed_wall_span_seconds"], 7)
        self.assertEqual(result["worker_busy_seconds"], 8)
        self.assertEqual(result["worker_busy_over_wall_span"], 1.142857)
        self.assertEqual(result["peak_concurrent_workers"], 2)
        self.assertEqual(result["critical_path_wall_seconds"], 7)
        self.assertEqual(result["critical_path_active_seconds"], 6)
        self.assertEqual(result["critical_path_event_ids"], ["worker-a", "integrate"])

    def test_missing_worker_events_are_unavailable_not_guessed(self):
        report = make_report(test_receipt={
            "elapsed_seconds": 3,
            "registered_tests": 1,
            "counts": {"passed": 1},
            "execution_binding": {},
        })
        self.assertEqual(report["worker_overlap"]["status"], "unavailable")
        self.assertIn("No worker event receipt", report["worker_overlap"]["reason"])

    def test_event_source_must_match_test_binding(self):
        receipt = {
            "elapsed_seconds": 2,
            "registered_tests": 1,
            "counts": {"passed": 1},
            "execution_binding": {
                "source_tree_before": {"sha256": HASH},
                "source_tree_after": {"sha256": HASH},
                "stable_during_test": True,
            },
        }
        document = events()
        document["source_tree_sha256"] = "b" * 64
        with self.assertRaisesRegex(ValueError, "does not match"):
            make_report(test_receipt=receipt, events_document=document, checkpoint="CPX")

    def test_rejects_dependency_overlap(self):
        document = events()
        document["events"].append({"id": "bad", "start_seconds": 3, "end_seconds": 6,
                                   "depends_on": ["worker-a"]})
        with self.assertRaisesRegex(ValueError, "ends after"):
            validate_event_file(document)

    def test_rejects_wrong_checkpoint_and_clock(self):
        with self.assertRaisesRegex(ValueError, "checkpoint"):
            validate_event_file(events(), checkpoint="CPY")
        document = events()
        document["clock_domain"] = "wall_clock"
        with self.assertRaisesRegex(ValueError, "clock domain"):
            validate_event_file(document)


if __name__ == "__main__":
    unittest.main()
