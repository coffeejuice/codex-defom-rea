param([Parameter(Mandatory)][string]$TrialDirectory)
$ErrorActionPreference='Stop'
throw 'This temporary trial runner restores originals automatically and is disabled by the current user deployment rule.'
Set-Location (Split-Path -Parent $PSScriptRoot)
$transcript=Join-Path $TrialDirectory 'elevated-trial.log'
Start-Transcript -LiteralPath $transcript | Out-Null
$result=1
try {
    & "$PSScriptRoot\substitute_def_pre.ps1" -Action Deploy -TrialDirectory $TrialDirectory
    python "$PSScriptRoot\test_def_pre_substitution.py" installed --trial $TrialDirectory
    if ($LASTEXITCODE -ne 0) { throw 'Installed comparison failed; see installed.json' }
    $result=0
} catch {
    Write-Output $_.ToString()
} finally {
    try {
        $journal=Join-Path $TrialDirectory 'deployment.json'
        if (Test-Path -LiteralPath $journal) {
            $state=Get-Content -LiteralPath $journal -Raw | ConvertFrom-Json
            if ($state.status -ne 'RESTORED') {
                & "$PSScriptRoot\substitute_def_pre.ps1" -Action Restore -TrialDirectory $TrialDirectory
            }
        }
    } catch { Write-Output $_.ToString(); $result=2 }
    @{exit_code=$result;completed_utc=[DateTime]::UtcNow.ToString('o')} | ConvertTo-Json | Set-Content -LiteralPath (Join-Path $TrialDirectory 'trial-exit.json')
    Stop-Transcript | Out-Null
}
exit $result
