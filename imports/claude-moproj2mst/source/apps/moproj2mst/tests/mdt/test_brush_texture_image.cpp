// ctest-labels: m2m-mdt
#include <testkit/check.hpp>
#include <algorithm>
#include <array>
#include <limits>
#include <utility>
#include "m2m/engine/brush_texture_image.hpp"

using namespace m2m::engine;
namespace {
using Stage = BrushTextureImageStage;
using Issue = BrushTextureImageIssue;
using Effect = BrushTextureImageEffect;

std::size_t count(const BrushTextureImageOpacityCall& call, Effect effect)
{
    return static_cast<std::size_t>(std::count(call.effects().begin(), call.effects().end(), effect));
}
struct Fixture {
    std::shared_ptr<BrushTextureImageState> source = std::make_shared<BrushTextureImageState>();
    std::shared_ptr<BrushTextureImageDataState> data = std::make_shared<BrushTextureImageDataState>(71);
    BrushTextureImageOpacityCall call{source};
    void image(bool nullData = false)
    {
        BrushTextureImageObservation o;
        o.pixmapSelected = false;
        o.imageData = nullData ? std::shared_ptr<BrushTextureImageDataState>{} : data;
        REQUIRE(source->observe(o));
    }
    void ordinaryCopy()
    {
        BrushTextureImageObservation o;
        o.imagePaintingActiveWord = 0;
        REQUIRE(source->observe(o));
    }
};
}

TEST_CASE(brush_texture_image_missing_source_and_selection_do_not_construct_values)
{
    BrushTextureImageOpacityCall absent{nullptr};
    CHECK(absent.resume().issue == Issue::SourceUnavailable);
    Fixture f;
    CHECK(f.call.resume().issue == Issue::SelectionUnavailable);
    CHECK(f.call.effects().empty());
    BrushTextureImageObservation o; o.pixmapSelected = false;
    REQUIRE(f.source->observe(o));
    CHECK(f.call.resume().issue == Issue::ImageDataUnavailable);
    CHECK(f.call.effects().empty());
}

TEST_CASE(brush_texture_image_pixmap_selection_stays_guarded_before_image_reads)
{
    Fixture f;
    BrushTextureImageObservation o; o.pixmapSelected = true;
    REQUIRE(f.source->observe(o));
    CHECK(f.call.resume().issue == Issue::PixmapPathUnrecovered);
    CHECK(f.call.effects().empty());
    CHECK(!f.call.report().opaque.has_value());
}

TEST_CASE(brush_texture_image_null_source_reads_nullness_once_before_cache_and_copy_guards)
{
    Fixture f; f.image(true);
    CHECK(f.call.resume().issue == Issue::PixmapCachePresenceUnavailable);
    CHECK(f.call.report().stage == Stage::ImagePixmapCache);
    CHECK(count(f.call, Effect::ImageNullnessRead) == 1);
    CHECK(f.call.resume().issue == Issue::PixmapCachePresenceUnavailable);
    CHECK(count(f.call, Effect::ImageNullnessRead) == 1);
    BrushTextureImageObservation o; o.cachedPixmapPresent = false;
    REQUIRE(f.source->observe(o));
    CHECK(f.call.resume().issue == Issue::PaintingActiveUnavailable);
    f.ordinaryCopy();
    const auto result = f.call.resume();
    CHECK(result.stage == Stage::Returned);
    CHECK(result.opaque == true);
    CHECK(count(f.call, Effect::ImageValueCopied) == 1);
    CHECK(count(f.call, Effect::ImageValueDestroyed) == 1);
    CHECK(count(f.call, Effect::ImageDataRetained) == 0);
    CHECK(count(f.call, Effect::ImageFormatRead) == 0);
    const auto effects = f.call.effects();
    CHECK(f.call.resume().opaque == true);
    CHECK(f.call.effects() == effects);
}

TEST_CASE(brush_texture_image_null_image_existing_pixmap_requires_conversion_before_copy)
{
    Fixture f; f.image(true); f.ordinaryCopy();
    BrushTextureImageObservation o; o.cachedPixmapPresent = true;
    REQUIRE(f.source->observe(o));
    CHECK(f.call.resume().issue == Issue::ImageConversionUnrecovered);
    CHECK(count(f.call, Effect::ImageNullnessRead) == 1);
    CHECK(count(f.call, Effect::ImageValueCopied) == 0);
    CHECK(!f.call.report().opaque.has_value());
}

TEST_CASE(brush_texture_image_cached_nonnull_data_skips_pixmap_cache_lookup_and_retains_copy)
{
    Fixture f; f.image();
    CHECK(f.call.resume().issue == Issue::PaintingActiveUnavailable);
    CHECK(f.data->temporaryValueReferences() == 0);
    f.ordinaryCopy();
    CHECK(f.call.resume().issue == Issue::ImageFormatUnavailable);
    CHECK(f.data->temporaryValueReferences() == 1);
    CHECK(f.call.resume().issue == Issue::ImageFormatUnavailable);
    CHECK(count(f.call, Effect::ImageValueCopied) == 1);
    CHECK(count(f.call, Effect::ImageDataRetained) == 1);
    CHECK(count(f.call, Effect::ImageNullnessRead) == 1);
}

