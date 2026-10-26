$ErrorActionPreference = 'Stop'
$converterRoot = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '..'))
$manifestPath = Join-Path $converterRoot 'analysis/wave3-editor-owned-open-fixture-manifest.json'
$receiptPath = Join-Path $converterRoot 'analysis/wave3-editor-owned-open-fixture-copy.json'
$manifest = Get-Content -LiteralPath $manifestPath -Raw | ConvertFrom-Json
$sourceRoot = [IO.Path]::GetFullPath((Join-Path $converterRoot 'evidence/gui_mo/s1/corpus/32_DB_deconstruction_02'))
$targetRoot = [IO.Path]::GetFullPath('C:\PROBLEM\m2m42')
if ($manifest.source_directory -ne $sourceRoot -or $manifest.proposed_directory -ne $targetRoot) { throw 'Unexpected manifest roots' }
if ($targetRoot -ne 'C:\PROBLEM\m2m42' -or !(Test-Path -LiteralPath 'C:\PROBLEM' -PathType Container)) { throw 'Invalid target parent' }
if (((Get-Item -LiteralPath 'C:\PROBLEM').Attributes -band [IO.FileAttributes]::ReparsePoint) -ne 0) { throw 'Target parent is a reparse point' }
if (Test-Path -LiteralPath $targetRoot) { throw 'Disposable target already exists; never overwrite it' }
if (Test-Path -LiteralPath $receiptPath) { throw 'Receipt already exists; retain it' }
if ($manifest.files.Count -ne 8 -or $manifest.total_bytes -ne 133041) { throw 'Unexpected manifest inventory' }
$prepared = @()
foreach ($entry in $manifest.files) {
    $sourcePath = [IO.Path]::GetFullPath((Join-Path $sourceRoot $entry.relative_path))
    $targetPath = [IO.Path]::GetFullPath((Join-Path $targetRoot $entry.relative_path))
    if (!$sourcePath.StartsWith($sourceRoot + '\', [StringComparison]::OrdinalIgnoreCase) -or
        !$targetPath.StartsWith($targetRoot + '\', [StringComparison]::OrdinalIgnoreCase) -or
        $targetPath -ne $entry.proposed_destination -or $targetPath.Length -ge 100) { throw 'Path escapes approved roots or exceeds original limit' }
    $item = Get-Item -LiteralPath $sourcePath
    if ($item.PSIsContainer -or ($item.Attributes -band [IO.FileAttributes]::ReparsePoint) -ne 0) { throw 'Source is not a regular file' }
    for ($ancestor = $item.Directory; $null -ne $ancestor; $ancestor = $ancestor.Parent) {
        if (($ancestor.Attributes -band [IO.FileAttributes]::ReparsePoint) -ne 0) { throw 'Source reparse traversal' }
    }
    $hash = (Get-FileHash -LiteralPath $sourcePath -Algorithm SHA256).Hash.ToLowerInvariant()
    if ($item.Length -ne $entry.bytes -or $hash -ne $entry.sha256) { throw 'Source fixture differs from manifest' }
    $prepared += [pscustomobject]@{ source = $sourcePath; destination = $targetPath; bytes = $item.Length; sha256 = $hash }
}
if (@($prepared.destination | Select-Object -Unique).Count -ne 8) { throw 'Duplicate destinations' }
$receipt = [ordered]@{ status = 'copy started'; utc = [DateTime]::UtcNow.ToString('o'); source_directory = $sourceRoot; directory = $targetRoot; manifest_sha256 = (Get-FileHash -LiteralPath $manifestPath -Algorithm SHA256).Hash.ToLowerInvariant(); files = @(); original_launched = $false }
try {
    New-Item -ItemType Directory -Path $targetRoot | Out-Null
    foreach ($entry in $prepared) {
        $parent = [IO.Path]::GetDirectoryName($entry.destination)
        if (!(Test-Path -LiteralPath $parent)) { New-Item -ItemType Directory -Path $parent | Out-Null }
        if (Test-Path -LiteralPath $entry.destination) { throw 'Destination unexpectedly exists' }
        Copy-Item -LiteralPath $entry.source -Destination $entry.destination
        $after = (Get-FileHash -LiteralPath $entry.destination -Algorithm SHA256).Hash.ToLowerInvariant()
        if ($after -ne $entry.sha256) { throw 'Copied hash mismatch' }
        $receipt.files += $entry
    }
    foreach ($entry in $prepared) {
        if ((Get-FileHash -LiteralPath $entry.source -Algorithm SHA256).Hash.ToLowerInvariant() -ne $entry.sha256) { throw 'Source changed during copy' }
    }
    $receipt.status = 'eight unchanged files copied and hash verified; original not launched'
} catch {
    $receipt.status = 'partial preparation; inspect preserved files before retry'
    $receipt.error = $_.Exception.Message
    throw
} finally {
    $receipt | ConvertTo-Json -Depth 8 | Set-Content -LiteralPath $receiptPath -Encoding utf8
}
[pscustomobject]@{ receipt = $receiptPath; directory = $targetRoot; files = $receipt.files.Count; bytes = 133041; maximum_path_length = ($prepared | ForEach-Object { $_.destination.Length } | Measure-Object -Maximum).Maximum } | ConvertTo-Json -Compress
