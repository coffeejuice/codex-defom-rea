#include "type3_sub_construction.hpp"

namespace m2m::mst::detail {

Type3SubConstructionReceipt constructType3SubOwners(
    Type3SubIdentity sub, Type3ModelIdentity initializedModel,
    const Type3SubConstructionHooks& hooks)
{
    Type3SubConstructionReceipt receipt;
    auto managerAllocation = hooks.allocateManager();
    if (managerAllocation && managerAllocation->value == 0) managerAllocation.reset();
    receipt.completed.push_back({Type3SubConstructionEventKind::ManagerAllocated,
                                 managerAllocation ? managerAllocation->value : 0});
    if (managerAllocation) {
        const auto manager = *managerAllocation;
        if (!hooks.managerConstructorPrefix(manager, sub, initializedModel)) {
            receipt.completed.push_back({Type3SubConstructionEventKind::InFlightAllocationReleased,
                                         manager.value});
            return receipt;
        }
        receipt.completed.push_back({Type3SubConstructionEventKind::ManagerConstructorPrefixCompleted,
                                     manager.value});
        for (std::size_t i = 0; i < hooks.directoryAttempts.size(); ++i) {
            const bool result = hooks.directoryAttempts[i]();
            receipt.completed.push_back({Type3SubConstructionEventKind::DirectoryAttempt,
                                         0, i, result});
            if (!result) break;
        }
        if (!hooks.constructManager(manager, sub, initializedModel)) {
            receipt.completed.push_back({Type3SubConstructionEventKind::InFlightAllocationReleased,
                                         manager.value});
            return receipt;
        }
        receipt.completed.push_back({Type3SubConstructionEventKind::ManagerConstructed,
                                     manager.value});
        receipt.manager = manager;  // publication occurs only after return
    }
    receipt.completed.push_back({Type3SubConstructionEventKind::ManagerPublished,
                                 receipt.manager ? receipt.manager->value : 0});

    auto optimizerAllocation = hooks.allocateOptimizer();
    if (optimizerAllocation && optimizerAllocation->value == 0) optimizerAllocation.reset();
    receipt.completed.push_back({Type3SubConstructionEventKind::OptimizerAllocated,
                                 optimizerAllocation ? optimizerAllocation->value : 0});
    if (optimizerAllocation) {
        const auto optimizer = *optimizerAllocation;
        if (!hooks.constructOptimizer(optimizer)) {
            receipt.completed.push_back({Type3SubConstructionEventKind::InFlightAllocationReleased,
                                         optimizer.value});
            return receipt;  // already-published manager remains visible
        }
        receipt.completed.push_back({Type3SubConstructionEventKind::OptimizerConstructed,
                                     optimizer.value});
        receipt.optimizer = optimizer;
    }
    receipt.completed.push_back({Type3SubConstructionEventKind::OptimizerPublished,
                                 receipt.optimizer ? receipt.optimizer->value : 0});
    receipt.completedNormally = true;
    return receipt;
}

}  // namespace m2m::mst::detail
