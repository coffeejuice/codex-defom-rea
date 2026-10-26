"""Fast read-only preflight for newly added C++ tests and standalone source lists.

This catches obvious entrypoint/include/DOE Item standalone-link omissions before
the grouped compile. It does not replace C++ compilation or linkage validation.
"""

import argparse
import json
from pathlib import Path
import re
import sys


ADMIN = Path(__file__).resolve().parent
ROOT = ADMIN.parent
DEFAULT_PROJECT = ROOT / "source/apps/moproj2mst"
INCLUDE_RE = re.compile(r'^\s*#\s*include\s*[<"](m2m/[^>"]+)[>"]', re.MULTILINE)
MAIN_RE = re.compile(r"\bint\s+(?:w?main)\s*\([^;{}]*\)\s*(?:noexcept\s*)?\{")
RUN_ALL_RE = re.compile(r"\btestkit\s*::\s*run_all\s*\(")
TESTKIT_RE = re.compile(r"(?:testkit/check\.hpp|\bTEST_CASE\s*\(|\b(?:CHECK|REQUIRE)(?:_[A-Z]+)?\s*\()")
SOURCE_RE = re.compile(r"(?i)(?:\$\{[^}]+\}/)?src/(?:mst/emit_b_doe|doe/item_owner)\.cpp\b")


