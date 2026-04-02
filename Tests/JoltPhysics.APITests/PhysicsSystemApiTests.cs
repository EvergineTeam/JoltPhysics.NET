using System;
using Evergine.Bindings.JoltPhysics;
using FluentAssertions;
using Xunit;
using static JoltPhysicsTests.NativeTestHelpers;

namespace JoltPhysicsTests;

[Collection("Jolt")]
public unsafe class PhysicsSystemApiTests
{
	[Fact]
	public void Gravity_set_and_get_roundtrip()
	{
		using var env = new JoltTestEnvironment();

		JoltPhysics.PhysicsSystem_SetGravity(env.PhysicsSystem, Vec3(0f, -20f, 0f));
		Vec3 gravity = JoltPhysics.PhysicsSystem_GetGravity(env.PhysicsSystem);
		gravity.X.Should().BeApproximately(0f, 1e-5f);
		gravity.Y.Should().BeApproximately(-20f, 1e-5f);
		gravity.Z.Should().BeApproximately(0f, 1e-5f);
	}

	[Fact]
	public void NumActiveBodies_tracks_dynamic_bodies()
	{
		using var env = new JoltTestEnvironment();

		uint before = JoltPhysics.PhysicsSystem_GetNumActiveBodies(env.PhysicsSystem, BodyType.Rigid);
		before.Should().Be(0u);

		env.CreateDynamicBox(Vec3(0.5f, 0.5f, 0.5f), RVec3(0f, 5f, 0f));

		uint after = JoltPhysics.PhysicsSystem_GetNumActiveBodies(env.PhysicsSystem, BodyType.Rigid);
		after.Should().Be(1u);
	}

	[Fact]
	public void MaxBodies_returns_configured_value()
	{
		using var env = new JoltTestEnvironment();
		uint maxBodies = JoltPhysics.PhysicsSystem_GetMaxBodies(env.PhysicsSystem);
		maxBodies.Should().Be(1024u, "JoltTestEnvironment creates system with 1024 max bodies");
	}

	[Fact]
	public void NarrowPhaseQuery_CastRay_hits_body()
	{
		using var env = new JoltTestEnvironment();

		// Put a big box at origin
		env.CreateStaticBox(Vec3(10f, 10f, 10f), RVec3(0f, 0f, 0f));
		JoltPhysics.PhysicsSystem_OptimizeBroadPhase(env.PhysicsSystem);

		IntPtr query = JoltPhysics.PhysicsSystem_GetNarrowPhaseQuery(env.PhysicsSystem);
		query.Should().NotBe(IntPtr.Zero);

		RayCastResult result = default;
		// Cast ray from far away toward the box
		int hit = JoltPhysics.NarrowPhaseQuery_CastRay(
			query,
			RVec3(-50f, 0f, 0f),   // origin: far left
			Vec3(100f, 0f, 0f),     // direction (unnormalized, represents max distance)
			&result);

		hit.Should().Be(1, "ray should hit the box");
		result.Fraction.Should().BeGreaterThan(0f);
		result.Fraction.Should().BeLessThan(1f);
	}

	[Fact]
	public void NarrowPhaseQuery_CastRay_misses_empty_world()
	{
		using var env = new JoltTestEnvironment();
		JoltPhysics.PhysicsSystem_OptimizeBroadPhase(env.PhysicsSystem);

		IntPtr query = JoltPhysics.PhysicsSystem_GetNarrowPhaseQuery(env.PhysicsSystem);

		RayCastResult result = default;
		int hit = JoltPhysics.NarrowPhaseQuery_CastRay(
			query,
			RVec3(0f, 0f, 0f),
			Vec3(1f, 0f, 0f),
			&result);

		hit.Should().Be(0, "no bodies in the world to hit");
	}

	[Fact]
	public void Step_multiple_times_without_crash()
	{
		using var env = new JoltTestEnvironment();
		env.CreateStaticBox(Vec3(10f, 1f, 10f), RVec3(0f, -1f, 0f));
		env.CreateDynamicBox(Vec3(0.5f, 0.5f, 0.5f), RVec3(0f, 10f, 0f));
		env.CreateDynamicBox(Vec3(0.5f, 0.5f, 0.5f), RVec3(1f, 12f, 0f));
		env.CreateDynamicBox(Vec3(0.5f, 0.5f, 0.5f), RVec3(-1f, 14f, 0f));

		JoltPhysics.PhysicsSystem_OptimizeBroadPhase(env.PhysicsSystem);

		for (int i = 0; i < 120; i++)
		{
			uint result = env.Step();
			// Step should succeed — result is an error code, 0 = OK
		}

		// All bodies should still exist
		JoltPhysics.PhysicsSystem_GetNumBodies(env.PhysicsSystem).Should().Be(4u);
	}

	[Fact]
	public void Zero_gravity_body_does_not_fall()
	{
		using var env = new JoltTestEnvironment();
		JoltPhysics.PhysicsSystem_SetGravity(env.PhysicsSystem, Vec3(0f, 0f, 0f));

		uint body = env.CreateDynamicBox(Vec3(0.5f, 0.5f, 0.5f), RVec3(0f, 5f, 0f));
		JoltPhysics.BodyInterface_SetLinearVelocity(env.BodyInterface, body, Vec3(0f, 0f, 0f));

		JoltPhysics.PhysicsSystem_OptimizeBroadPhase(env.PhysicsSystem);

		for (int i = 0; i < 60; i++)
			env.Step();

		RVec3 pos = JoltPhysics.BodyInterface_GetCenterOfMassPosition(env.BodyInterface, body);
		pos.Y.Should().BeApproximately(5f, 0.01f, "with zero gravity, body should stay at y=5");
	}

	[Fact]
	public void Error_handling_GetLastError_and_ClearLastError()
	{
		// After a successful operation the error should be empty
		JoltPhysics.ClearLastError();
		byte* err = JoltPhysics.GetLastError();
		string errorStr = err == null ? string.Empty : System.Runtime.InteropServices.Marshal.PtrToStringUTF8((IntPtr)err) ?? string.Empty;
		errorStr.Should().BeEmpty();
	}
}
