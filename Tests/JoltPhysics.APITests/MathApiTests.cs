using System;
using Evergine.Bindings.JoltPhysics;
using FluentAssertions;
using Xunit;
using static JoltPhysics.APITests.NativeTestHelpers;

namespace JoltPhysics.APITests;

[Collection("Jolt")]
public unsafe class MathApiTests
{
	[Fact]
	public void Vec3_AxisX_returns_unit_x()
	{
		JoltC_Vec3 result;
		JoltPhysicsNative.JoltC_Vec3_AxisX(&result);
		result.x.Should().Be(1f);
		result.y.Should().Be(0f);
		result.z.Should().Be(0f);
	}

	[Fact]
	public void Vec3_AxisY_returns_unit_y()
	{
		JoltC_Vec3 result;
		JoltPhysicsNative.JoltC_Vec3_AxisY(&result);
		result.x.Should().Be(0f);
		result.y.Should().Be(1f);
		result.z.Should().Be(0f);
	}

	[Fact]
	public void Vec3_AxisZ_returns_unit_z()
	{
		JoltC_Vec3 result;
		JoltPhysicsNative.JoltC_Vec3_AxisZ(&result);
		result.x.Should().Be(0f);
		result.y.Should().Be(0f);
		result.z.Should().Be(1f);
	}

	[Fact]
	public void Vec3_Length_returns_correct_magnitude()
	{
		JoltC_Vec3 v = Vec3(3f, 4f, 0f);
		float len = JoltPhysicsNative.JoltC_Vec3_Length(&v);
		len.Should().BeApproximately(5f, 1e-5f);
	}

	[Fact]
	public void Vec3_LengthSquared_returns_correct_value()
	{
		JoltC_Vec3 v = Vec3(3f, 4f, 0f);
		float lenSq = JoltPhysicsNative.JoltC_Vec3_LengthSquared(&v);
		lenSq.Should().BeApproximately(25f, 1e-5f);
	}

	[Fact]
	public void Vec3_Normalize_produces_unit_vector()
	{
		JoltC_Vec3 v = Vec3(0f, 0f, 5f);
		JoltC_Vec3 result;
		JoltPhysicsNative.JoltC_Vec3_Normalize(&v, &result);
		result.x.Should().BeApproximately(0f, 1e-5f);
		result.y.Should().BeApproximately(0f, 1e-5f);
		result.z.Should().BeApproximately(1f, 1e-5f);
	}

	[Fact]
	public void Vec3_Cross_product_is_correct()
	{
		JoltC_Vec3 a = Vec3(1f, 0f, 0f);
		JoltC_Vec3 b = Vec3(0f, 1f, 0f);
		JoltC_Vec3 result;
		JoltPhysicsNative.JoltC_Vec3_Cross(&a, &b, &result);
		result.x.Should().BeApproximately(0f, 1e-5f);
		result.y.Should().BeApproximately(0f, 1e-5f);
		result.z.Should().BeApproximately(1f, 1e-5f);
	}

	[Fact]
	public void Vec3_DotProduct_is_correct()
	{
		JoltC_Vec3 a = Vec3(1f, 2f, 3f);
		JoltC_Vec3 b = Vec3(4f, 5f, 6f);
		float result;
		JoltPhysicsNative.JoltC_Vec3_DotProduct(&a, &b, &result);
		result.Should().BeApproximately(32f, 1e-5f); // 1*4 + 2*5 + 3*6
	}

	[Fact]
	public void Vec3_IsClose_detects_close_vectors()
	{
		JoltC_Vec3 a = Vec3(1f, 2f, 3f);
		JoltC_Vec3 b = Vec3(1.001f, 2.001f, 3.001f);
		int close = JoltPhysicsNative.JoltC_Vec3_IsClose(&a, &b, 0.01f);
		close.Should().Be(1);
	}

	[Fact]
	public void Vec3_IsNearZero_detects_near_zero()
	{
		JoltC_Vec3 v = Vec3(0.0001f, 0.0001f, 0.0001f);
		int nearZero = JoltPhysicsNative.JoltC_Vec3_IsNearZero(&v, 0.001f);
		nearZero.Should().Be(1);
	}

