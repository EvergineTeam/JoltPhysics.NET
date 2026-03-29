/* JoltC - C bindings for JoltPhysics
 * SPDX-License-Identifier: MIT
 *
 * Shape creation and management.
 */

#ifndef JOLTC_SHAPE_H
#define JOLTC_SHAPE_H

#include <JoltC/common.h>

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */
/*  Shape ref-counting                                                        */
/* -------------------------------------------------------------------------- */
JOLTC_API void JoltC_Shape_AddRef(const JoltC_Shape* shape);
JOLTC_API void JoltC_Shape_Release(const JoltC_Shape* shape);

/* -------------------------------------------------------------------------- */
/*  Shape queries                                                             */
/* -------------------------------------------------------------------------- */
JOLTC_API JoltC_ShapeType    JoltC_Shape_GetType(const JoltC_Shape* shape);
JOLTC_API JoltC_ShapeSubType JoltC_Shape_GetSubType(const JoltC_Shape* shape);
JOLTC_API JoltC_Vec3         JoltC_Shape_GetCenterOfMass(const JoltC_Shape* shape);
JOLTC_API JoltC_AABox        JoltC_Shape_GetLocalBounds(const JoltC_Shape* shape);
JOLTC_API float              JoltC_Shape_GetInnerRadius(const JoltC_Shape* shape);
JOLTC_API float              JoltC_Shape_GetVolume(const JoltC_Shape* shape);

/* -------------------------------------------------------------------------- */
/*  Box shape                                                                 */
/* -------------------------------------------------------------------------- */
JOLTC_API const JoltC_Shape* JoltC_BoxShape_Create(JoltC_Vec3 halfExtent, float convexRadius);

/* -------------------------------------------------------------------------- */
/*  Sphere shape                                                              */
/* -------------------------------------------------------------------------- */
JOLTC_API const JoltC_Shape* JoltC_SphereShape_Create(float radius);

/* -------------------------------------------------------------------------- */
/*  Capsule shape                                                             */
/* -------------------------------------------------------------------------- */
JOLTC_API const JoltC_Shape* JoltC_CapsuleShape_Create(float halfHeightOfCylinder, float radius);

/* -------------------------------------------------------------------------- */
/*  Cylinder shape                                                            */
/* -------------------------------------------------------------------------- */
JOLTC_API const JoltC_Shape* JoltC_CylinderShape_Create(float halfHeight, float radius, float convexRadius);

/* -------------------------------------------------------------------------- */
/*  Tapered capsule shape (via settings, no direct ctor)                      */
/* -------------------------------------------------------------------------- */
JOLTC_API const JoltC_Shape* JoltC_TaperedCapsuleShape_Create(float halfHeight, float topRadius, float bottomRadius);

/* -------------------------------------------------------------------------- */
/*  Convex hull shape                                                         */
/* -------------------------------------------------------------------------- */
JOLTC_API const JoltC_Shape* JoltC_ConvexHullShape_Create(const JoltC_Vec3* points, int numPoints, float maxConvexRadius);

/* -------------------------------------------------------------------------- */
/*  Mesh shape                                                                */
/* -------------------------------------------------------------------------- */
JOLTC_API const JoltC_Shape* JoltC_MeshShape_Create(
    const JoltC_Vec3*            vertices,
    int                          numVertices,
    const JoltC_IndexedTriangle* triangles,
    int                          numTriangles);

/* -------------------------------------------------------------------------- */
/*  HeightField shape                                                         */
/* -------------------------------------------------------------------------- */
JOLTC_API const JoltC_Shape* JoltC_HeightFieldShape_Create(
    const float* samples,
    JoltC_Vec3   offset,
    JoltC_Vec3   scale,
    uint32_t     sampleCount);

/* -------------------------------------------------------------------------- */
/*  StaticCompoundShape                                                       */
/* -------------------------------------------------------------------------- */
typedef struct JoltC_CompoundShapeSubShape {
    JoltC_Vec3         position;
    JoltC_Quat         rotation;
    const JoltC_Shape* shape;
    uint32_t           userData;
} JoltC_CompoundShapeSubShape;

JOLTC_API const JoltC_Shape* JoltC_StaticCompoundShape_Create(
    const JoltC_CompoundShapeSubShape* subShapes,
    int                                numSubShapes);

