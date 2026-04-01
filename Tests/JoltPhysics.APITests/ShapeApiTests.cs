using System;
using Evergine.Bindings.JoltPhysics;
using FluentAssertions;
using Xunit;
using static JoltPhysics.APITests.NativeTestHelpers;

namespace JoltPhysics.APITests;

[Collection("Jolt")]
public unsafe class ShapeApiTests
{
	[Fact]
	public void SphereShape_create_and_query_radius()
	{
		IntPtr sphere = JoltPhysicsNative.JoltC_SphereShape_Create(2.5f);
		try
		{
			sphere.Should().NotBe(IntPtr.Zero);
			JoltPhysicsNative.JoltC_Shape_GetType(sphere).Should().Be(JoltC_ShapeType.JOLTC_SHAPE_TYPE_CONVEX);
			JoltPhysicsNative.JoltC_Shape_GetSubType(sphere).Should().Be(JoltC_ShapeSubType.JOLTC_SHAPE_SUB_TYPE_SPHERE);
			JoltPhysicsNative.JoltC_SphereShape_GetRadius(sphere).Should().BeApproximately(2.5f, 1e-5f);
		}
		finally
		{
			JoltPhysicsNative.JoltC_Shape_Destroy(sphere);
		}
	}

	[Fact]
	public void CapsuleShape_create_and_query_dimensions()
	{
		IntPtr capsule = JoltPhysicsNative.JoltC_CapsuleShape_Create(1.0f, 0.5f);
		try
		{
			capsule.Should().NotBe(IntPtr.Zero);
			JoltPhysicsNative.JoltC_Shape_GetSubType(capsule).Should().Be(JoltC_ShapeSubType.JOLTC_SHAPE_SUB_TYPE_CAPSULE);
			JoltPhysicsNative.JoltC_CapsuleShape_GetHalfHeightOfCylinder(capsule).Should().BeApproximately(1.0f, 1e-5f);
			JoltPhysicsNative.JoltC_CapsuleShape_GetRadius(capsule).Should().BeApproximately(0.5f, 1e-5f);
		}
		finally
		{
			JoltPhysicsNative.JoltC_Shape_Destroy(capsule);
		}
	}

	[Fact]
	public void CylinderShape_create_and_query_dimensions()
	{
		IntPtr cylinder = JoltPhysicsNative.JoltC_CylinderShape_Create(2.0f, 1.0f, 0.05f);
		try
		{
			cylinder.Should().NotBe(IntPtr.Zero);
			JoltPhysicsNative.JoltC_Shape_GetSubType(cylinder).Should().Be(JoltC_ShapeSubType.JOLTC_SHAPE_SUB_TYPE_CYLINDER);
			JoltPhysicsNative.JoltC_CylinderShape_GetHalfHeight(cylinder).Should().BeApproximately(2.0f, 1e-5f);
			JoltPhysicsNative.JoltC_CylinderShape_GetRadius(cylinder).Should().BeApproximately(1.0f, 1e-5f);
		}
		finally
		{
			JoltPhysicsNative.JoltC_Shape_Destroy(cylinder);
		}
	}

	[Fact]
	public void TaperedCapsuleShape_create_returns_valid_handle()
	{
		IntPtr tapered = JoltPhysicsNative.JoltC_TaperedCapsuleShape_Create(1.0f, 0.5f, 0.3f);
		try
		{
			tapered.Should().NotBe(IntPtr.Zero);
			JoltPhysicsNative.JoltC_Shape_GetSubType(tapered).Should().Be(JoltC_ShapeSubType.JOLTC_SHAPE_SUB_TYPE_TAPERED_CAPSULE);
		}
		finally
		{
			JoltPhysicsNative.JoltC_Shape_Destroy(tapered);
		}
	}

	[Fact]
	public void ConvexHullShape_create_from_tetrahedron()
	{
		JoltC_Vec3* points = stackalloc JoltC_Vec3[4]
		{
			Vec3(0f, 1f, 0f),
			Vec3(-1f, -1f, 1f),
			Vec3(1f, -1f, 1f),
			Vec3(0f, -1f, -1f),
		};

		IntPtr hull = JoltPhysicsNative.JoltC_ConvexHullShape_Create(points, 4, 0.05f);
		try
		{
			hull.Should().NotBe(IntPtr.Zero);
			JoltPhysicsNative.JoltC_Shape_GetSubType(hull).Should().Be(JoltC_ShapeSubType.JOLTC_SHAPE_SUB_TYPE_CONVEX_HULL);
		}
		finally
		{
			JoltPhysicsNative.JoltC_Shape_Destroy(hull);
		}
	}

