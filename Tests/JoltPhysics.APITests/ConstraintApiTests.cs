using System;
using Evergine.Bindings.JoltPhysics;
using FluentAssertions;
using Xunit;
using static JoltPhysicsTests.NativeTestHelpers;

namespace JoltPhysicsTests;

[Collection("Jolt")]
public unsafe class ConstraintApiTests
{
	[Fact]
	public void PointConstraint_create_and_add()
	{
		using var env = new JoltTestEnvironment();
		uint bodyA = env.CreateDynamicBox(Vec3(0.5f, 0.5f, 0.5f), RVec3(0f, 3f, 0f));
		uint bodyB = env.CreateDynamicBox(Vec3(0.5f, 0.5f, 0.5f), RVec3(1f, 3f, 0f));

		PointConstraintSettings settings = default;
		JoltPhysics.PointConstraintSettings_Init(&settings);
		settings.Space = ConstraintSpace.WorldSpace;
		settings.Point1 = RVec3(0.5f, 3f, 0f);
		settings.Point2 = RVec3(0.5f, 3f, 0f);

		IntPtr constraint = JoltPhysics.PointConstraint_Create(env.PhysicsSystem, bodyA, bodyB, &settings);
		constraint.Should().NotBe(IntPtr.Zero);

		IntPtr* ptr = stackalloc IntPtr[1] { constraint };
		JoltPhysics.PhysicsSystem_AddConstraints(env.PhysicsSystem, ptr, 1);

		JoltPhysics.PhysicsSystem_GetNumConstraints(env.PhysicsSystem).Should().Be(1u);

		// Cleanup
		JoltPhysics.PhysicsSystem_RemoveConstraints(env.PhysicsSystem, ptr, 1);
		JoltPhysics.Constraint_Destroy(constraint);
	}

	[Fact]
	public void DistanceConstraint_create_and_add()
	{
		using var env = new JoltTestEnvironment();
		uint bodyA = env.CreateDynamicBox(Vec3(0.5f, 0.5f, 0.5f), RVec3(0f, 3f, 0f));
		uint bodyB = env.CreateDynamicBox(Vec3(0.5f, 0.5f, 0.5f), RVec3(2f, 3f, 0f));

		DistanceConstraintSettings settings = default;
		JoltPhysics.DistanceConstraintSettings_Init(&settings);
		settings.Space = ConstraintSpace.WorldSpace;
		settings.Point1 = RVec3(0f, 3f, 0f);
		settings.Point2 = RVec3(2f, 3f, 0f);
		settings.MinDistance = 1.5f;
		settings.MaxDistance = 2.5f;

		IntPtr constraint = JoltPhysics.DistanceConstraint_Create(env.PhysicsSystem, bodyA, bodyB, &settings);
		constraint.Should().NotBe(IntPtr.Zero);

		IntPtr* ptr = stackalloc IntPtr[1] { constraint };
		JoltPhysics.PhysicsSystem_AddConstraints(env.PhysicsSystem, ptr, 1);

		JoltPhysics.PhysicsSystem_GetNumConstraints(env.PhysicsSystem).Should().Be(1u);

		JoltPhysics.PhysicsSystem_RemoveConstraints(env.PhysicsSystem, ptr, 1);
		JoltPhysics.Constraint_Destroy(constraint);
	}

	[Fact]
	public void HingeConstraint_create_and_add()
	{
		using var env = new JoltTestEnvironment();
		uint bodyA = env.CreateDynamicBox(Vec3(0.5f, 0.5f, 0.5f), RVec3(0f, 3f, 0f));
		uint bodyB = env.CreateDynamicBox(Vec3(0.5f, 0.5f, 0.5f), RVec3(1f, 3f, 0f));

		HingeConstraintSettings settings = default;
		JoltPhysics.HingeConstraintSettings_Init(&settings);
		settings.Space = ConstraintSpace.WorldSpace;
		settings.Point1 = RVec3(0.5f, 3f, 0f);
		settings.HingeAxis1 = Vec3(0f, 1f, 0f);
		settings.NormalAxis1 = Vec3(1f, 0f, 0f);
		settings.Point2 = RVec3(0.5f, 3f, 0f);
		settings.HingeAxis2 = Vec3(0f, 1f, 0f);
		settings.NormalAxis2 = Vec3(1f, 0f, 0f);
		settings.LimitsMin = -MathF.PI;
		settings.LimitsMax = MathF.PI;

		IntPtr constraint = JoltPhysics.HingeConstraint_Create(env.PhysicsSystem, bodyA, bodyB, &settings);
		constraint.Should().NotBe(IntPtr.Zero);

		IntPtr* ptr = stackalloc IntPtr[1] { constraint };
		JoltPhysics.PhysicsSystem_AddConstraints(env.PhysicsSystem, ptr, 1);

		JoltPhysics.PhysicsSystem_GetNumConstraints(env.PhysicsSystem).Should().Be(1u);

		JoltPhysics.PhysicsSystem_RemoveConstraints(env.PhysicsSystem, ptr, 1);
		JoltPhysics.Constraint_Destroy(constraint);
	}

