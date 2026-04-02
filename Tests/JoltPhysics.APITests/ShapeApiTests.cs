using System;
using Evergine.Bindings.JoltPhysics;
using FluentAssertions;
using Xunit;
using static JoltPhysicsTests.NativeTestHelpers;

namespace JoltPhysicsTests;

[Collection("Jolt")]
public unsafe class ShapeApiTests
{
	[Fact]
	public void SphereShape_create_and_query_radius()
	{
		IntPtr sphere = JoltPhysics.SphereShape_Create(2.5f);
		try
		{
			sphere.Should().NotBe(IntPtr.Zero);
			JoltPhysics.Shape_GetType(sphere).Should().Be(ShapeType.Convex);
			JoltPhysics.Shape_GetSubType(sphere).Should().Be(ShapeSubType.Sphere);
			JoltPhysics.SphereShape_GetRadius(sphere).Should().BeApproximately(2.5f, 1e-5f);
		}
		finally
		{
			JoltPhysics.Shape_Destroy(sphere);
		}
	}

	[Fact]
	public void CapsuleShape_create_and_query_dimensions()
	{
		IntPtr capsule = JoltPhysics.CapsuleShape_Create(1.0f, 0.5f);
		try
		{
			capsule.Should().NotBe(IntPtr.Zero);
			JoltPhysics.Shape_GetSubType(capsule).Should().Be(ShapeSubType.Capsule);
			JoltPhysics.CapsuleShape_GetHalfHeightOfCylinder(capsule).Should().BeApproximately(1.0f, 1e-5f);
			JoltPhysics.CapsuleShape_GetRadius(capsule).Should().BeApproximately(0.5f, 1e-5f);
		}
		finally
		{
			JoltPhysics.Shape_Destroy(capsule);
		}
	}

	[Fact]
	public void CylinderShape_create_and_query_dimensions()
	{
		IntPtr cylinder = JoltPhysics.CylinderShape_Create(2.0f, 1.0f, 0.05f);
		try
		{
			cylinder.Should().NotBe(IntPtr.Zero);
			JoltPhysics.Shape_GetSubType(cylinder).Should().Be(ShapeSubType.Cylinder);
			JoltPhysics.CylinderShape_GetHalfHeight(cylinder).Should().BeApproximately(2.0f, 1e-5f);
			JoltPhysics.CylinderShape_GetRadius(cylinder).Should().BeApproximately(1.0f, 1e-5f);
		}
		finally
		{
			JoltPhysics.Shape_Destroy(cylinder);
		}
	}

	[Fact]
	public void TaperedCapsuleShape_create_returns_valid_handle()
	{
		IntPtr tapered = JoltPhysics.TaperedCapsuleShape_Create(1.0f, 0.5f, 0.3f);
		try
		{
			tapered.Should().NotBe(IntPtr.Zero);
			JoltPhysics.Shape_GetSubType(tapered).Should().Be(ShapeSubType.TaperedCapsule);
		}
		finally
		{
			JoltPhysics.Shape_Destroy(tapered);
		}
	}

	[Fact]
	public void ConvexHullShape_create_from_tetrahedron()
	{
		Vec3* points = stackalloc Vec3[4]
		{
			Vec3(0f, 1f, 0f),
			Vec3(-1f, -1f, 1f),
			Vec3(1f, -1f, 1f),
			Vec3(0f, -1f, -1f),
		};

		IntPtr hull = JoltPhysics.ConvexHullShape_Create(points, 4, 0.05f);
		try
		{
			hull.Should().NotBe(IntPtr.Zero);
			JoltPhysics.Shape_GetSubType(hull).Should().Be(ShapeSubType.ConvexHull);
		}
		finally
		{
			JoltPhysics.Shape_Destroy(hull);
		}
	}

