// ctest-labels: defmodel gui mesh-defaults
// Original GUI 0x14085d9b0: .25/.25/.5 regardless of owner dimension.
// DEF_PRE 0x140189790 retains its separate 2D override (1/1/1).
#include <testkit/check.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/llobj_ctor/obj_defaults.hpp>
#include <defmodel/llobj_ctor/llobj_copy.hpp>
#include <defmodel/step/db_step.hpp>

using namespace def::model;

namespace {
template<class T> T& field(LLOBJ_DATA& o, std::size_t offset)
{
    return *reinterpret_cast<T*>(reinterpret_cast<std::byte*>(&o) + offset);
}
struct Variant {
    bool saved = g_gui_engine_variant;
    explicit Variant(bool gui) { g_gui_engine_variant = gui; }
    ~Variant() { g_gui_engine_variant = saved; }
};

void checkDefaults(bool gui, int dim)
{
    Variant variant(gui);
    DB_STEP_DATA model(dim);
    LLOBJ_DATA object(&model);
    const double strain = gui || dim != 2 ? .25 : 1.;
    const double curvature = gui || dim != 2 ? .5 : 1.;
    CHECK_EQ(field<double>(object, 0x2d30), strain);
    CHECK_EQ(field<double>(object, 0x2d38), strain);
    CHECK_EQ(field<double>(object, 0x2d48), curvature);
    CHECK_EQ(field<double>(object, 0x2d28), 0.);
    CHECK_EQ(field<double>(object, 0x2d40), 0.);
    // Original GUI still branches on dimension for the other mesh settings.
    CHECK_EQ(field<double>(object, 0x2d20), dim == 2 ? 3. : 2.);
    CHECK_EQ(field<std::int32_t>(object, 0x2d90), dim == 2 ? 1000 : 100);
    CHECK_EQ(field<double>(object, 0x2dc8), dim == 2 ? 0. : -.7);
    field<double>(object, 0x2d30) = 17.;
    field<double>(object, 0x2d38) = 23.;
    field<double>(object, 0x2d48) = 31.;
    llobj_ctor::meshgen_defaults(object.s2d10, true);
    CHECK_EQ(field<double>(object, 0x2d30), strain);
    CHECK_EQ(field<double>(object, 0x2d38), strain);
    CHECK_EQ(field<double>(object, 0x2d48), curvature);
}
} // namespace

TEST_CASE(gui_mesh_defaults_both_dimensions)
{
    checkDefaults(true, 2);
    checkDefaults(true, 3);
}

TEST_CASE(def_pre_mesh_defaults_preserve_dimension_branch)
{
    checkDefaults(false, 2);
    checkDefaults(false, 3);
}

TEST_CASE(gui_mesh_copy_preserves_weights_across_dimensions)
{
    Variant variant(true);
    DB_STEP_DATA two(2), three(3);
    LLOBJ_DATA source(&two), dest(&three);
    field<double>(source, 0x2d30) = .125;
    field<double>(source, 0x2d38) = .375;
    field<double>(source, 0x2d48) = .625;
    const char shapes = 0, byDim = 1;
    llobj_copy::meshgen_copy(dest.s2d10, source.s2d10, &shapes, &byDim);
    CHECK_EQ(field<double>(dest, 0x2d30), .125);
    CHECK_EQ(field<double>(dest, 0x2d38), .375);
    CHECK_EQ(field<double>(dest, 0x2d48), .625);
    CHECK_EQ(field<double>(source, 0x2d30), .125);
}

int main() { return testkit::run_all(); }
