import copy
import hashlib
import json
import tempfile
import unittest
from pathlib import Path
from check_continuation import evaluate, REQUIRED_GATES


class ContinuationTests(unittest.TestCase):
    def setUp(self):
        scratch = Path(__file__).resolve().parents[1] / 'build' / 'continuation-unit'
        scratch.mkdir(parents=True, exist_ok=True)
        self.temp = tempfile.TemporaryDirectory(dir=scratch)
        self.root = Path(self.temp.name)
        assert self.root.resolve().is_relative_to(scratch.resolve())
        self.addCleanup(self.temp.cleanup)
        (self.root / 'proof').write_text('test receipt')
        self.source_hash = 'a' * 64
        (self.root / 'analysis').mkdir()
        validation = self.root / 'analysis' / 'wave3-checkpoint57-validation.json'
        validation.write_text(json.dumps({'objective_complete': True}))
        candidate = self.root / 'build' / 'wave3-checkpoint57-verified'
        candidate.mkdir(parents=True)
        (candidate / 'receipt.json').write_text(json.dumps({
            'source_tree_sha256': self.source_hash,
            'validation': 'analysis/wave3-checkpoint57-validation.json',
        }))
        self.gate_evidence = {}
        for gate in REQUIRED_GATES:
            proof = self.root / 'analysis' / f'{gate}.json'
            payload = {'claim': gate}
            if gate == 'fresh_independent_comparison':
                payload.update(objective_complete=True, fresh_original_comparison=True)
            proof.write_text(json.dumps(payload))
            self.gate_evidence[gate] = [{
                'path': proof.relative_to(self.root).as_posix(),
                'sha256': hashlib.sha256(proof.read_bytes()).hexdigest(),
                'checkpoint': 57,
                'source_tree_sha256': self.source_hash,
                'claim': gate,
            }]
        self.item = dict(id='A', owner='root', status='verified', next_action='Retain evidence',
                         implementation_status='complete', integration_status='complete',
                         validation_status='complete', dependencies=[], blocker=None,
                         evidence=['proof'], acceptance_evidence=['proof'], required_validation=['Compare original'])
        self.state = {'resume_cursor': {
                          'root':'Verify final evidence',
                          'last_verified_validation':'analysis/wave3-checkpoint57-validation.json'},
                      'verified_candidate':'build/wave3-checkpoint57-verified/receipt.json',
                      'checks_receipt':'analysis/wave3-checkpoint57-validation.json',
                      'recovery_queue':[self.item], 'completion_gates':{
            k:dict(passed=True, evidence=self.gate_evidence[k]) for k in REQUIRED_GATES}}

    def check(self, state=None):
        return evaluate(self.state if state is None else state, self.root)

    def test_complete_record_requires_all_gates(self):
        self.assertTrue(self.check()['complete'])
        self.state['completion_gates'].pop(REQUIRED_GATES[0])
        self.assertFalse(self.check()['complete'])

    def test_interrupted_cursor_cannot_be_a_string(self):
        self.state['resume_cursor'] = 'Previously replaced object'
        self.assertEqual(self.check()['decision'], 'INVALID')
        self.assertFalse(self.check()['complete'])

    def test_tests_passed_does_not_close_unintegrated_work(self):
        self.item.update(status='active', integration_status='partial')
        self.assertEqual(self.check()['decision'], 'CONTINUE')
        self.item['status'] = 'verified'
        self.assertEqual(self.check()['decision'], 'INVALID')

    def test_local_blocker_does_not_stop_other_work(self):
        self.item.update(status='blocked', blocker='Need original callback target')
        other = copy.deepcopy(self.item)
        other.update(id='B', status='ready', blocker=None)
        self.state['recovery_queue'].append(other)
        self.assertEqual([x['id'] for x in self.check()['next_actions']], ['B'])
        self.assertFalse(self.check()['complete'])

    def test_missing_evidence_and_unknown_dependencies_fail_closed(self):
        self.item['evidence'] = ['absent']
        self.assertEqual(self.check()['decision'], 'INVALID')
        self.item['evidence'] = ['proof']
        self.item['dependencies'] = ['absent']
        self.assertEqual(self.check()['decision'], 'INVALID')

    def test_cycle_and_empty_queue_fail_closed(self):
        self.item['dependencies'] = ['A']
        self.assertEqual(self.check()['decision'], 'INVALID')
        self.assertEqual(self.check({'recovery_queue':[]})['decision'], 'INVALID')

    def test_unresolved_dependency_is_investigation_not_completion(self):
        self.item.update(status='blocked', blocker='Missing observed call edge')
        self.assertEqual(self.check()['decision'], 'INVESTIGATE_BLOCKERS')
        self.assertFalse(self.check()['complete'])

    def test_evidence_cannot_escape_converter_root(self):
        outside = self.root.parent / 'outside-proof'
        outside.write_text('sibling receipt')
        self.addCleanup(outside.unlink)
        self.item['evidence'] = ['../outside-proof']
        self.assertEqual(self.check()['decision'], 'INVALID')

    def test_latest_validation_must_match_current_receipt(self):
        self.state['checks_receipt'] = 'proof'
        self.state['resume_cursor']['last_verified_validation'] = 'different-proof'
        (self.root / 'different-proof').write_text('stale receipt')
        self.assertEqual(self.check()['decision'], 'INVALID')

    def test_passed_gate_requires_typed_hash_bound_evidence(self):
        gate = 'architecture_verified'
        evidence = self.state['completion_gates'][gate]['evidence'][0]
        evidence['sha256'] = '0' * 64
        result = self.check()
        self.assertEqual(result['decision'], 'INVALID')
        self.assertTrue(any('sha256 mismatch' in error for error in result['errors']))

    def test_passed_gate_must_match_candidate_checkpoint_source_and_claim(self):
        gate = 'lifecycle_integrated'
        evidence = self.state['completion_gates'][gate]['evidence'][0]
        evidence.update(checkpoint=56, source_tree_sha256='b' * 64, claim='other_gate')
        result = self.check()
        self.assertEqual(result['decision'], 'INVALID')
        self.assertTrue(any('checkpoint does not match' in error for error in result['errors']))
        self.assertTrue(any('source_tree_sha256 does not match' in error for error in result['errors']))
        self.assertTrue(any('claim must match gate' in error for error in result['errors']))

    def test_fresh_comparison_rejects_historical_incomplete_validation(self):
        gate = 'fresh_independent_comparison'
        stale = self.root / 'analysis' / 'historical-validation.json'
        stale.write_text(json.dumps({
            'objective_complete': False,
            'fresh_capture_in_this_chat': False,
            'historical_only': True,
            'historical': {'SAME': 93, 'DIFF': 5},
        }))
        record = self.state['completion_gates'][gate]['evidence'][0]
        record['path'] = stale.relative_to(self.root).as_posix()
        record['sha256'] = hashlib.sha256(stale.read_bytes()).hexdigest()
        result = self.check()
        self.assertEqual(result['decision'], 'INVALID')
        self.assertTrue(any('objective_complete is false' in error for error in result['errors']))
        self.assertTrue(any('historical-only' in error for error in result['errors']))
        self.assertTrue(any('lacks fresh_original_comparison: true' in error for error in result['errors']))

    def test_false_gates_remain_compatible_with_legacy_evidence(self):
        for gate in REQUIRED_GATES:
            self.state['completion_gates'][gate] = dict(passed=False, evidence=['legacy-unpinned-proof'])
        result = self.check()
        self.assertEqual(result['decision'], 'INVESTIGATE_BLOCKERS')
        self.assertFalse(result['complete'])
        self.assertEqual(result['pending_gates'], list(REQUIRED_GATES))
        self.assertEqual(result['errors'], [])


if __name__ == '__main__':
    unittest.main()
