#include "m2m/doe_xml_output_device.hpp"

#include <utility>

#include "m2m/common.hpp"
#include "m2m/mst/file_device.hpp"

namespace m2m::doe {
namespace {
class NativeBinary final : public StudyXmlFile {
public:
    explicit NativeBinary(const qt::String& path) : file_(path)
    { file_.setAboutToClose([this] { notifyAboutToClose(); }); }
    ~NativeBinary() override { close(); file_.setAboutToClose({}); }
    std::int64_t write(std::string_view b) override { return file_.write(b); }
    bool flush() override { return file_.flush(); }
    void close() override { file_.close(); }
    bool opened() const override { return file_.opened(); }
    int error() const override { return file_.error(); }
private:
    mst::FileDevice file_;
};
class RecordingBinary final : public StudyXmlFile {
public:
    explicit RecordingBinary(std::function<void(std::string_view)> append) : append_(std::move(append)) {}
    ~RecordingBinary() override { close(); }
    std::int64_t write(std::string_view b) override
    {
        if (!opened_) return -1;
        append_(b);
        return static_cast<std::int64_t>(b.size());
    }
    bool flush() override { return opened_; }
    void close() override
    {
        if (!opened_) return;
        notifyAboutToClose();
        opened_ = false;
    }
    bool opened() const override { return opened_; }
    int error() const override { return 0; } // Explicit simulation, never native success evidence.
private:
    std::function<void(std::string_view)> append_;
    bool opened_ = true;
};
class ForwardingFile : public StudyXmlFile {
public:
    explicit ForwardingFile(std::unique_ptr<StudyXmlFile> file) : file_(std::move(file))
    {
        if (!file_) throw Error(M2M_E_UNSUPPORTED, "DOE XML device owner is unavailable");
        file_->setAboutToClose([this] { notifyAboutToClose(); });
    }
    ~ForwardingFile() override { file_->setAboutToClose({}); }
    std::int64_t write(std::string_view b) override { return file_->write(b); }
    bool flush() override { return file_->flush(); }
    void close() override { file_->close(); }
    bool opened() const override { return file_->opened(); }
    int error() const override { return file_->error(); }
protected:
    std::unique_ptr<StudyXmlFile> file_;
};
class TextFile final : public ForwardingFile {
public:
    using ForwardingFile::ForwardingFile;
    ~TextFile() override { close(); }
    std::int64_t write(std::string_view b) override { return mst::writeTextMode(*file_, b); }
};
class ObservedFile final : public ForwardingFile {
public:
    ObservedFile(std::unique_ptr<StudyXmlFile> file, StudyXmlObserver observer)
        : ForwardingFile(std::move(file)), observer_(std::move(observer)) {}
    ~ObservedFile() override { close(); }
    std::int64_t write(std::string_view b) override
    {
        const auto result = file_->write(b);
        if (observer_.write) observer_.write(b, result);
        return result;
    }
    bool flush() override
    {
        const bool result = file_->flush();
        if (observer_.flush) observer_.flush(result);
        return result;
    }
    void close() override
    {
        if (!opened()) return;
        if (observer_.closing) observer_.closing();
        file_->close();
        if (observer_.closed) observer_.closed();
    }
    int error() const override
    {
        const int result = file_->error();
        if (observer_.error) observer_.error(result);
        return result;
    }
private:
    StudyXmlObserver observer_;
};
} // namespace

std::unique_ptr<StudyXmlFile> textModeStudyXmlFile(std::unique_ptr<StudyXmlFile> file)
{ return std::make_unique<TextFile>(std::move(file)); }
std::unique_ptr<StudyXmlFile> observeStudyXmlFile(std::unique_ptr<StudyXmlFile> file, StudyXmlObserver observer)
{ return std::make_unique<ObservedFile>(std::move(file), std::move(observer)); }
std::unique_ptr<StudyXmlFile> nativeStudyXmlFile(const qt::String& path)
{ return textModeStudyXmlFile(std::make_unique<NativeBinary>(path)); }
std::unique_ptr<StudyXmlFile> recordingStudyXmlFile(std::function<void(std::string_view)> append)
{ return textModeStudyXmlFile(std::make_unique<RecordingBinary>(std::move(append))); }

} // namespace m2m::doe
