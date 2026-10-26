"""Read-only source preflight for changed Testkit C++ tests before grouped builds.

Pass the changed/new tests explicitly, using paths relative to the converter
project root, paths prefixed from the checkout root, or absolute paths. This checks entrypoint shape and direct quoted
includes; it is a fast diagnostic, not a replacement for the Release build.
"""

import argparse
import json
from pathlib import Path
import re
import sys


ADMIN = Path(__file__).resolve().parent
ROOT = ADMIN.parent
DEFAULT_PROJECT = ROOT / "source/apps/moproj2mst"
INCLUDE_RE = re.compile(r'^\s*#\s*include\s*"([^"\r\n]+)"')
TOKEN_RE = re.compile(r'"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'|//[^\r\n]*|/\*[\s\S]*?\*/')
MAIN_RE = re.compile(r'\bint\s+main\s*\([^;{}]*\)\s*(?:noexcept\s*)?\{')
RUN_ALL_RE = re.compile(r'\btestkit\s*::\s*run_all\s*\(')


def _mask_non_code(text):
    """Replace comments and literals with spaces while preserving newlines."""
    def mask(match):
        return "".join("\n" if char == "\n" else " " for char in match.group(0))
    return TOKEN_RE.sub(mask, text)


def _test_path(value, project_root):
    path = Path(value)
    tests = (project_root / "tests").resolve()
    candidates = [path] if path.is_absolute() else [project_root / path, ROOT / path,
                                                     Path.cwd() / path]
    # Also accept a repository-relative path when the checkout is nested below
    # a larger workspace path, provided its tests/ suffix stays in this project.
    if not path.is_absolute():
        parts = path.parts
        for index, part in enumerate(parts):
            if part.lower() == "tests":
                candidates.append(project_root / Path(*parts[index:]))
    resolved = [candidate.resolve() for candidate in candidates]
    inside = [candidate for candidate in resolved if candidate.is_relative_to(tests)]
    for candidate in inside:
        if candidate.is_file():
            path = candidate
            break
    else:
        if inside:
            path = inside[0]
        else:
            raise ValueError(f"Expected a tests/**/test_*.cpp path beneath {tests}: {value}")
    if not path.name.startswith("test_") or path.suffix != ".cpp":
        raise ValueError(f"Expected a tests/**/test_*.cpp path beneath {tests}: {path}")
    return path


def _include_dirs(project_root, extras):
    source_root = project_root.parent.parent
    dirs = [project_root / "include", project_root / "src",
            source_root / "port/testkit/include"]
    port_root = source_root / "port"
    if port_root.is_dir():
        dirs.extend(sorted(p / "include" for p in port_root.iterdir()
                           if p.is_dir() and (p / "include").is_dir()))
    dirs.extend(Path(p).resolve() for p in extras)
    return list(dict.fromkeys(p.resolve() for p in dirs if p.is_dir()))


def _quoted_includes(path, include_dirs):
    text = path.read_text(encoding="utf-8-sig")
    issues = []
    code_lines = _mask_non_code(text).splitlines()
    for line_number, (line, code_line) in enumerate(zip(text.splitlines(), code_lines), 1):
        match = INCLUDE_RE.match(line)
        if not match or re.match(r'^\s*#\s*include\b', code_line) is None:
            continue
        name = match.group(1)
        include = Path(name)
        candidates = ([include] if include.is_absolute() else
                      [path.parent / include, *(directory / include for directory in include_dirs)])
        if not any(candidate.is_file() for candidate in candidates):
            issues.append({"code": "unresolved_quoted_include", "line": line_number,
                           "include": name})
    return issues


def inspect_test(path, include_dirs):
    text = path.read_text(encoding="utf-8-sig")
    code = _mask_non_code(text)
    issues = _quoted_includes(path, include_dirs)
    main = MAIN_RE.search(code)
    if main is None:
        issues.append({"code": "missing_int_main", "line": None,
                       "message": "Testkit test must define int main(...)"})
    else:
        depth = 1
        end = main.end()
        while end < len(code) and depth:
            if code[end] == "{":
                depth += 1
            elif code[end] == "}":
                depth -= 1
            end += 1
        body = code[main.end():end - 1] if depth == 0 else code[main.end():]
        if RUN_ALL_RE.search(body) is None:
            issues.append({"code": "missing_testkit_run_all", "line": None,
                           "message": "int main must call testkit::run_all()"})
    return {"file": str(path), "ok": not issues, "issues": issues}


def check_files(project_root, files, extra_include_dirs=()):
    project_root = Path(project_root).resolve()
    include_dirs = _include_dirs(project_root, extra_include_dirs)
    rows = []
    for value in files:
        path = _test_path(value, project_root)
        if not path.is_file():
            rows.append({"file": str(path), "ok": False,
                         "issues": [{"code": "missing_source_file", "line": None}]})
        else:
            rows.append(inspect_test(path, include_dirs))
    return {"ok": all(row["ok"] for row in rows), "files": rows,
            "include_dirs": [str(path) for path in include_dirs]}


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("files", nargs="+", help="changed/new test_*.cpp path(s)")
    parser.add_argument("--project-root", type=Path, default=DEFAULT_PROJECT,
                        help="converter project root (default: this checkout)")
    parser.add_argument("--include-dir", action="append", default=[], metavar="DIR",
                        help="additional target include directory; may be repeated")
    args = parser.parse_args(argv)
    try:
        result = check_files(args.project_root, args.files, args.include_dir)
    except (OSError, UnicodeError, ValueError) as exc:
        print(json.dumps({"ok": False, "error": f"{type(exc).__name__}: {exc}"}, indent=2))
        return 2
    print(json.dumps(result, indent=2))
    return 0 if result["ok"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