TEST_CASE(brush_texture_image_direct_alpha_formats_do_not_read_color_table_flag)
{
    for (const std::int32_t format : {5, 6, 8, 10, 12, 15}) {
        Fixture f; f.image(); f.ordinaryCopy();
        REQUIRE(f.data->observe({format, std::nullopt}));
        const auto result = f.call.resume();
        CHECK(result.stage == Stage::Returned);
        CHECK(result.opaque == false);
        CHECK(f.data->temporaryValueReferences() == 0);
        CHECK(count(f.call, Effect::ImageColorTableAlphaRead) == 0);
        const std::vector<Effect> expected{Effect::ImageNullnessRead, Effect::ImageValueCopied,
            Effect::ImageDataRetained, Effect::ImageFormatRead, Effect::ImageDataReleased,
            Effect::ImageValueDestroyed};
        CHECK(f.call.effects() == expected);
    }
}

TEST_CASE(brush_texture_image_other_formats_use_exact_numeric_flag_decision)
{
    // Derived from original650aa340 branches, including values outside the
    // named enum range. Pixel samples and color-table entries are not inputs.
    struct Row { std::int32_t format; bool opaqueWhenFlagSet; };
    constexpr std::array rows{Row{0, true}, Row{1, false}, Row{2, false}, Row{3, false},
        Row{4, true}, Row{7, true}, Row{9, true}, Row{11, true}, Row{13, true},
        Row{14, true}, Row{16, true}, Row{17, true}, Row{18, true}, Row{-1, true},
        Row{std::numeric_limits<std::int32_t>::max(), true}};
    for (const auto& row : rows) {
        for (const bool flag : {false, true}) {
            Fixture f; f.image(); f.ordinaryCopy();
            REQUIRE(f.data->observe({row.format, flag}));
            CHECK(f.call.resume().opaque == (flag ? row.opaqueWhenFlagSet : true));
            CHECK(count(f.call, Effect::ImageColorTableAlphaRead) == 1);
            CHECK(count(f.call, Effect::ImageDataReleased) == 1);
            CHECK(f.data->temporaryValueReferences() == 0);
        }
    }
}

TEST_CASE(brush_texture_image_all_other_formats_consume_flag_even_if_result_cannot_change)
{
    for (const std::int32_t format : {1, 4, -1}) {
        Fixture f; f.image(); f.ordinaryCopy();
        REQUIRE(f.data->observe({format, std::nullopt}));
        CHECK(f.call.resume().issue == Issue::ImageColorTableAlphaUnavailable);
        CHECK(f.data->temporaryValueReferences() == 1);
        CHECK(count(f.call, Effect::ImageFormatRead) == 1);
        CHECK(f.call.resume().issue == Issue::ImageColorTableAlphaUnavailable);
        CHECK(count(f.call, Effect::ImageFormatRead) == 1);
        CHECK(count(f.call, Effect::ImageDataRetained) == 1);
        REQUIRE(f.data->observe({std::nullopt, true}));
        CHECK(f.call.resume().opaque == (format != 1));
        CHECK(count(f.call, Effect::ImageColorTableAlphaRead) == 1);
        CHECK(f.data->temporaryValueReferences() == 0);
    }
}

TEST_CASE(brush_texture_image_distinct_value_calls_share_data_and_release_their_own_refs)
{
    Fixture f; f.image(); f.ordinaryCopy();
    BrushTextureImageOpacityCall other{f.source};
    CHECK(f.call.resume().issue == Issue::ImageFormatUnavailable);
    CHECK(other.resume().issue == Issue::ImageFormatUnavailable);
    CHECK(f.data->temporaryValueReferences() == 2);
    REQUIRE(f.data->observe({5, std::nullopt}));
    CHECK(f.call.resume().opaque == false);
    CHECK(f.data->temporaryValueReferences() == 1);
    CHECK(other.resume().opaque == false);
    CHECK(f.data->temporaryValueReferences() == 0);
}

TEST_CASE(brush_texture_image_abandoned_call_releases_held_temporary_data)
{
    Fixture f; f.image(); f.ordinaryCopy();
    {
        BrushTextureImageOpacityCall abandoned{f.source};
        CHECK(abandoned.resume().issue == Issue::ImageFormatUnavailable);
        CHECK(f.data->temporaryValueReferences() == 1);
    }
    CHECK(f.data->temporaryValueReferences() == 0);
}

