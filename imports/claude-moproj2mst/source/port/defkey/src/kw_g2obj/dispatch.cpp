// kw-g2obj: routing from the group 2 shells to the packets that own the other cases and
// writers (see dispatch.hpp). A packet whose header is not on the branch yet is skipped: its
// parser cases fail with 2, its writers write nothing.
#include <defmodel/program_info.hpp>
#include "dispatch.hpp"

#include <defkey/key_writer.hpp>
#include <defkey/kw_g2obj.hpp>
#include <defmodel/llobj/object_list.hpp>
#include <defmodel/port.hpp>

#include "internal.hpp"

namespace def::key::g2obj::ext {

int read_case(model::DB_STEP_DATA& m, std::FILE* f, model::LLOBJ_DATA& obj, int id)
{
#ifdef DEF_KW_G2BC
    if (g2bc::handles(id)) return g2bc::read_case(m, f, obj, id);
#endif
#ifdef DEF_KW_G2MG
    if (g2mg::handles(id)) return g2mg::read_case(m, f, obj, id);
#endif
#ifdef DEF_KW_G2GEO
    if (g2geo::handles(id)) return g2geo::read_case(m, f, obj, id);
#endif
#ifdef DEF_KW_G2SV
    if (g2sv::handles(id)) return g2sv::read_case(m, f, obj, id);
#endif
    (void)m;
    (void)f;
    (void)obj;
    (void)id;
    return 2;  // the switch default, or a case whose packet has not landed
}

int write_keyword(int id, std::FILE* f, model::DB_STEP_DATA& m, const KeyRef& ref)
{
    if (g2obj::handles_write(id)) return g2obj::write_keyword(id, f, m, ref);
#ifdef DEF_KW_G2BC
    if (g2bc::handles_write(id)) return g2bc::write_keyword(id, f, m, ref);
#endif
#ifdef DEF_KW_G2MG
    if (g2mg::handles_write(id)) return g2mg::write_keyword(id, f, m, ref);
#endif
#ifdef DEF_KW_G2GEO
    if (g2geo::handles_write(id)) return g2geo::write_keyword(id, f, m, ref);
#endif
#ifdef DEF_KW_G2SV
    if (g2sv::handles_write(id)) return g2sv::write_keyword(id, f, m, ref);
#endif
    return 0;
}

int write_stnout(model::DB_STEP_DATA& m, std::FILE* f)
{
#ifdef DEF_KW_G1A
    return g1a::write_stnout(&m, f);
#else
    (void)m;
    (void)f;
    return 0;
#endif
}

int write_elmnod(model::DB_STEP_DATA& m, std::FILE* f)
{
#ifdef DEF_KW_G1A
    return g1a::write_elmnod(&m, f);
#else
    (void)m;
    (void)f;
    return 0;
#endif
}

void write_file_header(model::DB_STEP_DATA& m, std::FILE* f)
{
    // FUN_1401c5a20(m, f, &0) as 14029e250 calls it: product name of the model's dimension
    // (140224dd0 on 140158e90) and the start-up version string (140225450, set by main through
    // 140224d80; defmodel/program_info.hpp, packet p5-01)
    def::key::write_file_banner(f, 0, model::product_name_for_dim(m.dimension), model::version());
}

int write_winprs_block(model::DB_STEP_DATA& m, std::FILE* f, model::LLOBJ_DATA& obj, std::uint32_t no)
{
#ifdef DEF_KW_G2BC
    return g2bc::write_winprs_block(m, f, obj, no);
#else
    (void)m, (void)f, (void)obj, (void)no;
    return 0;
#endif
}

void write_window_lists(model::DB_STEP_DATA& m, std::FILE* f, model::LLOBJ_DATA& obj, std::uint32_t no)
{
#ifdef DEF_KW_G2BC
    g2bc::write_window_lists(m, f, obj, no);
#else
    (void)m, (void)f, (void)obj, (void)no;
#endif
}

int write_edge_face_bcs(model::DB_STEP_DATA& m, std::FILE* f, model::LLOBJ_DATA& obj, std::uint32_t no)
{
#ifdef DEF_KW_G2BC
    return g2bc::write_edge_face_bcs(m, f, obj, no);
#else
    (void)m, (void)f, (void)obj, (void)no;
    return 0;
#endif
}

int write_rsedef(std::FILE* f, model::DB_STEP_DATA& m, const KeyRef& ref)
{
#ifdef DEF_KW_G2BC
    return g2bc::write_rsedef(f, m, ref);
#else
    (void)f, (void)m, (void)ref;
    return 0;
#endif
}

int write_mesh_block(model::DB_STEP_DATA& m, std::FILE* f, model::LLOBJ_DATA& obj, std::uint32_t no)
{
#ifdef DEF_KW_G2MG
    return g2mg::write_mesh_block(m, f, obj, no);
#else
    (void)m, (void)f, (void)obj, (void)no;
    return 0;
#endif
}

int write_opstop(model::DB_STEP_DATA& m, std::FILE* f, model::LLOBJ_DATA& obj, std::uint32_t no)
{
#ifdef DEF_KW_G2MG
    return g2mg::write_opstop(m, f, obj, no);
#else
    (void)m, (void)f, (void)obj, (void)no;
    return 0;
#endif
}

int write_dynary(model::DB_STEP_DATA& m, std::FILE* f, model::LLOBJ_DATA& obj, std::uint32_t no)
{
#ifdef DEF_KW_G2MG
    return g2mg::write_dynary(m, f, obj, no);
#else
    (void)m, (void)f, (void)obj, (void)no;
    return 0;
#endif
}

int write_local_bcs(model::DB_STEP_DATA& m, std::FILE* f, model::LLOBJ_DATA& obj, std::uint32_t no)
{
#ifdef DEF_KW_G2BC
    return g2bc::write_local_bcs(m, f, obj, no);
#else
    (void)m, (void)f, (void)obj, (void)no;
    return 0;
#endif
}

void write_geometry(model::DB_STEP_DATA& m, std::FILE* f, model::LLOBJ_DATA& obj, std::uint32_t no)
{
#ifdef DEF_KW_G2GEO
    g2geo::write_geometry(m, f, obj, no);
#else
    (void)m, (void)f, (void)obj, (void)no;
#endif
}

int write_nodes(model::DB_STEP_DATA& m, std::FILE* f, model::LLOBJ_DATA& obj, std::uint32_t no)
{
#ifdef DEF_KW_G2GEO
    return g2geo::write_nodes(m, f, obj, no);
#else
    (void)m, (void)f, (void)obj, (void)no;
    return 0;
#endif
}

int write_elements(model::DB_STEP_DATA& m, std::FILE* f, model::LLOBJ_DATA& obj, std::uint32_t no)
{
#ifdef DEF_KW_G2GEO
    return g2geo::write_elements(m, f, obj, no);
#else
    (void)m, (void)f, (void)obj, (void)no;
    return 0;
#endif
}

int write_symsuf(model::DB_STEP_DATA& m, std::FILE* f, model::LLOBJ_DATA& obj, std::uint32_t no)
{
#ifdef DEF_KW_G2GEO
    return g2geo::write_symsuf(m, f, obj, no);
#else
    (void)m, (void)f, (void)obj, (void)no;
    return 0;
#endif
}

int write_wpaxis(model::DB_STEP_DATA& m, std::FILE* f, model::LLOBJ_DATA& obj, std::uint32_t no)
{
#ifdef DEF_KW_G2GEO
    return g2geo::write_wpaxis(m, f, obj, no);
#else
    (void)m, (void)f, (void)obj, (void)no;
    return 0;
#endif
}

int write_state_var(model::DB_STEP_DATA& m, std::FILE* f, model::LLOBJ_DATA& obj, int code,
                    std::uint32_t no, std::int32_t k)
{
#ifdef DEF_KW_G2SV
    return g2sv::write_state_var(m, f, obj, code, no, k);
#else
    (void)m, (void)f, (void)obj, (void)code, (void)no, (void)k;
    return 0;
#endif
}

int custom_count(model::LLOBJ_DATA& obj)
{
#ifdef DEF_KW_G2SV
    return g2sv::custom_count(obj);
#else
    (void)obj;
    return 0;
#endif
}

void* custom_at(model::LLOBJ_DATA& obj, int i)
{
#ifdef DEF_KW_G2SV
    return g2sv::custom_at(obj, i);
#else
    (void)obj, (void)i;
    return nullptr;
#endif
}

std::int32_t custom_id(void* entry)
{
#ifdef DEF_KW_G2SV
    return g2sv::custom_id(entry);
#else
    (void)entry;
    return 0;
#endif
}

} // namespace def::key::g2obj::ext

namespace def::key::g2obj {

// FUN_14017aaf0 (kw-g4b's def::model::object_list_find on model+0x1380)
LLOBJ_DATA* find_object(DB_STEP_DATA& m, std::int32_t no) { return model::object_list_find(&m.objects, no); }

} // namespace def::key::g2obj
