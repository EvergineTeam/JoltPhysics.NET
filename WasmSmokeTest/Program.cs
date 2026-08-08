// Proves that the published package works from a .NET wasm application.
//
// The scenario is the one DesktopSmokeTest runs, and it is here for a different reason. On the
// desktop identifiers the archive is loaded at run time out of runtimes/<rid>/native, and this
// package got that right from the start. browser-wasm is linked at publish time instead, through
// a NativeFileReference the package's buildTransitive targets file adds -- and for months there
// was no targets file and the archive was packed under a name no DllImport could match, so the
// wasm library shipped in every release and no consumer could have linked it. Nothing failed,
// because nothing tried.
//
// This is the only test that tries. It goes through PackageReference on purpose, so the targets
// file and the packed file name are part of what is under test.
//
// Two things differ from the desktop version, and both are about the platform rather than taste:
//
//   numThreads = 0. There is no pthread here, so the pool is created with no workers and
//   JoltPhysics runs jobs on the thread that waits on the barrier.
//
//   Looser bounds on the result. The same drop settles in 63 steps at y=0.486 here against 48
//   steps at y=0.480 on the desktop legs, so asserting the desktop numbers would fail for a
//   reason that has nothing to do with the package being correct.

using System;
using System.Runtime.InteropServices;
using Evergine.Bindings.JoltPhysics;

internal static unsafe class Program
{
    private const ushort LayerNonMoving = 0;
    private const ushort LayerMoving = 1;
    private const uint NumObjectLayers = 2;
    private const uint NumBroadPhaseLayers = 2;

    private const float DeltaTime = 1.0f / 60.0f;
    private const int MaxSteps = 600;   // ten seconds of simulation

    private static int _failures;

    private static void Check(bool condition, string what)
    {
        Console.WriteLine((condition ? "  ok    " : "  FAIL  ") + what);
        if (!condition)
        {
            _failures++;
        }
    }

