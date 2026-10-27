"""Bounded CP283 native creation-stop tests; only the fixed CP282 child runs."""
import hashlib
import json
from pathlib import Path
import unittest
from unittest.mock import patch

import color_create_event_probe as probe
import color_entry_supervisor as supervisor

ROOT = Path(__file__).resolve().parent.parent


class CreateEventProbeTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.receipts = {}
        for mode in ('normal', 'reject-image', 'held-entry', 'reject-create-event', 'no-ack'):
            receipt = supervisor.run_supervised(mode, backend='native-create-event')
            cls.receipts[mode] = receipt
            (ROOT / f'analysis/cp284-create-event-{mode}.raw.json').write_text(
                json.dumps(receipt, separators=(',', ':'), sort_keys=True) + '\n', encoding='utf-8')

    def test_fixed_image_binding(self):
        blob, pe = probe.fixed_fixture()
        self.assertEqual(hashlib.sha256(blob).hexdigest(), probe.FIXTURE_SHA)
        self.assertEqual(pe['entry_rva'], 0x1000)
        self.assertEqual(pe['entry_bytes'], probe.PREFIX.hex())

    def test_requires_supervisor_before_creation(self):
        with patch.object(probe.abi, 'create', side_effect=AssertionError('must not create')):
            with self.assertRaisesRegex(RuntimeError, 'independent fixed supervisor required'):
                probe.run_harmless()

    def test_original_unconditionally_refused(self):
        with patch.object(probe, 'ORIGINAL_RUN_ENABLED', True):
            with self.assertRaisesRegex(RuntimeError, 'HOLD'):
                probe.run_original()
        with self.assertRaises(ValueError):
            supervisor.run_supervised(backend='arbitrary.exe')

    def test_creation_stop_armed_before_continue(self):
        receipt = self.receipts['normal']
        worker = receipt['workerResult']
        self.assertEqual(receipt['errors'], [])
        self.assertTrue(receipt['childHandleAcknowledged'])
        self.assertTrue(receipt['deadlineStartedBeforeWorkerCreation'])
        self.assertTrue(worker['independentSupervisorEstablished'])
        self.assertEqual(worker['resumePreviousCount'], 1)
        self.assertTrue(worker['explicitKillOnExitChecked'])
        first = worker['events'][0]
        self.assertEqual(first['code'], 3)
        self.assertTrue(first['soleResumeAlreadyIssued'])
        self.assertTrue(first['armedBeforeSuccessfulContinuation'])
        self.assertTrue(first['continued'])
        self.assertTrue(all(worker['armChecks'].values()))
        self.assertTrue(worker['initialEventArmed'])
        self.assertEqual(worker['initialImage']['exportRva'], '0x1000')
        self.assertEqual(worker['createImageFile']['sha256'], probe.FIXTURE_SHA)
        self.assertEqual(worker['mappedHeaderImageBase'], worker['loadedImageBase'])
        self.assertTrue(all(296 <= d['offset'] < 304 for d in worker['headerDifferences']))

    def test_positive_entry_and_cleanup_separate(self):
        receipt = self.receipts['normal']
        worker = receipt['workerResult']
        self.assertTrue(worker['entryQualified'])
        self.assertTrue(all(worker['entry']['checks'].values()))
        self.assertTrue(worker['harmlessPassed'])
        self.assertTrue(worker['cleanupComplete'])
        self.assertTrue(receipt['independentChildExitVerified'])
        self.assertTrue(receipt['workerExitVerified'])
        self.assertEqual(receipt['workerExitCode'], 0)
        self.assertTrue(receipt['sourceStable'])
        self.assertTrue(all(r['closed'] for r in worker['handles']))
        self.assertTrue(all(e['continued'] for e in worker['events']))
        self.assertFalse(worker['observerWritesExecutableBytes'])
        self.assertFalse(worker['originalReady'])
        self.assertEqual(len(worker['entry']['rawContextHex']), 2464)

    def test_image_failure_before_arm_cleans_owned_child(self):
        receipt = self.receipts['reject-image']
        worker = receipt['workerResult']
        self.assertFalse(worker['entryQualified'])
        self.assertFalse(worker['initialEventArmed'])
        self.assertIn('fixed injected image gate refusal before arm', worker['errors'])
        self.assertTrue(worker['cleanupComplete'])
        self.assertTrue(receipt['independentChildExitVerified'])
        self.assertTrue(worker['events'][0]['continued'])

    def test_create_event_failure_after_arm_retains_pending_and_cleans(self):
        receipt = self.receipts['reject-create-event']
        worker = receipt['workerResult']
        self.assertEqual(receipt['errors'], [])
        self.assertTrue(receipt['childHandleAcknowledged'])
        injection = worker['createEventFailureInjection']
        self.assertTrue(injection['imageGatePassed'])
        self.assertTrue(injection['armReadbackPassed'])
        held = injection['pendingEvent']
        self.assertEqual((held['sequence'], held['code']), (1, 3))
        self.assertEqual((held['pid'], held['tid']), (worker['pid'], worker['primaryTid']))
        self.assertFalse(held['continued'])
        self.assertFalse(held['continuationAttempted'])
        self.assertEqual(held['continuationStatus'], 'not attempted at injection')
        self.assertEqual(held['disposition'], hex(probe.CONTINUE))
        self.assertTrue(worker['initialEventArmed'])
        self.assertTrue(all(worker['armChecks'].values()))
        first = worker['events'][0]
        self.assertTrue(first['continuationAttempted'])
        self.assertTrue(first['continued'])
        self.assertEqual(first['continuationStatus'], 'successful')
        self.assertEqual(first['disposition'], held['disposition'])
        self.assertIn('fixed injected CREATE_PROCESS failure after arm before continuation', worker['errors'])
        self.assertFalse(worker['entryQualified'])
        self.assertFalse(worker['harmlessPassed'])
        self.assertTrue(worker['termination']['ok'])
        self.assertEqual(worker['cleanupErrors'], [])
        self.assertTrue(worker['cleanupComplete'])
        self.assertIsNone(worker['pendingEvent'])
        self.assertTrue(all(e['continued'] for e in worker['events']))
        self.assertTrue(all(h['closed'] for h in worker['handles']))
        self.assertTrue(receipt['independentChildExitVerified'])
        self.assertTrue(receipt['workerExitVerified'])
        self.assertTrue(receipt['sourceStable'])
        self.assertLess(receipt['elapsedSeconds'], 9)
        with self.assertRaises(ValueError):
            supervisor.run_supervised('reject-create-event', backend='python-marker')

    def test_native_no_ack_independent_exit_does_not_imply_worker_cleanup(self):
        receipt = self.receipts['no-ack']
        self.assertFalse(receipt['childHandleAcknowledged'])
        self.assertTrue(receipt['independentChildExitVerified'])
        self.assertTrue(receipt['workerExitVerified'])
        self.assertFalse(receipt['workerCleanupComplete'])
        self.assertTrue(receipt['sourceStable'])
        self.assertEqual(receipt['errors'], [])

    def test_held_entry_abandoned_not_continued(self):
        receipt = self.receipts['held-entry']
        self.assertTrue(receipt['childHandleAcknowledged'])
        self.assertTrue(receipt['workerTermination']['ok'])
        self.assertTrue(receipt['workerExitVerified'])
        self.assertTrue(receipt['independentChildExitVerified'])
        self.assertIsNone(receipt['workerResult'])
        self.assertFalse(receipt['workerCleanupComplete'])
        self.assertFalse(receipt['pendingEvent']['continued'])
        self.assertEqual(receipt['pendingEvent']['continuationStatus'],
                         'abandoned by debugger-worker termination')
        self.assertLess(receipt['elapsedSeconds'], 9)

    def test_only_reserved_one_normalization_allowed(self):
        self.assertTrue(all(probe.validate_dr7_transition(1, 0x401).values()))
        self.assertTrue(all(probe.validate_dr7_transition(0x401, 0x401).values()))
        for changed in (0x403, 0x405, 0x10401, 0x1401, 0x400, 0):
            self.assertFalse(all(probe.validate_dr7_transition(1, changed).values()))
        self.assertFalse(all(probe.validate_dr7_transition(0x401, 1).values()))


if __name__ == '__main__':
    unittest.main()
