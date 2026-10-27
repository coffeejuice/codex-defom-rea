"""CP289: one creation-bound B98 clone acquisition; never runs the application.

CLI paths, pins and output are fixed. Private parameterized seams exist solely
for tiny disposable tests. No failure deletes, retries or adopts an existing name.
"""
from __future__ import annotations

import ctypes
from ctypes import wintypes as w
from datetime import datetime, timezone
import hashlib
import json
import os
from pathlib import Path, PurePosixPath
import re
import sys
import uuid

import color_clone_manifest_gate as gate
from color_installed_file_identity import Info, INVALID_HANDLE_VALUE

ROOT = Path(__file__).resolve().parents[1]
REFERENCE = ROOT / "analysis/resume214-original-postopen-2026-10-26T13-17-52-922Z/source-fixture-reference.json"
INSTALLED_RECEIPT = ROOT / "analysis/cp288-installed-file-identities.json"
INSTALLED_RECEIPT_SHA256 = "b9260b578b177dda69e21438e8c7338c93231959a472e0ce070c4088618c3884"
OUTPUT = ROOT / "analysis/cp289-fresh-clone-acquisition.json"
FILE_CREATE, FILE_CREATED = 2, 2
FILE_DIRECTORY_FILE, FILE_NON_DIRECTORY_FILE = 1, 0x40
FILE_SYNCHRONOUS_IO_NONALERT, FILE_OPEN_REPARSE_POINT = 0x20, 0x200000
SYNCHRONIZE, FILE_READ_ATTRIBUTES, FILE_TRAVERSE = 0x100000, 0x80, 0x20
DIRECTORY_ACCESS = SYNCHRONIZE | FILE_READ_ATTRIBUTES | FILE_TRAVERSE
FILE_WRITE_ACCESS = SYNCHRONIZE | FILE_READ_ATTRIBUTES | 2
OBJ_CASE_INSENSITIVE, OBJ_DONT_REPARSE = 0x40, 0x1000


class UNICODE_STRING(ctypes.Structure):
    _fields_ = [("Length", ctypes.c_uint16), ("MaximumLength", ctypes.c_uint16),
                ("Buffer", ctypes.c_void_p)]


class OBJECT_ATTRIBUTES(ctypes.Structure):
    _fields_ = [("Length", ctypes.c_uint32), ("RootDirectory", w.HANDLE),
                ("ObjectName", ctypes.POINTER(UNICODE_STRING)),
                ("Attributes", ctypes.c_uint32), ("SecurityDescriptor", ctypes.c_void_p),
                ("SecurityQualityOfService", ctypes.c_void_p)]


class IO_STATUS_UNION(ctypes.Union):
    _fields_ = [("Status", ctypes.c_int32), ("Pointer", ctypes.c_void_p)]


class IO_STATUS_BLOCK(ctypes.Structure):
    _anonymous_ = ("result",)
    _fields_ = [("result", IO_STATUS_UNION), ("Information", ctypes.c_size_t)]


def layout():
    return {"pointerBytes": ctypes.sizeof(ctypes.c_void_p),
            "ntstatusBytes": ctypes.sizeof(ctypes.c_int32),
            "unicodeStringBytes": ctypes.sizeof(UNICODE_STRING),
            "objectAttributesBytes": ctypes.sizeof(OBJECT_ATTRIBUTES),
            "ioStatusBlockBytes": ctypes.sizeof(IO_STATUS_BLOCK),
            "unicodeBufferOffset": UNICODE_STRING.Buffer.offset,
            "rootDirectoryOffset": OBJECT_ATTRIBUTES.RootDirectory.offset,
            "objectNameOffset": OBJECT_ATTRIBUTES.ObjectName.offset,
            "attributesOffset": OBJECT_ATTRIBUTES.Attributes.offset,
            "informationOffset": IO_STATUS_BLOCK.Information.offset}


def _valid_handle(handle):
    return handle not in (None, 0, INVALID_HANDLE_VALUE)


def _leaf(name):
    if (not name or name in (".", "..") or name[-1:] in (".", " ")
            or any(c in name for c in '\\/:\x00')
            or any(ord(c) < 32 for c in name)):
        raise ValueError("unsafe relative leaf: " + repr(name))
    return name