	[Fact]
	public void StaticCompoundShape_with_two_boxes()
	{
		IntPtr box1 = JoltPhysicsNative.JoltC_BoxShape_Create(Vec3(0.5f, 0.5f, 0.5f), 0.05f);
		IntPtr box2 = JoltPhysicsNative.JoltC_BoxShape_Create(Vec3(0.3f, 0.3f, 0.3f), 0.05f);
		try
		{
			JoltC_CompoundShapeSubShape* subShapes = stackalloc JoltC_CompoundShapeSubShape[2];
			subShapes[0].position = Vec3(-1f, 0f, 0f);
			subShapes[0].rotation = IdentityQuat();
			subShapes[0].shape = box1;
			subShapes[0].userData = 0;

			subShapes[1].position = Vec3(1f, 0f, 0f);
			subShapes[1].rotation = IdentityQuat();
			subShapes[1].shape = box2;
			subShapes[1].userData = 0;

			IntPtr compound = JoltPhysicsNative.JoltC_StaticCompoundShape_Create(subShapes, 2);
			try
			{
				compound.Should().NotBe(IntPtr.Zero);
				JoltPhysicsNative.JoltC_Shape_GetType(compound).Should().Be(JoltC_ShapeType.JOLTC_SHAPE_TYPE_COMPOUND);
				JoltPhysicsNative.JoltC_Shape_GetSubType(compound).Should().Be(JoltC_ShapeSubType.JOLTC_SHAPE_SUB_TYPE_STATIC_COMPOUND);
			}
			finally
			{
				JoltPhysicsNative.JoltC_Shape_Destroy(compound);
			}
		}
		finally
		{
			JoltPhysicsNative.JoltC_Shape_Destroy(box1);
			JoltPhysicsNative.JoltC_Shape_Destroy(box2);
		}
	}

	[Fact]
	public void Shape_GetVolume_returns_positive_for_box()
	{
		IntPtr box = JoltPhysicsNative.JoltC_BoxShape_Create(Vec3(1f, 1f, 1f), 0f);
		try
		{
			float volume = JoltPhysicsNative.JoltC_Shape_GetVolume(box);
			volume.Should().BeGreaterThan(0f);
			// Box with half-extents (1,1,1) has full extents (2,2,2) → volume ≈ 8
			volume.Should().BeApproximately(8f, 0.5f);
		}
		finally
		{
			JoltPhysicsNative.JoltC_Shape_Destroy(box);
		}
	}

	[Fact]
	public void Shape_GetLocalBounds_for_sphere()
	{
		IntPtr sphere = JoltPhysicsNative.JoltC_SphereShape_Create(3.0f);
		try
		{
			JoltC_AABox bounds = JoltPhysicsNative.JoltC_Shape_GetLocalBounds(sphere);
			bounds.min.x.Should().BeApproximately(-3f, 1e-5f);
			bounds.min.y.Should().BeApproximately(-3f, 1e-5f);
			bounds.min.z.Should().BeApproximately(-3f, 1e-5f);
			bounds.max.x.Should().BeApproximately(3f, 1e-5f);
			bounds.max.y.Should().BeApproximately(3f, 1e-5f);
			bounds.max.z.Should().BeApproximately(3f, 1e-5f);
		}
		finally
		{
			JoltPhysicsNative.JoltC_Shape_Destroy(sphere);
		}
	}

	[Fact]
	public void Shape_GetCenterOfMass_for_symmetric_box_is_zero()
	{
		IntPtr box = JoltPhysicsNative.JoltC_BoxShape_Create(Vec3(1f, 2f, 3f), 0.05f);
		try
		{
			JoltC_Vec3 com = JoltPhysicsNative.JoltC_Shape_GetCenterOfMass(box);
			com.x.Should().BeApproximately(0f, 1e-5f);
			com.y.Should().BeApproximately(0f, 1e-5f);
			com.z.Should().BeApproximately(0f, 1e-5f);
		}
		finally
		{
			JoltPhysicsNative.JoltC_Shape_Destroy(box);
		}
	}

	[Fact]
	public void Shape_GetInnerRadius_for_sphere()
	{
		IntPtr sphere = JoltPhysicsNative.JoltC_SphereShape_Create(1.5f);
		try
		{
			float inner = JoltPhysicsNative.JoltC_Shape_GetInnerRadius(sphere);
			inner.Should().BeApproximately(1.5f, 1e-5f);
		}
		finally
		{
			JoltPhysicsNative.JoltC_Shape_Destroy(sphere);
		}
	}

