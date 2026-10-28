param(
    [ValidateSet('Release', 'Debug')]
    [string]$Configuration = 'Release',
    [ValidateRange(1, 32)]
    [int]$Parallel = 4,
    [switch]$SkipTests
)

$ErrorActionPreference = 'Stop'
$projectRoot = Split-Path -Parent $PSScriptRoot
$sourceDirectory = Join-Path $projectRoot 'imports\claude-def-pre\port'
$buildDirectory = Join-Path $projectRoot 'build\claude-def-pre'
if (-not (Test-Path -LiteralPath (Join-Path $sourceDirectory 'CMakeLists.txt'))) {
    throw "Imported project not found: $sourceDirectory"
}
New-Item -ItemType Directory -Path $buildDirectory -Force | Out-Null

function Invoke-LoggedTool {
    param([string]$Program, [string[]]$ToolArguments, [string]$LogName)
    $logPath = Join-Path $buildDirectory $LogName
    & $Program @ToolArguments *> $logPath
    $toolExit = $LASTEXITCODE
    Write-Host "$Program exit=$toolExit log=$logPath"
    if ($toolExit -ne 0) {
        Get-Content -LiteralPath $logPath -Tail 40 | Write-Host
        throw "$Program failed with exit code $toolExit"
    }
}

# Build only the imported copy. Offline mode forbids original-process corpus runs.
Invoke-LoggedTool -Program 'cmake' -ToolArguments @(
    '-S', $sourceDirectory, '-B', $buildDirectory,
    '-G', 'Visual Studio 17 2022', '-A', 'x64', '-DDEF_PRE_PORT_OFFLINE=ON'
) -LogName 'configure.log'
Invoke-LoggedTool -Program 'cmake' -ToolArguments @(
    '--build', $buildDirectory, '--config', $Configuration, '--parallel', "$Parallel",
    '--', '/nr:false'
) -LogName 'build.log'

if (-not $SkipTests) {
    $priorOffline = $env:DEFORM_ORACLE_OFFLINE
    try {
        $env:DEFORM_ORACLE_OFFLINE = '1'
        Invoke-LoggedTool -Program 'ctest' -ToolArguments @(
            '--test-dir', $buildDirectory, '-C', $Configuration, '-L', 'tier1',
            '--output-on-failure', '--no-tests=error', '-j', "$Parallel"
        ) -LogName 'tier1-tests.log'
        Get-Content -LiteralPath (Join-Path $buildDirectory 'tier1-tests.log') -Tail 5 | Write-Host
    } finally {
        $env:DEFORM_ORACLE_OFFLINE = $priorOffline
    }
}
Write-Host "Executable: $(Join-Path $buildDirectory "$Configuration\DEF_PRE_64.exe")"
