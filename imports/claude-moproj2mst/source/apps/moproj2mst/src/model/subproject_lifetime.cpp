#include "m2m/model.hpp"


#include "../doe/flow_receiver_witness.hpp"

namespace m2m {

SubProject::~SubProject() noexcept
{
    if (flowReceiverWitness_)
        if (const auto registry = flowReceiverRegistry_.lock())
            registry->recordReceiverDeath(flowReceiverWitness_);
    // Registry release invalidates escaped witnesses if its owner expired first.
}

} // namespace m2m
