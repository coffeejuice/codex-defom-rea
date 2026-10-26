param(
    [Parameter(Mandatory=$true)][int]$OwnedPid,
    [Parameter(Mandatory=$true)][ValidatePattern('^H[0-9]+$')][string]$TrialId,
    [ValidateRange(1,60)][int]$DeadlineSeconds=45
)
$ErrorActionPreference='Stop'
$expected='C:\Program Files\SFTC\DEFORM\v11.0.2\DEFJobGroupSetup.exe'
$repoRoot=Split-Path -Parent $PSScriptRoot
$report=Join-Path $repoRoot "analysis\helper-$TrialId-supervisor.json"
if(Test-Path -LiteralPath $report){throw 'Refusing to overwrite a trial supervisor receipt'}
$cim=Get-CimInstance Win32_Process -Filter "ProcessId=$OwnedPid"
if($null -eq $cim -or $cim.ExecutablePath -ne $expected){throw 'Trial image identity mismatch'}
$proc=Get-Process -Id $OwnedPid
$started=$proc.StartTime.ToUniversalTime()
$record=[ordered]@{pid=$OwnedPid;parent_pid=$cim.ParentProcessId;path=$cim.ExecutablePath;command_line=$cim.CommandLine;creation_utc=$started.ToString('o');armed_utc=[DateTime]::UtcNow.ToString('o');deadline_seconds=$DeadlineSeconds;gui_control_acquired=$false}
$record | ConvertTo-Json -Depth 5 | Set-Content -LiteralPath $report
$timedOut=-not $proc.WaitForExit($DeadlineSeconds*1000)
if($timedOut){
    $current=Get-Process -Id $OwnedPid -ErrorAction SilentlyContinue
    if($current -and $current.StartTime.ToUniversalTime() -eq $started -and $current.Path -eq $expected){
        Stop-Process -Id $OwnedPid -Force
        $record['termination']='trial-owned deadline'
        $proc.WaitForExit(5000) | Out-Null
    }else{$record['termination']='identity changed; no kill'}
}else{$record['termination']='exited independently'}
$record['timed_out']=$timedOut
$record['exit_code']=if($proc.HasExited){$proc.ExitCode}else{$null}
$record['still_running']=@((Get-Process -Id $OwnedPid -ErrorAction SilentlyContinue)).Count -gt 0
$record['finished_utc']=[DateTime]::UtcNow.ToString('o')
$record | ConvertTo-Json -Depth 5 | Set-Content -LiteralPath $report
$record | ConvertTo-Json -Compress
