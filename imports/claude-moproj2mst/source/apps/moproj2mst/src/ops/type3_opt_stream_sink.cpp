#include "type3_opt_stream_sink.hpp"
#include <filesystem>

namespace m2m::mst::detail {
Type3OptStreamSink::Type3OptStreamSink(unsigned codepage) : codepage_(codepage) {}
Type3OptStreamSink::~Type3OptStreamSink() { if (device_) device_->close(); }

bool Type3OptStreamSink::open(const std::string& path)
{
    if (device_) return false;
    const auto path16 = std::filesystem::u8path(path).u16string();
    device_ = std::make_unique<FileDevice>(qt::String(path16), true);
    if (!device_->opened()) {
        diagnostic_ = path;
        device_.reset();
        return false;
    }
    stream_ = std::make_unique<Stream>(codepage_, device_.get());
    return true;
}
bool Type3OptStreamSink::insertLiteral(const char* value)
{
    if (!stream_ || !device_->opened()) return false;
    *stream_ << value;
    return !stream_->writeFailed();
}
bool Type3OptStreamSink::insertInt(std::int32_t value)
{
    if (!stream_ || !device_->opened()) return false;
    *stream_ << static_cast<int>(value);
    return !stream_->writeFailed();
}
bool Type3OptStreamSink::insertDouble(double value)
{
    if (!stream_ || !device_->opened()) return false;
    *stream_ << value;
    return !stream_->writeFailed();
}
bool Type3OptStreamSink::insertString(const qt::String& value)
{
    if (!stream_ || !device_->opened()) return false;
    *stream_ << value;
    return !stream_->writeFailed();
}
bool Type3OptStreamSink::insertEndl()
{
    if (!stream_ || !device_->opened()) return false;
    *stream_ << endl;
    return !stream_->writeFailed();
}
bool Type3OptStreamSink::insert(const std::string&)
{
    // Opaque prepared fragments have no typed/codec-qualified interpretation.
    return false;
}
bool Type3OptStreamSink::close()
{
    if (!device_) return false;
    device_->close(); // emits aboutToClose while Stream is alive; pending text flushes there
    return !device_->opened();
}
void Type3OptStreamSink::assignOpenFailureDiagnostic(const std::string& path)
{
    diagnostic_ = path;
}
bool Type3OptStreamSink::opened() const { return device_ && device_->opened(); }
const std::string& Type3OptStreamSink::bytes() const
{
    static const std::string empty;
    return stream_ ? stream_->bytes() : empty;
}
bool Type3OptStreamSink::writeFailed() const { return stream_ && stream_->writeFailed(); }

Type3OptStreamRun runType3OptStreamProfile(
    Type3OptStreamProfile profile, const Type3OptSavePrepared& prepared,
    const Type3OptimizerScalarState& state, doe::Model& manager,
    Type3OptStreamSink& sink, Type3OptGroupReceipt& receipt)
{
    if (profile != Type3OptStreamProfile::TypedManager) return {};
    return {true, attemptType3OptSaveWithTypedSections(prepared, state, manager, sink, receipt)};
}
} // namespace m2m::mst::detail
