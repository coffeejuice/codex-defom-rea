param(
    [string]$Candidate,
    [switch]$Elevated
)
$ErrorActionPreference='Stop'
$workspace=Split-Path -Parent $PSScriptRoot
if (-not $Candidate) { $Candidate=Join-Path $workspace 'build\claude-def-pre\Release\DEF_PRE_64.exe' }
$Candidate=(Resolve-Path -LiteralPath $Candidate).Path
if ([IO.Path]::GetFileName($Candidate) -cne 'DEF_PRE_64.exe') { throw 'Candidate must be named DEF_PRE_64.exe' }
$identity=[Security.Principal.WindowsIdentity]::GetCurrent()
$principal=[Security.Principal.WindowsPrincipal]::new($identity)
if (-not $principal.IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)) {
    if ($Elevated) { throw 'Administrator rights were not granted' }
    $shell=(Get-Process -Id $PID).Path
    $arguments=@('-NoProfile','-File',('"'+$PSCommandPath+'"'),'-Candidate',('"'+$Candidate+'"'),'-Elevated')
    $process=Start-Process -FilePath $shell -ArgumentList $arguments -Verb RunAs -WindowStyle Hidden -Wait -PassThru
    if ($process.ExitCode -ne 0) { throw "Elevated deployment failed: exit $($process.ExitCode). Originals will not be restored automatically." }
    Get-Content -LiteralPath (Join-Path $workspace 'analysis\def-pre-deployment-state.json') -Raw | Write-Output
    exit 0
}
$install='C:\Program Files\SFTC\DEFORM\v11.0.2'
$relative=@('DEF_PRE_64.exe','2D\DEF_PRE_64.exe','3D\DEF_PRE_64.exe')
$originalHash='F78A2A5C3EBB3C1AFFCA3D2D0EA31F0ED802F6FE1E9083168681502AC6CEA794'
function Hash([string]$Path) { (Get-FileHash -LiteralPath $Path -Algorithm SHA256).Hash }
$candidateHash=Hash $Candidate
$candidateVersion=(Get-Item -LiteralPath $Candidate).VersionInfo.FileVersion
if ($candidateVersion -notmatch '^\d+\.\d+\.\d+$') { throw 'Candidate has no valid semantic file version' }
$policy=Get-Content -LiteralPath (Join-Path $workspace 'analysis\def-pre-deployment-policy.json') -Raw | ConvertFrom-Json
if (-not $policy.active) { throw 'Standing deployment policy is disabled' }
$busy=@(Get-Process -Name DEF_PRE_64 -ErrorAction SilentlyContinue)
if ($busy.Count) { throw 'DEF_PRE_64 is running; deployment deferred. No process terminated.' }
$actual=@(Get-ChildItem -LiteralPath $install -Recurse -Force -File -Filter DEF_PRE_64.exe | ForEach-Object { [IO.Path]::GetRelativePath($install,$_.FullName) })
if (@(Compare-Object ($relative | Sort-Object) ($actual | Sort-Object)).Count) { throw 'Installation target inventory changed; inspect before deploying' }
$statePath=Join-Path $workspace 'analysis\def-pre-deployment-state.json'
$previous=$null
if (Test-Path -LiteralPath $statePath) { $previous=Get-Content -LiteralPath $statePath -Raw | ConvertFrom-Json }
$receipt=@{status='PREPARING';utc=[DateTime]::UtcNow.ToString('o');candidate=$Candidate;candidate_version=$candidateVersion;candidate_sha256=$candidateHash;targets=@();automatic_restore=$false}
$journal=Join-Path $workspace ('build\deployments\'+[DateTime]::UtcNow.ToString('yyyyMMdd-HHmmss-ffff')+'.json')
New-Item -ItemType Directory -Path (Split-Path -Parent $journal) -Force | Out-Null
function Save { $receipt | ConvertTo-Json -Depth 6 | Set-Content -LiteralPath $journal -Encoding utf8 }
Save
try {
    foreach ($rel in $relative) {
        $target=Join-Path $install $rel
        $backup=$target+'.original'
        $hash=Hash $target
        if (Test-Path -LiteralPath $backup) {
            if ((Hash $backup) -ne $originalHash) { throw "Original backup mismatch: $backup" }
            if ($hash -ne $candidateHash -and $hash -ne $previous.candidate_sha256) { throw "Unrecognized active executable: $target" }
        } elseif ($hash -ne $originalHash) { throw "Original backup is missing: $target" }
        if ((Test-Path -LiteralPath ($target+'.new')) -and (Hash ($target+'.new')) -ne $candidateHash) { throw "Different staging file already exists: $target.new" }
    }
    # Stage and verify every candidate before any original is renamed.
    foreach ($rel in $relative) {
        $target=Join-Path $install $rel
        if (-not (Test-Path -LiteralPath ($target+'.new'))) { Copy-Item -LiteralPath $Candidate -Destination ($target+'.new') }
        Set-Acl -LiteralPath ($target+'.new') -AclObject (Get-Acl -LiteralPath $target)
        if ((Hash ($target+'.new')) -ne $candidateHash) { throw 'Staging verification failed' }
    }
    $receipt.status='STAGED'; Save
    foreach ($rel in $relative) {
        $target=Join-Path $install $rel
        $backup=$target+'.original'
        if (-not (Test-Path -LiteralPath $backup)) {
            Rename-Item -LiteralPath $target -NewName 'DEF_PRE_64.exe.original'
            [IO.File]::Move($target+'.new',$target)
        } else {
            $previousReconstruction=$journal+'.'+$rel.Replace('\','_')+'.previous'
            [IO.File]::Replace($target+'.new',$target,$previousReconstruction)
        }
        if ((Hash $target) -ne $candidateHash -or (Hash $backup) -ne $originalHash) { throw 'Installed hash verification failed' }
        $version=(& $target --port-version | Out-String).Trim()
        if ($LASTEXITCODE -ne 0 -or $version -notlike "def_pre port $candidateVersion *") { throw "Version smoke check failed: $target" }
        if ((Get-Item -LiteralPath $target).VersionInfo.FileVersion -ne $candidateVersion) { throw 'Installed file version mismatch' }
        $receipt.targets+=@{path=$target;sha256=(Hash $target);backup=$backup;original_sha256=(Hash $backup);version=$version;file_version=$candidateVersion}
        Save
    }
    $receipt.status='DEPLOYED_KEEP_RECONSTRUCTED'; $receipt.utc=[DateTime]::UtcNow.ToString('o'); Save
    $receipt | ConvertTo-Json -Depth 6 | Set-Content -LiteralPath $statePath -Encoding utf8
    Write-Output "Reconstructed DEF_PRE_64 v$candidateVersion installed at all three paths. SHA256=$candidateHash"
} catch {
    $receipt.status='DEPLOYMENT_FAILED_NO_AUTOMATIC_RESTORE'; $receipt.error=$_.ToString(); Save
    throw
}
