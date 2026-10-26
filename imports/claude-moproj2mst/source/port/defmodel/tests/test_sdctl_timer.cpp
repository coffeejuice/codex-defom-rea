// StepTimer (ctor 14019cc60): constructed fields, checkpoint bookkeeping and the 100-entry cap
// of 140199df0, stop (140199eb0), and the five globals' names and flags as the static
// initialisers 140503880.. pass them (strings read from the binary by p1-02).
// ctest-labels: p1-05 sdctl
#include <defmodel/sdctl/step_timer.hpp>
#include <testkit/check.hpp>

#include <cstring>
#include <memory>
#include <string>

#if __has_include(<defmodel/options/static_objects.hpp>)
#include <defmodel/options/static_objects.hpp>
#define HAVE_STATIC_OBJECTS 1
#endif

using namespace def::model;

TEST_CASE(timer_ctor)
{
    const bool f = true;
    auto t = std::make_unique<StepTimer>("abc", &f);
    CHECK_EQ(std::string(t->name), std::string("abc"));
    CHECK_EQ(int(t->flag), 1);
    CHECK_EQ(t->count, 0);
}

TEST_CASE(timer_checkpoints)
{
    const bool f = false;
    auto t = std::make_unique<StepTimer>("x", &f);
    int sub = 0;
    t->start("Start");
    CHECK_EQ(t->count, 1);
    CHECK_EQ(std::string(t->labels[0]), std::string("Start"));
    CHECK(t->subs[0] == nullptr);
    t->checkpoint("step", &sub);
    CHECK_EQ(t->count, 2);
    CHECK(t->subs[1] == &sub);
    t->checkpoint(nullptr, nullptr);   // label 0: ignored
    CHECK_EQ(t->count, 2);
    const std::int32_t dt = t->stop("");
    CHECK_EQ(t->count, 3);
    CHECK_EQ(std::string(t->labels[2]), std::string("Stop"));
    CHECK_EQ(dt, t->times[2] - t->times[0]);
    CHECK(dt >= 0);
    for (int i = 0; i < 200; ++i) t->checkpoint("more", nullptr);
    CHECK_EQ(t->count, 100);           // count < 100 guard
    t->start("Start");                 // reset
    CHECK_EQ(t->count, 1);
}

TEST_CASE(global_timers)
{
    CHECK_EQ(std::string(g_timer_read_elem_integer_info.name),
             std::string("LLOBJ_DATA::ReadElemIntegerInfoFromDB()"));
    CHECK_EQ(int(g_timer_read_elem_integer_info.flag), 0);
    CHECK_EQ(std::string(g_timer_read_record.name), std::string("_DB_STEP_DATA::ReadRecordFromDB()"));
    CHECK_EQ(int(g_timer_read_record.flag), 0);
    CHECK_EQ(std::string(g_timer_read_arrays.name), std::string("_DB_STEP_DATA::ReadArraysFromDB"));
    CHECK_EQ(int(g_timer_read_arrays.flag), 1);
    CHECK_EQ(std::string(g_timer_read_obj_state_variable.name),
             std::string("_DB_STEP_DATA::ReadObjStateVariableFromDB"));
    CHECK_EQ(int(g_timer_read_obj_state_variable.flag), 1);
    CHECK_EQ(std::string(g_timer_read_obj_node_state_variable.name),
             std::string("_DB_STEP_DATA::ReadObjNodeStateVariableFromDB()"));
    CHECK_EQ(int(g_timer_read_obj_node_state_variable.flag), 0);
    CHECK_EQ(g_timer_read_arrays.count, 0);
#if defined(HAVE_STATIC_OBJECTS)
    // same records as p1-02's binary-checked table
    const StepTimer* globals[] = {&g_timer_read_elem_integer_info, &g_timer_read_record,
                                  &g_timer_read_arrays, &g_timer_read_obj_state_variable,
                                  &g_timer_read_obj_node_state_variable};
    int k = 0;
    for (const auto& r : def::model::options::kStepTimerInits) {
        CHECK_EQ(std::string(globals[k]->name), std::string(r.name));
        CHECK_EQ(int(globals[k]->flag), int(r.flag));
        ++k;
    }
    CHECK_EQ(k, 5);
#endif
}

int main() { return testkit::run_all(); }
