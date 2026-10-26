"""Oracle runner: run the original DEF_PRE_64.exe on copies of test inputs and cache the result.

Every case runs in a fresh scratch directory that holds only copies of its inputs. The runner
collects stdout, stderr, the exit code and every file DEF_PRE created, changed or deleted there.
Results are cached under a key that hashes (runner schema, DEF_PRE build, argv, stdin, input
names and contents), so the original runs at most once per case; later calls, from any process,
read the cache. See docs/oracle-runner.md.

Safe for many processes at once: each run has its own scratch directory, a lock file per key
keeps two processes from running the same case, and a finished entry appears in the cache with a
single directory rename.

Library use:

    from def_pre_oracle import Oracle, Case
    oracle = Oracle()
    res = oracle.run(Case.m_mode("header", r"C:\\PROBLEM\\TEST_REA\\COG_P1_B1\\COG_P1_B1.DB"))
    print(res.cached, res.exit_code, res.read_text("header.txt"))

Only DEF_PRE's own files are touched read-only: inputs are copied (or hard-linked to a read-only
staged copy inside the cache), never used in place.
"""
from __future__ import annotations

import argparse
import concurrent.futures
import ctypes
import hashlib
import json
import os
import shutil
import stat
import subprocess
import sys
import tempfile
import time
import uuid
from dataclasses import dataclass, field
from pathlib import Path

SCHEMA = 1  # bump when the key or the entry layout changes
DEFAULT_EXE = r"C:\Program Files\SFTC\DEFORM\v11.0.2\DEF_PRE_64.exe"
DEFAULT_TIMEOUT = 600.0
DEFAULT_MAX_STORE = 2 << 30  # files larger than this are recorded by hash only
TMP_SUBDIR = ".oracle_tmp"  # TEMP/TMP of the child, inside the scratch dir
KEY_EXPORT_SCRIPT = "\n2\n1\nCMD.KEY\n\nE\nE\nY\n"
M_MODES = ("header", "stepList", "lastStepNo", "simStepList")
CHUNK = 8 << 20


class OracleError(RuntimeError):
    pass


# ---------------------------------------------------------------------------------------------
# Cases


@dataclass
class Input:
    """One file placed in the scratch directory.

    Exactly one of `source` (a file on disk) or `data` (inline bytes) is set.
    `link=True` hard-links a read-only staged copy instead of copying; use it for large inputs
    DEF_PRE only reads (DBs for -m and DBREAD). The staged copy lives in the cache, never the
    original file.
    """
    name: str
    source: str | None = None
    data: bytes | None = None
    link: bool = False

    def to_json(self):
        d = {"name": self.name, "link": self.link}
        if self.source is not None:
            d["source"] = self.source
        else:
            d["text"] = self.data.decode("latin-1")
        return d


