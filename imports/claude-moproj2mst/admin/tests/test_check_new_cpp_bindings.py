"""Focused static tests for new C++ source-binding checks."""

import sys
import tempfile
import unittest
from pathlib import Path

ADMIN = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ADMIN))

import check_new_cpp_bindings as checker


class NewCppBindingTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)
        self.project = self.root / "source/apps/moproj2mst"
        self.test_dir = self.project / "tests/p09"
        self.test_dir.mkdir(parents=True)
        self.include = self.project / "include"
        (self.include / "m2m").mkdir(parents=True)
        (self.include / "m2m/local.hpp").write_text("#pragma once\n", encoding="utf-8")

    def test_testkit_int_main_and_configured_m2m_include_pass(self):
        test = self.test_dir / "test_ok.cpp"
        test.write_text('#include <m2m/local.hpp>\nint main(){ return testkit::run_all(); }\n',
                        encoding="utf-8")
        result = checker.check_files(self.project, [test], [self.include])
        self.assertTrue(result["ok"], result)

    def test_testkit_wmain_is_accepted(self):
        test = self.test_dir / "test_windows.cpp"
        test.write_text('int wmain(){ return testkit::run_all(); }\n', encoding="utf-8")
        result = checker.check_files(self.project, [test], [self.include])
        self.assertTrue(result["ok"], result)

    def test_missing_testkit_main_is_reported(self):
        test = self.test_dir / "test_no_main.cpp"
        test.write_text("#include <testkit/check.hpp>\nTEST_CASE(one) {}\n", encoding="utf-8")
        result = checker.check_files(self.project, [test], [self.include])
        codes = {issue["code"] for row in result["files"] for issue in row["issues"]}
        self.assertIn("missing_test_main", codes)

    def test_main_without_run_all_is_reported(self):
        test = self.test_dir / "test_no_run_all.cpp"
        test.write_text("#include <testkit/check.hpp>\nint main(){ return 0; }\n", encoding="utf-8")
        result = checker.check_files(self.project, [test], [self.include])
        codes = {issue["code"] for row in result["files"] for issue in row["issues"]}
        self.assertIn("missing_testkit_run_all", codes)

    def test_non_testkit_legacy_harness_main_is_not_mislabeled(self):
        test = self.test_dir / "test_legacy_harness.cpp"
        test.write_text("int main(){ run_legacy_checks(); return 0; }\n", encoding="utf-8")
        result = checker.check_files(self.project, [test], [self.include])
        self.assertTrue(result["ok"], result)

    def test_unresolved_m2m_include_is_reported(self):
        test = self.test_dir / "test_missing_include.cpp"
        test.write_text('#include "m2m/not_here.hpp"\nint main(){ return testkit::run_all(); }\n',
                        encoding="utf-8")
        result = checker.check_files(self.project, [test], [self.include])
        issues = [issue for row in result["files"] for issue in row["issues"]]
        self.assertEqual([issue["code"] for issue in issues], ["unresolved_m2m_include"])
        self.assertEqual(issues[0]["line"], 1)

    def test_changed_source_cpp_unresolved_m2m_include_is_reported(self):
        source = self.project / "src/doe"
        source.mkdir(parents=True)
        cpp = source / "output_regions.cpp"
        cpp.write_text('#include "m2m/mst/p17.hpp"\n', encoding="utf-8")
        result = checker.check_files(self.project, [cpp], [self.include])
        issues = [issue for row in result["files"] for issue in row["issues"]]
        self.assertEqual([issue["code"] for issue in issues], ["unresolved_m2m_include"])

    def test_standalone_directive_missing_item_owner_is_reported(self):
        test = self.test_dir / "test_emit.cpp"
        test.write_text("// ctest-standalone: src/mst/emit_b_doe.cpp src/core/basic.cpp\n"
                        "int main(){ return testkit::run_all(); }\n", encoding="utf-8")
        result = checker.check_files(self.project, [test], [self.include])
        codes = {issue["code"] for row in result["files"] for issue in row["issues"]}
        self.assertIn("standalone_missing_item_owner", codes)

    def test_standalone_directive_with_item_owner_passes(self):
        test = self.test_dir / "test_emit.cpp"
        test.write_text("// ctest-standalone: src/mst/emit_b_doe.cpp src/doe/item_owner.cpp\n"
                        "int main(){ return testkit::run_all(); }\n", encoding="utf-8")
        result = checker.check_files(self.project, [test], [self.include])
        self.assertTrue(result["ok"], result)

    def test_packet_cmake_checks_each_add_executable_separately(self):
        test = self.test_dir / "test_emit.cpp"
        test.write_text("int main(){ return testkit::run_all(); }\n", encoding="utf-8")
        (self.test_dir / "CMakeLists.txt").write_text(
            "add_executable(good test_emit.cpp ${M2M_ROOT}/src/mst/emit_b_doe.cpp "
            "${M2M_ROOT}/src/doe/item_owner.cpp)\n"
            "add_executable(bad other.cpp ${M2M_ROOT}/src/mst/emit_b_doe.cpp)\n",
            encoding="utf-8")
        result = checker.check_files(self.project, [test], [self.include])
        issues = [issue for row in result["files"] for issue in row["issues"]]
        self.assertEqual([issue["code"] for issue in issues],
                         ["cmake_standalone_missing_item_owner"])

    def test_cmake_comments_do_not_create_false_target(self):
        cmake = self.test_dir / "CMakeLists.txt"
        cmake.write_text("# add_executable(fake src/mst/emit_b_doe.cpp)\n"
                         "add_executable(real main.cpp)\n", encoding="utf-8")
        issues = checker._standalone_issues(cmake, cmake.read_text(encoding="utf-8"))
        self.assertEqual(issues, [])

    def test_cmake_target_linking_core_is_not_mistaken_for_standalone(self):
        cmake = self.test_dir / "CMakeLists.txt"
        cmake.write_text("add_executable(core_consumer src/mst/emit_b_doe.cpp)\n"
                         "target_link_libraries(core_consumer PRIVATE m2m_core)\n",
                         encoding="utf-8")
        issues = checker._standalone_issues(cmake, cmake.read_text(encoding="utf-8"))
        self.assertEqual(issues, [])


if __name__ == "__main__":
    unittest.main()
