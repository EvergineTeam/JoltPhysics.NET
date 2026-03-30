# JoltPhysics.NET

This repository contains low-level bindings for [JoltPhysics](https://github.com/jrouwe/JoltPhysics) (via [JoltPhysicsC](https://github.com/EvergineTeam/JoltPhysicsC)) used in [Evergine](https://evergine.com/).

[![CI](https://github.com/EvergineTeam/JoltPhysics.NET/actions/workflows/CI.yml/badge.svg)](https://github.com/EvergineTeam/JoltPhysics.NET/actions/workflows/CI.yml)
[![CD](https://github.com/EvergineTeam/JoltPhysics.NET/actions/workflows/CD.yml/badge.svg)](https://github.com/EvergineTeam/JoltPhysics.NET/actions/workflows/CD.yml)
[![Nuget](https://img.shields.io/nuget/v/Evergine.Bindings.JoltPhysics?logo=nuget)](https://www.nuget.org/packages/Evergine.Bindings.JoltPhysics)

## Purpose

[JoltPhysics](https://github.com/jrouwe/JoltPhysics) is a high-performance, multi-core friendly rigid body physics engine written in C++. JoltPhysicsC provides a pure C API layer, and this repository auto-generates .NET P/Invoke bindings from those C headers using [CppAst](https://github.com/xoofx/CppAst.NET).

## Features

- **Full JoltPhysics C API** — Covers the complete JoltPhysicsC surface: physics system, body management, shapes, constraints, queries, character controllers, vehicles, skeletons, and ragdolls
- **Auto-generated bindings** — Generated from JoltPhysicsC headers using CppAst; easily regenerated when the upstream API evolves
- **Blittable structs** — Math types (Vec3, Quat, Mat44, etc.) and settings structs are blittable, allowing zero-copy interop
- **Unsafe raw pointers** — Native pointer semantics preserved for maximum performance
- **Pre-built native libraries** — Ships with JoltPhysicsC binaries for all supported platforms

## Supported Platforms

- [x] Windows x64
- [x] Windows ARM64
- [x] Linux x64
- [x] Linux ARM64
- [x] macOS ARM64

## Development

### Generate bindings locally

```bash
dotnet run --project JoltPhysicsGen/JoltPhysicsGen.csproj
```

### Build the binding library

```bash
dotnet build Evergine.Bindings.JoltPhysics/Evergine.Bindings.JoltPhysics.csproj
```

### Pack for NuGet

```bash
dotnet pack Evergine.Bindings.JoltPhysics/Evergine.Bindings.JoltPhysics.csproj -c Release
```

### Run API smoke tests (Windows)

```bash
dotnet test Tests/JoltPhysics.APITests/JoltPhysics.APITests.csproj -c Debug -r win-x64
```

## Workflows

| Workflow | Trigger | Description |
|----------|---------|-------------|
| CI | push/PR to `main` | Build and validate bindings |
| CD | manual | Regenerate bindings and publish NuGet |
| Sync standards | monthly / manual | Sync shared scripts from evergine-standards |

## License

See [LICENSE](LICENSE).