/* -------------------------------------------------------------------------- */
/*  MutableCompoundShape                                                      */
/* -------------------------------------------------------------------------- */
JOLTC_API const JoltC_Shape* JoltC_MutableCompoundShape_Create(
    const JoltC_CompoundShapeSubShape* subShapes,
    int                                numSubShapes);

/* -------------------------------------------------------------------------- */
/*  Decorated shapes                                                          */
/* -------------------------------------------------------------------------- */
JOLTC_API const JoltC_Shape* JoltC_RotatedTranslatedShape_Create(JoltC_Vec3 position, JoltC_Quat rotation, const JoltC_Shape* innerShape);
JOLTC_API const JoltC_Shape* JoltC_ScaledShape_Create(const JoltC_Shape* innerShape, JoltC_Vec3 scale);
JOLTC_API const JoltC_Shape* JoltC_OffsetCenterOfMassShape_Create(const JoltC_Shape* innerShape, JoltC_Vec3 offset);

/* -------------------------------------------------------------------------- */
/*  CompoundShape accessors                                                   */
/* -------------------------------------------------------------------------- */
JOLTC_API int              JoltC_CompoundShape_GetNumSubShapes(const JoltC_Shape* shape);
JOLTC_API const JoltC_Shape* JoltC_CompoundShape_GetSubShape(const JoltC_Shape* shape, int index);
JOLTC_API uint32_t         JoltC_CompoundShape_GetSubShapeUserData(const JoltC_Shape* shape, int index);
JOLTC_API JoltC_Vec3       JoltC_CompoundShape_GetSubShapePosition(const JoltC_Shape* shape, int index);
JOLTC_API JoltC_Quat       JoltC_CompoundShape_GetSubShapeRotation(const JoltC_Shape* shape, int index);
JOLTC_API uint32_t         JoltC_CompoundShape_GetSubShapeIndexFromID(const JoltC_Shape* shape, uint32_t subShapeId, uint32_t* outRemainder);

/* -------------------------------------------------------------------------- */
/*  DecoratedShape accessor                                                   */
/* -------------------------------------------------------------------------- */
JOLTC_API const JoltC_Shape* JoltC_DecoratedShape_GetInnerShape(const JoltC_Shape* shape);

/* -------------------------------------------------------------------------- */
/*  Shape user data                                                           */
/* -------------------------------------------------------------------------- */
JOLTC_API void     JoltC_Shape_SetUserData(const JoltC_Shape* shape, uint64_t userData);
JOLTC_API uint64_t JoltC_Shape_GetUserData(const JoltC_Shape* shape);

/* -------------------------------------------------------------------------- */
/*  Extended Shape queries                                                    */
/* -------------------------------------------------------------------------- */
JOLTC_API int      JoltC_Shape_MustBeStatic(const JoltC_Shape* shape);
JOLTC_API uint32_t JoltC_Shape_GetSubShapeIDBitsRecursive(const JoltC_Shape* shape);
JOLTC_API void     JoltC_Shape_GetMassProperties(const JoltC_Shape* shape, float* outMass, JoltC_Mat44* outInertia);
JOLTC_API int      JoltC_Shape_CastRay(const JoltC_Shape* shape, JoltC_Vec3 origin, JoltC_Vec3 direction, float* outFraction);
JOLTC_API int      JoltC_Shape_CollidePoint(const JoltC_Shape* shape, JoltC_Vec3 point);

/* -------------------------------------------------------------------------- */
/*  ConvexShape density                                                       */
/* -------------------------------------------------------------------------- */
JOLTC_API float JoltC_ConvexShape_GetDensity(const JoltC_Shape* shape);
JOLTC_API void  JoltC_ConvexShape_SetDensity(const JoltC_Shape* shape, float density);

/* -------------------------------------------------------------------------- */
/*  Box shape getters                                                         */
/* -------------------------------------------------------------------------- */
JOLTC_API JoltC_Vec3 JoltC_BoxShape_GetHalfExtent(const JoltC_Shape* shape);
JOLTC_API float      JoltC_BoxShape_GetConvexRadius(const JoltC_Shape* shape);

/* -------------------------------------------------------------------------- */
/*  Sphere shape getters                                                      */
/* -------------------------------------------------------------------------- */
JOLTC_API float JoltC_SphereShape_GetRadius(const JoltC_Shape* shape);