class WindowsCloneFiles:
    def __init__(self):
        if os.name != "nt" or layout() != {
            "pointerBytes": 8, "ntstatusBytes": 4, "unicodeStringBytes": 16,
            "objectAttributesBytes": 48, "ioStatusBlockBytes": 16,
            "unicodeBufferOffset": 8, "rootDirectoryOffset": 8,
            "objectNameOffset": 16, "attributesOffset": 24, "informationOffset": 8,
        }:
            raise OSError("CP289 requires the verified Windows x64 ctypes layout")
        self.k = ctypes.WinDLL("kernel32", use_last_error=True)
        self.n = ctypes.WinDLL("ntdll")
        signatures = {
            "CreateFileW": ([w.LPCWSTR, w.DWORD, w.DWORD, ctypes.c_void_p, w.DWORD, w.DWORD, w.HANDLE], w.HANDLE),
            "GetFileInformationByHandle": ([w.HANDLE, ctypes.POINTER(Info)], w.BOOL),
            "GetFinalPathNameByHandleW": ([w.HANDLE, w.LPWSTR, w.DWORD, w.DWORD], w.DWORD),
            "GetFileType": ([w.HANDLE], w.DWORD),
            "ReadFile": ([w.HANDLE, ctypes.c_void_p, w.DWORD, ctypes.POINTER(w.DWORD), ctypes.c_void_p], w.BOOL),
            "WriteFile": ([w.HANDLE, ctypes.c_void_p, w.DWORD, ctypes.POINTER(w.DWORD), ctypes.c_void_p], w.BOOL),
            "FlushFileBuffers": ([w.HANDLE], w.BOOL),
            "CloseHandle": ([w.HANDLE], w.BOOL),
        }
        for name, (args, result) in signatures.items():
            fn = getattr(self.k, name)
            fn.argtypes, fn.restype = args, result
        self.n.NtCreateFile.argtypes = [ctypes.POINTER(w.HANDLE), ctypes.c_uint32,
            ctypes.POINTER(OBJECT_ATTRIBUTES), ctypes.POINTER(IO_STATUS_BLOCK),
            ctypes.c_void_p, ctypes.c_uint32, ctypes.c_uint32, ctypes.c_uint32,
            ctypes.c_uint32, ctypes.c_void_p, ctypes.c_uint32]
        self.n.NtCreateFile.restype = ctypes.c_int32

    def open_directory(self, path):
        # No delete sharing: this particular directory cannot be renamed/replaced
        # while retained. READ|WRITE sharing does not freeze its descendants.
        handle = self.k.CreateFileW(str(path), DIRECTORY_ACCESS, 3, None, 3,
                                    0x02000000 | 0x00200000, None)
        if not _valid_handle(handle):
            raise ctypes.WinError(ctypes.get_last_error())
        return handle

    def open_source(self, path):
        handle = self.k.CreateFileW(str(path), 0x80000000, 1, None, 3, 0x00200000, None)
        if not _valid_handle(handle):
            raise ctypes.WinError(ctypes.get_last_error())
        return handle

    def create_relative(self, parent, name, directory):
        _leaf(name)
        encoded = name.encode("utf-16-le", "strict")
        if len(encoded) + 2 > 0xffff:
            raise ValueError("relative name exceeds UNICODE_STRING capacity")
        # Keep backing, string, attributes and IOSB alive across the synchronous
        # call. Buffer is a pointer, lengths are BYTES excluding/including NUL.
        backing = ctypes.create_unicode_buffer(name)
        text = UNICODE_STRING(len(encoded), len(encoded) + 2, ctypes.addressof(backing))
        attrs = OBJECT_ATTRIBUTES(ctypes.sizeof(OBJECT_ATTRIBUTES), parent,
            ctypes.pointer(text), OBJ_CASE_INSENSITIVE | OBJ_DONT_REPARSE, None, None)
        iosb = IO_STATUS_BLOCK()
        iosb.Status = ctypes.c_int32(0xC0000001).value  # sentinel, not a result
        handle = w.HANDLE()
        options = FILE_SYNCHRONOUS_IO_NONALERT | (
            FILE_DIRECTORY_FILE if directory else FILE_NON_DIRECTORY_FILE | FILE_OPEN_REPARSE_POINT)
        access = DIRECTORY_ACCESS if directory else FILE_WRITE_ACCESS
        status = self.n.NtCreateFile(ctypes.byref(handle), access, ctypes.byref(attrs),
            ctypes.byref(iosb), None, 0x10 if directory else 0x80, 3 if directory else 1,
            FILE_CREATE, options, None, 0)
        # Return even an unexpected handle before checking status, so the caller
        # can register and close it on every failure path. NT errors are NOT
        # kernel32 GetLastError values; retain both raw NT status fields.
        return handle.value, {
            "api": "NtCreateFile", "relativeLeaf": name,
            "directory": directory, "createDisposition": FILE_CREATE,
            "createOptions": options, "desiredAccess": access,
            "shareAccess": 3 if directory else 1,
            "objectAttributes": attrs.Attributes,
            "ntstatusSigned": int(status), "ntstatusHex": f"0x{status & 0xffffffff:08x}",
            "ioStatusSigned": int(iosb.Status),
            "ioStatusHex": f"0x{iosb.Status & 0xffffffff:08x}",
            "information": int(iosb.Information), "returnedHandle": str(handle.value),
        }

    def snapshot(self, handle, path, directory):
        info = Info()
        if not self.k.GetFileInformationByHandle(handle, ctypes.byref(info)):
            raise ctypes.WinError(ctypes.get_last_error())
        if self.k.GetFileType(handle) != 1 or info.attributes & 0x400:
            raise ValueError("reparse point or non-disk handle rejected: " + str(path))
        if bool(info.attributes & 0x10) != directory:
            raise ValueError("wrong filesystem entry kind: " + str(path))
        buf = ctypes.create_unicode_buffer(32768)
        length = self.k.GetFinalPathNameByHandleW(handle, buf, len(buf), 0)
        if not length or length >= len(buf):
            raise OSError("final-path query failed/truncated")
        if buf.value != "\\\\?\\" + str(path):
            raise ValueError("exact final path mismatch: " + buf.value)
        return {"finalPath": buf.value, "volumeSerialNumber": info.volume,
                "fileIndex": str((info.index_high << 32) | info.index_low),
                "creationFileTime": str((info.creation.dwHighDateTime << 32) | info.creation.dwLowDateTime),
                "lastWriteFileTime": str((info.write.dwHighDateTime << 32) | info.write.dwLowDateTime),
                "size": (info.size_high << 32) | info.size_low,
                "attributes": info.attributes}

    def copy(self, source, destination, expected_size):
        digest, total = hashlib.sha256(), 0
        buf = ctypes.create_string_buffer(65536)
        while True:
            count = w.DWORD()
            if not self.k.ReadFile(source, buf, len(buf), ctypes.byref(count), None):
                raise ctypes.WinError(ctypes.get_last_error())
            if not count.value:
                break
            total += count.value
            if total > expected_size:
                raise ValueError("source grew beyond pinned size")
            digest.update(buf.raw[:count.value])
            written = 0
            while written < count.value:
                step = w.DWORD()
                if not self.k.WriteFile(destination, ctypes.byref(buf, written),
                        count.value - written, ctypes.byref(step), None):
                    raise ctypes.WinError(ctypes.get_last_error())
                if not 0 < step.value <= count.value - written:
                    raise OSError("WriteFile made no progress or returned an invalid count")
                written += step.value
        if total != expected_size:
            raise ValueError("source read size differs from pin")
        if not self.k.FlushFileBuffers(destination):
            raise ctypes.WinError(ctypes.get_last_error())
        return digest.hexdigest(), total

    def close(self, handle):
        if not self.k.CloseHandle(handle):
            raise ctypes.WinError(ctypes.get_last_error())


