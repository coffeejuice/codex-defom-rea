"""ctypes wrapper for moproj2mst.dll (64-bit Python on Windows; standard library only).

    python moproj2mst.py [--dll path\\to\\moproj2mst.dll] <project.moproj> [out.MST]

As a module:
    from moproj2mst import Converter
    m2m = Converter(r"C:\\tools\\moproj2mst.dll")
    rc, message = m2m.convert(r"C:\\p\\job.moproj")      # rc 0 = OK, message = warnings
"""
import ctypes
import os
import sys

M2M_OK = 0
M2M_E_ARG, M2M_E_OPEN, M2M_E_PARSE, M2M_E_INPUT = 1, 2, 3, 4
M2M_E_NO_OPERATIONS, M2M_E_WRITE, M2M_E_UNSUPPORTED = 5, 6, 7
M2M_FLAG_DOE_TEMP = 0x1
M2M_FLAG_STRICT = 0x2


class Converter:
    def __init__(self, dll_path=None):
        if dll_path is None:
            dll_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'moproj2mst.dll')
        self._dll = ctypes.CDLL(os.path.abspath(dll_path))
        self._dll.m2m_api_version.restype = ctypes.c_int
        self._dll.m2m_api_version.argtypes = []
        self._dll.m2m_set_codepage.restype = ctypes.c_int
        self._dll.m2m_set_codepage.argtypes = [ctypes.c_uint]
        self._dll.m2m_set_gui_unit.restype = ctypes.c_int
        self._dll.m2m_set_gui_unit.argtypes = [ctypes.c_uint]
        self._dll.m2m_convert_w.restype = ctypes.c_int
        self._dll.m2m_convert_w.argtypes = [ctypes.c_wchar_p, ctypes.c_wchar_p, ctypes.c_uint,
                                            ctypes.c_wchar_p, ctypes.c_size_t]

    def api_version(self):
        return self._dll.m2m_api_version()

    def set_codepage(self, codepage):
        return self._dll.m2m_set_codepage(codepage)

    def set_gui_unit(self, unit):
        """0 = derive the GUI start-up unit as DEF_GUI_MO does (default), 1 = SI, 2 = English."""
        return self._dll.m2m_set_gui_unit(unit)

    def convert(self, moproj_path, out_mst_path=None, flags=0):
        """Returns (result code, message). out_mst_path None: next to the project."""
        msg = ctypes.create_unicode_buffer(4096)
        rc = self._dll.m2m_convert_w(moproj_path, out_mst_path, flags, msg, len(msg))
        return rc, msg.value


def main(argv):
    dll = None
    if len(argv) >= 2 and argv[0] == '--dll':
        dll, argv = argv[1], argv[2:]
    if not argv:
        print(__doc__, file=sys.stderr)
        return M2M_E_ARG
    m2m = Converter(dll)
    print('api', m2m.api_version())
    print('unit', m2m.set_gui_unit(0))
    rc, message = m2m.convert(argv[0], argv[1] if len(argv) > 1 else None)
    print('result', rc)
    if message:
        print(message)
    return rc


if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))
