using System;
using Evergine.Bindings.JoltPhysics;
using FluentAssertions;
using Xunit;
using static JoltPhysics.APITests.NativeTestHelpers;

namespace JoltPhysics.APITests;

[Collection("Jolt")]
public unsafe class BodyInterfaceApiTests
{
	[Fact]
	public void SetLinearVelocity_and_read_back()
	{
		using var env = new JoltTestEnvironment();
		uint body = env.CreateDynamicBox(Vec3(0.5f, 0.5f, 0.5f), RVec3(0f, 5f, 0f));

		JoltPhysicsNative.JoltC_BodyInterface_SetLinearVelocity(env.BodyInterface, body, Vec3(1f, 2f, 3f));
		JoltC_Vec3 vel = JoltPhysicsNative.JoltC_BodyInterface_GetLinearVelocity(env.BodyInterface, body);
		vel.x.Should().BeApproximately(1f, 1e-5f);
		vel.y.Should().BeApproximately(2f, 1e-5f);
		vel.z.Should().BeApproximately(3f, 1e-5f);
	}

	[Fact]
	public void SetAngularVelocity_and_read_back()
	{
		using var env = new JoltTestEnvironment();
		uint body = env.CreateDynamicBox(Vec3(0.5f, 0.5f, 0.5f), RVec3(0f, 5f, 0f));

		JoltPhysicsNative.JoltC_BodyInterface_SetAngularVelocity(env.BodyInterface, body, Vec3(0f, 3.14f, 0f));
		JoltC_Vec3 angVel = JoltPhysicsNative.JoltC_BodyInterface_GetAngularVelocity(env.BodyInterface, body);
		angVel.y.Should().BeApproximately(3.14f, 1e-3f);
	}

	[Fact]
	public void Friction_and_restitution_roundtrip()
	{
		using var env = new JoltTestEnvironment();
		uint body = env.CreateDynamicBox(Vec3(0.5f, 0.5f, 0.5f), RVec3(0f, 5f, 0f));

		JoltPhysicsNative.JoltC_BodyInterface_SetFriction(env.BodyInterface, body, 0.75f);
		JoltPhysicsNative.JoltC_BodyInterface_GetFriction(env.BodyInterface, body).Should().BeApproximately(0.75f, 1e-5f);

		JoltPhysicsNative.JoltC_BodyInterface_SetRestitution(env.BodyInterface, body, 0.9f);
		JoltPhysicsNative.JoltC_BodyInterface_GetRestitution(env.BodyInterface, body).Should().BeApproximately(0.9f, 1e-5f);
	}

	[Fact]
	public void Activate_and_deactivate_body()
	{
		using var env = new JoltTestEnvironment();
		uint body = env.CreateDynamicBox(Vec3(0.5f, 0.5f, 0.5f), RVec3(0f, 5f, 0f));

		JoltPhysicsNative.JoltC_BodyInterface_IsActive(env.BodyInterface, body).Should().Be(1, "body should start active");

		JoltPhysicsNative.JoltC_BodyInterface_DeactivateBody(env.BodyInterface, body);
		JoltPhysicsNative.JoltC_BodyInterface_IsActive(env.BodyInterface, body).Should().Be(0, "body should be deactivated");

		JoltPhysicsNative.JoltC_BodyInterface_ActivateBody(env.BodyInterface, body);
		JoltPhysicsNative.JoltC_BodyInterface_IsActive(env.BodyInterface, body).Should().Be(1, "body should be reactivated");
	}

	[Fact]
	public void SetPosition_moves_body()
	{
		using var env = new JoltTestEnvironment();
		uint body = env.CreateDynamicBox(Vec3(0.5f, 0.5f, 0.5f), RVec3(0f, 5f, 0f));

		JoltPhysicsNative.JoltC_BodyInterface_SetPosition(env.BodyInterface, body, RVec3(10f, 20f, 30f), JoltC_Activation.JOLTC_ACTIVATION_ACTIVATE);
		JoltC_RVec3 pos = JoltPhysicsNative.JoltC_BodyInterface_GetPosition(env.BodyInterface, body);
		pos.x.Should().BeApproximately(10f, 1e-3f);
		pos.y.Should().BeApproximately(20f, 1e-3f);
		pos.z.Should().BeApproximately(30f, 1e-3f);
	}

	[Fact]
	public void GetPositionAndRotation_returns_consistent_values()
	{
		using var env = new JoltTestEnvironment();
		uint body = env.CreateDynamicBox(Vec3(0.5f, 0.5f, 0.5f), RVec3(1f, 2f, 3f));

		JoltC_RVec3 pos;
		JoltC_Quat rot;
		JoltPhysicsNative.JoltC_BodyInterface_GetPositionAndRotation(env.BodyInterface, body, &pos, &rot);
		pos.x.Should().BeApproximately(1f, 1e-3f);
		pos.y.Should().BeApproximately(2f, 1e-3f);
		pos.z.Should().BeApproximately(3f, 1e-3f);
		rot.w.Should().BeApproximately(1f, 1e-3f, "should be identity rotation");
	}

