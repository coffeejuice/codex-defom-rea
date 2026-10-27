"""CP300 synthetic-only cloned-image handoff scaffold; no original execution.

The fixed B98 source has no synthetic executable entry and cannot use this
coordinator. No CLI or real-execution entry point is provided pending review.
"""
from __future__ import annotations

import hashlib
import os
from pathlib import Path, PureWindowsPath
import re
import uuid

import color_atomic_clone_acquire as clone
from cp214_owned_suspended_process import OwnedProcess


ORIGINAL_EXE_SHA256 = "fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc"
PRIOR_CLONES = {"p214a", "m289_999cfa585601449a"}


def _validate_pin(pin):
    if not isinstance(pin, dict) or set(pin) != {"exe", "sha256"}:
        raise ValueError("exact synthetic fixture path and SHA-256 pin required")
    exe, digest = pin["exe"], pin["sha256"]
    if not isinstance(exe, str) or not isinstance(digest, str):
        raise ValueError("synthetic fixture pin types invalid")
    path = PureWindowsPath(exe)
    if (not path.is_absolute() or len(exe) >= 100 or
            str(path.parent).casefold().startswith("c:\\problem\\") is False or
            path.name.casefold() != "fixture.exe"):
        raise ValueError("synthetic fixture must be C:\\PROBLEM\\...\\fixture.exe under 100 characters")
    if re.fullmatch(r"[0-9a-f]{64}", digest) is None or digest == ORIGINAL_EXE_SHA256:
        raise ValueError("synthetic fixture digest invalid or matches the original")
    if os.path.normcase(os.path.abspath(exe)) == os.path.normcase(os.path.abspath(clone.gate.EXECUTABLE)):
        raise ValueError("original executable is forbidden")
    resolved = Path(exe).resolve(strict=True)
    resolved_windows = PureWindowsPath(str(resolved))
    if (str(resolved_windows.parent).casefold().startswith("c:\\problem\\") is False or
            resolved_windows.name.casefold() != "fixture.exe" or len(str(resolved)) >= 100):
        raise ValueError("synthetic fixture resolved outside C:\\PROBLEM or changed identity")
    # The bytes are checked here and again inside OwnedProcess immediately before
    # CreateProcessW. An intervening path change remains a refusal there.
    if hashlib.sha256(resolved.read_bytes()).hexdigest() != digest:
        raise ValueError("synthetic fixture SHA-256 mismatch")
    return {"exe": str(resolved), "sha256": digest}


