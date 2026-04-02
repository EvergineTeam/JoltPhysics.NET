using System;
using Evergine.Bindings.JoltPhysics;
using FluentAssertions;
using Xunit;
using static JoltPhysicsTests.NativeTestHelpers;

namespace JoltPhysicsTests;

[Collection("Jolt")]
public unsafe class BodyInterfaceApiTests
{
	[Fact]
	public void SetLinearVelocity_and_read_back()
	{
		using var env = new JoltTestEnvironment();
		uint body = env.CreateDynamicBox(Vec3(0.5f, 0.5f, 0.5f), RVec3(0f, 5f, 0f));

		JoltPhysics.BodyInterface_SetLinearVelocity(env.BodyInterface, body, Vec3(1f, 2f, 3f));
		Vec3 vel = JoltPhysics.BodyInterface_GetLinearVelocity(env.BodyInterface, body);
		vel.X.Should().BeApproximately(1f, 1e-5f);
		vel.Y.Should().BeApproximately(2f, 1e-5f);
		vel.Z.Should().BeApproximately(3f, 1e-5f);
	}

	[Fact]
	public void SetAngularVelocity_and_read_back()
	{
		using var env = new JoltTestEnvironment();
		uint body = env.CreateDynamicBox(Vec3(0.5f, 0.5f, 0.5f), RVec3(0f, 5f, 0f));

		JoltPhysics.BodyInterface_SetAngularVelocity(env.BodyInterface, body, Vec3(0f, 3.14f, 0f));
		Vec3 angVel = JoltPhysics.BodyInterface_GetAngularVelocity(env.BodyInterface, body);
		angVel.Y.Should().BeApproximately(3.14f, 1e-3f);
	}

	[Fact]
	public void Friction_and_restitution_roundtrip()
	{
		using var env = new JoltTestEnvironment();
		uint body = env.CreateDynamicBox(Vec3(0.5f, 0.5f, 0.5f), RVec3(0f, 5f, 0f));

		JoltPhysics.BodyInterface_SetFriction(env.BodyInterface, body, 0.75f);
		JoltPhysics.BodyInterface_GetFriction(env.BodyInterface, body).Should().BeApproximately(0.75f, 1e-5f);

		JoltPhysics.BodyInterface_SetRestitution(env.BodyInterface, body, 0.9f);
		JoltPhysics.BodyInterface_GetRestitution(env.BodyInterface, body).Should().BeApproximately(0.9f, 1e-5f);
	}

	[Fact]
	public void Activate_and_deactivate_body()
	{
		using var env = new JoltTestEnvironment();
		uint body = env.CreateDynamicBox(Vec3(0.5f, 0.5f, 0.5f), RVec3(0f, 5f, 0f));

		JoltPhysics.BodyInterface_IsActive(env.BodyInterface, body).Should().BeTrue("body should start active");

		JoltPhysics.BodyInterface_DeactivateBody(env.BodyInterface, body);
		JoltPhysics.BodyInterface_IsActive(env.BodyInterface, body).Should().BeFalse("body should be deactivated");

		JoltPhysics.BodyInterface_ActivateBody(env.BodyInterface, body);
		JoltPhysics.BodyInterface_IsActive(env.BodyInterface, body).Should().BeTrue("body should be reactivated");
	}

	[Fact]
	public void SetPosition_moves_body()
	{
		using var env = new JoltTestEnvironment();
		uint body = env.CreateDynamicBox(Vec3(0.5f, 0.5f, 0.5f), RVec3(0f, 5f, 0f));

		JoltPhysics.BodyInterface_SetPosition(env.BodyInterface, body, RVec3(10f, 20f, 30f), Activation.Activate);
		RVec3 pos = JoltPhysics.BodyInterface_GetPosition(env.BodyInterface, body);
		pos.X.Should().BeApproximately(10f, 1e-3f);
		pos.Y.Should().BeApproximately(20f, 1e-3f);
		pos.Z.Should().BeApproximately(30f, 1e-3f);
	}

	[Fact]
	public void GetPositionAndRotation_returns_consistent_values()
	{
		using var env = new JoltTestEnvironment();
		uint body = env.CreateDynamicBox(Vec3(0.5f, 0.5f, 0.5f), RVec3(1f, 2f, 3f));

		RVec3 pos;
		Quat rot;
		JoltPhysics.BodyInterface_GetPositionAndRotation(env.BodyInterface, body, &pos, &rot);
		pos.X.Should().BeApproximately(1f, 1e-3f);
		pos.Y.Should().BeApproximately(2f, 1e-3f);
		pos.Z.Should().BeApproximately(3f, 1e-3f);
		rot.W.Should().BeApproximately(1f, 1e-3f, "should be identity rotation");
	}

