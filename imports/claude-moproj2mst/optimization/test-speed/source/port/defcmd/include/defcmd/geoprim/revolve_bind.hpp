// defcmd/geoprim/revolve_bind.hpp: points the p9-05 revolve hooks (def::model::geoprim::P905Ext)
// at their owners where those are in the build (packet p9-05). Called by the GEOPRIM builder
// 1402ba500 (p9-04, defcmd/geoprim/builder.cpp) before it runs the revolve.
#pragma once

namespace def::cmd::geoprim {

// Sets each P905Ext pointer whose owner header is present; the others keep their stubs.
void bind_p905_ext();

} // namespace def::cmd::geoprim
