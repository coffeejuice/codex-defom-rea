#pragma once

#include "mesh_storage.hpp"
#include "geometry_storage.hpp"
#include "geometry_segments.hpp"
#include "custom_fields.hpp"
#include "property_table.hpp"
#include "unit_system.hpp"
#include "furnace_model.hpp"

#include <array>
#include <cstdint>
#include <memory>
#include <map>
#include <optional>
#include <span>
#include <string>
#include <vector>

namespace deform::recovered {

// 1e7e90 defaults, reached by root initialization and changed-dimension reset.
// Native offsets 750/7a1/7e2/834 are evidence anchors, not a binary layout.
struct RootMetadata {
    std::string title = "SIMULATION";
    std::string operation_name = "OPERATION 1";
    std::string simulation_name = "SIMULATION 1";
    std::array<int, 3> cursim{1, 0, 0};
};

// Owned type0x80 table subset at root938. Rank and mode are deliberately not
// duplicated here: they alias SimulationControls::control_0x93c/mode_0x990.
// Supports only the fresh/reset rank0/1 allocation domain. Higher ranks,
// borrowed native pointers, allocation failure, and interpolation are outside
// this reconstruction. Raw metadata54/60 has no asserted domain meaning.
struct TimeTableControls {
    std::int32_t type_0x00 = 0x80;
    double constant_0x08 = 0.0;
    std::array<std::int32_t, 3> selectors_0x10{};
    std::array<std::int32_t, 3> counts_0x1c{};
    std::array<std::vector<double>, 3> axes;
    std::int32_t values_per_point_0x40 = 1;
    std::vector<double> dependent;
    std::int32_t flag_0x50 = 0;
    std::int32_t preserved_0x54 = 0;
    std::uint64_t preserved_0x60 = 0;
};

// Asserted reset context for the selected configuration fields. Material state
// retains default properties with the recovered reference graph below.
// This profile is not inferred from an externally supplied component table.
struct StoppingControls {
    std::array<double, 3> smax_0x8d0{};
    std::array<double, 3> vmin_0x8e8{};
    std::array<double, 3> lmax_0x900{};
    std::int32_t nstep_0x8bc = 1; // 1e8490 final controls16c store.
    std::int32_t stpdef_0x918 = 1; // 1e8650 controls1c8.
    double emax_0x8c0 = 0.0; // 1e8fb0 controls170.
    double tmax_0x8c8 = 0.0; // 1e8fb0 controls178.
    double demax_0x9b0 = 0.0; // Factory supplies owner-dimension default.
};

struct TimeStepImportControls {
    double slderr_0x9b8 = 0.0;
    // DTPMAX destinations: 9c0,9f0,9f8,a00,a08; only the first three in3D.
    std::array<double, 5> dtpmax{};
    std::array<double, 2> dvmax_0x9c8{};
    std::array<int, 3> mdsobj_0x9d8{};
    double mdsobj_0x9e8 = 0.0;
};

struct PlanarStepControls {
    std::array<int, 4> osctrl_0x1108{};
    int cnterr_0x1118 = 0;
    double cnterr_0x1120 = 0.0;
};

struct SpatialVfactrControls {
    int active_count_0x112c = 0;
    PropertyTable table_0x1130{0xb5}; // Its width aliases root1170.
};

struct DimensionalStepControls {
    std::optional<PlanarStepControls> planar;
    std::optional<SpatialVfactrControls> spatial;
    int vfactr_mode_0x1128 = 0;
    double dtsub_0xa10 = 0.0; // Factory supplies1 in3D,0 in2D.
    double dplen_0xa18 = 0.5;
    double odmax_0x1318 = 0.0; //39d2b0 writes controlsbc8, not roota18.
};

struct IterationControls {
    std::array<double, 2> cvgerr_0xa20{}; // Factory supplies exact dimension defaults.
    int itrmth_0xa30 = 1;
    int solmtd_0xa34 = 1;
    int parameter_count_0xa38 = 0;
    std::array<double, 4> parameters_0xa40{};
    int itrmxd_0xa60 = 200;
    int defbwd_0xa64 = 1;
    int solmtt_0xa68 = 1;
    int itrmxt_0xa6c = 200;
    int tmpbwd_0xa70 = 1;
    int latent_0xa74 = 0;
};

struct RootRecordTables {
    int frequency_rows_0xa78 = 6;
    int frequency_columns_0xa7c = 1;
    PropertyTable frequency_0xa80{0xb5}; // Width aliases rootac0.
    std::array<PropertyTable, 3> records{
        PropertyTable{0xb0}, PropertyTable{0xb1}, PropertyTable{0xb2}};
};

struct UserDefinitionText {
    //USRDEF writes80 bytes into each81-byte slot. The untouched sentinel
    //and initial uninitialized tail are not fabricated as native zero bytes.
    std::array<std::uint8_t, 80> bytes{};
    std::size_t known_prefix = 1; //1ead60 writes only the first NUL.
};

struct CustomVariableNames {
    std::int32_t id;
    std::string name;
    std::vector<std::string> labels;
};

struct RootGeneralFields {
    std::array<double, 4> geometry_errors_0xdb0{};
    double peninf_0xaf8 = 0.0;
    double tintgf_0xb08 = 0.0;
    double ehrate_0xd88 = 0.0;
    std::string frnnam_0xb90 = "Furnace model"; //375470 constructor, not defaults.
    std::array<UserDefinitionText, 10> user_definitions_0xdd8;
};

struct SimulationControls {
    FurnaceCollection furnace;
    RootGeneralFields general;
    RootRecordTables record_tables;
    IterationControls iteration;
    DimensionalStepControls dimensional;
    TimeStepImportControls time_step_imports;
    StoppingControls stopping;
    RootUnitControls units;
    std::int32_t mesh_number_0x840 = 1; // MESHNO raw integer, default from1e7e90.
    std::int32_t mode_0x844 = 1; // SMODE raw int; no implied scientific enum.
    std::int32_t type_0x848 = 1; // STYPE direct int; downstream meaning unestablished.
    std::int32_t flag_0x854 = 0;
    std::int32_t flag_0x858 = 1;
    int geometry_type = 1; // Root +0x850, GEOTYP.
    std::int32_t material_gate_0x85c = 0; // Independent feature49 gate.
    std::int32_t flag_0x860 = 0;
    std::int32_t grain_gate_0x864 = 0;
    std::int32_t method_0x868 = 0; // HTMTHD raw int; scientific consumers pending.
    std::array<int, 10> element_layout_0x888{}; // ELMNOD; initialized by1e7e90.
    std::array<bool, 10> element_layout_known{true,true,true,true,true,true,true,true,true,true};
    int& grain_mode_0x898() noexcept { return element_layout_0x888[4]; }
    const int& grain_mode_0x898() const noexcept { return element_layout_0x888[4]; }
    std::array<int, 7> output_flags{0, 0, 0, 0, 1, 0, 0}; // +0x86c..0x884.
    std::int32_t step_number_0x8b0 = -1; // NSTART.
    std::int32_t stpinc_0x8b4 = 10; // 1e8490 controls164; raw STPINC integer.
    std::int32_t saved_step_0x744 = -1;
    std::int32_t selected_object_0x8b8 = 0;
    std::int32_t pdie_secondary_0x1104 = 0; // 1ead60: controls+9b4, an int32.
    double time_step_0x920 = 0.0;
    double time_step_secondary_0x928 = 0.0;
    double step_increment_0x930 = 0.0;
    std::int32_t control_0x93c = 0;
    std::int32_t mode_0x990 = 1;
    TimeTableControls time_table;
    double field_0xd90 = 0.0; // TNOW; scientific consumers remain unrecovered.
    double field_0xd98 = 0.0;
    std::int32_t field_0xda0 = 0;
    double field_0xda8 = 0.0;
    // 1d21c0/1d2090 retain root-wide user-field definitions. The observed
    // records are64-byte calloc blocks; their contents remain opaque here.
    std::int32_t user_node_width_0x3398 = 0;
    std::int32_t user_element_width_0x33a8 = 0;
    std::vector<std::array<std::uint8_t, 64>> user_node_definitions;
    std::vector<std::array<std::uint8_t, 64>> user_element_definitions;
};

// Import controls may be supplied explicitly or generated for an empty reset
// root. Scientific interpretation of descriptor widths is not inferred here.
struct RootControls {
    std::array<int, 79> component_counts;
    int dimension;
    double node8_default;
    std::int32_t direct61_default;
    bool suppress_boundaries;
    std::array<int, 3> element_multiplicities{1, 1, 1};
    bool alternate_root_enabled = false;
    std::optional<SimulationControls> simulation;
    // 1cb170 zeros all280 raw entries. DB mapping of this table remains
    // pending; query29 reads the dimension-selected185/186 fallback entry.
    std::array<std::int32_t, 280> raw_0x1dc0{};
};

// Recovered 1c8c00/1e7c50/1d09c0 empty-root profile. Temperature initialization
// still comes from the process unit context, supplied explicitly by the caller.
[[nodiscard]] RootControls initialized_root_controls(int dimension, double node8_default,
    bool suppress_boundaries = false);

struct ObjectRecord {
    explicit ObjectRecord(int object_id, int dimension);

