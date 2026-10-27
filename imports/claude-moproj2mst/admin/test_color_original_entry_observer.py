"""Bounded harmless subprocess tests; no original, GUI, Frida or REA access."""
import copy
import json
from pathlib import Path
import subprocess
import sys
import unittest

import color_original_entry_observer as observer


class EntryObserverTest(unittest.TestCase):
    def child(self, mode):
        command = [sys.executable, '-I', '-S', str(Path(observer.__file__).resolve()), '--harmless', mode]
        completed = subprocess.run(command, capture_output=True, text=True, timeout=22, check=True)
        self.assertLess(len(completed.stdout), observer.MAX_RECEIPT + 65536)
        self.assertEqual(completed.stderr, '')
        result = json.loads(completed.stdout)
        self.assertFalse(result['originalLaunched'])
        self.assertFalse(result['originalReady'])
        self.assertFalse(result['independentSupervisorEstablished'])
        self.assertFalse(result['guiControlAcquired'])
        self.assertFalse(result['observerWritesExecutableBytes'])
        self.assertTrue(result['cleanupComplete'], result['cleanupErrors'])
        self.assertTrue(result['ownedProcessSignaled'])
        self.assertTrue(result['exitObserved'])
        self.assertTrue(result['logicalHandlesClosed'])
        self.assertTrue(all(e['continued'] for e in result['events']))
        self.assertTrue(all(h['closed'] for h in result['handles']))
        self.assertEqual(len({h['generation'] for h in result['handles']}), len(result['handles']))
        self.assertTrue(result['sourceStable'])
        self.assertTrue(result['creationFiletime'].isdigit())
        self.assertEqual(result['resumePreviousCount'], 1)
        self.assertFalse(result['designCreateEventBeforeResumeVerified'])
        self.assertFalse(result['initialEventArmed'])
        self.assertFalse(result['independentAbsenceVerified'])
        self.assertFalse(result['osInvalidHandlesVerified'])
        return result

    def test_original_is_literal_refusal_and_no_cli_bypass(self):
        self.assertIs(observer.ORIGINAL_RUN_ENABLED, False)
        with self.assertRaisesRegex(RuntimeError, 'HOLD'):
            observer.run_original()
        with self.assertRaises(ValueError):
            observer.run_harmless('arbitrary-image')
        command = [sys.executable, observer.__file__, '--original']
        result = subprocess.run(command, capture_output=True, text=True, timeout=3)
        self.assertNotEqual(result.returncode, 0)
        self.assertIn('original remains HOLD', result.stderr)

    def test_primary_entry_and_intentional_endpoint(self):
        result = self.child('normal')
        self.assertTrue(result['harmlessPassed'], result['errors'])
        self.assertTrue(all(result['entry']['checks'].values()))
        self.assertEqual(result['entry']['context']['Rip'], result['entry']['context']['Dr0'])
        self.assertEqual(result['entry']['prefixHex'], observer.PREFIX.hex())
        self.assertEqual(result['entry']['callHex'], 'e837000000')
        self.assertEqual(result['exitCode'], '0xe281')
        loader = [e for e in result['exceptions'] if e['classification'] == 'qualified-startup-loader-breakpoint']
        self.assertEqual(len(loader), 1)
        self.assertEqual(loader[0]['disposition'], hex(observer.CONTINUE))
        self.assertEqual(result['loaderModule']['sha256'], observer.NTDLL_SHA)

    def test_handled_first_chance_exception_reaches_entry(self):
        result = self.child('handled')
        self.assertTrue(result['harmlessPassed'], result['errors'])
        benign = [e for e in result['exceptions'] if e['code'] == '0xe0424281']
        self.assertEqual(len(benign), 1)
        self.assertTrue(benign[0]['firstChance'])
        self.assertEqual(benign[0]['disposition'], hex(observer.FORWARD))
        self.assertLess(benign[0]['sequence'], result['entry']['eventSequence'])

    def test_target_breakpoint_is_forwarded_to_own_handler(self):
        result = self.child('target-int3')
        self.assertTrue(result['harmlessPassed'], result['errors'])
        breaks = [e for e in result['exceptions'] if e['code'] == hex(observer.BREAK)]
        self.assertEqual(len(breaks), 2)
        self.assertEqual(breaks[0]['classification'], 'qualified-startup-loader-breakpoint')
        self.assertEqual(breaks[1]['classification'], 'forwarded-target-exception')
        self.assertNotEqual(breaks[0]['address'], breaks[1]['address'])
        self.assertEqual(breaks[1]['disposition'], hex(observer.FORWARD))

    def test_unselected_caller_rf_progress_to_selected_entry(self):
        result = self.child('wrong-caller')
        self.assertTrue(result['harmlessPassed'], result['errors'])
        self.assertEqual(result['wrongCallerCount'], 1)
        rejected = [e for e in result['exceptions'] if e['classification'] == 'owned-unselected-caller']
        self.assertEqual(len(rejected), 1)
        self.assertTrue(rejected[0]['readback']['EFlags'] & observer.RF)
        self.assertEqual(rejected[0]['readback']['Dr6'] & 0xe00f, 0)
        self.assertIn('no absence claim', rejected[0]['rfSuppressionInterval'])

    def test_worker_only_has_no_primary_witness(self):
        result = self.child('worker-only')
        self.assertFalse(result['entryQualified'])
        self.assertFalse(result['harmlessPassed'])
        self.assertEqual(result['errors'], [])
        self.assertEqual(result['exitCode'], '0x0')
        self.assertTrue(any(e['code'] == 2 for e in result['events']))

    def test_extra_thread_does_not_poison_positive_entry(self):
        result = self.child('extra-thread')
        self.assertTrue(result['harmlessPassed'], result['errors'])
        self.assertTrue(any(e['code'] == 2 for e in result['events']))
        self.assertEqual(result['entry']['context']['Dr0'], result['entry']['context']['Rip'])

    def test_natural_pre_entry_exit_is_no_witness(self):
        result = self.child('natural-exit')
        self.assertFalse(result['entryQualified'])
        self.assertEqual(result['errors'], [])
        self.assertEqual(result['exitCode'], '0x0')

    def test_observation_timeout_is_no_witness_and_owned_cleanup(self):
        result = self.child('timeout')
        self.assertFalse(result['entryQualified'])
        self.assertEqual(result['errors'], ['fixed harmless observation timeout'])
        self.assertTrue(result['termination']['ok'])
        self.assertEqual(result['exitCode'], '0xe281')

    def test_unhandled_second_chance_forwarding(self):
        result = self.child('unhandled')
        self.assertFalse(result['entryQualified'])
        self.assertTrue(result['unhandledSecondChance'])
        faults = [e for e in result['exceptions'] if e['code'] == '0xc000001d']
        self.assertEqual([e['firstChance'] for e in faults], [True, False])
        self.assertTrue(all(e['disposition'] == hex(observer.FORWARD) for e in faults))
        self.assertEqual(result['exitCode'], '0xc000001d')

    def test_entry_predicate_rejects_corrupt_bytes_frame_cause_and_reads(self):
        base = 0x100000
        snapshot = dict(Rip=base + 64, Rsp=0x20000, Rcx=0x30000,
                        Dr0=base + 64, Dr6=1, Dr7=1)
        good = dict(snapshot=snapshot, event_pid=11, pid=11, event_tid=22, tid=22,
            first=1, code=observer.STEP, exception_address=base + 64, base=base,
            call_bytes=observer.native_code()[4:9], prefix=observer.PREFIX,
            return_word=base + 9, view_sample=b'\x01' + b'\0' * 7,
            view_reread=b'\x01' + b'\0' * 7)
        self.assertTrue(all(observer.validate_entry(**good).values()))
        for field, value in [('event_pid', 12), ('event_tid', 23), ('first', 0),
                             ('base', base + 1), ('call_bytes', b'\xe8'),
                             ('prefix', b'wrong'), ('return_word', base + 25),
                             ('view_sample', b''), ('view_reread', b'\x02' + b'\0' * 7)]:
            with self.subTest(field=field):
                changed = copy.deepcopy(good)
                changed[field] = value
                self.assertFalse(all(observer.validate_entry(**changed).values()))
        for register, value in [('Rip', base + 65), ('Dr0', base + 65),
                                 ('Dr7', 0x10001), ('Dr6', 3), ('Dr6', 0x4001),
                                 ('Dr6', 0x2001), ('Dr6', 0x8001), ('Rcx', 0), ('Rsp', 0)]:
            with self.subTest(register=register, value=value):
                changed = copy.deepcopy(good)
                changed['snapshot'][register] = value
                self.assertFalse(all(observer.validate_entry(**changed).values()))

    def test_handle_generation_reuse_rejects_stale_close(self):
        ledger = observer.HandleLedger()
        first = ledger.own(100, 'file', 'manual')
        self.assertEqual(ledger.own(100, 'alias', 'manual'), first)
        ledger.closed(100, first, 'test')
        second = ledger.own(100, 'reused-file', 'manual')
        unrelated = ledger.own(200, 'unrelated', 'manual')
        self.assertNotEqual(first, second)
        with self.assertRaisesRegex(RuntimeError, 'stale handle generation'):
            ledger.closed(100, first, 'must-not-close')
        self.assertFalse(ledger.live[100]['closed'])
        self.assertEqual(ledger.live[200]['generation'], unrelated)


if __name__ == '__main__':
    unittest.main()
