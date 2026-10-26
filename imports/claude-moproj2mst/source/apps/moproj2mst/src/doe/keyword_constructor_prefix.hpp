#pragma once

#include <vector>

#include "m2m/qt4.hpp"

namespace m2m::doe {
struct Study;
struct HeaderVariableRecord;
struct KeywordConstructorRecord;

// Explicit already-decoded values for 816b40/9206e0, not parser/registry receipts.
struct KeywordConstructorPrefixInput {
    int keywordId;
    qt::String parameterName;
    int unitType;
    int dimension;
    double nominalValue;
    std::vector<int> operations;
    std::vector<int> entities;
};
struct KeywordConstructorPrefixResult {
    const KeywordConstructorRecord* record; // Borrowed; actual Header/Item owns it.
    static constexpr bool completeForReader = false;
    static constexpr bool completeForPublicSave = false;
};

// Normal value prefix on an actual coherent Study/Header owner with an empty
// canonical Item list. Store dimension, construct/append, then union operations.
// No native allocation-failure parity, notifier, later text setters or conversion.
KeywordConstructorPrefixResult appendFreshKeywordConstructorPrefix(
    Study&, HeaderVariableRecord&, const KeywordConstructorPrefixInput&);
} // namespace m2m::doe