    // Addresses of these records survive subsequent list expansion.
    const int id;
    int type = 0;
    int secondary = 0;
    std::string name; // Original name bytes, not decoded Unicode.
    bool flag_0x3990 = false; // Constructor15f0b0; gates2059a0 geometry work.
    int flag_0x3994 = 0;
    GeometryStorage geometry; // Slot0 at3c00; other slots remain unrecovered.
    // Owner-bound15f0b0 creates one default container at3998 in both dimensions.
    // Absent containers from ownerless construction/allocation failure and
    // additional containers are outside this owned ObjectRecord profile.
    GeometrySegments geometry_segments;
    // Separate object+b58 custom-variable collection; ordinary mesh clear
    // does not destroy its entries. Object destruction releases owned records.
    CustomFields custom_fields;
    DynamicArrays dynamic_arrays; // Independent positional list at object+ba8.
    std::array<double, 3> stroke_0x22d0{}; // 18b500 zeros;18bc30 preserves.
    std::int32_t material_group_0x3980 = 0; // Constructor0; deactivation preserves.
    int element_kind;
    MeshStorage mesh;

    // 38b8a0 OBJTYP: map type, store secondary, then deactivate if type 7.
    // This is the recovered field/mesh subset, not all original subobjects.
    void apply_type(int file_type, int secondary_value);
};

// Explicit initialized fields of288ae0 and selected28a3e0 constructor state.
// Native padding, pointers and scientific evaluation caches are not inferred.
struct FstresProperties {
    std::int32_t mode_0x50 = 1;
    std::array<double, 4> coefficients_0x58{};
    PropertyTable table_0x78{0x4f};
    std::array<double, 5> coefficients_0xf0{};
    std::array<double, 4> coefficients_0x118{};
    PropertyTable table_0x138{0x25};
    PropertyTable table_0x1b0{0x24};
    PropertyTable table_0x228{0x50};
    std::array<double, 7> coefficients_0x2a0{};
    std::array<double, 7> coefficients_0x2d8{};
    std::array<double, 6> coefficients_0x310{};
    std::array<double, 5> coefficients_0x340{};
    std::array<double, 4> coefficients_0x368{};
    PropertyTable table_0x388{0xae}; // Initial scalar constant is1.0.
    PropertyTable table_0x400{0x26};
    std::int32_t field_0x478 = 0;
    PropertyTable table_0x480{0xa6}; // Constructor allocates axis1/dependent2.
    PropertyTable table_0x4f8{0xa7}; // Rank0 width1, but owns one dependent value.
    PropertyTable table_0x570{0xa8};
    PropertyTable table_0x5e8{0x51}; // Typed initialization defers allocation.
    std::int32_t field_0x660 = 0;
};

struct GrndatRecord {
    std::int32_t count_0x00 = 12;
    std::array<double, 12> inline_0x08{};
    double scalar_0x68 = 0;
    std::int32_t integer_0x70 = 0;
    std::array<PropertyTable, 3> tables{PropertyTable{0x5e}, PropertyTable{0x5f}, PropertyTable{0x60}};
    bool skip_first_two_0x1e0 = false;
};

struct GrndatProperties {
    std::array<std::int32_t, 4> flags_0x1078{};
    std::array<PropertyTable, 9> tables_0x1088{PropertyTable{0x55}, PropertyTable{0x54},
        PropertyTable{0x56}, PropertyTable{0x57}, PropertyTable{0x58}, PropertyTable{0x59},
        PropertyTable{0x5a}, PropertyTable{0x5b}, PropertyTable{0x5c}};
    double scalar_0x14c0 = 0, scalar_0x14c8 = 0;
    std::int32_t secondary_0x14d0 = 0;
    GrndatRecord embedded_0x14d8;
    // Layout order16c8/16e0/16f8/1710/1728. Import traversal starts at index2.
    // Owned value copies serve observation; native pointer-sharing is unproved.
    std::array<std::vector<GrndatRecord>, 5> lists;
    std::array<double, 6> coefficients_0x17b8{};
    std::int32_t secondary_0x17ec = 0;
    PropertyTable table_0x1740{0xac}; // Initial rank2, selectors3/8, no arrays.
    GrndatProperties() { embedded_0x14d8.skip_first_two_0x1e0 = true; }
};

// Selected constructor defaults from28c6f0/28cdc0, MSTMTR references and
// SIZEMD/TXTURE controls and thirteen shared property-table import destinations.
// Other scientific properties and table consumers remain unrecovered.
// Logical count may be nonpositive while the previous allocation is retained.
struct MaterialDirectFields {
    double frae2h_0x1ec8 = 0.9; //28cdc0: binary64 3feccccccccccccd.
    double field_0x1ed0 = 0.0; //LTHEAT reads a temporary, not this field.
    double tmpsol_0x1ed8 = 0.0;
    double tmpliq_0x1ee0 = 0.0;
    double matden_0x1ee8 = 0.0; //Distinct from MASDEN's owned table.
    std::array<double, 2> gbmobi_0x1868{};
};

struct MaterialCurveImports {
    std::array<PropertyTable, 2> fperv{PropertyTable{0}, PropertyTable{0}}; //1ef0/1f68.
    std::int32_t difcoe_mode_0x950 = 0;
    std::array<PropertyTable, 3> difcoe{PropertyTable{0x29}, PropertyTable{0x27},
        PropertyTable{0x28}}; //958/9d0/a48; inactive tables retain their storage.
    std::int32_t difcoe_reference_0xac0 = 0;
    PropertyTable difbnd_0x2160{0xaf};
    MaterialCurveImports() {
        //289930 resets only rank after initializing the first type29 table.
        difcoe[0].rank_0x04 = 0;
    }
};

struct FrcmodProperties {
    std::int32_t mode_0x1ad0 = 0;
    std::array<double, 6> scalars_0x1ad8{}; //Five inline coefficients plus1b00.
    std::int32_t reference_0x1b08 = 0;
    std::array<PropertyTable, 3> tables{PropertyTable{0x1b}, PropertyTable{0x1c},
        PropertyTable{0xb4}}; //1b10/1b90/1c08; first shared with UTSDAT119.
    double scalar_0x1b88 = 0.0;
    std::array<double, 9> coefficients_0x1c80{}; //Physical ascending order, not read order.
};

struct MaterialHardeningImports {
    std::int32_t hdnrul_0x668 = 0; //28cdc0, live integer destination.
    PropertyTable hdnpha_0xf10{0};
    PropertyTable hdntim_0xf88{0x53};
    PropertyTable jominy_0x1000{0x52};
};

struct MaterialRecord {
    enum class Profile { default_properties, default_name_only = default_properties };
    explicit MaterialRecord(int material_id);

