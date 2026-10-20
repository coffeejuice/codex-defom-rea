"""Read-only structural inspection of the supplied DEFORM 3D v11 DB fixtures.

Layout is derived from the saved REA dossiers listed in EVIDENCE_FILES. This is
an independent fixture validator, not a general DEFORM loader or a replacement
for runtime comparison with the original executable.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import math
import struct
from pathlib import Path

RECORD_BYTES = 512
EVIDENCE_FILES = (
    "physical_header", "read_database_header", "step_table",
    "integer_directory", "real_directory", "load_integers", "load_reals",
    "unpack_integers", "unpack_reals",
)


def inspect(path: Path) -> dict:
    size = path.stat().st_size
    errors: list[str] = []
    structure_hash = hashlib.sha256()
    full_hash = hashlib.sha256()
    with path.open("rb") as source:
        while block := source.read(8 * 1024 * 1024):
            full_hash.update(block)

        def read_record(record: int, length: int, context: str) -> bytes:
            offset = (record - 1) * RECORD_BYTES
            if record <= 0 or length < 0 or offset + length > size:
                raise ValueError(
                    f"{context}: record={record}, offset={offset}, bytes={length}, "
                    f"file size={size}"
                )
            source.seek(offset)
            data = source.read(length)
            if len(data) != length:
                raise ValueError(f"{context}: short read")
            structure_hash.update(struct.pack("<QQ", offset, length))
            structure_hash.update(data)
            return data

        first = read_record(1, RECORD_BYTES, "signature")
        header = read_record(2, RECORD_BYTES, "physical header")
        signature = first.split(b"\0", 1)[0].decode("ascii")
        # The requested fixtures are confirmed big-endian. Other format/version
        # families are deliberately rejected rather than guessed.
        if signature != "SFTC-DEF-3D-DB100-00":
            raise ValueError(f"Unsupported fixture signature: {signature!r}")
        ints = struct.unpack_from(">6i", header, 112)
        title = header[:80].decode("ascii").rstrip(" \0")
        text_fields = [
            header[offset:offset + 8].decode("ascii").rstrip(" \0")
            for offset in (80, 88, 96, 104)
        ]
        if text_fields[0] != "11.00":
            raise ValueError(f"Unsupported fixture version: {text_fields[0]}")
        stored_steps = ints[2]
        if stored_steps < 0:
            raise ValueError("Negative stored-step count")
        expected_tables = math.ceil(stored_steps / 128)
        expected_roots = math.ceil(expected_tables / 127)
        if expected_roots > 200:
            raise ValueError("Root table count exceeds observed original limit")
        roots = []
        table_records = []
        record = 3
        seen_roots: set[int] = set()
        for root_index in range(expected_roots):
            if record in seen_roots:
                raise ValueError("Root table cycle")
            seen_roots.add(record)
            raw = read_record(record, RECORD_BYTES, "step roots")
            values = list(struct.unpack(">128i", raw))
            remaining = expected_tables - len(table_records)
            used = values[:min(remaining, 127)]
            if any(pointer <= 0 for pointer in used):
                raise ValueError("Missing required step-table pointer")
            roots.append({
                "record": record,
                "byte_offset": (record - 1) * RECORD_BYTES,
                "used_table_records": used,
                "next_root_record": values[-1],
                "unused_nonzero_pointers": [
                    value for value in values[len(used):127] if value != 0
                ],
            })
            table_records.extend(used)
            record = values[-1]

        def inspect_directory(row: tuple[int, ...], kind: str) -> dict:
            length_index, record_index, element_size = (
                (3, 11, 4) if kind == "integer" else (4, 12, 8)
            )
            word_count, directory_record = row[length_index], row[record_index]
            result = {
                "word_count": word_count,
                "record": directory_record,
                "read_performed": word_count != 0,
            }
            if word_count == 0:
                return result
            raw = read_record(directory_record, word_count * 4, f"{kind} directory")
            values = struct.unpack(">" + str(word_count) + "i", raw)
            if len(values) < 2:
                raise ValueError("Directory missing its two-word header")
            count = values[0]
            if count < 0 or 2 + count * 4 > word_count:
                raise ValueError(f"Directory count {count} exceeds {word_count} words")
            result.update({
                "count": count,
                "second_header_word": values[1],
                "exact_directory_word_count": 2 + count * 4 == word_count,
                "entries_columns": [
                    "logical_capacity", "destination_start_1based",
                    "stored_element_count", "physical_record",
                ],
            })
            entries = [list(values[2 + i * 4:6 + i * 4]) for i in range(count)]
            result["entries"] = entries
            physical_spans = []
            logical_spans = []
            packed_payloads = 0
            packed_run_nodes = 0
            no_runs = 0
            packing_errors = []
            errors_before = len(errors)
            for index, (capacity, destination, stored_count, data_record) in enumerate(entries):
                if capacity < 0 or stored_count < 0:
                    errors.append(f"step {row[1]} {kind} entry {index}: negative count")
                    continue
                if capacity > 0 and destination < 1:
                    errors.append(f"step {row[1]} {kind} entry {index}: invalid destination")
                if capacity > 0 and destination >= 1:
                    logical_spans.append((destination - 1, destination - 1 + capacity))
                if stored_count == 0:
                    continue
                start = (data_record - 1) * RECORD_BYTES
                end = start + stored_count * element_size
                if data_record <= 0 or start < 0 or end > size:
                    errors.append(
                        f"step {row[1]} {kind} entry {index}: out-of-file payload "
                        f"record={data_record}, stored={stored_count}"
                    )
                else:
                    source.seek(start)
                    payload = source.read(stored_count * element_size)
                    code = "i" if kind == "integer" else "d"
                    if stored_count < 2:
                        packing_errors.append(f"entry {index}: no two-element trailer")
                    else:
                        output_count, pointer_value = struct.unpack_from(
                            ">2" + code, payload, len(payload) - 2 * element_size
                        )
                        if not (
                            math.isfinite(output_count)
                            and math.isfinite(pointer_value)
                            and int(output_count) == output_count
                            and int(pointer_value) == pointer_value
                        ):
                            packing_errors.append(f"entry {index}: invalid trailer integers")
                        else:
                            pointer = int(pointer_value)
                            if pointer == 0:
                                no_runs += 1
                            expanded_count = stored_count - 2
                            previous_pointer = stored_count + 1
                            chain_valid = True
                            while pointer:
                                if (
                                    pointer < 3 or pointer > stored_count - 2
                                    or pointer > previous_pointer - 3
                                ):
                                    chain_valid = False
                                    break
                                repeat, next_value = struct.unpack_from(
                                    ">2" + code, payload, (pointer - 2) * element_size
                                )
                                if not (
                                    math.isfinite(repeat) and math.isfinite(next_value)
                                    and repeat == int(repeat) and next_value == int(next_value)
                                    and repeat > 0
                                ):
                                    chain_valid = False
                                    break
                                expanded_count += int(repeat) - 3
                                previous_pointer, pointer = pointer, int(next_value)
                                packed_run_nodes += 1
                            if (
                                chain_valid and expanded_count == capacity
                                and output_count == capacity
                            ):
                                packed_payloads += 1
                            else:
                                packing_errors.append(
                                    f"entry {index}: expanded={expanded_count}, "
                                    f"trailer={output_count}, capacity={capacity}, "
                                    f"chain_valid={chain_valid}"
                                )
                physical_spans.append((start, end))
            result.update({
                "stored_element_count_total": sum(entry[2] for entry in entries),
                "nonempty_payloads": len(physical_spans),
                "logical_capacity_sum": sum(entry[0] for entry in entries),
                "logical_end_exclusive": max((e for _, e in logical_spans), default=0),
                "payload_min_byte_offset": min((s for s, _ in physical_spans), default=None),
                "payload_max_byte_end_exclusive": max((e for _, e in physical_spans), default=None),
                "payloads_starting_at_or_above_4gib": sum(s >= 2**32 for s, _ in physical_spans),
                "payloads_crossing_4gib": sum(s < 2**32 < e for s, e in physical_spans),
                "bounds_valid": len(errors) == errors_before,
                "packing": {
                    "profile": "two-element length/link trailer; backward three-element run nodes",
                    "payloads_with_valid_chain_and_count": packed_payloads,
                    "payloads_without_runs": no_runs,
                    "run_nodes": packed_run_nodes,
                    "all_payload_chains_and_counts_valid": packed_payloads == len(physical_spans),
                    "errors": packing_errors,
                },
            })
            return result

        tables = []
        steps = []
        for table_index, table_record in enumerate(table_records):
            raw = read_record(table_record, 128 * 13 * 4, "step table")
            rows = list(struct.iter_unpack(">13i", raw))
            used = min(128, stored_steps - table_index * 128)
            table_rows = rows[:used]
            tables.append({
                "table_index_1based": table_index + 1,
                "record": table_record,
                "byte_offset": (table_record - 1) * RECORD_BYTES,
                "byte_length": len(raw),
                "rows_used": used,
                "first_step_number": table_rows[0][1] if table_rows else None,
                "last_step_number": table_rows[-1][1] if table_rows else None,
                "unused_nonzero_rows": [
                    index + used + 1 for index, row in enumerate(rows[used:]) if any(row)
                ],
                "sha256": hashlib.sha256(raw).hexdigest(),
            })
            for index, row in enumerate(table_rows):
                step_index = table_index * 128 + index + 1
                if row[7] != 3 or row[8] < 6000:
                    raise ValueError(
                        f"Step {step_index}: unsupported dimension/version {row[7]}/{row[8]}"
                    )
                header_data = read_record(row[10], row[2] * 8, "step header")
                steps.append({
                    "stored_index_1based": step_index,
                    "step_number": row[1],
                    "row": list(row),
                    "step_header": {
                        "record": row[10],
                        "byte_offset": (row[10] - 1) * RECORD_BYTES,
                        "double_count": row[2],
                        "sha256": hashlib.sha256(header_data).hexdigest(),
                    },
                    "integer_directory": inspect_directory(row, "integer"),
                    "real_directory": inspect_directory(row, "real"),
                })
        if path.stat().st_size != size:
            raise ValueError("Fixture size changed during inspection")
        return {
            "artifact": {
                "path": str(path.resolve()), "bytes": size,
                "sha256": full_hash.hexdigest(),
            },
            "signature": signature,
            "byte_order": "big-endian",
            "record_bytes": RECORD_BYTES,
            "physical_header": {
                "title": title,
                "version": text_fields[0],
                "release_date": text_fields[1],
                "initial_simulation_date": text_fields[2],
                "current_simulation_date": text_fields[3],
                "integer_fields": list(ints),
                "stored_step_count": stored_steps,
            },
            "roots": roots,
            "tables": tables,
            "steps": steps,
            "summary": {
                "step_rows_checked": len(steps),
                "integer_directories_read": sum(s["integer_directory"]["read_performed"] for s in steps),
                "real_directories_read": sum(s["real_directory"]["read_performed"] for s in steps),
                "step_headers_at_or_above_4gib": sum(s["step_header"]["byte_offset"] >= 2**32 for s in steps),
                "payloads_starting_at_or_above_4gib": sum(
                    s[k].get("payloads_starting_at_or_above_4gib", 0)
                    for s in steps for k in ("integer_directory", "real_directory")
                ),
                "file_record_aligned": size % RECORD_BYTES == 0,
                "next_record_matches_eof": (ints[3] - 1) * RECORD_BYTES == size,
                "bytes_after_next_record_offset": size - (ints[3] - 1) * RECORD_BYTES,
                "payloads_checked": sum(
                    s[k].get("nonempty_payloads", 0)
                    for s in steps for k in ("integer_directory", "real_directory")
                ),
                "run_nodes_checked": sum(
                    s[k].get("packing", {}).get("run_nodes", 0)
                    for s in steps for k in ("integer_directory", "real_directory")
                ),
                "all_payload_chains_and_counts_valid": all(
                    s[k].get("packing", {}).get("all_payload_chains_and_counts_valid", True)
                    for s in steps for k in ("integer_directory", "real_directory")
                ),
                "bounds_valid": not errors,
                "errors": errors,
            },
            "selected_structural_reads_sha256": structure_hash.hexdigest(),
        }


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("root", type=Path)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    repository = Path(__file__).resolve().parent.parent
    evidence = []
    for name in EVIDENCE_FILES:
        path = repository / "analysis" / f"{name}.json"
        dossier = json.loads(path.read_text(encoding="utf-8-sig"))
        evidence.append({
            "file": str(path), "evidence_id": dossier["evidence_id"],
            "procedure": dossier.get("parameters", {}).get("procedure"),
        })
    paths = sorted(p for p in args.root.rglob("*") if p.is_file() and p.suffix.lower() == ".db")
    fixtures = []
    for path in paths:
        print(f"Inspecting {path}", flush=True)
        fixtures.append(inspect(path))
        print(json.dumps(fixtures[-1]["summary"]), flush=True)
    report = {
        "method": "Independent read-only decoding of REA-derived physical records, roots, step tables, and array directories",
        "source_evidence": evidence,
        "fixtures": fixtures,
        "limitations": [
            "Restricted to observed big-endian SFTC-DEF-3D-DB100-00 v11 fixtures.",
            "Zero-length directories are reported without inferring inherited or stale state.",
            "Packed payload trailers, linked run nodes, and reconstructed element counts are checked independently; expanded value arrays are not materialized.",
            "Semantic field names and original executable runtime parity are not established by this validator.",
            "Directory capacities may be smaller than stored packed counts because compression metadata consumes words.",
            "No target process is executed or mutated by this script.",
        ],
    }
    args.output.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")


if __name__ == "__main__":
    main()
