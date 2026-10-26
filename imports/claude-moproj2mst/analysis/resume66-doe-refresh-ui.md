# DOE refresh: first effect after QTimer stop

The `SfMoDoeTable` virtual **`+0x1b8` resolves exactly to `0x1400f47e0`**. After a qualified actual QTimer stop returns, its first persistent effect is the unconditional **table `+0x98 = 1`** store at `0x1400f4800`. The next call targets the actual **Delete QToolButton**, UI payload `+0x30`, and applies Qt enabled-state semantics. That widget/private/parent state is the next unresolved owner boundary; it must not become a plain Boolean assignment.

Only this report and uniquely named bounded static evidence were written. No source, build, executable test, GUI, original execution or mutable REA changed. This refines CP65's identified slot/body rather than repeating its receiver-construction audit.

## Exact dispatch and reached order

CP65 confirmed `slotUpdateUI()` local method0 -> static-metacall case `0x1400d98dc` -> `0x1400f4e10`. That 51-byte slot loads its actual timer at table `+0xb0`, calls QTimer::stop at `0x1400f4e25`, then invokes table virtual `+0x1b8` at `0x1400f4e38`.

The primary table vtable is `0x14218bd38`; its `+0x1b8` slot at `0x14218bef0` contains **`0x1400f47e0`**, raw-slot SHA-256 `0fb79267b2de9978b96a49eac5aada2b48b91da9c243188ed158ae35b2401055`. The complete selected refresh body `[0x1400f47e0,0x1400f4e06)` was captured in CP65, body SHA-256 `e38e7942cf586759712b6bc860d9eb12debe2d10fdfe2179fe4fa0190eef566a`.

The bounded first prefix is:

1. `0x1400f4800`: write byte `table+0x98 = 1`.
2. Read signed integer `table+0xa0`, compare with -1. Values **greater than -1** request enabled=true; values <=-1 request false. This is a selector predicate, not a public validation rule.
3. Load `table+0x38` UI payload, then its `+0x30` widget. Call import `0x141e0ba40 = QWidget::setEnabled(bool)` at `0x1400f482a` (true) or `0x1400f4844` (false).
4. Only after that call returns does `0x1400f484a` begin actual global main-window/current-Project resolution. No passed Study or XML-result Boolean participates in the first prefix.

The native code has no null check on the table, UI payload or widget. A valid reconstructed binding must establish their actual identity/lifetime or refuse after retaining already reached timer/flag effects. It must not treat a null/unavailable pointer as a native no-op. The normal body later clears `+0x98`; uncompleted UI effects do not justify inventing unconditional cleanup.

## UI payload +0x30 ownership is now identified

Within setupUi `0x1400fcca0`, the newly bounded window establishes:

- `0x1400fd61f`: allocate **0x28** bytes.
- `0x1400fd64b`: construct a **QToolButton**, passing the actual UI payload `+0x08` container as parent.
- Install primary/secondary QToolButton vtables `0x142173958` / `0x142173918`; RTTI identifies `.?AVQToolButton@@`.
- `0x1400fd6b4`: publish the new pointer into UI payload **`+0x30`**.
- `0x1400fd6ff`: set objectName to **`m_tbDelete`**, literal at `0x14218cbe8`.

This is an actual control instance created inside the table's setup, not an editor300 widget or an abstract enable flag. The container at UI `+0x08`, completed setupUi, Qt parent linkage and live QWidget/private objects must belong to the same actual `SfMoDoeTable` owner. The bounded window does not reconstruct the whole layout, icon/resource setup or descendants of QToolButton construction.

CP65 established the table is cached by main-window `+0x458`, installed in dock `+0x460`, constructs its child timer and initially refreshes before global registration. Those constructor facts do not establish the current selector or widget attributes at a later Save; selection, parent changes and other UI calls can intervene.

## The actual Qt call does more than assign enabled

The local shipped `QtGui4_64.dll` was inspected without loading it. Export `QWidget::setEnabled(bool)` is at **preferred-base** address `0x6506f080` (module base `0x65000000`; runtime relocation unobserved). Its complete 55-byte body:

1. Load QWidgetPrivate from widget `+0x08`.
2. At `0x6506f09c`, call **QWidget::setAttribute(raw attribute32, !enabled)** through `0x65068fe0`.
3. Tail-call **QWidgetPrivate::setEnabled_helper(enabled)** at `0x65069f00`.