	[Fact]
	public void Shape_GetMassProperties_for_box()
	{
		IntPtr box = JoltPhysicsNative.JoltC_BoxShape_Create(Vec3(1f, 1f, 1f), 0f);
		try
		{
			float mass;
			JoltC_Mat44 inertia;
			JoltPhysicsNative.JoltC_Shape_GetMassProperties(box, &mass, &inertia);
			mass.Should().BeGreaterThan(0f);
			// Inertia diagonal should be positive
			inertia.m[0].Should().BeGreaterThan(0f);
			inertia.m[5].Should().BeGreaterThan(0f);
			inertia.m[10].Should().BeGreaterThan(0f);
		}
		finally
		{
			JoltPhysicsNative.JoltC_Shape_Destroy(box);
		}
	}

	[Fact]
	public void Shape_CastRay_hits_sphere()
	{
		IntPtr sphere = JoltPhysicsNative.JoltC_SphereShape_Create(1.0f);
		try
		{
			// Ray from (-5,0,0) with direction (10,0,0) — direction is the full ray extent, not unit
			float fraction;
			int hit = JoltPhysicsNative.JoltC_Shape_CastRay(sphere, Vec3(-5f, 0f, 0f), Vec3(10f, 0f, 0f), &fraction);
			hit.Should().Be(1);
			fraction.Should().BeGreaterThan(0f);
			fraction.Should().BeLessThan(1f);
		}
		finally
		{
			JoltPhysicsNative.JoltC_Shape_Destroy(sphere);
		}
	}

	[Fact]
	public void Shape_CastRay_misses_when_aiming_away()
	{
		IntPtr sphere = JoltPhysicsNative.JoltC_SphereShape_Create(1.0f);
		try
		{
			// Ray from (5,0,0) going further away (+1,0,0) should miss
			float fraction;
			int hit = JoltPhysicsNative.JoltC_Shape_CastRay(sphere, Vec3(5f, 0f, 0f), Vec3(1f, 0f, 0f), &fraction);
			hit.Should().Be(0);
		}
		finally
		{
			JoltPhysicsNative.JoltC_Shape_Destroy(sphere);
		}
	}

	[Fact]
	public void Shape_CollidePoint_inside_sphere()
	{
		IntPtr sphere = JoltPhysicsNative.JoltC_SphereShape_Create(2.0f);
		try
		{
			int inside = JoltPhysicsNative.JoltC_Shape_CollidePoint(sphere, Vec3(0f, 0f, 0f));
			inside.Should().Be(1, "origin should be inside the sphere");
		}
		finally
		{
			JoltPhysicsNative.JoltC_Shape_Destroy(sphere);
		}
	}

	[Fact]
	public void Shape_CollidePoint_outside_sphere()
	{
		IntPtr sphere = JoltPhysicsNative.JoltC_SphereShape_Create(1.0f);
		try
		{
			int inside = JoltPhysicsNative.JoltC_Shape_CollidePoint(sphere, Vec3(5f, 0f, 0f));
			inside.Should().Be(0, "point should be outside the sphere");
		}
		finally
		{
			JoltPhysicsNative.JoltC_Shape_Destroy(sphere);
		}
	}

	[Fact]
	public void MeshShape_create_single_triangle()
	{
		JoltC_Vec3* vertices = stackalloc JoltC_Vec3[3]
		{
			Vec3(0f, 0f, 0f),
			Vec3(1f, 0f, 0f),
			Vec3(0f, 1f, 0f),
		};

		JoltC_IndexedTriangle* triangles = stackalloc JoltC_IndexedTriangle[1];
		triangles[0].i1 = 0;
		triangles[0].i2 = 1;
		triangles[0].i3 = 2;
		triangles[0].materialIndex = 0;
		triangles[0].userData = 0;

		IntPtr mesh = JoltPhysicsNative.JoltC_MeshShape_Create(vertices, 3, triangles, 1);
		try
		{
			mesh.Should().NotBe(IntPtr.Zero);
			JoltPhysicsNative.JoltC_Shape_GetType(mesh).Should().Be(JoltC_ShapeType.JOLTC_SHAPE_TYPE_MESH);
			JoltPhysicsNative.JoltC_Shape_GetSubType(mesh).Should().Be(JoltC_ShapeSubType.JOLTC_SHAPE_SUB_TYPE_MESH);
		}
		finally
		{
			JoltPhysicsNative.JoltC_Shape_Destroy(mesh);
		}
	}
}
