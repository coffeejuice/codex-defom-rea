"""Offline valid-domain model of original RVA 11a0; never runs the original.

Malformed strings are intentionally rejected here, not assigned invented native
results: the original ignores %x's return and has a ceil/floor count mismatch.
"""
import json


def encode(path):
    raw = path.encode("utf-16-le", errors="surrogatepass")
    return "".join(f"{int.from_bytes(raw[i:i+2], 'little'):04x}" for i in range(0, len(raw), 2))


def decode_valid(encoded):
    if len(encoded) % 4 or any(c not in "0123456789abcdefABCDEF" for c in encoded):
        raise ValueError("Outside the deterministic four-hex-digit domain")
    raw = b"".join(int(encoded[i:i+4], 16).to_bytes(2, "little") for i in range(0, len(encoded), 4))
    return raw.decode("utf-16-le", errors="surrogatepass")


if __name__ == "__main__":
    path = r"C:\PROBLEM\p202a\Gear_blank.moproj"
    probes = [path, "A", "\u00e9", "\U0001f600", ""]
    for probe in probes:
        if decode_valid(encode(probe)) != probe:
            raise RuntimeError("Valid-domain roundtrip failed")
    malformed_rejected = False
    try:
        decode_valid(path)
    except ValueError:
        malformed_rejected = True
    if not malformed_rejected:
        raise RuntimeError("Literal CLI path must not be accepted by the valid-domain model")
    print(json.dumps({
        "qualification": "offline static-derived valid-domain model; no original execution",
        "path": path, "literalCodeUnits": len(path), "literalRemainderMod4": len(path) % 4,
        "encoded": encode(path), "encodedCodeUnits": len(encode(path)),
        "roundtrips": len(probes), "literalOutsideValidDomain": malformed_rejected,
    }, indent=2))
