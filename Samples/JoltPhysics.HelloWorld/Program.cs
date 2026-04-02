// HelloWorld sample — C# port of the JoltC HelloWorld example.
// Validates the Evergine.Bindings.JoltPhysics binding by creating a static floor,
// dropping a dynamic sphere onto it, and printing position/velocity each step.

using System;
using System.Runtime.InteropServices;
using Evergine.Bindings.JoltPhysics;

unsafe
{
    // --- Layer definitions (matching the C example) ---
    const ushort LayerNonMoving = 0;
    const ushort LayerMoving = 1;
    const uint NumObjectLayers = 2;
    const uint NumBroadPhaseLayers = 2;

    // ------------------------------------------------------------------
    // 1. Initialize Jolt
    // ------------------------------------------------------------------
    JoltPhysics.RegisterDefaultAllocator();
    JoltPhysics.Init();
    JoltPhysics.CreateFactory();
    JoltPhysics.RegisterTypes();

    // ------------------------------------------------------------------
    // 2. Create allocator & job system
    // ------------------------------------------------------------------
    IntPtr tempAllocator = JoltPhysics.TempAllocator_Create(10 * 1024 * 1024);
    IntPtr jobSystem = JoltPhysics.JobSystemThreadPool_Create(2048, 8, 1);

    // ------------------------------------------------------------------
    // 3. Create broad phase / collision filter tables
    //    (The C example uses callback-based interfaces; this binding
    //     provides convenient table-based helpers.)
    // ------------------------------------------------------------------
    IntPtr broadPhase = JoltPhysics.BroadPhaseLayerInterfaceTable_Create(NumObjectLayers, NumBroadPhaseLayers);
    JoltPhysics.BroadPhaseLayerInterfaceTable_MapObjectToBroadPhaseLayer(broadPhase, LayerNonMoving, 0);
    JoltPhysics.BroadPhaseLayerInterfaceTable_MapObjectToBroadPhaseLayer(broadPhase, LayerMoving, 1);

    IntPtr objectLayerPairFilter = JoltPhysics.ObjectLayerPairFilterTable_Create(NumObjectLayers);
    JoltPhysics.ObjectLayerPairFilterTable_EnableCollision(objectLayerPairFilter, LayerNonMoving, LayerMoving);
    JoltPhysics.ObjectLayerPairFilterTable_EnableCollision(objectLayerPairFilter, LayerMoving, LayerMoving);

    IntPtr objectVsBroadPhaseFilter = JoltPhysics.ObjectVsBroadPhaseLayerFilterTable_Create(
        broadPhase, NumBroadPhaseLayers, objectLayerPairFilter, NumObjectLayers);

    // ------------------------------------------------------------------
    // 4. Create the physics system
    // ------------------------------------------------------------------
    const uint cMaxBodies = 1024;
    const uint cNumBodyMutexes = 0;
    const uint cMaxBodyPairs = 1024;
    const uint cMaxContactConstraints = 1024;

    IntPtr physicsSystem = JoltPhysics.PhysicsSystem_Create();
    JoltPhysics.PhysicsSystem_Init(
        physicsSystem,
        cMaxBodies,
        cNumBodyMutexes,
        cMaxBodyPairs,
        cMaxContactConstraints,
        broadPhase,
        objectVsBroadPhaseFilter,
        objectLayerPairFilter);

    IntPtr bodyInterface = JoltPhysics.PhysicsSystem_GetBodyInterface(physicsSystem);

    // ------------------------------------------------------------------
    // 5. Create a static floor (box shape: 200 x 2 x 200)
    // ------------------------------------------------------------------
    Vec3 floorHalfExtent = new() { X = 100.0f, Y = 1.0f, Z = 100.0f };
    IntPtr floorShape = JoltPhysics.BoxShape_Create(floorHalfExtent, 0.05f);

    BodyCreationSettings floorSettings = default;
    JoltPhysics.BodyCreationSettings_SetDefault(&floorSettings);
    floorSettings.Position = new RVec3 { X = 0.0f, Y = -1.0f, Z = 0.0f };
    floorSettings.Rotation = new Quat { X = 0f, Y = 0f, Z = 0f, W = 1f };
    floorSettings.MotionType = MotionType.Static;
    floorSettings.ObjectLayer = LayerNonMoving;
    floorSettings .Shape = floorShape;

    uint floorBodyId = JoltPhysics.BodyInterface_CreateBody(bodyInterface, &floorSettings);
    JoltPhysics.BodyInterface_AddBody(bodyInterface, floorBodyId, Activation.DontActivate);

    // ------------------------------------------------------------------
    // 6. Create a dynamic sphere (radius 0.5) at y = 2
    // ------------------------------------------------------------------
    IntPtr sphereShape = JoltPhysics.SphereShape_Create(0.5f);

    BodyCreationSettings sphereSettings = default;
    JoltPhysics.BodyCreationSettings_SetDefault(&sphereSettings);
    sphereSettings.Position = new RVec3 { X = 0.0f, Y = 2.0f, Z = 0.0f };
    sphereSettings.Rotation = new Quat { X = 0f, Y = 0f, Z = 0f, W = 1f };
    sphereSettings.MotionType = MotionType.Dynamic;
    sphereSettings.ObjectLayer = LayerMoving;
    sphereSettings .Shape = sphereShape;

    uint sphereBodyId = JoltPhysics.BodyInterface_CreateBody(bodyInterface, &sphereSettings);
    JoltPhysics.BodyInterface_AddBody(bodyInterface, sphereBodyId, Activation.Activate);

    // Give the sphere an initial downward velocity (as in the C example)
    JoltPhysics.BodyInterface_SetLinearVelocity(bodyInterface, sphereBodyId, new Vec3 { X = 0.0f, Y = -5.0f, Z = 0.0f });

    // ------------------------------------------------------------------
    // 7. Optimize broad phase & run simulation
    // ------------------------------------------------------------------
    JoltPhysics.PhysicsSystem_OptimizeBroadPhase(physicsSystem);

    const float cDeltaTime = 1.0f / 60.0f;
    const int cCollisionSteps = 1;

    int step = 0;
    while (JoltPhysics.BodyInterface_IsActive(bodyInterface, sphereBodyId) != 0)
    {
        step++;

        RVec3 position = JoltPhysics.BodyInterface_GetCenterOfMassPosition(bodyInterface, sphereBodyId);
        Vec3 velocity = JoltPhysics.BodyInterface_GetLinearVelocity(bodyInterface, sphereBodyId);

        Console.WriteLine($"Step {step}: Position = ({position.X:F6}, {position.Y:F6}, {position.Z:F6}), Velocity = ({velocity.X:F6}, {velocity.Y:F6}, {velocity.Z:F6})");

        JoltPhysics.PhysicsSystem_Update(physicsSystem, cDeltaTime, cCollisionSteps, tempAllocator, jobSystem);
    }

    // ------------------------------------------------------------------
    // 8. Cleanup — remove and destroy bodies, then tear down the world
    // ------------------------------------------------------------------
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

    Console.WriteLine("Hello, JoltPhysics from C#!");
}
