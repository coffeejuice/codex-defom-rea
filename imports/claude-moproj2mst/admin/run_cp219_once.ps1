$ErrorActionPreference = 'Stop'
$adminDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$projectDir = Split-Path -Parent $adminDir
$reviewed = Join-Path $adminDir 'original_uninstrumented_directory_compare.mjs'
$live = Join-Path $adminDir '.cp219-live-once.mjs'
$console = Join-Path $projectDir 'analysis\cp219-launch-console.txt'
$executed = Join-Path $projectDir 'analysis\cp219-executed-runner.txt'
$receipt = Join-Path $projectDir 'analysis\cp219-launch-receipt.json'
$expected = '84370534E343D03DEE21168A9DA3F77D0CD2BBF180FDDB3515F7C6E1B9C20467'

if ((Get-FileHash -LiteralPath $reviewed -Algorithm SHA256).Hash -ne $expected) {
    throw 'Reviewed runner hash changed'
}
foreach ($target in @('C:\PROBLEM\p217b', 'C:\PROBLEM\p217n')) {
    if (Test-Path -LiteralPath $target) { throw "Disposable target already exists: $target" }
}
foreach ($target in @($live, $console, $executed, $receipt)) {
    if (Test-Path -LiteralPath $target) { throw "One-use artifact already exists: $target" }
}
$source = [System.IO.File]::ReadAllText($reviewed)
$old = 'const RUN_ORIGINAL_COMPARISON = false;'
$new = 'const RUN_ORIGINAL_COMPARISON = true;'
if ($source.Split([string[]]@($old), [System.StringSplitOptions]::None).Length -ne 2) {
    throw 'Expected exactly one literal launch guard'
}
$enabled = $source.Replace($old, $new)
$utf8 = [System.Text.UTF8Encoding]::new($false)
[System.IO.File]::WriteAllText($live, $enabled, $utf8)
[System.IO.File]::WriteAllText($executed, $enabled, $utf8)
$liveHash = (Get-FileHash -LiteralPath $live -Algorithm SHA256).Hash
$exit = $null
try {
    & node $live *> $console
    $exit = $LASTEXITCODE
} finally {
    if (Test-Path -LiteralPath $live) { Remove-Item -LiteralPath $live -Force }
    $report = [ordered]@{
        reviewed_runner_sha256 = $expected
        executed_runner_sha256 = $liveHash
        node_exit_code = $exit
        original_guard_restored = ((Get-FileHash -LiteralPath $reviewed -Algorithm SHA256).Hash -eq $expected)
        transient_live_copy_absent = -not (Test-Path -LiteralPath $live)
        console = $console
        executed_source = $executed
    }
    $report | ConvertTo-Json -Depth 4 | Set-Content -LiteralPath $receipt -Encoding utf8
}
if ($exit -ne 0) { throw "Reviewed runner exited $exit; inspect $console and raw case receipts" }
Get-Content -LiteralPath $console