@dataclass
class Case:
    inputs: list[Input] = field(default_factory=list)
    args: list[str] = field(default_factory=list)
    stdin: bytes = b""
    label: str = ""
    timeout: float = DEFAULT_TIMEOUT
    dirs: list[str] = field(default_factory=list)  # empty directories to create first
    # extra environment for the run (not part of the cache key; the port runner uses it to pin
    # the clock with DEF_PORT_FIXED_TIME)
    env: dict = field(default_factory=dict)

    # -- builders for the three batch paths --------------------------------------------------

    @staticmethod
    def m_mode(mode: str, db: str, out: str | None = None, link: bool = True) -> "Case":
        """`DEF_PRE -m <mode> -d <db> -o <out>`."""
        if mode not in M_MODES:
            raise OracleError(f"unknown -m mode {mode!r}; expected one of {M_MODES}")
        name = Path(db).name
        out = out or f"{mode}.txt"
        return Case(inputs=[Input(name, source=str(db), link=link)],
                    args=["-m", mode, "-d", name, "-o", out], label=f"m:{mode}:{name}")

    @staticmethod
    def script(stdin: str | bytes, inputs: list[Input], label: str = "") -> "Case":
        """Scripted stdin session (menu protocol), e.g. DB_CONVERT.INP."""
        if isinstance(stdin, str):
            stdin = stdin.encode("latin-1")
        return Case(inputs=list(inputs), stdin=stdin, label=label or "stdin")

    @staticmethod
    def keyfile(commands: str, inputs: list[Input], label: str = "") -> "Case":
        """Run a keyword program: menu 2 (file input) -> 1 (KEYWORD file) CMD.KEY, then exit.

        `commands` is the CMD.KEY text, e.g. "DBREAD 25\\na.DB\\nKFWRIT\\nOUT.KEY\\n".
        """
        if not commands.endswith("\n"):
            commands += "\n"
        return Case.script(KEY_EXPORT_SCRIPT,
                           list(inputs) + [Input("CMD.KEY", data=commands.encode("latin-1"))],
                           label or "keyfile")

    @staticmethod
    def key_export(db: str, step: int, out: str | None = None, link: bool = True) -> "Case":
        """`DBREAD <step> / <db> / KFWRIT / <out>`: DEF_PRE's KEY export of one DB step."""
        name = Path(db).name
        out = out or (f"STEP_{step}.KEY" if step >= 0 else f"STEP_M{-step}.KEY")
        return Case.keyfile(f"DBREAD {step}\n{name}\nKFWRIT\n{out}\n",
                            [Input(name, source=str(db), link=link)],
                            label=f"key:{name}:{step}")

    @staticmethod
    def multi(ini: str, inputs: list[Input], label: str = "") -> "Case":
        """DEF_MULTI.INI in the working directory; DEF_PRE processes it and exits."""
        return Case(inputs=[Input("DEF_MULTI.INI", source=str(ini))] + list(inputs),
                    label=label or "multi")

    # -- JSON case files ---------------------------------------------------------------------

    @staticmethod
    def from_json(d: dict, base: Path | None = None) -> "Case":
        """Case file format (see docs/oracle-runner.md):

        {"label": "...", "args": [...], "stdin": "...", "timeout": 600,
         "inputs": [{"name": "a.DB", "source": "C:/x/a.DB", "link": true},
                    {"name": "CMD.KEY", "text": "DBREAD 1\\n..."}]}

        Shorthands: {"m": "header", "db": "..."}, {"key_export": "...DB", "step": 25}.
        Relative sources resolve against `base`.
        """
        def src(p):
            p = Path(p)
            return str(p if p.is_absolute() or base is None else base / p)

        if "m" in d:
            c = Case.m_mode(d["m"], src(d["db"]), d.get("out"), d.get("link", True))
        elif "key_export" in d:
            c = Case.key_export(src(d["key_export"]), int(d["step"]), d.get("out"),
                                d.get("link", True))
        else:
            inputs = []
            for i in d.get("inputs", []):
                if "source" in i:
                    sp = Path(src(i["source"]))
                    if sp.is_dir():
                        inputs += dir_inputs(sp, i.get("name", sp.name), i.get("link", False))
                        continue
                    inputs.append(Input(i.get("name", sp.name), source=str(sp),
                                        link=i.get("link", False)))
                else:
                    inputs.append(Input(i["name"], data=i["text"].encode("latin-1")))
            c = Case(inputs=inputs, args=list(d.get("args", [])),
                     stdin=d.get("stdin", "").encode("latin-1"), dirs=list(d.get("dirs", [])))
        c.label = d.get("label", c.label)
        c.timeout = float(d.get("timeout", c.timeout))
        return c


def dir_inputs(root: Path, prefix: str, link: bool = False) -> list[Input]:
    """All files under `root`, placed under `prefix/` (e.g. a Materials folder)."""
    out = []
    for p in sorted(root.rglob("*")):
        if p.is_file():
            rel = p.relative_to(root).as_posix()
            out.append(Input(f"{prefix}/{rel}" if prefix else rel, source=str(p), link=link))
    return out


# ---------------------------------------------------------------------------------------------
# Results