def _mask_non_code(text):
    """Mask C++ comments and literals while preserving newlines."""
    token = re.compile(r'"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'|//[^\r\n]*|/\*[\s\S]*?\*/')
    return token.sub(lambda match: "".join("\n" if c == "\n" else " "
                                           for c in match.group(0)), text)


def _mask_cpp_comments(text):
    """Mask comments but keep include operands and other string literals."""
    output = list(text)
    quote = None
    escaped = False
    line_comment = False
    block_comment = False
    index = 0
    while index < len(text):
        char = text[index]
        following = text[index + 1] if index + 1 < len(text) else ""
        if line_comment:
            if char in "\r\n":
                line_comment = False
            else:
                output[index] = " "
        elif block_comment:
            if char == "*" and following == "/":
                output[index] = output[index + 1] = " "
                block_comment = False
                index += 1
            elif char not in "\r\n":
                output[index] = " "
        elif quote:
            if escaped:
                escaped = False
            elif char == "\\":
                escaped = True
            elif char == quote:
                quote = None
        elif char in ('"', "'"):
            quote = char
        elif char == "/" and following == "/":
            output[index] = output[index + 1] = " "
            line_comment = True
            index += 1
        elif char == "/" and following == "*":
            output[index] = output[index + 1] = " "
            block_comment = True
            index += 1
        index += 1
    return "".join(output)


def _main_body(code):
    match = MAIN_RE.search(code)
    if match is None:
        return None
    depth = 1
    end = match.end()
    while end < len(code) and depth:
        if code[end] == "{":
            depth += 1
        elif code[end] == "}":
            depth -= 1
        end += 1
    return code[match.end():end - 1] if depth == 0 else code[match.end():]


def _default_include_roots(project_root):
    source_root = project_root.parent.parent
    roots = [project_root / "include", project_root / "src"]
    port = source_root / "port"
    if port.is_dir():
        roots.extend(sorted(path / "include" for path in port.iterdir()
                            if path.is_dir() and (path / "include").is_dir()))
    return [path.resolve() for path in roots if path.is_dir()]


def _relative_path(value):
    return value.replace("\\", "/").lower()


def _has_source(paths, leaf):
    normalized = [_relative_path(value) for value in paths]
    return any(value.endswith(leaf) for value in normalized)


def _mask_cmake_comments(text):
    output = list(text)
    quote = False
    escaped = False
    comment = False
    for index, char in enumerate(text):
        if comment:
            if char in "\r\n":
                comment = False
            else:
                output[index] = " "
        elif quote:
            if escaped:
                escaped = False
            elif char == "\\":
                escaped = True
            elif char == '"':
                quote = False
        elif char == '"':
            quote = True
        elif char == "#":
            comment = True
            output[index] = " "
    return "".join(output)


def _cmake_commands(text, command="add_executable"):
    """Yield selected CMake command argument bodies, honoring comments/nesting."""
    text = _mask_cmake_comments(text)
    pattern = re.compile(r"\b" + re.escape(command) + r"\s*\(", re.IGNORECASE)
    for match in pattern.finditer(text):
        start = match.end()
        depth = 1
        quote = False
        escaped = False
        comment = False
        at = start
        while at < len(text) and depth:
            char = text[at]
            if comment:
                if char in "\r\n":
                    comment = False
            elif quote:
                if escaped:
                    escaped = False
                elif char == "\\":
                    escaped = True
                elif char == '"':
                    quote = False
            elif char == "#":
                comment = True
            elif char == '"':
                quote = True
            elif char == "(":
                depth += 1
            elif char == ")":
                depth -= 1
            at += 1
        if depth == 0:
            yield text[start:at - 1]


def _standalone_issues(path, text):
    issues = []
    if path.name.startswith("test_"):
        for line_number, line in enumerate(text.splitlines(), 1):
            match = re.match(r"\s*//\s*ctest-standalone:\s*(.*)$", line)
            if not match:
                continue
            paths = match.group(1).split()
            if _has_source(paths, "src/mst/emit_b_doe.cpp") and not _has_source(
                    paths, "src/doe/item_owner.cpp"):
                issues.append({"code": "standalone_missing_item_owner",
                               "line": line_number,
                               "message": "standalone list compiles emit_b_doe.cpp but omits src/doe/item_owner.cpp"})

    if path.name == "CMakeLists.txt":
        commands = list(_cmake_commands(text))
        links = {}
        for body in _cmake_commands(text, "target_link_libraries"):
            arguments = body.split()
            if len(arguments) > 1:
                links.setdefault(arguments[0].lower(), set()).update(
                    argument.lower() for argument in arguments[1:])
        for target_index, body in enumerate(commands, 1):
            paths = SOURCE_RE.findall(body)
            target = body.split()[0].lower() if body.split() else ""
            if (_has_source(paths, "src/mst/emit_b_doe.cpp") and
                    not _has_source(paths, "src/doe/item_owner.cpp") and
                    "m2m_core" not in links.get(target, set())):
                issues.append({"code": "cmake_standalone_missing_item_owner",
                               "target_index": target_index,
                               "message": "add_executable source list compiles emit_b_doe.cpp but omits src/doe/item_owner.cpp"})
    return issues


def inspect_file(path, include_roots):
    text = path.read_text(encoding="utf-8-sig")
    code = _mask_non_code(text)
    issues = []
    if path.name.startswith("test_") and path.suffix == ".cpp":
        body = _main_body(code)
        if body is None:
            issues.append({"code": "missing_test_main",
                           "message": "new test must define int main or int wmain"})
        elif TESTKIT_RE.search(code) and not RUN_ALL_RE.search(body):
            issues.append({"code": "missing_testkit_run_all",
                           "message": "Testkit entrypoint must call testkit::run_all()"})

    include_text = _mask_cpp_comments(text)
    for match in INCLUDE_RE.finditer(include_text):
        line_number = include_text.count("\n", 0, match.start()) + 1
        include = match.group(1)
        if not any((root / include).is_file() for root in include_roots):
            issues.append({"code": "unresolved_m2m_include", "line": line_number,
                           "include": include})

    issues.extend(_standalone_issues(path, text))
    return {"file": str(path), "ok": not issues, "issues": issues}


def check_files(project_root, files, include_roots=None):
    project_root = Path(project_root).resolve()
    roots = ([Path(root).resolve() for root in include_roots]
             if include_roots else _default_include_roots(project_root))
    rows = []
    for value in files:
        path = Path(value)
        if not path.is_absolute():
            candidates = [project_root / path, ROOT / path, Path.cwd() / path]
            resolved = [candidate.resolve() for candidate in candidates]
            path = next((candidate for candidate in resolved if candidate.is_file()), resolved[0])
        if not path.is_file():
            rows.append({"file": str(path), "ok": False,
                         "issues": [{"code": "missing_file"}]})
        else:
            rows.append(inspect_file(path.resolve(), roots))

    # For each supplied Testkit test, inspect its local packet CMake file too.
    for row in list(rows):
        test_path = Path(row["file"])
        if not test_path.name.startswith("test_"):
            continue
        packet_cmake = test_path.parent / "CMakeLists.txt"
        if packet_cmake.is_file() and all(item["file"] != str(packet_cmake.resolve()) for item in rows):
            rows.append(inspect_file(packet_cmake.resolve(), roots))
    return {"ok": all(row["ok"] for row in rows), "files": rows,
            "include_roots": [str(root) for root in roots]}


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("files", nargs="+", help="new tests/changed C++ files and optional CMakeLists.txt")
    parser.add_argument("--project-root", type=Path, default=DEFAULT_PROJECT)
    parser.add_argument("--include-root", action="append", default=[], metavar="DIR",
                        help="override default include roots; may be repeated")
    args = parser.parse_args(argv)
    try:
        result = check_files(args.project_root, args.files, args.include_root or None)
    except (OSError, UnicodeError, ValueError) as exc:
        print(json.dumps({"ok": False, "error": f"{type(exc).__name__}: {exc}"}, indent=2))
        return 2
    print(json.dumps(result, indent=2))
    return 0 if result["ok"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