def _identity(snapshot):
    # Adding children changes a directory's write time; object identity remains
    # the final path + volume/index/creation + exact attributes.
    return {k: v for k, v in snapshot.items() if k not in ("lastWriteFileTime", "size")}


def _validate_entries(entries, clone):
    seen = set()
    for entry in entries:
        rel = entry["path"]
        parts = PurePosixPath(rel).parts
        if not parts or "/".join(parts) != rel or rel.startswith("/"):
            raise ValueError("noncanonical relative manifest path")
        for part in parts:
            _leaf(part)
        if rel.casefold() in seen or entry["kind"] not in ("file", "directory"):
            raise ValueError("duplicate/invalid manifest entry")
        seen.add(rel.casefold())
        if len(str(clone.joinpath(*parts))) >= 100:
            raise ValueError("full generated fixture path must be shorter than 100 characters")
    directories = {entry["path"] for entry in entries if entry["kind"] == "directory"}
    for entry in entries:
        parent = PurePosixPath(entry["path"]).parent.as_posix()
        if parent != "." and parent not in directories:
            raise ValueError("manifest parent missing or not a directory")


def _command_gate(source, clone, encoded, cwd, argv, production):
    project = clone / gate.PROJECT_NAME
    if (len(str(project)) >= 100 or cwd != clone or
            gate.decode_utf16_units(encoded) != str(project) or
            gate.encode_utf16_units(str(project)) != encoded or
            argv != [str(gate.EXECUTABLE), "-m", "open", "-f", encoded]):
        raise ValueError("exact UTF-16 argv/cwd gate failed")
    if production:
        return gate.validate(source, clone, encoded, cwd, argv)
    return {"status": "HOLD", "exactUtf16ArgvCwd": True,
            "qualification": "disposable test only; no fixed B98 gate"}