@dataclass
class Result:
    key: str
    dir: Path
    meta: dict
    cached: bool

    @property
    def exit_code(self) -> int:
        return self.meta["exit_code"]

    @property
    def start_time(self) -> int:
        """Start second of the run: recorded since 2026-10-21; for older entries, the earliest
        output file's modification time minus the run's duration."""
        if "start_time" in self.meta:
            return int(self.meta["start_time"])
        files = self.dir / "files"
        times = [f.stat().st_mtime for f in files.rglob("*") if f.is_file()] if files.is_dir() else []
        if not times:
            times = [(self.dir / "stdout.txt").stat().st_mtime]
        return int(min(times) - float(self.meta.get("duration_s", 0.0)))

    @property
    def stdout(self) -> bytes:
        return (self.dir / "stdout.txt").read_bytes()

    @property
    def stderr(self) -> bytes:
        return (self.dir / "stderr.txt").read_bytes()

    @property
    def files(self) -> dict:
        """relpath -> {status: created|modified|deleted, size, sha256, stored}."""
        return self.meta["files"]

    def path(self, name: str) -> Path:
        info = self.files.get(name)
        if info is None or info["status"] == "deleted":
            raise KeyError(f"{name} was not written by this case")
        if not info["stored"]:
            raise KeyError(f"{name} ({info['size']} bytes) is recorded by hash only")
        return self.dir / "files" / name

    def read_bytes(self, name: str) -> bytes:
        return self.path(name).read_bytes()

    def read_text(self, name: str) -> str:
        return self.read_bytes(name).decode("latin-1")

    def copy_to(self, dest: str | Path) -> None:
        """Copy stdout/stderr/result.json and every stored output file into `dest`."""
        dest = Path(dest)
        dest.mkdir(parents=True, exist_ok=True)
        for n in ("result.json", "stdout.txt", "stderr.txt"):
            shutil.copyfile(self.dir / n, dest / n)
        files = self.dir / "files"
        if files.is_dir():
            shutil.copytree(files, dest / "files", dirs_exist_ok=True)


# ---------------------------------------------------------------------------------------------
# Helpers


def _hash_copy(src: Path, dst: Path | None) -> tuple[str, int]:
    """sha256 of src, optionally copying it to dst in the same pass."""
    h = hashlib.sha256()
    n = 0
    with open(src, "rb") as f, (open(dst, "wb") if dst else _Null()) as g:
        while True:
            b = f.read(CHUNK)
            if not b:
                break
            h.update(b)
            g.write(b)
            n += len(b)
    return h.hexdigest(), n


class _Null:
    def __enter__(self):
        return self

    def __exit__(self, *a):
        return False

    def write(self, b):
        pass


def _atomic_write(path: Path, data: bytes) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    tmp = path.with_name(f".{path.name}.{uuid.uuid4().hex}.tmp")
    tmp.write_bytes(data)
    for attempt in range(20):
        try:
            os.replace(tmp, path)
            return
        except PermissionError:
            # Windows refuses to replace a file another process has open; our writers all
            # write the same bytes for the same name, so an existing file is as good.
            if path.exists():
                tmp.unlink(missing_ok=True)
                return
            time.sleep(0.05 * (attempt + 1))
    os.replace(tmp, path)


def _rmtree(p: Path) -> None:
    def onerr(func, path, _exc):
        try:
            os.chmod(path, stat.S_IWRITE)
            func(path)
        except OSError:
            pass
    if p.exists():
        shutil.rmtree(p, onerror=onerr)


def _pid_alive(pid: int) -> bool:
    if os.name != "nt":
        try:
            os.kill(pid, 0)
            return True
        except OSError:
            return False
    k32 = ctypes.windll.kernel32
    h = k32.OpenProcess(0x1000, False, pid)  # PROCESS_QUERY_LIMITED_INFORMATION
    if not h:
        return False
    try:
        code = ctypes.c_ulong()
        ok = k32.GetExitCodeProcess(h, ctypes.byref(code))
        return bool(ok) and code.value == 259  # STILL_ACTIVE
    finally:
        k32.CloseHandle(h)


