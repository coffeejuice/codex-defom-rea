/* CP312 inert native WORKER fixture. Source review is not run authorization. */
#define _WIN32_WINNT 0x0A00
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <bcrypt.h>
#include <stdint.h>
#include <string.h>

#define CP312_MAGIC 0x32313343u
#define CP312_VERSION 2u
#define CP302_BYTES 108032u

static const WCHAR child_path[] = L"C:\\PROBLEM\\s302\\fixture.exe";
static const unsigned char child_sha256[32] = {
    0x66,0xea,0xda,0xc3,0xf5,0xdd,0x6e,0xe0,0xd0,0xb8,0x56,0xa5,0x37,0x60,0xdc,0x92,
    0x70,0xec,0xfd,0x17,0x08,0x08,0xc6,0x2d,0xff,0x01,0xb9,0x9a,0x58,0x3f,0x6a,0x28
};

#pragma pack(push, 1)
typedef struct {
    uint32_t magic, version;
    uint64_t child_job;
    unsigned char nonce[32];
} CP312_INIT;
typedef struct {
    uint32_t magic, version, create_called, create_ok, create_error;
    uint32_t pid, tid;
    uint32_t child_query_ok, child_query_error, child_member;
    uint64_t child_process_handle;
    unsigned char nonce[32];
} CP312_RESULT;
typedef struct {
    uint32_t magic, version;
    uint64_t child_process_handle;
    unsigned char nonce[32];
} CP312_ACK;
#pragma pack(pop)

static int read_exact(HANDLE pipe, void *out, DWORD length)
{
    unsigned char *bytes = (unsigned char *)out;
    DWORD done = 0, count;
    while (done < length) {
        if (!ReadFile(pipe, bytes + done, length - done, &count, NULL) || count == 0)
            return 0;
        done += count;
    }
    return 1;
}

static int write_exact(HANDLE pipe, const void *data, DWORD length)
{
    const unsigned char *bytes = (const unsigned char *)data;
    DWORD done = 0, count;
    while (done < length) {
        if (!WriteFile(pipe, bytes + done, length - done, &count, NULL) || count == 0)
            return 0;
        done += count;
    }
    return 1;
}

static void hold_until_supervisor_terminates(void)
{
    for (;;) Sleep(INFINITE);
}

static int noninheritable(HANDLE handle)
{
    DWORD flags = 0;
    return GetHandleInformation(handle, &flags) && !(flags & HANDLE_FLAG_INHERIT);
}

static int safe_job_limits(HANDLE job)
{
    JOBOBJECT_EXTENDED_LIMIT_INFORMATION info;
    DWORD returned = 0;
    ZeroMemory(&info, sizeof(info));
    return QueryInformationJobObject(job, JobObjectExtendedLimitInformation,
        &info, sizeof(info), &returned) &&
        (info.BasicLimitInformation.LimitFlags & JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE) != 0 &&
        (info.BasicLimitInformation.LimitFlags & JOB_OBJECT_LIMIT_SILENT_BREAKAWAY_OK) == 0;
}

