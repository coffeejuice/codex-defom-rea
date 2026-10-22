"""Read aggregate resources and recommend the measured reference-test parallelism."""
import ctypes
import json
import os
import sys


def main():
    cpu = getattr(os, 'process_cpu_count', os.cpu_count)()
    total = available = None
    if sys.platform == 'win32':
        class Memory(ctypes.Structure):
            _fields_ = [('length', ctypes.c_uint32), ('load', ctypes.c_uint32),
                        *[(name, ctypes.c_uint64) for name in
                          ['total_phys', 'available_phys', 'total_page', 'available_page',
                           'total_virtual', 'available_virtual', 'extended']]]
        memory = Memory()
        memory.length = ctypes.sizeof(memory)
        if ctypes.windll.kernel32.GlobalMemoryStatusEx(ctypes.byref(memory)):
            total, available = memory.total_phys, memory.available_phys
    jobs = 8 if cpu is not None and cpu >= 16 and available is not None and available >= 32 * 2**30 else 4
    print(json.dumps(dict(logical_processors=cpu, total_memory_bytes=total,
                          available_memory_bytes=available, recommended_ctest_jobs=jobs,
                          benchmark='analysis/efficiency-regression-parallelism-20261021.json')))


if __name__ == '__main__':
    main()
