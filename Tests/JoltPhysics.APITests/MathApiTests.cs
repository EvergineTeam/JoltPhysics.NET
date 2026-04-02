using System;
using Evergine.Bindings.JoltPhysics;
using FluentAssertions;
using Xunit;
using static JoltPhysicsTests.NativeTestHelpers;

namespace JoltPhysicsTests;

[Collection("Jolt")]
public unsafe class MathApiTests
{
	[Fact]
	public void Vec3_AxisX_returns_unit_x()
	{
		Vec3 result;
		JoltPhysics.Vec3_AxisX(&result);
		result.X.Should().Be(1f);
		result.Y.Should().Be(0f);
		result.Z.Should().Be(0f);
	}

	[Fact]
	public void Vec3_AxisY_returns_unit_y()
	{
		Vec3 result;
		JoltPhysics.Vec3_AxisY(&result);
		result.X.Should().Be(0f);
		result.Y.Should().Be(1f);
		result.Z.Should().Be(0f);
	}

	[Fact]
	public void Vec3_AxisZ_returns_unit_z()
	{
		Vec3 result;
		JoltPhysics.Vec3_AxisZ(&result);
		result.X.Should().Be(0f);
		result.Y.Should().Be(0f);
		result.Z.Should().Be(1f);
	}

	[Fact]
	public void Vec3_Length_returns_correct_magnitude()
	{
		Vec3 v = Vec3(3f, 4f, 0f);
		float len = JoltPhysics.Vec3_Length(&v);
		len.Should().BeApproximately(5f, 1e-5f);
	}

	[Fact]
	public void Vec3_LengthSquared_returns_correct_value()
	{
		Vec3 v = Vec3(3f, 4f, 0f);
		float lenSq = JoltPhysics.Vec3_LengthSquared(&v);
		lenSq.Should().BeApproximately(25f, 1e-5f);
	}

	[Fact]
	public void Vec3_Normalize_produces_unit_vector()
	{
		Vec3 v = Vec3(0f, 0f, 5f);
		Vec3 result;
		JoltPhysics.Vec3_Normalize(&v, &result);
		result.X.Should().BeApproximately(0f, 1e-5f);
		result.Y.Should().BeApproximately(0f, 1e-5f);
		result.Z.Should().BeApproximately(1f, 1e-5f);
	}

	[Fact]
	public void Vec3_Cross_product_is_correct()
	{
		Vec3 a = Vec3(1f, 0f, 0f);
		Vec3 b = Vec3(0f, 1f, 0f);
		Vec3 result;
		JoltPhysics.Vec3_Cross(&a, &b, &result);
		result.X.Should().BeApproximately(0f, 1e-5f);
		result.Y.Should().BeApproximately(0f, 1e-5f);
		result.Z.Should().BeApproximately(1f, 1e-5f);
	}

	[Fact]
	public void Vec3_DotProduct_is_correct()
	{
		Vec3 a = Vec3(1f, 2f, 3f);
		Vec3 b = Vec3(4f, 5f, 6f);
		float result;
		JoltPhysics.Vec3_DotProduct(&a, &b, &result);
		result.Should().BeApproximately(32f, 1e-5f); // 1*4 + 2*5 + 3*6
	}

	[Fact]
	public void Vec3_IsClose_detects_close_vectors()
	{
		Vec3 a = Vec3(1f, 2f, 3f);
		Vec3 b = Vec3(1.001f, 2.001f, 3.001f);
		int close = JoltPhysics.Vec3_IsClose(&a, &b, 0.01f);
		close.Should().Be(1);
	}

	[Fact]
	public void Vec3_IsNearZero_detects_near_zero()
	{
		Vec3 v = Vec3(0.0001f, 0.0001f, 0.0001f);
		int nearZero = JoltPhysics.Vec3_IsNearZero(&v, 0.001f);
		nearZero.Should().Be(1);
	}

