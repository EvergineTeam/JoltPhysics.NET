/* JoltC - C bindings for JoltPhysics
 * SPDX-License-Identifier: MIT
 *
 * Soft body construction and runtime access.
 */

#ifndef JOLTC_SOFT_BODY_H
#define JOLTC_SOFT_BODY_H

#include <JoltC/common.h>

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */
/*  SoftBodySharedSettings — ref-counted vertex/constraint description        */
/* -------------------------------------------------------------------------- */
JOLTC_API JoltC_SoftBodySharedSettings* JoltC_SoftBodySharedSettings_Create(void);
JOLTC_API void JoltC_SoftBodySharedSettings_AddRef(const JoltC_SoftBodySharedSettings* settings);
JOLTC_API void JoltC_SoftBodySharedSettings_Release(const JoltC_SoftBodySharedSettings* settings);

/* Returns the index of the added vertex. Inverse mass 0 pins the vertex. */
JOLTC_API uint32_t JoltC_SoftBodySharedSettings_AddVertex(JoltC_SoftBodySharedSettings* settings, float x, float y, float z, float invMass);
JOLTC_API void JoltC_SoftBodySharedSettings_AddFace(JoltC_SoftBodySharedSettings* settings, uint32_t vertex0, uint32_t vertex1, uint32_t vertex2, uint32_t materialIndex);

/* Builds edge/shear/bend constraints from the faces added so far. Compliance FLT_MAX disables a constraint kind. */
JOLTC_API void JoltC_SoftBodySharedSettings_CreateConstraints(JoltC_SoftBodySharedSettings* settings, float compliance, float shearCompliance, float bendCompliance, JoltC_SoftBodyBendType bendType);
JOLTC_API void JoltC_SoftBodySharedSettings_CalculateEdgeLengths(JoltC_SoftBodySharedSettings* settings);
JOLTC_API void JoltC_SoftBodySharedSettings_CalculateVolumeConstraintVolumes(JoltC_SoftBodySharedSettings* settings);
JOLTC_API void JoltC_SoftBodySharedSettings_Optimize(JoltC_SoftBodySharedSettings* settings);

/* Factory: solid cube of gridSize^3 vertices with edge and volume constraints plus surface faces. */
JOLTC_API JoltC_SoftBodySharedSettings* JoltC_SoftBodySharedSettings_CreateCube(uint32_t gridSize, float gridSpacing);

JOLTC_API uint32_t JoltC_SoftBodySharedSettings_GetVertexCount(const JoltC_SoftBodySharedSettings* settings);
JOLTC_API uint32_t JoltC_SoftBodySharedSettings_GetFaceCount(const JoltC_SoftBodySharedSettings* settings);
JOLTC_API void JoltC_SoftBodySharedSettings_GetFace(const JoltC_SoftBodySharedSettings* settings, uint32_t index, uint32_t* outVertex0, uint32_t* outVertex1, uint32_t* outVertex2);

/* -------------------------------------------------------------------------- */
/*  SoftBodyCreationSettings — configuration                                  */
/* -------------------------------------------------------------------------- */
/* Takes a reference on sharedSettings; the caller keeps its own. */
JOLTC_API void JoltC_SoftBodyCreationSettings_SetSettings(JoltC_SoftBodyCreationSettings* settings, const JoltC_SoftBodySharedSettings* sharedSettings);
JOLTC_API void JoltC_SoftBodyCreationSettings_SetPosition(JoltC_SoftBodyCreationSettings* settings, JoltC_RVec3 position);
JOLTC_API void JoltC_SoftBodyCreationSettings_SetRotation(JoltC_SoftBodyCreationSettings* settings, JoltC_Quat rotation);
JOLTC_API void JoltC_SoftBodyCreationSettings_SetObjectLayer(JoltC_SoftBodyCreationSettings* settings, JoltC_ObjectLayer objectLayer);
JOLTC_API void JoltC_SoftBodyCreationSettings_SetNumIterations(JoltC_SoftBodyCreationSettings* settings, uint32_t numIterations);
JOLTC_API void JoltC_SoftBodyCreationSettings_SetLinearDamping(JoltC_SoftBodyCreationSettings* settings, float linearDamping);
JOLTC_API void JoltC_SoftBodyCreationSettings_SetMaxLinearVelocity(JoltC_SoftBodyCreationSettings* settings, float maxLinearVelocity);
JOLTC_API void JoltC_SoftBodyCreationSettings_SetRestitution(JoltC_SoftBodyCreationSettings* settings, float restitution);
JOLTC_API void JoltC_SoftBodyCreationSettings_SetFriction(JoltC_SoftBodyCreationSettings* settings, float friction);
JOLTC_API void JoltC_SoftBodyCreationSettings_SetPressure(JoltC_SoftBodyCreationSettings* settings, float pressure);
JOLTC_API void JoltC_SoftBodyCreationSettings_SetGravityFactor(JoltC_SoftBodyCreationSettings* settings, float gravityFactor);
JOLTC_API void JoltC_SoftBodyCreationSettings_SetVertexRadius(JoltC_SoftBodyCreationSettings* settings, float vertexRadius);
JOLTC_API void JoltC_SoftBodyCreationSettings_SetUpdatePosition(JoltC_SoftBodyCreationSettings* settings, JoltC_Bool updatePosition);
JOLTC_API void JoltC_SoftBodyCreationSettings_SetMakeRotationIdentity(JoltC_SoftBodyCreationSettings* settings, JoltC_Bool makeRotationIdentity);
JOLTC_API void JoltC_SoftBodyCreationSettings_SetAllowSleeping(JoltC_SoftBodyCreationSettings* settings, JoltC_Bool allowSleeping);
JOLTC_API void JoltC_SoftBodyCreationSettings_SetUserData(JoltC_SoftBodyCreationSettings* settings, uint64_t userData);

/* -------------------------------------------------------------------------- */
/*  SoftBodyMotionProperties — runtime vertex access                          */
/* -------------------------------------------------------------------------- */
/* Returns null when the body is not a soft body. Valid while the body is alive. */
JOLTC_API JoltC_SoftBodyMotionProperties* JoltC_Body_GetSoftBodyMotionProperties(JoltC_Body* body);

JOLTC_API uint32_t JoltC_SoftBodyMotionProperties_GetVertexCount(const JoltC_SoftBodyMotionProperties* motionProperties);

/* Bulk copy of vertex positions (local space, relative to the body's center of mass).
 * Writes at most capacity entries and returns the number written. */
JOLTC_API uint32_t JoltC_SoftBodyMotionProperties_GetVertexPositions(const JoltC_SoftBodyMotionProperties* motionProperties, JoltC_Vec3* outPositions, uint32_t capacity);
JOLTC_API void JoltC_SoftBodyMotionProperties_GetVertexPosition(const JoltC_SoftBodyMotionProperties* motionProperties, uint32_t index, JoltC_Vec3* outPosition);

JOLTC_API float JoltC_SoftBodyMotionProperties_GetVolume(const JoltC_SoftBodyMotionProperties* motionProperties);
JOLTC_API void JoltC_SoftBodyMotionProperties_GetLocalBounds(const JoltC_SoftBodyMotionProperties* motionProperties, JoltC_Vec3* outMin, JoltC_Vec3* outMax);
JOLTC_API float JoltC_SoftBodyMotionProperties_GetPressure(const JoltC_SoftBodyMotionProperties* motionProperties);
JOLTC_API void JoltC_SoftBodyMotionProperties_SetPressure(JoltC_SoftBodyMotionProperties* motionProperties, float pressure);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* JOLTC_SOFT_BODY_H */
