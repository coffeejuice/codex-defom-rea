#pragma once

#include <optional>

#include "type2_save_node_prefix.hpp"

namespace m2m::doe {

struct Type2InputTableXmlResult {
    qt::String path;
    bool opened = false;
    bool writerSucceeded = false;
    bool streamFailed = false; // Diagnostic only; not the writer's result.
    std::optional<int> fileErrorAfterClose;
    const char* guardMessage() const noexcept
    { return "Type2 study Save: signalDoeTableRefresh() receiver delivery remains unsupported"; }
};

// Continues only the actual owned, empty-variable node prefix. Immediate XML
// open/truncate precedes DOM serialization; close precedes QFile.error query.
// Ordinary false still returns at the refresh boundary. No delivery/PAR/main
// file continuation is enabled. Locale codepage0 is Windows CP_ACP; explicit
// alternatives describe an externally qualified codec profile, not MST settings.
Type2InputTableXmlResult writeType2InputTableXmlPrefix(engine::ConversionSession&,
    Project&, SubProject&, const Type2SaveNodePrefixResult&, unsigned localeCodepage = 0);

} // namespace m2m::doe