def _acquire(source, parent, entries, *, api=None, name=None, production=False,
             installed_evidence=None, on_retained=None):
    """Private disposable-fixture seam. main() alone selects production pins."""
    receipt = {"schema": "cp289-creation-bound-clone-v1", "status": "HOLD",
        "original_run_ready": False, "runtimeLoadedModuleIdentity": "unknown",
        "startedUtc": datetime.now(timezone.utc).isoformat(),
        "collectorSha256": hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),
        "nativeLayout": layout(), "acquisitionOutcome": "INCOMPLETE",
        "production": production, "sourceRoot": str(source), "parentRoot": str(parent),
        "cloneRoot": None, "creationAttempts": [], "copiedFiles": [],
        "handlesOpened": 0, "handlesClosed": 0, "allHandlesClosed": False,
        "errors": [], "installedIdentityEvidence": installed_evidence,
        "freshnessQualification": "bounded creation/copy/manifest interval only; no post-close namespace guarantee",
        "partialClonePolicy": "preserve without deletion, retry, overwrite or reuse"}
    handles, directories, source_files, destination_files = [], {}, [], []
    phase = "preflight"
    clone = None
    def retain(handle, role, path, directory):
        handles.append((handle, role, path, directory))
        receipt["handlesOpened"] += 1
        return handle
    def create(parent_handle, leaf, path, directory):
        handle, result = api.create_relative(parent_handle, leaf, directory)
        result["path"] = str(path)
        receipt["creationAttempts"].append(result)
        if _valid_handle(handle):
            retain(handle, "cloneDirectory" if directory else "cloneFile", path, directory)
        if (result["ntstatusSigned"] != 0 or result["ioStatusSigned"] != 0
                or result["information"] != FILE_CREATED or not _valid_handle(handle)):
            raise OSError("NtCreateFile did not return synchronous STATUS_SUCCESS, FILE_CREATED and a valid creation-bound handle")
        return handle
    try:
        api = api or WindowsCloneFiles()
        source, parent = Path(os.path.abspath(source)), Path(os.path.abspath(parent))
        gate._check_components(source)
        gate._check_components(parent)
        if production and (source != gate.EXPECTED_SOURCE_ROOT or parent != gate.PROBLEM_ROOT):
            raise ValueError("production source/parent fixed path mismatch")
        chosen = name if name is not None else "m289_" + uuid.uuid4().hex[:16]
        if re.fullmatch(r"m289_[0-9a-f]{16}", chosen) is None:
            raise ValueError("one UUID-derived m289_<16hex> leaf required")
        clone = parent / chosen
        receipt["cloneRoot"] = str(clone)
        _validate_entries(entries, clone)
        project = clone / gate.PROJECT_NAME
        if len(str(clone)) >= 100 or len(str(project)) >= 100:
            raise ValueError("clone/project path must be shorter than 100 characters")
        # Acquire the verified parent before any clone creation or source scan.
        parent_handle = retain(api.open_directory(parent), "parentDirectory", parent, True)
        parent_before = api.snapshot(parent_handle, parent, True)
        receipt["parentBefore"] = parent_before
        source_before = gate.directory_manifest(source, enforce_problem_containment=False)
        receipt["sourceBefore"] = source_before
        if source_before["entries"] != entries:
            raise ValueError("source differs from pinned manifest before creation")
        phase = "createClone"
        clone_handle = create(parent_handle, chosen, clone, True)
        clone_before = api.snapshot(clone_handle, clone, True)
        receipt["createdCloneIdentity"] = clone_before
        if _identity(api.snapshot(parent_handle, parent, True)) != _identity(parent_before):
            raise ValueError("parent identity changed at child creation")
        directories["."] = clone_handle
        dir_identities = {".": clone_before}
        phase = "copy"
        for entry in sorted((e for e in entries if e["kind"] == "directory"),
                            key=lambda e: (len(PurePosixPath(e["path"]).parts), e["path"])):
            rel = PurePosixPath(entry["path"])
            path = clone.joinpath(*rel.parts)
            handle = create(directories[rel.parent.as_posix()], rel.name, path, True)
            directories[rel.as_posix()] = handle
            dir_identities[rel.as_posix()] = api.snapshot(handle, path, True)
        for entry in (e for e in entries if e["kind"] == "file"):
            rel = PurePosixPath(entry["path"])
            source_path, dest_path = source.joinpath(*rel.parts), clone.joinpath(*rel.parts)
            gate._check_components(source_path)
            src = retain(api.open_source(source_path), "sourceFile", source_path, False)
            src_before = api.snapshot(src, source_path, False)
            if src_before["size"] != int(entry["bytes"]):
                raise ValueError("source handle size differs from manifest")
            dst = create(directories[rel.parent.as_posix()], rel.name, dest_path, False)
            dst_initial = api.snapshot(dst, dest_path, False)
            if dst_initial["size"] != 0:
                raise ValueError("new destination file is not empty")
            digest, size = api.copy(src, dst, int(entry["bytes"]))
            src_after, dst_after = api.snapshot(src, source_path, False), api.snapshot(dst, dest_path, False)
            if src_before != src_after or _identity(dst_initial) != _identity(dst_after):
                raise ValueError("copy handle identity changed")
            if digest != entry["sha256"] or size != int(entry["bytes"]) or dst_after["size"] != size:
                raise ValueError("copied bytes do not match pin")
            source_files.append((src, source_path, src_before))
            destination_files.append((dst, dest_path, dst_after))
            receipt["copiedFiles"].append({"path": rel.as_posix(), "bytes": size, "sha256": digest,
                "sourceIdentity": src_before, "destinationIdentity": dst_after})
        phase = "manifestsAndCommand"
        clone_after = gate.directory_manifest(clone, enforce_problem_containment=production)
        receipt["cloneAfter"] = clone_after
        source_after = gate.directory_manifest(source, enforce_problem_containment=False)
        receipt["sourceAfter"] = source_after
        if clone_after["entries"] != entries or source_after != source_before:
            raise ValueError("clone manifest mismatch or source changed during acquisition")
        encoded = gate.encode_utf16_units(str(project))
        argv = [str(gate.EXECUTABLE), "-m", "open", "-f", encoded]
        receipt["launchContract"] = {"projectPath": str(project), "encodedPath": encoded,
            "cwd": str(clone), "argv": argv,
            "maxFixturePathCharacters": max([len(str(project)), *(len(str(clone.joinpath(*PurePosixPath(e['path']).parts))) for e in entries)]),
            "futureUnlistedGeneratedPaths": "must be separately gated below 100 characters before any original execution"}
        receipt["commandGate"] = _command_gate(source, clone, encoded, clone, argv, production)
        # All directory and per-file handles are still retained at this boundary.
        for rel, handle in directories.items():
            path = clone if rel == "." else clone.joinpath(*PurePosixPath(rel).parts)
            if _identity(api.snapshot(handle, path, True)) != _identity(dir_identities[rel]):
                raise ValueError("clone directory identity changed")
        for handle, path, before in source_files + destination_files:
            if api.snapshot(handle, path, False) != before:
                raise ValueError("retained file identity changed at collective validation")
        parent_after = api.snapshot(parent_handle, parent, True)
        if _identity(parent_after) != _identity(parent_before):
            raise ValueError("retained parent identity changed")
        receipt["parentAfter"] = parent_after
        receipt["cloneFinalIdentity"] = api.snapshot(clone_handle, clone, True)
        receipt["sourceStableAcrossAcquisition"] = True
        receipt["manifestsMatchPinnedEntries"] = True
        if on_retained is not None:
            phase = "retainedHandoff"
            handoff = on_retained(receipt)
            receipt["retainedHandoff"] = handoff
            if not isinstance(handoff, dict) or handoff.get("completed") is not True:
                raise ValueError("retained handoff did not verify completion")
        receipt["acquisitionOutcome"] = "ACQUIRED_FRESH_CLONE_STATIC_ONLY"
    except Exception as exc:
        receipt["errors"].append({"phase": phase, "type": type(exc).__name__, "message": str(exc)})
    finally:
        for handle, role, path, directory in reversed(handles):
            try:
                api.close(handle)
                receipt["handlesClosed"] += 1
            except Exception as exc:
                receipt["errors"].append({"phase": "CloseHandle", "role": role, "path": str(path),
                    "type": type(exc).__name__, "message": str(exc)})
        receipt["allHandlesClosed"] = receipt["handlesClosed"] == receipt["handlesOpened"]
        if receipt["errors"]:
            receipt["acquisitionOutcome"] = "INCOMPLETE"
        receipt["finishedUtc"] = datetime.now(timezone.utc).isoformat()
    return receipt


