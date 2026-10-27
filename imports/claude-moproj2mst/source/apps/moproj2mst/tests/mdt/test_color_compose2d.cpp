// CP298 supplied-provider composition checks; no public color bind.
// ctest-labels: m2m-mdt mdt-viewer
#include <testkit/check.hpp>
#include "../../src/engine/color_compose2d.hpp"
#include "../../src/engine/color_pack2d.hpp"
#include "../../src/engine/viewer2d.hpp"
#include <defmodel/layout/obj_geo2d.hpp>

#include <array>
#include <cstddef>
#include <cstdint>
#include <span>
#include <stdexcept>
#include <vector>

using namespace m2m::engine;

TEST_CASE(color_compose2d_zero_count_still_reads_twice) {
    CurveItemState owner;
    std::vector<int> calls;
    SuppliedColorProviders2D p;
    p.paletteRgb = [&] {
        calls.push_back(static_cast<int>(calls.size()) + 1);
        return calls.size() == 1 ? std::array<float, 3>{1, 0, 0}
                                 : std::array<float, 3>{0, 0, 1};
    };
    composeSuppliedRenderColor2D(owner, 0, p);
    CHECK(calls == std::vector<int>({1, 2}));
    CHECK(owner.renderWords().empty());
}

TEST_CASE(color_compose2d_getter_order_and_narrowing) {
    CurveItemState owner;
    std::vector<char> order;
    SuppliedColorProviders2D p;
    p.redF = [&] { order.push_back('r'); return 0.5000000298023224; };
    p.greenF = [&] { order.push_back('g'); return 0.25; };
    p.blueF = [&] { order.push_back('b'); return 1.0; };
    composeSuppliedRenderColor2D(owner, 255, p);
    CHECK(order == std::vector<char>({'r', 'g', 'b'}));
    CHECK(owner.renderWords().empty());
}

TEST_CASE(color_compose2d_provider_failures_leave_owner_untouched) {
    for (int fail = 1; fail <= 2; ++fail) {
        CurveItemState owner;
        int calls = 0;
        SuppliedColorProviders2D p;
        p.paletteRgb = [&]() -> std::array<float, 3> {
            if (++calls == fail) throw std::runtime_error("palette");
            return {1, 0, 0};
        };
        bool threw = false;
        try { composeSuppliedRenderColor2D(owner, 0, p); }
        catch (const std::runtime_error&) { threw = true; }
        CHECK(threw); CHECK_EQ(calls, fail); CHECK(owner.renderWords().empty());
    }
    for (int fail = 1; fail <= 3; ++fail) {
        CurveItemState owner;
        int calls = 0;
        const auto getter = [&]() -> double {
            if (++calls == fail) throw std::runtime_error("getter");
            return 0.5;
        };
        SuppliedColorProviders2D p;
        p.redF = getter; p.greenF = getter; p.blueF = getter;
        bool threw = false;
        try { composeSuppliedRenderColor2D(owner, 1, p); }
        catch (const std::runtime_error&) { threw = true; }
        CHECK(threw); CHECK_EQ(calls, fail); CHECK(owner.renderWords().empty());
    }
}

TEST_CASE(color_compose2d_writer_rejection_follows_preparation) {
    CurveItemState owner;
    def::model::OBJ_GEO2D curve;
    curve.curve_type = 1; curve.n_points = 1;
    auto* points = curve.allocate_points(); REQUIRE(points);
    points[0] = 0; points[1] = 0; points[2] = 0;
    owner.bind(&curve, 0.01);
    REQUIRE(owner.vertexCount() == 1);
    int calls = 0;
    SuppliedColorProviders2D p;
    p.paletteRgb = [&] { ++calls; return std::array<float, 3>{0, 1, 0}; };
    bool rejected = false;
    try { composeSuppliedRenderColor2D(owner, 0, p); }
    catch (const std::length_error&) { rejected = true; }
    CHECK(rejected);
    CHECK_EQ(calls, 2);
    CHECK(owner.renderWords().empty());
}

TEST_CASE(color_compose2d_distinct_palette_words_on_owner) {
    CurveItemState owner;
    def::model::OBJ_GEO2D curve;
    curve.curve_type = 1; curve.n_points = 1;
    auto* points = curve.allocate_points(); REQUIRE(points);
    points[0] = 0; points[1] = 0; points[2] = 0;
    owner.bind(&curve, 0.01);
    const std::array<double, 2> xy{0, 0};
    owner.packSuppliedRender2D(xy, xy,
        [](CurveItemState&, std::span<std::uint32_t>) {});
    const auto before = owner.renderWords();
    int reads = 0;
    SuppliedColorProviders2D p;
    p.paletteRgb = [&] {
        ++reads;
        return reads == 1 ? std::array<float, 3>{1, 0, 0}
                          : std::array<float, 3>{0, 0, 1};
    };
    composeSuppliedRenderColor2D(owner, 0, p);
    CHECK_EQ(reads, 2);
    CHECK_EQ(owner.renderWords()[0], 0xfc0000ffu);
    CHECK_EQ(owner.renderWords()[3], 0x00ff0000u);
    for (std::size_t i : {1u, 2u, 4u, 5u}) CHECK_EQ(owner.renderWords()[i], before[i]);
}

TEST_CASE(color_compose2d_getters_write_narrowed_words_and_fail_before_stores) {
    CurveItemState owner;
    def::model::OBJ_GEO2D curve;
    curve.curve_type = 1; curve.n_points = 1;
    auto* points = curve.allocate_points(); REQUIRE(points);
    points[0] = 0; points[1] = 0; points[2] = 0;
    owner.bind(&curve, 0.01);
    const std::array<double, 2> xy{0, 0};
    owner.packSuppliedRender2D(xy, xy,
        [](CurveItemState&, std::span<std::uint32_t>) {});
    std::vector<char> order;
    SuppliedColorProviders2D p;
    // This red double is halfway between adjacent binary32 values and narrows
    // to exactly 0.5f under the qualified default rounding environment.
    p.redF = [&] { order.push_back('r'); return 0.5000000298023224; };
    p.greenF = [&] { order.push_back('g'); return 0.25; };
    p.blueF = [&] { order.push_back('b'); return 1.0; };
    composeSuppliedRenderColor2D(owner, 1, p);
    CHECK(order == std::vector<char>({'r', 'g', 'b'}));
    CHECK_EQ(owner.renderWords()[0], 0xfcff3f7fu);
    CHECK_EQ(owner.renderWords()[3], 0x00ff3f7fu);
    const auto before = owner.renderWords();
    int paletteReads = 0;
    SuppliedColorProviders2D bad;
    bad.paletteRgb = [&]() -> std::array<float, 3> {
        if (++paletteReads == 2) throw std::runtime_error("second palette read");
        return {0, 1, 0};
    };
    bool threw = false;
    try { composeSuppliedRenderColor2D(owner, 0, bad); }
    catch (const std::runtime_error&) { threw = true; }
    CHECK(threw); CHECK_EQ(paletteReads, 2);
    CHECK(owner.renderWords() == before);
}

int main() { return testkit::run_all(); }
