param(
    [Parameter(Mandatory = $true)]
    [ValidatePattern('^[0-9a-f]{32}$')]
    [string]$OccurrenceId
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'
$adminDirectory = [IO.Path]::GetFullPath($PSScriptRoot)
$converterDirectory = [IO.Path]::GetFullPath((Join-Path $adminDirectory '..'))
$analysisDirectory = [IO.Path]::GetFullPath((Join-Path $converterDirectory 'analysis'))
$childReceipt = Join-Path $analysisDirectory ('cp372-receipt-' + $OccurrenceId + '.json')
$childOutput = Join-Path $analysisDirectory ('cp372-child-' + $OccurrenceId + '.json')
$monitor = Join-Path $analysisDirectory ('cp372-monitor-' + $OccurrenceId + '.json')
if ([IO.File]::Exists($childReceipt) -or [IO.File]::Exists($childOutput) -or
    [IO.File]::Exists($monitor)) {
    throw 'A CP372 child or monitor path already exists; this occurrence cannot be retried.'
}

$pythonImage = 'C:\Program Files\Python312\python.exe'
if (-not [IO.File]::Exists($pythonImage) -or $adminDirectory.Contains("'")) {
    throw 'The fixed Python image or admin path is unavailable.'
}
$code = @"
import sys
sys.path.insert(0,r'$adminDirectory')
import cp350_native_trial as pre
import cp325_real_bindings as bind
import cp372_startup_only_probe as probe
before=pre._preflight()
owner=None
result=None
try:
    result=probe._run_once(bind.load_native_exports(),occurrence_id=sys.argv[1])
except probe.ProbeFailure as failure:
    owner=failure.owner
    result=failure.receipt
finally:
    after=pre._preflight()
    equal=before==after
    print(__import__('json').dumps({'source_manifests_equal':equal,'before':before['admin_modules'],'after':after['admin_modules'],'probe_passed':bool(result and result.get('probe_passed') is True)},sort_keys=True,separators=(',',':')))
    sys.exit(0 if result and result.get('probe_passed') is True and equal else 1)
"@
$code = $code.Trim()
$startInfo = [Diagnostics.ProcessStartInfo]::new()
$startInfo.FileName = $pythonImage
$startInfo.WorkingDirectory = $converterDirectory
$startInfo.UseShellExecute = $false
$startInfo.CreateNoWindow = $true
$startInfo.RedirectStandardOutput = $true
$startInfo.RedirectStandardError = $true
foreach ($arg in @('-I', '-B', '-c', $code, $OccurrenceId)) { [void]$startInfo.ArgumentList.Add($arg) }

$startedUtc = [DateTime]::UtcNow.ToString('o')
$process = $null; $state = 'launch-not-attempted'; $exitCode = $null
$killOutcome = 'not-needed'; $exactProcessSignaled = $false
$stdout = $null; $stderr = $null; $stdoutTask = $null; $stderrTask = $null
$stdoutTerminal = $false; $stderrTerminal = $false; $errorText = $null
try {
    $state = 'launch-entered-outcome-unknown'
    $process = [Diagnostics.Process]::Start($startInfo)
    if ($null -eq $process) { throw 'Process.Start returned no exact process owner.' }
    $stdoutTask = $process.StandardOutput.ReadToEndAsync()
    $stderrTask = $process.StandardError.ReadToEndAsync()
    $state = 'running'
    if ($process.WaitForExit(15000)) {
        $exactProcessSignaled = $true; $exitCode = $process.ExitCode
        $state = if ($exitCode -eq 0) { 'exact-process-exit-zero-report-still-to-check' } else { 'exact-process-exit-nonzero' }
    } else {
        $state = 'outer-timeout-cleanup-uncertain'
        try {
            if (-not $process.HasExited) { $process.Kill(); $killOutcome = 'exact-process-kill-returned' }
            $exactProcessSignaled = $process.WaitForExit(5000)
            if ($exactProcessSignaled) { $exitCode = $process.ExitCode }
        } catch { $killOutcome = 'exact-process-kill-or-wait-error: ' + $_.Exception.Message }
    }
} catch {
    $errorText = $_.Exception.Message; $state = 'monitor-error-cleanup-uncertain'
    if ($null -ne $process) {
        try {
            if (-not $process.HasExited) { $process.Kill(); $killOutcome = 'exact-process-kill-returned-after-error' }
            $exactProcessSignaled = $process.WaitForExit(5000)
            if ($exactProcessSignaled) { $exitCode = $process.ExitCode }
        } catch { $killOutcome = 'exact-process-kill-or-wait-error: ' + $_.Exception.Message }
    }
} finally {
    foreach ($item in @(@('stdout', $stdoutTask), @('stderr', $stderrTask))) {
        if ($null -ne $item[1]) {
            try {
                if ($item[1].Wait(1000)) {
                    if ($item[0] -eq 'stdout') { $stdout = $item[1].Result; $stdoutTerminal = $true }
                    else { $stderr = $item[1].Result; $stderrTerminal = $true }
                }
            } catch { $errorText = $item[0] + ' collection: ' + $_.Exception.Message }
        }
    }
    if ($null -ne $process) { $process.Dispose() }
}

$receipt = [ordered]@{
    kind = 'cp372-exact-outer-monitor'; occurrence_id = $OccurrenceId
    started_utc = $startedUtc; ended_utc = [DateTime]::UtcNow.ToString('o')
    state = $state; exact_process_signaled = $exactProcessSignaled; exit_code = $exitCode
    kill_outcome = $killOutcome; child_receipt_exists = [IO.File]::Exists($childReceipt)
    stdout_terminal = $stdoutTerminal; stderr_terminal = $stderrTerminal
    stdout = $stdout; stderr = $stderr; error = $errorText
    source_manifests_equal = $false; probe_passed = $false
}
if ($state -eq 'exact-process-exit-zero-report-still-to-check' -and
    (-not $stdoutTerminal -or -not $stderrTerminal -or $null -ne $errorText)) {
    $receipt.state = 'exact-process-zero-but-output-drain-uncertain'
}
if ($state -eq 'exact-process-exit-zero-report-still-to-check' -and
    $receipt.child_receipt_exists -and $stdoutTerminal -and $stderrTerminal -and $null -eq $errorText) {
    try {
        $child = Get-Content -LiteralPath $childReceipt -Raw | ConvertFrom-Json
        $controller = $stdout | ConvertFrom-Json
        $receipt.source_manifests_equal = ($controller.source_manifests_equal -eq $true)
        $receiptPathMatches = [IO.Path]::GetFullPath([string]$child.receipt_path).Equals(
            $childReceipt, [StringComparison]::OrdinalIgnoreCase)
        $childPathMatches = [IO.Path]::GetFullPath([string]$child.child_path).Equals(
            $childOutput, [StringComparison]::OrdinalIgnoreCase)
        if ($child.schema -eq 'cp372-parent-v1' -and
            $child.occurrence_id -ceq $OccurrenceId -and
            $child.image -ceq $pythonImage -and
            $receiptPathMatches -and $childPathMatches -and
            $controller.probe_passed -eq $true -and
            $child.probe_passed -eq $true -and $child.cleanup -eq 'closed' -and
            $receipt.source_manifests_equal) {
            $receipt.probe_passed = $true; $receipt.state = 'exact-process-and-child-receipt-accepted'
        } else { $receipt.state = 'exact-process-zero-but-child-receipt-unaccepted' }
    } catch { $receipt.state = 'exact-process-zero-but-child-receipt-invalid'; $receipt.error = $_.Exception.Message }
}
$json = $receipt | ConvertTo-Json -Depth 6
$stream = [IO.File]::Open($monitor, [IO.FileMode]::CreateNew, [IO.FileAccess]::Write, [IO.FileShare]::None)
try { $writer = [IO.StreamWriter]::new($stream, [Text.UTF8Encoding]::new($false)); try { $writer.Write($json); $writer.Flush(); $stream.Flush($true) } finally { $writer.Dispose() } }
finally { $stream.Dispose() }
Write-Output $json
if (-not $receipt.probe_passed) { exit 1 }