    const int id; // Exact signed ID; neither positivity nor contiguous IDs required.
    std::string name; // At most64 original bytes, with native capacity65.
    const Profile profile = Profile::default_properties;
    const std::int32_t field_0x04 = 0;
    const std::int32_t field_0x08 = 0;
    std::int32_t feature_0x17e8 = 0; // GRNDAT selector; queries29/53 gated by root864.
    std::int32_t mode_0x1cc8 = 0;
    std::int32_t count_0x1ccc = 0;
    const std::int32_t feature_0x1e40 = 0;
    std::int32_t feature_0x2150 = 0;
    std::int32_t feature_0x2154 = 0;
    std::int32_t field_0x2158 = 1;
    std::int32_t field_0x215c = 1;
    // Native keyword IDs identify fixed destinations, not an extensible native
    // dictionary. Construction installs exactly the thirteen recovered slots.
    std::map<int, PropertyTable> property_tables;
    double expansion_reference_0x7d8 = 0.0;
    std::int32_t heat_capacity_mode_0x8d0 = 0;
    std::int32_t coarsening_enabled_0x20d0 = 0;
    MaterialDirectFields direct;
    MaterialCurveImports curves;
    FrcmodProperties frcmod;
    MaterialHardeningImports hardening;
    FstresProperties fstres;
    GrndatProperties grndat;
    // 29b300/28c5a0: count only; no inferred scientific interpretation.
    [[nodiscard]] int texture_value_count() const noexcept;
    [[nodiscard]] std::span<const std::int32_t> reference_ids() const;
    [[nodiscard]] std::span<const std::int32_t> retained_reference_ids() const noexcept {
        return reference_allocation_;
    }
    [[nodiscard]] bool has_reference_allocation() const noexcept {
        return !reference_allocation_.empty();
    }

private:
    friend class ObjectModel;
    std::vector<std::int32_t> reference_allocation_;
};

// 398350 maps sorted material ranks to the columns present in a VOLFC/VOLFS
// input. Unselected ranks retain -1; this is not a transitive graph traversal.
struct MaterialColumnMapping {
    std::vector<int> columns;
    int input_width = 0;
};

// Explicit333f60 fields and owned tables used by383120. Native padding and
// uninitialized name-tail bytes are not represented; this is not a780-byte ABI image.
struct RelationRecord {
    RelationRecord(int first, int second);
    const int key0;
    const int key1; // Ordered exact signed pair, not a material-rank pair.
    std::int32_t mode_0x08 = 0;
    std::int32_t active_0x0c = 0;
    std::int32_t field_0x10 = 0;
    std::int32_t field_0x14 = 0;
    std::string name = " "; // Native constructor writes space and terminator only.
    std::int32_t field_0x6c = 2;
    std::int32_t field_0xf0 = 0;
    std::int32_t field_0x470 = 0;
    std::int32_t field_0x5e0 = 0;
    std::int32_t field_0x5e4 = 0;
    std::int32_t field_0x660 = 0;
    std::int32_t field_0x768 = 0;
    double field_0x70 = 0.0;
    double field_0x170;
    double field_0x178;
    double field_0x3e8 = 0.0;
    double field_0x3f0 = 0.0;
    std::array<double, 11> coefficients_0x270{};
    std::array<double, 4> coefficients_0x340{};
    std::array<double, 6> coefficients_0x360{};
    std::array<double, 4> coefficients_0x390{};
    std::array<double, 7> coefficients_0x3b0{};
    std::array<double, 2> coefficients_0x668{};
    // Keys are native byte offsets, not keyword IDs: TTTD selects several tables.
    std::map<int, PropertyTable> property_tables;
};

class ObjectModel {
public:
    explicit ObjectModel(RootControls controls);
    ObjectModel(const ObjectModel&) = delete;
    ObjectModel& operator=(const ObjectModel&) = delete;
    ObjectModel(ObjectModel&&) = delete;
    ObjectModel& operator=(ObjectModel&&) = delete;

