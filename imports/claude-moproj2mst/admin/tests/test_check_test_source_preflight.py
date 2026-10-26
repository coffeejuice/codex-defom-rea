"""Focused tests for the read-only Testkit source preflight checker."""

import sys
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

ADMIN = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ADMIN))

from check_test_source_preflight import check_files


class SourcePreflightTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)
        self.project = self.root / "source/apps/moproj2mst"
        self.test_dir = self.project / "tests/s"
        self.test_dir.mkdir(parents=True)
        self.include = self.project / "include"
        self.include.mkdir()
        (self.include / "present.hpp").write_text("#pragma once\n", encoding="utf-8")

    def write(self, source):
        path = self.test_dir / "test_sample.cpp"
        path.write_text(source, encoding="utf-8")
        return path

    def test_valid_testkit_entrypoint_and_resolvable_includes_pass(self):
        path = self.write(
            '#include "present.hpp"\n'
            "int main() { return testkit::run_all(); }\n")
        result = check_files(self.project, [path])
        self.assertTrue(result["ok"], result)

    def test_repository_root_prefixed_path_is_accepted(self):
        self.write('int main() { return testkit::run_all(); }\n')
        with patch("check_test_source_preflight.ROOT", self.root):
            result = check_files(self.project,
                                 ["source/apps/moproj2mst/tests/s/test_sample.cpp"])
        self.assertTrue(result["ok"], result)

    def test_missing_main_is_reported(self):
        path = self.write(
            '#include "present.hpp"\n'
            "TEST_CASE(sample) { }\n")
        result = check_files(self.project, [path])
        self.assertEqual([issue["code"] for issue in result["files"][0]["issues"]],
                         ["missing_int_main"])

    def test_main_without_testkit_runner_is_reported(self):
        path = self.write("int main() { return 0; }\n")
        result = check_files(self.project, [path])
        self.assertIn("missing_testkit_run_all",
                      [issue["code"] for issue in result["files"][0]["issues"]])

    def test_unresolved_quoted_include_reports_line(self):
        path = self.write(
            '#include "present.hpp"\n'
            '#include "missing.hpp"\n'
            "int main() { return testkit::run_all(); }\n")
        result = check_files(self.project, [path])
        unresolved = [issue for issue in result["files"][0]["issues"]
                      if issue["code"] == "unresolved_quoted_include"]
        self.assertEqual(unresolved, [{"code": "unresolved_quoted_include",
                                       "line": 2, "include": "missing.hpp"}])

    def test_quoted_include_inside_comment_is_ignored(self):
        path = self.write("/*\n#include \\\"missing.hpp\\\"\n*/\n"
                          "int main() { return testkit::run_all(); }\n")
        result = check_files(self.project, [path])
        self.assertTrue(result["ok"], result)

    def test_main_in_comment_does_not_satisfy_entrypoint_check(self):
        path = self.write('// int main() { return testkit::run_all(); }\n'
                          "TEST_CASE(sample) { }\n")
        result = check_files(self.project, [path])
        self.assertIn("missing_int_main",
                      [issue["code"] for issue in result["files"][0]["issues"]])

    def test_non_test_path_is_rejected(self):
        outside = self.project / "src/test_sample.cpp"
        outside.parent.mkdir(parents=True)
        outside.write_text("int main() { return testkit::run_all(); }\n")
        with self.assertRaises(ValueError):
            check_files(self.project, [outside])


if __name__ == "__main__":
    unittest.main()
