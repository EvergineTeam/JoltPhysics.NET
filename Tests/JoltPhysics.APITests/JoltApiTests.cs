using System;
using System.Runtime.InteropServices;
using Evergine.Bindings.JoltPhysics;
using FluentAssertions;
using Xunit;
using static JoltPhysics.APITests.NativeTestHelpers;

namespace JoltPhysics.APITests;

[Collection("Jolt")]
public unsafe class JoltApiTests
{
	[Fact]
	public void HelloWorld_create_box_step_and_read_position_rotation()
	{
		using var env = new JoltTestEnvironment();

		// Create a static floor
		env.CreateStaticBox(Vec3(100f, 1f, 100f), RVec3(0f, -1f, 0f));

		// Create a dynamic box above the floor
		uint boxId = env.CreateDynamicBox(Vec3(0.5f, 0.5f, 0.5f), RVec3(0f, 5f, 0f));

		// Read initial position and rotation
		JoltC_RVec3 initialPos = JoltPhysicsNative.JoltC_BodyInterface_GetCenterOfMassPosition(env.BodyInterface, boxId);
		JoltC_Quat initialRot = JoltPhysicsNative.JoltC_BodyInterface_GetRotation(env.BodyInterface, boxId);

		initialPos.y.Should().BeApproximately(5f, 0.01f, "box should start at y=5");
		initialRot.w.Should().BeApproximately(1f, 0.01f, "box should start with identity rotation");

		// Optimize the broad phase after adding bodies
		JoltPhysicsNative.JoltC_PhysicsSystem_OptimizeBroadPhase(env.PhysicsSystem);

		// Step the simulation for ~1 second
		const int numSteps = 60;
		for (int i = 0; i < numSteps; i++)
		{
			env.Step(1f / 60f);
		}

		// Read updated position and rotation
		JoltC_RVec3 finalPos = JoltPhysicsNative.JoltC_BodyInterface_GetCenterOfMassPosition(env.BodyInterface, boxId);
		JoltC_Quat finalRot = JoltPhysicsNative.JoltC_BodyInterface_GetRotation(env.BodyInterface, boxId);

		// After ~1s of gravity the box must have fallen
		finalPos.y.Should().BeLessThan(initialPos.y, "gravity should move the box downward");
		finalPos.y.Should().BeGreaterThan(-2f, "the floor should stop the box");

		// Rotation quaternion should still be valid (unit length)
		float rotLen = MathF.Sqrt(finalRot.x * finalRot.x + finalRot.y * finalRot.y + finalRot.z * finalRot.z + finalRot.w * finalRot.w);
		rotLen.Should().BeApproximately(1f, 0.01f, "rotation quaternion should be unit length");

		// Verify the physics system tracked the bodies
		JoltPhysicsNative.JoltC_PhysicsSystem_GetNumBodies(env.PhysicsSystem).Should().Be(2);
	}

	[Fact]
	public void Physics_system_initializes_and_steps()
	{
		using var env = new JoltTestEnvironment();

		JoltPhysicsNative.JoltC_PhysicsSystem_GetMaxBodies(env.PhysicsSystem).Should().BeGreaterThan(0);
		JoltPhysicsNative.JoltC_PhysicsSystem_GetNumBodies(env.PhysicsSystem).Should().Be(0);

		JoltPhysicsNative.JoltC_PhysicsSystem_OptimizeBroadPhase(env.PhysicsSystem);
		env.Step();
	}

	[Fact]
	public void Body_creation_and_gravity_moves_body()
	{
		using var env = new JoltTestEnvironment();
		env.CreateStaticBox(Vec3(50f, 1f, 50f), RVec3(0f, -1f, 0f));
		uint bodyId = env.CreateDynamicBox(Vec3(0.5f, 0.5f, 0.5f), RVec3(0f, 2f, 0f));

		JoltPhysicsNative.JoltC_PhysicsSystem_OptimizeBroadPhase(env.PhysicsSystem);

		for (int i = 0; i < 90; i++)
		{
			env.Step();
		}

		JoltC_RVec3 position = JoltPhysicsNative.JoltC_BodyInterface_GetCenterOfMassPosition(env.BodyInterface, bodyId);
		position.y.Should().BeLessThan(1.5f, "gravity should move the body down");
		JoltPhysicsNative.JoltC_PhysicsSystem_GetNumBodies(env.PhysicsSystem).Should().Be(2);
	}

	[Fact]
	public void Shape_creation_returns_expected_properties()
	{
		IntPtr box = JoltPhysicsNative.JoltC_BoxShape_Create(Vec3(1f, 2f, 3f), 0.05f);
		try
		{
			JoltPhysicsNative.JoltC_Shape_GetType(box).Should().Be(JoltC_ShapeType.JOLTC_SHAPE_TYPE_CONVEX);
			JoltPhysicsNative.JoltC_Shape_GetSubType(box).Should().Be(JoltC_ShapeSubType.JOLTC_SHAPE_SUB_TYPE_BOX);
			JoltC_Vec3 halfExtent = JoltPhysicsNative.JoltC_BoxShape_GetHalfExtent(box);
			halfExtent.x.Should().BeApproximately(1f, 1e-5f);
			halfExtent.y.Should().BeApproximately(2f, 1e-5f);
			halfExtent.z.Should().BeApproximately(3f, 1e-5f);
		}
		finally
		{
			JoltPhysicsNative.JoltC_Shape_Destroy(box);
		}
	}