    private static int Main()
    {
        Console.WriteLine($"JoltC on {RuntimeInformation.RuntimeIdentifier}");

        // Reaching native code at all. On wasm this is where a missing targets file, or an archive
        // packed under the wrong name, stops being invisible.
        JoltPhysics.RegisterDefaultAllocator();
        JoltPhysics.Init();
        JoltPhysics.CreateFactory();
        JoltPhysics.RegisterTypes();

        IntPtr tempAllocator = JoltPhysics.TempAllocator_Create(4 * 1024 * 1024);
        IntPtr jobSystem = JoltPhysics.JobSystemThreadPool_Create(1024, 8, 0);
        Check(tempAllocator != IntPtr.Zero && jobSystem != IntPtr.Zero,
              "the allocator and job system were created, so the archive linked and the DllImports resolved");

        IntPtr broadPhase = JoltPhysics.BroadPhaseLayerInterfaceTable_Create(NumObjectLayers, NumBroadPhaseLayers);
        JoltPhysics.BroadPhaseLayerInterfaceTable_MapObjectToBroadPhaseLayer(broadPhase, LayerNonMoving, 0);
        JoltPhysics.BroadPhaseLayerInterfaceTable_MapObjectToBroadPhaseLayer(broadPhase, LayerMoving, 1);

        IntPtr objectLayerPairFilter = JoltPhysics.ObjectLayerPairFilterTable_Create(NumObjectLayers);
        JoltPhysics.ObjectLayerPairFilterTable_EnableCollision(objectLayerPairFilter, LayerNonMoving, LayerMoving);
        JoltPhysics.ObjectLayerPairFilterTable_EnableCollision(objectLayerPairFilter, LayerMoving, LayerMoving);

        IntPtr objectVsBroadPhaseFilter = JoltPhysics.ObjectVsBroadPhaseLayerFilterTable_Create(
            broadPhase, NumBroadPhaseLayers, objectLayerPairFilter, NumObjectLayers);

        IntPtr physicsSystem = JoltPhysics.PhysicsSystem_Create();
        JoltPhysics.PhysicsSystem_Init(
            physicsSystem, 1024, 0, 1024, 1024,
            broadPhase, objectVsBroadPhaseFilter, objectLayerPairFilter);
        Check(physicsSystem != IntPtr.Zero, "the physics system was created");

        IntPtr bodyInterface = JoltPhysics.PhysicsSystem_GetBodyInterface(physicsSystem);

        // A floor whose top surface sits at y = 0.
        IntPtr floorShape = JoltPhysics.BoxShape_Create(new Vec3 { X = 100.0f, Y = 1.0f, Z = 100.0f }, 0.05f);
        BodyCreationSettings floorSettings = default;
        JoltPhysics.BodyCreationSettings_SetDefault(&floorSettings);
        floorSettings.Position = new RVec3 { X = 0.0f, Y = -1.0f, Z = 0.0f };
        floorSettings.Rotation = new Quat { X = 0f, Y = 0f, Z = 0f, W = 1f };
        floorSettings.MotionType = MotionType.Static;
        floorSettings.ObjectLayer = LayerNonMoving;
        floorSettings.Shape = floorShape;
        uint floorBodyId = JoltPhysics.BodyInterface_CreateBody(bodyInterface, &floorSettings);
        JoltPhysics.BodyInterface_AddBody(bodyInterface, floorBodyId, Activation.DontActivate);

        // A sphere of radius 0.5 dropped from y = 2.
        IntPtr sphereShape = JoltPhysics.SphereShape_Create(0.5f);
        BodyCreationSettings sphereSettings = default;
        JoltPhysics.BodyCreationSettings_SetDefault(&sphereSettings);
        sphereSettings.Position = new RVec3 { X = 0.0f, Y = 2.0f, Z = 0.0f };
        sphereSettings.Rotation = new Quat { X = 0f, Y = 0f, Z = 0f, W = 1f };
        sphereSettings.MotionType = MotionType.Dynamic;
        sphereSettings.ObjectLayer = LayerMoving;
        sphereSettings.Shape = sphereShape;
        uint sphereBodyId = JoltPhysics.BodyInterface_CreateBody(bodyInterface, &sphereSettings);
        JoltPhysics.BodyInterface_AddBody(bodyInterface, sphereBodyId, Activation.Activate);

        JoltPhysics.PhysicsSystem_OptimizeBroadPhase(physicsSystem);

        double startY = JoltPhysics.BodyInterface_GetCenterOfMassPosition(bodyInterface, sphereBodyId).Y;

        int steps = 0;
        while (JoltPhysics.BodyInterface_IsActive(bodyInterface, sphereBodyId) && steps < MaxSteps)
        {
            JoltPhysics.PhysicsSystem_Update(physicsSystem, DeltaTime, 1, tempAllocator, jobSystem);
            steps++;
        }

        double endY = JoltPhysics.BodyInterface_GetCenterOfMassPosition(bodyInterface, sphereBodyId).Y;
        Console.WriteLine($"  sphere went from y={startY:F3} to y={endY:F3} in {steps} steps");

        // Simulation ran rather than merely being set up. Without this the test would pass against
        // a library whose Update does nothing at all.
        Check(steps > 0 && steps < MaxSteps,
              $"the sphere came to rest in {steps} steps, so the simulation both ran and settled");
        Check(endY < startY, "the sphere fell, so gravity was integrated");

        // Resting on the floor rather than through it. A sphere of radius 0.5 on a surface at
        // y = 0 settles with its centre near y = 0.5, and anything below zero means collision did
        // not happen -- which a build with the wrong flags can produce while still running.
        Check(endY > 0.2 && endY < 0.8,
              $"it settled on the floor rather than falling through it (y={endY:F3})");

        JoltPhysics.BodyInterface_RemoveBody(bodyInterface, sphereBodyId);
        JoltPhysics.BodyInterface_DestroyBody(bodyInterface, sphereBodyId);
        JoltPhysics.BodyInterface_RemoveBody(bodyInterface, floorBodyId);
        JoltPhysics.BodyInterface_DestroyBody(bodyInterface, floorBodyId);
        JoltPhysics.Shape_Destroy(sphereShape);
        JoltPhysics.Shape_Destroy(floorShape);
        JoltPhysics.PhysicsSystem_Destroy(physicsSystem);
        JoltPhysics.ObjectLayerPairFilter_Destroy(objectLayerPairFilter);
        JoltPhysics.ObjectVsBroadPhaseLayerFilter_Destroy(objectVsBroadPhaseFilter);
        JoltPhysics.BroadPhaseLayerInterface_Destroy(broadPhase);
        JoltPhysics.JobSystem_Destroy(jobSystem);
        JoltPhysics.TempAllocator_Destroy(tempAllocator);
        JoltPhysics.UnregisterTypes();
        JoltPhysics.DestroyFactory();
        JoltPhysics.Shutdown();

        Console.WriteLine(_failures == 0 ? "PASS" : $"FAIL ({_failures})");
        return _failures == 0 ? 0 : 1;
    }
}
