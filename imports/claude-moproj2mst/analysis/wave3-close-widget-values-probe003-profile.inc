// Integration draft. Call after actual runtime/MainView/manager binding and before
// ensureDefaultSplitView. Data/state references are in the probe003 contract.
engine::WidgetRuntimeValueObservation widgetValues;
widgetValues.mainView = mainView;
widgetValues.manager = manager;
widgetValues.qualification = "SFTC11.0.2 probe003 first SplitView parent context; source-bound cache/value/receiver contract";
widgetValues.fontData = {{1, 168}};
widgetValues.paletteData = {{1}, {2}, {3}};
widgetValues.applicationFont.defaultValue = engine::WidgetFontValueObservation{1, 51};
widgetValues.applicationFont.mutexPresent = true;
widgetValues.applicationFont.classCachePresent = true;
widgetValues.applicationFont.classes = std::vector<engine::WidgetFontClassObservation>{};
widgetValues.applicationPalette.defaultValue = engine::WidgetPaletteValueObservation{1, 13893616};
widgetValues.applicationPalette.classCachePresent = true;
widgetValues.applicationPalette.classes = std::vector<engine::WidgetPaletteClassObservation>{
    {"QMenuBar", {2, 13893616}}, {"QMenu", {3, 13893616}}};
auto& mainValues = widgetValues.mainViewValues;
mainValues.font = engine::WidgetFontValueObservation{1, 0};
mainValues.palette = engine::WidgetPaletteValueObservation{1, 0};
mainValues.locale = engine::WidgetLocaleValue{391, 0};
mainValues.inheritedFontMask = mainValues.inheritedPaletteMask = 0;
mainValues.attributesLow = 65536;
mainValues.extendedAttributes = std::array<std::uint32_t, 3>{{12, 4112, 0}};
mainValues.windowFlags = 0x0800f001u;
mainValues.opaqueChildrenDirty = true;
mainValues.opaque = true;
mainValues.graphicsEffectPresent = false;
mainValues.autoFillBackground = false;
mainValues.stylePresent = false;
mainValues.stylesheetLength = 0;
mainValues.proxyPresent = false;
mainValues.events.objectFilterCount = 0;
mainValues.events.sameApplicationThread = true;
mainValues.events.layoutPresent = false;
mainValues.events.gestureContextCount = 0;
// Inferred from unchanged actual map identity across the natural raw15 call,
// together with the source's allocate-before-release/no-restore detach body.
mainValues.events.gestureMapExclusive = true;
mainValues.pendingChildren = std::vector<engine::WidgetPendingChildObservation>{
    {1, {}}, {2, {}}, {3, {}}, {4, {}}, {5, {}}, {0, manager->widgetState()}};
// Manager values are unnecessary for first-parent consumption; leave unknown.
widgetValues.freshPrivateLocale = engine::WidgetLocaleValue{391, 0};
widgetValues.applicationModalState = false;
widgetValues.applicationStylesheetLength = 0;
widgetValues.applicationEvents.present = true;
widgetValues.applicationEvents.closing = false;
widgetValues.applicationEvents.internalCallbackCount = 0;
widgetValues.applicationEvents.filters = std::vector<engine::WidgetApplicationEventObservation::Filter>{
    {engine::WidgetApplicationEventObservation::Filter::Kind::WindowsVistaStyle, true}};
widgetValues.applicationEvents.gestureManagerPresent = true;
widgetValues.freshWidgetEvents.objectFilterCount = 0;
widgetValues.freshWidgetEvents.sameApplicationThread = true;
widgetValues.freshWidgetEvents.layoutPresent = false;
widgetValues.freshWidgetEvents.gestureContextCount = 0;
widgetValues.freshSendChildEvents = true;
widgetValues.freshFieldD0Present = false;
widgetRuntime->observeStartupValues(widgetValues);