/* -------------------------------------------------------------------------- */
/*  Capsule shape getters                                                     */
/* -------------------------------------------------------------------------- */
JOLTC_API float JoltC_CapsuleShape_GetRadius(const JoltC_Shape* shape);
JOLTC_API float JoltC_CapsuleShape_GetHalfHeightOfCylinder(const JoltC_Shape* shape);

/* -------------------------------------------------------------------------- */
/*  Cylinder shape getters                                                    */
/* -------------------------------------------------------------------------- */
JOLTC_API float JoltC_CylinderShape_GetRadius(const JoltC_Shape* shape);
JOLTC_API float JoltC_CylinderShape_GetHalfHeight(const JoltC_Shape* shape);

/* -------------------------------------------------------------------------- */
/*  TaperedCapsule shape getters                                              */
/* -------------------------------------------------------------------------- */
JOLTC_API float JoltC_TaperedCapsuleShape_GetTopRadius(const JoltC_Shape* shape);
JOLTC_API float JoltC_TaperedCapsuleShape_GetBottomRadius(const JoltC_Shape* shape);
JOLTC_API float JoltC_TaperedCapsuleShape_GetHalfHeight(const JoltC_Shape* shape);

/* -------------------------------------------------------------------------- */
/*  ConvexHull shape getters                                                  */
/* -------------------------------------------------------------------------- */
JOLTC_API uint32_t   JoltC_ConvexHullShape_GetNumPoints(const JoltC_Shape* shape);
JOLTC_API JoltC_Vec3 JoltC_ConvexHullShape_GetPoint(const JoltC_Shape* shape, uint32_t index);
JOLTC_API uint32_t   JoltC_ConvexHullShape_GetNumFaces(const JoltC_Shape* shape);
JOLTC_API uint32_t   JoltC_ConvexHullShape_GetNumVerticesInFace(const JoltC_Shape* shape, uint32_t faceIndex);
JOLTC_API uint32_t   JoltC_ConvexHullShape_GetFaceVertices(const JoltC_Shape* shape, uint32_t faceIndex, uint32_t maxVertices, uint32_t* vertices);

/* -------------------------------------------------------------------------- */
/*  RotatedTranslatedShape getters                                            */
/* -------------------------------------------------------------------------- */
JOLTC_API JoltC_Vec3 JoltC_RotatedTranslatedShape_GetPosition(const JoltC_Shape* shape);
JOLTC_API JoltC_Quat JoltC_RotatedTranslatedShape_GetRotation(const JoltC_Shape* shape);

/* -------------------------------------------------------------------------- */
/*  ScaledShape getters                                                       */
/* -------------------------------------------------------------------------- */
JOLTC_API JoltC_Vec3 JoltC_ScaledShape_GetScale(const JoltC_Shape* shape);

/* -------------------------------------------------------------------------- */
/*  OffsetCenterOfMassShape getters                                           */
/* -------------------------------------------------------------------------- */
JOLTC_API JoltC_Vec3 JoltC_OffsetCenterOfMassShape_GetOffset(const JoltC_Shape* shape);

/* -------------------------------------------------------------------------- */
/*  MutableCompoundShape modification                                         */
/* -------------------------------------------------------------------------- */
JOLTC_API uint32_t JoltC_MutableCompoundShape_AddShape(const JoltC_Shape* compound, JoltC_Vec3 position, JoltC_Quat rotation, const JoltC_Shape* child, uint32_t userData);
JOLTC_API void     JoltC_MutableCompoundShape_RemoveShape(const JoltC_Shape* compound, uint32_t index);
JOLTC_API void     JoltC_MutableCompoundShape_ModifyShape(const JoltC_Shape* compound, uint32_t index, JoltC_Vec3 position, JoltC_Quat rotation);
JOLTC_API void     JoltC_MutableCompoundShape_ModifyShapeWithShape(const JoltC_Shape* compound, uint32_t index, JoltC_Vec3 position, JoltC_Quat rotation, const JoltC_Shape* newShape);
JOLTC_API void     JoltC_MutableCompoundShape_AdjustCenterOfMass(const JoltC_Shape* compound);
JOLTC_API void     JoltC_MutableCompoundShape_ModifyShape2(JoltC_Shape* shape, uint32_t subShapeIndex, JoltC_Vec3 position, JoltC_Quat rotation, const JoltC_Shape* newShape);

