#include "m2m/mst/doe_model.hpp"

#include <utility>

#include "m2m/common.hpp"
#include "keyword_constructor_record.hpp"

namespace m2m::doe {

Item::Item() : storage_(std::in_place_type<LegacyItemText>) {}
Item::Item(String before, String after)
    : storage_(LegacyItemText{std::move(before), std::move(after)}) {}
Item::~Item() = default;
Item::Item(Item&&) noexcept = default;
Item& Item::operator=(Item&&) noexcept = default;

Item Item::ownKeyword(std::unique_ptr<KeywordConstructorRecord> record)
{
    Item item;
    item.storage_.emplace<std::unique_ptr<KeywordConstructorRecord>>(std::move(record));
    return item;
}

ItemKind Item::kind() const noexcept
{
    return std::holds_alternative<LegacyItemText>(storage_)
        ? ItemKind::LegacyValue : ItemKind::NativeKeyword;
}
LegacyItemText* Item::legacyText() noexcept
{ return std::get_if<LegacyItemText>(&storage_); }
const LegacyItemText* Item::legacyText() const noexcept
{ return std::get_if<LegacyItemText>(&storage_); }
KeywordConstructorRecord* Item::keyword() noexcept
{
    const auto* owner = std::get_if<std::unique_ptr<KeywordConstructorRecord>>(&storage_);
    return owner ? owner->get() : nullptr;
}
const KeywordConstructorRecord* Item::keyword() const noexcept
{
    const auto* owner = std::get_if<std::unique_ptr<KeywordConstructorRecord>>(&storage_);
    return owner ? owner->get() : nullptr;
}

ItemSnapshot snapshotItems(const Var& variable)
{
    ItemSnapshot snapshot;
    snapshot.reserve(variable.items.size());
    for (const auto& item : variable.items) {
        if (const auto* legacy = item.legacyText()) snapshot.emplace_back(*legacy);
        else snapshot.emplace_back(item.keyword()); // Borrowed, including an explicit null.
    }
    return snapshot;
}

LegacyItemText itemSnapshotText(const ItemSnapshotEntry& entry)
{
    if (const auto* legacy = std::get_if<LegacyItemText>(&entry)) return *legacy;
    const auto* record = std::get<const KeywordConstructorRecord*>(entry);
    if (!record)
        throw Error(M2M_E_UNSUPPORTED,
            "DOE Item snapshot: native-null keyword emitter behavior remains unsupported");
    // Read at consumption, not snapshot creation. The actual Item owner must
    // remain live; the snapshot deliberately does not extend that lifetime.
    return {record->upperText, record->lowerText};
}

} // namespace m2m::doe
