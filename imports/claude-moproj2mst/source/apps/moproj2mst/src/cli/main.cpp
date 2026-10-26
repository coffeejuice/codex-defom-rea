// moproj2mst.exe: command line over the C ABI (owner: G0).
//   moproj2mst <project.moproj> [out.MST] [--strict] [--codepage N] [--write-mdt] [--write-save] [--deform-dir D]
// Exit code = result code (0 = OK). Errors and warnings go to stderr (UTF-8).
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <cstdio>
#include <cwchar>
#include <filesystem>
#include <string>
#include <vector>

#include "moproj2mst.h"
#include "isolated_save.hpp"

static void printUtf8(const wchar_t* s)
{
    int n = WideCharToMultiByte(CP_UTF8, 0, s, -1, nullptr, 0, nullptr, nullptr);
    if (n <= 1) return;
    std::string u(static_cast<size_t>(n), '\0');
    WideCharToMultiByte(CP_UTF8, 0, s, -1, u.data(), n, nullptr, nullptr);
    std::fputs(u.c_str(), stderr);
    std::fputs("\n", stderr);
}

static int usage()
{
    std::fputs("usage: moproj2mst <project.moproj> [out.MST] [--strict] [--codepage N] [--write-mdt] [--write-save] [--deform-dir D] [--isolated-save --isolated-receipt ABSOLUTE_PATH]\n", stderr);
    return M2M_E_ARG;
}

int wmain(int argc, wchar_t** argv)
{
    // No error dialogs when run unattended by the corpus runner.
    SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX | SEM_NOOPENFILEERRORBOX);
    if (argc > 1 && std::wcscmp(argv[1], L"--m2m-isolated-save-worker-v1") == 0)
        return m2m::cli::isolatedSaveWorkerMain(argc, argv);

    std::vector<const wchar_t*> pos;
    unsigned flags = 0;
    unsigned codepage = 1252;
    bool isolatedSave = false;
    std::wstring isolatedReceipt, deformDir;
    for (int i = 1; i < argc; ++i) {
        const wchar_t* a = argv[i];
        if (std::wcscmp(a, L"--strict") == 0) {
            flags |= M2M_FLAG_STRICT;
        } else if (std::wcscmp(a, L"--codepage") == 0 && i + 1 < argc) {
            wchar_t* end = nullptr;
            unsigned long cp = std::wcstoul(argv[++i], &end, 10);
            if (!end || *end || m2m_set_codepage(static_cast<unsigned>(cp)) != M2M_OK) return usage();
            codepage = static_cast<unsigned>(cp);
        } else if (std::wcscmp(a, L"--write-mdt") == 0) {
            flags |= M2M_FLAG_WRITE_MDT;
        } else if (std::wcscmp(a, L"--write-save") == 0) {
            flags |= M2M_FLAG_WRITE_SAVE;
        } else if (std::wcscmp(a, L"--deform-dir") == 0 && i + 1 < argc) {
            if (m2m_set_deform_dir_w(argv[++i]) != M2M_OK) return usage();
            deformDir = argv[i];
        } else if (std::wcscmp(a, L"--isolated-save") == 0) {
            isolatedSave = true;
        } else if (std::wcscmp(a, L"--isolated-receipt") == 0 && i + 1 < argc) {
            isolatedReceipt = argv[++i];
        } else if (std::wcscmp(a, L"--version") == 0) {
            std::printf("moproj2mst api %d\n", m2m_api_version());
            return 0;
        } else if (a[0] == L'-' && a[1] == L'-') {
            return usage();
        } else {
            pos.push_back(a);
        }
    }
    if (pos.empty() || pos.size() > 2) return usage();
    if (isolatedSave || !isolatedReceipt.empty()) {
        if (!isolatedSave || !(flags & M2M_FLAG_WRITE_SAVE) || pos.size() != 2 ||
            isolatedReceipt.empty() || deformDir.empty() ||
            !std::filesystem::path(pos[0]).is_absolute() ||
            !std::filesystem::path(pos[1]).is_absolute() ||
            !std::filesystem::path(deformDir).is_absolute() ||
            !std::filesystem::path(isolatedReceipt).is_absolute())
            return usage();
        return m2m::cli::runIsolatedSave({pos[0], pos[1], deformDir,
                                          isolatedReceipt, flags, codepage});
    }

    std::vector<wchar_t> msg(1 << 16);
    int rc = m2m_convert_w(pos[0], pos.size() > 1 ? pos[1] : nullptr, flags, msg.data(), msg.size());
    if (msg[0]) printUtf8(msg.data());
    return rc;
}
