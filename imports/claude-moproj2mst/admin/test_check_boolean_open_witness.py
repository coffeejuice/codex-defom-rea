"""CP112 in-memory extractor tests. Synthetic frames are never native evidence."""
import copy
import json
import unittest

import check_boolean_open_witness as checker


def synthetic_frames():
    """Declared matching intervals exercise the checker, not original scheduling."""
    owners = {name: {'address': hex(0x1000 + i * 0x100), 'generation': 1}
              for i, name in enumerate(('window', 'project', 'area', 'impl', 'editor', 'opi'))}
    data = []
    for role, enter, leave, source in [
        ('open', 1, 8, 0x1ca30), ('update_data', 2, 7, 0xdae340),
        ('apply', 3, 6, 0x11e2870), ('add', 4, 5, 0x57ba0),
    ]:
        data.append({'kind': 'boolean_witness_frame', 'frame': {
            'role': role, 'frame_id': role, 'selected_parent_id': None if role == 'open' else data[-1]['frame']['frame_id'],
            'enter_ordinal': enter, 'leave_ordinal': leave, 'source_rva': source,
            'normal_return': True, 'read_errors': [], 'pid': 101, 'tid': 201,
            'module_sha256': checker.MODULES['DEF_GUI_MO_64.exe'], 'owners': copy.deepcopy(owners),
        }})
    del data[0]['frame']['normal_return']
    data[0]['frame'].update(boundary_kind='CliStartupBranchCompleted', completion_rva=0x1e4e,
                            normal_completion=True)
    data[1]['frame']['owner_links_observed'] = True
    data[2]['frame']['opi_guard_resolved'] = True
    data[3]['frame'].update(caller_return_rva=0x11e295a, rcx_role='OPI',
                            returned_object={'address': '0x9000', 'number': 3})
    return data