def _release(lock: Path) -> None:
    # On Windows the remove fails while another thread or process has the lock open to read
    # it (_lock_stale); retry, or the waiters only get in once the lock goes stale.
    for _ in range(200):
        try:
            os.remove(lock)
            return
        except FileNotFoundError:
            return
        except OSError:
            time.sleep(0.01)


def _lock_stale(lock: Path, timeout: float) -> bool:
    try:
        info = json.loads(lock.read_text())
        return not _pid_alive(int(info["pid"])) or time.time() - float(info["t"]) > timeout + 300
    except (ValueError, KeyError):
        try:  # being written right now, or garbage
            return time.time() - lock.stat().st_mtime > 30
        except OSError:
            return False
    except OSError:
        return False


def _quiet_error_mode() -> None:
    """No Windows error dialogs (missing DLL, GP fault) in DEF_PRE or the programs it launches
    (e.g. DEF_REM.exe without mpich.dll on PATH): the error mode is inherited by child processes,
    so a failing launch returns an error instead of waiting on a dialog nobody answers."""
    if os.name == "nt":
        SEM_FAILCRITICALERRORS, SEM_NOGPFAULTERRORBOX, SEM_NOOPENFILEERRORBOX = 0x1, 0x2, 0x8000
        k32 = ctypes.windll.kernel32
        k32.SetErrorMode(k32.GetErrorMode() | SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX
                         | SEM_NOOPENFILEERRORBOX)


def _kill_tree(pid: int) -> None:
    if os.name == "nt":
        subprocess.run(["taskkill", "/T", "/F", "/PID", str(pid)],
                       stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)


def _snapshot(root: Path) -> dict[str, tuple[int, int]]:
    snap = {}
    for p in root.rglob("*"):
        if p.is_file():
            st = p.stat()
            snap[p.relative_to(root).as_posix()] = (st.st_size, st.st_mtime_ns)
    return snap


# ---------------------------------------------------------------------------------------------
# Oracle


