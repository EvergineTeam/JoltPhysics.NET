using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using Evergine.Bindings.JoltPhysics;
using Xunit;

namespace JoltPhysicsTests;

public static class NativeTestHelpers
{
	public static Vec3 Vec3(float x, float y, float z) => new() { X = x, Y = y, Z = z };
	public static RVec3 RVec3(float x, float y, float z) => new() { X = x, Y = y, Z = z };
	public static Quat IdentityQuat() => new() { X = 0f, Y = 0f, Z = 0f, W = 1f };
}

/// <summary>
/// One-time global JoltC init/shutdown shared across all tests in the "Jolt" collection.
/// xUnit creates this once before the first test and disposes it after the last test.
/// </summary>
public sealed unsafe class JoltGlobalFixture : IDisposable
{
	public JoltGlobalFixture()
	{
		JoltPhysics.RegisterDefaultAllocator();
		JoltPhysics.Init();
		JoltPhysics.CreateFactory();
		JoltPhysics.RegisterTypes();
	}

	public void Dispose()
	{
		JoltPhysics.UnregisterTypes();
		JoltPhysics.DestroyFactory();
		JoltPhysics.Shutdown();
	}
}

[CollectionDefinition("Jolt")]
public class JoltCollection : ICollectionFixture<JoltGlobalFixture> { }

/// <summary>
/// Per-test physics world. Creates and destroys a PhysicsSystem, filters, allocator,
/// and job system. Global init/shutdown is handled by <see cref="JoltGlobalFixture"/>.
/// </summary>
public sealed unsafe class JoltTestEnvironment : IDisposable
{
	private readonly List<uint> _bodyIds = new();
	private readonly List<IntPtr> _constraints = new();
	private readonly List<IntPtr> _shapes = new();
	private bool _disposed;

	public const ushort LayerNonMoving = 0;
	public const ushort LayerMoving = 1;
	private const uint NumObjectLayers = 2;
	private const uint NumBroadPhaseLayers = 2;

	public IntPtr PhysicsSystem { get; }
	public IntPtr BodyInterface { get; }
	public IntPtr TempAllocator { get; }
	public IntPtr JobSystem { get; }
	private IntPtr BroadPhase { get; }
	private IntPtr ObjectVsBroadPhaseFilter { get; }
	private IntPtr ObjectLayerPairFilter { get; }

	public static string GetLastErrorString()
	{
		byte* err = JoltPhysics.GetLastError();
		return err == null ? string.Empty : Marshal.PtrToStringUTF8((IntPtr)err) ?? string.Empty;
	}

	public JoltTestEnvironment()
	{
		JoltPhysics.ClearLastError();

		BroadPhase = JoltPhysics.BroadPhaseLayerInterfaceTable_Create(NumObjectLayers, NumBroadPhaseLayers);
		if (BroadPhase == IntPtr.Zero)
			throw new InvalidOperationException($"Failed to create BroadPhase: {GetLastErrorString()}");
		JoltPhysics.BroadPhaseLayerInterfaceTable_MapObjectToBroadPhaseLayer(BroadPhase, LayerNonMoving, 0);
		JoltPhysics.BroadPhaseLayerInterfaceTable_MapObjectToBroadPhaseLayer(BroadPhase, LayerMoving, 1);

		ObjectLayerPairFilter = JoltPhysics.ObjectLayerPairFilterTable_Create(NumObjectLayers);
		if (ObjectLayerPairFilter == IntPtr.Zero)
			throw new InvalidOperationException($"Failed to create ObjectLayerPairFilter: {GetLastErrorString()}");
		JoltPhysics.ObjectLayerPairFilterTable_EnableCollision(ObjectLayerPairFilter, LayerNonMoving, LayerMoving);
		JoltPhysics.ObjectLayerPairFilterTable_EnableCollision(ObjectLayerPairFilter, LayerMoving, LayerMoving);

		ObjectVsBroadPhaseFilter = JoltPhysics.ObjectVsBroadPhaseLayerFilterTable_Create(BroadPhase, NumBroadPhaseLayers, ObjectLayerPairFilter, NumObjectLayers);
		if (ObjectVsBroadPhaseFilter == IntPtr.Zero)
			throw new InvalidOperationException($"Failed to create ObjectVsBroadPhaseLayerFilter: {GetLastErrorString()}");

		PhysicsSystem = JoltPhysics.PhysicsSystem_Create();
		if (PhysicsSystem == IntPtr.Zero)
			throw new InvalidOperationException($"Failed to create PhysicsSystem: {GetLastErrorString()}");

		const uint maxBodies = 1024;
		const uint maxBodyPairs = 1024;
		const uint maxContactConstraints = 1024;
		uint numBodyMutexes = Math.Max(1u, maxBodies / 64);
		JoltPhysics.PhysicsSystem_Init(PhysicsSystem, maxBodies, numBodyMutexes, maxBodyPairs, maxContactConstraints, BroadPhase, ObjectVsBroadPhaseFilter, ObjectLayerPairFilter);

		BodyInterface = JoltPhysics.PhysicsSystem_GetBodyInterface(PhysicsSystem);
		if (BodyInterface == IntPtr.Zero)
			throw new InvalidOperationException($"Failed to get BodyInterface: {GetLastErrorString()}");

		TempAllocator = JoltPhysics.TempAllocator_Create(10 * 1024 * 1024);
		if (TempAllocator == IntPtr.Zero)
			throw new InvalidOperationException($"Failed to create TempAllocator: {GetLastErrorString()}");

		JobSystem = JoltPhysics.JobSystemThreadPool_Create(2048, 8, 1);
		if (JobSystem == IntPtr.Zero)
			throw new InvalidOperationException($"Failed to create JobSystem: {GetLastErrorString()}");

		JoltPhysics.PhysicsSystem_SetGravity(PhysicsSystem, NativeTestHelpers.Vec3(0f, -9.81f, 0f));
	}