The attribute setter's first branch reads current state and returns without mutation only when it equals the requested attribute value. For raw32, its bit is read at QWidgetPrivate `+0x1c8`, bit0. The equal branch at `0x6506904d` goes to the verified plain epilogue `0x650696f1..0x650696fe`. Thus an otherwise unchanged enabled-state request can still reach an earlier raw32 change; do not skip this call based only on the apparent disabled bit.

The private helper reads the widget's current disabled bit0 from QWidgetData `+0x08`. Enable=true additionally considers window/parent state before its state comparison. The state-changing branch invokes further attribute/platform/focus/child/event work; that mutation path is not newly closed here and must not be summarized as one flag assignment. The retained qualified `wave3-close-splitter-post-value-parent-contract.json` documents the same helper boundary and explicitly retains its mutation/event guard.

One narrow **conditional unchanged branch** is fully bounded by the new entry/return views:

- actual selector `+0xa0 <= -1`, so requested enabled=false;
- actual Delete widget raw attribute32 is already true;
- actual Delete widget disabled bit0 is already true;
- widget, private/data and table generation are the actual live objects.

Then setAttribute32 takes its equality return, and setEnabled_helper(false) skips the enable-only parent checks, compares 0 with current disabled1 at `0x65069f6b`, and returns through `0x6506a215..0x6506a21e`. No widget state/event mutation is reached in this selected branch. The table's earlier `+0x98=1` store still occurred. This is conditional native evidence, **not** proof these current attribute facts hold at public Save or permission to infer them from constructor defaults.

## Minimal integrated continuation and retained guard

Once the XML seam supplies complete actual notifier population and the real table/timer owner, require that timer stop has returned on that same live generation. The next source-closed transition is the `+0x98=1` store and actual selector read. Refuse before the Delete widget call unless its actual QWidget/private/parent state qualifies either the selected unchanged branch or the complete state-changing effects. Preserve previous XML/error effects, timer stop and refresh-state store on a later refusal.

Do not add another isolated owner, use a generic editor control or Boolean diagram timer, copy the caller's Study into the table cache, or assume ctor selector -1 implies current disabled state. After successful widget return the next read is the actual global current Project, as documented in CP65. The UI owner remains session/main-window scoped across Project release and needs its own destruction evidence.

True and false InputTable results still enter the same no-argument refresh path: the wrapper saves the Boolean separately and invokes DoeTableRefresh unconditionally. A later UI guard retains the already committed XML/cleanup/error prefix and skips unreached PAR/final project phases. Current candidate owner state does not establish the table/timer/button facts above, so public refresh/B121 success remains guarded.

Acceptance should exercise the real integrated message stage: timer stop before flag store; selector -2/-1/0 boundary; exact Delete control identity; raw32 setter before private helper; the qualified already-disabled branch; an unknown or changing-attribute branch refusing before unmodeled effects; owner release after timer stop/flag store retaining the prefix; and true/false XML results reaching identical first UI decisions. These are source-contract fixtures until actual original owner/state and full public context are independently qualified. No fixtures were added or run here.

## Receipt

`analysis/resume66-doe-refresh-ui-receipt.json` SHA-256 is **`adac48ff7291775c4aecfa34ba1e283dd2f5c2c3ef7f70f394115ae8b35e4391`**. Six bounded raw captures total **11,582 bytes**: Delete allocation/name window, complete Qt setEnabled wrapper, attribute/private-helper prefixes and their plain return tails. Attribute jump-table data after its return is excluded.

| Artifact | SHA-256 |
| --- | --- |
| Original `DEF_GUI_MO_64.exe` | `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` |
| Original `QtGui4_64.dll` | `c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887` |
| Complete setEnabled wrapper body | `7e0693d88f50edd8f6476a47c944948a1e0564b60173ec8809cbce57e27ee49a` |
| Retained Qt post-value/enable contract | `2d4115f72d919b1790ac5d534acce46e976f1b256eee2e3a080acaeabce628f` |

No runtime QWidget attributes, timer activity, thread ownership, completed dock/setup lifetime, complete enabled-state mutation path or end-to-end active DOE Save is established. Token measurements are unavailable.