class Oracle:
    """Runs cases against one DEF_PRE build with one shared cache directory.

    cache_dir: $DEFORM_ORACLE_CACHE, else ~/.deform-oracle (not under AppData, which packaged
               apps such as the Claude desktop app see through a private redirect).
    exe:       $DEF_PRE_EXE, else the v11.0.2 install path.
    offline:   $DEFORM_ORACLE_OFFLINE=1 -> never run DEF_PRE, cache lookups only.
    """

    def __init__(self, cache_dir=None, exe=None, offline=None, max_store=DEFAULT_MAX_STORE):
        if cache_dir is None:
            cache_dir = os.environ.get("DEFORM_ORACLE_CACHE") or Path.home() / ".deform-oracle"
        self.cache = Path(cache_dir)
        self.exe = Path(exe or os.environ.get("DEF_PRE_EXE") or DEFAULT_EXE)
        if offline is None:
            offline = os.environ.get("DEFORM_ORACLE_OFFLINE", "") not in ("", "0")
        self.offline = offline
        self.max_store = max_store
        for d in ("entries", "tmp", "locks", "stage", "hashes", "builds"):
            (self.cache / d).mkdir(parents=True, exist_ok=True)
        self._build = None

    # -- identity ----------------------------------------------------------------------------

    def file_hash(self, path: str | Path) -> str:
        """sha256 of a file, memoized on (path, size, mtime) in the cache."""
        p = Path(path).resolve()
        st = p.stat()
        memo_id = hashlib.sha1(
            f"{os.path.normcase(str(p))}|{st.st_size}|{st.st_mtime_ns}".encode()).hexdigest()
        memo = self.cache / "hashes" / memo_id
        try:
            return memo.read_text().strip()
        except OSError:
            pass
        digest, _ = _hash_copy(p, None)
        _atomic_write(memo, digest.encode())
        return digest

    def build_id(self) -> str:
        """sha256 of the DEF_PRE executable (the 'build' part of every key)."""
        if self._build:
            return self._build
        if self.exe.is_file():
            self._build = self.file_hash(self.exe)
            info = {"exe": str(self.exe), "size": self.exe.stat().st_size, "sha256": self._build}
            rec = self.cache / "builds" / f"{self._build}.json"
            if not rec.exists():
                _atomic_write(rec, json.dumps(info, indent=1).encode())
            return self._build
        known = [p.stem for p in (self.cache / "builds").glob("*.json")]
        if len(known) == 1:
            self._build = known[0]
            return self._build
        raise OracleError(f"{self.exe} not found and the cache knows {len(known)} builds; "
                          "set DEF_PRE_EXE")

    def _input_hashes(self, case: Case) -> list[tuple[str, str]]:
        out = []
        seen = set()
        for i in case.inputs:
            name = i.name.replace("\\", "/")
            if name in seen or name.startswith("/") or ".." in name.split("/"):
                raise OracleError(f"bad or duplicate input name {i.name!r}")
            seen.add(name)
            h = hashlib.sha256(i.data).hexdigest() if i.data is not None \
                else self.file_hash(i.source)
            out.append((name, h))
        return sorted(out)

    def key(self, case: Case, _hashes=None) -> str:
        ident = {
            "schema": SCHEMA,
            "build": self.build_id(),
            "args": case.args,
            "stdin": case.stdin.hex(),
            "inputs": _hashes or self._input_hashes(case),
        }
        if case.dirs:
            ident["dirs"] = sorted(d.replace("\\", "/") for d in case.dirs)
        return hashlib.sha256(json.dumps(ident, sort_keys=True).encode()).hexdigest()

    # -- cache -------------------------------------------------------------------------------

    def _entry(self, key: str) -> Path:
        return self.cache / "entries" / key[:2] / key

    def lookup(self, case: Case) -> Result | None:
        return self._load(self.key(case), cached=True)

    def _load(self, key: str, cached: bool) -> Result | None:
        d = self._entry(key)
        try:
            meta = json.loads((d / "result.json").read_text(encoding="utf-8"))
        except OSError:
            return None
        return Result(key, d, meta, cached)

    def run(self, case: Case, refresh: bool = False, keep_scratch: bool = False) -> Result:
        """Return the cached result of `case`, running DEF_PRE if there is none."""
        hashes = self._input_hashes(case)
        key = self.key(case, hashes)
        if not refresh:
            r = self._load(key, cached=True)
            if r:
                return r
        if self.offline:
            raise OracleError(f"offline and no cache entry for {case.label or key}")
        lock = self.cache / "locks" / f"{key}.lock"
        done = (lambda: None) if refresh else (lambda: self._load(key, cached=True))
        r = self._acquire(lock, case.timeout, done)
        if r:
            return r  # another process ran it while we waited
        try:
            return self._execute(case, key, hashes, refresh, keep_scratch)
        finally:
            _release(lock)

    @staticmethod
    def _acquire(lock: Path, timeout: float, done):
        """Take `lock` and return None, or return done() once the holder has finished.

        A lock is stale when its process is gone or it is older than timeout + 300 s.
        """
        while True:
            r = done()
            if r:
                return r
            try:
                fd = os.open(lock, os.O_CREAT | os.O_EXCL | os.O_WRONLY)
            except FileExistsError:
                if _lock_stale(lock, timeout):
                    _release(lock)
                else:
                    time.sleep(0.25)
                continue
            os.write(fd, json.dumps({"pid": os.getpid(), "t": time.time()}).encode())
            os.close(fd)
            r = done()  # finished between our check and taking the lock
            if r:
                _release(lock)
                return r
            return None

    # -- one run -----------------------------------------------------------------------------

    def _stage(self, src: Path, digest: str) -> Path:
        """Read-only copy of src in the cache, used as a hard-link target."""
        st = self.cache / "stage" / digest
        lock = self.cache / "locks" / f"stage-{digest}.lock"
        if self._acquire(lock, 3600, lambda: st if st.exists() else None):
            return st
        try:
            tmp = st.with_name(f".{digest}.{uuid.uuid4().hex}.tmp")
            got, _ = _hash_copy(src, tmp)
            if got != digest:
                tmp.unlink()
                raise OracleError(f"{src} changed while it was being staged")
            os.chmod(tmp, stat.S_IREAD)
            os.replace(tmp, st)
            return st
        finally:
            _release(lock)

    def _execute(self, case, key, hashes, refresh, keep_scratch) -> Result:
        if not self.exe.is_file():
            raise OracleError(f"DEF_PRE not found: {self.exe}")
        work = Path(tempfile.mkdtemp(prefix=f"{key[:12]}-", dir=self.cache / "tmp"))
        scratch = work / "s"
        out = work / "r"
        scratch.mkdir()
        (out / "files").mkdir(parents=True)
        want = dict(hashes)
        linked = {}
        try:
            for i in case.inputs:
                name = i.name.replace("\\", "/")
                dst = scratch / name
                dst.parent.mkdir(parents=True, exist_ok=True)
                if i.data is not None:
                    dst.write_bytes(i.data)
                elif i.link:
                    staged = self._stage(Path(i.source), want[name])
                    os.link(staged, dst)
                    linked[name] = staged
                else:
                    got, _ = _hash_copy(Path(i.source), dst)
                    if got != want[name]:
                        raise OracleError(f"{i.source} changed while it was being copied")
            for d in case.dirs:
                (scratch / d).mkdir(parents=True, exist_ok=True)
            before = _snapshot(scratch)
            (scratch / TMP_SUBDIR).mkdir()
            env = dict(os.environ)
            env["TEMP"] = env["TMP"] = str(scratch / TMP_SUBDIR)
            env.update(case.env)

            t0 = time.time()
            timed_out = False
            _quiet_error_mode()
            with open(out / "stdout.txt", "wb") as so, open(out / "stderr.txt", "wb") as se:
                p = subprocess.Popen([str(self.exe)] + case.args, cwd=scratch, env=env,
                                     stdin=subprocess.PIPE, stdout=so, stderr=se)
                try:
                    p.communicate(case.stdin, timeout=case.timeout)
                except subprocess.TimeoutExpired:
                    timed_out = True
                    _kill_tree(p.pid)
                    p.kill()
                    p.wait()
            duration = time.time() - t0
            if timed_out:
                raise OracleError(f"{case.label or key}: DEF_PRE timed out after "
                                  f"{case.timeout:.0f} s (scratch kept in {work})")

            files = self._collect(scratch, before, want, linked, out / "files")
            meta = {
                "schema": SCHEMA,
                "key": key,
                "label": case.label,
                "build": self.build_id(),
                "exe": str(self.exe),
                "args": case.args,
                "stdin": case.stdin.decode("latin-1"),
                "inputs": [dict(i.to_json(), sha256=want[i.name.replace("\\", "/")])
                           for i in case.inputs],
                "dirs": case.dirs,
                "exit_code": p.returncode,
                "duration_s": round(duration, 3),
                # start second of the run (time() as DEF_PRE sees it): some routines seed rand()
                # with it (14045c530, CNVT3D cap triangulation), so a port run pins its clock
                # here (DEF_PORT_FIXED_TIME) to reproduce the output byte for byte
                "start_time": int(t0),
                "created": time.strftime("%Y-%m-%dT%H:%M:%S"),
                "host": os.environ.get("COMPUTERNAME", ""),
                "files": files,
            }
            (out / "result.json").write_text(json.dumps(meta, indent=1), encoding="utf-8")
            self._publish(out, key, refresh)
            return self._load(key, cached=False)
        except OracleError:
            keep_scratch = True  # leave the evidence for inspection
            raise
        finally:
            if not keep_scratch:
                # A hard link shares the read-only attribute with its staged copy: clear it to
                # delete the link, then set it again on the staged copy.
                for name, staged in linked.items():
                    try:
                        os.chmod(scratch / name, stat.S_IWRITE | stat.S_IREAD)
                        os.unlink(scratch / name)
                    except OSError:
                        pass
                    os.chmod(staged, stat.S_IREAD)
                _rmtree(work)

    def _collect(self, scratch, before, want, linked, dest) -> dict:
        after = _snapshot(scratch)
        files = {}
        for name, (size, mtime) in sorted(after.items()):
            old = before.get(name)
            if old == (size, mtime) and name in want:
                continue  # input untouched
            src = scratch / name
            digest = None
            if old is not None and old[0] == size:
                digest, _ = _hash_copy(src, None)
                if digest == want.get(name):
                    continue  # rewritten with the same bytes or only touched
            if name in linked:
                raise OracleError(f"DEF_PRE modified linked input {name}; "
                                  "pass it with link=False")
            stored = size <= self.max_store
            if stored:
                tgt = dest / name
                tgt.parent.mkdir(parents=True, exist_ok=True)
                digest, _ = _hash_copy(src, tgt)
            elif digest is None:
                digest, _ = _hash_copy(src, None)
            files[name] = {"status": "modified" if old else "created", "size": size,
                           "sha256": digest, "stored": stored}
        for name in sorted(set(before) - set(after)):
            files[name] = {"status": "deleted", "size": 0, "sha256": None, "stored": False}
        for name in linked:
            st = linked[name].stat()
            if st.st_size != after.get(name, (-1,))[0]:
                raise OracleError(f"staged copy of {name} changed")
        return files

    def _publish(self, out: Path, key: str, refresh: bool) -> None:
        """Move the finished result into the cache with one rename."""
        entry = self._entry(key)
        entry.parent.mkdir(parents=True, exist_ok=True)
        if refresh and entry.exists():
            trash = self.cache / "tmp" / f"old-{key[:12]}-{uuid.uuid4().hex}"
            os.replace(entry, trash)
            _rmtree(trash)
        try:
            os.rename(out, entry)
        except OSError:
            if not (entry / "result.json").exists():
                raise  # lost a race only if the winner's entry is there


