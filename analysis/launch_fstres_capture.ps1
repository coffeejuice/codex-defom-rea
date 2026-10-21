param(
    [Parameter(Mandatory=$true)]
    [ValidateSet('training-a','training-b','held-out-a','held-out-b')]
    [string]$Batch,
    [ValidatePattern('^(-v[2-9][0-9]*)?$')]
    [string]$RunSuffix = ''
)
$ErrorActionPreference = 'Stop'
$taskRoot = Split-Path $PSScriptRoot -Parent
Set-Location -LiteralPath $taskRoot
$manifestPath = Join-Path $taskRoot "analysis/fstres-$Batch-cases.json"
$freezePath = Join-Path $taskRoot "analysis/fstres-$Batch-input-freeze.json"
$manifest = Get-Content -LiteralPath $manifestPath -Raw | ConvertFrom-Json
$freeze = Get-Content -LiteralPath $freezePath -Raw | ConvertFrom-Json
if ((Get-FileHash -LiteralPath $manifestPath -Algorithm SHA256).Hash.ToLowerInvariant() -ne $freeze.fixture_manifest.sha256) {
    throw 'Fixture manifest differs from frozen inputs'
}
foreach ($item in $freeze.inputs) {
    $inputPath = Join-Path $taskRoot $item.path
    if ((Get-FileHash -LiteralPath $inputPath -Algorithm SHA256).Hash.ToLowerInvariant() -ne $item.sha256) {
        throw "Frozen input changed: $($item.path)"
    }
}
$candidate = Get-Content -LiteralPath (Join-Path $taskRoot $freeze.candidate_manifest.path) -Raw | ConvertFrom-Json
foreach ($item in $candidate.files) {
    if ((Get-FileHash -LiteralPath (Join-Path $taskRoot $item.path) -Algorithm SHA256).Hash.ToLowerInvariant() -ne $item.sha256) {
        throw "Frozen candidate changed: $($item.path)"
    }
}
$captureRoot = Join-Path $taskRoot "analysis/oracle/fstres-$Batch$RunSuffix"
$gatePath = "$captureRoot.ready"
$pidPath = "$captureRoot.pid.json"
$probePath = "$captureRoot.js"
foreach ($path in @($captureRoot,$gatePath,$pidPath,$probePath)) {
    if (Test-Path -LiteralPath $path) { throw "Preserve existing capture: $path" }
}
$template = [IO.File]::ReadAllText((Join-Path $taskRoot $manifest.probe.path))
[IO.File]::WriteAllText($probePath, $template.Replace('__READY_GATE__', $gatePath.Replace('\','/')), [Text.UTF8Encoding]::new($false))
$fixtures = Split-Path (Join-Path $taskRoot $manifest.cases[0].path) -Parent
$keys = @($manifest.cases | ForEach-Object { Split-Path $_.path -Leaf })
& python tools/capture_keywords.py --exe source/DEF_PRE_64.exe --fixtures $fixtures --output $captureRoot --keys $keys --timeout 45 --ready-gate $gatePath --pid-file $pidPath --gate-timeout 120 *> "$captureRoot-launch.log"
exit $LASTEXITCODE
