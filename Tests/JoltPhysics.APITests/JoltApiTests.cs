using System;
using System.Runtime.InteropServices;
using Evergine.Bindings.JoltPhysics;
using FluentAssertions;
using Xunit;
using static JoltPhysicsTests.NativeTestHelpers;

namespace JoltPhysicsTests;

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
		RVec3 initialPos = JoltPhysics.BodyInterface_GetCenterOfMassPosition(env.BodyInterface, boxId);
		Quat initialRot = JoltPhysics.BodyInterface_GetRotation(env.BodyInterface, boxId);

		initialPos.Y.Should().BeApproximately(5f, 0.01f, "box should start at y=5");
		initialRot.W.Should().BeApproximately(1f, 0.01f, "box should start with identity rotation");

		// Optimize the broad phase after adding bodies
		JoltPhysics.PhysicsSystem_OptimizeBroadPhase(env.PhysicsSystem);

		// Step the simulation for ~1 second
		const int numSteps = 60;
		for (int i = 0; i < numSteps; i++)
		{
			env.Step(1f / 60f);
		}

		// Read updated position and rotation
		RVec3 finalPos = JoltPhysics.BodyInterface_GetCenterOfMassPosition(env.BodyInterface, boxId);
		Quat finalRot = JoltPhysics.BodyInterface_GetRotation(env.BodyInterface, boxId);

		// After ~1s of gravity the box must have fallen
		finalPos.Y.Should().BeLessThan(initialPos.Y, "gravity should move the box downward");
		finalPos.Y.Should().BeGreaterThan(-2f, "the floor should stop the box");

		// Rotation quaternion should still be valid (unit length)
		float rotLen = MathF.Sqrt(finalRot.X * finalRot.X + finalRot.Y * finalRot.Y + finalRot.Z * finalRot.Z + finalRot.W * finalRot.W);
		rotLen.Should().BeApproximately(1f, 0.01f, "rotation quaternion should be unit length");

		// Verify the physics system tracked the bodies
		JoltPhysics.PhysicsSystem_GetNumBodies(env.PhysicsSystem).Should().Be(2);
	}

	[Fact]
	public void Physics_system_initializes_and_steps()
	{
		using var env = new JoltTestEnvironment();

		JoltPhysics.PhysicsSystem_GetMaxBodies(env.PhysicsSystem).Should().BeGreaterThan(0);
		JoltPhysics.PhysicsSystem_GetNumBodies(env.PhysicsSystem).Should().Be(0);

		JoltPhysics.PhysicsSystem_OptimizeBroadPhase(env.PhysicsSystem);
		env.Step();
	}

	[Fact]
	public void Body_creation_and_gravity_moves_body()
	{
		using var env = new JoltTestEnvironment();
		env.CreateStaticBox(Vec3(50f, 1f, 50f), RVec3(0f, -1f, 0f));
		uint bodyId = env.CreateDynamicBox(Vec3(0.5f, 0.5f, 0.5f), RVec3(0f, 2f, 0f));

		JoltPhysics.PhysicsSystem_OptimizeBroadPhase(env.PhysicsSystem);

		for (int i = 0; i < 90; i++)
		{
			env.Step();
		}

		RVec3 position = JoltPhysics.BodyInterface_GetCenterOfMassPosition(env.BodyInterface, bodyId);
		position.Y.Should().BeLessThan(1.5f, "gravity should move the body down");
		JoltPhysics.PhysicsSystem_GetNumBodies(env.PhysicsSystem).Should().Be(2);
	}

	[Fact]
	public void Shape_creation_returns_expected_properties()
	{
		IntPtr box = JoltPhysics.BoxShape_Create(Vec3(1f, 2f, 3f), 0.05f);
		try
		{
			JoltPhysics.Shape_GetType(box).Should().Be(ShapeType.Convex);
			JoltPhysics.Shape_GetSubType(box).Should().Be(ShapeSubType.Box);
			Vec3 halfExtent = JoltPhysics.BoxShape_GetHalfExtent(box);
			halfExtent.X.Should().BeApproximately(1f, 1e-5f);
			halfExtent.Y.Should().BeApproximately(2f, 1e-5f);
			halfExtent.Z.Should().BeApproximately(3f, 1e-5f);
		}
		finally
		{
			JoltPhysics.Shape_Destroy(box);
		}
	}

	[Fact]
	public void Character_virtual_can_create_and_update()
	{
		using var env = new JoltTestEnvironment();
		IntPtr capsule = JoltPhysics.CapsuleShape_Create(0.5f, 0.3f);
		IntPtr character = IntPtr.Zero;
		try
		{
			CharacterVirtualSettings settings = default;
			JoltPhysics.CharacterVirtualSettings_SetDefault(&settings);
			settings .Shape = capsule;
			settings.Mass = 50f;
			settings.MaxStrength = 2000f;

			character = JoltPhysics.CharacterVirtual_Create(&settings, RVec3(0f, 2f, 0f), IdentityQuat(), 0, env.PhysicsSystem);
			character.Should().NotBe(IntPtr.Zero);

			JoltPhysics.CharacterVirtual_SetLinearVelocity(character, Vec3(0f, 0f, 0f));
			JoltPhysics.CharacterVirtual_Update(character, 0.016f, Vec3(0f, -9.81f, 0f), env.TempAllocator);

			RVec3 pos = JoltPhysics.CharacterVirtual_GetPosition(character);
			pos.Y.Should().BeLessThan(2.1f);
		}
		finally
		{
			if (character != IntPtr.Zero)
			{
				JoltPhysics.CharacterVirtual_Destroy(character);
			}
			JoltPhysics.Shape_Destroy(capsule);
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

		uint before = JoltPhysics.PhysicsSystem_GetNumConstraints(env.PhysicsSystem);
		before.Should().BeGreaterThan(0u);
	}

	[Fact]
	public void Filters_mask_roundtrip()
	{
		ushort layer = JoltPhysics.ObjectLayerPairFilterMask_GetObjectLayer(1, 2);
		uint group = JoltPhysics.ObjectLayerPairFilterMask_GetGroup(layer);
		uint mask = JoltPhysics.ObjectLayerPairFilterMask_GetMask(layer);

		layer.Should().NotBe(0);
		group.Should().Be(1);
		mask.Should().Be(2);
	}

	[Fact]
	public void Skeleton_allows_joint_addition()
	{
		IntPtr skeleton = JoltPhysics.Skeleton_Create();
		try
		{
			uint hipIndex = JoltPhysics.Skeleton_AddJoint(skeleton, "Hip");
			uint kneeIndex = JoltPhysics.Skeleton_AddJoint2(skeleton, "Knee", (int)hipIndex);

			JoltPhysics.Skeleton_GetJointCount(skeleton).Should().Be(2);
			JoltPhysics.Skeleton_AreJointsCorrectlyOrdered(skeleton).Should().BeTrue();
			SkeletonJoint joint = default;
			JoltPhysics.Skeleton_GetJoint(skeleton, (int)kneeIndex, &joint);
			joint.ParentJointIndex.Should().Be((int)hipIndex);
		}
		finally
		{
			JoltPhysics.Skeleton_Destroy(skeleton);
		}
	}

	[Fact]
	public void Vehicle_wheel_settings_roundtrip()
	{
		using var env = new JoltTestEnvironment();
		IntPtr wheelSettings = JoltPhysics.WheelSettings_Create();
		try
		{
			JoltPhysics.WheelSettings_SetPosition(wheelSettings, Vec3(0f, 1f, 0f));
			Vec3 pos = JoltPhysics.WheelSettings_GetPosition(wheelSettings);
			pos.Y.Should().BeApproximately(1f, 1e-5f);

			VehicleConstraintSettings settings = default;
			JoltPhysics.VehicleConstraintSettings_Init(&settings);
			settings.MaxPitchRollAngle = 0.35f;
			settings.MaxPitchRollAngle.Should().BeApproximately(0.35f, 1e-5f);
		}
		finally
		{
			JoltPhysics.WheelSettings_Destroy(wheelSettings);
		}
	}
}