/* -------------------------------------------------------------------------- */
/*  Triangle shape                                                            */
/* -------------------------------------------------------------------------- */
JOLTC_API const JoltC_Shape* JoltC_TriangleShape_Create(JoltC_Vec3 v1, JoltC_Vec3 v2, JoltC_Vec3 v3, float convexRadius);

/* -------------------------------------------------------------------------- */
/*  Plane shape                                                               */
/* -------------------------------------------------------------------------- */
JOLTC_API const JoltC_Shape* JoltC_PlaneShape_Create(JoltC_Vec3 normal, float distance, float halfExtent);

/* -------------------------------------------------------------------------- */
/*  Empty shape                                                               */
/* -------------------------------------------------------------------------- */
JOLTC_API const JoltC_Shape* JoltC_EmptyShape_Create(JoltC_Vec3 centerOfMass);

/* -------------------------------------------------------------------------- */
/*  HeightFieldShapeSettings (opaque handle)                                  */
/* -------------------------------------------------------------------------- */
JOLTC_API JoltC_HeightFieldShapeSettings* JoltC_HeightFieldShapeSettings_Create(
    const float* samples, JoltC_Vec3 offset, JoltC_Vec3 scale, uint32_t sampleCount);
JOLTC_API void     JoltC_HeightFieldShapeSettings_Destroy(JoltC_HeightFieldShapeSettings* settings);
JOLTC_API JoltC_Vec3 JoltC_HeightFieldShapeSettings_GetOffset(const JoltC_HeightFieldShapeSettings* settings);
JOLTC_API void     JoltC_HeightFieldShapeSettings_SetOffset(JoltC_HeightFieldShapeSettings* settings, JoltC_Vec3 value);
JOLTC_API JoltC_Vec3 JoltC_HeightFieldShapeSettings_GetScale(const JoltC_HeightFieldShapeSettings* settings);
JOLTC_API void     JoltC_HeightFieldShapeSettings_SetScale(JoltC_HeightFieldShapeSettings* settings, JoltC_Vec3 value);
JOLTC_API uint32_t JoltC_HeightFieldShapeSettings_GetSampleCount(const JoltC_HeightFieldShapeSettings* settings);
JOLTC_API float    JoltC_HeightFieldShapeSettings_GetMinHeightValue(const JoltC_HeightFieldShapeSettings* settings);
JOLTC_API void     JoltC_HeightFieldShapeSettings_SetMinHeightValue(JoltC_HeightFieldShapeSettings* settings, float value);
JOLTC_API float    JoltC_HeightFieldShapeSettings_GetMaxHeightValue(const JoltC_HeightFieldShapeSettings* settings);
JOLTC_API void     JoltC_HeightFieldShapeSettings_SetMaxHeightValue(JoltC_HeightFieldShapeSettings* settings, float value);
JOLTC_API uint32_t JoltC_HeightFieldShapeSettings_GetBlockSize(const JoltC_HeightFieldShapeSettings* settings);
JOLTC_API void     JoltC_HeightFieldShapeSettings_SetBlockSize(JoltC_HeightFieldShapeSettings* settings, uint32_t value);
JOLTC_API uint32_t JoltC_HeightFieldShapeSettings_GetBitsPerSample(const JoltC_HeightFieldShapeSettings* settings);
JOLTC_API void     JoltC_HeightFieldShapeSettings_SetBitsPerSample(JoltC_HeightFieldShapeSettings* settings, uint32_t value);
JOLTC_API const JoltC_Shape* JoltC_HeightFieldShapeSettings_CreateShape(JoltC_HeightFieldShapeSettings* settings);

