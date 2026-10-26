#pragma once

// Adapter observation of real directory allocations; no native layout fields.
// Calls belong to the serialized DB/model thread. A snapshot does not own its
// reader: consumers must first retain the actual DbHandle and match identities.
#include <cstddef>
#include <cstdint>
#include <limits>
#include <optional>
#include <unordered_map>

#include <defmodel/layout/cls_1401dbfe0.hpp>

namespace def::model::directory_receipts {

struct Receipt {
    const void* owner = nullptr;
    const Cls_1401dbfe0* reader = nullptr;
    const void* allocation = nullptr;
    std::size_t elementCount = 0;
    std::uint64_t ownerGeneration = 0;
    std::uint64_t allocationGeneration = 0;
};

namespace detail {
struct Entry {
    Receipt receipt;
    bool ready = false;
};
struct Registry {
    std::unordered_map<const Cls_1401dbfe0*, Entry> entries;
    std::uint64_t serial = 0;
    bool exhausted = false;

    std::uint64_t next() noexcept
    {
        if (exhausted || serial == std::numeric_limits<std::uint64_t>::max()) {
            exhausted = true;
            return 0; // observation becomes unavailable; never reuse a generation
        }
        return ++serial;
    }
};
inline Registry& registry() noexcept { static Registry value; return value; }
inline void clear(Entry& entry) noexcept
{
    entry.ready = false;
    entry.receipt.allocation = nullptr;
    entry.receipt.elementCount = 0;
    entry.receipt.allocationGeneration = 0;
}
} // namespace detail

// Called only from the actual DbHandle constructor, after its unique reader is
// constructed. Failure loses observation only; it cannot change native DB flow.
inline bool bindOwner(const void* owner, const Cls_1401dbfe0* reader) noexcept
{
    if (!owner || !reader) return false;
    auto& registry = detail::registry();
    if (registry.exhausted) return false;
    const auto found = registry.entries.find(reader);
    if (found != registry.entries.end()) return found->second.receipt.owner == owner;
    const auto generation = registry.next();
    if (!generation) return false;
    try {
        detail::Entry entry;
        entry.receipt.owner = owner;
        entry.receipt.reader = reader;
        entry.receipt.ownerGeneration = generation;
        return registry.entries.emplace(reader, entry).second;
    } catch (...) { return false; }
}

inline void retireOwner(const void* owner, const Cls_1401dbfe0* reader) noexcept
{
    auto& entries = detail::registry().entries;
    const auto found = entries.find(reader);
    if (found != entries.end() && found->second.receipt.owner == owner) entries.erase(found);
}

// Reader destruction also retires the binding if the unique reader was replaced
// directly. No DbHandle identity is dereferenced by this fallback.
inline void retireReader(const Cls_1401dbfe0* reader) noexcept
{
    detail::registry().entries.erase(reader);
}

inline void clearReaderAllocation(const Cls_1401dbfe0* reader) noexcept
{
    auto& entries = detail::registry().entries;
    const auto found = entries.find(reader);
    if (found != entries.end()) detail::clear(found->second);
}

// Called immediately after the real new[] result is published in reader.uad8.
// Count is the operand of that allocation, never later logical reader.uac8.
inline void allocationPublished(const Cls_1401dbfe0& reader, const void* allocation,
                                std::size_t elementCount) noexcept
{
    auto& registry = detail::registry();
    const auto found = registry.entries.find(&reader);
    if (found == registry.entries.end()) return;
    auto& entry = found->second;
    detail::clear(entry);
    if (registry.exhausted || !allocation || reader.uad8 != allocation) return;
    const auto generation = registry.next();
    if (!generation) return;
    entry.receipt.allocation = allocation;
    entry.receipt.elementCount = elementCount;
    entry.receipt.allocationGeneration = generation;
}

// Published storage is not queryable until the loader's initialization/copy
// phase returns. This records no native write and performs no allocation.
inline void allocationReady(const Cls_1401dbfe0& reader, const void* allocation) noexcept
{
    auto& registry = detail::registry();
    const auto found = registry.entries.find(&reader);
    if (found == registry.entries.end() || registry.exhausted) return;
    auto& entry = found->second;
    if (allocation && allocation == reader.uad8 && allocation == entry.receipt.allocation &&
        entry.receipt.allocationGeneration != 0) entry.ready = true;
}

// Actual delete[] may be called with an old pointer after a replacement was
// published. Retire matching allocations only; leave the new receipt intact.
inline void allocationRetired(const void* allocation) noexcept
{
    if (!allocation) return;
    for (auto& item : detail::registry().entries)
        if (item.second.receipt.allocation == allocation) detail::clear(item.second);
}

inline std::optional<Receipt> current(const void* owner, const Cls_1401dbfe0& reader) noexcept
{
    const auto& registry = detail::registry();
    const auto found = registry.entries.find(&reader);
    if (registry.exhausted || found == registry.entries.end()) return std::nullopt;
    const auto& entry = found->second;
    if (!entry.ready || entry.receipt.owner != owner || reader.uad8 != entry.receipt.allocation)
        return std::nullopt;
    return entry.receipt;
}

// Lifetime/generation recheck never dereferences a possibly retired reader.
// A live consumer must still hold the actual native owner lease.
inline bool isCurrent(const Receipt& receipt) noexcept
{
    const auto& registry = detail::registry();
    const auto found = registry.entries.find(receipt.reader);
    if (registry.exhausted || found == registry.entries.end()) return false;
    const auto& entry = found->second;
    const auto& now = entry.receipt;
    return entry.ready && now.owner == receipt.owner && now.reader == receipt.reader &&
        now.ownerGeneration == receipt.ownerGeneration &&
        now.allocationGeneration == receipt.allocationGeneration &&
        now.allocation == receipt.allocation && now.elementCount == receipt.elementCount &&
        now.reader->uad8 == now.allocation;
}

} // namespace def::model::directory_receipts