	[Fact]
	public void StaticCompoundShape_with_two_boxes()
	{
		IntPtr box1 = JoltPhysics.BoxShape_Create(Vec3(0.5f, 0.5f, 0.5f), 0.05f);
		IntPtr box2 = JoltPhysics.BoxShape_Create(Vec3(0.3f, 0.3f, 0.3f), 0.05f);
		try
		{
			CompoundShapeSubShape* subShapes = stackalloc CompoundShapeSubShape[2];
			subShapes[0].Position = Vec3(-1f, 0f, 0f);
			subShapes[0].Rotation = IdentityQuat();
			subShapes[0] .Shape = box1;
			subShapes[0].UserData = 0;

			subShapes[1].Position = Vec3(1f, 0f, 0f);
			subShapes[1].Rotation = IdentityQuat();
			subShapes[1] .Shape = box2;
			subShapes[1].UserData = 0;

			IntPtr compound = JoltPhysics.StaticCompoundShape_Create(subShapes, 2);
			try
			{
				compound.Should().NotBe(IntPtr.Zero);
				JoltPhysics.Shape_GetType(compound).Should().Be(ShapeType.Compound);
				JoltPhysics.Shape_GetSubType(compound).Should().Be(ShapeSubType.StaticCompound);
			}
			finally
			{
				JoltPhysics.Shape_Destroy(compound);
			}
		}
		finally
		{
			JoltPhysics.Shape_Destroy(box1);
			JoltPhysics.Shape_Destroy(box2);
		}
	}

	[Fact]
	public void Shape_GetVolume_returns_positive_for_box()
	{
		IntPtr box = JoltPhysics.BoxShape_Create(Vec3(1f, 1f, 1f), 0f);
		try
		{
			float volume = JoltPhysics.Shape_GetVolume(box);
			volume.Should().BeGreaterThan(0f);
			// Box with half-extents (1,1,1) has full extents (2,2,2) → volume ≈ 8
			volume.Should().BeApproximately(8f, 0.5f);
		}
		finally
		{
			JoltPhysics.Shape_Destroy(box);
		}
	}

	[Fact]
	public void Shape_GetLocalBounds_for_sphere()
	{
		IntPtr sphere = JoltPhysics.SphereShape_Create(3.0f);
		try
		{
			AABox bounds = JoltPhysics.Shape_GetLocalBounds(sphere);
			bounds.Min.X.Should().BeApproximately(-3f, 1e-5f);
			bounds.Min.Y.Should().BeApproximately(-3f, 1e-5f);
			bounds.Min.Z.Should().BeApproximately(-3f, 1e-5f);
			bounds.Max.X.Should().BeApproximately(3f, 1e-5f);
			bounds.Max.Y.Should().BeApproximately(3f, 1e-5f);
			bounds.Max.Z.Should().BeApproximately(3f, 1e-5f);
		}
		finally
		{
			JoltPhysics.Shape_Destroy(sphere);
		}
	}

	[Fact]
	public void Shape_GetCenterOfMass_for_symmetric_box_is_zero()
	{
		IntPtr box = JoltPhysics.BoxShape_Create(Vec3(1f, 2f, 3f), 0.05f);
		try
		{
			Vec3 com = JoltPhysics.Shape_GetCenterOfMass(box);
			com.X.Should().BeApproximately(0f, 1e-5f);
			com.Y.Should().BeApproximately(0f, 1e-5f);
			com.Z.Should().BeApproximately(0f, 1e-5f);
		}
		finally
		{
			JoltPhysics.Shape_Destroy(box);
		}
	}

	[Fact]
	public void Shape_GetInnerRadius_for_sphere()
	{
		IntPtr sphere = JoltPhysics.SphereShape_Create(1.5f);
		try
		{
			float inner = JoltPhysics.Shape_GetInnerRadius(sphere);
			inner.Should().BeApproximately(1.5f, 1e-5f);
		}
		finally
		{
			JoltPhysics.Shape_Destroy(sphere);
		}
	}

