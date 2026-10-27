"""CP307 private fixed-fixture preflight; deliberately does not launch a process.

The only accepted image is cloneRoot\\fixture.exe from a retained CP289
acquisition, with CP302's pinned source, image and build-receipt hashes. This
module checks protected-file-to-image provenance and measured file bytes. It
does not measure loaded bytes or prove that a process maps this file. The
injected fake API is a trusted convention and may have arbitrary side effects.
"""
from __future__ import annotations

import hashlib
import json
from pathlib import Path, PureWindowsPath
import re

import color_atomic_clone_acquire as clone
import color_clone_manifest_gate as gate

CP302_SHA256 = "66eadac3f5dd6ee0d0b856a53760dc9270ecfd170808c62dff01b99a583f6a28"
CP302_SOURCE_SHA256 = "31afbe79ea842eb663be7d278ea389dddd70d5c628106371b5ef3e6a55b703b1"
CP302_BUILD_RECEIPT_SHA256 = "53a71fc4383b293c45c8751ead0fe67cb2686372af3a3acdf86e68f697cad4d7"
HERE = Path(__file__).resolve().parent
FIXTURE = HERE / "fixtures/cp302/fixture.exe"
SOURCE = HERE / "fixtures/cp302/exit_only.c"
BUILD_RECEIPT = HERE.parent / "analysis/cp302-fixture-build.json"


def _verify_pins():
    if hashlib.sha256(FIXTURE.read_bytes()).hexdigest() != CP302_SHA256:
        raise ValueError("CP302 fixture hash mismatch")
    if hashlib.sha256(SOURCE.read_bytes()).hexdigest() != CP302_SOURCE_SHA256:
        raise ValueError("CP302 source hash mismatch")
    raw = BUILD_RECEIPT.read_bytes()
    if hashlib.sha256(raw).hexdigest() != CP302_BUILD_RECEIPT_SHA256:
        raise ValueError("CP302 build receipt hash mismatch")
    receipt = json.loads(raw)
    pe = receipt.get("pe", {})
    if (receipt.get("source_sha256") != CP302_SOURCE_SHA256 or
            pe.get("sha256") != CP302_SHA256 or pe.get("bytes") != FIXTURE.stat().st_size or
            pe.get("machine") != "AMD64" or pe.get("subsystem") != "CONSOLE" or
            pe.get("imports") != ["KERNEL32.DLL"]):
        raise ValueError("CP302 build receipt content mismatch")
    return FIXTURE.stat().st_size


def _same_identity(a, b):
    return clone._identity(a) == clone._identity(b)


def _fixed_contract(receipt, root):
    """Validate CP289's retained *original* B98 contract, never use it to launch."""
    contract = receipt.get("launchContract")
    project = str(root / gate.PROJECT_NAME)
    encoded = gate.encode_utf16_units(project)
    expected = [str(gate.EXECUTABLE), "-m", "open", "-f", encoded]
    if (not isinstance(contract, dict) or contract.get("cwd") != str(root) or
            contract.get("projectPath") != project or contract.get("encodedPath") != encoded or
            contract.get("argv") != expected or
            receipt.get("commandGate", {}).get("exactUtf16ArgvCwd") is not True):
        raise ValueError("CP289 original launchContract does not match fixed B98 provenance")