/* -------------------------------------------------------------------------- */
/*  HeightFieldShape instance queries                                         */
/* -------------------------------------------------------------------------- */
JOLTC_API uint32_t JoltC_HeightFieldShape_GetSampleCount(const JoltC_Shape* shape);
JOLTC_API uint32_t JoltC_HeightFieldShape_GetBlockSize(const JoltC_Shape* shape);
JOLTC_API JoltC_Vec3 JoltC_HeightFieldShape_GetPosition(const JoltC_Shape* shape, uint32_t x, uint32_t y);
JOLTC_API JoltC_Bool JoltC_HeightFieldShape_IsNoCollision(const JoltC_Shape* shape, uint32_t x, uint32_t y);
JOLTC_API float      JoltC_HeightFieldShape_GetMinHeightValue(const JoltC_Shape* shape);
JOLTC_API float      JoltC_HeightFieldShape_GetMaxHeightValue(const JoltC_Shape* shape);
JOLTC_API int        JoltC_HeightFieldShape_ProjectOntoSurface(const JoltC_Shape* shape, JoltC_Vec3 localPosition, JoltC_Vec3* outSurfacePosition, uint32_t* outSubShapeId);

/* -------------------------------------------------------------------------- */
/*  HeightFieldShapeSettings — generic (via JoltC_ShapeSettings*)             */
/* -------------------------------------------------------------------------- */
JOLTC_API float    JoltC_HeightFieldShapeSettings_GetActiveEdgeCosThresholdAngle(const JoltC_ShapeSettings* settings);
JOLTC_API void     JoltC_HeightFieldShapeSettings_SetActiveEdgeCosThresholdAngle(JoltC_ShapeSettings* settings, float angle);
JOLTC_API void     JoltC_HeightFieldShapeSettings_SetSampleCount(JoltC_ShapeSettings* settings, uint32_t count);
JOLTC_API uint8_t  JoltC_HeightFieldShapeSettings_CalculateBitsPerSampleForError(JoltC_ShapeSettings* settings, float maxError);
JOLTC_API void     JoltC_HeightFieldShapeSettings_DetermineMinAndMaxSample(JoltC_ShapeSettings* settings, float* outMin, float* outMax, float* outQuantizationScale);

/* -------------------------------------------------------------------------- */
/*  MeshShapeSettings (opaque handle)                                         */
/* -------------------------------------------------------------------------- */
JOLTC_API JoltC_MeshShapeSettings* JoltC_MeshShapeSettings_Create(const JoltC_Triangle* triangles, uint32_t triangleCount);
JOLTC_API JoltC_MeshShapeSettings* JoltC_MeshShapeSettings_Create2(
    const JoltC_Vec3* vertices, uint32_t vertexCount,
    const JoltC_IndexedTriangle* triangles, uint32_t triangleCount);
JOLTC_API void     JoltC_MeshShapeSettings_Destroy(JoltC_MeshShapeSettings* settings);
JOLTC_API uint32_t JoltC_MeshShapeSettings_GetMaxTrianglesPerLeaf(const JoltC_MeshShapeSettings* settings);
JOLTC_API void     JoltC_MeshShapeSettings_SetMaxTrianglesPerLeaf(JoltC_MeshShapeSettings* settings, uint32_t value);
JOLTC_API void     JoltC_MeshShapeSettings_Sanitize(JoltC_MeshShapeSettings* settings);
JOLTC_API const JoltC_Shape* JoltC_MeshShapeSettings_CreateShape(const JoltC_MeshShapeSettings* settings);

/* -------------------------------------------------------------------------- */
/*  MeshShapeSettings — generic (via JoltC_ShapeSettings*)                    */
/* -------------------------------------------------------------------------- */
JOLTC_API float    JoltC_MeshShapeSettings_GetActiveEdgeCosThresholdAngle(const JoltC_ShapeSettings* settings);
JOLTC_API void     JoltC_MeshShapeSettings_SetActiveEdgeCosThresholdAngle(JoltC_ShapeSettings* settings, float angle);
JOLTC_API int      JoltC_MeshShapeSettings_GetPerTriangleUserData(const JoltC_ShapeSettings* settings);
JOLTC_API void     JoltC_MeshShapeSettings_SetPerTriangleUserData(JoltC_ShapeSettings* settings, int perTriangleUserData);
JOLTC_API int      JoltC_MeshShapeSettings_GetBuildQuality(const JoltC_ShapeSettings* settings);
JOLTC_API void     JoltC_MeshShapeSettings_SetBuildQuality(JoltC_ShapeSettings* settings, int quality);

/* -------------------------------------------------------------------------- */
/*  MeshShape — instance queries                                              */
/* -------------------------------------------------------------------------- */
JOLTC_API uint32_t JoltC_MeshShape_GetTriangleUserData(const JoltC_Shape* shape, uint32_t subShapeId);