	[Fact]
	public void Shape_GetMassProperties_for_box()
	{
		IntPtr box = JoltPhysics.BoxShape_Create(Vec3(1f, 1f, 1f), 0f);
		try
		{
			float mass;
			Mat44 inertia;
			JoltPhysics.Shape_GetMassProperties(box, &mass, &inertia);
			mass.Should().BeGreaterThan(0f);
			// Inertia diagonal should be positive
			inertia.M[0].Should().BeGreaterThan(0f);
			inertia.M[5].Should().BeGreaterThan(0f);
			inertia.M[10].Should().BeGreaterThan(0f);
		}
		finally
		{
			JoltPhysics.Shape_Destroy(box);
		}
	}

	[Fact]
	public void Shape_CastRay_hits_sphere()
	{
		IntPtr sphere = JoltPhysics.SphereShape_Create(1.0f);
		try
		{
			// Ray from (-5,0,0) with direction (10,0,0) — direction is the full ray extent, not unit
			float fraction;
			int hit = JoltPhysics.Shape_CastRay(sphere, Vec3(-5f, 0f, 0f), Vec3(10f, 0f, 0f), &fraction);
			hit.Should().Be(1);
			fraction.Should().BeGreaterThan(0f);
			fraction.Should().BeLessThan(1f);
		}
		finally
		{
			JoltPhysics.Shape_Destroy(sphere);
		}
	}

	[Fact]
	public void Shape_CastRay_misses_when_aiming_away()
	{
		IntPtr sphere = JoltPhysics.SphereShape_Create(1.0f);
		try
		{
			// Ray from (5,0,0) going further away (+1,0,0) should miss
			float fraction;
			int hit = JoltPhysics.Shape_CastRay(sphere, Vec3(5f, 0f, 0f), Vec3(1f, 0f, 0f), &fraction);
			hit.Should().Be(0);
		}
		finally
		{
			JoltPhysics.Shape_Destroy(sphere);
		}
	}

	[Fact]
	public void Shape_CollidePoint_inside_sphere()
	{
		IntPtr sphere = JoltPhysics.SphereShape_Create(2.0f);
		try
		{
			int inside = JoltPhysics.Shape_CollidePoint(sphere, Vec3(0f, 0f, 0f));
			inside.Should().Be(1, "origin should be inside the sphere");
		}
		finally
		{
			JoltPhysics.Shape_Destroy(sphere);
		}
	}

	[Fact]
	public void Shape_CollidePoint_outside_sphere()
	{
		IntPtr sphere = JoltPhysics.SphereShape_Create(1.0f);
		try
		{
			int inside = JoltPhysics.Shape_CollidePoint(sphere, Vec3(5f, 0f, 0f));
			inside.Should().Be(0, "point should be outside the sphere");
		}
		finally
		{
			JoltPhysics.Shape_Destroy(sphere);
		}
	}

	[Fact]
	public void MeshShape_create_single_triangle()
	{
		Vec3* vertices = stackalloc Vec3[3]
		{
			Vec3(0f, 0f, 0f),
			Vec3(1f, 0f, 0f),
			Vec3(0f, 1f, 0f),
		};

		IndexedTriangle* triangles = stackalloc IndexedTriangle[1];
		triangles[0].I1 = 0;
		triangles[0].I2 = 1;
		triangles[0].I3 = 2;
		triangles[0].MaterialIndex = 0;
		triangles[0].UserData = 0;

		IntPtr mesh = JoltPhysics.MeshShape_Create(vertices, 3, triangles, 1);
		try
		{
			mesh.Should().NotBe(IntPtr.Zero);
			JoltPhysics.Shape_GetType(mesh).Should().Be(ShapeType.Mesh);
			JoltPhysics.Shape_GetSubType(mesh).Should().Be(ShapeSubType.Mesh);
		}
		finally
		{
			JoltPhysics.Shape_Destroy(mesh);
		}
	}
}