	[Fact]
	public void SetPositionAndRotation_roundtrip()
	{
		using var env = new JoltTestEnvironment();
		uint body = env.CreateDynamicBox(Vec3(0.5f, 0.5f, 0.5f), RVec3(0f, 5f, 0f));

		Quat quat90y = new() { X = 0f, Y = 0.7071068f, Z = 0f, W = 0.7071068f };
		JoltPhysics.BodyInterface_SetPositionAndRotation(env.BodyInterface, body, RVec3(5f, 10f, 15f), quat90y, Activation.Activate);

		RVec3 pos;
		Quat rot;
		JoltPhysics.BodyInterface_GetPositionAndRotation(env.BodyInterface, body, &pos, &rot);
		pos.X.Should().BeApproximately(5f, 1e-3f);
		pos.Y.Should().BeApproximately(10f, 1e-3f);
		rot.Y.Should().BeApproximately(0.7071068f, 1e-3f);
	}

	[Fact]
	public void GetMotionType_returns_correct_type()
	{
		using var env = new JoltTestEnvironment();
		uint staticBody = env.CreateStaticBox(Vec3(10f, 1f, 10f), RVec3(0f, 0f, 0f));
		uint dynamicBody = env.CreateDynamicBox(Vec3(0.5f, 0.5f, 0.5f), RVec3(0f, 5f, 0f));

		JoltPhysics.BodyInterface_GetMotionType(env.BodyInterface, staticBody)
			.Should().Be(MotionType.Static);
		JoltPhysics.BodyInterface_GetMotionType(env.BodyInterface, dynamicBody)
			.Should().Be(MotionType.Dynamic);
	}

	[Fact]
	public void AddImpulse_changes_velocity()
	{
		using var env = new JoltTestEnvironment();
		uint body = env.CreateDynamicBox(Vec3(0.5f, 0.5f, 0.5f), RVec3(0f, 5f, 0f));

		// Zero out velocity first
		JoltPhysics.BodyInterface_SetLinearVelocity(env.BodyInterface, body, Vec3(0f, 0f, 0f));
		JoltPhysics.BodyInterface_AddImpulse(env.BodyInterface, body, Vec3(100f, 0f, 0f));

		Vec3 vel = JoltPhysics.BodyInterface_GetLinearVelocity(env.BodyInterface, body);
		vel.X.Should().BeGreaterThan(0f, "impulse should have added velocity in x direction");
	}

	[Fact]
	public void AddForce_followed_by_step_changes_velocity()
	{
		using var env = new JoltTestEnvironment();
		uint body = env.CreateDynamicBox(Vec3(0.5f, 0.5f, 0.5f), RVec3(0f, 5f, 0f));

		JoltPhysics.BodyInterface_SetLinearVelocity(env.BodyInterface, body, Vec3(0f, 0f, 0f));
		JoltPhysics.BodyInterface_AddForce(env.BodyInterface, body, Vec3(1000f, 0f, 0f));

		JoltPhysics.PhysicsSystem_OptimizeBroadPhase(env.PhysicsSystem);
		env.Step();

		Vec3 vel = JoltPhysics.BodyInterface_GetLinearVelocity(env.BodyInterface, body);
		vel.X.Should().BeGreaterThan(0f, "force should produce velocity after stepping");
	}

	[Fact]
	public void MoveKinematic_moves_body_to_target()
	{
		using var env = new JoltTestEnvironment();

		// Create a kinematic body
		IntPtr shape = JoltPhysics.BoxShape_Create(Vec3(0.5f, 0.5f, 0.5f), 0.05f);
		BodyCreationSettings settings = default;
		JoltPhysics.BodyCreationSettings_SetDefault(&settings);
		settings .Shape = shape;
		settings.MotionType = MotionType.Kinematic;
		settings.Position = RVec3(0f, 5f, 0f);
		settings.ObjectLayer = JoltTestEnvironment.LayerMoving;
		settings.AllowDynamicOrKinematic = 1;

		uint body = JoltPhysics.BodyInterface_CreateBody(env.BodyInterface, &settings);
		JoltPhysics.BodyInterface_AddBody(env.BodyInterface, body, Activation.Activate);

		JoltPhysics.BodyInterface_MoveKinematic(env.BodyInterface, body, RVec3(10f, 5f, 0f), IdentityQuat(), 1f / 60f);

		JoltPhysics.PhysicsSystem_OptimizeBroadPhase(env.PhysicsSystem);
		env.Step();

		RVec3 pos = JoltPhysics.BodyInterface_GetCenterOfMassPosition(env.BodyInterface, body);
		pos.X.Should().BeApproximately(10f, 0.5f, "kinematic body should have moved toward target");

		// Cleanup
		JoltPhysics.BodyInterface_RemoveBody(env.BodyInterface, body);
		JoltPhysics.BodyInterface_DestroyBody(env.BodyInterface, body);
		JoltPhysics.Shape_Destroy(shape);
	}
}
