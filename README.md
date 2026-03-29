# Evergine.Bindings.YourBinding

Low-level .NET bindings for YourBinding, used in Evergine.

## Initializing from template

After creating a new repository from this template, run the initialization script to replace the `YourBinding` placeholder with your actual binding name:

```powershell
.\Initialize-Binding.ps1 -BindingName "OpenGL"
```

The script will:
- Replace all occurrences of `YourBinding` in file contents, file names, and folder names
- Move the CI/CD workflows from `.github/workflow-templates/` to `.github/workflows/` so they become active

After running it, review the changes and commit.

## Structure

```
YourBindingGen/
  YourBindingGen/                         # Generator console app (produces C# bindings)
  Evergine.Bindings.YourBinding/          # Generated .NET bindings NuGet package
build/scripts/                            # Synced from evergine-standards
```

## Development

### Generate bindings locally

```powershell
pwsh ./build/scripts/Generate-Bindings-DotNet.ps1 `
  -GeneratorProject "YourBindingGen/YourBindingGen/YourBindingGen.csproj" `
  -GeneratorName "YourBindingGen"
```

### Generate NuGet packages

```powershell
pwsh ./build/scripts/Generate-NuGets-DotNet.ps1 `
  -Version "1.0.0-local" `
  -Projects @("YourBindingGen/Evergine.Bindings.YourBinding/Evergine.Bindings.YourBinding.csproj")
```

## Workflows

| Workflow | Trigger | Description |
|----------|---------|-------------|
| CI | push/PR to `main` | Build and validate bindings |
| CD | manual | Regenerate bindings and publish NuGet |
| Sync standards | monthly / manual | Sync shared scripts from evergine-standards |

## License

See [LICENSE](LICENSE).
