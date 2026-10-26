// GENDB check chain (packet p4-04): 1401bebf0 and its checks 1 (simulation control) and 5
// (inter-object data). Checks 2..4 and the summary hook are p4-03's (ext_p4_03.cpp).
#include <cstdio>

#include <defdbio/gendb/checks_p4_04.hpp>
#include <defrt/stdlib/legacy_printf.hpp>

#include "ext_p4_04.hpp"

namespace def::dbio::gendb {

// @src 1401bebf0  check chain; the index (1..6) is passed by address to every check
void check_chain(DB_STEP_DATA& s, std::int32_t* err, std::int32_t* warn)
{
    std::int32_t index;
    *err = 0;
    *warn = 0;
    index = 1;
    check_sim_control(s, &index, err, warn);
    index = 2;
    ext::fun_1401bfdf0(s, &index, err, warn);
    index = 3;
    ext::fun_1401bfe60(s, &index, err, warn);
    index = 4;
    ext::fun_1401bfed0(s, &index, err, warn);
    index = 5;
    check_inter_object(s, &index, err, warn);
    index = 6;
    ext::fun_14001cdc0(&index, err);
}

// @src 1401becf0  check 1 (__iob_func() + 0x30 is stdout)
void check_sim_control(DB_STEP_DATA& s, const std::int32_t* index, std::int32_t* err, std::int32_t* warn)
{
    ext::fun_14001cdc0(index, err);
    def::rt::crt::fprintf(stdout, "INFO   : Checking simulation controls\n");
    sim_ctrl_check(s.sim_ctrl, err, warn);
}

// @src 1401bff40  check 5
void check_inter_object(DB_STEP_DATA& s, const std::int32_t* index, std::int32_t* err, std::int32_t* warn)
{
    ext::fun_14001cdc0(index, err);
    def::rt::crt::fprintf(stdout, "INFO   : Checking inter-object data\n");
    inter_obj_check(s.inter_obj, err, warn);
}

void reset_globals_p4_04() { g_method_codes = {}; }

} // namespace def::dbio::gendb
