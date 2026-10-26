param(
    [Parameter(Mandatory)][ValidateSet('Deploy','Restore')][string]$Action,
    [Parameter(Mandatory)][string]$TrialDirectory
)
$ErrorActionPreference='Stop'
$workspace=Split-Path -Parent $PSScriptRoot
$policyPath=Join-Path $workspace 'analysis\def-pre-deployment-policy.json'
if ((Test-Path -LiteralPath $policyPath) -and (Get-Content -LiteralPath $policyPath -Raw | ConvertFrom-Json).active) {
    throw 'Temporary substitution/automatic restoration superseded by user policy. Use deploy_latest_def_pre.ps1; originals require explicit user instruction to restore.'
}
$trial=[IO.Path]::GetFullPath($TrialDirectory)
$allowed=Join-Path $workspace 'build\substitution-trials'
if (-not $trial.StartsWith($allowed+'\',[StringComparison]::OrdinalIgnoreCase)) { throw 'Trial must be inside workspace build/substitution-trials' }
$journalPath=Join-Path $trial 'deployment.json'
$install='C:\Program Files\SFTC\DEFORM\v11.0.2'
$relative=@('DEF_PRE_64.exe','2D\DEF_PRE_64.exe','3D\DEF_PRE_64.exe')
$originalHash='F78A2A5C3EBB3C1AFFCA3D2D0EA31F0ED802F6FE1E9083168681502AC6CEA794'
function Hash([string]$p) { (Get-FileHash -LiteralPath $p -Algorithm SHA256).Hash }
function Save { $script:journal | ConvertTo-Json -Depth 6 | Set-Content -LiteralPath $journalPath -Encoding utf8 }
function Restore-All {
    foreach ($rel in $relative) {
        $target=Join-Path $install $rel
        $backup=$target+'.original'
        if (Test-Path -LiteralPath $backup) {
            if ((Hash $backup) -ne $originalHash) { throw "Original backup hash mismatch: $backup" }
            if (Test-Path -LiteralPath $target) {
                if ((Hash $target) -ne $script:journal.candidate_sha256) { throw "Unexpected active file: $target" }
                $quarantine=Join-Path $trial ('restored-candidates\'+$rel)
                New-Item -ItemType Directory -Path (Split-Path -Parent $quarantine) -Force | Out-Null
                if (Test-Path -LiteralPath $quarantine) { throw "Quarantine already exists: $quarantine" }
                Move-Item -LiteralPath $target -Destination $quarantine
            }
            Rename-Item -LiteralPath $backup -NewName 'DEF_PRE_64.exe'
        }
        if ((Hash $target) -ne $originalHash) { throw "Restoration verification failed: $target" }
        $stage=$target+'.trial-new'
        if (Test-Path -LiteralPath $stage) {
            if ((Hash $stage) -ne $script:journal.candidate_sha256) { throw "Unexpected staged file: $stage" }
            $saved=Join-Path $trial ('unused-stages\'+$rel)
            New-Item -ItemType Directory -Path (Split-Path -Parent $saved) -Force | Out-Null
            Move-Item -LiteralPath $stage -Destination $saved
        }
    }
    $script:journal.status='RESTORED'; Save
}
$busy=@(Get-CimInstance Win32_Process | Where-Object { $_.ExecutablePath -and $_.ExecutablePath.StartsWith($install+'\',[StringComparison]::OrdinalIgnoreCase) })
if ($busy.Count) { throw ('DEFORM installation is in use: '+(($busy | ForEach-Object { $_.Name+':'+$_.ProcessId }) -join ', ')) }
if ($Action -eq 'Restore') {
    $script:journal=Get-Content -LiteralPath $journalPath -Raw | ConvertFrom-Json
    if ($journal.installation_root -ne $install) { throw 'Wrong journal root' }
    Restore-All
    Write-Output 'All three originals restored and hash verified.'
    exit
}
if (Test-Path -LiteralPath $journalPath) { throw 'Existing deployment journal; inspect or restore it first' }
$candidate=Join-Path $trial 'candidate\DEF_PRE_64.exe'
$candidateHash=Hash $candidate
$actual=@(Get-ChildItem -LiteralPath $install -Recurse -Force -File -Filter DEF_PRE_64.exe | ForEach-Object { [IO.Path]::GetRelativePath($install,$_.FullName) })
if (@(Compare-Object ($relative | Sort-Object) ($actual | Sort-Object)).Count) { throw 'Installation inventory changed' }
$metadata=@()
foreach ($rel in $relative) {
    $target=Join-Path $install $rel
    if ((Hash $target) -ne $originalHash) { throw "Original hash mismatch: $target" }
    if ((Test-Path -LiteralPath ($target+'.original')) -or (Test-Path -LiteralPath ($target+'.trial-new'))) { throw "Existing backup/stage: $target" }
    $f=Get-Item -LiteralPath $target
    $metadata+=@{relative_path=$rel;sha256=$originalHash;bytes=$f.Length;last_write_utc=$f.LastWriteTimeUtc.ToString('o');sddl=(Get-Acl -LiteralPath $target).Sddl}
}
$script:journal=@{installation_root=$install;candidate_sha256=$candidateHash;originals=$metadata;status='PREPARED';operations=@()}
Save
try {
    foreach ($rel in $relative) {
        $target=Join-Path $install $rel
        $stage=$target+'.trial-new'
        Copy-Item -LiteralPath $candidate -Destination $stage
        Set-Acl -LiteralPath $stage -AclObject (Get-Acl -LiteralPath $target)
        if ((Hash $stage) -ne $candidateHash) { throw 'Staging hash mismatch' }
        $journal.operations+=@{relative_path=$rel;operation='rename_original_pending'}; Save
        Rename-Item -LiteralPath $target -NewName 'DEF_PRE_64.exe.original'
        if ((Hash ($target+'.original')) -ne $originalHash) { throw 'Renamed original hash mismatch' }
        Move-Item -LiteralPath $stage -Destination $target
        if ((Hash $target) -ne $candidateHash) { throw 'Installed candidate hash mismatch' }
        $journal.operations+=@{relative_path=$rel;operation='installed'}; Save
    }
    $journal.status='DEPLOYED'; Save
    Write-Output 'All three candidate paths and adjacent originals hash verified.'
} catch {
    $failure=$_
    $journal.status='FAILED'; Save
    Restore-All
    throw $failure
}
