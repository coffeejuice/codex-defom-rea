[CmdletBinding()]
param(
    # Print the computed next version without writing it.
    [Parameter(Mandatory = $false)]
    [switch]$CheckOnly
)

# Increments DEF_PRE_VERSION (src/app/Version.h.in) to one patch above the highest
# version visible anywhere in this repository: the working file of every git
# worktree (other agent sessions edit their own worktrees concurrently) plus
# the committed file on every local and remote-tracking branch. Two sessions
# that each run this before their builds therefore never deploy two different
# DEF_PRE_64.exe binaries carrying the same title-bar version, whichever of them
# builds first. The major and minor numbers are taken from that highest version
# and never changed here.
Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

$relative = "src/app/Version.h.in"
$pattern = '#define\s+DEF_PRE_VERSION\s+"(\d+)\.(\d+)\.(\d+)"'

function Parse-Version {
    param([string]$Text, [string]$Origin)
    if ($null -eq $Text) { return $null }
    $m = [regex]::Match($Text, $pattern)
    if (-not $m.Success) { return $null }
    [pscustomobject]@{
        Version = [version]::new([int]$m.Groups[1].Value, [int]$m.Groups[2].Value, [int]$m.Groups[3].Value)
        Origin  = $Origin
    }
}

$root = (& git rev-parse --show-toplevel 2>$null)
if ($LASTEXITCODE -ne 0 -or -not $root) { throw "Not inside a git worktree." }
$root = (Resolve-Path -LiteralPath $root).Path
$target = Join-Path $root $relative
if (-not (Test-Path -LiteralPath $target -PathType Leaf)) { throw "Missing $target" }

$seen = [System.Collections.Generic.List[object]]::new()

# 1. Working files of every worktree (uncommitted bumps of sibling sessions).
$worktrees = (& git worktree list --porcelain) | Where-Object { $_ -like "worktree *" } | ForEach-Object { $_.Substring(9) }
foreach ($wt in $worktrees) {
    $file = Join-Path $wt $relative
    if (Test-Path -LiteralPath $file -PathType Leaf) {
        $parsed = Parse-Version -Text (Get-Content -LiteralPath $file -Raw) -Origin "worktree $wt"
        if ($parsed) { $seen.Add($parsed) }
    }
}

# 2. Committed files on every local branch and remote-tracking branch.
$refs = (& git for-each-ref --format='%(refname)' refs/heads refs/remotes) | Where-Object { $_ -and ($_ -notlike '*/HEAD') }
foreach ($ref in $refs) {
    $text = (& git show "${ref}:$relative" 2>$null) -join "`n"
    if ($LASTEXITCODE -eq 0) {
        $parsed = Parse-Version -Text $text -Origin "ref $ref"
        if ($parsed) { $seen.Add($parsed) }
    }
}

$current = Parse-Version -Text (Get-Content -LiteralPath $target -Raw) -Origin "this worktree"
if (-not $current) { throw "DEF_PRE_VERSION not found in $target" }
$highest = $seen | Sort-Object -Property Version -Descending | Select-Object -First 1
if (-not $highest -or $highest.Version -lt $current.Version) { $highest = $current }

$next = [version]::new($highest.Version.Major, $highest.Version.Minor, $highest.Version.Build + 1)
Write-Host ("Highest DEF_PRE_VERSION seen: {0} ({1})" -f $highest.Version, $highest.Origin)
Write-Host ("This worktree:            {0}" -f $current.Version)
Write-Host ("Next DEF_PRE_VERSION:         {0}" -f $next)

if ($CheckOnly) { exit 0 }

$content = Get-Content -LiteralPath $target -Raw
$updated = [regex]::Replace($content, $pattern, ('#define DEF_PRE_VERSION "{0}"' -f $next), 1)
$updated = [regex]::Replace($updated, '#define\s+DEF_PRE_FILE_VERSION\s+\d+,\d+,\d+,0', ('#define DEF_PRE_FILE_VERSION {0},{1},{2},0' -f $next.Major, $next.Minor, $next.Build))
if ($updated -eq $content) { throw "Failed to rewrite DEF_PRE_VERSION in $target" }
[System.IO.File]::WriteAllText($target, $updated, [System.Text.UTF8Encoding]::new($false))
Write-Host ("Wrote {0} -> {1}" -f $target, $next)
