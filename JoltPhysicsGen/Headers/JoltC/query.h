/* JoltC - C bindings for JoltPhysics
 * SPDX-License-Identifier: MIT
 *
 * Narrow phase queries (raycasting, shape casting, collision).
 * Broad phase queries (AABB, sphere, point overlap).
 */

#ifndef JOLTC_QUERY_H
#define JOLTC_QUERY_H

#include <JoltC/common.h>

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */
/*  NarrowPhaseQuery — obtained from PhysicsSystem, not owned by caller       */
/* -------------------------------------------------------------------------- */
JOLTC_API const JoltC_NarrowPhaseQuery* JoltC_PhysicsSystem_GetNarrowPhaseQuery(const JoltC_PhysicsSystem* system);
JOLTC_API const JoltC_NarrowPhaseQuery* JoltC_PhysicsSystem_GetNarrowPhaseQueryNoLock(const JoltC_PhysicsSystem* system);

/* -------------------------------------------------------------------------- */
/*  CastRay — single closest hit (no filters)                                 */
/* -------------------------------------------------------------------------- */
JOLTC_API JoltC_Bool JoltC_NarrowPhaseQuery_CastRay(
    const JoltC_NarrowPhaseQuery* query,
    JoltC_RVec3                   origin,
    JoltC_Vec3                    direction,
    JoltC_RayCastResult*          outResult);

/* -------------------------------------------------------------------------- */
/*  CastRay — multiple hits via callback (no filters)                         */
/* -------------------------------------------------------------------------- */
typedef void (*JoltC_CastRayCollectorFn)(void* userData, const JoltC_RayCastResult* result);

JOLTC_API void JoltC_NarrowPhaseQuery_CastRayAll(
    const JoltC_NarrowPhaseQuery*  query,
    JoltC_RVec3                    origin,
    JoltC_Vec3                     direction,
    const JoltC_RayCastSettings*   rayCastSettings,
    JoltC_CastRayCollectorFn       callback,
    void*                          userData);

/* -------------------------------------------------------------------------- */
/*  CollidePoint — check if point is inside any shape (no filters)            */
/* -------------------------------------------------------------------------- */
typedef void (*JoltC_CollidePointCollectorFn)(void* userData, JoltC_BodyID bodyID, uint32_t subShapeID);

JOLTC_API void JoltC_NarrowPhaseQuery_CollidePoint(
    const JoltC_NarrowPhaseQuery*    query,
    JoltC_RVec3                      point,
    JoltC_CollidePointCollectorFn    callback,
    void*                            userData);

/* -------------------------------------------------------------------------- */
/*  CastRay2 — single closest hit with filter support                         */
/* -------------------------------------------------------------------------- */
JOLTC_API JoltC_Bool JoltC_NarrowPhaseQuery_CastRay2(
    const JoltC_NarrowPhaseQuery*        query,
    JoltC_RVec3                          origin,
    JoltC_Vec3                           direction,
    JoltC_RayCastResult*                 outResult,
    const JoltC_BroadPhaseLayerFilter*   bpFilter,      /* nullable */
    const JoltC_ObjectLayerFilter*       olFilter,      /* nullable */
    const JoltC_BodyFilter*              bodyFilter);   /* nullable */

/* -------------------------------------------------------------------------- */
/*  CastRay3 — multiple hits via callback with all filters                    */
/* -------------------------------------------------------------------------- */
JOLTC_API void JoltC_NarrowPhaseQuery_CastRay3(
    const JoltC_NarrowPhaseQuery*        query,
    JoltC_RVec3                          origin,
    JoltC_Vec3                           direction,
    const JoltC_RayCastSettings*         rayCastSettings,
    JoltC_CastRayCollectorFn             callback,
    void*                                userData,
    const JoltC_BroadPhaseLayerFilter*   bpFilter,      /* nullable */
    const JoltC_ObjectLayerFilter*       olFilter,      /* nullable */
    const JoltC_BodyFilter*              bodyFilter,    /* nullable */
    const JoltC_ShapeFilter*             shapeFilter);  /* nullable */

