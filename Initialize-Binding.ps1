<#
.SYNOPSIS
    Renames the YourBinding placeholder throughout the repository with the actual binding name.

.DESCRIPTION
    Replaces all occurrences of "YourBinding" in file contents, file names, and folder names
    with the provided BindingName. Run this script once after creating a new repo from this template.

.PARAMETER BindingName
    The name of the new binding (e.g. "OpenGL", "OpenXR", "WebGPU").

.EXAMPLE
    .\Initialize-Binding.ps1 -BindingName "OpenGL"
#>

[CmdletBinding()]
param (
    [Parameter(Mandatory = $true)]
    [ValidatePattern('^[A-Za-z][A-Za-z0-9.]*$')]
    [string]$BindingName
)

$ErrorActionPreference = "Stop"
$placeholder = "YourBinding"
$root = $PSScriptRoot

if ($BindingName -eq $placeholder) {
    Write-Error "BindingName cannot be '$placeholder'. Please provide a real name."
    exit 1
}

Write-Host "Initializing binding '$BindingName'..." -ForegroundColor Cyan

# 1. Replace in file contents (text files only)
$textExtensions = @("*.cs", "*.csproj", "*.sln", "*.json", "*.yml", "*.yaml", "*.md", "*.props", "*.targets", "*.config", "*.xml")
$files = Get-ChildItem -Path $root -Recurse -File -Include $textExtensions |
Where-Object { $_.FullName -notmatch '\\.git\\' }

foreach ($file in $files) {
    $content = Get-Content $file.FullName -Raw -Encoding UTF8
    if ($content -match [regex]::Escape($placeholder)) {
        $updated = $content -replace [regex]::Escape($placeholder), $BindingName
        Set-Content $file.FullName -Value $updated -Encoding UTF8 -NoNewline
        Write-Host "  Updated contents: $($file.FullName -replace [regex]::Escape($root + '\'), '')"
    }
}

# 2. Rename files containing the placeholder (deepest first to avoid path conflicts)
$filesToRename = Get-ChildItem -Path $root -Recurse -File |
Where-Object { $_.Name -match [regex]::Escape($placeholder) -and $_.FullName -notmatch '\\.git\\' } |
Sort-Object { $_.FullName.Length } -Descending

foreach ($file in $filesToRename) {
    $newName = $file.Name -replace [regex]::Escape($placeholder), $BindingName
    Rename-Item -Path $file.FullName -NewName $newName
    Write-Host "  Renamed file: $($file.Name) -> $newName"
}

# 3. Rename folders containing the placeholder (deepest first)
$foldersToRename = Get-ChildItem -Path $root -Recurse -Directory |
Where-Object { $_.Name -match [regex]::Escape($placeholder) -and $_.FullName -notmatch '\\.git\\' } |
Sort-Object { $_.FullName.Length } -Descending

foreach ($folder in $foldersToRename) {
    $newName = $folder.Name -replace [regex]::Escape($placeholder), $BindingName
    Rename-Item -Path $folder.FullName -NewName $newName
    Write-Host "  Renamed folder: $($folder.Name) -> $newName"
}

# 4. Activate CI/CD workflows (move from workflow-templates to workflows)
$workflowTemplatesDir = Join-Path $root ".github\workflow-templates"
$workflowsDir = Join-Path $root ".github\workflows"
if (Test-Path $workflowTemplatesDir) {
    Get-ChildItem $workflowTemplatesDir -File | ForEach-Object {
        Move-Item $_.FullName (Join-Path $workflowsDir $_.Name)
        Write-Host "  Activated workflow: $($_.Name)"
    }
    Remove-Item $workflowTemplatesDir
}

Write-Host ""
Write-Host "Done! '$placeholder' has been replaced with '$BindingName'." -ForegroundColor Green
Write-Host "Next steps:" -ForegroundColor Yellow
Write-Host "  1. Review the changes and commit them."
Write-Host "  2. Implement the generator logic in ${BindingName}Gen\${BindingName}Gen\Program.cs."
