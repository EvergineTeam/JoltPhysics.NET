using System;
using Evergine.Bindings.JoltPhysics;
using FluentAssertions;
using Xunit;
using static JoltPhysics.APITests.NativeTestHelpers;

namespace JoltPhysics.APITests;

[Collection("Jolt")]
public unsafe class ConstraintApiTests
{
	[Fact]
	public void PointConstraint_create_and_add()
	{
		using var env = new JoltTestEnvironment();
		uint bodyA = env.CreateDynamicBox(Vec3(0.5f, 0.5f, 0.5f), RVec3(0f, 3f, 0f));
		uint bodyB = env.CreateDynamicBox(Vec3(0.5f, 0.5f, 0.5f), RVec3(1f, 3f, 0f));

		JoltC_PointConstraintSettings settings = default;
		JoltPhysicsNative.JoltC_PointConstraintSettings_Init(&settings);
		settings.space = JoltC_ConstraintSpace.JOLTC_CONSTRAINT_SPACE_WORLD_SPACE;
		settings.point1 = RVec3(0.5f, 3f, 0f);
		settings.point2 = RVec3(0.5f, 3f, 0f);

		IntPtr constraint = JoltPhysicsNative.JoltC_PointConstraint_Create(env.PhysicsSystem, bodyA, bodyB, &settings);
		constraint.Should().NotBe(IntPtr.Zero);

		IntPtr* ptr = stackalloc IntPtr[1] { constraint };
		JoltPhysicsNative.JoltC_PhysicsSystem_AddConstraints(env.PhysicsSystem, ptr, 1);

		JoltPhysicsNative.JoltC_PhysicsSystem_GetNumConstraints(env.PhysicsSystem).Should().Be(1u);

		// Cleanup
		JoltPhysicsNative.JoltC_PhysicsSystem_RemoveConstraints(env.PhysicsSystem, ptr, 1);
		JoltPhysicsNative.JoltC_Constraint_Destroy(constraint);
	}

	[Fact]
	public void DistanceConstraint_create_and_add()
	{
		using var env = new JoltTestEnvironment();
		uint bodyA = env.CreateDynamicBox(Vec3(0.5f, 0.5f, 0.5f), RVec3(0f, 3f, 0f));
		uint bodyB = env.CreateDynamicBox(Vec3(0.5f, 0.5f, 0.5f), RVec3(2f, 3f, 0f));

		JoltC_DistanceConstraintSettings settings = default;
		JoltPhysicsNative.JoltC_DistanceConstraintSettings_Init(&settings);
		settings.space = JoltC_ConstraintSpace.JOLTC_CONSTRAINT_SPACE_WORLD_SPACE;
		settings.point1 = RVec3(0f, 3f, 0f);
		settings.point2 = RVec3(2f, 3f, 0f);
		settings.minDistance = 1.5f;
		settings.maxDistance = 2.5f;

		IntPtr constraint = JoltPhysicsNative.JoltC_DistanceConstraint_Create(env.PhysicsSystem, bodyA, bodyB, &settings);
		constraint.Should().NotBe(IntPtr.Zero);

		IntPtr* ptr = stackalloc IntPtr[1] { constraint };
		JoltPhysicsNative.JoltC_PhysicsSystem_AddConstraints(env.PhysicsSystem, ptr, 1);

		JoltPhysicsNative.JoltC_PhysicsSystem_GetNumConstraints(env.PhysicsSystem).Should().Be(1u);

		JoltPhysicsNative.JoltC_PhysicsSystem_RemoveConstraints(env.PhysicsSystem, ptr, 1);
		JoltPhysicsNative.JoltC_Constraint_Destroy(constraint);
	}

