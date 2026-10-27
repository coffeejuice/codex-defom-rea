"""CP282 bounded harmless supervisor checks; no original process/control APIs."""
import json
from pathlib import Path
import subprocess
import sys
import unittest

import color_entry_supervisor as supervisor


class SupervisorTest(unittest.TestCase):
    def run_case(self, mode):
        command = [sys.executable, '-I', '-S', str(Path(supervisor.__file__).resolve()),
                   '--harmless-supervised', mode]
        child = subprocess.run(command, capture_output=True, text=True, timeout=12, check=True)
        self.assertEqual(child.stderr, '')
        self.assertLess(len(child.stdout), supervisor.MAX_IPC)
        result = json.loads(child.stdout)
        self.assertFalse(result['originalLaunched'])
        self.assertFalse(result['originalReady'])
        self.assertFalse(result['guiControlAcquired'])
        self.assertEqual(result['errors'], [])
        self.assertTrue(result['deadlineStartedBeforeWorkerCreation'])
        self.assertTrue(result['workerIdentity']['creationHandleOwnedBeforeSetup'])
        self.assertTrue(result['workerExitVerified'])
        self.assertTrue(result['workerHandleClosed'])
        self.assertTrue(result['sourceStable'])
        self.assertLess(result['elapsedSeconds'], 9)
        return result

    def test_normal_creation_identity_ack_and_entry(self):
        result = self.run_case('normal')
        self.assertTrue(result['childHandleAcknowledged'])
        self.assertTrue(result['independentChildExitVerified'])
        self.assertTrue(result['childHandleClosed'])
        self.assertTrue(result['workerCleanupComplete'])
        self.assertEqual(result['eventTail'], 'complete')
        worker = result['workerResult']
        self.assertTrue(worker['harmlessPassed'])
        self.assertTrue(worker['independentSupervisorEstablished'])
        self.assertTrue(worker['explicitKillOnExitChecked'])
        self.assertEqual(worker['resumePreviousCount'], 1)
        self.assertEqual(worker['creationFiletime'], result['childIdentity']['creationFiletime'])
        self.assertIsNone(result['workerTermination'])

    def assert_held(self, mode, event_code, disposition):
        result = self.run_case(mode)
        self.assertTrue(result['deadlineFired'])
        self.assertTrue(result['cooperativeCleanupRequested'])
        self.assertIn('ok', result['childTermination'])
        if mode == 'held-entry':
            self.assertTrue(result['childTermination']['ok'])
        self.assertTrue(result['workerTermination']['ok'])
        self.assertTrue(result['independentChildExitVerified'])
        self.assertFalse(result['workerCleanupComplete'])
        self.assertIsNone(result['workerResult'])
        self.assertEqual(result['pendingEvent']['code'], event_code)
        self.assertFalse(result['pendingEvent']['continued'])
        self.assertEqual(result['pendingEvent']['disposition'], disposition)
        self.assertEqual(result['pendingEvent']['continuationStatus'],
                         'abandoned by debugger-worker termination')
        self.assertEqual(result['eventTail'], 'unknown after debugger-worker termination')
        return result

    def test_selected_entry_hang_terminates_worker_then_independent_child_wait(self):
        self.assert_held('held-entry', 1, hex(supervisor.observer.CONTINUE))

    def test_exit_event_hang_is_abandoned_not_continued(self):
        self.assert_held('held-exit', 5, hex(supervisor.observer.CONTINUE))

    def test_missing_ack_never_resumes_and_owned_cleanup(self):
        result = self.run_case('no-ack')
        self.assertFalse(result['childHandleAcknowledged'])
        self.assertTrue(result['independentChildExitVerified'])
        worker = result['workerResult']
        self.assertIsNotNone(worker)
        self.assertNotIn('resumePreviousCount', worker)
        self.assertFalse(worker['entryQualified'])
        self.assertTrue(any('acknowledgement timeout' in e for e in worker['errors']))

    def test_pre_explicit_policy_hang_keeps_unverified_child_exit_unknown(self):
        result = self.run_case('pre-kill-hang')
        self.assertTrue(result['workerTermination']['ok'])
        self.assertFalse(result['childHandleAcknowledged'])
        self.assertFalse(result['independentChildExitVerified'])
        self.assertIn('unknown', result['childExitStatus'])
        self.assertIsNone(result['workerResult'])

    def test_arbitrary_target_and_original_stay_refused(self):
        with self.assertRaises(ValueError):
            supervisor.run_supervised('arbitrary-pid')
        with self.assertRaisesRegex(RuntimeError, 'HOLD'):
            supervisor.observer.run_original()


if __name__ == '__main__':
    unittest.main()
