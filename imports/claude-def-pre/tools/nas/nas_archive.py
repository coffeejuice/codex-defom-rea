"""Archive folders to the NAS with SHA-256 manifests (docs/storage-plan.md, section 6.3).

    python tools/nas/nas_archive.py pack SRC DEST.7z [--only GLOB ...] [--exclude GLOB ...]
    python tools/nas/nas_archive.py copy SRC DESTDIR [--exclude GLOB ...]
    python tools/nas/nas_archive.py verify DEST.7z|DESTDIR

pack: hashes every file under SRC into DEST.files.sha256 (sha256, size, path relative to SRC), packs SRC into a 7z
archive (LZMA2, -mx=5, multithreaded), tests it with `7z t`, and writes DEST.7z.sha256. --only limits the archive to
top-level names of SRC that match; --exclude skips matching file or folder names anywhere (scratch: build output,
staging). copy: robocopy SRC to DESTDIR, then compares SHA-256 of every copied file with its source and writes
DESTDIR.files.sha256. Nothing is ever deleted.
"""
import argparse
import fnmatch
import hashlib
import os
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

SEVEN_ZIP = r"C:\Program Files\7-Zip\7z.exe"


def sha256(path, bufsize=1 << 20):
    h = hashlib.sha256()
    with open(path, "rb") as f:
        while True:
            b = f.read(bufsize)
            if not b:
                break
            h.update(b)
    return h.hexdigest()


def walk(src, only, exclude):
    """Relative paths of the files to archive, in a stable order."""
    src = Path(src)
    tops = [p for p in sorted(src.iterdir()) if not only or any(fnmatch.fnmatch(p.name, g) for g in only)]
    out = []
    for top in tops:
        if any(fnmatch.fnmatch(top.name, g) for g in exclude):
            continue
        if top.is_file():
            out.append(top.relative_to(src))
            continue
        for root, dirs, files in os.walk(top):
            dirs[:] = sorted(d for d in dirs if not any(fnmatch.fnmatch(d, g) for g in exclude))
            for f in sorted(files):
                if not any(fnmatch.fnmatch(f, g) for g in exclude):
                    out.append((Path(root) / f).relative_to(src))
    return [p for p in out if not (src / p).is_symlink()]


def try_sha256(path):
    try:
        return sha256(path)
    except OSError:  # held open exclusively by a running program (a live log, a REA session)
        return None


def write_file_manifest(src, rels, dest_manifest):
    """Hashes the files; the ones that cannot be read are reported and left out (returned list)."""
    src = Path(src)
    with ThreadPoolExecutor(16) as ex:
        digests = list(ex.map(lambda r: try_sha256(src / r), rels))
    kept = []
    with open(dest_manifest, "w", encoding="utf-8", newline="\n") as f:
        for r, d in zip(rels, digests):
            if d is None:
                print(f"skipped (in use): {r.as_posix()}")
                continue
            f.write(f"{d}  {(src / r).stat().st_size}  {r.as_posix()}\n")
            kept.append(r)
    return kept, dict(zip((r.as_posix() for r in rels), digests))


def pack(a):
    src, dest = Path(a.src), Path(a.dest)
    dest.parent.mkdir(parents=True, exist_ok=True)
    side = Path(str(dest) + ".sha256")
    if side.exists() and dest.exists():
        print(f"done already: {dest}")
        return
    if dest.exists():
        # an archive without its .sha256 was cut off mid-write (this tool writes the .sha256 last): start it again
        dest.unlink()
    rels = walk(src, a.only, a.exclude)
    if not rels:
        sys.exit(f"nothing to archive under {src}")
    rels, _ = write_file_manifest(src, rels, str(dest) + ".files.sha256")
    listfile = Path(os.environ.get("TEMP", ".")) / f"nas_archive_{os.getpid()}.lst"
    listfile.write_text("\n".join(str(r) for r in rels) + "\n", encoding="utf-8")
    try:
        cmd = [SEVEN_ZIP, "a", "-t7z", "-mx=5", "-mmt=on", "-bd", "-scsUTF-8", str(dest), f"@{listfile}"]
        r = subprocess.run(cmd, cwd=src, capture_output=True, text=True)
        if r.returncode != 0:
            sys.exit(f"7z a failed ({r.returncode}): {r.stdout[-2000:]} {r.stderr[-2000:]}")
    finally:
        listfile.unlink(missing_ok=True)
    verify_archive(dest, len(rels))


def verify_archive(dest, expect=None):
    r = subprocess.run([SEVEN_ZIP, "t", "-bd", str(dest)], capture_output=True, text=True)
    if r.returncode != 0 or "Everything is Ok" not in r.stdout:
        sys.exit(f"7z t failed for {dest}: {r.stdout[-2000:]}")
    files = next((l.split(":")[1].strip() for l in r.stdout.splitlines() if l.startswith("Files:")), "?")
    if expect is not None and files != "?" and int(files) != expect:
        sys.exit(f"{dest}: archive holds {files} files, expected {expect}")
    digest = sha256(dest)
    side = Path(str(dest) + ".sha256")
    if side.exists() and side.read_text().split()[0] != digest:
        sys.exit(f"{dest}: SHA-256 differs from {side}")
    side.write_text(f"{digest}  {dest.name}\n", encoding="utf-8")
    print(f"ok {dest} files={files} size={dest.stat().st_size} sha256={digest}")


def copy(a):
    src, dest = Path(a.src), Path(a.dest)
    rels = walk(src, None, a.exclude)
    cmd = ["robocopy", str(src), str(dest), "/E", "/Z", "/J", "/R:2", "/W:5", "/NP", "/NFL", "/NDL"]
    for g in a.exclude:
        cmd += ["/XF", g, "/XD", g]
    r = subprocess.run(cmd, capture_output=True, text=True)
    if r.returncode >= 8:
        sys.exit(f"robocopy failed ({r.returncode}): {r.stdout[-2000:]}")
    _, have = write_file_manifest(src, rels, str(dest) + ".files.sha256")
    have = {p: d for p, d in have.items() if d is not None}
    bad = [p for p, d in have.items() if not (dest / p).exists() or sha256(dest / p) != d]
    if bad:
        sys.exit(f"{len(bad)} copied files differ from their source, e.g. {bad[:3]}")
    print(f"ok {dest} files={len(have)}")


def verify(a):
    p = Path(a.target)
    if p.suffix == ".7z":
        verify_archive(p)
        return
    man = Path(str(p) + ".files.sha256")
    bad = 0
    for line in man.read_text(encoding="utf-8").splitlines():
        d, _, rel = line.split("  ", 2)
        if sha256(p / rel) != d:
            bad += 1
            print("differs:", rel)
    print(f"{'ok' if bad == 0 else 'FAILED'} {p} ({bad} differ)")
    sys.exit(1 if bad else 0)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="cmd", required=True)
    p = sub.add_parser("pack")
    p.add_argument("src")
    p.add_argument("dest")
    p.add_argument("--only", action="append", default=[])
    p.add_argument("--exclude", action="append", default=[])
    c = sub.add_parser("copy")
    c.add_argument("src")
    c.add_argument("dest")
    c.add_argument("--exclude", action="append", default=[])
    v = sub.add_parser("verify")
    v.add_argument("target")
    a = ap.parse_args()
    {"pack": pack, "copy": copy, "verify": verify}[a.cmd](a)


if __name__ == "__main__":
    main()