	[Fact]
	public void HingeConstraint_create_and_add()
	{
		using var env = new JoltTestEnvironment();
		uint bodyA = env.CreateDynamicBox(Vec3(0.5f, 0.5f, 0.5f), RVec3(0f, 3f, 0f));
		uint bodyB = env.CreateDynamicBox(Vec3(0.5f, 0.5f, 0.5f), RVec3(1f, 3f, 0f));

		JoltC_HingeConstraintSettings settings = default;
		JoltPhysicsNative.JoltC_HingeConstraintSettings_Init(&settings);
		settings.space = JoltC_ConstraintSpace.JOLTC_CONSTRAINT_SPACE_WORLD_SPACE;
		settings.point1 = RVec3(0.5f, 3f, 0f);
		settings.hingeAxis1 = Vec3(0f, 1f, 0f);
		settings.normalAxis1 = Vec3(1f, 0f, 0f);
		settings.point2 = RVec3(0.5f, 3f, 0f);
		settings.hingeAxis2 = Vec3(0f, 1f, 0f);
		settings.normalAxis2 = Vec3(1f, 0f, 0f);
		settings.limitsMin = -MathF.PI;
		settings.limitsMax = MathF.PI;

		IntPtr constraint = JoltPhysicsNative.JoltC_HingeConstraint_Create(env.PhysicsSystem, bodyA, bodyB, &settings);
		constraint.Should().NotBe(IntPtr.Zero);

		IntPtr* ptr = stackalloc IntPtr[1] { constraint };
		JoltPhysicsNative.JoltC_PhysicsSystem_AddConstraints(env.PhysicsSystem, ptr, 1);

		JoltPhysicsNative.JoltC_PhysicsSystem_GetNumConstraints(env.PhysicsSystem).Should().Be(1u);

		JoltPhysicsNative.JoltC_PhysicsSystem_RemoveConstraints(env.PhysicsSystem, ptr, 1);
		JoltPhysicsNative.JoltC_Constraint_Destroy(constraint);
	}

	[Fact]
	public void SliderConstraint_create_and_add()
	{
		using var env = new JoltTestEnvironment();
		uint bodyA = env.CreateDynamicBox(Vec3(0.5f, 0.5f, 0.5f), RVec3(0f, 3f, 0f));
		uint bodyB = env.CreateDynamicBox(Vec3(0.5f, 0.5f, 0.5f), RVec3(1f, 3f, 0f));

		JoltC_SliderConstraintSettings settings = default;
		JoltPhysicsNative.JoltC_SliderConstraintSettings_Init(&settings);
		settings.space = JoltC_ConstraintSpace.JOLTC_CONSTRAINT_SPACE_WORLD_SPACE;
		settings.autoDetectPoint = 1;
		settings.sliderAxis1 = Vec3(1f, 0f, 0f);
		settings.normalAxis1 = Vec3(0f, 1f, 0f);
		settings.sliderAxis2 = Vec3(1f, 0f, 0f);
		settings.normalAxis2 = Vec3(0f, 1f, 0f);
		settings.limitsMin = -5f;
		settings.limitsMax = 5f;

		IntPtr constraint = JoltPhysicsNative.JoltC_SliderConstraint_Create(env.PhysicsSystem, bodyA, bodyB, &settings);
		constraint.Should().NotBe(IntPtr.Zero);

		IntPtr* ptr = stackalloc IntPtr[1] { constraint };
		JoltPhysicsNative.JoltC_PhysicsSystem_AddConstraints(env.PhysicsSystem, ptr, 1);

		JoltPhysicsNative.JoltC_PhysicsSystem_GetNumConstraints(env.PhysicsSystem).Should().Be(1u);

		JoltPhysicsNative.JoltC_PhysicsSystem_RemoveConstraints(env.PhysicsSystem, ptr, 1);
		JoltPhysicsNative.JoltC_Constraint_Destroy(constraint);
	}