	[Fact]
	public void Character_virtual_can_create_and_update()
	{
		using var env = new JoltTestEnvironment();
		IntPtr capsule = JoltPhysicsNative.JoltC_CapsuleShape_Create(0.5f, 0.3f);
		IntPtr character = IntPtr.Zero;
		try
		{
			JoltC_CharacterVirtualSettings settings = default;
			JoltPhysicsNative.JoltC_CharacterVirtualSettings_SetDefault(&settings);
			settings.shape = capsule;
			settings.mass = 50f;
			settings.maxStrength = 2000f;

			character = JoltPhysicsNative.JoltC_CharacterVirtual_Create(&settings, RVec3(0f, 2f, 0f), IdentityQuat(), 0, env.PhysicsSystem);
			character.Should().NotBe(IntPtr.Zero);

			JoltPhysicsNative.JoltC_CharacterVirtual_SetLinearVelocity(character, Vec3(0f, 0f, 0f));
			JoltPhysicsNative.JoltC_CharacterVirtual_Update(character, 0.016f, Vec3(0f, -9.81f, 0f), env.TempAllocator);

			JoltC_RVec3 pos = JoltPhysicsNative.JoltC_CharacterVirtual_GetPosition(character);
			pos.y.Should().BeLessThan(2.1f);
		}
		finally
		{
			if (character != IntPtr.Zero)
			{
				JoltPhysicsNative.JoltC_CharacterVirtual_Destroy(character);
			}
			JoltPhysicsNative.JoltC_Shape_Destroy(capsule);
		}
	}

	[Fact]
	public void Constraint_can_attach_two_bodies()
	{
		using var env = new JoltTestEnvironment();
		uint bodyA = env.CreateDynamicBox(Vec3(0.5f, 0.5f, 0.5f), RVec3(0f, 3f, 0f));
		uint bodyB = env.CreateDynamicBox(Vec3(0.5f, 0.5f, 0.5f), RVec3(0.5f, 4f, 0f));

		IntPtr constraint = env.CreateFixedConstraint(bodyA, bodyB);
		constraint.Should().NotBe(IntPtr.Zero);

		uint before = JoltPhysicsNative.JoltC_PhysicsSystem_GetNumConstraints(env.PhysicsSystem);
		before.Should().BeGreaterThan(0u);
	}

	[Fact]
	public void Filters_mask_roundtrip()
	{
		ushort layer = JoltPhysicsNative.JoltC_ObjectLayerPairFilterMask_GetObjectLayer(1, 2);
		uint group = JoltPhysicsNative.JoltC_ObjectLayerPairFilterMask_GetGroup(layer);
		uint mask = JoltPhysicsNative.JoltC_ObjectLayerPairFilterMask_GetMask(layer);

		layer.Should().NotBe(0);
		group.Should().Be(1);
		mask.Should().Be(2);
	}

	[Fact]
	public void Skeleton_allows_joint_addition()
	{
		IntPtr skeleton = JoltPhysicsNative.JoltC_Skeleton_Create();
		try
		{
			byte* hip = stackalloc byte[] { (byte)'H', (byte)'i', (byte)'p', 0 };
			byte* knee = stackalloc byte[] { (byte)'K', (byte)'n', (byte)'e', (byte)'e', 0 };

			uint hipIndex = JoltPhysicsNative.JoltC_Skeleton_AddJoint(skeleton, hip);
			uint kneeIndex = JoltPhysicsNative.JoltC_Skeleton_AddJoint2(skeleton, knee, (int)hipIndex);

			JoltPhysicsNative.JoltC_Skeleton_GetJointCount(skeleton).Should().Be(2);
			JoltPhysicsNative.JoltC_Skeleton_AreJointsCorrectlyOrdered(skeleton).Should().Be(1);
			JoltC_SkeletonJoint joint = default;
			JoltPhysicsNative.JoltC_Skeleton_GetJoint(skeleton, (int)kneeIndex, &joint);
			joint.parentJointIndex.Should().Be((int)hipIndex);
		}
		finally
		{
			JoltPhysicsNative.JoltC_Skeleton_Destroy(skeleton);
		}
	}

	[Fact]
	public void Vehicle_wheel_settings_roundtrip()
	{
		using var env = new JoltTestEnvironment();
		IntPtr wheelSettings = JoltPhysicsNative.JoltC_WheelSettings_Create();
		try
		{
			JoltPhysicsNative.JoltC_WheelSettings_SetPosition(wheelSettings, Vec3(0f, 1f, 0f));
			JoltC_Vec3 pos = JoltPhysicsNative.JoltC_WheelSettings_GetPosition(wheelSettings);
			pos.y.Should().BeApproximately(1f, 1e-5f);

			JoltC_VehicleConstraintSettings settings = default;
			JoltPhysicsNative.JoltC_VehicleConstraintSettings_Init(&settings);
			settings.maxPitchRollAngle = 0.35f;
			settings.maxPitchRollAngle.Should().BeApproximately(0.35f, 1e-5f);
		}
		finally
		{
			JoltPhysicsNative.JoltC_WheelSettings_Destroy(wheelSettings);
		}
	}
}