/* -------------------------------------------------------------------------- */
/*  CollidePoint2 — with filter support                                       */
/* -------------------------------------------------------------------------- */
JOLTC_API void JoltC_NarrowPhaseQuery_CollidePoint2(
    const JoltC_NarrowPhaseQuery*        query,
    JoltC_RVec3                          point,
    JoltC_CollidePointCollectorFn        callback,
    void*                                userData,
    const JoltC_BroadPhaseLayerFilter*   bpFilter,      /* nullable */
    const JoltC_ObjectLayerFilter*       olFilter,      /* nullable */
    const JoltC_BodyFilter*              bodyFilter,    /* nullable */
    const JoltC_ShapeFilter*             shapeFilter);  /* nullable */

/* -------------------------------------------------------------------------- */
/*  CollideShape — find all body shapes overlapping with a query shape        */
/* -------------------------------------------------------------------------- */
JOLTC_API void JoltC_NarrowPhaseQuery_CollideShape(
    const JoltC_NarrowPhaseQuery*        query,
    const JoltC_Shape*                   shape,
    JoltC_Vec3                           scale,
    JoltC_Mat44                          centerOfMassTransform,
    JoltC_RVec3                          baseOffset,
    JoltC_CollideShapeResultFn           callback,
    void*                                userData,
    const JoltC_BroadPhaseLayerFilter*   bpFilter,      /* nullable */
    const JoltC_ObjectLayerFilter*       olFilter,      /* nullable */
    const JoltC_BodyFilter*              bodyFilter,    /* nullable */
    const JoltC_ShapeFilter*             shapeFilter);  /* nullable */

/* -------------------------------------------------------------------------- */
/*  CastShape — sweep a shape along a direction                               */
/* -------------------------------------------------------------------------- */
JOLTC_API void JoltC_NarrowPhaseQuery_CastShape(
    const JoltC_NarrowPhaseQuery*        query,
    const JoltC_Shape*                   shape,
    JoltC_Vec3                           scale,
    JoltC_Mat44                          centerOfMassTransform,
    JoltC_Vec3                           direction,
    JoltC_RVec3                          baseOffset,
    JoltC_CastShapeResultFn              callback,
    void*                                userData,
    const JoltC_BroadPhaseLayerFilter*   bpFilter,      /* nullable */
    const JoltC_ObjectLayerFilter*       olFilter,      /* nullable */
    const JoltC_BodyFilter*              bodyFilter,    /* nullable */
    const JoltC_ShapeFilter*             shapeFilter);  /* nullable */

/* -------------------------------------------------------------------------- */
/*  CollideShape2 / CastShape2 — with collision settings                      */
/* -------------------------------------------------------------------------- */
/* The variants that consume JoltC_CollideShapeSettings and JoltC_ShapeCastSettings, which existed
 * with their Init helpers and no function that accepted them: max separation distance, back face
 * modes and the rest were unreachable. Null settings keep the defaults. */
JOLTC_API void JoltC_NarrowPhaseQuery_CollideShape2(
    const JoltC_NarrowPhaseQuery*        query,
    const JoltC_Shape*                   shape,
    JoltC_Vec3                           scale,
    JoltC_Mat44                          centerOfMassTransform,
    const JoltC_CollideShapeSettings*    settings,      /* nullable */
    JoltC_RVec3                          baseOffset,
    JoltC_CollideShapeResultFn           callback,
    void*                                userData,
    const JoltC_BroadPhaseLayerFilter*   bpFilter,      /* nullable */
    const JoltC_ObjectLayerFilter*       olFilter,      /* nullable */
    const JoltC_BodyFilter*              bodyFilter,    /* nullable */
    const JoltC_ShapeFilter*             shapeFilter);  /* nullable */

JOLTC_API void JoltC_NarrowPhaseQuery_CastShape2(
    const JoltC_NarrowPhaseQuery*        query,
    const JoltC_Shape*                   shape,
    JoltC_Vec3                           scale,
    JoltC_Mat44                          centerOfMassTransform,
    JoltC_Vec3                           direction,
    const JoltC_ShapeCastSettings*       settings,      /* nullable */
    JoltC_RVec3                          baseOffset,
    JoltC_CastShapeResultFn              callback,
    void*                                userData,
    const JoltC_BroadPhaseLayerFilter*   bpFilter,      /* nullable */
    const JoltC_ObjectLayerFilter*       olFilter,      /* nullable */
    const JoltC_BodyFilter*              bodyFilter,    /* nullable */
    const JoltC_ShapeFilter*             shapeFilter);  /* nullable */

