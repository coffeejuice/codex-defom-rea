#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace m2m::doe { class SlicePlaneHeap; }

namespace m2m::engine {
struct DbHandle;
struct GuiModel;
class ConversionSession;

// c79390's two QObject connections. The actual session owns the receiver;
// weak sender identities do not prolong native child ownership.
struct ModelSignalConnection {
    std::weak_ptr<DbHandle> sender;
    std::string signal, slot;
    int rawConnectionType = 3;
    ConversionSession* receiver = nullptr; // native manager+10 mainwindow projection
};

// SfModelManager/SfMoModelManager list/current projection (c781d0/045420).
// The session owner represents mainwindow+50. DbHandle is a projection of the
// SAME native ModelBase/MoModel child, not another object nested in that child.
// QObject parent/receiver registration and arbitrary external shared leases do
// not have native QObject lifetime equivalence through this adapter.
class ModelManager {
public:
    explicit ModelManager(ConversionSession* owner = nullptr);
    ~ModelManager(); //045bf0->045460->c78250->c782e0->c78300
    ModelManager(const ModelManager&) = delete;
    ModelManager& operator=(const ModelManager&) = delete;

    std::size_t size() const noexcept { return models_.size(); }
    DbHandle* modelAt(int ordinal) const noexcept; //c79100
    DbHandle* modelWithId(std::int32_t id) const noexcept; //c78ee0
    DbHandle* currentModel() const noexcept; //c78ad0: live AND list member
    std::int32_t currentModelId() const noexcept; //c78b60/c79cd0; absent=-1
    ConversionSession* ownerSession() const noexcept { return owner_; } //native manager+10 mainwindow
    bool selectModel(DbHandle* model) noexcept; //c78bb0; absent retains current
    bool selectOrdinal(int ordinal) noexcept; //045730
    std::int32_t nextModelId() const; //c79470: first unused positive ID

    // c79390 AFTER successful native construction/path assignment and BEFORE
    // its two QObject::connect registrations. This is a prefix, not the entire
    // native factory. Null allocation/duplicate caller input is unrepresented
    // seam misuse and guards; no original safe failure return is invented.
    // Kind and path are supplied by that actual preceding constructor path.
    std::int32_t appendCreatedModel(std::shared_ptr<DbHandle> model);
    void registerModelSignals(const std::shared_ptr<DbHandle>& model);
    const auto& connections() const noexcept { return connections_; }
    // Explicit future emit boundary. Registration alone does not implement
    // mainwindow receiver effects or type3 dispatch; never silently drop emits.
    void emitModelSignal(DbHandle& sender, const std::string& signal, GuiModel* argument = nullptr);

    // c78300 encounter order: release each manager-owned child, clear list,
    // then clear current. External adapter leases can extend a child's life;
    // this does not claim native deletion/Qt teardown equivalence for them.
    void clear() noexcept;

private:
    ConversionSession* owner_; // nonowning; adapter leases do not establish QObject lifetime parity
    std::weak_ptr<doe::SlicePlaneHeap> slicePlaneHeap_; // never dereference expired session to bind a child
    std::vector<std::shared_ptr<DbHandle>> models_; //native owning QList+18
    std::weak_ptr<DbHandle> current_;              //native QPointer+20
    std::vector<ModelSignalConnection> connections_;
};

// 01d440 ->01d510 ->045780 ->current Mo child ->Project ->active subproject.
// Return the actual owner identity. Missing lazy Project/type4 construction is
// guarded at its encounter; null current/cast/subproject follows native null.
std::shared_ptr<DbHandle> currentSubProjectDbOwner(ModelManager& manager);
} // namespace m2m::engine
