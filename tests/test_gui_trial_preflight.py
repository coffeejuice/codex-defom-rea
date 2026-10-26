"""Offline adversarial tests; never launches DEFORM or touches prepared trials."""
import importlib.util
from pathlib import Path
import tempfile
import unittest

MODULE = Path(__file__).resolve().parents[1] / 'tools/check_gui_trial_preflight.py'
SPEC = importlib.util.spec_from_file_location('gui_preflight', MODULE)
preflight = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(preflight)


class TrialPreflightTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory(prefix='gpf_')
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)
        self.metadata = dict(operations=1, steps=1, workers=1, deadline_seconds=120, remeshing=False)
        self.write('case.moproj', '''<Project>
          <Object_List><Object><Path>Objects/Object00001.KEY</Path></Object></Object_List>
          <Operation_List Number="1"><Operation><Id>1</Id><Type>SfOprHeatTransfer</Type>
          <Objects Number="1"><Object Id="1" Remesh="0"/></Objects>
          <Properties><Property><Name>DTMAX</Name><Value>0.1</Value></Property>
          <Property><Name>HeatingTime</Name><Value>0.1 sec</Value></Property></Properties>
          <UserDefinedKey/><RestartKey/></Operation></Operation_List>
          <Sub_Project Number="1"><DiagramItem Operation_Id="1"><Connectors>
          <Connections Number="0"/></Connectors></DiagramItem></Sub_Project></Project>''')
        self.write('case.MST', 'SIMULATION 1\nDIM 3\nNSTART -1\nKFREAD 1\nOperations/Task00001/SimCtrl.KEY\nKFREAD 1\nObjects/Object00001.KEY\n')
        self.write('Operations/Task00001/SimCtrl.KEY', 'NSTEP 1\nDTMAX 0.1 0\nTMAX 0.1\n')
        self.write('Objects/Object00001.KEY', 'NODTMP 1 1090\n')
        self.write('64bit.dat', '')
        self.write('JobGroupSetupConfigureFile.txt', 'nSimultaneousSimulations=1\nnCoreForEachSimulation=1\nComputerNum=0\nJobRunMode=E_RUN_ON_LOCALLY\n')

    def write(self, relative, content):
        path = self.root / relative
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(content, encoding='latin-1')

    def controller(self):
        return preflight.check_controller(self.root, self.metadata)

    def case(self, expected=None, **kwargs):
        return preflight.check_case(self.root, expected or preflight.inventory(self.root),
                                    controller=self.metadata, path_limit=1000, **kwargs)[0]

    def test_valid_is_prepared_not_accepted(self):
        result = self.case()
        self.assertEqual(result['errors'], [])
        self.assertEqual(result['status'], 'prepared_not_runtime_validated')
        self.assertFalse(result['runtime_acceptance'])
        self.assertFalse(result['execution_authorized_by_preflight'])

    def test_manifest_detects_modified_missing_and_extra_files(self):
        expected = preflight.inventory(self.root)
        self.write('case.MST', 'changed')
        (self.root / '64bit.dat').unlink()
        self.write('extra.txt', 'extra')
        result = self.case(expected)
        self.assertFalse(result['manifest_match'])
        self.assertEqual(result['manifest_differences']['changed'], ['case.MST'])
        self.assertEqual(result['manifest_differences']['missing'], ['64bit.dat'])
        self.assertEqual(result['manifest_differences']['unexpected'], ['extra.txt'])

    def test_p4p_mpi_file_rejected_even_when_frozen_in_manifest(self):
        self.write('DEF_MPIp4penv.DAT', '-env MPICH_SHMSIZE=52000000\nSIM2 4\n')
        result = self.case()
        self.assertTrue(result['manifest_match'])
        self.assertIn('DEF_MPIp4penv.DAT', result['controller']['facts']['stale_controller_inputs'])
        self.assertTrue(result['errors'])

    def test_incorrect_nstep_rejected(self):
        self.write('Operations/Task00001/SimCtrl.KEY', 'NSTEP 2\nDTMAX 0.1\nTMAX 0.1\n')
        self.assertTrue(any('NSTEP' in error for error in self.controller()['errors']))

    def test_later_include_cannot_silently_override_nstep(self):
        self.write('Objects/Object00001.KEY', 'NSTEP 20\n')
        self.assertTrue(any('NSTEP' in error for error in self.controller()['errors']))

    def test_one_hundred_character_generated_path_is_rejected(self):
        expected = preflight.inventory(self.root)
        baseline, _ = preflight.check_case(self.root, expected, path_limit=1000)
        limit = baseline['current_and_known_generated_max_path']
        bad, _ = preflight.check_case(self.root, expected, path_limit=limit)
        good, _ = preflight.check_case(self.root, expected, path_limit=limit + 1)
        self.assertTrue(bad['overlong_paths'])
        self.assertFalse(good['overlong_paths'])

    def test_missing_xml_reference_rejected(self):
        self.write('case.moproj', '<Project><Path>missing.KEY</Path></Project>')
        result = self.case()
        self.assertEqual(result['unresolved_xml_references'], [{'path': 'missing.KEY', 'resolved': False}])

    def test_outside_xml_reference_rejected_even_if_it_exists(self):
        self.write('case.moproj', '<Project><Path>../outside.KEY</Path></Project>')
        self.assertTrue(any('outside XML' in e for e in self.case()['errors']))

    def test_duplicate_worker_setting_rejected(self):
        path = self.root / 'JobGroupSetupConfigureFile.txt'
        path.write_text(path.read_text() + 'nCoreForEachSimulation=4\n')
        self.assertTrue(any('nCoreForEachSimulation' in e for e in self.controller()['errors']))

    def test_duplicate_xml_control_rejected_regardless_of_order_or_value(self):
        path = self.root / 'case.moproj'
        baseline = path.read_text()
        for name, valid, invalid in [('DTMAX', '0.1', '999'),
                                     ('HeatingTime', '0.1 sec', '999 sec')]:
            original = f'<Property><Name>{name}</Name><Value>{valid}</Value></Property>'
            conflicting = f'<Property><Name>{name}</Name><Value>{invalid}</Value></Property>'
            for replacement in (conflicting + original, original + conflicting, original + original):
                with self.subTest(name=name, replacement=replacement):
                    path.write_text(baseline.replace(original, replacement))
                    self.assertTrue(any(name in e for e in self.controller()['errors']))

    def test_multiple_values_in_one_xml_control_rejected(self):
        path = self.root / 'case.moproj'
        baseline = path.read_text()
        for name, valid in [('DTMAX', '0.1'), ('HeatingTime', '0.1 sec')]:
            with self.subTest(name=name):
                original = f'<Name>{name}</Name><Value>{valid}</Value>'
                path.write_text(baseline.replace(original, original + '<Value>999</Value>'))
                self.assertTrue(any(name in e for e in self.controller()['errors']))

    def test_remesh_enabled_rejected(self):
        path = self.root / 'case.moproj'
        path.write_text(path.read_text().replace('Remesh="0"', 'Remesh="1"'))
        self.assertTrue(any('remeshing' in e for e in self.controller()['errors']))

    def test_multi_operation_master_rejected(self):
        path = self.root / 'case.MST'
        path.write_text(path.read_text() + '\nSIMULATION 2\n')
        self.assertTrue(any('SIMULATION' in e for e in self.controller()['errors']))

    def test_invalid_deadline_rejected(self):
        self.metadata['deadline_seconds'] = 121
        self.assertTrue(any('deadline' in e for e in self.controller()['errors']))

    def test_historical_database_cannot_become_pass(self):
        self.write('case.DB', 'old database')
        self.write('case.MSG', 'old Done message')
        result = self.case()
        self.assertEqual(result['historical_outputs_excluded_from_new_result_evidence'], ['case.DB', 'case.MSG'])
        self.assertFalse(result['runtime_acceptance'])

    def test_static_two_dimensional_fixture_never_claims_gui_verification(self):
        self.write('RING.KEY', '* DEFORM-2D V11.0\nGEOTYP 1\nNSTEP 30\n')
        result, _ = preflight.check_case(self.root, preflight.inventory(self.root), dimension2=True, path_limit=1000)
        self.assertEqual(result['errors'], [])
        self.assertIn('GUI verification pending', result['dimension_qualification'])
        self.assertFalse(result['runtime_acceptance'])
        self.write('RING.KEY', '* DEFORM-2D V11.0\nGEOTYP 0\n')
        result, _ = preflight.check_case(self.root, preflight.inventory(self.root), dimension2=True, path_limit=1000)
        self.assertTrue(result['errors'])


if __name__ == '__main__':
    unittest.main()
