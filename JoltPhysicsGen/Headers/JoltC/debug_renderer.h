/* JoltC - C bindings for JoltPhysics
 * SPDX-License-Identifier: MIT
 *
 * The debug renderer: Jolt draws what the solver actually holds -- shapes after cooking and
 * quantisation, constraint limits, soft body structure, vehicle wheels -- and hands it to the
 * caller as lines, triangles and text through three callbacks. The caller's renderer decides how
 * they reach a screen, which keeps this agnostic of any graphics API.
 */

#ifndef JOLTC_DEBUG_RENDERER_H
#define JOLTC_DEBUG_RENDERER_H

#include <JoltC/common.h>

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */
/*  Creation                                                                  */
/* -------------------------------------------------------------------------- */
/* drawLine is mandatory. A null drawTriangle falls back to drawing the triangle's three edges as
 * lines; a null drawText3D skips text. Jolt keeps a single global renderer instance, so create
 * exactly one and destroy it after everything that draws. */
JOLTC_API JoltC_DebugRenderer* JoltC_DebugRenderer_Create(
    JoltC_DebugDrawLineFn     drawLine,
    JoltC_DebugDrawTriangleFn drawTriangle,
    JoltC_DebugDrawText3DFn   drawText3D,
    void*                     userData);
JOLTC_API void JoltC_DebugRenderer_Destroy(JoltC_DebugRenderer* renderer);

/* -------------------------------------------------------------------------- */
/*  Collecting instead of calling back                                        */
/* -------------------------------------------------------------------------- */
/* One vertex of a collected line, in the shape a vertex buffer wants: a position and a packed
 * colour, red in the lowest byte. */
typedef struct JoltC_DebugVertex {
    float    x;
    float    y;
    float    z;
    uint32_t color;
} JoltC_DebugVertex;

/* A renderer that appends what it is asked to draw instead of calling back per line. A caller
 * drawing wireframe gizmos crosses the language boundary once per frame rather than once per line,
 * which for a scene of a few hundred bodies is the difference between thousands of calls and a
 * memory block: the callback renderer above spends most of a frame in the boundary alone.
 *
 * Lines are appended as two vertices, triangles as their three edges, and text is dropped: this
 * collects lines, and a caller drawing lines has nothing to do with a glyph. */
JOLTC_API JoltC_DebugRenderer* JoltC_DebugRenderer_CreateCollector(void);

/* Hands over everything collected and empties the buffer for the next frame. Writes the block to
 * outVertices and returns how many vertices it holds, always an even number. The block stays valid
 * until the next drawing into this renderer, which is what refills it. */
JOLTC_API uint32_t JoltC_DebugRenderer_TakeVertices(JoltC_DebugRenderer* renderer, const JoltC_DebugVertex** outVertices);

/* Where the camera is, which is what decides the level of detail Jolt draws a shape at: its
 * geometries carry LODs for five, ten and forty metres, and a renderer that never hears about a
 * camera draws every one of them at the finest level, however far away it is. */
JOLTC_API void JoltC_DebugRenderer_SetCameraPos(JoltC_DebugRenderer* renderer, JoltC_RVec3 position);

/* -------------------------------------------------------------------------- */
/*  What to draw of the bodies                                                */
/* -------------------------------------------------------------------------- */
typedef enum JoltC_ShapeColor {
    JOLTC_SHAPE_COLOR_INSTANCE    = 0, /* random color per instance */
    JOLTC_SHAPE_COLOR_SHAPE_TYPE  = 1,
    JOLTC_SHAPE_COLOR_MOTION_TYPE = 2,
    JOLTC_SHAPE_COLOR_SLEEP       = 3,
    JOLTC_SHAPE_COLOR_ISLAND      = 4,
    JOLTC_SHAPE_COLOR_MATERIAL    = 5
} JoltC_ShapeColor;

/* Mirror of JPH::BodyManager::DrawSettings. Init writes Jolt's defaults (shapes only). */
typedef struct JoltC_BodyDrawSettings {
    JoltC_Bool       drawGetSupportFunction;
    JoltC_Bool       drawSupportDirection;
    JoltC_Bool       drawGetSupportingFace;
    JoltC_Bool       drawShape;
    JoltC_Bool       drawShapeWireframe;
    JoltC_ShapeColor drawShapeColor;
    JoltC_Bool       drawBoundingBox;
    JoltC_Bool       drawCenterOfMassTransform;
    JoltC_Bool       drawWorldTransform;
    JoltC_Bool       drawVelocity;
    JoltC_Bool       drawMassAndInertia;
    JoltC_Bool       drawSleepStats;
    JoltC_Bool       drawSoftBodyVertices;
    JoltC_Bool       drawSoftBodyVertexVelocities;
    JoltC_Bool       drawSoftBodyEdgeConstraints;
    JoltC_Bool       drawSoftBodyBendConstraints;
    JoltC_Bool       drawSoftBodyVolumeConstraints;
    JoltC_Bool       drawSoftBodySkinConstraints;
    JoltC_Bool       drawSoftBodyLRAConstraints;
    JoltC_Bool       drawSoftBodyRods;
    JoltC_Bool       drawSoftBodyRodStates;
    JoltC_Bool       drawSoftBodyRodBendTwistConstraints;
    JoltC_Bool       drawSoftBodyPredictedBounds;
} JoltC_BodyDrawSettings;

JOLTC_API void JoltC_BodyDrawSettings_Init(JoltC_BodyDrawSettings* settings);

/* -------------------------------------------------------------------------- */
/*  Drawing                                                                   */
/* -------------------------------------------------------------------------- */
/* Null settings draw the defaults. Call between updates, never during one. */
JOLTC_API void JoltC_PhysicsSystem_DrawBodies(JoltC_PhysicsSystem* system, const JoltC_BodyDrawSettings* settings, JoltC_DebugRenderer* renderer);
JOLTC_API void JoltC_PhysicsSystem_DrawConstraints(JoltC_PhysicsSystem* system, JoltC_DebugRenderer* renderer);
JOLTC_API void JoltC_PhysicsSystem_DrawConstraintLimits(JoltC_PhysicsSystem* system, JoltC_DebugRenderer* renderer);

/* Shape::Draw. DrawBodies above draws every body where the solver has it, which is the last simulated
 * instant; a caller that renders interpolated poses draws its meshes a fraction of a step behind that,
 * and an outline that does not sit on the mesh it outlines is worse than no outline. This draws one
 * shape wherever the caller says, so the two can be made to agree. It lives with the renderer because
 * Shape::Draw only exists when Jolt is built with its debug renderer.
 *
 * The transform is the centre of mass one, which is the body transform composed with the shape's
 * centre of mass, and it travels by pointer like every other Mat44 here. */
JOLTC_API void JoltC_Shape_Draw(
    const JoltC_Shape*   shape,
    JoltC_DebugRenderer* renderer,
    const JoltC_Mat44*   centerOfMassTransform,
    JoltC_Vec3           scale,
    uint32_t             color,
    JoltC_Bool           useMaterialColors,
    JoltC_Bool           drawWireframe);

/* The palette DrawBodies colours dynamic bodies with, so a caller drawing the shapes itself can keep
 * the colours it had. */
JOLTC_API uint32_t JoltC_Color_GetDistinctColor(uint32_t index);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* JOLTC_DEBUG_RENDERER_H */
