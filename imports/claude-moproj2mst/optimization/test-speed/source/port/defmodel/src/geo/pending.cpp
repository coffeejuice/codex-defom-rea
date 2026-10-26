// Defaults of the pending cross-packet calls (defmodel/geo/pending.hpp).
#include <defmodel/geo/pending.hpp>

namespace def::model::pending {

#define DEF_PENDING_RESET(addr, name, R, ...) name = &wired::name;

void reset_pending() { DEF_PENDING_LIST(DEF_PENDING_RESET) }

} // namespace def::model::pending
