using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using Evergine.Bindings.JoltPhysics;
using Xunit;

namespace JoltPhysics.APITests;

public static class NativeTestHelpers
{
	public static JoltC_Vec3 Vec3(float x, float y, float z) => new() { x = x, y = y, z = z };
	public static JoltC_RVec3 RVec3(float x, float y, float z) => new() { x = x, y = y, z = z };
	public static JoltC_Quat IdentityQuat() => new() { x = 0f, y = 0f, z = 0f, w = 1f };
}

/// <summary>
/// One-time global JoltC init/shutdown shared across all tests in the "Jolt" collection.
/// xUnit creates this once before the first test and disposes it after the last test.
/// </summary>
public sealed unsafe class JoltGlobalFixture : IDisposable
{
	public JoltGlobalFixture()
	{
		JoltPhysicsNative.JoltC_RegisterDefaultAllocator();
		JoltPhysicsNative.JoltC_Init();
		JoltPhysicsNative.JoltC_CreateFactory();
		JoltPhysicsNative.JoltC_RegisterTypes();
	}

	public void Dispose()
	{
		JoltPhysicsNative.JoltC_UnregisterTypes();
		JoltPhysicsNative.JoltC_DestroyFactory();
		JoltPhysicsNative.JoltC_Shutdown();
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
		byte* err = JoltPhysicsNative.JoltC_GetLastError();
		return err == null ? string.Empty : Marshal.PtrToStringUTF8((IntPtr)err) ?? string.Empty;
	}

	public JoltTestEnvironment()
	{
		JoltPhysicsNative.JoltC_ClearLastError();

		BroadPhase = JoltPhysicsNative.JoltC_BroadPhaseLayerInterfaceTable_Create(NumObjectLayers, NumBroadPhaseLayers);
		if (BroadPhase == IntPtr.Zero)
			throw new InvalidOperationException($"Failed to create BroadPhase: {GetLastErrorString()}");
		JoltPhysicsNative.JoltC_BroadPhaseLayerInterfaceTable_MapObjectToBroadPhaseLayer(BroadPhase, LayerNonMoving, 0);
		JoltPhysicsNative.JoltC_BroadPhaseLayerInterfaceTable_MapObjectToBroadPhaseLayer(BroadPhase, LayerMoving, 1);

		ObjectLayerPairFilter = JoltPhysicsNative.JoltC_ObjectLayerPairFilterTable_Create(NumObjectLayers);
		if (ObjectLayerPairFilter == IntPtr.Zero)
			throw new InvalidOperationException($"Failed to create ObjectLayerPairFilter: {GetLastErrorString()}");
		JoltPhysicsNative.JoltC_ObjectLayerPairFilterTable_EnableCollision(ObjectLayerPairFilter, LayerNonMoving, LayerMoving);
		JoltPhysicsNative.JoltC_ObjectLayerPairFilterTable_EnableCollision(ObjectLayerPairFilter, LayerMoving, LayerMoving);

		ObjectVsBroadPhaseFilter = JoltPhysicsNative.JoltC_ObjectVsBroadPhaseLayerFilterTable_Create(BroadPhase, NumBroadPhaseLayers, ObjectLayerPairFilter, NumObjectLayers);
		if (ObjectVsBroadPhaseFilter == IntPtr.Zero)
			throw new InvalidOperationException($"Failed to create ObjectVsBroadPhaseLayerFilter: {GetLastErrorString()}");

		PhysicsSystem = JoltPhysicsNative.JoltC_PhysicsSystem_Create();
		if (PhysicsSystem == IntPtr.Zero)
			throw new InvalidOperationException($"Failed to create PhysicsSystem: {GetLastErrorString()}");

		const uint maxBodies = 1024;
		const uint maxBodyPairs = 1024;
		const uint maxContactConstraints = 1024;
		uint numBodyMutexes = Math.Max(1u, maxBodies / 64);
		JoltPhysicsNative.JoltC_PhysicsSystem_Init(PhysicsSystem, maxBodies, numBodyMutexes, maxBodyPairs, maxContactConstraints, BroadPhase, ObjectVsBroadPhaseFilter, ObjectLayerPairFilter);

		BodyInterface = JoltPhysicsNative.JoltC_PhysicsSystem_GetBodyInterface(PhysicsSystem);
		if (BodyInterface == IntPtr.Zero)
			throw new InvalidOperationException($"Failed to get BodyInterface: {GetLastErrorString()}");

		TempAllocator = JoltPhysicsNative.JoltC_TempAllocator_Create(10 * 1024 * 1024);
		if (TempAllocator == IntPtr.Zero)
			throw new InvalidOperationException($"Failed to create TempAllocator: {GetLastErrorString()}");

		JobSystem = JoltPhysicsNative.JoltC_JobSystemThreadPool_Create(2048, 8, 1);
		if (JobSystem == IntPtr.Zero)
			throw new InvalidOperationException($"Failed to create JobSystem: {GetLastErrorString()}");

		JoltPhysicsNative.JoltC_PhysicsSystem_SetGravity(PhysicsSystem, NativeTestHelpers.Vec3(0f, -9.81f, 0f));
	}

