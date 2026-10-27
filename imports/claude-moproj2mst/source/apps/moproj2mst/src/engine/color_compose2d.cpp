#include "color_compose2d.hpp"

#include "color_pack2d.hpp"
#include "viewer2d.hpp"

namespace m2m::engine {

void composeSuppliedRenderColor2D(CurveItemState& owner, std::uint8_t mode,
    const SuppliedColorProviders2D& providers) {
    std::uint32_t first;
    std::uint32_t second;
    if (mode == 0) {
        first = packColor2D(providers.paletteRgb(), 0.99f);
        second = packColor2D(providers.paletteRgb(), 0.0f);
    } else {
        const float red = static_cast<float>(providers.redF());
        const float green = static_cast<float>(providers.greenF());
        const float blue = static_cast<float>(providers.blueF());
        const std::array<float, 3> rgb{red, green, blue};
        first = packColor2D(rgb, 0.99f);
        second = packColor2D(rgb, 0.0f);
    }
    owner.applySuppliedRenderColorWords2D(first, second);
}

} // namespace m2m::engine
