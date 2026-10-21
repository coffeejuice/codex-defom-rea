#pragma once

#include "object_model.hpp"

#include <cstddef>
#include <functional>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace deform::recovered {

struct KeywordDefinition {
    std::string_view name;
    int id;
    int category;
};

struct KeywordEvent {
    std::string token;
    int id = 305;
    int category = 0;
    std::size_t offset = 0; // First token byte, after skipped whitespace.
    std::size_t cursor = 0; // Cursor after handler or bounded unknown-line read.
    std::optional<int> handler_status;
    bool unknown_warning = false;
    std::string skipped_line;
};

struct KeywordImportResult {
    enum class Outcome { parsed, unsupported };
    Outcome outcome = Outcome::parsed;
    // Original parser status over the handled prefix. On unsupported outcome
    // this is NOT a success/error status for the unimplemented remainder.
    int status = 0;
    std::size_t cursor = 0;
    bool reached_eof = false;
    // Requested1d7f40 remains pending on unsupported parser outcomes or when
    // the asserted reset/recovered-material profile is unavailable.
    bool finalization_pending = true;
    std::string unsupported_reason;
    std::vector<KeywordEvent> events;
};

class KeywordImporter {
public:
    using HandlerObserver = std::function<void(const KeywordEvent&, const ObjectModel&)>;

    // Catalog must contain the complete 304-row recovered table in original
    // order. Names are copied; caller-provided string views need not outlive
    // construction. This prevents treating a known unimplemented keyword as
    // unknown and incorrectly skipping only one line of its payload.
    KeywordImporter(ObjectModel& model, std::span<const KeywordDefinition> catalog);

    // Synchronous observation after a handler returns and before finalization.
    // No callback for unknown or unsupported keywords. References are valid
    // only for the call; observers must not mutate the model/importer, reenter
    // import, or throw. Passing an empty callback disables observation.
    void set_handler_observer(HandlerObserver observer);

    // Already text-translated bytes, using KeywordReader's bounded numeric
    // compatibility domain. Normal wrapper option0 only. Supported handlers:
    // TITLE, CURSIM, OPRNAM, SIMNAM; STYPE, SMODE, GEOTYP, MESHNO, STNOUT, NSTART, PDIE,
    // DSMAX, DSMAX_NSTEP, DTMAX, DTTYPE,
    // MTNAME, MSTMTR, SIZEMD, TXTURE, SIZESH, TXTODF, OBJNAM, OBJTYP,
    // YOUNG, POISON, EXPAND, THRCND, HEATCP, EMSVTY, ELRST, PMEAB, PMITT,
    // MASDEN, COARSE, GBENGY, NUCSIZ; PHASTF, TTTD, TTTN, PHASLH, PHASVL,
    // FSTRES and GRNDAT (all selector branches in the disabled grain profile);
    // TRNSFP, PHEQVF, COARSN, IPSNRG, VARSEL; STROKE, DIEGEO, RZ, ELMCON,
    // MTLGRP, VOLFC, VOLFS; fixed mesh fields DRZ, BCCDEF, BCCDFN, URZ,
    // FRZ, PRZ, BCCTMP, BCCTFN, NDHEAT, NDFLUX, STNCMP, DENSTY, DVOLF,
    // TICF, VOTAGE, DATOM, HDNOBJ, BCCCRB, BCCRHT, CRBFLX, RHTFLX,
    // YLDS, ZEFI, ZMFI, CURRNT; NDTMP, USRNOD, USRELM, MATAXI, NWEAR,
    // DRMESH, CUSVAR and DYNARY. Fixed field widths come from the current root;
    // support for a field does not imply recovery of every width producer.
    // Mutations are immediate; errors preserve created records and written rows.
    // Unsupported paths stop explicitly without inventing an original status.
    // request_finalization matches wrapper*param3: reset import flags before
    // parsing, then finalize handled returns (including reader status2/3).
    [[nodiscard]] KeywordImportResult import_translated(
        std::string_view bytes, bool request_finalization = true);

private:
    struct Entry {
        int id;
        int category;
        std::string name;
    };
    [[nodiscard]] const Entry* lookup(std::string_view token) const noexcept;
    ObjectModel& model_;
    std::vector<Entry> catalog_;
    HandlerObserver handler_observer_;
};

} // namespace deform::recovered