	public uint CreateStaticBox(JoltC_Vec3 halfExtent, JoltC_RVec3 position)
	{
		var shape = JoltPhysicsNative.JoltC_BoxShape_Create(halfExtent, 0.05f);

		JoltC_BodyCreationSettings settings = default;
		JoltPhysicsNative.JoltC_BodyCreationSettings_SetDefault(&settings);
		settings.shape = shape;
		settings.motionType = JoltC_MotionType.JOLTC_MOTION_TYPE_STATIC;
		settings.position = position;
		settings.objectLayer = LayerNonMoving;

		uint bodyId = JoltPhysicsNative.JoltC_BodyInterface_CreateBody(BodyInterface, &settings);
		if (bodyId == 0)
		{
			throw new InvalidOperationException($"CreateBody(static) failed: {GetLastErrorString()}");
		}
		JoltPhysicsNative.JoltC_BodyInterface_AddBody(BodyInterface, bodyId, JoltC_Activation.JOLTC_ACTIVATION_DONT_ACTIVATE);
		_bodyIds.Add(bodyId);
		_shapes.Add(shape);
		return bodyId;
	}

	public uint CreateDynamicBox(JoltC_Vec3 halfExtent, JoltC_RVec3 position)
	{
		var shape = JoltPhysicsNative.JoltC_BoxShape_Create(halfExtent, 0.05f);

		JoltC_BodyCreationSettings settings = default;
		JoltPhysicsNative.JoltC_BodyCreationSettings_SetDefault(&settings);
		settings.shape = shape;
		settings.motionType = JoltC_MotionType.JOLTC_MOTION_TYPE_DYNAMIC;
		settings.position = position;
		settings.objectLayer = LayerMoving;

		uint bodyId = JoltPhysicsNative.JoltC_BodyInterface_CreateBody(BodyInterface, &settings);
		if (bodyId == 0)
		{
			throw new InvalidOperationException($"CreateBody(dynamic) failed: {GetLastErrorString()}");
		}
		JoltPhysicsNative.JoltC_BodyInterface_AddBody(BodyInterface, bodyId, JoltC_Activation.JOLTC_ACTIVATION_ACTIVATE);
		_bodyIds.Add(bodyId);
		_shapes.Add(shape);
		return bodyId;
	}

	public IntPtr CreateFixedConstraint(uint body1, uint body2)
	{
		JoltC_FixedConstraintSettings settings = default;
		JoltPhysicsNative.JoltC_FixedConstraintSettings_Init(&settings);
		settings.space = JoltC_ConstraintSpace.JOLTC_CONSTRAINT_SPACE_WORLD_SPACE;
		settings.autoDetectPoint = 1;
		settings.axisX1 = NativeTestHelpers.Vec3(1f, 0f, 0f);
		settings.axisY1 = NativeTestHelpers.Vec3(0f, 1f, 0f);
		settings.axisX2 = NativeTestHelpers.Vec3(1f, 0f, 0f);
		settings.axisY2 = NativeTestHelpers.Vec3(0f, 1f, 0f);

		IntPtr constraint = JoltPhysicsNative.JoltC_FixedConstraint_Create(PhysicsSystem, body1, body2, &settings);
		IntPtr* constraintPtr = stackalloc IntPtr[1] { constraint };
		JoltPhysicsNative.JoltC_PhysicsSystem_AddConstraints(PhysicsSystem, constraintPtr, 1);
		_constraints.Add(constraint);
		return constraint;
	}

	public uint Step(float deltaTime = 1f / 60f, int collisionSteps = 1)
	{
		return JoltPhysicsNative.JoltC_PhysicsSystem_Update(PhysicsSystem, deltaTime, collisionSteps, TempAllocator, JobSystem);
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
			JoltPhysicsNative.JoltC_BodyInterface_RemoveBody(BodyInterface, bodyId);
			JoltPhysicsNative.JoltC_BodyInterface_DestroyBody(BodyInterface, bodyId);
		}
		_bodyIds.Clear();

		foreach (IntPtr constraint in _constraints)
		{
			var constraintPtrArray = new IntPtr[1] { constraint };
			fixed (IntPtr* constraintPtr = constraintPtrArray)
			{
				JoltPhysicsNative.JoltC_PhysicsSystem_RemoveConstraints(PhysicsSystem, constraintPtr, 1);
			}
			JoltPhysicsNative.JoltC_Constraint_Destroy(constraint);
		}
		_constraints.Clear();

		foreach (IntPtr shape in _shapes)
		{
			JoltPhysicsNative.JoltC_Shape_Destroy(shape);
		}
		_shapes.Clear();

		JoltPhysicsNative.JoltC_PhysicsSystem_Destroy(PhysicsSystem);

		JoltPhysicsNative.JoltC_ObjectLayerPairFilter_Destroy(ObjectLayerPairFilter);
		JoltPhysicsNative.JoltC_ObjectVsBroadPhaseLayerFilter_Destroy(ObjectVsBroadPhaseFilter);
		JoltPhysicsNative.JoltC_BroadPhaseLayerInterface_Destroy(BroadPhase);

		JoltPhysicsNative.JoltC_JobSystem_Destroy(JobSystem);
		JoltPhysicsNative.JoltC_TempAllocator_Destroy(TempAllocator);
	}
}