/* -------------------------------------------------------------------------- */
/*  CollideShapeWithInternalEdgeRemoval — no ghost hits on mesh seams         */
/* -------------------------------------------------------------------------- */
/* Same contract as CollideShape2, but contacts against the internal edges of triangle meshes and
 * height fields are removed, which is what a character or vehicle sliding along flat ground wants. */
JOLTC_API void JoltC_NarrowPhaseQuery_CollideShapeWithInternalEdgeRemoval(
    const JoltC_NarrowPhaseQuery*        query,
    const JoltC_Shape*                   shape,
    JoltC_Vec3                           scale,
    JoltC_Mat44                          centerOfMassTransform,
    const JoltC_CollideShapeSettings*    settings,      /* nullable */
    JoltC_RVec3                          baseOffset,
    JoltC_CollideShapeResultFn           callback,
    void*                                userData,
    const JoltC_BroadPhaseLayerFilter*   bpFilter,      /* nullable */
    const JoltC_ObjectLayerFilter*       olFilter,      /* nullable */
    const JoltC_BodyFilter*              bodyFilter,    /* nullable */
    const JoltC_ShapeFilter*             shapeFilter);  /* nullable */

/* -------------------------------------------------------------------------- */
/*  CastShapeClosest — first hit only, with the early-out the collector earns */
/* -------------------------------------------------------------------------- */
/* The all-hits cast visits everything the sweep touches; this one uses Jolt's closest-hit
 * collector, whose early-out shrinks the sweep as hits land — the efficient way to ask "what do
 * I hit first". Returns JOLTC_FALSE when the sweep hits nothing. */
JOLTC_API JoltC_Bool JoltC_NarrowPhaseQuery_CastShapeClosest(
    const JoltC_NarrowPhaseQuery*        query,
    const JoltC_Shape*                   shape,
    JoltC_Vec3                           scale,
    JoltC_Mat44                          centerOfMassTransform,
    JoltC_Vec3                           direction,
    const JoltC_ShapeCastSettings*       settings,      /* nullable */
    JoltC_RVec3                          baseOffset,
    JoltC_ShapeCastResult*               outResult,
    const JoltC_BroadPhaseLayerFilter*   bpFilter,      /* nullable */
    const JoltC_ObjectLayerFilter*       olFilter,      /* nullable */
    const JoltC_BodyFilter*              bodyFilter,    /* nullable */
    const JoltC_ShapeFilter*             shapeFilter);  /* nullable */

/* -------------------------------------------------------------------------- */
/*  Default layer filters — the ones a query against one layer wants          */
/* -------------------------------------------------------------------------- */
/* Wrap the system's own layer logic as filters, so a caller querying "what layer X can hit" does
 * not have to reimplement the layer matrix on its side. The caller owns the returned filter and
 * frees it with the matching Destroy from filters.h. */
JOLTC_API JoltC_BroadPhaseLayerFilter* JoltC_PhysicsSystem_GetDefaultBroadPhaseLayerFilter(const JoltC_PhysicsSystem* system, JoltC_ObjectLayer layer);
JOLTC_API JoltC_ObjectLayerFilter*     JoltC_PhysicsSystem_GetDefaultLayerFilter(const JoltC_PhysicsSystem* system, JoltC_ObjectLayer layer);

/* -------------------------------------------------------------------------- */
/*  BroadPhaseQuery — obtained from PhysicsSystem, not owned by caller        */
/* -------------------------------------------------------------------------- */
JOLTC_API const JoltC_BroadPhaseQuery* JoltC_PhysicsSystem_GetBroadPhaseQuery(const JoltC_PhysicsSystem* system);

/* -------------------------------------------------------------------------- */
/*  BroadPhaseQuery — CastRay                                                 */
/* -------------------------------------------------------------------------- */
JOLTC_API void JoltC_BroadPhaseQuery_CastRay(
    const JoltC_BroadPhaseQuery*         query,
    JoltC_Vec3                           origin,
    JoltC_Vec3                           direction,
    JoltC_BroadPhaseCastResultFn         callback,
    void*                                userData,
    const JoltC_BroadPhaseLayerFilter*   bpFilter,      /* nullable */
    const JoltC_ObjectLayerFilter*       olFilter);     /* nullable */

