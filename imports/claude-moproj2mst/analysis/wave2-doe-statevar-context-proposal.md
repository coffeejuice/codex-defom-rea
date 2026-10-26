# StateVar constructor input and ownership proposal

The current candidate guards before the lazy StateVar constructor. The next increment can consume an explicitly supplied profile snapshot; an unavailable snapshot remains unsupported. It must not read the current user's settings or equate unavailable inputs with absent settings.

Proposed owned header: `source/apps/moproj2mst/include/m2m/doe_statevar.hpp`.

```cpp
#pragma once
#include <array>
#include <optional>
#include <string>
#include "m2m/qt4.hpp"
namespace m2m::doe {
enum class SettingObservation { Unavailable, Absent, Value };
template<class T> struct StateVarSetting {
    SettingObservation observation = SettingObservation::Unavailable;
    T converted{};
    std::optional<T> result() const {
        if (observation == SettingObservation::Unavailable) return {};
        return observation == SettingObservation::Absent ? T{} : converted;
    }
};
struct StateVarPalette {
    std::array<int,25> codes;             //143d76584 ->1446b3d60
    std::array<int,25> alternateCodes;    //143d765e8 ->1446b3dd0
    std::array<std::array<float,4>,25> rgba; //143d763f4 ->1446b3bd0
};
struct StateVarGlobalValues {
    int colorbarNo;                      //143d763c4 ->+40
    qt::String title;                    //143d763d8 ->+438
    std::array<float,4> deflectionReferenceRgba; //143d77590+270 ->+490
    int paletteMode;                     //143d763ec ->+528
    bool paletteFlag0, paletteFlag1;      //143d763f0/f1 ->+52c/d
    StateVarPalette palette;
    int principalMode;                   //143d7664c ->+530
    std::array<float,12> principalRgba;   //143d76650 ->+534
    std::array<int,7> iso;                //143d76688..6a0 ->+564..57c
};
struct StateVarConstructorContext {
    std::string profileIdentity; //declared or captured source; never imply runtime observation
    std::optional<StateVarGlobalValues> globals; //unavailable distinguished from all-zero
    StateVarSetting<int> deflectionScaleType;     //displayOption/dispDeflectionScaleType
    StateVarSetting<int> deflectionScale;         //displayOption/dispDeflectionScale
    StateVarSetting<int> deflectionLineThickness; //displayOption/dispDeflectionLineThkType
    StateVarSetting<bool> showDeflectionReference; //displayOption/dispDeflectionRefShow
    StateVarSetting<bool> transparentDeflectionReference; //.../dispDeflectionRefTransparent
};
}
```

Root-owned minimal shared change, after source freeze releases: forward-declare `namespace m2m::doe { struct StateVarConstructorContext; }` in common.hpp and add `std::shared_ptr<const doe::StateVarConstructorContext> doeStateVarConstructor;` to OriginalSettings, default null. No ABI entry point or CLI setting is added. Owned Session callback passes `project.originalSettings.doeStateVarConstructor.get()` to the ordered StepData import.

An owned per-conversion StateVar runtime state should reside with StudyIo, which already owns the recovered per-conversion original error/diagnostic observations. It retains current global palette copies and deferred histogram registrations. StepDataState owns the lazy StateVar object separately from its property-table marker, preserving it through GuiModel reset. The immutable constructor context is input; it must not double as mutable palette output state. No extra Project/GuiModel shared field is needed.

Constructor d31610 really writes all three global palette arrays. Retaining them only as opaque input fields would lose an established effect. The runtime palette state is updated at d31610 after the earlier constructor steps, and subsequent unsupported guards preserve that prefix. Histogram411700 installs four deferred notifier registrations via2cfbe0 and does not immediately invoke their slots. Full notification dispatch remains a later consumer boundary.

Before source integration, close remaining constructor nested calls:9dd490 ->a390b0, Histogram font12c08c0 ->12c0d40, and d312d0 ->d312a0. These may be represented as opaque local blocks only where no current consumer observes them; this does not claim full QObject/Qt ownership parity. Scalar input then follows d328c0 encounter order, including integer conversion into double ranges and colorbar assignment before auxiliary recreation. Changed colorbar auxiliary state, SVHistoGraph payload and postparse effects must retain their own exact guards until recovered.

Source qualifications and exact raw references are retained in `wave2-doe-statevar-successor-contract.json` and its next addendum. Current source remains frozen; this file is a proposal, not implemented or validated behavior.