/* -------------------------------------------------------------------------- */
/*  ConvexHullShapeSettings (opaque handle)                                   */
/* -------------------------------------------------------------------------- */
JOLTC_API JoltC_ConvexHullShapeSettings* JoltC_ConvexHullShapeSettings_Create(
    const JoltC_Vec3* points, uint32_t pointCount, float maxConvexRadius);
JOLTC_API void     JoltC_ConvexHullShapeSettings_Destroy(JoltC_ConvexHullShapeSettings* settings);
JOLTC_API const JoltC_Shape* JoltC_ConvexHullShapeSettings_CreateShape(const JoltC_ConvexHullShapeSettings* settings);

/* -------------------------------------------------------------------------- */
/*  ShapeSettings base                                                        */
/* -------------------------------------------------------------------------- */
JOLTC_API void     JoltC_ShapeSettings_Destroy(JoltC_ShapeSettings* settings);
JOLTC_API uint64_t JoltC_ShapeSettings_GetUserData(const JoltC_ShapeSettings* settings);
JOLTC_API void     JoltC_ShapeSettings_SetUserData(JoltC_ShapeSettings* settings, uint64_t userData);

/* -------------------------------------------------------------------------- */
/*  Shape_Destroy                                                             */
/* -------------------------------------------------------------------------- */
JOLTC_API void JoltC_Shape_Destroy(const JoltC_Shape* shape);

/* -------------------------------------------------------------------------- */
/*  BoxShapeSettings                                                          */
/* -------------------------------------------------------------------------- */
JOLTC_API JoltC_ShapeSettings* JoltC_BoxShapeSettings_Create(JoltC_Vec3 halfExtent, float convexRadius);
JOLTC_API const JoltC_Shape*   JoltC_BoxShapeSettings_CreateShape(JoltC_Vec3 halfExtent, float convexRadius);

/* -------------------------------------------------------------------------- */
/*  SphereShapeSettings                                                       */
/* -------------------------------------------------------------------------- */
JOLTC_API JoltC_ShapeSettings* JoltC_SphereShapeSettings_Create(float radius);
JOLTC_API const JoltC_Shape*   JoltC_SphereShapeSettings_CreateShape(float radius);
JOLTC_API float                JoltC_SphereShapeSettings_GetRadius(const JoltC_ShapeSettings* settings);
JOLTC_API void                 JoltC_SphereShapeSettings_SetRadius(JoltC_ShapeSettings* settings, float radius);

/* -------------------------------------------------------------------------- */
/*  CapsuleShapeSettings                                                      */
/* -------------------------------------------------------------------------- */
JOLTC_API JoltC_ShapeSettings* JoltC_CapsuleShapeSettings_Create(float halfHeight, float radius);
JOLTC_API const JoltC_Shape*   JoltC_CapsuleShapeSettings_CreateShape(float halfHeight, float radius);

/* -------------------------------------------------------------------------- */
/*  CylinderShapeSettings                                                     */
/* -------------------------------------------------------------------------- */
JOLTC_API JoltC_ShapeSettings* JoltC_CylinderShapeSettings_Create(float halfHeight, float radius, float convexRadius);
JOLTC_API const JoltC_Shape*   JoltC_CylinderShapeSettings_CreateShape(float halfHeight, float radius, float convexRadius);

/* -------------------------------------------------------------------------- */
/*  TaperedCapsuleShapeSettings                                               */
/* -------------------------------------------------------------------------- */
JOLTC_API JoltC_ShapeSettings* JoltC_TaperedCapsuleShapeSettings_Create(float halfHeight, float topRadius, float bottomRadius);
JOLTC_API const JoltC_Shape*   JoltC_TaperedCapsuleShapeSettings_CreateShape(float halfHeight, float topRadius, float bottomRadius);

/* -------------------------------------------------------------------------- */
/*  TaperedCylinderShapeSettings                                              */
/* -------------------------------------------------------------------------- */
JOLTC_API JoltC_ShapeSettings* JoltC_TaperedCylinderShapeSettings_Create(float halfHeight, float topRadius, float bottomRadius, float convexRadius);
JOLTC_API const JoltC_Shape*   JoltC_TaperedCylinderShapeSettings_CreateShape(float halfHeight, float topRadius, float bottomRadius, float convexRadius);