def _validate_retained(receipt, size):
    if (not isinstance(receipt, dict) or
            receipt.get("schema") != "cp289-creation-bound-clone-v1" or
            receipt.get("production") is not False or
            receipt.get("sourceStableAcrossAcquisition") is not True or
            receipt.get("manifestsMatchPinnedEntries") is not True or
            receipt.get("acquisitionOutcome") != "ACQUIRED_FRESH_CLONE_STATIC_ONLY"):
        raise ValueError("complete retained CP289 fake acquisition required")
    root = PureWindowsPath(receipt.get("cloneRoot", ""))
    if (root.parent != PureWindowsPath(r"C:\PROBLEM") or
            re.fullmatch(r"m289_[0-9a-f]{16}", root.name) is None):
        raise ValueError("fixed fresh CP289 clone root required")
    image = root / "fixture.exe"
    if len(str(image)) >= 100:
        raise ValueError("fixture path must be shorter than 100 characters")
    if not _same_identity(receipt.get("createdCloneIdentity", {}),
                          receipt.get("cloneFinalIdentity", {})):
        raise ValueError("CP289 clone identity mismatch")
    if receipt["cloneFinalIdentity"].get("finalPath") != "\\\\?\\" + str(root):
        raise ValueError("CP289 retained clone path mismatch")
    _fixed_contract(receipt, root)
    copied = [item for item in receipt.get("copiedFiles", [])
              if item.get("path", "").casefold() == "fixture.exe"]
    if (len(copied) != 1 or copied[0].get("bytes") != size or
            copied[0].get("sha256") != CP302_SHA256 or
            copied[0].get("destinationIdentity", {}).get("finalPath") != "\\\\?\\" + str(image) or
            copied[0].get("destinationIdentity", {}).get("size") != size or
            not copied[0].get("destinationIdentity", {}).get("fileIndex") or
            not copied[0].get("sourceIdentity", {}).get("fileIndex")):
        raise ValueError("CP289 retained fixture destination identity/digest mismatch")
    return str(image), str(root), copied[0]


def preflight_retained(receipt, *, file_api):
    """Fake-only preflight from caller-supplied CP289 static receipt evidence.

    file_api must expose open_read(path), snapshot(handle,path), read_all(handle),
    close(handle), and the explicit cp307_fake_file_api marker. Snapshots are
    expected to be handle-backed identities. CP307 itself makes no launch call;
    the injected API is trusted test code and may have arbitrary side effects.
    """
    if getattr(file_api, "cp307_fake_file_api", False) is not True:
        raise RuntimeError("CP307 real file/process preflight HOLD")
    size = _verify_pins()
    image, root, copied = _validate_retained(receipt, size)
    handle = file_api.open_read(image)
    if handle is None:
        raise OSError("fake image open returned no handle")
    primary_error = None
    close_error = None
    result = None
    try:
        before = file_api.snapshot(handle, image)
        data = file_api.read_all(handle)
        digest = hashlib.sha256(data).hexdigest()
        after = file_api.snapshot(handle, image)
        if not _same_identity(before, after):
            raise ValueError("pre/post handle-backed file identity mismatch")
        if (len(data) != size or digest != CP302_SHA256 or
                before.get("size") != size or after.get("size") != size):
            raise ValueError("measured clone fixture bytes differ from CP302 pin")
        if not _same_identity(copied["destinationIdentity"], before):
            raise ValueError("retained CP289 destination identity differs from measured file")
        result = {"schema": "cp307-fixed-clone-preflight-v1", "status": "PREFLIGHT_ONLY",
                "cloneRoot": root, "image": image, "cwd": root, "argv": [image],
                "fixtureSha256": digest, "fixtureBytes": len(data),
                "protectedFileToImageBinding": "matched retained CP289 destination identity and CP302 digest",
                "measuredLoadedBytes": "unknown; no process was created or inspected",
                "preIdentity": before, "postIdentity": after,
                "receiptQualification": "caller-supplied CP289 static receipt; does not prove handles remain live",
                "cp307OwnedCalls": "file preflight only; no process or GUI calls",
                "operationScope": "CP307-owned calls only; supplied fake API may have arbitrary side effects"}
    except BaseException as error:
        primary_error = error
    finally:
        try:
            file_api.close(handle)
        except BaseException as error:
            close_error = error
    if primary_error is not None:
        if close_error is not None:
            raise RuntimeError(
                f"preflight failed ({type(primary_error).__name__}: {primary_error}); "
                f"fake handle close is uncertain ({type(close_error).__name__}: {close_error})"
            ) from primary_error
        raise primary_error
    if close_error is not None:
        raise RuntimeError(f"preflight succeeded but fake handle close is uncertain: {close_error}") from close_error
    result["handleClose"] = "fake API close returned successfully"
    return result


def acquire_real(*_args, **_kwargs):
    raise RuntimeError("CP307 real process, original application and GUI entry HOLD")
