// DB info object Cls_1401dbfe0 (packet p1-03): .DB header and step directory of the open DB.
// Layout: defmodel/layout/cls_1401dbfe0.hpp. The loader FUN_1401dc3f0 is in defdbio/step/.
#include <defmodel/step/db_step.hpp>
#include <defmodel/step/db_directory_receipt.hpp>

#include <cstdlib>

namespace def::model {

// @src 1401d9080  directory entry ctor
DbDirRecord::DbDirRecord()
    : mesh_no(0), step(0), hsize(0), mar_size(0), a_size(0), sim_no(0), sim_mode(0), dim(0),
      version(0), stored(0)
{
}

// @src 1401d9110  directory entry dtor (empty)
DbDirRecord::~DbDirRecord() = default;

// @src 1401d91d0  0xe0 entry dtor
DbInfoEntryE0::~DbInfoEntryE0()
{
    if (u20 != nullptr) {
        std::free(u20);
    }
}

// @src 1401e6730  vector deleting dtor of the directory array, called with flag 3 (delete[])
void delete_dir_records(DbDirRecord* p)
{
    directory_receipts::allocationRetired(p);
    delete[] p;
}

// @src 1401e67c0  vector deleting dtor of the 0xe0 array, called with flag 3 (delete[])
void delete_entries_e0(DbInfoEntryE0* p) { delete[] p; }

// @src 1401dbfe0  ctor
Cls_1401dbfe0::Cls_1401dbfe0()
{
    db_info_init(*this);
    u1908 = nullptr;
    u1910 = 0;
}

// @src 1401dc1c0  dtor
Cls_1401dbfe0::~Cls_1401dbfe0()
{
    directory_receipts::retireReader(this);
    db_info_reset(*this);
    if (u1908 != nullptr) {
        if (u1908 != nullptr) {
            std::free(u1908);
        }
        u1908 = nullptr;
    }
}

// @src 1401dc030  init
void db_info_init(Cls_1401dbfe0& d)
{
    directory_receipts::clearReaderAllocation(&d);
    d.u650[0] = '\0';
    d.ua50[0] = '\0';
    d.uaa1[0] = '\0';
    d.uaa9[0] = '\0';
    d.uab1[0] = '\0';
    d.uabc = 0;
    d.uac0 = 0;
    d.uac4 = 0;
    d.uac8 = 0;
    d.uad0 = 0;
    d.uacc = 0;
    d.u000 = 0;
    d.u004 = 0;
    d.uad8 = nullptr;
    d.uae8 = nullptr;
    d.uae0 = -1;
    d.u1900 = 0;
    for (std::int32_t i = 0; i < 400; ++i) {
        d.uaf0[i] = 0;
    }
    d.u008 = 1;
    for (std::int32_t i = 0; i < 100; ++i) {
        d.u00c[i].b = 0;
        d.u00c[i].a = 0;
        d.u330[i] = nullptr;
    }
}

// @src 1401dc220  drop the directory and the 100 buffers
void db_info_reset(Cls_1401dbfe0& d)
{
    if (d.uac8 != 0) {
        if (d.uad8 != nullptr && d.uad8 != nullptr) {
            delete_dir_records(static_cast<DbDirRecord*>(d.uad8));
        }
        if (d.uae8 != nullptr && d.uae8 != nullptr) {
            delete_entries_e0(static_cast<DbInfoEntryE0*>(d.uae8));
        }
        d.uac8 = 0;
        d.uacc = 0;
        d.uad8 = nullptr;
        directory_receipts::clearReaderAllocation(&d);
        d.uae8 = nullptr;
        d.u1900 = 0;
    }
    for (std::int32_t i = 0; i < 100; ++i) {
        if (d.u330[i] != nullptr) {
            std::free(d.u330[i]);
            d.u330[i] = nullptr;
        }
    }
}

} // namespace def::model
