# What DEF_GUI23 starts on Run (preliminary)

DEFORM v11.0.2. Static evidence only: strings of the installed exes (read in
place, nothing modified) and copies of the leftover files in
`C:\PROBLEM\TEST_REA\COG_P1_B1`. No call sites were decompiled yet, so argument
order inside each format string is read from the strings, not from code.
Licence and process-monitor code was not looked at.

## Chain for a normal 3D Run

```
DEF_GUI23.exe
  └─ DEF_SIMULATION.EXE            (job wrapper; job info passed in env vars)
       └─ 3D\DEF_ARM_CTL3.COM <prob> <c>   (batch controller, one per problem)
            ├─ [MST present] DEF_RDMS3.EXE "<prob>" ["<n>"]   (parse master file)
            ├─ [MST present] DEF_PRE.EXE                     (rebuild DEF_PRE.DB per sub-run)
            └─ 3D\DEF_ARM.COM "<prob>" B >> "<prob>.LOG"     (auto-remesh driver, per run)
                 ├─ DEF_MPI_CTL[_64].COM DEF_MPIenv.DAT DEF_SIM > "<prob>.MSG"
                 │     └─ N x DEF_SIM[_64].EXE   (FEM solver, N from DEF_MPIenv.DAT)
                 │   or DEF_SIM[_64].EXE > "<msg>"  (serial)
                 ├─ remesh: DEF_MPI_CTL.COM DEF_MPIenv.DAT DEF_REM  or  DEF_REM[_64].EXE < DEF_INT3.INP
                 ├─ local remesh: DEF_LOCREM3.EXE / DEF_MPI_CTL.COM ... DEF_LOC_REM
                 ├─ mesh gen: DEF_AMG[_64].EXE < DEF_AMG3.INP, AMG_DXT[_64].EXE
                 ├─ DB ops: DEF_PRE[_64].EXE < DEF_PRE3.INP > DEF_PRE3.OUT
                 └─ misc: DEF_BRK2, DEF_CMESH, DEF_SPLDB
```

2D uses the same shape with `2D\DEF_ARM_CTL.COM <prob> <c>` and `2D\DEF_ARM.COM`.
Each `.COM` file is a normal PE exe (it starts with `MZ`).

## What each one does

| App | Role | Evidence |
|---|---|---|
| DEF_GUI23.exe | Main GUI. Builds the command and starts DEF_SIMULATION. Adds the env vars `ENV_JOB_TYPE_TAG=E_MO_JOB`, `ENV_JOB_DIMENSION_TAG=`, `ENV_RUNNING_DIRCTORY_TAG=`, `ENV_PROBLEM_ID_TAG=`. Holds the driver formats `DEF_ARM_CTL3.COM %s %c`, `DEF_ARM_CTL.COM %s %c`, `DEF_CTRL.COM %s`, `DEF_MACHDIST_MST23.EXE %s` | strings at 0x2df9dc to 0x2dfad8; uses CreateProcessA and QProcess |
| DEF_SIMULATION.exe | Job wrapper: reads those env vars, gets a job id from the job manager, picks the job type (`E_MO_JOB`, `E_DOE_JOB` → DEF_DOE.EXE, `E_OPT_JOB` → DEF_OPT/DEF_OPT3.EXE), starts the controller and waits, then cleans up. Sets `SPAWN_BY_DEF_SIMULATION_EXE` for the child | `CRunSimulation::*` strings; `DEF_SIMULATION.log` shows `Start process: ...\3D\DEF_ARM_CTL3.COM` |
| DEF_ARM_CTL3.COM / DEF_ARM_CTL.COM | Batch controller. Handles the MST multi-run loop, calls DEF_ARM per run, then batch report (DEF_TEXT_PST.EXE) and FILE_REMOVE.EXE | `"MASTER FILE PRESENT"`, `DEF_RDMS3.EXE" "%s" "%d"`, `DEF_ARM.COM" "%s" B >> "%s"`, `.DEF_ARM_CTL_LOG` |
| DEF_ARM.COM | Automatic remeshing driver: loops solver → remesh → solver until the stop condition. Writes `<prob>.LOG` | `.LOG` line 1 is `...\3D\DEF_ARM.COM COG_P1_B1 B` |
| DEF_MPI_CTL[_64].COM | Home-made MPICH launcher. Reads `DEF_MPIenv.DAT` (`-env MPICH_SHMSIZE=…`, `SIM2 4`), starts N copies of DEF_SIM/DEF_REM/DEF_LOCREM3 with `MPICH_JOBID/NPROC/ROOTHOST/IPROC` env, records them in `DefMpi.tmp` | DefMpi.tmp lists 4 `Sim2` PIDs; `.MSG`: "4 processors are found from DEF_MPIenv.DAT" |
| DEF_SIM[_64].EXE | FEM solver, one step batch per call | SIMUL.MSG, `<prob>.MSG` |
| DEF_REM, DEF_LOCREM3, DEF_AMG, AMG_DXT | Remesh, local remesh, mesh generation | DEF_ARM strings |
| DEF_PRE[_64].EXE | Batch preprocessor, run with stdin scripts for DB work | DEF_ARM and ARM_CTL3 strings |

## Other Run paths

- **Cogging / ring-rolling style (COG_SIM_INPUT.INI present):** the GUI uses
  `3D\DEF_CTRL.COM <prob>`. DEF_CTRL starts `COG_CTRL3.EXE` (not in the 3D folder;
  DEF_CTRL also looks up the Python App Path, so this step may be Python) to stage
  each bite, then
  `DEF_ARM.COM < DEF_RMSH.DAT > <log>` per bite.
- **Machining distortion:** `DEF_MACHDIST_MST23.EXE <arg>` (not shipped under that
  name; `DEF_MACHDIST_CTRL23.exe` is).
- **MST file (DEFORM.MST / `<prob>.MST`):** the same GUI → DEF_SIMULATION →
  ARM_CTL3 path. ARM_CTL3 sees the master file, runs `DEF_RDMS3.EXE "<prob>" "<n>"`
  to unpack run n, `DEF_PRE.EXE` to rebuild `DEF_PRE.DB`/`.TDB`, then
  `DEF_ARM.COM "<prob>" B`; it logs "DEF_ARM.COM HAS BEEN SKIPPED. NEXT
  SIMULATION IS %d" for skipped runs. DEF_SIMULATION also mentions
  `DEF_RDMS.EXE` and `DEF_MULTI.INI`, so part of this may sit in the wrapper.
- **Batch queue (several problems, needs Tools licence):** GUI starts
  `BATCH_QUEUE.EXE`, which uses `_spawnv` to run the problems one after another.
- **Run (options) / job groups:** `DEFJobGroupSetup.exe -pp %s -pid %s -dim %d
  -jt MO` writes `JobGroupSetupConfigureFile.txt` (user, simultaneous runs, cores,
  `JobRunMode=E_RUN_ON_LOCALLY`), which DEF_SIMULATION reads. `CheckMPI2.EXE` and
  `mpiexec.exe` relate to the MPICH2 cluster option.

## Open points

- What the `%c` argument of ARM_CTL3 is (the log shows only the exe path).
  Probably the Interactive/Batch mode letter; confirm at the CreateProcess call site.
- Exact split of the MST loop between DEF_SIMULATION and ARM_CTL3.
- Which GUI menu item maps to which branch (Run vs Run (options)).
