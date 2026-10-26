// dbread/w1_wiring.cpp: the fun_<addr> names the lead and helpers W2..W4 of packet p-dbread declared
// for W1's functions (pending_w2/w3/w4.hpp, lead.cpp). Thin forwards; the ports carry the @src tags.
#include <defdbio/dbread/w1.hpp>

namespace def::dbio::dbread {

// part of @src 1401d2230
void fun_1401d2230(model::DB_STEP_DATA* m) { read_obj_info(m); }

// part of @src 1401309b0 (the function reads iifnc / llfnc only)
void fun_1401309b0(model::DYNARY* f, const std::int32_t* iifnc, const double* llfnc, std::int32_t fid,
                   const char* flag)
{
    read_func_from_db_array(f, const_cast<std::int32_t*>(iifnc), const_cast<double*>(llfnc), fid, flag);
}
void fun_1401309b0(model::DYNARY* f, std::int32_t* iifnc, double* llfnc, std::int32_t fnum, char* flag)
{
    read_func_from_db_array(f, iifnc, llfnc, fnum, flag);
}

// part of @src 140131ea0 (reads data only)
void fun_140131ea0(model::DYNARY* f, const double* data) { read_table_rows_1d(f, const_cast<double*>(data)); }
void fun_140131ea0(model::DYNARY* f, double* data) { read_table_rows_1d(f, data); }

// part of @src 14025de80
void fun_14025de80(model::LLWINDEF_DATA* w, double* rec, double* llwrz, std::int32_t* iifnc, double* llfnc)
{
    read_window(w, rec, llwrz, iifnc, llfnc);
}

// part of @src 140324020
void fun_140324020(void* s, std::int32_t platform, std::int32_t nwords, std::int32_t ints_only)
{
    text_byteswap(s, platform, nwords, ints_only);
}
void fun_140324020(char* s, std::int32_t platform, std::int32_t n, std::int32_t flag)
{
    text_byteswap(s, platform, n, flag);
}

} // namespace def::dbio::dbread
