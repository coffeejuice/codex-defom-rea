#include "cogging_file.hpp"
#include <cstdint>
#include <fcntl.h>
#include <io.h>
#include <windows.h>

namespace m2m::datafiles::detail {

CoggingFile::CoggingFile(const qt::String& path) : CoggingFile(qt::toAscii(path)) {}

CoggingFile::CoggingFile(const std::string& bytes)
{
    // fopen0x14146fcd8 uses _SH_DENYNO40; wt flags4301 reach
    // CreateFileA0x141487c38 with GENERIC_WRITE/share3/CREATE_ALWAYS.
    // Keep the byte-path API, including first-NUL termination, instead of
    // the host UCRT's narrow-to-wide path policy.
    SECURITY_ATTRIBUTES security{sizeof(SECURITY_ATTRIBUTES), nullptr, TRUE};
    HANDLE handle = CreateFileA(bytes.c_str(), GENERIC_WRITE,
                                FILE_SHARE_READ | FILE_SHARE_WRITE, &security,
                                CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (handle == INVALID_HANDLE_VALUE) return;
    const int fd = _open_osfhandle(reinterpret_cast<std::intptr_t>(handle), _O_WRONLY | _O_TEXT);
    if (fd < 0) { CloseHandle(handle); return; }
    file_ = _fdopen(fd, "wt");
    if (!file_) { _close(fd); return; }
    // _getbuf0x14148a7c0 normally allocates4096. Pin this instead of the
    // host CRT default. Its allocation-failure two-byte fallback, and host
    // FILE/descriptor exhaustion before/after open, remain qualified.
    (void)std::setvbuf(file_, buffer_.data(), _IOFBF, buffer_.size());
}

CoggingFile::~CoggingFile() { close(); }

void CoggingFile::close()
{
    if (!file_) return;
    (void)std::fclose(file_); // 0x140761c46: failed flush/close result is ignored
    file_ = nullptr;
}

} // namespace m2m::datafiles::detail