	[Fact]
	public void ConeConstraint_create_and_add()
	{
		using var env = new JoltTestEnvironment();
		uint bodyA = env.CreateDynamicBox(Vec3(0.5f, 0.5f, 0.5f), RVec3(0f, 3f, 0f));
		uint bodyB = env.CreateDynamicBox(Vec3(0.5f, 0.5f, 0.5f), RVec3(1f, 3f, 0f));

		JoltC_ConeConstraintSettings settings = default;
		JoltPhysicsNative.JoltC_ConeConstraintSettings_Init(&settings);
		settings.space = JoltC_ConstraintSpace.JOLTC_CONSTRAINT_SPACE_WORLD_SPACE;
		settings.point1 = RVec3(0.5f, 3f, 0f);
		settings.twistAxis1 = Vec3(1f, 0f, 0f);
		settings.point2 = RVec3(0.5f, 3f, 0f);
		settings.twistAxis2 = Vec3(1f, 0f, 0f);
		settings.halfConeAngle = MathF.PI / 4f;

		IntPtr constraint = JoltPhysicsNative.JoltC_ConeConstraint_Create(env.PhysicsSystem, bodyA, bodyB, &settings);
		constraint.Should().NotBe(IntPtr.Zero);

		IntPtr* ptr = stackalloc IntPtr[1] { constraint };
		JoltPhysicsNative.JoltC_PhysicsSystem_AddConstraints(env.PhysicsSystem, ptr, 1);

		JoltPhysicsNative.JoltC_PhysicsSystem_GetNumConstraints(env.PhysicsSystem).Should().Be(1u);

		JoltPhysicsNative.JoltC_PhysicsSystem_RemoveConstraints(env.PhysicsSystem, ptr, 1);
		JoltPhysicsNative.JoltC_Constraint_Destroy(constraint);
	}

	[Fact]
	public void FixedConstraint_holds_bodies_together_during_simulation()
	{
		using var env = new JoltTestEnvironment();
		uint bodyA = env.CreateDynamicBox(Vec3(0.5f, 0.5f, 0.5f), RVec3(0f, 5f, 0f));
		uint bodyB = env.CreateDynamicBox(Vec3(0.5f, 0.5f, 0.5f), RVec3(0.5f, 5f, 0f));

		IntPtr constraint = env.CreateFixedConstraint(bodyA, bodyB);
		constraint.Should().NotBe(IntPtr.Zero);

		JoltPhysicsNative.JoltC_PhysicsSystem_OptimizeBroadPhase(env.PhysicsSystem);

		// Step a few times
		for (int i = 0; i < 30; i++)
			env.Step();

		JoltC_RVec3 posA = JoltPhysicsNative.JoltC_BodyInterface_GetCenterOfMassPosition(env.BodyInterface, bodyA);
		JoltC_RVec3 posB = JoltPhysicsNative.JoltC_BodyInterface_GetCenterOfMassPosition(env.BodyInterface, bodyB);

		// Bodies should have fallen (gravity) but stayed close together
		float dx = posA.x - posB.x;
		float dy = posA.y - posB.y;
		float dz = posA.z - posB.z;
		float dist = MathF.Sqrt(dx * dx + dy * dy + dz * dz);
		dist.Should().BeLessThan(1.5f, "fixed constraint should keep bodies close together");
	}

	[Fact]
	public void Multiple_constraints_can_coexist()
	{
		using var env = new JoltTestEnvironment();
		uint bodyA = env.CreateDynamicBox(Vec3(0.5f, 0.5f, 0.5f), RVec3(0f, 3f, 0f));
		uint bodyB = env.CreateDynamicBox(Vec3(0.5f, 0.5f, 0.5f), RVec3(1f, 3f, 0f));
		uint bodyC = env.CreateDynamicBox(Vec3(0.5f, 0.5f, 0.5f), RVec3(2f, 3f, 0f));

		IntPtr c1 = env.CreateFixedConstraint(bodyA, bodyB);
		IntPtr c2 = env.CreateFixedConstraint(bodyB, bodyC);

		c1.Should().NotBe(IntPtr.Zero);
		c2.Should().NotBe(IntPtr.Zero);

		JoltPhysicsNative.JoltC_PhysicsSystem_GetNumConstraints(env.PhysicsSystem).Should().Be(2u);
	}
}