def acquire_fixed():
    raw = INSTALLED_RECEIPT.read_bytes()
    digest = hashlib.sha256(raw).hexdigest()
    if digest != INSTALLED_RECEIPT_SHA256:
        raise ValueError("CP288 installed-identity receipt SHA-256 mismatch: " + digest)
    installed = json.loads(raw)
    if (installed.get("status") != "ACQUIRED_STATIC_IDENTITIES_ONLY"
            or installed.get("original_run_ready") is not False
            or installed.get("runtimeLoadedModuleIdentity") != "unknown"):
        raise ValueError("CP288 static-evidence qualification mismatch")
    entries = gate.load_reference(REFERENCE)
    receipt = _acquire(gate.EXPECTED_SOURCE_ROOT, gate.PROBLEM_ROOT, entries, production=True,
        installed_evidence={"path": str(INSTALLED_RECEIPT), "sha256": digest,
            "qualification": "static installed-file evidence; not runtime loader proof"})
    receipt["sourcePins"] = {"referencePath": str(REFERENCE),
        "referenceSha256": gate.EXPECTED_REFERENCE_SHA256,
        "contentDigest": gate.EXPECTED_CONTENT_DIGEST,
        "projectSha256": gate.EXPECTED_PROJECT_SHA256,
        "gateSourceSha256": hashlib.sha256(Path(gate.__file__).read_bytes()).hexdigest()}
    return receipt