    [[nodiscard]] const RootControls& controls() const noexcept { return controls_; }
    [[nodiscard]] double node8_default() const noexcept;
    [[nodiscard]] const RootMetadata& metadata() const noexcept { return metadata_; }
    [[nodiscard]] RootMetadata& metadata() noexcept { return metadata_; }
    // 1c8ba0: a changed dimension destroys existing objects and resets controls.
    // Same dimension is a no-op, including object identity and component counts.
    void set_dimension(int dimension);
    // 1d8000/1d8050: caller owns the linked root and keeps it alive while linked.
    // Linking aligns its dimension; low-level set_dimension does not propagate.
    void link_root(ObjectModel* root);
    [[nodiscard]] ObjectModel* linked_root() noexcept { return linked_root_; }
    [[nodiscard]] const ObjectModel* linked_root() const noexcept { return linked_root_; }
    // 1b6520 applies the recognized header to primary, then its captured link.
    void apply_header_dimension(int dimension);
    // Store the configuration first, then regenerate the root component table
    // before visiting type!=7 objects. Reader failures still call these setters
    // with their zero/partial results. No replacement defaults or finalization.
    // All three require reset controls and selected default material properties.
    void set_geometry_type(int value);
    // 1f3ff0 stores every value; only modes1/2/3 replace flags854/858.
    // It does not refresh component layout or propagate to the linked root.
    void set_simulation_mode(std::int32_t value);
    void set_simulation_type(std::int32_t value);
    void set_mesh_number(std::int32_t value);
    void set_unit(std::int32_t value);
    void set_tnow(const std::array<double, 4>& values);
    void set_trans(const std::array<int, 5>& values);
    void set_heat_method(std::int32_t value);
    void set_stpinc(std::int32_t value);
    void set_stopping_integer(int keyword, std::int32_t value);
    void set_stopping_real(int keyword, double value);
    void set_stopping_vector(int keyword, std::span<const double> values);
    void set_time_step_real(int keyword, std::span<const double> values);
    void set_mdsobj(const std::array<int, 3>& integers, double value);
    void set_osctrl(const std::array<int, 4>& values);
    void set_cnterr(int value, double coefficient);
    [[nodiscard]] PropertyTable* set_vfactr_header(int mode, int count);
    void set_dimensional_real(int keyword, double value);
    void set_convergence(const std::array<double, 2>& values);
    void set_iteration_integer(int keyword, int value);
    [[nodiscard]] bool set_iteration_selector(int keyword, int value);
    void set_solver_parameters(const std::array<double, 4>& values);
    [[nodiscard]] PropertyTable* set_frequency_header(int rows, int columns);
    [[nodiscard]] PropertyTable* reset_record_table(int keyword, int rows, int columns);
    void set_root_scalar(int keyword, double value);
    void set_geometry_errors(const std::array<double, 4>& values);
    void set_root_name(std::string value);
    [[nodiscard]] PropertyTable& environment_table(int keyword);
    [[nodiscard]] FurnaceCollection& furnace_collection();
    void set_element_layout_prefix(std::span<const int> values);
    void mark_element_layout_indeterminate(std::size_t known, std::size_t copied);
    void set_output_flags(const std::array<int, 7>& values);
    void refresh_components();
    // Changed user widths resize their root definition table before global
    // preserving refresh. Equal widths have no allocation/refresh effects.
    void set_user_field_width(bool nodes, int width);
    void set_user_field_name(bool nodes, std::size_t index, std::string_view name);
    void set_user_definition(std::size_t index, std::string_view text);
    void set_custom_name(std::int32_t id, std::string name);
    void clear_custom_labels(std::int32_t id);
    void append_custom_label(std::int32_t id, std::string label);
    [[nodiscard]] std::span<const std::unique_ptr<CustomVariableNames>> custom_names() const noexcept {
        return custom_names_;
    }
    // Native getters use stored container counts and a single descriptor
    // selector; even an equal-size call updates descriptor shape history.
    [[nodiscard]] std::span<double> user_field_values(ObjectRecord& object, bool nodes);
    [[nodiscard]] std::span<double> prepare_material_axes(ObjectRecord& object);
    [[nodiscard]] std::span<double> prepare_node_wear(ObjectRecord& object);
    // 417c80 writes the root width before rejecting an unsupported selector.
    // Successful selector0 performs both183840 and the selected getter.
    void set_material_shape_width(int component_id, int width);
    [[nodiscard]] std::span<double> prepare_material_shape(ObjectRecord& object, int component_id);
    // Mode2 writes this live root field after its header, independently of
    // the descriptor width/physical storage chosen by the preceding getter.
    [[nodiscard]] int& material_axes_width() noexcept { return controls_.component_counts[46]; }
    // 19a4e0/32f630: successful CUSVAR rows overwrite one signed root field.
    // Constructor32f100 initializes it; dimension reset1c8c00 preserves it.
    void select_custom_variable(std::int32_t id) noexcept { custom_last_id_0x33b8_ = id; }
    [[nodiscard]] std::int32_t custom_last_id() const noexcept { return custom_last_id_0x33b8_; }
    // Normal wrapper option0: the integer reader always stores its output;
    // only a successful read sets2f88, and a later failure does not clear it.
    void set_start_step(std::int32_t value, bool read_succeeded);
    // DSMAX/DTMAX preserve zero/partial reader outputs. Only positive primary
    // values clear the competing control and invoke1ee510 normalization.
    void set_displacement_step(double value);
    void set_time_steps(const std::array<double, 2>& values);
    // DSMAX_NSTEP ignores reader status and writes930 only when the selected
    // record exists and both supplied values are nonzero. Type7 is eligible.
    void set_step_distance(double value, int count);
    void set_pdie(int selected_object, int secondary);
    // DTTYPE after a successful integer-pair helper: reset type80 preserving
    // metadata54/60, select rank/mode, allocate zero-filled rank1 buffers.
    // Returned storage is for the immediate interleaved stream-read stage.
    [[nodiscard]] TimeTableControls& initialize_time_table(int selector, int count);
    // 1d7f10 runs only when the wrapper's finalization argument is true.
    // These flags survive dimension reset, unlike SimulationControls.
    void reset_import_flags() noexcept;
    [[nodiscard]] bool import_flag_0x2f88() const noexcept {
        return import_flag_0x2f88_;
    }
    [[nodiscard]] bool import_flag_0x2f89() const noexcept {
        return import_flag_0x2f89_;
    }
    // 1d7f40 for the selected material-reference profile and recovered finite
    // slot0 geometry. False means the root profile is unavailable.
    [[nodiscard]] bool finalize_import();
    [[nodiscard]] std::uint64_t generation() const noexcept { return generation_; }
    [[nodiscard]] ObjectRecord* find(int id) noexcept;
    [[nodiscard]] const ObjectRecord* find(int id) const noexcept;
    // Nonpositive IDs return null. A missing positive ID appends intervening
    // type-7 placeholders, followed by the requested constructor-default record.
    [[nodiscard]] ObjectRecord* find_or_create(int id);
    [[nodiscard]] std::span<const std::unique_ptr<ObjectRecord>> objects() const noexcept {
        return objects_;
    }