	[Fact]
	public void Vec3_IsNormalized_true_for_unit_vector()
	{
		JoltC_Vec3 v = Vec3(1f, 0f, 0f);
		int normalized = JoltPhysicsNative.JoltC_Vec3_IsNormalized(&v, 1e-5f);
		normalized.Should().Be(1);
	}

	[Fact]
	public void Vec3_IsNaN_false_for_normal_vector()
	{
		JoltC_Vec3 v = Vec3(1f, 2f, 3f);
		int isNan = JoltPhysicsNative.JoltC_Vec3_IsNaN(&v);
		isNan.Should().Be(0);
	}

	[Fact]
	public void Quat_Multiply_combines_rotations()
	{
		JoltC_Quat identity = IdentityQuat();
		JoltC_Quat q = new() { x = 0f, y = 0.7071068f, z = 0f, w = 0.7071068f }; // ~90° around Y
		JoltC_Quat result;
		JoltPhysicsNative.JoltC_Quat_Multiply(&identity, &q, &result);

		// identity * q == q
		result.x.Should().BeApproximately(q.x, 1e-5f);
		result.y.Should().BeApproximately(q.y, 1e-5f);
		result.z.Should().BeApproximately(q.z, 1e-5f);
		result.w.Should().BeApproximately(q.w, 1e-5f);
	}

	[Fact]
	public void Quat_Slerp_halfway_between_identity_and_90y()
	{
		JoltC_Quat from = IdentityQuat();
		JoltC_Quat to = new() { x = 0f, y = 0.7071068f, z = 0f, w = 0.7071068f };
		JoltC_Quat result;
		JoltPhysicsNative.JoltC_Quat_Slerp(&from, &to, 0.5f, &result);

		// Should be ~45° around Y
		float len = MathF.Sqrt(result.x * result.x + result.y * result.y + result.z * result.z + result.w * result.w);
		len.Should().BeApproximately(1f, 1e-4f, "slerp result should be unit quaternion");
		result.y.Should().BeGreaterThan(0f);
		result.w.Should().BeGreaterThan(result.y, "half-slerp w > y for 90° rotation");
	}

	[Fact]
	public void Mat44_Identity_has_ones_on_diagonal()
	{
		JoltC_Mat44 identity;
		JoltPhysicsNative.JoltC_Mat44_Identity(&identity);
		identity.m[0].Should().Be(1f);
		identity.m[5].Should().Be(1f);
		identity.m[10].Should().Be(1f);
		identity.m[15].Should().Be(1f);
		identity.m[1].Should().Be(0f);
		identity.m[4].Should().Be(0f);
	}

	[Fact]
	public void Mat44_Multiply_identity_times_identity_is_identity()
	{
		JoltC_Mat44 a, b, result;
		JoltPhysicsNative.JoltC_Mat44_Identity(&a);
		JoltPhysicsNative.JoltC_Mat44_Identity(&b);
		JoltPhysicsNative.JoltC_Mat44_Multiply(&a, &b, &result);

		result.m[0].Should().BeApproximately(1f, 1e-5f);
		result.m[5].Should().BeApproximately(1f, 1e-5f);
		result.m[10].Should().BeApproximately(1f, 1e-5f);
		result.m[15].Should().BeApproximately(1f, 1e-5f);
		result.m[1].Should().BeApproximately(0f, 1e-5f);
	}

	[Fact]
	public void Math_Sin_and_Cos_match_system_math()
	{
		float angle = MathF.PI / 4f; // 45 degrees
		JoltPhysicsNative.JoltC_Math_Sin(angle).Should().BeApproximately(MathF.Sin(angle), 1e-5f);
		JoltPhysicsNative.JoltC_Math_Cos(angle).Should().BeApproximately(MathF.Cos(angle), 1e-5f);
	}

	[Fact]
	public void RayCast_GetPointOnRay_computes_correct_point()
	{
		JoltC_Vec3 origin = Vec3(0f, 0f, 0f);
		JoltC_Vec3 direction = Vec3(1f, 0f, 0f);
		JoltC_Vec3 result;
		JoltPhysicsNative.JoltC_RayCast_GetPointOnRay(&origin, &direction, 0.5f, &result);
		result.x.Should().BeApproximately(0.5f, 1e-5f);
		result.y.Should().BeApproximately(0f, 1e-5f);
		result.z.Should().BeApproximately(0f, 1e-5f);
	}
}
