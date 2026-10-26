#pragma once

#include <cstddef>
#include <utility>
#include <vector>

#include "m2m/qt4.hpp"

namespace m2m::doe {

// Normal value prefix of 0x1409206e0. This is not a native-layout object or a
// QString/QList ref owner. Only constructor-null data slots are representable;
// scalar nominal-text continuation is bounded to the exact empty-operation
// TLOC reader profile; other later reader setters and notification remain guarded.
struct KeywordConstructorRecord final {
    KeywordConstructorRecord(int id, qt::String name, int unit, double nominal,
                             std::vector<int> operationIds, std::vector<int> entityIds)
        : parameterName(std::move(name)), unitType(unit), keywordId(id),
          nominalValue(nominal), entities(std::move(entityIds)), operations(std::move(operationIds))
    {}
    KeywordConstructorRecord(const KeywordConstructorRecord&) = delete;
    KeywordConstructorRecord& operator=(const KeywordConstructorRecord&) = delete;
    KeywordConstructorRecord(KeywordConstructorRecord&&) = default;
    KeywordConstructorRecord& operator=(KeywordConstructorRecord&&) = default;

    qt::String parameterName;                 // +00, supplied constructor value
    qt::String text08;                        // +08, default QString; meaning not inferred
    qt::String nominalText, upperText, lowerText; // +10/+18/+20, default QString
    int unitType;                            // +28
    int keywordId;                           // +2c
    bool function = false, enumeration = false; // +31/+32
    bool lowerDefined = false, upperDefined = false; // +33/+34
    double lowerValue = 0.0, upperValue = 0.0; // +38/+40
    double nominalValue;                     // +48, supplied constructor value
    std::vector<int> entities, operations;    // +50/+60, supplied values
    int parameter = -1;                      // +80, distinct from Save's +84 selector
    int saveSelector = -1;                   // +84
    std::nullptr_t lowerData = nullptr;      // +88: no nonnull data owner admitted
    std::nullptr_t upperData = nullptr;      // +90
    std::nullptr_t nominalData = nullptr;    // +98
    bool lowerDataDefined = false, upperDataDefined = false; // +a0/+a1
};

} // namespace m2m::doe
