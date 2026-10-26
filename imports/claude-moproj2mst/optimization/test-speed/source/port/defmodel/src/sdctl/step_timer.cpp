// StepTimer (ctor 14019cc60), see defmodel/sdctl/step_timer.hpp.
#include <defmodel/sdctl/step_timer.hpp>

#include <cstring>
#include <ctime>

namespace def::model {

namespace {
const bool kFalse = false;
const bool kTrue = true;
} // namespace

StepTimer g_timer_read_elem_integer_info("LLOBJ_DATA::ReadElemIntegerInfoFromDB()", &kFalse);
StepTimer g_timer_read_record("_DB_STEP_DATA::ReadRecordFromDB()", &kFalse);
StepTimer g_timer_read_arrays("_DB_STEP_DATA::ReadArraysFromDB", &kTrue);
StepTimer g_timer_read_obj_state_variable("_DB_STEP_DATA::ReadObjStateVariableFromDB", &kTrue);
StepTimer g_timer_read_obj_node_state_variable("_DB_STEP_DATA::ReadObjNodeStateVariableFromDB()",
                                               &kFalse);

// @src 14019cc60  ctor: strcpy(this, name) (140484f90), flag = *flag, reset (140199f30)
StepTimer::StepTimer(const char* timer_name, const bool* flag_in)
{
    std::strcpy(name, timer_name);
    flag = static_cast<std::uint8_t>(*flag_in);
    reset();
}

// @src 140199f30  reset
void StepTimer::reset() { count = 0; }

// @src 140199dc0  start
void StepTimer::start(const char* label)
{
    reset();
    checkpoint(label, nullptr);
}

// @src 140199df0  checkpoint
void StepTimer::checkpoint(const char* label, void* sub)
{
    if (label != nullptr && count < 100) {
        times[count] = static_cast<std::int32_t>(clock_ms());
        std::strcpy(labels[count], label);
        subs[count] = sub;
        count = count + 1;
    }
}

// @src 140199eb0  stop; "Stop" is the string at 140522f88
std::int32_t StepTimer::stop(const char* report_flag)
{
    checkpoint("Stop", nullptr);
    if (*report_flag != '\0') report();
    return times[count - 1] - times[0];
}

// @src 140199f50  1000.0 is the double at 140522f90 (used as divisor and factor). The value is
//                 only stored in the timer, never written out, so the real CRT clock is used.
std::int64_t StepTimer::clock_ms()
{
    const std::clock_t c = std::clock();
    return static_cast<std::int64_t>((static_cast<double>(static_cast<std::int64_t>(c)) / 1000.0) * 1000.0);
}

} // namespace def::model