# ---------------------------------------------------------------------------------------------
# Command line


def _decode_script(s: str) -> bytes:
    """Backslash escapes as in the docs: "\\n2\\n1\\nCMD.KEY\\n\\nE\\nE\\nY\\n"."""
    return s.encode("latin-1").decode("unicode_escape").encode("latin-1")


def _parse_inputs(specs, link_all=False) -> list[Input]:
    """--input PATH[=NAME]; directories are added recursively under NAME ("." = top level)."""
    out = []
    for spec in specs or []:
        path, _, name = spec.rpartition("=") if "=" in spec else (spec, "", "")
        p = Path(path)
        if p.is_dir():
            out += dir_inputs(p, "" if name == "." else (name or p.name), link_all)
        else:
            out.append(Input(name or p.name, source=str(p), link=link_all))
    return out


def _report(case: Case, r: Result | None, err: str | None, copy_to: str | None) -> dict:
    if err:
        return {"label": case.label, "error": err}
    if copy_to:
        r.copy_to(Path(copy_to) / (case.label.replace(":", "_") or r.key[:12]))
    return {"label": case.label, "key": r.key, "cached": r.cached, "exit_code": r.exit_code,
            "dir": str(r.dir),
            "outputs": {k: v["status"] for k, v in r.files.items()}}


