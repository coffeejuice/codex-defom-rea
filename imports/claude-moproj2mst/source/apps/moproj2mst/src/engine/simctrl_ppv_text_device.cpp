#include "m2m/engine/simctrl_ppv_text_device.hpp"

#include "m2m/mst/file_device.hpp"

namespace m2m::engine::simctrl_ppv {
namespace {

class TextModeDevice final : public mst::OutputDevice {
public:
    explicit TextModeDevice(mst::OutputDevice& file) : file_(file) {}

    std::int64_t write(std::string_view bytes) override
    {
        return mst::writeTextMode(file_, bytes);
    }
    bool flush() override { return file_.flush(); }
    void close() override { file_.close(); }
    bool opened() const override { return file_.opened(); }

private:
    mst::OutputDevice& file_;
};

} // namespace

TextDeviceWriteResult writePpvTextSegments(mst::OutputDevice& file,
                                           unsigned localeCodepage,
                                           std::span<const std::string> lines)
{
    TextDeviceWriteResult result;
    result.openAttempted = true;
    result.opened = file.opened();
    if (!result.opened) return result;

    TextModeDevice textFile(file);
    {
        mst::Stream stream(localeCodepage, &textFile);
        // QFile::close first flushes its own ring, then emits aboutToClose;
        // the attached QTextStream flushes while QFile is still open.
        file.setAboutToClose([&stream] { stream.flush(); });
        for (const std::string& line : lines) {
            stream << qt::String::latin1(line) << mst::endl;
        }
        file.close();
        result.streamFailed = stream.writeFailed();
        file.setAboutToClose({});
    }
    result.writerReturned = true; // Original returns true after close without checking errors.
    return result;
}

TextDeviceWriteResult writePpvTextFile(const qt::String& path,
                                       unsigned localeCodepage,
                                       std::span<const std::string> lines)
{
    mst::FileDevice file(path);
    return writePpvTextSegments(file, localeCodepage, lines);
}

} // namespace m2m::engine::simctrl_ppv
