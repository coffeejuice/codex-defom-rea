#!/usr/bin/env python3
"""Scratch-only warm-cache benchmark for recursive fixture copying on Windows.

Compares the exact std::filesystem::copy(..., recursive) baseline with an
explicit recursive_directory_iterator + create_directory/copy_file traversal.
The custom traversal is a candidate, not presumed equivalent; every output is
checked against the baseline before the benchmark is considered valid.
"""
from __future__ import annotations

import argparse
import hashlib
import os
from pathlib import Path
import shutil
import statistics
import subprocess
import sys
import tempfile
import time


CPP = r'''#include <chrono>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>
#include <system_error>
namespace fs = std::filesystem;
using clock_type = std::chrono::steady_clock;
static double ms(clock_type::time_point start) {
    return std::chrono::duration<double, std::milli>(clock_type::now() - start).count();
}
static void manual_copy(const fs::path& source, const fs::path& destination) {
    std::error_code ec;
    fs::create_directory(destination, source, ec);
    if (ec) throw fs::filesystem_error("create_directory", source, destination, ec);
    for (fs::recursive_directory_iterator it(source), end; it != end; ++it) {
        const auto from = it->path();
        const auto rel = fs::relative(from, source);
        const auto to = destination / rel;
        const auto status = it->symlink_status();
        if (fs::is_symlink(status) || (!fs::is_directory(status) && !fs::is_regular_file(status)))
            throw std::runtime_error("symlink/special object is outside this benchmark's declared scope: " + rel.string());
        if (fs::is_directory(status)) {
            fs::create_directory(to, from, ec);
            if (ec) throw fs::filesystem_error("create_directory", from, to, ec);
        } else {
            const auto start = clock_type::now();
            fs::copy_file(from, to, fs::copy_options::none, ec);
            const auto elapsed = ms(start);
            if (ec) throw fs::filesystem_error("copy_file", from, to, ec);
            std::cout << "FILE\t" << rel.generic_string() << "\t" << fs::file_size(from)
                      << "\t" << elapsed << "\n";
        }
    }
}
int main(int argc, char** argv) {
    if (argc != 4) return 64;
    try {
        const fs::path source = fs::u8path(argv[2]);
        const fs::path destination = fs::u8path(argv[3]);
        const auto start = clock_type::now();
        if (std::string(argv[1]) == "baseline")
            fs::copy(source, destination, fs::copy_options::recursive);
        else if (std::string(argv[1]) == "manual")
            manual_copy(source, destination);
        else return 64;
        std::cout << "TOTAL\t" << argv[1] << "\t" << ms(start) << "\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "copy benchmark failed: " << e.what() << "\n";
        return 1;
    }
}
'''


def digest(path: Path) -> str:
    h = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            h.update(block)
    return h.hexdigest()


def inventory(root: Path) -> dict[str, tuple[str, str, int, int, int, int]]:
    result = {}
    for path in [root, *root.rglob("*")]:
        rel = "." if path == root else path.relative_to(root).as_posix()
        info = path.lstat()
        if path.is_symlink():
            kind, content = "symlink", os.readlink(path)
        elif path.is_dir():
            kind, content = "directory", ""
        elif path.is_file():
            kind, content = "file", digest(path)
        else:
            kind, content = "special", ""
        attrs = getattr(info, "st_file_attributes", 0)
        modified = info.st_mtime_ns if kind == "file" else 0
        result[rel] = (kind, content, info.st_size, info.st_mode & 0o777,
                       attrs, modified)
    return result


def compare_outputs(source: Path, baseline: Path, candidate: Path) -> None:
    source_inv = inventory(source)
    baseline_inv = inventory(baseline)
    candidate_inv = inventory(candidate)
    for label, actual in (("baseline", baseline_inv), ("candidate", candidate_inv)):
        if set(actual) != set(source_inv):
            missing = sorted(set(source_inv) - set(actual))[:5]
            extra = sorted(set(actual) - set(source_inv))[:5]
            raise RuntimeError(f"{label} inventory differs; missing={missing} extra={extra}")
        for rel, expected in source_inv.items():
            got = actual[rel]
            if expected[0] != got[0] or (expected[0] == "file" and expected[1] != got[1]):
                raise RuntimeError(f"{label} content/type mismatch at {rel}")
    for rel, expected in baseline_inv.items():
        if expected != candidate_inv[rel]:
            raise RuntimeError(f"candidate metadata/content differs from baseline at {rel}")