def main():
    if len(sys.argv) != 1:
        raise SystemExit("No arguments supported; one fixed B98 acquisition and output only.")
    # Reserve the diagnostic output first. A repeated CLI invocation cannot
    # silently produce a second clone or overwrite the sole acquisition receipt.
    with OUTPUT.open("x", encoding="utf-8") as stream:
        stream.write(json.dumps({"status": "HOLD", "original_run_ready": False,
            "acquisitionOutcome": "STARTED_RECEIPT_RESERVED"}) + "\n")
        stream.flush()
        try:
            receipt = acquire_fixed()
        except Exception as exc:
            receipt = {"status": "HOLD", "original_run_ready": False,
                "acquisitionOutcome": "INCOMPLETE", "errors": [{"phase": "fixedPreflight",
                "type": type(exc).__name__, "message": str(exc)}]}
        stream.seek(0)
        stream.write(json.dumps(receipt, indent=2) + "\n")
        stream.truncate()
        stream.flush()
    print(json.dumps({k: receipt.get(k) for k in ("status", "original_run_ready", "cloneRoot",
        "acquisitionOutcome", "handlesOpened", "handlesClosed", "allHandlesClosed", "errors")}))
    return 0 if receipt["acquisitionOutcome"] == "ACQUIRED_FRESH_CLONE_STATIC_ONLY" else 1


if __name__ == "__main__":
    raise SystemExit(main())
