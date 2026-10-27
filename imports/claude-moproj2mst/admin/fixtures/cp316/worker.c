/* CP316 WORKER-only fixture. No child-launch mode exists. Native use is HOLD. */
#define _WIN32_WINNT 0x0A00
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdint.h>
#include <string.h>

#define CP312_MAGIC 0x32313343u
#define CP312_VERSION 2u
#define MAX_REMAINING_MS 5000u

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
_Static_assert(sizeof(CP312_INIT) == 48, "INIT layout");
_Static_assert(sizeof(CP312_RESULT) == 80, "RESULT layout");
_Static_assert(sizeof(CP312_ACK) == 48, "ACK layout");

/* Static lifetime: cancellation is requested, never assumed complete. */
static OVERLAPPED operation;
static CP312_INIT init;
static CP312_RESULT result;
static CP312_ACK ack;

__declspec(noreturn) static void hold_until_supervisor_terminates(void)
{
    for (;;) Sleep(INFINITE);
}

static DWORD remaining_ms(ULONGLONG deadline)
{
    ULONGLONG now = GetTickCount64();
    if (now >= deadline || deadline - now > MAX_REMAINING_MS)
        hold_until_supervisor_terminates();
    return (DWORD)(deadline - now);
}

/* The supervisor computes this absolute tick deadline BEFORE process creation. */
static ULONGLONG parse_deadline(int argc, WCHAR **argv)
{
    ULONGLONG value = 0;
    unsigned int i;
    if (argc != 2 || argv[1][0] == L'\0') hold_until_supervisor_terminates();
    for (i = 0; argv[1][i] != L'\0'; ++i) {
        unsigned int digit;
        if (i >= 20 || argv[1][i] < L'0' || argv[1][i] > L'9')
            hold_until_supervisor_terminates();
        digit = (unsigned int)(argv[1][i] - L'0');
        if (value > (UINT64_MAX - digit) / 10u) hold_until_supervisor_terminates();
        value = value * 10u + digit;
    }
    (void)remaining_ms(value);
    return value;
}

static int byte_pipe_client(HANDLE pipe)
{
    DWORD flags = 0;
    return pipe != NULL && pipe != INVALID_HANDLE_VALUE &&
        GetFileType(pipe) == FILE_TYPE_PIPE &&
        GetNamedPipeInfo(pipe, &flags, NULL, NULL, NULL) &&
        !(flags & (PIPE_SERVER_END | PIPE_TYPE_MESSAGE));
}

static int safe_child_job(HANDLE job)
{
    DWORD flags = 0, returned = 0;
    BOOL member = FALSE;
    JOBOBJECT_EXTENDED_LIMIT_INFORMATION info;
    ZeroMemory(&info, sizeof(info));
    return GetHandleInformation(job, &flags) && !(flags & HANDLE_FLAG_INHERIT) &&
        QueryInformationJobObject(job, JobObjectExtendedLimitInformation,
            &info, sizeof(info), &returned) &&
        (info.BasicLimitInformation.LimitFlags & JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE) &&
        !(info.BasicLimitInformation.LimitFlags & JOB_OBJECT_LIMIT_SILENT_BREAKAWAY_OK) &&
        IsProcessInJob(GetCurrentProcess(), job, &member) && !member;
}

/* Precondition: supervisor proved both client endpoints opened OVERLAPPED.
   GetNamedPipeInfo proves pipe kind, NOT asynchronous handle mode. */
static void transfer_exact(HANDLE pipe, void *buffer, DWORD length,
                           int writing, ULONGLONG deadline, HANDLE event)
{
    unsigned char *bytes = (unsigned char *)buffer;
    DWORD done = 0;
    while (done < length) {
        DWORD count = 0, budget, error;
        BOOL completed;
        (void)remaining_ms(deadline);
        ZeroMemory(&operation, sizeof(operation));
        operation.hEvent = event;
        if (!ResetEvent(event)) hold_until_supervisor_terminates();
        (void)remaining_ms(deadline);
        completed = writing ? WriteFile(pipe, bytes + done, length - done, NULL, &operation)
                            : ReadFile(pipe, bytes + done, length - done, NULL, &operation);
        error = completed ? ERROR_SUCCESS : GetLastError();
        if (!completed) {
            if (error != ERROR_IO_PENDING) hold_until_supervisor_terminates();
            budget = remaining_ms(deadline);
            if (WaitForSingleObject(event, budget) != WAIT_OBJECT_0) {
                (void)CancelIoEx(pipe, &operation);
                hold_until_supervisor_terminates();
            }
        }
        if (!GetOverlappedResult(pipe, &operation, &count, FALSE))
            hold_until_supervisor_terminates();
        (void)remaining_ms(deadline);
        if (count == 0 || count > length - done) hold_until_supervisor_terminates();
        done += count;
    }
}

int wmain(int argc, WCHAR **argv)
{
    ULONGLONG deadline = parse_deadline(argc, argv);
    HANDLE input = GetStdHandle(STD_INPUT_HANDLE), output = GetStdHandle(STD_OUTPUT_HANDLE);
    HANDLE event, child_job;
    if (sizeof(void *) != 8 || input == output ||
        !byte_pipe_client(input) || !byte_pipe_client(output))
        hold_until_supervisor_terminates();
    event = CreateEventW(NULL, TRUE, FALSE, NULL);
    if (event == NULL) hold_until_supervisor_terminates();
    transfer_exact(input, &init, sizeof(init), 0, deadline, event);
    if (init.magic != CP312_MAGIC || init.version != CP312_VERSION || init.child_job == 0)
        hold_until_supervisor_terminates();
    child_job = (HANDLE)(uintptr_t)init.child_job;
    if (!safe_child_job(child_job)) hold_until_supervisor_terminates();
    ZeroMemory(&result, sizeof(result));
    result.magic = CP312_MAGIC;
    result.version = CP312_VERSION;
    memcpy(result.nonce, init.nonce, sizeof(init.nonce));
    transfer_exact(output, &result, sizeof(result), 1, deadline, event);
    transfer_exact(input, &ack, sizeof(ack), 0, deadline, event);
    if (ack.magic != CP312_MAGIC || ack.version != CP312_VERSION ||
        ack.child_process_handle != 0 ||
        memcmp(ack.nonce, result.nonce, sizeof(ack.nonce)) != 0)
        hold_until_supervisor_terminates();
    (void)remaining_ms(deadline);
    CloseHandle(child_job);
    CloseHandle(event);
    return 0;
}
