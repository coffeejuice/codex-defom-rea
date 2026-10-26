// ctest-labels: m2m-p04
#include <testkit/check.hpp>
#include "m2m/mst/stream.hpp"

using namespace m2m;
namespace {
struct Device final : mst::OutputDevice {
    int writes = 0, flushes = 0, closes = 0;
    std::int64_t count = -2; // -2 means accept whole input
    bool flushResult = true;
    std::string received;
    std::int64_t write(std::string_view bytes) override
    {
        ++writes;
        const auto result = count == -2 ? static_cast<std::int64_t>(bytes.size()) : count;
        if (result > 0) received.append(bytes.substr(0, static_cast<size_t>(result)));
        return result;
    }
    bool flush() override { ++flushes; return flushResult; }
    void close() override { notifyAboutToClose(); ++closes; }
    bool opened() const override { return closes == 0; }
};
}

TEST_CASE(device_stream_flush_boundaries_and_close_notification_tail)
{
    Device device;
    {
        mst::Stream stream(1252, &device);
        stream << "one";
        CHECK(device.writes == 0);
        stream << mst::endl;
        CHECK(device.received == "one\n" && device.flushes == 1);
        stream << "unflushed";
        CHECK(!stream.writeFailed());
        device.close();
    }
    CHECK(device.received == "one\nunflushed" && device.closes == 1);
}

TEST_CASE(device_stream_short_write_sets_sticky_status_without_retry)
{
    Device device;
    device.count = 2;
    mst::Stream stream(1252, &device);
    stream << "first" << mst::endl;
    CHECK(stream.writeFailed() && device.received == "fi");
    CHECK(device.writes == 1 && device.flushes == 1);
    stream << "later" << mst::endl;
    CHECK(device.writes == 1 && device.flushes == 1);
    CHECK(stream.pending() == u"later\n");
}

TEST_CASE(device_stream_nonpositive_write_skips_device_flush)
{
    for (auto count : {0, -1}) {
        Device device;
        device.count = count;
        mst::Stream stream(1252, &device);
        stream << "line" << mst::endl;
        CHECK(stream.writeFailed() && device.flushes == 0);
        CHECK(stream.pending().empty());
        stream << "later" << mst::endl;
        CHECK(device.writes == 1);
    }
}

TEST_CASE(device_stream_flush_failure_suppresses_subsequent_text)
{
    Device device;
    device.flushResult = false;
    mst::Stream stream(1252, &device);
    stream << "first" << mst::endl;
    CHECK(stream.writeFailed());
    device.flushResult = true;
    stream << "second" << mst::endl;
    CHECK(device.received == "first\n" && device.flushes == 1);
    device.close(); // device may independently retry its own bytes; stream does not
    CHECK(device.closes == 1);
}

TEST_CASE(device_stream_destructor_flushes_without_closing_external_device)
{
    Device device;
    {
        mst::Stream stream(1252, &device);
        stream << "destructor tail";
        CHECK(device.writes == 0);
    }
    CHECK(device.received == "destructor tail");
    CHECK(device.flushes == 1 && device.closes == 0);
    device.close(); // callback has been disconnected; no dangling Stream receiver
    CHECK(device.writes == 1 && device.closes == 1);
}

int main() { return testkit::run_all(); }
