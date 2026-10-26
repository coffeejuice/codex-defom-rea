# CP171: constructor E still does not establish first-Open Apply timing

The constructor and first-Open Apply/add questions remain separate. The current retained evidence contains no new same-invocation Boolean Object witness: it does not bind factory allocation E through Boolean constructor stages to the constructor notifier, nor bind a Boolean Apply add to a complete first-Open interval. Static order and later editor values cannot supply either occurrence.

This was a read-only source/evidence audit. Only this note was written. No source/test change, build, GUI/app control, original process, live REA session, or mutable analysis database was used. The findings are conditional on the earlier CP156/159/164 qualifications; this note does not claim a new original-runtime observation.

## What the two timings mean

The native Boolean factory's selected Object branch obtains a non-null `0x50` allocation E at `11e065b` and calls `11e2480(E, OPI)` at `11e0677`. The constructor enters the base wrapper, whose QWidget construction returns at `377990`; wrapper guard setup and `130830(E, OPI)` follow. The latter calls the canonical notifier at `130ad1`, whose return continuation is `130ad6`, before Boolean table publication at `11e24b7`, UI setup, or selection fill. A selected constructor notifier occurrence therefore needs the **same E and constructor generation**, consumed normal-return stages, and caller-filtered notifier occurrence. Existing Boolean class/editor values and the canonical Session notifier registry establish ownership/access only; they do not establish this constructor epoch.

The public first-Open effect is a different question: a complete fresh Open interval must contain a current-widget UpdateData invocation on that Boolean E, then Apply `11e2870`, resolution of E+38 to OPI O, and a normal add return at `11e295a` (Workpiece) or `11e2bcb` (Cutter), all with matching owner and invocation lifetimes. Constructor notification, postLoad's synchronous zero-slot creation (`11df0e9` / return `11df0ee`), editor fill, and CLI loader return `1c94` or postOpen return `1cae` do not prove the Apply-origin add. An Apply/add occurrence by itself also does not establish that it is first; that needs full-interval coverage and source-origin discrimination.

## Smallest useful source-bound probe without GUI control

The next safe probe is a **headless candidate-source trace test**, not a native capture. Keep it strictly observational: add no production behavior, no constructor admission, and no synthetic “native return” values. Using the existing Boolean lifecycle/editor and session fixtures, have the test drive explicitly labeled source-stage inputs through a trace recorder and assert that:

1. a constructor trace can only advance when the fixture supplies the matching Object-branch E allocation generation and each required normal-return stage (including QWidget continuation `377990`, guard/model setup, and notifier caller continuation `130ad6`); another editor's fill, a Forming300 frame, or a constructor return lacking the earlier stage must not qualify;
2. an Apply trace requires a separately supplied complete Open token, current-widget UpdateData-to-E identity, Apply entry, resolved OPI identity, and the normal add return; a `postLoad`-origin add is labeled separately;
3. events on different E generations, threads/intervals, or origins cannot be joined to manufacture either result, and a prefix or missing normal return remains incomplete.

This can test whether the **candidate source model and test seam preserve the required distinctions** without starting DEFORM or acquiring GUI control. It cannot prove original timing, establish that a real constructor or timer callback ran, or authorize production integration. If even a test-only trace seam would require mutating a shared lifecycle/fixture file, defer implementation until that file is released; this note requires no such edits.

The smallest source review before such a test is to inspect `src/ops/boolean.cpp`, the Boolean factory/lifecycle owner, the existing Apply trace owner, and their focused tests, then identify an already-owned test seam. Do not attach constructor E to `BooleanObjectEditor` text state or reuse the Forming300 factory owner. If no existing seam can represent generation plus ordered return stages without production changes, the source-bound probe is not currently available; retain the guards and ask for a fresh original witness only after app-control release is verifiably supported.

## Evidence limits and test plan

The nearest evidence remains:

- `resume164-boolean-next.md`: constructor-versus-first-Open separation, retained capture coverage result, required Object allocation/QWidget/setup/notifier chain, and full Open plus Apply/add acceptance conditions.
- `resume159-boolean-frame.md`: exact constructor frame and `377990` barrier; retained Editor004 is a **Forming** constructor capture and cannot be relabeled Boolean.
- `resume156-boolean-constructor-notifier.md`: `130830` caller argument suffix, nullable payload handling, and notifier continuation `130ad6`; it does not provide an actual Boolean constructor invocation.
- `resume120-boolean-first-open-order.md`: existing tree/delegate evidence does not establish Boolean Apply during Open; full interval and actual Apply/add chain remain necessary.

Those are selected retained reports, not fresh exhaustive binary or runtime surveys. The prior checker result described by CP164 is a negative coverage result for the retained material, not proof that the original path never executes. No reconstructed `DEF_PRE_64` instance is discussed here; no version claim applies.

If the source-only probe is approved and the relevant files are available, the focused test plan is: **(a)** incomplete constructor prefix; **(b)** wrong E generation; **(c)** missing QWidget return; **(d)** notifier with wrong caller continuation; **(e)** valid ordered constructor trace; **(f)** Apply add without complete Open token; **(g)** valid Apply-origin add; **(h)** postLoad-origin add kept distinct; **(i)** mismatched E/OPI identities and incomplete add returns. These are model-contract checks only. Later native acceptance still needs the CP164 same-invocation constructor witness and, separately, the complete fresh Open/UpdateData/Apply/OPI/add chain with healthy capture coverage and independent input/probe identity checks.
