// ctest-labels: m2m-p04
#include <testkit/check.hpp>
#include <vector>
#include "m2m/mst/file_device.hpp"

using namespace m2m;
namespace {
struct Device final : mst::OutputDevice {
    std::vector<std::int64_t> results;
    std::vector<std::string> calls;
    std::string accepted;
    bool isOpen = true;
    std::int64_t write(std::string_view bytes) override
    {
        const auto index = calls.size();
        calls.emplace_back(bytes);
        const auto result = index < results.size() ? results[index] : static_cast<std::int64_t>(bytes.size());
        if (result > 0) accepted.append(bytes.substr(0, static_cast<std::size_t>(result)));
        return result;
    }
    bool flush() override { return true; }
    void close() override { isOpen = false; }
    bool opened() const override { return isOpen; }
};
}

TEST_CASE(text_device_preserves_carriage_returns_and_splits_each_newline)
{
    Device d;
    CHECK_EQ(mst::writeTextMode(d, "a\r\n\nb"), 5);
    CHECK(d.calls == std::vector<std::string>({"a\r", "\r\n", "\r\n", "b"}));
    CHECK(d.accepted == "a\r\r\n\r\nb");
}

TEST_CASE(text_device_positive_short_span_skips_remaining_span_then_continues)
{
    Device d;
    d.results = {2};
    CHECK_EQ(mst::writeTextMode(d, "abcd\ntail"), 7);
    CHECK(d.accepted == "ab\r\ntail");
    CHECK(d.calls == std::vector<std::string>({"abcd", "\r\n", "tail"}));
}

TEST_CASE(text_device_short_crlf_counts_one_source_byte_and_continues)
{
    Device d;
    d.results = {1};
    CHECK_EQ(mst::writeTextMode(d, "\na"), 2);
    CHECK(d.accepted == "\ra");
}

TEST_CASE(text_device_failure_returns_prior_source_progress_and_stops)
{
    for (auto result : {0, -1}) {
        Device first;
        first.results = {result};
        CHECK_EQ(mst::writeTextMode(first, "abc\nlater"), result);
        CHECK_EQ(first.calls.size(), 1u);
        Device newline;
        newline.results = {3, result};
        CHECK_EQ(mst::writeTextMode(newline, "abc\nlater"), 3);
        CHECK(newline.accepted == "abc");
        CHECK_EQ(newline.calls.size(), 2u);
    }
}

TEST_CASE(text_device_empty_open_write_has_no_call_and_closed_returns_error)
{
    Device d;
    CHECK_EQ(mst::writeTextMode(d, ""), 0);
    CHECK(d.calls.empty());
    d.close();
    CHECK_EQ(mst::writeTextMode(d, ""), -1);
    CHECK_EQ(mst::writeTextMode(d, "lost"), -1);
    CHECK(d.calls.empty());
}

int main() { return testkit::run_all(); }
