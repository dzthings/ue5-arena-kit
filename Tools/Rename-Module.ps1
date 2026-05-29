<#
.SYNOPSIS
    Renames the ArenaKit module to a new name throughout the project.

.PARAMETER NewName
    PascalCase name for the new module (e.g. "MyGame", "TowerDefense").

.EXAMPLE
    .\Tools\Rename-Module.ps1 -NewName MyGame
#>
param(
    [Parameter(Mandatory)]
    [ValidatePattern('^[A-Z][A-Za-z0-9]+$')]
    [string]$NewName
)

$OldName     = "ArenaKit"
$OldApi      = "ARENAKIT_API"
$NewApi      = "$($NewName.ToUpper())_API"
$OldPrefix   = "AKit"
$NewPrefix   = "A$NewName"
$Root        = Split-Path $PSScriptRoot -Parent

Write-Host "Renaming '$OldName' -> '$NewName' in $Root"

# --- 1. Text replacement in source/config files ---
$extensions = "*.h", "*.cpp", "*.cs", "*.uproject"
Get-ChildItem -Path $Root -Recurse -Include $extensions |
    Where-Object { $_.FullName -notmatch '\\Plugins\\' } |
    ForEach-Object {
        $content = Get-Content $_.FullName -Raw
        $updated = $content `
            -replace [regex]::Escape($OldApi),    $NewApi `
            -replace [regex]::Escape($OldName),   $NewName `
            -replace [regex]::Escape($OldPrefix), $NewPrefix
        if ($updated -ne $content) {
            Set-Content -Path $_.FullName -Value $updated -NoNewline
            Write-Host "  Updated: $($_.Name)"
        }
    }

# --- 2. Rename files whose names contain the old strings ---
$filesToRename = Get-ChildItem -Path $Root -Recurse -File |
    Where-Object { $_.Name -match $OldName -or $_.Name -match "Kit" } |
    Where-Object { $_.FullName -notmatch '\\Plugins\\' } |
    Sort-Object { $_.FullName.Length } -Descending  # deepest first

foreach ($file in $filesToRename) {
    $newFileName = $file.Name `
        -replace [regex]::Escape($OldName), $NewName `
        -replace "Kit",                     $NewName
    if ($newFileName -ne $file.Name) {
        $dest = Join-Path $file.DirectoryName $newFileName
        Rename-Item -Path $file.FullName -NewName $dest -Force
        Write-Host "  Renamed: $($file.Name) -> $newFileName"
    }
}

# --- 3. Rename the Source/<OldName>/ folder ---
$srcDir = Join-Path $Root "Source\$OldName"
if (Test-Path $srcDir) {
    $newSrcDir = Join-Path $Root "Source\$NewName"
    Rename-Item -Path $srcDir -NewName $newSrcDir -Force
    Write-Host "  Renamed folder: Source\$OldName -> Source\$NewName"
}

Write-Host ""
Write-Host "Done. Next steps:"
Write-Host "  1. Right-click $NewName.uproject -> Generate Visual Studio project files"
Write-Host "     (or in Rider: File -> Reload All Projects)"
Write-Host "  2. Build the project (Ctrl+F9 in Rider or open UE5 Editor)"
