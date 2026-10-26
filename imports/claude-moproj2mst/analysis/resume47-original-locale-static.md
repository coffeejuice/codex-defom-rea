# Checkpoint 47: static locale evidence before Save

## Question

Does retained original evidence determine the CRT locale or active codepage before Save, or is a process-local runtime observation still needed?

## What static evidence establishes

- **Qt QTextStream numeric output in the recovered MST closure is statically specified.** The retained source-qualified format note `source/docs/re/moproj2mst/s1-02/spec.md` states that this writer closure makes no `QTextStream::setLocale` or number-format setter calls, that `QLocale::setDefault` is not imported, and that Qt 4 therefore uses `QLocale::c()` defaults. Its numeric contract includes decimal point `.` and no grouping. This supports that particular Qt writer's numeric profile; it does not establish the Windows CRT `LC_NUMERIC` state used by unrelated `sprintf`/`fscanf`/`printf` paths.
- **Qt's default QString locale codec delegates to the active ANSI code page.** `analysis/wave3-editor-observed-language-binding.json` binds `QString::toLocal8Bit` through Qt's `QWindowsLocalCodec` to `WideCharToMultiByte(CP_ACP, 0, ...)`. `analysis/wave3-ht-native-contract.json` documents `codecForLocale`/`fromUnicode` and the null-codec Latin1 fallback, while `analysis/wave3-editor-translator-codec-observation-contract.json` warns that null locale-codec storage can be lazily initialized and that foreign codec setters depend on startup language. Thus the conversion mechanism and selected non-default Qt codec branches are statically known.
- **The shipped EXE manifest does not specify `activeCodePage`.** `analysis/wave3-original-manifest-codepage-qualification.json` records the original executable/resource hashes, absence of an embedded activeCodePage override, and absence of adjacent external manifest. This removes one source of an application-specific ACP override but does not reveal the numeric ACP returned by this process.
- **Imports are not setters.** The startup static inventory `analysis/wave3-closure-next-close-splitter-startup-entry-static.json` identifies `GetACP`, `GetOEMCP`, and `SetThreadLocale` as imported APIs. Import presence alone does not prove they execute before Save or what values they return. The original-locale static search found no source-qualified CRT `setlocale`, `_configthreadlocale`, `_setmbcp`, or `SetLocaleInfo` call in the reviewed converter source. That source absence says nothing conclusive about CRT initialization, linked runtime behavior, indirect calls, or code outside the reviewed closure.

## What remains runtime-dependent

- The target process's `GetACP()` and `GetOEMCP()` values. Host ACP 1252/OEM 437 is not a substitute. The app manifest result only says no explicit manifest override was found.
- The live Qt locale codec pointer/name after startup and any language-specific codec selection. Startup evidence says the language global controls whether Shift-JIS, Big5, KOI8-R, EUC-KR, or GBK setters are used; default language leaves the locale codec unset for Qt's system behavior. Which state is active for the captured original process requires reading the actual stage's startup state or a process-local ACP observation. A raw null codec pointer before lazy initialization is not a stable answer.
- The original process's CRT locale categories and any mutations before Save, especially `LC_NUMERIC`. Static evidence establishes C-locale behavior for the identified QTextStream closure, not for every CRT formatted I/O path. A process may also initialize CRT locale independently of Qt's QLocale.
- The effective C runtime's precise treatment of codepage/locale edge cases (unmappable characters, invalid sequences, numeric parsing) under that live state.

## Highest-value next observation

At the next already-authorized, already-required original-process observation, record `GetACP()` and `GetOEMCP()` in the target process and capture the raw locale-codec startup fields at the planned post-startup stage. If a Save action is already occurring, also record `setlocale(LC_NUMERIC, NULL)` without changing locale. Keep these observations separate: Win32 ACP governs `CP_ACP` conversions; Qt's current codec may be a language override or lazy system codec; CRT numeric locale governs CRT formatted numbers. Do not infer one from another.

If the existing probe cannot make an in-process CRT query without exceeding its frozen observation contract, leave CRT numeric locale explicitly unresolved and use the statically established QLocale::C rule only for the corresponding QTextStream writer closure. Do not expand the probe or launch another app-control session solely for this audit.

## Limits

This is a static evidence reconciliation, not a new original capture. The manifest resource/hash and code references are inherited from the named receipts; no source, binary, build, app-control session, or REA database was modified. Static absence is bounded to the reviewed source/decompile/import evidence and does not prove absence of indirect or external runtime effects.
