"""Focused benign one-slot debugger fixture checks."""
import os
import unittest

import color_one_slot_debug_fixture as fixture


class ColorOneSlotFixtureTest(unittest.TestCase):
    def test_original_execution_is_disabled(self):
        self.assertIs(fixture.ORIGINAL_RUN_ENABLED, False)
        self.assertEqual(fixture.VIEW, 0x1234567890ABCDEF)
        with self.assertRaises(ValueError):
            fixture.run('any-other-image')

    @unittest.skipUnless(os.name == 'nt', 'requires Windows debug events')
    def test_fixed_child_dynamic_return_handoff(self):
        result = fixture.run()
        self.assertTrue(result['passed'], result)
        self.assertFalse(result['originalLaunched'])
        self.assertFalse(result['guiControlAcquired'])
        self.assertEqual([h['stage'] for h in result['hits']],
                         ['2d-entry', '2d-return', '3d-entry', '3d-return'])
        self.assertEqual(result['hits'][0]['returnFromRsp'], result['hits'][1]['rip'])
        self.assertEqual(result['hits'][2]['returnFromRsp'], result['hits'][3]['rip'])
        self.assertTrue(all(h['dr6'] and h['tid'] == result['tid'] for h in result['hits']))
        self.assertTrue(result['hits'][3]['readback']['restored'])
        self.assertEqual(result['events'], result['continuations'])
        self.assertTrue(result['handlesClosed'])
        self.assertTrue(result['logicalHandlesClosed'])
        self.assertFalse(result['osInvalidHandlesVerified'])

    @unittest.skipUnless(os.name == 'nt', 'requires Windows debug events')
    def test_fixed_negative_cleanup_cases(self):
        expected = {
            'post-marker-thread': 'additional thread after marker rejected',
            'duplicate-marker': 'duplicate marker/reentry signal',
            'benign-exception': 'unexpected exception 0xe0424242',
            'timeout': 'fixed timeout bound',
        }
        for mode, error in expected.items():
            with self.subTest(mode=mode):
                result = fixture.run(mode)
                self.assertEqual(result.get('error'), error, result)
                self.assertTrue(result['negativePassed'], result)
                self.assertTrue(result['forcedCleanup'])
                self.assertTrue(result['ownedProcessSignaled'])
                self.assertTrue(result['exitObserved'])
                self.assertEqual(result['events'], result['continuations'])
                self.assertTrue(result['logicalHandlesClosed'])
                self.assertTrue(all(row['closed'] for row in result['handles']))
                self.assertEqual(len({row['generation'] for row in result['handles']}), len(result['handles']))
                self.assertFalse(result['osInvalidHandlesVerified'])
                self.assertFalse(result['originalLaunched'])


if __name__ == '__main__':
    unittest.main()
