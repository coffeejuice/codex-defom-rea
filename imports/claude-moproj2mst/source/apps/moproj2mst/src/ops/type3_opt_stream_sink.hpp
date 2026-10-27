#pragma once

#include "type3_opt_save_attempt.hpp"
#include "m2m/mst/file_device.hpp"
#include <memory>

namespace m2m::mst::detail {

// Private opt-in bridge from the prepared Type3 insertion protocol to the
// shared QTextStream/FileDevice text-mode boundary. It deliberately has no
// opaque-fragment implementation: callers must supply typed insertions.
class Type3OptStreamSink final : public Type3OptAttemptSink {
public:
    explicit Type3OptStreamSink(unsigned codepage);
    ~Type3OptStreamSink() override;
    // Path is UTF-8; conversion to the QFile-facing UTF-16 path is explicit.
    bool open(const std::string& path) override;
    bool insertLiteral(const char*) override;
    bool insertInt(std::int32_t) override;
    bool insertDouble(double) override;
    bool insertString(const qt::String&) override;
    bool insertEndl() override;
    bool insert(const std::string&) override;
    bool close() override;
    void assignOpenFailureDiagnostic(const std::string&) override;

    bool opened() const;
    const std::string& bytes() const;
    bool writeFailed() const;
    const std::string& diagnostic() const { return diagnostic_; }
private:
    unsigned codepage_;
    std::unique_ptr<FileDevice> device_;
    std::unique_ptr<Stream> stream_;
    std::string diagnostic_;
};

// Explicit private profile gate. Rejected profiles return before sink.open,
// so generic prepared fragments cannot truncate a destination through this
// bridge. The writer's historical false return is reported separately.
enum class Type3OptStreamProfile { OpaquePrepared, TypedManager };
struct Type3OptStreamRun {
    bool admitted = false;
    bool writerResult = false;
};
Type3OptStreamRun runType3OptStreamProfile(
    Type3OptStreamProfile profile, const Type3OptSavePrepared& prepared,
    const Type3OptimizerScalarState& state, doe::Model& manager,
    Type3OptStreamSink& sink, Type3OptGroupReceipt& receipt);

} // namespace m2m::mst::detail
