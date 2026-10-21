#include "recovered/object_model.hpp"
#include <stdexcept>

using namespace deform::recovered;

namespace {
void require(bool condition, const char* reason) {
    if (!condition) throw std::runtime_error(reason);
}
}

int main() {
    // Source-contract checks for positive DB fallback entries that the native
    // console keyword fixtures cannot populate. Not native DB-import parity.
    for (const int dimension : {2, 3}) {
        auto controls = initialized_root_controls(dimension, 20);
        controls.raw_0x1dc0[185] = 7;
        controls.raw_0x1dc0[186] = 11;
        ObjectModel model(controls);
        auto* material = model.find_or_create_material(31);
        material->feature_0x17e8 = 2;
        model.set_trans({-1, -1, -1, -1, 1});
        require(model.controls().component_counts[29] == 0, "TRANS must defer refresh");
        model.refresh_components();
        require(model.controls().component_counts[29] == (dimension == 2 ? 7 : 11),
                "query29 must use its dimension-selected positive fallback");
        require(model.controls().simulation->grain_mode_0x898 == 1, "query29 mode side effect");
        model.set_trans({-1, -1, -1, -1, 0});
        model.refresh_components();
        require(model.controls().component_counts[29] == 0, "disabled query must clear width");
        require(model.controls().simulation->grain_mode_0x898 == 1, "disabled query retains898");
        model.set_dimension(dimension == 2 ? 3 : 2);
        require(model.controls().raw_0x1dc0[185] == 0 && model.controls().raw_0x1dc0[186] == 0,
                "changed-dimension construction must zero fallback table");
    }
    auto controls = initialized_root_controls(3, 20);
    controls.raw_0x1dc0[186] = -5;
    ObjectModel model(controls);
    auto* material = model.find_or_create_material(1);
    material->feature_0x17e8 = 2;
    model.set_trans({-1, -1, -1, -1, 1});
    model.refresh_components();
    require(model.controls().component_counts[29] == 0, "nonpositive fallback ignored");
}