/* -------------------------------------------------------------------------- */
/*  BroadPhaseQuery — CollideAABox                                            */
/* -------------------------------------------------------------------------- */
JOLTC_API void JoltC_BroadPhaseQuery_CollideAABox(
    const JoltC_BroadPhaseQuery*         query,
    JoltC_AABox                          box,
    JoltC_CollideShapeBodyResultFn       callback,
    void*                                userData,
    const JoltC_BroadPhaseLayerFilter*   bpFilter,      /* nullable */
    const JoltC_ObjectLayerFilter*       olFilter);     /* nullable */

/* -------------------------------------------------------------------------- */
/*  BroadPhaseQuery — CollideSphere                                           */
/* -------------------------------------------------------------------------- */
JOLTC_API void JoltC_BroadPhaseQuery_CollideSphere(
    const JoltC_BroadPhaseQuery*         query,
    JoltC_Vec3                           center,
    float                                radius,
    JoltC_CollideShapeBodyResultFn       callback,
    void*                                userData,
    const JoltC_BroadPhaseLayerFilter*   bpFilter,      /* nullable */
    const JoltC_ObjectLayerFilter*       olFilter);     /* nullable */

/* -------------------------------------------------------------------------- */
/*  BroadPhaseQuery — CollidePoint                                            */
/* -------------------------------------------------------------------------- */
JOLTC_API void JoltC_BroadPhaseQuery_CollidePoint(
    const JoltC_BroadPhaseQuery*         query,
    JoltC_Vec3                           point,
    JoltC_CollideShapeBodyResultFn       callback,
    void*                                userData,
    const JoltC_BroadPhaseLayerFilter*   bpFilter,      /* nullable */
    const JoltC_ObjectLayerFilter*       olFilter);     /* nullable */

/* Every body whose bounds an axis aligned box sweep touches: coarse, fast, no narrow phase. */
JOLTC_API void JoltC_BroadPhaseQuery_CastAABox(
    const JoltC_BroadPhaseQuery*         query,
    JoltC_AABox                          box,
    JoltC_Vec3                           direction,
    JoltC_BroadPhaseCastResultFn         callback,
    void*                                userData,
    const JoltC_BroadPhaseLayerFilter*   bpFilter,      /* nullable */
    const JoltC_ObjectLayerFilter*       olFilter);     /* nullable */

/* Every body whose bounds overlap an oriented box. */
JOLTC_API void JoltC_BroadPhaseQuery_CollideOrientedBox(
    const JoltC_BroadPhaseQuery*         query,
    const JoltC_OrientedBox*             box,
    JoltC_CollideShapeBodyResultFn       callback,
    void*                                userData,
    const JoltC_BroadPhaseLayerFilter*   bpFilter,      /* nullable */
    const JoltC_ObjectLayerFilter*       olFilter);     /* nullable */

/* -------------------------------------------------------------------------- */
/*  ContactManifold reader functions                                          */
/* -------------------------------------------------------------------------- */
JOLTC_API JoltC_Vec3 JoltC_ContactManifold_GetWorldSpaceNormal(const JoltC_ContactManifold* manifold);
JOLTC_API float      JoltC_ContactManifold_GetPenetrationDepth(const JoltC_ContactManifold* manifold);
JOLTC_API JoltC_SubShapeID JoltC_ContactManifold_GetSubShapeID1(const JoltC_ContactManifold* manifold);
JOLTC_API JoltC_SubShapeID JoltC_ContactManifold_GetSubShapeID2(const JoltC_ContactManifold* manifold);
JOLTC_API uint32_t   JoltC_ContactManifold_GetPointCount(const JoltC_ContactManifold* manifold);
JOLTC_API JoltC_RVec3 JoltC_ContactManifold_GetWorldSpaceContactPointOn1(const JoltC_ContactManifold* manifold, uint32_t index);
JOLTC_API JoltC_RVec3 JoltC_ContactManifold_GetWorldSpaceContactPointOn2(const JoltC_ContactManifold* manifold, uint32_t index);

#ifdef __cplusplus
}
#endif

#endif /* JOLTC_QUERY_H */
