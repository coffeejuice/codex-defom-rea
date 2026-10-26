"""Explicit test fixtures and disposable generator output; no upstream fallbacks."""
import os
from pathlib import Path


def fixture(name):
    value = os.environ.get(name)
    if not value:
        raise SystemExit("Required fixture setting missing: " + name)
    path = Path(value)
    if not path.is_absolute():
        raise SystemExit("Fixture setting must be absolute: " + name)
    return path


def output_path(value):
    path = Path(value).resolve()
    root = fixture("M2M_TEST_SCRATCH").resolve()
    if path == root or not path.is_relative_to(root):
        raise SystemExit("Generator output must be below M2M_TEST_SCRATCH: " + str(path))
    path.parent.mkdir(parents=True, exist_ok=True)
    return path