    [[nodiscard]] MaterialRecord* find_material(int id) noexcept;
    [[nodiscard]] const MaterialRecord* find_material(int id) const noexcept;
    // Missing IDs append one stable record, update1320, then refresh the root
    // layout before MTNAME reads its name. Existing IDs skip those operations.
    // Existing active objects receive preserving mesh refresh; inactive ones
    // are skipped. Mode1 enables the native ID/rank remap of element49/50.
    // Shared native storage remains outside this owned model.
    [[nodiscard]] MaterialRecord* find_or_create_material(int id);
    // MSTMTR: begin zeros the direct reader destination and returns its old
    // logical count. Apply always stores parsed_count; failed reads preserve
    // mode/allocation. No reference operation eagerly refreshes components.
    [[nodiscard]] std::int32_t begin_material_reference_count(MaterialRecord& material);
    void apply_material_reference_count(MaterialRecord& material,
        std::int32_t previous_count, std::int32_t parsed_count, int reader_status);
    void write_material_reference(MaterialRecord& material, std::size_t index,
        std::int32_t reference_id);
    // Requires a live owned mesh and initialized direct61 element IDs. The
    // native helper otherwise leaves its output width uninitialized. Apply is
    // the success-only 3981c0 commit; a failed reader must not call it.
    [[nodiscard]] MaterialColumnMapping material_column_mapping(const ObjectRecord& object) const;
    void apply_material_columns(ObjectRecord& object, int descriptor_id,
        const MaterialColumnMapping& mapping, std::span<const double> values);
    // 202960 runs even after failed MTLGRP input. It selects the most frequent
    // element direct61 ID (first occurrence wins ties), retaining the old3980
    // when that selected value is zero. Requires the asserted reset profile.
    void finalize_material_groups(ObjectRecord& object);
    [[nodiscard]] std::int32_t material_count() const noexcept { return material_count_; }
    [[nodiscard]] std::span<const std::unique_ptr<MaterialRecord>> materials() const noexcept {
        return materials_;
    }
    [[nodiscard]] RelationRecord* find_relation(int key0, int key1) noexcept;
    [[nodiscard]] RelationRecord& find_or_create_relation(int key0, int key1);
    // PHASTF: nonzero enables, recounts exact active==1 and refreshes(option0).
    // Zero disables without recount/refresh, preserving a potentially stale13f4.
    void apply_relation_mode(RelationRecord& relation, std::int32_t mode);
    [[nodiscard]] std::int32_t relation_count() const noexcept { return relation_count_; }
    [[nodiscard]] std::span<const std::unique_ptr<RelationRecord>> relations() const noexcept {
        return relations_;
    }

private:
    RootControls controls_;
    RootMetadata metadata_;
    std::vector<std::unique_ptr<ObjectRecord>> objects_;
    std::vector<std::unique_ptr<MaterialRecord>> materials_;
    std::vector<std::unique_ptr<RelationRecord>> relations_;
    std::int32_t material_count_ = 0; // Root1320, separate from list ownership.
    std::int32_t relation_count_ = 0; // Root13f4; not the relation list count.
    std::int32_t custom_last_id_0x33b8_ = 0;
    //Adjacent33c0 collection is independent of selection and survives reset.
    std::vector<std::unique_ptr<CustomVariableNames>> custom_names_;
    [[nodiscard]] CustomVariableNames& custom_name_record(std::int32_t id);
    ObjectModel* linked_root_ = nullptr;
    std::uint64_t generation_ = 0;
    // Constructor1c8950 calls1d7f10 after1c8c00.
    bool import_flag_0x2f88_ = false;
    bool import_flag_0x2f89_ = false;
    [[nodiscard]] bool material_columns_enabled() const noexcept;
    [[nodiscard]] std::array<int, 2> grain_component_widths();
    void refresh_components_impl(std::span<const int> old_material_ids,
        bool remap_materials);
    void finalize_material_references();
};

} // namespace deform::recovered