class BooleanWitnessExtractionTests(unittest.TestCase):
    def extract(self, packets):
        result = checker.extract_same_owner_frames(packets)
        self.assertFalse(result['witness_admitted'])
        return result

    def rejected_mutation(self, index, field, value):
        data = synthetic_frames()
        data[index]['frame'][field] = value
        result = self.extract(data)
        self.assertEqual(result['status'], 'rejected')
        self.assertTrue(result['errors'])
        self.assertFalse(result['structurally_complete'])

    def test_complete_synthetic_workpiece_chain_is_structural_only(self):
        result = self.extract(synthetic_frames())
        self.assertEqual(result['status'], 'structurally_complete_unadmitted')
        self.assertEqual(result['origin'], 'applyWorkpiece')
        self.assertTrue(result['structurally_complete'])

    def test_cutter_and_gui_have_distinct_supported_boundaries(self):
        data = synthetic_frames()
        data[3]['frame']['caller_return_rva'] = 0x11e2bcb
        data[0]['frame'].update(boundary_kind='GuiHandlerReturned', source_rva=0x271b0,
                                completion_rva=0x28514)
        result = self.extract(data)
        self.assertEqual(result['status'], 'structurally_complete_unadmitted')
        self.assertEqual(result['origin'], 'applyCutter')

    def test_labels_and_aggregate_object_count_supply_no_frames(self):
        result = self.extract([{'kind': 'observation', 'stage': 'boolean_apply_return',
                                'data': {'object_count': 3}}])
        self.assertEqual(result['status'], 'unobserved')
        self.assertEqual(result['missing_roles'], ['open', 'update_data', 'apply', 'add'])

    def test_retained_editor004_is_forming_and_has_no_typed_owner_chain(self):
        blobs = tuple(checker.contained_bytes(name) for name in
                      (checker.CAPTURE, checker.RECEIPT, checker.PROBE))
        result = checker.evaluate(*blobs)
        self.assertTrue(result['valid_preflight'])
        self.assertFalse(result['witness_admitted'])
        self.assertEqual(result['coverage']['observed_editor_kind'], 'Forming')
        self.assertEqual(result['coverage']['same_owner_frame_extraction']['status'], 'unobserved')

    def test_missing_apply_and_missing_return_remain_incomplete_or_rejected(self):
        data = synthetic_frames()
        del data[2]
        result = self.extract(data)
        self.assertEqual(result['status'], 'incomplete')
        self.assertIn('apply', result['missing_roles'])
        self.rejected_mutation(2, 'leave_ordinal', None)

    def test_foreign_owner_and_reused_address_with_new_lifetime_rejected(self):
        for role_index, owner in [(1, 'window'), (1, 'project'), (2, 'editor'), (3, 'opi')]:
            for change in ({'address': '0xbeef'}, {'generation': 2}):
                with self.subTest(role=role_index, owner=owner, change=change):
                    data = synthetic_frames()
                    data[role_index]['frame']['owners'][owner].update(change)
                    self.assertEqual(self.extract(data)['status'], 'rejected')

    def test_owners_need_observed_nonnull_address_and_lifetime(self):
        for value in ({'address': '0x0', 'generation': 1},
                      {'address': '0x1234', 'generation': 0},
                      {'address': '0x1234', 'generation': True},
                      {'address': 'unknown', 'generation': 1}, None):
            with self.subTest(value=value):
                data = synthetic_frames()
                data[2]['frame']['owners']['opi'] = value
                self.assertEqual(self.extract(data)['status'], 'rejected')

    def test_postload_origin_does_not_become_apply(self):
        result = self.extract([{'kind': 'boolean_witness_frame', 'frame': {
            'role': 'postload', 'caller_return_rva': 0x11df0ee}}])
        self.assertEqual(result['status'], 'incomplete')
        self.assertEqual(result['postload_origin_packets_unadmitted'], 1)
        self.assertIn('apply', result['missing_roles'])
        self.rejected_mutation(3, 'caller_return_rva', 0x11df0ee)

    def test_wrong_opi_role_or_object_return_rejected(self):
        for value in ('GuiModel', None):
            with self.subTest(role=value):
                self.rejected_mutation(3, 'rcx_role', value)
        for value in (None, {'address': '0x0', 'number': 3},
                      {'address': '0x9000', 'number': True}, {'address': 'invalid', 'number': 3}):
            with self.subTest(returned=value):
                self.rejected_mutation(3, 'returned_object', value)

    def test_loader_and_postopen_completion_cannot_stand_for_whole_open(self):
        for boundary, site in [('CliLoaderReturned', 0x1c94), ('CliPostOpenReturned', 0x1cae)]:
            with self.subTest(boundary=boundary):
                data = synthetic_frames()
                data[0]['frame'].update(boundary_kind=boundary, completion_rva=site)
                self.assertEqual(self.extract(data)['status'], 'rejected')
        self.rejected_mutation(0, 'completion_rva', 0x1c94)

    def test_cross_thread_process_and_module_chains_rejected(self):
        for key, value in [('pid', 102), ('tid', 202), ('module_sha256', 'f' * 64)]:
            with self.subTest(key=key):
                self.rejected_mutation(3, key, value)

    def test_source_parent_interval_and_frame_reuse_rejected(self):
        for index, key, value in [(2, 'source_rva', 0xeb26f0),
                                  (3, 'selected_parent_id', 'another-apply'),
                                  (3, 'frame_id', 'apply'), (3, 'leave_ordinal', 7),
                                  (2, 'leave_ordinal', 9), (2, 'enter_ordinal', 0),
                                  (2, 'enter_ordinal', True)]:
            with self.subTest(index=index, key=key):
                self.rejected_mutation(index, key, value)

    def test_read_errors_abnormal_return_and_unresolved_links_rejected(self):
        for index, key, value in [(2, 'normal_return', False), (3, 'read_errors', ['unreadable']),
                                  (3, 'read_errors', None), (1, 'owner_links_observed', False),
                                  (2, 'opi_guard_resolved', None)]:
            with self.subTest(index=index, key=key):
                self.rejected_mutation(index, key, value)

    def test_reentrant_or_duplicate_selected_frames_are_explicitly_unrepresented(self):
        data = synthetic_frames()
        extra = copy.deepcopy(data[2])
        extra['frame']['frame_id'] = 'reentrant-apply'
        data.append(extra)
        self.assertEqual(self.extract(data)['status'], 'rejected')

    def test_malformed_packets_reject_without_admitting(self):
        for frame in (None, [], {'role': []}, {'role': 'unknown'}):
            with self.subTest(frame=frame):
                self.assertEqual(self.extract([{'kind': 'boolean_witness_frame', 'frame': frame}])['status'], 'rejected')

    def test_injected_complete_frames_still_fail_capture_identity(self):
        blobs = tuple(checker.contained_bytes(name) for name in
                      (checker.CAPTURE, checker.RECEIPT, checker.PROBE))
        document = json.loads(blobs[0].decode('utf-8-sig'))
        document['normalized_result']['messages'].extend({'payload': p} for p in synthetic_frames())
        result = checker.evaluate(json.dumps(document).encode(), *blobs[1:])
        self.assertFalse(result['valid_preflight'])
        self.assertFalse(result['witness_admitted'])
        self.assertEqual(result['coverage']['same_owner_frame_extraction']['status'],
                         'structurally_complete_unadmitted')


if __name__ == '__main__':
    unittest.main()