	public uint CreateStaticBox(Vec3 halfExtent, RVec3 position)
	{
		var shape = JoltPhysics.BoxShape_Create(halfExtent, 0.05f);

		BodyCreationSettings settings = default;
		JoltPhysics.BodyCreationSettings_SetDefault(&settings);
		settings .Shape = shape;
		settings.MotionType = MotionType.Static;
		settings.Position = position;
		settings.ObjectLayer = LayerNonMoving;

		uint bodyId = JoltPhysics.BodyInterface_CreateBody(BodyInterface, &settings);
		if (bodyId == 0)
		{
			throw new InvalidOperationException($"CreateBody(static) failed: {GetLastErrorString()}");
		}
		JoltPhysics.BodyInterface_AddBody(BodyInterface, bodyId, Activation.DontActivate);
		_bodyIds.Add(bodyId);
		_shapes.Add(shape);
		return bodyId;
	}

	public uint CreateDynamicBox(Vec3 halfExtent, RVec3 position)
	{
		var shape = JoltPhysics.BoxShape_Create(halfExtent, 0.05f);

		BodyCreationSettings settings = default;
		JoltPhysics.BodyCreationSettings_SetDefault(&settings);
		settings .Shape = shape;
		settings.MotionType = MotionType.Dynamic;
		settings.Position = position;
		settings.ObjectLayer = LayerMoving;

		uint bodyId = JoltPhysics.BodyInterface_CreateBody(BodyInterface, &settings);
		if (bodyId == 0)
		{
			throw new InvalidOperationException($"CreateBody(dynamic) failed: {GetLastErrorString()}");
		}
		JoltPhysics.BodyInterface_AddBody(BodyInterface, bodyId, Activation.Activate);
		_bodyIds.Add(bodyId);
		_shapes.Add(shape);
		return bodyId;
	}

	public IntPtr CreateFixedConstraint(uint body1, uint body2)
	{
		FixedConstraintSettings settings = default;
		JoltPhysics.FixedConstraintSettings_Init(&settings);
		settings.Space = ConstraintSpace.WorldSpace;
		settings.AutoDetectPoint = 1;
		settings.AxisX1 = NativeTestHelpers.Vec3(1f, 0f, 0f);
		settings.AxisY1 = NativeTestHelpers.Vec3(0f, 1f, 0f);
		settings.AxisX2 = NativeTestHelpers.Vec3(1f, 0f, 0f);
		settings.AxisY2 = NativeTestHelpers.Vec3(0f, 1f, 0f);

		IntPtr constraint = JoltPhysics.FixedConstraint_Create(PhysicsSystem, body1, body2, &settings);
		IntPtr* constraintPtr = stackalloc IntPtr[1] { constraint };
		JoltPhysics.PhysicsSystem_AddConstraints(PhysicsSystem, constraintPtr, 1);
		_constraints.Add(constraint);
		return constraint;
	}

	public uint Step(float deltaTime = 1f / 60f, int collisionSteps = 1)
	{
		return JoltPhysics.PhysicsSystem_Update(PhysicsSystem, deltaTime, collisionSteps, TempAllocator, JobSystem);
	}

	public void Dispose()
	{
		if (_disposed)
		{
			return;
		}
		_disposed = true;

		foreach (uint bodyId in _bodyIds)
		{
			JoltPhysics.BodyInterface_RemoveBody(BodyInterface, bodyId);
			JoltPhysics.BodyInterface_DestroyBody(BodyInterface, bodyId);
		}
		_bodyIds.Clear();

		foreach (IntPtr constraint in _constraints)
		{
			var constraintPtrArray = new IntPtr[1] { constraint };
			fixed (IntPtr* constraintPtr = constraintPtrArray)
			{
				JoltPhysics.PhysicsSystem_RemoveConstraints(PhysicsSystem, constraintPtr, 1);
			}
			JoltPhysics.Constraint_Destroy(constraint);
		}
		_constraints.Clear();

		foreach (IntPtr shape in _shapes)
		{
			JoltPhysics.Shape_Destroy(shape);
		}
		_shapes.Clear();

		JoltPhysics.PhysicsSystem_Destroy(PhysicsSystem);

		JoltPhysics.ObjectLayerPairFilter_Destroy(ObjectLayerPairFilter);
		JoltPhysics.ObjectVsBroadPhaseLayerFilter_Destroy(ObjectVsBroadPhaseFilter);
		JoltPhysics.BroadPhaseLayerInterface_Destroy(BroadPhase);

		JoltPhysics.JobSystem_Destroy(JobSystem);
		JoltPhysics.TempAllocator_Destroy(TempAllocator);
	}
}