/* -------------------------------------------------------------------------- */
/*  TriangleShapeSettings                                                     */
/* -------------------------------------------------------------------------- */
JOLTC_API JoltC_ShapeSettings* JoltC_TriangleShapeSettings_Create(JoltC_Vec3 v1, JoltC_Vec3 v2, JoltC_Vec3 v3, float convexRadius);
JOLTC_API const JoltC_Shape*   JoltC_TriangleShapeSettings_CreateShape(JoltC_Vec3 v1, JoltC_Vec3 v2, JoltC_Vec3 v3, float convexRadius);

/* -------------------------------------------------------------------------- */
/*  PlaneShapeSettings                                                        */
/* -------------------------------------------------------------------------- */
JOLTC_API JoltC_ShapeSettings* JoltC_PlaneShapeSettings_Create(JoltC_Vec3 normal, float distance, float halfExtent);
JOLTC_API const JoltC_Shape*   JoltC_PlaneShapeSettings_CreateShape(JoltC_Vec3 normal, float distance, float halfExtent);

/* -------------------------------------------------------------------------- */
/*  EmptyShapeSettings                                                        */
/* -------------------------------------------------------------------------- */
JOLTC_API JoltC_ShapeSettings* JoltC_EmptyShapeSettings_Create(JoltC_Vec3 centerOfMass);
JOLTC_API const JoltC_Shape*   JoltC_EmptyShapeSettings_CreateShape(JoltC_Vec3 centerOfMass);

/* -------------------------------------------------------------------------- */
/*  RotatedTranslatedShapeSettings                                            */
/* -------------------------------------------------------------------------- */
JOLTC_API JoltC_ShapeSettings* JoltC_RotatedTranslatedShapeSettings_Create(JoltC_Vec3 position, JoltC_Quat rotation, const JoltC_ShapeSettings* innerSettings);
JOLTC_API JoltC_ShapeSettings* JoltC_RotatedTranslatedShapeSettings_Create2(JoltC_Vec3 position, JoltC_Quat rotation, const JoltC_Shape* innerShape);
JOLTC_API const JoltC_Shape*   JoltC_RotatedTranslatedShapeSettings_CreateShape(JoltC_Vec3 position, JoltC_Quat rotation, const JoltC_Shape* innerShape);

/* -------------------------------------------------------------------------- */
/*  ScaledShapeSettings                                                       */
/* -------------------------------------------------------------------------- */
JOLTC_API JoltC_ShapeSettings* JoltC_ScaledShapeSettings_Create(const JoltC_ShapeSettings* innerSettings, JoltC_Vec3 scale);
JOLTC_API JoltC_ShapeSettings* JoltC_ScaledShapeSettings_Create2(const JoltC_Shape* innerShape, JoltC_Vec3 scale);
JOLTC_API const JoltC_Shape*   JoltC_ScaledShapeSettings_CreateShape(const JoltC_Shape* innerShape, JoltC_Vec3 scale);

/* -------------------------------------------------------------------------- */
/*  OffsetCenterOfMassShapeSettings                                           */
/* -------------------------------------------------------------------------- */
JOLTC_API JoltC_ShapeSettings* JoltC_OffsetCenterOfMassShapeSettings_Create(JoltC_Vec3 offset, const JoltC_ShapeSettings* innerSettings);
JOLTC_API JoltC_ShapeSettings* JoltC_OffsetCenterOfMassShapeSettings_Create2(JoltC_Vec3 offset, const JoltC_Shape* innerShape);
JOLTC_API const JoltC_Shape*   JoltC_OffsetCenterOfMassShapeSettings_CreateShape(JoltC_Vec3 offset, const JoltC_Shape* innerShape);

/* -------------------------------------------------------------------------- */
/*  StaticCompoundShapeSettings / MutableCompoundShapeSettings                */
/* -------------------------------------------------------------------------- */
JOLTC_API JoltC_ShapeSettings* JoltC_StaticCompoundShapeSettings_Create(void);
JOLTC_API JoltC_ShapeSettings* JoltC_MutableCompoundShapeSettings_Create(void);

