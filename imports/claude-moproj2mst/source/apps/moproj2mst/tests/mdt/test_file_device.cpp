#include "../support/test_paths.hpp"
// ctest-labels: m2m-mdt
#include <testkit/check.hpp>
#include <process.h>
#include <filesystem>
#include <fstream>
#include <string>
#include "m2m/mst/file_device.hpp"

namespace fs = std::filesystem;
using m2m::mst::FileDevice;
using m2m::qt::String;

namespace {
fs::path freshDir()
{
    static unsigned serial = 0;
    const auto path = m2m_test::scratchOutput(m2m_test::scratchRoot() /
        ("file_device_" + std::to_string(_getpid())) / std::to_string(++serial));
    fs::create_directories(path);
    return path;
}

std::string readFile(const fs::path& path)
{
    std::ifstream file(path, std::ios::binary);
    return {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
}
}

TEST_CASE(file_device_open_truncates_and_small_writes_wait_for_flush)
{
    const auto path = freshDir() / "device.txt";
    { std::ofstream seed(path, std::ios::binary); seed << "previous longer contents"; }
    FileDevice file{String(path.u16string())};
    REQUIRE(file.opened());
    CHECK_EQ(file.error(), 0);
    CHECK_EQ(fs::file_size(path), 0u);                     // truncate on successful open
    CHECK_EQ(file.write("abc"), 3);
    CHECK_EQ(fs::file_size(path), 0u);                     // QFile buffer, not a raw WriteFile
    CHECK(file.flush());
    CHECK(readFile(path) == "abc");
    CHECK_EQ(file.write("tail"), 4);
    CHECK_EQ(fs::file_size(path), 3u);
    file.close();                                        // independent QFile close flush
    CHECK(readFile(path) == "abctail");
    CHECK(!file.opened());
    file.close();                                        // no second physical close/flush
    CHECK_EQ(file.write("lost"), -1);
    CHECK(!file.flush());
}

TEST_CASE(file_device_text_mode_direct_write_and_state_lifecycle)
{
    const auto path = freshDir() / "text-direct.txt";
    FileDevice binary{String(path.u16string())};
    REQUIRE(binary.opened());
    CHECK(!binary.isTextModeEnabled());
    binary.setTextModeEnabled(true);
    CHECK(binary.isTextModeEnabled());
    CHECK_EQ(binary.write("first\nsecond"), 12); // return source-byte count
    CHECK_EQ(fs::file_size(path), 0u);             // same QFile buffering path
    binary.setTextModeEnabled(false);
    CHECK(!binary.isTextModeEnabled());
    CHECK_EQ(binary.write("\n"), 1);
    binary.close();
    CHECK(!binary.isTextModeEnabled());
    CHECK(readFile(path) == "first\r\nsecond\n");
    binary.setTextModeEnabled(true);               // closed QIODevice setter is a no-op
    CHECK(!binary.isTextModeEnabled());

    const auto optPath = freshDir() / "text-opt-in.txt";
    FileDevice text{String(optPath.u16string()), true};
    REQUIRE(text.opened());
    CHECK(text.isTextModeEnabled());
    CHECK_EQ(text.write("a\nb\n"), 4);
    text.close();
    CHECK(readFile(optPath) == "a\r\nb\r\n");
}

TEST_CASE(file_device_opt_in_stream_produces_one_crlf_and_close_flushes_tail)
{
    const auto path = freshDir() / "stream-text.txt";
    FileDevice file{String(path.u16string()), true};
    REQUIRE(file.opened());
    m2m::mst::Stream stream(1252, &file);
    stream << "line" << m2m::mst::endl << "tail\nlast";
    CHECK(file.isTextModeEnabled());               // Stream restores Text after flush
    file.close();                                  // callback emits unterminated tail
    CHECK(readFile(path) == "line\r\ntail\r\nlast");
    CHECK(stream.pending().empty());
    CHECK(!file.isTextModeEnabled());
}

TEST_CASE(file_device_buffer_boundary_and_large_direct_write)
{
    const auto path = freshDir() / "boundary.txt";
    FileDevice file{String(path.u16string())};
    REQUIRE(file.opened());
    const std::string block(0x4000, 'a');
    CHECK_EQ(file.write(block), static_cast<std::int64_t>(block.size()));
    CHECK_EQ(fs::file_size(path), 0u);                     // equality still buffers
    CHECK_EQ(file.write("b"), 1);                        // overflow flushes earlier bytes
    CHECK_EQ(fs::file_size(path), block.size());
    const std::string large(0x4001, 'c');
    CHECK_EQ(file.write(large), static_cast<std::int64_t>(large.size()));
    CHECK_EQ(fs::file_size(path), block.size() + 1 + large.size());
    CHECK(readFile(path) == block + "b" + large);          // >16KiB bypasses QFile buffer
    CHECK(file.flush());
    file.close();
}

TEST_CASE(file_device_text_newline_flushes_prior_span_at_buffer_boundary)
{
    const auto path = freshDir() / "text.txt";
    FileDevice file{String(path.u16string())};
    REQUIRE(file.opened());
    const std::string line(0x3fff, 'a');
    CHECK_EQ(m2m::mst::writeTextMode(file, line + "\ntail"), 0x4004);
    CHECK_EQ(fs::file_size(path), line.size()); // CRLF overflow flushes preceding span only
    file.close();
    CHECK_EQ(readFile(path), line + "\r\ntail");
}

TEST_CASE(file_device_failed_open_does_not_create_parent_or_serialize)
{
    const auto root = freshDir();
    const auto missing = root / "absent" / "device.txt";
    FileDevice absent{String(missing.u16string())};
    CHECK(!absent.opened());
    CHECK_EQ(absent.error(), 5);                          // QFile OpenError
    CHECK_EQ(absent.write("ignored"), -1);
    CHECK(!absent.flush());
    CHECK(!fs::exists(missing.parent_path()));
    FileDevice directory{String(root.u16string())};
    CHECK(!directory.opened());
    CHECK(fs::is_directory(root));
    const auto path = root / "destructor.txt";
    {
        FileDevice file{String(path.u16string())};
        REQUIRE(file.opened());
        CHECK_EQ(file.write("on close"), 8);
        CHECK_EQ(fs::file_size(path), 0u);
    }
    CHECK(readFile(path) == "on close");
}

TEST_CASE(file_device_close_retries_locked_line_after_stream_failure)
{
    const auto path = freshDir() / "locked.txt";
    FileDevice file{String(path.u16string())};
    REQUIRE(file.opened());
    struct Handle {
        HANDLE value;
        ~Handle() { if (value != INVALID_HANDLE_VALUE) CloseHandle(value); }
    } lock{CreateFileW(path.c_str(), GENERIC_READ | GENERIC_WRITE,
                       FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING,
                       FILE_ATTRIBUTE_NORMAL, nullptr)};
    REQUIRE(lock.value != INVALID_HANDLE_VALUE);
    OVERLAPPED range{};
    REQUIRE(LockFileEx(lock.value, LOCKFILE_EXCLUSIVE_LOCK | LOCKFILE_FAIL_IMMEDIATELY,
                       0, 4096, 0, &range));
    m2m::mst::Stream stream(1252, &file);
    stream << "first" << m2m::mst::endl;
    CHECK(stream.writeFailed());                         // QFile retained the blocked line
    CHECK_EQ(file.error(), 2);                            // QFile WriteError, independent of Stream
    CHECK_EQ(fs::file_size(path), 0u);
    stream << "suppressed" << m2m::mst::endl;
    CHECK(stream.writeFailed());                         // no later QTextStream flush
    REQUIRE(UnlockFileEx(lock.value, 0, 4096, 0, &range));
    file.close();                                        // retry is independent of stream status
    CHECK(readFile(path) == "first\n");
    CHECK(stream.writeFailed());
    CHECK_EQ(file.error(), 0);                            // successful close retry unsets QFile error
    CHECK(stream.pending() == u"suppressed\n");
}

TEST_CASE(file_device_error_after_close_uses_saved_initial_flush_result)
{
    for (const bool flushedBeforeClose : {false, true}) {
        const auto path = freshDir() / "persistent-lock.txt";
        FileDevice file{String(path.u16string())};
        REQUIRE(file.opened());
        struct Handle {
            HANDLE value;
            ~Handle() { if (value != INVALID_HANDLE_VALUE) CloseHandle(value); }
        } lock{CreateFileW(path.c_str(), GENERIC_READ | GENERIC_WRITE,
                           FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING,
                           FILE_ATTRIBUTE_NORMAL, nullptr)};
        REQUIRE(lock.value != INVALID_HANDLE_VALUE);
        OVERLAPPED range{};
        REQUIRE(LockFileEx(lock.value, LOCKFILE_EXCLUSIVE_LOCK | LOCKFILE_FAIL_IMMEDIATELY,
                           0, 4096, 0, &range));
        m2m::mst::Stream stream(1252, &file);
        stream << "blocked";
        if (flushedBeforeClose) stream << m2m::mst::endl;
        file.close();                                    // keep lock during the close attempt
        CHECK(stream.writeFailed());
        CHECK(!file.opened());
        // A failed initial QFile byte-buffer flush preserves its error. If
        // initially empty, it succeeded BEFORE aboutToClose's text flush failed;
        // successful native close therefore clears the later QFile error.
        CHECK_EQ(file.error(), flushedBeforeClose ? 2 : 0);
        REQUIRE(UnlockFileEx(lock.value, 0, 4096, 0, &range));
        CHECK(readFile(path).empty());
    }
}

TEST_CASE(file_device_close_and_stream_destructor_flush_unterminated_text)
{
    const auto root = freshDir();
    const auto closePath = root / "close-tail.txt";
    FileDevice closeFile{String(closePath.u16string())};
    REQUIRE(closeFile.opened());
    m2m::mst::Stream tail(1252, &closeFile);
    tail << "no newline";
    CHECK_EQ(fs::file_size(closePath), 0u);
    CHECK(tail.pending() == u"no newline");
    closeFile.close();                                    // aboutToClose -> flushStream
    CHECK(readFile(closePath) == "no newline");
    CHECK(tail.pending().empty());

    const auto scopePath = root / "scope-tail.txt";
    FileDevice scopeFile{String(scopePath.u16string())};
    REQUIRE(scopeFile.opened());
    {
        m2m::mst::Stream inner(1252, &scopeFile);
        inner << "scope exit";
        CHECK_EQ(fs::file_size(scopePath), 0u);
    }                                                    // ~QTextStream flush, then disconnect
    CHECK(scopeFile.opened());                            // external QFile is not owned
    CHECK(readFile(scopePath) == "scope exit");
    scopeFile.close();
    CHECK(readFile(scopePath) == "scope exit");           // detached callback cannot run again
}

int main() { return testkit::run_all(); }
