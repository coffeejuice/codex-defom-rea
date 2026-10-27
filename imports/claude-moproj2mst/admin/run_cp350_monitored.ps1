param(
    [Parameter(Mandatory = $true)]
    [string]$ReportPath
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'
$adminDirectory = [IO.Path]::GetFullPath($PSScriptRoot)
$converterDirectory = [IO.Path]::GetFullPath((Join-Path $adminDirectory '..'))
$analysisDirectory = [IO.Path]::GetFullPath((Join-Path $converterDirectory 'analysis'))
$report = [IO.Path]::GetFullPath($ReportPath)
$match = [regex]::Match([IO.Path]::GetFileName($report), '^cp350-inert-trial-([0-9a-f]{32})\.json$')
if (-not $match.Success -or
    -not [IO.Path]::GetDirectoryName($report).Equals($analysisDirectory, [StringComparison]::OrdinalIgnoreCase) -or
    [IO.File]::Exists($report)) {
    throw 'A fresh fixed CP350 report path under converter analysis is required.'
}
$monitor = Join-Path $analysisDirectory ('cp350-monitor-' + $match.Groups[1].Value + '.json')
if ([IO.File]::Exists($monitor)) {
    throw 'The CP350 monitor receipt already exists.'
}

$pythonImage = 'C:\Program Files\Python312\python.exe'
if (-not [IO.File]::Exists($pythonImage) -or $adminDirectory.Contains("'")) {
    throw 'The fixed Python image or admin path is unavailable.'
}
$code = "import sys;sys.path.insert(0,r'$adminDirectory');import cp350_native_trial as t;r=t.run_once(sys.argv[1]);sys.exit(0 if r.accepted else 1)"
$startInfo = [Diagnostics.ProcessStartInfo]::new()
$startInfo.FileName = $pythonImage
$startInfo.WorkingDirectory = $converterDirectory
$startInfo.UseShellExecute = $false
$startInfo.CreateNoWindow = $true
$startInfo.RedirectStandardOutput = $true
$startInfo.RedirectStandardError = $true
foreach ($arg in @('-I', '-B', '-c', $code, $report)) {
    [void]$startInfo.ArgumentList.Add($arg)
}

$startedUtc = [DateTime]::UtcNow.ToString('o')
$process = $null
$state = 'launch-not-attempted'
$exitCode = $null
$killOutcome = 'not-needed'
$exactProcessSignaled = $false
$stdout = $null
$stderr = $null
$stdoutTask = $null
$stderrTask = $null
$stdoutTerminal = $false
$stderrTerminal = $false
$errorText = $null
try {
    $state = 'launch-entered-outcome-unknown'
    $process = [Diagnostics.Process]::Start($startInfo)
    if ($null -eq $process) {
        throw 'Process.Start returned no exact process owner.'
    }
    # Drain concurrently; synchronous ReadToEnd can outlive the controller if
    # an owned descendant still holds a redirected stream open.
    $stdoutTask = $process.StandardOutput.ReadToEndAsync()
    $stderrTask = $process.StandardError.ReadToEndAsync()
    $state = 'running'
    if ($process.WaitForExit(20000)) {
        $exactProcessSignaled = $true
        $exitCode = $process.ExitCode
        $state = if ($exitCode -eq 0) { 'exact-process-exit-zero-report-still-to-check' } else { 'exact-process-exit-nonzero' }
    } else {
        $state = 'outer-timeout-cleanup-uncertain'
        try {
            if (-not $process.HasExited) {
                $process.Kill()
                $killOutcome = 'exact-process-kill-returned'
            }
            $exactProcessSignaled = $process.WaitForExit(5000)
            if ($exactProcessSignaled) {
                $exitCode = $process.ExitCode
            }
        } catch {
            $killOutcome = 'exact-process-kill-or-wait-error: ' + $_.Exception.Message
        }
    }
} catch {
    $errorText = $_.Exception.Message
    $state = 'monitor-error-cleanup-uncertain'
    if ($null -ne $process) {
        try {
            if (-not $process.HasExited) {
                $process.Kill()
                $killOutcome = 'exact-process-kill-returned-after-error'
            }
            $exactProcessSignaled = $process.WaitForExit(5000)
            if ($exactProcessSignaled) { $exitCode = $process.ExitCode }
        } catch {
            $killOutcome = 'exact-process-kill-or-wait-error: ' + $_.Exception.Message
        }
    }
} finally {
    if ($null -ne $stdoutTask) {
        try {
            if ($stdoutTask.Wait(1000)) {
                $stdout = $stdoutTask.Result
                $stdoutTerminal = $true
            }
        }
        catch { $errorText = 'stdout collection: ' + $_.Exception.Message }
    }
    if ($null -ne $stderrTask) {
        try {
            if ($stderrTask.Wait(1000)) {
                $stderr = $stderrTask.Result
                $stderrTerminal = $true
            }
        }
        catch { $errorText = 'stderr collection: ' + $_.Exception.Message }
    }
    if ($null -ne $process) { $process.Dispose() }
}

$receipt = [ordered]@{
    kind = 'cp350-exact-outer-monitor'
    report_path = $report
    started_utc = $startedUtc
    ended_utc = [DateTime]::UtcNow.ToString('o')
    state = $state
    exact_process_signaled = $exactProcessSignaled
    exit_code = $exitCode
    kill_outcome = $killOutcome
    report_exists = [IO.File]::Exists($report)
    stdout_terminal = $stdoutTerminal
    stderr_terminal = $stderrTerminal
    stdout = $stdout
    stderr = $stderr
    error = $errorText
    trial_accepted = $false
}
if ($state -eq 'exact-process-exit-zero-report-still-to-check' -and
    (-not $stdoutTerminal -or -not $stderrTerminal -or $null -ne $errorText)) {
    $receipt.state = 'exact-process-zero-but-output-drain-uncertain'
}
if ($state -eq 'exact-process-exit-zero-report-still-to-check' -and
    $receipt.report_exists -and $stdoutTerminal -and $stderrTerminal -and
    $null -eq $errorText) {
    try {
        $trial = Get-Content -LiteralPath $report -Raw | ConvertFrom-Json
        if ($trial.accepted -eq $true) {
            $receipt.trial_accepted = $true
            $receipt.state = 'exact-process-and-report-accepted'
        } else {
            $receipt.state = 'exact-process-zero-but-report-unaccepted'
        }
    } catch {
        $receipt.state = 'exact-process-zero-but-report-invalid'
        $receipt.error = $_.Exception.Message
    }
}
$json = $receipt | ConvertTo-Json -Depth 6
$stream = [IO.File]::Open($monitor, [IO.FileMode]::CreateNew,
    [IO.FileAccess]::Write, [IO.FileShare]::None)
try {
    $writer = [IO.StreamWriter]::new($stream, [Text.UTF8Encoding]::new($false))
    try { $writer.Write($json); $writer.Flush() }
    finally { $writer.Dispose() }
} finally {
    $stream.Dispose()
}
Write-Output $json
if (-not $receipt.trial_accepted) { exit 1 }
