// defmodel/sdctl/step_timer.hpp: the step timer of the DB readers (no RTTI; ctor 14019cc60).
// Records up to 100 named checkpoints with their clock() time in ms; the report function
// (140199f20) is empty in DEF_PRE_64, so the timer never writes output.
//
//   ctor     14019cc60(this, name, &flag): strcpy(this, name), flag at 0x539c, reset 140199f30
//   use      140199dc0(&t, "Start") at the start of a reader, 140199df0(&t, "<checkpoint>", sub)
//            after each step, 140199eb0(&t, &report) at the end ("Stop")
//   dtor     14019ccb0 (empty; registered with atexit by the static initialisers)
//   size     0x53a0 (last field: the byte at 0x539c; the globals are 0x53a0 apart at most)
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/layout_check.hpp>

namespace def::model {

class StepTimer {
public:
    // @src 14019cc60  ctor
    StepTimer(const char* timer_name, const bool* flag);

    // @src 140199f30  reset: count = 0
    void reset();
    // @src 140199dc0  start: reset, then checkpoint(label, 0)
    void start(const char* label);
    // @src 140199df0  checkpoint: when label != 0 and count < 100, record now(), the label
    //                 (strcpy into a 200-byte slot) and sub, count++
    void checkpoint(const char* label, void* sub);
    // @src 140199eb0  stop: checkpoint("Stop", 0); when *report, report (140199f20, empty);
    //                 return time[count - 1] - time[0]
    std::int32_t stop(const char* report);
    // @src 140199f20  report: empty in DEF_PRE_64
    void report() {}

    // @src 140199f50  clock() in ms, (int64)((double)clock() / 1000.0 * 1000.0)
    static std::int64_t clock_ms();

    char name[200];                  // 0x0000
    char labels[100][200];           // 0x00c8
    std::int32_t times[100];         // 0x4ee8 clock_ms() at each checkpoint (truncated to 32 bits)
    void* subs[100];                 // 0x5078 third argument of checkpoint
    std::int32_t count;              // 0x5398
    std::uint8_t flag;               // 0x539c
    std::byte pad_539d[3];           // 0x539d
};

static_assert(sizeof(StepTimer) == 0x53a0);
DEF_LAYOUT_OFFSET(StepTimer, labels, 0xc8);
DEF_LAYOUT_OFFSET(StepTimer, times, 0x4ee8);
DEF_LAYOUT_OFFSET(StepTimer, subs, 0x5078);
DEF_LAYOUT_OFFSET(StepTimer, count, 0x5398);
DEF_LAYOUT_OFFSET(StepTimer, flag, 0x539c);

// The five global timers, built by p1-02's static initialisers (arguments recovered in
// options/static_objects.hpp, kStepTimerInits); atexit dtors 140504780.. call the empty 14019ccb0.
// Strings are the binary's.

// @src 140503880  static initialiser: 14019cc60(&DAT_1406e2e20, "...", &false)
// @data 1406e2e20  timer of LLOBJ_DATA::ReadElemIntegerInfoFromDB()
extern StepTimer g_timer_read_elem_integer_info;
// @src 140503980  static initialiser (flag 0)
// @data 1406f2e60  timer of _DB_STEP_DATA::ReadRecordFromDB()
extern StepTimer g_timer_read_record;
// @src 1405039c0  static initialiser (flag 1)
// @data 1406f8200  timer of _DB_STEP_DATA::ReadArraysFromDB
extern StepTimer g_timer_read_arrays;
// @src 140503a00  static initialiser (flag 1)
// @data 1406edac0  timer of _DB_STEP_DATA::ReadObjStateVariableFromDB
extern StepTimer g_timer_read_obj_state_variable;
// @src 140503a40  static initialiser (flag 0)
// @data 1406e8720  timer of _DB_STEP_DATA::ReadObjNodeStateVariableFromDB()
extern StepTimer g_timer_read_obj_node_state_variable;

} // namespace def::model