def main(argv=None) -> int:
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0],
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--cache", help="cache dir (default $DEFORM_ORACLE_CACHE or "
                                    "~/.deform-oracle)")
    ap.add_argument("--exe", help="DEF_PRE executable (default $DEF_PRE_EXE or v11.0.2)")
    ap.add_argument("--offline", action="store_true", help="cache only, never run DEF_PRE")
    ap.add_argument("--refresh", action="store_true", help="re-run and replace the entry")
    ap.add_argument("--keep-scratch", action="store_true")
    ap.add_argument("-j", "--jobs", type=int, default=os.cpu_count() or 4)
    ap.add_argument("--timeout", type=float, default=None)
    ap.add_argument("--copy-to", help="copy each result (stdout, files) into DIR/<label>")
    sub = ap.add_subparsers(dest="cmd", required=True)

    s = sub.add_parser("m", help="-m header|stepList|lastStepNo|simStepList on DBs")
    s.add_argument("mode", choices=M_MODES + ("all",))
    s.add_argument("db", nargs="+")
    s.add_argument("--out", help="-o name (default <mode>.txt)")

    s = sub.add_parser("key-export", help="DBREAD <step> + KFWRIT for each step of a DB")
    s.add_argument("db")
    s.add_argument("steps", nargs="+", type=int)

    s = sub.add_parser("keyfile", help="run a keyword program (CMD.KEY) via menu 2/1")
    s.add_argument("cmdkey", help="KEY file with commands (DBREAD, KFWRIT, GENDB ...)")
    s.add_argument("--input", action="append", metavar="PATH[=NAME]")
    s.add_argument("--link", action="store_true", help="link inputs read-only")
    s.add_argument("--mkdir", action="append", metavar="DIR", help="create empty dir")

    s = sub.add_parser("stdin", help="scripted stdin session")
    g = s.add_mutually_exclusive_group(required=True)
    g.add_argument("--script", help="stdin text with backslash escapes")
    g.add_argument("--script-file")
    s.add_argument("--input", action="append", metavar="PATH[=NAME]")
    s.add_argument("--link", action="store_true", help="link inputs read-only")
    s.add_argument("args", nargs="*", help="extra DEF_PRE arguments")
    s.add_argument("--mkdir", action="append", metavar="DIR", help="create empty dir")

    s = sub.add_parser("multi", help="DEF_MULTI.INI batch")
    s.add_argument("ini")
    s.add_argument("--input", action="append", metavar="PATH[=NAME]")
    s.add_argument("--link", action="store_true", help="link inputs read-only")
    s.add_argument("--mkdir", action="append", metavar="DIR", help="create empty dir")

    s = sub.add_parser("cases", help="run JSON case files (one case or a list per file)")
    s.add_argument("files", nargs="+")

    s = sub.add_parser("show", help="print result.json of a cache key")
    s.add_argument("key")

    a = ap.parse_args(argv)
    oracle = Oracle(a.cache, a.exe, offline=a.offline or None)

    if a.cmd == "show":
        r = oracle._load(a.key, cached=True)
        if not r:
            print(f"no entry {a.key}", file=sys.stderr)
            return 1
        print(json.dumps(r.meta, indent=1))
        return 0

    cases: list[Case] = []
    if a.cmd == "m":
        modes = M_MODES if a.mode == "all" else (a.mode,)
        cases = [Case.m_mode(m, db, a.out) for db in a.db for m in modes]
    elif a.cmd == "key-export":
        cases = [Case.key_export(a.db, st) for st in a.steps]
    elif a.cmd == "keyfile":
        cases = [Case.keyfile(Path(a.cmdkey).read_text(encoding="latin-1"),
                              _parse_inputs(a.input, a.link), label=Path(a.cmdkey).name)]
    elif a.cmd == "stdin":
        script = Path(a.script_file).read_bytes() if a.script_file else _decode_script(a.script)
        c = Case.script(script, _parse_inputs(a.input, a.link))
        c.args = list(a.args)
        cases = [c]
    elif a.cmd == "multi":
        cases = [Case.multi(a.ini, _parse_inputs(a.input, a.link))]
    elif a.cmd == "cases":
        for f in a.files:
            d = json.loads(Path(f).read_text(encoding="utf-8"))
            for item in d if isinstance(d, list) else [d]:
                cases.append(Case.from_json(item, Path(f).parent))
    for c in cases:
        c.timeout = a.timeout or c.timeout
        c.dirs += getattr(a, "mkdir", None) or []

    def one(c):
        try:
            return _report(c, oracle.run(c, a.refresh, a.keep_scratch), None, a.copy_to)
        except (OracleError, OSError) as e:
            return _report(c, None, str(e), None)

    failed = 0
    with concurrent.futures.ThreadPoolExecutor(max(1, a.jobs)) as ex:
        for rep in ex.map(one, cases):
            print(json.dumps(rep), flush=True)
            failed += "error" in rep
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