	[Fact]
	public void SetPositionAndRotation_roundtrip()
	{
		using var env = new JoltTestEnvironment();
		uint body = env.CreateDynamicBox(Vec3(0.5f, 0.5f, 0.5f), RVec3(0f, 5f, 0f));

		JoltC_Quat quat90y = new() { x = 0f, y = 0.7071068f, z = 0f, w = 0.7071068f };
		JoltPhysicsNative.JoltC_BodyInterface_SetPositionAndRotation(env.BodyInterface, body, RVec3(5f, 10f, 15f), quat90y, JoltC_Activation.JOLTC_ACTIVATION_ACTIVATE);

		JoltC_RVec3 pos;
		JoltC_Quat rot;
		JoltPhysicsNative.JoltC_BodyInterface_GetPositionAndRotation(env.BodyInterface, body, &pos, &rot);
		pos.x.Should().BeApproximately(5f, 1e-3f);
		pos.y.Should().BeApproximately(10f, 1e-3f);
		rot.y.Should().BeApproximately(0.7071068f, 1e-3f);
	}

	[Fact]
	public void GetMotionType_returns_correct_type()
	{
		using var env = new JoltTestEnvironment();
		uint staticBody = env.CreateStaticBox(Vec3(10f, 1f, 10f), RVec3(0f, 0f, 0f));
		uint dynamicBody = env.CreateDynamicBox(Vec3(0.5f, 0.5f, 0.5f), RVec3(0f, 5f, 0f));

		JoltPhysicsNative.JoltC_BodyInterface_GetMotionType(env.BodyInterface, staticBody)
			.Should().Be(JoltC_MotionType.JOLTC_MOTION_TYPE_STATIC);
		JoltPhysicsNative.JoltC_BodyInterface_GetMotionType(env.BodyInterface, dynamicBody)
			.Should().Be(JoltC_MotionType.JOLTC_MOTION_TYPE_DYNAMIC);
	}

	[Fact]
	public void AddImpulse_changes_velocity()
	{
		using var env = new JoltTestEnvironment();
		uint body = env.CreateDynamicBox(Vec3(0.5f, 0.5f, 0.5f), RVec3(0f, 5f, 0f));

		// Zero out velocity first
		JoltPhysicsNative.JoltC_BodyInterface_SetLinearVelocity(env.BodyInterface, body, Vec3(0f, 0f, 0f));
		JoltPhysicsNative.JoltC_BodyInterface_AddImpulse(env.BodyInterface, body, Vec3(100f, 0f, 0f));

		JoltC_Vec3 vel = JoltPhysicsNative.JoltC_BodyInterface_GetLinearVelocity(env.BodyInterface, body);
		vel.x.Should().BeGreaterThan(0f, "impulse should have added velocity in x direction");
	}

	[Fact]
	public void AddForce_followed_by_step_changes_velocity()
	{
		using var env = new JoltTestEnvironment();
		uint body = env.CreateDynamicBox(Vec3(0.5f, 0.5f, 0.5f), RVec3(0f, 5f, 0f));

		JoltPhysicsNative.JoltC_BodyInterface_SetLinearVelocity(env.BodyInterface, body, Vec3(0f, 0f, 0f));
		JoltPhysicsNative.JoltC_BodyInterface_AddForce(env.BodyInterface, body, Vec3(1000f, 0f, 0f));

		JoltPhysicsNative.JoltC_PhysicsSystem_OptimizeBroadPhase(env.PhysicsSystem);
		env.Step();

		JoltC_Vec3 vel = JoltPhysicsNative.JoltC_BodyInterface_GetLinearVelocity(env.BodyInterface, body);
		vel.x.Should().BeGreaterThan(0f, "force should produce velocity after stepping");
	}

	[Fact]
	public void MoveKinematic_moves_body_to_target()
	{
		using var env = new JoltTestEnvironment();

		// Create a kinematic body
		IntPtr shape = JoltPhysicsNative.JoltC_BoxShape_Create(Vec3(0.5f, 0.5f, 0.5f), 0.05f);
		JoltC_BodyCreationSettings settings = default;
		JoltPhysicsNative.JoltC_BodyCreationSettings_SetDefault(&settings);
		settings.shape = shape;
		settings.motionType = JoltC_MotionType.JOLTC_MOTION_TYPE_KINEMATIC;
		settings.position = RVec3(0f, 5f, 0f);
		settings.objectLayer = JoltTestEnvironment.LayerMoving;
		settings.allowDynamicOrKinematic = 1;

		uint body = JoltPhysicsNative.JoltC_BodyInterface_CreateBody(env.BodyInterface, &settings);
		JoltPhysicsNative.JoltC_BodyInterface_AddBody(env.BodyInterface, body, JoltC_Activation.JOLTC_ACTIVATION_ACTIVATE);

		JoltPhysicsNative.JoltC_BodyInterface_MoveKinematic(env.BodyInterface, body, RVec3(10f, 5f, 0f), IdentityQuat(), 1f / 60f);

		JoltPhysicsNative.JoltC_PhysicsSystem_OptimizeBroadPhase(env.PhysicsSystem);
		env.Step();

		JoltC_RVec3 pos = JoltPhysicsNative.JoltC_BodyInterface_GetCenterOfMassPosition(env.BodyInterface, body);
		pos.x.Should().BeApproximately(10f, 0.5f, "kinematic body should have moved toward target");

		// Cleanup
		JoltPhysicsNative.JoltC_BodyInterface_RemoveBody(env.BodyInterface, body);
		JoltPhysicsNative.JoltC_BodyInterface_DestroyBody(env.BodyInterface, body);
		JoltPhysicsNative.JoltC_Shape_Destroy(shape);
	}
}