TEST_CASE(brush_texture_image_conflicting_metadata_observation_does_not_partially_merge)
{
    Fixture f; f.image(); f.ordinaryCopy();
    REQUIRE(f.data->observe({4, std::nullopt}));
    CHECK(!f.data->observe({5, true}));
    CHECK(f.call.resume().issue == Issue::ImageColorTableAlphaUnavailable);
    REQUIRE(f.data->observe({4, false}));
    CHECK(f.call.resume().opaque == true);
    BrushTextureImageDataState invalid{0};
    CHECK(!invalid.observe({5, false}));
}

TEST_CASE(brush_texture_image_source_observation_rejects_owner_change_before_merging_selection)
{
    Fixture f;
    BrushTextureImageObservation first; first.imageData = f.data;
    REQUIRE(f.source->observe(first));
    BrushTextureImageObservation conflict;
    conflict.imageData = std::make_shared<BrushTextureImageDataState>(72);
    conflict.pixmapSelected = false;
    CHECK(!f.source->observe(conflict));
    CHECK(f.call.resume().issue == Issue::SelectionUnavailable);
    // Identical raw address under a different control block is not the same owner.
    conflict.imageData = std::shared_ptr<BrushTextureImageDataState>(f.data.get(),
        [](BrushTextureImageDataState*) {});
    CHECK(!f.source->observe(conflict));
    CHECK(f.call.resume().issue == Issue::SelectionUnavailable);
}

TEST_CASE(brush_texture_image_active_copy_remains_guarded_for_null_and_nonnull_source)
{
    for (const bool nullData : {false, true}) {
        Fixture f; f.image(nullData);
        BrushTextureImageObservation o;
        o.imagePaintingActiveWord = 1;
        o.cachedPixmapPresent = false;
        REQUIRE(f.source->observe(o));
        CHECK(f.call.resume().issue == Issue::ActiveImageCopyUnrecovered);
        CHECK(count(f.call, Effect::ImageValueCopied) == 0);
        CHECK(f.data->temporaryValueReferences() == 0);
        CHECK(!f.call.report().opaque.has_value());
    }
}

TEST_CASE(brush_texture_image_preflight_is_const_noexcept_and_does_not_refine_pending_reads)
{
    static_assert(noexcept(std::declval<const BrushTextureImageDataState&>().canObserve(
        BrushTextureImageDataObservation{})));
    static_assert(noexcept(std::declval<const BrushTextureImageState&>().canObserve(
        BrushTextureImageObservation{})));
    Fixture f; f.image();
    BrushTextureImageObservation ordinary; ordinary.imagePaintingActiveWord = 0;
    const auto& source = *f.source;
    REQUIRE(source.canObserve(ordinary));
    CHECK(f.call.resume().issue == Issue::PaintingActiveUnavailable);
    REQUIRE(f.source->observe(ordinary));
    CHECK(f.call.resume().issue == Issue::ImageFormatUnavailable);
    const auto effects = f.call.effects();
    const auto& data = *f.data;
    REQUIRE(data.canObserve({5, true}));
    CHECK(f.data->temporaryValueReferences() == 1u);
    CHECK(f.call.effects() == effects);
    CHECK(f.call.resume().issue == Issue::ImageFormatUnavailable);
    REQUIRE(f.data->observe({5, true}));
    CHECK(f.call.resume().opaque == false);
    CHECK(f.data->temporaryValueReferences() == 0u);
}

TEST_CASE(brush_texture_image_conflicting_preflight_leaves_all_other_unknown_fields_untouched)
{
    Fixture f; f.image();
    BrushTextureImageObservation conflict;
    conflict.pixmapSelected = true; conflict.imagePaintingActiveWord = 0;
    CHECK(!f.source->canObserve(conflict));
    CHECK(f.call.resume().issue == Issue::PaintingActiveUnavailable);
    f.ordinaryCopy(); REQUIRE(f.data->observe({4, std::nullopt}));
    CHECK(f.call.resume().issue == Issue::ImageColorTableAlphaUnavailable);
    CHECK(!f.data->canObserve({5, true}));
    REQUIRE(f.data->canObserve({4, false}));
    CHECK(f.call.resume().issue == Issue::ImageColorTableAlphaUnavailable);
    CHECK(f.data->temporaryValueReferences() == 1u);
    REQUIRE(f.data->observe({4, false}));
    CHECK(f.call.resume().opaque == true);
}

TEST_CASE(brush_texture_image_preflight_rejects_zero_alias_and_foreign_source_owner)
{
    BrushTextureImageDataState invalid{0};
    CHECK(!invalid.canObserve({5, true}));
    Fixture f; f.image();
    BrushTextureImageObservation change;
    change.imageData = std::shared_ptr<BrushTextureImageDataState>(f.data.get(),
        [](BrushTextureImageDataState*) {});
    CHECK(!f.source->canObserve(change));
    BrushTextureImageState empty;
    change.imageData = std::make_shared<BrushTextureImageDataState>(0);
    CHECK(!empty.canObserve(change));
    change.imageData = std::shared_ptr<BrushTextureImageDataState>{};
    REQUIRE(empty.canObserve(change));
}

int main() { return testkit::run_all(); }
