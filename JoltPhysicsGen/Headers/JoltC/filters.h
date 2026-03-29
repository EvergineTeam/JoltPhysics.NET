/* JoltC - C bindings for JoltPhysics
 * SPDX-License-Identifier: MIT
 *
 * Query filters: BroadPhaseLayerFilter, ObjectLayerFilter, BodyFilter,
 * ShapeFilter, SimShapeFilter.
 */

#ifndef JOLTC_FILTERS_H
#define JOLTC_FILTERS_H

#include <JoltC/common.h>

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */
/*  BroadPhaseLayerFilter                                                     */
/* -------------------------------------------------------------------------- */
JOLTC_API JoltC_BroadPhaseLayerFilter* JoltC_BroadPhaseLayerFilter_Create(
    JoltC_BroadPhaseLayerFilterFn fn,
    void* userData);
JOLTC_API void JoltC_BroadPhaseLayerFilter_Destroy(JoltC_BroadPhaseLayerFilter* filter);

/* -------------------------------------------------------------------------- */
/*  ObjectLayerFilter                                                         */
/* -------------------------------------------------------------------------- */
JOLTC_API JoltC_ObjectLayerFilter* JoltC_ObjectLayerFilter_Create(
    JoltC_ObjectLayerFilterFn fn,
    void* userData);
JOLTC_API void JoltC_ObjectLayerFilter_Destroy(JoltC_ObjectLayerFilter* filter);

/* -------------------------------------------------------------------------- */
/*  BodyFilter                                                                */
/* -------------------------------------------------------------------------- */
JOLTC_API JoltC_BodyFilter* JoltC_BodyFilter_Create(
    JoltC_BodyFilterFn         fn,
    JoltC_BodyFilterLockedFn   fnLocked,   /* may be NULL */
    void*                      userData);
JOLTC_API void JoltC_BodyFilter_Destroy(JoltC_BodyFilter* filter);

/* -------------------------------------------------------------------------- */
/*  ShapeFilter                                                               */
/* -------------------------------------------------------------------------- */
JOLTC_API JoltC_ShapeFilter* JoltC_ShapeFilter_Create(
    JoltC_ShapeFilterFn  fn,       /* single-shape query (may be NULL) */
    JoltC_ShapeFilter2Fn fn2,      /* shape-vs-shape query (may be NULL) */
    void*                userData);
JOLTC_API void JoltC_ShapeFilter_Destroy(JoltC_ShapeFilter* filter);

JOLTC_API JoltC_BodyID JoltC_ShapeFilter_GetBodyID2(const JoltC_ShapeFilter* filter);
JOLTC_API void         JoltC_ShapeFilter_SetBodyID2(JoltC_ShapeFilter* filter, JoltC_BodyID bodyID);

/* -------------------------------------------------------------------------- */
/*  SimShapeFilter                                                            */
/* -------------------------------------------------------------------------- */
JOLTC_API JoltC_SimShapeFilter* JoltC_SimShapeFilter_Create(
    JoltC_SimShapeFilterFn fn,
    void*                  userData);
JOLTC_API void JoltC_SimShapeFilter_Destroy(JoltC_SimShapeFilter* filter);

/* -------------------------------------------------------------------------- */
/*  SetProcs — update function pointers on existing filter objects            */
/* -------------------------------------------------------------------------- */
JOLTC_API void JoltC_BroadPhaseLayerFilter_SetProcs(JoltC_BroadPhaseLayerFilter* filter, JoltC_BroadPhaseLayerFilter_Procs procs, void* userData);
JOLTC_API void JoltC_ObjectLayerFilter_SetProcs(JoltC_ObjectLayerFilter* filter, JoltC_ObjectLayerFilter_Procs procs, void* userData);
JOLTC_API void JoltC_BodyFilter_SetProcs(JoltC_BodyFilter* filter, JoltC_BodyFilter_Procs procs, void* userData);
JOLTC_API void JoltC_ShapeFilter_SetProcs(JoltC_ShapeFilter* filter, JoltC_ShapeFilter_Procs procs, void* userData);
JOLTC_API void JoltC_SimShapeFilter_SetProcs(JoltC_SimShapeFilter* filter, JoltC_SimShapeFilter_Procs procs, void* userData);

#ifdef __cplusplus
}
#endif

#endif /* JOLTC_FILTERS_H */