/* -------------------------------------------------------------------------- */
/*  CompoundShapeSettings_AddShape                                            */
/* -------------------------------------------------------------------------- */
JOLTC_API void JoltC_CompoundShapeSettings_AddShape(JoltC_ShapeSettings* settings, JoltC_Vec3 position, JoltC_Quat rotation, const JoltC_ShapeSettings* shape, uint32_t userData);
JOLTC_API void JoltC_CompoundShapeSettings_AddShape2(JoltC_ShapeSettings* settings, JoltC_Vec3 position, JoltC_Quat rotation, const JoltC_Shape* shape, uint32_t userData);

/* -------------------------------------------------------------------------- */
/*  ConvexShapeSettings density                                               */
/* -------------------------------------------------------------------------- */
JOLTC_API float JoltC_ConvexShapeSettings_GetDensity(const JoltC_ShapeSettings* settings);
JOLTC_API void  JoltC_ConvexShapeSettings_SetDensity(JoltC_ShapeSettings* settings, float density);

/* -------------------------------------------------------------------------- */
/*  TriangleShape getters                                                     */
/* -------------------------------------------------------------------------- */
JOLTC_API float      JoltC_TriangleShape_GetConvexRadius(const JoltC_Shape* shape);
JOLTC_API JoltC_Vec3 JoltC_TriangleShape_GetVertex1(const JoltC_Shape* shape);
JOLTC_API JoltC_Vec3 JoltC_TriangleShape_GetVertex2(const JoltC_Shape* shape);
JOLTC_API JoltC_Vec3 JoltC_TriangleShape_GetVertex3(const JoltC_Shape* shape);

/* -------------------------------------------------------------------------- */
/*  TaperedCylinderShape getters                                              */
/* -------------------------------------------------------------------------- */
JOLTC_API float JoltC_TaperedCylinderShape_GetHalfHeight(const JoltC_Shape* shape);
JOLTC_API float JoltC_TaperedCylinderShape_GetTopRadius(const JoltC_Shape* shape);
JOLTC_API float JoltC_TaperedCylinderShape_GetBottomRadius(const JoltC_Shape* shape);
JOLTC_API float JoltC_TaperedCylinderShape_GetConvexRadius(const JoltC_Shape* shape);

/* -------------------------------------------------------------------------- */
/*  PlaneShape getters                                                        */
/* -------------------------------------------------------------------------- */
JOLTC_API float      JoltC_PlaneShape_GetHalfExtent(const JoltC_Shape* shape);
JOLTC_API JoltC_Vec3 JoltC_PlaneShape_GetPlane(const JoltC_Shape* shape, float* outDistance);

/* -------------------------------------------------------------------------- */
/*  Shape base — additional functions                                         */
/* -------------------------------------------------------------------------- */
JOLTC_API JoltC_Vec3         JoltC_Shape_GetSurfaceNormal(const JoltC_Shape* shape, uint32_t subShapeId, JoltC_Vec3 localSurfacePosition);
JOLTC_API void               JoltC_Shape_GetWorldSpaceBounds(const JoltC_Shape* shape, JoltC_Mat44 centerOfMassTransform, JoltC_Vec3 scale, JoltC_Vec3* outMin, JoltC_Vec3* outMax);
JOLTC_API int                JoltC_Shape_IsValidScale(const JoltC_Shape* shape, JoltC_Vec3 scale);
JOLTC_API JoltC_Vec3         JoltC_Shape_MakeScaleValid(const JoltC_Shape* shape, JoltC_Vec3 scale);
JOLTC_API const JoltC_Shape* JoltC_Shape_ScaleShape(const JoltC_Shape* shape, JoltC_Vec3 scale);

/* -------------------------------------------------------------------------- */
/*  CollideShapeSettings / ShapeCastSettings init helpers                     */
/* -------------------------------------------------------------------------- */
JOLTC_API void JoltC_CollideShapeSettings_Init(JoltC_CollideShapeSettings* settings);
JOLTC_API void JoltC_ShapeCastSettings_Init(JoltC_ShapeCastSettings* settings);

/* -------------------------------------------------------------------------- */
/*  CollideShapeResult / CollisionEstimationResult free helpers               */
/* -------------------------------------------------------------------------- */
JOLTC_API void JoltC_CollideShapeResult_FreeMembers(JoltC_CollideShapeResult* result);
JOLTC_API void JoltC_CollisionEstimationResult_FreeMembers(JoltC_CollisionEstimationResult* result);

#ifdef __cplusplus
}
#endif

#endif /* JOLTC_SHAPE_H */
