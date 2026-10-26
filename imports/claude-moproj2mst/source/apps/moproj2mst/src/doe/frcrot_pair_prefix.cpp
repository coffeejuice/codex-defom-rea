#include "frcrot_pair_prefix.hpp"

#include <algorithm>

namespace m2m::doe {
namespace {
bool contains(const std::vector<FrcrotPairIdentity>& pairs, FrcrotPairIdentity identity)
{
    return std::find(pairs.begin(), pairs.end(), identity) != pairs.end();
}
}

FrcrotPairPrefixResult applyFrcrotPairPrefix(const FrcrotPairPrefixInput& input)
{
    FrcrotPairPrefixResult result;
    result.pairs = input.existingPairs;

    const FrcrotPairIdentity forward{input.master, input.slave};
    if (contains(result.pairs, forward)) {
        result.events.push_back(FrcrotPairEvent::ForwardFound);
    } else {
        result.pairs.push_back(forward);
        result.events.push_back(FrcrotPairEvent::ForwardAppended);
    }

    if (!input.modeLineSucceeded) {
        result.events.push_back(FrcrotPairEvent::ModeLineFailed);
        result.status = FrcrotPairPrefixStatus::ModeLineFailure;
        return result;
    }

    result.events.push_back(FrcrotPairEvent::ModeLineAccepted);
    result.forwardTable.resetReached = true;
    result.events.push_back(FrcrotPairEvent::TableResetReached);
    if (input.mode == 0) {
        result.forwardTable.dimensionWrite = 0;
        result.forwardTable.constantWrite = input.constantValue;
        result.events.push_back(FrcrotPairEvent::DimensionWritten);
        result.events.push_back(FrcrotPairEvent::ValueWritten);
    } else if (input.mode == 1 || input.mode == 2) {
        result.forwardTable.dimensionWrite = 1;
        result.forwardTable.codeWrite = input.mode == 1 ? 1 : 4;
        result.forwardTable.integerValueWrite = input.integerValue;
        result.events.push_back(FrcrotPairEvent::DimensionWritten);
        result.events.push_back(FrcrotPairEvent::CodeWritten);
        result.events.push_back(FrcrotPairEvent::ValueWritten);
    }

    result.events.push_back(FrcrotPairEvent::TableReaderCalled);
    const bool tableFailure = !input.tableReadSucceeded;

    if (input.master == input.slave) {
        result.events.push_back(FrcrotPairEvent::SelfPairSkippedReverse);
    } else {
        const FrcrotPairIdentity reverse{input.slave, input.master};
        if (contains(result.pairs, reverse)) {
            result.events.push_back(FrcrotPairEvent::ReverseFound);
        } else {
            result.pairs.push_back(reverse);
            result.events.push_back(FrcrotPairEvent::ReverseAppended);
        }
        result.events.push_back(FrcrotPairEvent::ReverseDirectionSet);
        result.events.push_back(FrcrotPairEvent::MirrorCopyReached);
    }

    result.status = tableFailure ? FrcrotPairPrefixStatus::TableReadFailure
                                 : FrcrotPairPrefixStatus::TableReadSuccess;
    return result;
}
} // namespace m2m::doe