	[Fact]
	public void SliderConstraint_create_and_add()
	{
		using var env = new JoltTestEnvironment();
		uint bodyA = env.CreateDynamicBox(Vec3(0.5f, 0.5f, 0.5f), RVec3(0f, 3f, 0f));
		uint bodyB = env.CreateDynamicBox(Vec3(0.5f, 0.5f, 0.5f), RVec3(1f, 3f, 0f));

		SliderConstraintSettings settings = default;
		JoltPhysics.SliderConstraintSettings_Init(&settings);
		settings.Space = ConstraintSpace.WorldSpace;
		settings.AutoDetectPoint = 1;
		settings.SliderAxis1 = Vec3(1f, 0f, 0f);
		settings.NormalAxis1 = Vec3(0f, 1f, 0f);
		settings.SliderAxis2 = Vec3(1f, 0f, 0f);
		settings.NormalAxis2 = Vec3(0f, 1f, 0f);
		settings.LimitsMin = -5f;
		settings.LimitsMax = 5f;

		IntPtr constraint = JoltPhysics.SliderConstraint_Create(env.PhysicsSystem, bodyA, bodyB, &settings);
		constraint.Should().NotBe(IntPtr.Zero);

		IntPtr* ptr = stackalloc IntPtr[1] { constraint };
		JoltPhysics.PhysicsSystem_AddConstraints(env.PhysicsSystem, ptr, 1);

		JoltPhysics.PhysicsSystem_GetNumConstraints(env.PhysicsSystem).Should().Be(1u);

		JoltPhysics.PhysicsSystem_RemoveConstraints(env.PhysicsSystem, ptr, 1);
		JoltPhysics.Constraint_Destroy(constraint);
	}

	[Fact]
	public void ConeConstraint_create_and_add()
	{
		using var env = new JoltTestEnvironment();
		uint bodyA = env.CreateDynamicBox(Vec3(0.5f, 0.5f, 0.5f), RVec3(0f, 3f, 0f));
		uint bodyB = env.CreateDynamicBox(Vec3(0.5f, 0.5f, 0.5f), RVec3(1f, 3f, 0f));

		ConeConstraintSettings settings = default;
		JoltPhysics.ConeConstraintSettings_Init(&settings);
		settings.Space = ConstraintSpace.WorldSpace;
		settings.Point1 = RVec3(0.5f, 3f, 0f);
		settings.TwistAxis1 = Vec3(1f, 0f, 0f);
		settings.Point2 = RVec3(0.5f, 3f, 0f);
		settings.TwistAxis2 = Vec3(1f, 0f, 0f);
		settings.HalfConeAngle = MathF.PI / 4f;

		IntPtr constraint = JoltPhysics.ConeConstraint_Create(env.PhysicsSystem, bodyA, bodyB, &settings);
		constraint.Should().NotBe(IntPtr.Zero);

		IntPtr* ptr = stackalloc IntPtr[1] { constraint };
		JoltPhysics.PhysicsSystem_AddConstraints(env.PhysicsSystem, ptr, 1);

		JoltPhysics.PhysicsSystem_GetNumConstraints(env.PhysicsSystem).Should().Be(1u);

		JoltPhysics.PhysicsSystem_RemoveConstraints(env.PhysicsSystem, ptr, 1);
		JoltPhysics.Constraint_Destroy(constraint);
	}

	[Fact]
	public void FixedConstraint_holds_bodies_together_during_simulation()
	{
		using var env = new JoltTestEnvironment();
		uint bodyA = env.CreateDynamicBox(Vec3(0.5f, 0.5f, 0.5f), RVec3(0f, 5f, 0f));
		uint bodyB = env.CreateDynamicBox(Vec3(0.5f, 0.5f, 0.5f), RVec3(0.5f, 5f, 0f));

		IntPtr constraint = env.CreateFixedConstraint(bodyA, bodyB);
		constraint.Should().NotBe(IntPtr.Zero);

		JoltPhysics.PhysicsSystem_OptimizeBroadPhase(env.PhysicsSystem);

		// Step a few times
		for (int i = 0; i < 30; i++)
			env.Step();

		RVec3 posA = JoltPhysics.BodyInterface_GetCenterOfMassPosition(env.BodyInterface, bodyA);
		RVec3 posB = JoltPhysics.BodyInterface_GetCenterOfMassPosition(env.BodyInterface, bodyB);

		// Bodies should have fallen (gravity) but stayed close together
		float dx = posA.X - posB.X;
		float dy = posA.Y - posB.Y;
		float dz = posA.Z - posB.Z;
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

		JoltPhysics.PhysicsSystem_GetNumConstraints(env.PhysicsSystem).Should().Be(2u);
	}
}