def compile_helper(work: Path) -> Path:
    source = work / "copy_bench.cpp"
    exe = work / "copy_bench.exe"
    source.write_text(CPP, encoding="utf-8")
    compiler = shutil.which("cl.exe")
    if not compiler:
        raise RuntimeError("cl.exe not found; run from a Visual Studio developer shell")
    command = [compiler, "/nologo", "/EHsc", "/std:c++17", "/O2", str(source), f"/Fe:{exe}"]
    subprocess.run(command, cwd=work, check=True)
    return exe


def run_copy(exe: Path, mode: str, source: Path, destination: Path) -> tuple[float, list[tuple[str, int, float]]]:
    result = subprocess.run([str(exe), mode, str(source), str(destination)],
                            text=True, capture_output=True, check=True)
    total = None
    files = []
    for line in result.stdout.splitlines():
        fields = line.split("\t")
        if fields[0] == "TOTAL":
            total = float(fields[2])
        elif fields[0] == "FILE":
            files.append((fields[1], int(fields[2]), float(fields[3])))
    if total is None:
        raise RuntimeError(f"helper omitted total for {mode}")
    return total, files


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--input", required=True, type=Path, help="read-only fixture directory")
    parser.add_argument("--scratch", required=True, type=Path, help="existing scratch parent for disposable copies")
    parser.add_argument("--repetitions", type=int, default=5, choices=range(3, 11))
    parser.add_argument("--keep", action="store_true", help="retain benchmark outputs and helper under scratch")
    args = parser.parse_args()
    if os.name != "nt":
        parser.error("this benchmark uses the Windows cl.exe toolchain")
    source = args.input.resolve(strict=True)
    scratch = args.scratch.resolve(strict=True)
    if not source.is_dir() or not scratch.is_dir():
        parser.error("--input and --scratch must be existing directories")
    if source == scratch or source in scratch.parents or scratch in source.parents:
        parser.error("--scratch and --input must be disjoint directory trees")

    work = Path(tempfile.mkdtemp(prefix="save-io-copy-bench-", dir=scratch)).resolve(strict=True)
    if work.parent != scratch or not work.name.startswith("save-io-copy-bench-"):
        raise RuntimeError("benchmark scratch escaped the intended parent")
    try:
        exe = compile_helper(work)
        orderings = (("baseline", "manual"), ("manual", "baseline"))
        totals = {"baseline": [], "manual": []}
        latest = {}
        for index in range(args.repetitions):
            round_outputs = {}
            for mode in orderings[index % len(orderings)]:
                destination = work / f"run-{index}-{mode}"
                elapsed, per_file = run_copy(exe, mode, source, destination)
                totals[mode].append(elapsed)
                latest[mode] = (destination, per_file)
                round_outputs[mode] = destination
                print(f"run={index + 1} mode={mode} total_ms={elapsed:.3f}")
            compare_outputs(source, round_outputs["baseline"], round_outputs["manual"])
        manual_dir, per_file = latest["manual"]
        for mode, values in totals.items():
            print(f"summary mode={mode} median_ms={statistics.median(values):.3f} "
                  f"min_ms={min(values):.3f} max_ms={max(values):.3f}")
        for rel, size, elapsed in sorted(per_file, key=lambda x: x[2], reverse=True):
            print(f"file_cost mode=manual path={rel} bytes={size} ms={elapsed:.3f}")
        print("verification=passed inventory=equal sha256=equal candidate_metadata=baseline_metadata")
        print("cache=warm-expected; no cold-cache claim")
        return 0
    finally:
        if args.keep:
            print(f"scratch_retained={work}")
        elif work.parent == scratch and work.name.startswith("save-io-copy-bench-"):
            shutil.rmtree(work, ignore_errors=True)


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except (OSError, subprocess.CalledProcessError, RuntimeError) as error:
        print(f"benchmark error: {error}", file=sys.stderr)
        raise SystemExit(2)