/* Keep the opened image non-writable and non-deletable through CreateProcessW. */
static HANDLE checked_child_image(void)
{
    HANDLE file = INVALID_HANDLE_VALUE;
    BCRYPT_ALG_HANDLE algorithm = NULL;
    LARGE_INTEGER size;
    unsigned char *bytes = NULL, hash[32];
    int accepted = 0;
    file = CreateFileW(child_path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
                       FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    if (file == INVALID_HANDLE_VALUE || !GetFileSizeEx(file, &size) || size.QuadPart != CP302_BYTES)
        goto done;
    bytes = (unsigned char *)HeapAlloc(GetProcessHeap(), 0, CP302_BYTES);
    if (bytes == NULL || !read_exact(file, bytes, CP302_BYTES))
        goto done;
    if (BCryptOpenAlgorithmProvider(&algorithm, BCRYPT_SHA256_ALGORITHM, NULL, 0) < 0)
        goto done;
    if (BCryptHash(algorithm, NULL, 0, bytes, CP302_BYTES, hash, sizeof(hash)) < 0)
        goto done;
    accepted = memcmp(hash, child_sha256, sizeof(hash)) == 0;
done:
    if (algorithm != NULL) BCryptCloseAlgorithmProvider(algorithm, 0);
    if (bytes != NULL) HeapFree(GetProcessHeap(), 0, bytes);
    if (!accepted && file != INVALID_HANDLE_VALUE) {
        CloseHandle(file);
        file = INVALID_HANDLE_VALUE;
    }
    return file;
}

int wmain(void)
{
    HANDLE input = GetStdHandle(STD_INPUT_HANDLE), output = GetStdHandle(STD_OUTPUT_HANDLE);
    HANDLE child_job, image = INVALID_HANDLE_VALUE;
    HANDLE list[1];
    LPPROC_THREAD_ATTRIBUTE_LIST attributes = NULL;
    SIZE_T attribute_size = 0;
    STARTUPINFOEXW startup;
    PROCESS_INFORMATION child;
    CP312_INIT init;
    CP312_RESULT result;
    CP312_ACK ack;
    BOOL member = FALSE, created;
    DWORD flags = CREATE_SUSPENDED | EXTENDED_STARTUPINFO_PRESENT | CREATE_NO_WINDOW;
    WCHAR command[] = L"\"C:\\PROBLEM\\s302\\fixture.exe\"";

    if (input == INVALID_HANDLE_VALUE || output == INVALID_HANDLE_VALUE ||
        input == NULL || output == NULL || !read_exact(input, &init, sizeof(init)))
        hold_until_supervisor_terminates();
    if (init.magic != CP312_MAGIC || init.version != CP312_VERSION ||
        init.child_job == 0 || sizeof(void *) != 8)
        hold_until_supervisor_terminates();
    child_job = (HANDLE)(uintptr_t)init.child_job;
    if (!noninheritable(child_job) || !safe_job_limits(child_job) ||
        !IsProcessInJob(GetCurrentProcess(), child_job, &member) || member)
        hold_until_supervisor_terminates();

    ZeroMemory(&result, sizeof(result));
    result.magic = CP312_MAGIC;
    result.version = CP312_VERSION;
    memcpy(result.nonce, init.nonce, sizeof(init.nonce));
    ZeroMemory(&startup, sizeof(startup));
    ZeroMemory(&child, sizeof(child));
    image = checked_child_image();
    if (image != INVALID_HANDLE_VALUE) {
        InitializeProcThreadAttributeList(NULL, 1, 0, &attribute_size);
        if (attribute_size != 0)
            attributes = (LPPROC_THREAD_ATTRIBUTE_LIST)HeapAlloc(GetProcessHeap(), 0, attribute_size);
        if (attributes != NULL && InitializeProcThreadAttributeList(attributes, 1, 0, &attribute_size)) {
            list[0] = child_job;
            if (UpdateProcThreadAttribute(attributes, 0, PROC_THREAD_ATTRIBUTE_JOB_LIST,
                                          list, sizeof(list), NULL, NULL)) {
                startup.StartupInfo.cb = sizeof(startup);
                startup.lpAttributeList = attributes;
                result.create_called = 1;
                SetLastError(0);
                created = CreateProcessW(child_path, command, NULL, NULL, FALSE, flags,
                                         NULL, NULL, &startup.StartupInfo, &child);
                result.create_ok = (uint32_t)created;
                result.create_error = GetLastError(); /* Capture immediately, even on success. */
                if (created) {
                    result.pid = child.dwProcessId;
                    result.tid = child.dwThreadId;
                    result.child_process_handle = (uint64_t)(uintptr_t)child.hProcess;
                    SetLastError(0);
                    result.child_query_ok = IsProcessInJob(child.hProcess, child_job, &member) ? 1u : 0u;
                    result.child_query_error = GetLastError();
                    if (result.child_query_ok) result.child_member = member ? 1u : 0u;
                }
            }
            DeleteProcThreadAttributeList(attributes);
        }
    }
    if (image != INVALID_HANDLE_VALUE) CloseHandle(image);
    if (attributes != NULL) HeapFree(GetProcessHeap(), 0, attributes);

    if (!write_exact(output, &result, sizeof(result))) hold_until_supervisor_terminates();
    if (!read_exact(input, &ack, sizeof(ack)) || ack.magic != CP312_MAGIC ||
        ack.version != CP312_VERSION ||
        ack.child_process_handle != result.child_process_handle ||
        memcmp(ack.nonce, result.nonce, sizeof(ack.nonce)) != 0)
        hold_until_supervisor_terminates();
    if (child.hThread != NULL) CloseHandle(child.hThread);
    if (child.hProcess != NULL) CloseHandle(child.hProcess);
    CloseHandle(child_job);
    return 0;
}
