// Minimal C++ caller of moproj2mst.dll.
//   convert <project.moproj> [out.MST]
// Build: include apps/moproj2mst/include, link moproj2mst.lib, ship moproj2mst.dll next to the exe.
#include <cstdio>
#include <cwchar>

#include "moproj2mst.h"

int wmain(int argc, wchar_t** argv)
{
    if (argc < 2) {
        std::fputs("usage: convert <project.moproj> [out.MST]\n", stderr);
        return M2M_E_ARG;
    }
    std::printf("api %d\n", m2m_api_version());
    // Unit for unit-dependent defaults: 0 = as the original derives it (default), 1 = SI, 2 = English.
    std::printf("unit %d\n", m2m_set_gui_unit(0));

    wchar_t msg[4096];
    // NULL output path: <project folder>/<project name>.MST, as DEF_GUI_MO saves it.
    int rc = m2m_convert_w(argv[1], argc > 2 ? argv[2] : nullptr, 0, msg, sizeof msg / sizeof msg[0]);

    // M2M_OK: msg holds warnings (one per line) or is empty. Otherwise msg is the error text.
    std::printf("result %d\n", rc);
    if (msg[0]) std::wprintf(L"%ls\n", msg);
    return rc;
}
