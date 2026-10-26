# CP213: nonnull postOpen and its completion boundary

A nonzero `1ca30` return is earlier than completion of the original Open continuation. Main subsequently runs `2a0e0`, which selects a model, conditionally activates subproject zero and binds views, then executes common virtual callbacks. Even a normal `2a0e0` return does not alone prove that activation happened: its inner selection helper can return normally with no current Project. These are distinct measurable boundaries.

## Evidence and scope

The target is the copied original installation v11.0.2 `evidence/original-binaries/DEF_GUI_MO_64.exe`, independently rehashed as `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Addresses below are RVAs, image base `140000000`. No reconstructed DEF_PRE or DMO artifact was used. Applied the reverse-engineer-anything skill using retained instruction evidence and fresh offline DUMPBIN 14.44.35228.0 views. No original process, GUI, REA session/database, source, fixture, build, deployment or shared plan was changed; only this report was written.

Read root/subtree AGENTS, takeover/import qualifications, research charter, CP209 headless-entry and independent outer-loader review, CP213 encoded Open plan, the retained actual-open contract, CP171 conditional auto-save review and candidate Open/selection/activation source. CP212's corrected transport qualification remains controlling: historical literal `-f` runs are malformed-input observations, not valid B98 Open failure or candidate parity evidence. This report derives control flow, not a new runtime outcome.

Independently compared addressed instruction prefixes with the copied PE: `wave3-boolean-external-callback-14002a0e0.asm` 432 prefixes, `...1400a7250.asm` 31, and `...140038380.asm` 136, all zero mismatches. This comparison checks addressed prefixes; it is not a claim to have separately decoded wrapped continuation bytes. Retained hashes are respectively `e3e74166115892e04fe23160955278cf3101abacb6d818cd41746bca94c43f63`, `3c0c2d933e0d5808fc5c5d48dc2b3e7c25ac31b91922c10ec1ea63c8ad26af2c`, and `d83ed1a61bee0279b68dc3058845ffe10ad3a1d2f63dac98d6a242f525e2f4ae`.

Fresh bounded disassembly covered `[14002a9b0,14002aad5)` (raw bytes SHA-256 `06f5ea36b228daef66c5e569f9ac046a4a27d063751a5dd525e651790a0851e8`) and `[140116ea0,140116eca)` (`0fc9190fcb5ee72b438d307bdea473133edfa401a70a5e6d949cf2b5a4e5a3c5`), plus `45870..458e1`. These are PE unwind regions, not assertions about entire transitive closures. The retained `evidence/gui_mo/s1/s1-01/dc/act1.c` corroborates the `2a9b0` interpretation.

## Nonnull arm and common tail

At entry, `M+3c0` is written zero (`2a107`); `13d40(M,0,false)` runs before the saved incoming Project pointer is tested at `2a120`. Its return is not used as an acceptance gate. The nonnull fallthrough is:

| Site | Decision or effect |
|---|---|
| `2a133..2a14e` | If `M+2e0` is nonnull, call `116ea0(owner,true)`. Its body follows owner+28 then +10 and calls imported `QWidget::setEnabled(bool)` at `116ebf`; this enables that child widget, not proof the whole application is ready. |
| `2a163` | `a5890(P)` obtains P's registered model backlink (`P+8` in retained instruction evidence). |
| `2a173..2a183` | Fresh `133a0(M)` reads/casts M+50, then `c79ee0(manager,child)` supplies an integer saved at stack+74. The retained actual-open contract identifies it as list ordinal, absent=-1, rather than model ID. No local check rejects -1. |
| `2a19c` | `2a9b0(M,&ordinal,&zero,true)` performs the continuation below. |
| `2a1a1` | Jump to common tail `2a94e`; bypass direct null-arm ProjectChanged, RefreshMainView and DiagEditorFitAll notifier sites. |
| `2a961` | Unconditional virtual M+2c0 call. Direct PE vtable read from Mo table `1420fb108` gives target `11020`. |
| `2a96f..2a98f` | Reread M+3c0 after earlier callees. If zero, virtual M+7f0(M,false); otherwise skip. Same Mo vtable gives `14260`. |
| `2a99e` | Return. CLI caller resumes at `1cae`, then recent-path processing `1dc00`, main display and event loop occur. |

M+3c0 starts zero but can be changed by intervening callbacks. Therefore the final false-argument refresh cannot be assumed solely from the entry write. Both virtual calls can invoke further effects before returning; a nonnull arm does not establish absence of all refreshes merely because it bypasses the particular null-arm notifier at `2a720`.

## What `2a9b0` actually completes

1. `2a9cd ->133a0`, then `2a9d5 ->45780` query the **old** current Project. Only when nonnull and singleton predicate `2cb870` returns true does `2a9f7` call auto-save `10b80`. A truly fresh null-current manager skips this old-current Save. Freshness must be witnessed or source-qualified, not inferred from a new filename.
2. Because this caller passes true, `2aa12 ->45870` runs. Its own fresh current lookup can skip its body if null; otherwise it follows old-current cleanup/reset helpers and calls `a72e0(P,-1)`. It does not imply unconditional deletion of the Project.
3. `2aa24 ->457f0` reads current ordinal. When different from the requested ordinal, `2aa48 ->45840` selects it, then `2aa5a` invokes M+2c0. Repeated `133a0` calls reread the live manager; callbacks separate these reads.
4. `2aa6d ->45780` obtains the **newly current** Project, independently of the incoming P. If null, `2aa7f` jumps to normal epilogue `2aad0`: no busy bracket, activation or view-binding body runs. Neither `2a9b0` nor `2a0e0` returns a tested success Boolean to main.
5. If nonnull, `2aa8a` sets singleton busy true; `2aa9c` calls `a7250(currentP,0)`; `2aaa8` calls `38380(M,true)`; `2aabc` calls virtual M+7f0(M,true); only afterward `2aacb` sets busy false. Fault/exception before the latter is not a completed bracket. The busy writes are overwritten state, not a demonstrated nesting counter or guaranteed restoration.

`a7250` calls `a7b90` at `a7288`, then engine-unit helper `917d60`, and conditionally synchronizes current operation through `47af0`. The retained `a7b90` body conditionally lazy-parses through `8f530`, binds a diagram through virtual+1b8, and invokes notifier `2cf3c0` at `a7ebe`/`a80bb`; exact PE literals are `2signalDoeTableRefresh()` and `2signalDoeOutputTableRefresh()`. Registration/invocation of a notifier does not establish receiver delivery.

`38380` freshly resolves current Project and tests `3f670`; only its admitted path with both view and active subproject available reaches the final view virtual+1b8 at `38625`. With true argument it first traverses items, compares mapped names and may call item setters through virtual methods. Function entry/return alone cannot certify nonnull model binding. Diagram binding can request delayed model-update work (retained CP82 timer qualification); synchronous completion is earlier than timer delivery.

The common false-argument `14260` is also consequential. CP171 traces a conditional route through `151a0 ->3a990 ->10b80 ->980d0` after the inner false-busy setter. Its process/current/visibility/busy guards and ignored Save result remain attached. Do not classify any Open-era file changes as user Save, or claim no Save can occur during Open. This bounded pass did not close every virtual target or callback descendant.

## Minimal runtime witnesses, by claim

These are requirements for a future separately reviewed observer, not authorization to modify or execute CP213's current runner. Keep same PID, primary TID, invocation depth/occurrence, M, P and return-site attribution; keep hook completeness and cleanup separate.

| Claim | Smallest useful additional witness beyond CP213 decoded-path/reader/outer-return records |
|---|---|
| Loader returned a Project | `1ca30` normal return P!=0. This is exactly the limited CP213 endpoint. |
| Synchronous postOpen completed | Bind incoming `(M,P)` at `2a0e0` to that loader occurrence and observe normal return to CLI `1cae`. Interior arrival at `2a94e` is insufficient; both common virtual callbacks may still fault. Do not interpret RAX from this void function as success. |
| Intended Project was selected and activation ran | Record ordinal/model backlink, actual newly current P at `2aa72` after `45780`, equality to the loader P, `a7250` input index zero and its normal return at `2aaa1`. This distinguishes the silent current-null skip. |
| Nonnull diagram binding happened | Record actual view/model arguments and normal return of the admitted `38380` final virtual+1b8 (`38625`, return `3862b`), under the same outer occurrence. A `38380` entry alone permits predicate exits. |
| PostOpen state is usable for the selected next stage | At the synchronized postOpen-return boundary record current-model/Project identity, active-subproject state, P+55 parser status, M+3c0 branch result, and actual busy state after the inner setter; record relevant callback/timer requests separately from delivery. No finite no-crash timeout proves all queued initialization complete. |

The minimal discriminator for **postOpen return versus outer return** is its correlated normal return at `1cae`. Add current-P equality plus activation return to support **activation completion**. None of these establishes successful Save/accepted Close or full scientific Open parity. P+55 is independent of accepted suffix/nonzero outer return; XML-read success needs its own witness.

## Candidate comparison and consequence

Current `src/core/convert.cpp:212..249` prepares/registers the model, constructs its backlink, calls `openProject`, then `commitProjectModelSelection` and `activateSubProject(p,0,ctx)`. The full-Save route intentionally accepts an admitted Project with `openedOk=false`; narrow adapters still reject that status. This preserves the needed distinction between admission and XML read status.

`ConversionSession::commitProjectModelSelection` selects the live child by identity/list ordinal and throws when absent. It does not itself express the original old-current branch, reset, manager rereads across callbacks, conditional null-current skip, nested busy bracket or common virtual tail. `activateSubProject` directly mutates `p.activeSub`, conditionally parses and marks loaded, then refreshes diagram items. Its comment that remaining effects are GUI with no MST effect is a historical source hypothesis, not evidence that those effects are irrelevant to the charter's full lifecycle fidelity.

`commitNonNullDiagramBinding` can record a session-owned binding and timer request; that representation alone does not establish this Open path calls it at every native-equivalent occurrence. No candidate code was changed here. Mapping missing boundaries requires original witnesses and callback/state evidence; adding null-arm RefreshMainView to every accepted candidate Open would encode the wrong branch. The reviewed encoded CP213 run can validly end at outer return and remain useful, provided its report does not promote that endpoint to postOpen/activation/Save/Close completion.