	[Fact]
	public void Vec3_IsNormalized_true_for_unit_vector()
	{
		Vec3 v = Vec3(1f, 0f, 0f);
		int normalized = JoltPhysics.Vec3_IsNormalized(&v, 1e-5f);
		normalized.Should().Be(1);
	}

	[Fact]
	public void Vec3_IsNaN_false_for_normal_vector()
	{
		Vec3 v = Vec3(1f, 2f, 3f);
		int isNan = JoltPhysics.Vec3_IsNaN(&v);
		isNan.Should().Be(0);
	}

	[Fact]
	public void Quat_Multiply_combines_rotations()
	{
		Quat identity = IdentityQuat();
		Quat q = new() { X = 0f, Y = 0.7071068f, Z = 0f, W = 0.7071068f }; // ~90° around Y
		Quat result;
		JoltPhysics.Quat_Multiply(&identity, &q, &result);

		// identity * q == q
		result.X.Should().BeApproximately(q.X, 1e-5f);
		result.Y.Should().BeApproximately(q.Y, 1e-5f);
		result.Z.Should().BeApproximately(q.Z, 1e-5f);
		result.W.Should().BeApproximately(q.W, 1e-5f);
	}

	[Fact]
	public void Quat_Slerp_halfway_between_identity_and_90y()
	{
		Quat from = IdentityQuat();
		Quat to = new() { X = 0f, Y = 0.7071068f, Z = 0f, W = 0.7071068f };
		Quat result;
		JoltPhysics.Quat_Slerp(&from, &to, 0.5f, &result);

		// Should be ~45° around Y
		float len = MathF.Sqrt(result.X * result.X + result.Y * result.Y + result.Z * result.Z + result.W * result.W);
		len.Should().BeApproximately(1f, 1e-4f, "slerp result should be unit quaternion");
		result.Y.Should().BeGreaterThan(0f);
		result.W.Should().BeGreaterThan(result.Y, "half-slerp w > y for 90° rotation");
	}

	[Fact]
	public void Mat44_Identity_has_ones_on_diagonal()
	{
		Mat44 identity;
		JoltPhysics.Mat44_Identity(&identity);
		identity.M[0].Should().Be(1f);
		identity.M[5].Should().Be(1f);
		identity.M[10].Should().Be(1f);
		identity.M[15].Should().Be(1f);
		identity.M[1].Should().Be(0f);
		identity.M[4].Should().Be(0f);
	}

	[Fact]
	public void Mat44_Multiply_identity_times_identity_is_identity()
	{
		Mat44 a, b, result;
		JoltPhysics.Mat44_Identity(&a);
		JoltPhysics.Mat44_Identity(&b);
		JoltPhysics.Mat44_Multiply(&a, &b, &result);

		result.M[0].Should().BeApproximately(1f, 1e-5f);
		result.M[5].Should().BeApproximately(1f, 1e-5f);
		result.M[10].Should().BeApproximately(1f, 1e-5f);
		result.M[15].Should().BeApproximately(1f, 1e-5f);
		result.M[1].Should().BeApproximately(0f, 1e-5f);
	}

	[Fact]
	public void Math_Sin_and_Cos_match_system_math()
	{
		float angle = MathF.PI / 4f; // 45 degrees
		JoltPhysics.Math_Sin(angle).Should().BeApproximately(MathF.Sin(angle), 1e-5f);
		JoltPhysics.Math_Cos(angle).Should().BeApproximately(MathF.Cos(angle), 1e-5f);
	}

	[Fact]
	public void RayCast_GetPointOnRay_computes_correct_point()
	{
		Vec3 origin = Vec3(0f, 0f, 0f);
		Vec3 direction = Vec3(1f, 0f, 0f);
		Vec3 result;
		JoltPhysics.RayCast_GetPointOnRay(&origin, &direction, 0.5f, &result);
		result.X.Should().BeApproximately(0.5f, 1e-5f);
		result.Y.Should().BeApproximately(0f, 1e-5f);
		result.Z.Should().BeApproximately(0f, 1e-5f);
	}
}