def retained_child_handoff(receipt, pin, *, owned_factory):
    """Called only from CP289 while all verified clone/source/parent handles live."""
    if owned_factory is OwnedProcess:
        raise RuntimeError("real owned child HOLD pending fixed synthetic fixture review")
    handoff = {"schema": "cp300-synthetic-retained-handoff-v1", "completed": False,
               "originalRun": False, "resumed": False, "childIdentity": None,
               "suspendedStatus": None, "cleanup": None,
               "cleanupCertainty": "unknown", "errors": []}
    child = None
    try:
        root = PureWindowsPath(receipt["cloneRoot"])
        command = receipt["commandGate"]
        command_verified = (command.get("exactUtf16ArgvCwd") is True or
                            (command.get("sourceMatchesFrozenManifest") is True and
                             command.get("canonicalUtf16CodeUnitHex") is True and
                             command.get("proposedClone", {}).get("matchesFrozenSource") is True and
                             command.get("argv") == receipt["launchContract"]["argv"] and
                             command.get("cwd") == receipt["launchContract"]["cwd"]))
        if (root.name.casefold() in PRIOR_CLONES or
                re.fullmatch(r"m289_[0-9a-f]{16}", root.name) is None or
                receipt["sourceStableAcrossAcquisition"] is not True or
                receipt["manifestsMatchPinnedEntries"] is not True or
                not command_verified or
                clone._identity(receipt["createdCloneIdentity"]) !=
                clone._identity(receipt["cloneFinalIdentity"])):
            raise ValueError("fresh clone identity or collective validation mismatch")
        handoff["cloneRoot"] = str(root)
        handoff["cloneIdentity"] = receipt["cloneFinalIdentity"]
        image = root / "fixture.exe"
        matches = [file for file in receipt["copiedFiles"]
                   if file["path"].casefold() == "fixture.exe"]
        if (len(matches) != 1 or matches[0]["sha256"] != pin["sha256"] or
                matches[0]["destinationIdentity"]["finalPath"] !=
                "\\\\?\\" + str(image)):
            raise ValueError("cloned fixture digest or retained destination identity mismatch")
        handoff["fixturePin"] = pin
        handoff["clonedImage"] = str(image)
        handoff["clonedImageDestinationIdentity"] = matches[0]["destinationIdentity"]
        child = owned_factory(str(image), str(root), [], pin["sha256"])
        identity = child.identity
        handoff["childIdentity"] = identity
        required = ("pid", "primary_tid", "creation_filetime", "canonical_image",
                    "sha256", "cwd", "argv")
        if (not isinstance(identity, dict) or any(k not in identity for k in required)
                or not all(isinstance(identity[k], int) and identity[k] > 0
                           for k in required[:3])
                or identity["sha256"] != pin["sha256"]
                or os.path.normcase(identity["canonical_image"]) != os.path.normcase(str(image))
                or os.path.normcase(identity["cwd"]) != os.path.normcase(str(root))
                or identity["argv"] != [identity["canonical_image"]]):
            raise ValueError("owned child identity mismatch")
        status = child.status()
        handoff["suspendedStatus"] = status
        if (status.get("state") != "suspended" or status.get("wait_result") != 258
                or status.get("exit_verified") is not False):
            raise ValueError("owned child suspended state not verified")
    except BaseException as exc:
        handoff["errors"].append({"phase": "childOrIdentity", "type": type(exc).__name__,
                                  "message": str(exc)})
    finally:
        if child is not None:
            try:
                cleanup = child.close()  # CP214 terminates and waits its exact retained process.
                handoff["cleanup"] = cleanup
                attempts = cleanup.get("termination_attempts", [])
                if (cleanup.get("state") != "exited" or cleanup.get("exit_verified") is not True
                        or cleanup.get("wait_result") != 0 or
                        cleanup.get("termination_requested") is not True or
                        not attempts or attempts[-1].get("exit_verified") is not True or
                        attempts[-1].get("wait_result") != 0):
                    raise ValueError("exact owned child termination/wait not verified")
                handoff["cleanupCertainty"] = "verified_exited_and_closed"
            except BaseException as exc:
                handoff["errors"].append({"phase": "childCleanup", "type": type(exc).__name__,
                                          "message": str(exc)})
                handoff["cleanupCertainty"] = "uncertain_child_may_remain_live"
    handoff["completed"] = not handoff["errors"]
    return handoff


def _acquire_synthetic_test(source, parent, entries, pin, *, api,
                            owned_factory, name=None):
    """Private fake-only seam; real execution entry point remains disabled."""
    if api is None or owned_factory is None or owned_factory is OwnedProcess or isinstance(api, clone.WindowsCloneFiles):
        raise ValueError("fake API and fake owned-process factory required")
    pin = _validate_pin(pin)  # No clone creation before the fixture is pinned.
    if (Path(pin["exe"]) != source / "fixture.exe" or
            len([item for item in entries if item.get("path", "").casefold() == "fixture.exe"
                 and item.get("kind") == "file" and item.get("sha256") == pin["sha256"]]) != 1):
        raise ValueError("source fixture must be the pinned manifest fixture.exe entry")
    chosen = name if name is not None else "m289_" + uuid.uuid4().hex[:16]
    if chosen.casefold() in PRIOR_CLONES:
        raise ValueError("prior clone reuse forbidden")
    return clone._acquire(source, parent, entries, api=api, name=chosen,
                          production=False,
                          on_retained=lambda receipt: retained_child_handoff(
                              receipt, pin, owned_factory=owned_factory))


def acquire_synthetic(*_args, **_kwargs):
    """Real execution is held until source, manifest and cleanup review."""
    raise RuntimeError("CP300 cloned-image real execution HOLD; use fake tests only")
