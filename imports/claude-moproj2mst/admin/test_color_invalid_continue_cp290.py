"""CP290 one fixed native child plus isolated FALSE/TRUE/unknown branch checks."""
import json
from pathlib import Path
import unittest
from unittest.mock import patch

import color_create_event_probe as probe
import color_entry_supervisor as supervisor

ROOT = Path(__file__).resolve().parent.parent


class InvalidContinueTests(unittest.TestCase):
    def held(self):
        return dict(sequence=1, code=3, pid=17, tid=19, continued=False)

    def test_false_retains_event_for_one_ordinary_continuation(self):
        row, calls = self.held(), []
        with patch.object(probe.abi, 'cont', side_effect=lambda *args: calls.append(args) or 0), \
                patch.object(probe.C, 'get_last_error', side_effect=lambda: calls.append('last-error') or 87):
            self.assertFalse(probe.invalid_continue_once(row, probe.CONTINUE))
        self.assertEqual(calls, [(17, 19, 0), 'last-error'])
        self.assertEqual(row['invalidContinuation']['immediateLastError'], 87)
        self.assertEqual(row['invalidContinuation']['savedDisposition'], '0x10002')
        self.assertFalse(row['continued'])
        self.assertFalse(row['continuationAttempted'])
        with self.assertRaisesRegex(RuntimeError, 'already attempted'):
            probe.invalid_continue_once(row, probe.CONTINUE)

    def test_true_marks_event_consumed(self):
        row = self.held()
        with patch.object(probe.abi, 'cont', return_value=1), patch.object(probe.C, 'get_last_error', return_value=999):
            self.assertTrue(probe.invalid_continue_once(row, probe.CONTINUE))
        self.assertTrue(row['continued'])
        self.assertTrue(row['continuationAttempted'])
        self.assertEqual(row['invalidContinuation']['outcome'], 'consumed')
        self.assertEqual(row['disposition'], '0x0')

    def test_unknown_retains_no_retry_state(self):
        row = self.held()
        with patch.object(probe.abi, 'cont', side_effect=RuntimeError('unknown native outcome')), \
                patch.object(probe.C, 'get_last_error') as last_error:
            with self.assertRaisesRegex(RuntimeError, 'unknown native outcome'):
                probe.invalid_continue_once(row, probe.CONTINUE)
        last_error.assert_not_called()
        self.assertEqual(row['invalidContinuation']['outcome'], 'unknown')
        self.assertTrue(row['continuationAttempted'])
        self.assertFalse(row['continued'])

    def test_gate_and_backend_refusals(self):
        with patch.object(probe.abi, 'cont') as cont:
            for update, disposition in (({'code': 1}, probe.CONTINUE), ({'sequence': 2}, probe.CONTINUE), ({}, 0)):
                row = dict(self.held(), **update)
                with self.assertRaisesRegex(RuntimeError, 'held initial'):
                    probe.invalid_continue_once(row, disposition)
            cont.assert_not_called()
        with self.assertRaises(ValueError):
            supervisor.run_supervised('invalid-continue-status', backend='python-marker')
        with patch.object(probe, 'ORIGINAL_RUN_ENABLED', True):
            with self.assertRaisesRegex(RuntimeError, 'HOLD'):
                probe.run_original()

    def test_native_false_return_and_owned_cleanup(self):
        receipt = supervisor.run_supervised('invalid-continue-status', backend='native-create-event')
        (ROOT / 'analysis/cp290-invalid-continue-status.raw.json').write_text(
            json.dumps(receipt, separators=(',', ':'), sort_keys=True) + '\n', encoding='utf-8')
        worker = receipt['workerResult']
        self.assertIsNotNone(worker)
        self.assertEqual(receipt['errors'], [])
        for key in ('childHandleAcknowledged', 'independentChildExitVerified', 'workerExitVerified', 'sourceStable'):
            self.assertTrue(receipt[key], key)
        self.assertEqual(receipt['workerExitCode'], 0)
        self.assertTrue(worker['invalidContinuePassed'])
        self.assertTrue(all(worker['armChecks'].values()))
        self.assertTrue(all(worker['invalidContinueGate'][key] for key in ('imageGatePassed', 'armReadbackPassed')))
        attempt = worker['invalidContinuation']
        self.assertFalse(attempt['boolResult'])
        self.assertEqual(attempt['outcome'], 'rejected')
        self.assertIsInstance(attempt['immediateLastError'], int)
        self.assertEqual(attempt['status'], '0x0')
        self.assertEqual((attempt['pid'], attempt['tid']), (worker['pid'], worker['primaryTid']))
        self.assertEqual([event['code'] for event in worker['events']], [3, 6, 5])
        first = worker['events'][0]
        self.assertTrue(first['continuationAttempted'])
        self.assertTrue(first['continued'])
        self.assertEqual(first['disposition'], attempt['savedDisposition'])
        self.assertTrue(worker['termination']['ok'])
        self.assertEqual(worker['cleanupErrors'], [])
        self.assertTrue(worker['cleanupComplete'])
        self.assertIsNone(worker['pendingEvent'])
        self.assertTrue(all(event['continued'] for event in worker['events']))
        self.assertTrue(all(handle['closed'] for handle in worker['handles']))
        self.assertFalse(worker['originalLaunched'])
        self.assertFalse(worker['originalReady'])
        self.assertFalse(worker['guiControlAcquired'])
        self.assertLess(receipt['elapsedSeconds'], 9)


if __name__ == '__main__':
    unittest.main()
