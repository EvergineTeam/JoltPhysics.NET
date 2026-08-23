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

/* Adds a vertex with an initial velocity as well as a position. */
JOLTC_API uint32_t JoltC_SoftBodySharedSettings_AddVertex2(JoltC_SoftBodySharedSettings* settings, float x, float y, float z, float vx, float vy, float vz, float invMass);

/* Builds edge/shear/bend constraints from the faces added so far. Compliance FLT_MAX disables a constraint kind. */
JOLTC_API void JoltC_SoftBodySharedSettings_CreateConstraints(JoltC_SoftBodySharedSettings* settings, float compliance, float shearCompliance, float bendCompliance, JoltC_SoftBodyBendType bendType);

/* The full form: one attribute struct per vertex (when fewer are given the last one repeats, which
 * is Jolt's own convention), so cloth can be stiff at the hem and loose in the middle, and long
 * range attachments can finally be created -- an lraType other than NONE needs at least one pinned
 * vertex to anchor to. angleTolerance (radians) controls when two triangles count as a quad for
 * shear edges; Jolt's default is 8 degrees. */
JOLTC_API void JoltC_SoftBodySharedSettings_CreateConstraints2(
    JoltC_SoftBodySharedSettings*         settings,
    const JoltC_SoftBodyVertexAttributes* vertexAttributes,
    uint32_t                              vertexAttributesCount,
    JoltC_SoftBodyBendType                bendType,
    float                                 angleTolerance);

JOLTC_API void JoltC_SoftBodySharedSettings_CalculateEdgeLengths(JoltC_SoftBodySharedSettings* settings);
JOLTC_API void JoltC_SoftBodySharedSettings_CalculateVolumeConstraintVolumes(JoltC_SoftBodySharedSettings* settings);
JOLTC_API void JoltC_SoftBodySharedSettings_CalculateBendConstraintConstants(JoltC_SoftBodySharedSettings* settings);
JOLTC_API void JoltC_SoftBodySharedSettings_CalculateLRALengths(JoltC_SoftBodySharedSettings* settings, float maxDistanceMultiplier);
JOLTC_API void JoltC_SoftBodySharedSettings_CalculateRodProperties(JoltC_SoftBodySharedSettings* settings);
JOLTC_API void JoltC_SoftBodySharedSettings_CalculateSkinnedConstraintNormals(JoltC_SoftBodySharedSettings* settings);
JOLTC_API void JoltC_SoftBodySharedSettings_Optimize(JoltC_SoftBodySharedSettings* settings);

/* -------------------------------------------------------------------------- */
/*  Direct constraint construction, for bodies not built from faces           */
/* -------------------------------------------------------------------------- */
/* Every Add returns the index of the added constraint in its own list. Rest lengths, volumes,
 * angles and rod frames are left for the matching Calculate function unless noted. */
JOLTC_API uint32_t JoltC_SoftBodySharedSettings_AddEdgeConstraint(JoltC_SoftBodySharedSettings* settings, uint32_t vertex1, uint32_t vertex2, float compliance);
JOLTC_API uint32_t JoltC_SoftBodySharedSettings_AddDihedralBendConstraint(JoltC_SoftBodySharedSettings* settings, uint32_t vertex1, uint32_t vertex2, uint32_t vertex3, uint32_t vertex4, float compliance);
JOLTC_API uint32_t JoltC_SoftBodySharedSettings_AddVolumeConstraint(JoltC_SoftBodySharedSettings* settings, uint32_t vertex1, uint32_t vertex2, uint32_t vertex3, uint32_t vertex4, float compliance);
/* vertex1 should be kinematic (inverse mass zero), vertex2 dynamic. maxDistance 0 leaves the
 * length for CalculateLRALengths. */
JOLTC_API uint32_t JoltC_SoftBodySharedSettings_AddLRAConstraint(JoltC_SoftBodySharedSettings* settings, uint32_t vertex1, uint32_t vertex2, float maxDistance);
/* A Cosserat rod between two vertices: fixed length, carries orientation. Constrain every rod
 * with at least one bend/twist constraint or it will spin freely about its own axis. */
JOLTC_API uint32_t JoltC_SoftBodySharedSettings_AddRodStretchShearConstraint(JoltC_SoftBodySharedSettings* settings, uint32_t vertex1, uint32_t vertex2, float compliance);
/* rod1/rod2 are indices returned by AddRodStretchShearConstraint. */
JOLTC_API uint32_t JoltC_SoftBodySharedSettings_AddRodBendTwistConstraint(JoltC_SoftBodySharedSettings* settings, uint32_t rod1, uint32_t rod2, float compliance);

/* -------------------------------------------------------------------------- */
/*  Skinning: vertices constrained to joints                                  */
/* -------------------------------------------------------------------------- */
/* Registers the inverse bind matrix of a joint and returns its index in the list. */
JOLTC_API uint32_t JoltC_SoftBodySharedSettings_AddInvBindMatrix(JoltC_SoftBodySharedSettings* settings, uint32_t jointIndex, const JoltC_Mat44* invBind);
/* Skins one vertex to up to four inverse bind matrices. Weights are normalized to one. maxDistance
 * 0 skins hard, FLT_MAX disables the distance limit; the back stop is disabled while
 * backStopDistance >= maxDistance. Call CalculateSkinnedConstraintNormals once all skinned
 * constraints and faces exist. */
JOLTC_API uint32_t JoltC_SoftBodySharedSettings_AddSkinnedConstraint(
    JoltC_SoftBodySharedSettings* settings,
    uint32_t                      vertex,
    float                         maxDistance,
    float                         backStopDistance,
    float                         backStopRadius,
    const uint32_t*               invBindIndices,
    const float*                  weights,
    int                           numWeights);

/* Constraint counts, one per list, so a builder can assert what it produced. */
JOLTC_API uint32_t JoltC_SoftBodySharedSettings_GetEdgeConstraintCount(const JoltC_SoftBodySharedSettings* settings);
JOLTC_API uint32_t JoltC_SoftBodySharedSettings_GetDihedralBendConstraintCount(const JoltC_SoftBodySharedSettings* settings);
JOLTC_API uint32_t JoltC_SoftBodySharedSettings_GetVolumeConstraintCount(const JoltC_SoftBodySharedSettings* settings);
JOLTC_API uint32_t JoltC_SoftBodySharedSettings_GetSkinnedConstraintCount(const JoltC_SoftBodySharedSettings* settings);
JOLTC_API uint32_t JoltC_SoftBodySharedSettings_GetLRAConstraintCount(const JoltC_SoftBodySharedSettings* settings);
JOLTC_API uint32_t JoltC_SoftBodySharedSettings_GetRodStretchShearConstraintCount(const JoltC_SoftBodySharedSettings* settings);
JOLTC_API uint32_t JoltC_SoftBodySharedSettings_GetRodBendTwistConstraintCount(const JoltC_SoftBodySharedSettings* settings);

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
/* Both sides of every face collide, for cloth that things hit from either side. */
JOLTC_API void JoltC_SoftBodyCreationSettings_SetFacesDoubleSided(JoltC_SoftBodyCreationSettings* settings, JoltC_Bool facesDoubleSided);
/* Same collision group semantics as rigid bodies: bodies sharing a filter, group id and colliding
 * sub group ids are kept apart or let through by the filter. Takes a reference on the filter. */
JOLTC_API void JoltC_SoftBodyCreationSettings_SetCollisionGroup(JoltC_SoftBodyCreationSettings* settings, JoltC_GroupFilter* filter, uint32_t groupId, uint32_t subGroupId);

/* Read back what the setters above wrote, so a builder can be audited. */
JOLTC_API JoltC_RVec3 JoltC_SoftBodyCreationSettings_GetPosition(const JoltC_SoftBodyCreationSettings* settings);
JOLTC_API JoltC_Quat  JoltC_SoftBodyCreationSettings_GetRotation(const JoltC_SoftBodyCreationSettings* settings);
JOLTC_API JoltC_ObjectLayer JoltC_SoftBodyCreationSettings_GetObjectLayer(const JoltC_SoftBodyCreationSettings* settings);
JOLTC_API uint32_t    JoltC_SoftBodyCreationSettings_GetNumIterations(const JoltC_SoftBodyCreationSettings* settings);
JOLTC_API float       JoltC_SoftBodyCreationSettings_GetLinearDamping(const JoltC_SoftBodyCreationSettings* settings);
JOLTC_API float       JoltC_SoftBodyCreationSettings_GetMaxLinearVelocity(const JoltC_SoftBodyCreationSettings* settings);
JOLTC_API float       JoltC_SoftBodyCreationSettings_GetRestitution(const JoltC_SoftBodyCreationSettings* settings);
JOLTC_API float       JoltC_SoftBodyCreationSettings_GetFriction(const JoltC_SoftBodyCreationSettings* settings);
JOLTC_API float       JoltC_SoftBodyCreationSettings_GetPressure(const JoltC_SoftBodyCreationSettings* settings);
JOLTC_API float       JoltC_SoftBodyCreationSettings_GetGravityFactor(const JoltC_SoftBodyCreationSettings* settings);
JOLTC_API float       JoltC_SoftBodyCreationSettings_GetVertexRadius(const JoltC_SoftBodyCreationSettings* settings);
JOLTC_API JoltC_Bool  JoltC_SoftBodyCreationSettings_GetUpdatePosition(const JoltC_SoftBodyCreationSettings* settings);
JOLTC_API JoltC_Bool  JoltC_SoftBodyCreationSettings_GetMakeRotationIdentity(const JoltC_SoftBodyCreationSettings* settings);
JOLTC_API JoltC_Bool  JoltC_SoftBodyCreationSettings_GetAllowSleeping(const JoltC_SoftBodyCreationSettings* settings);
JOLTC_API uint64_t    JoltC_SoftBodyCreationSettings_GetUserData(const JoltC_SoftBodyCreationSettings* settings);
JOLTC_API JoltC_Bool  JoltC_SoftBodyCreationSettings_GetFacesDoubleSided(const JoltC_SoftBodyCreationSettings* settings);

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

/* -------------------------------------------------------------------------- */
/*  Per-vertex write access                                                   */
/* -------------------------------------------------------------------------- */
/* Positions are local to the body's center of mass, like the readers above. Writing while the
 * simulation steps the body is a race, exactly as in Jolt: write between updates. */
JOLTC_API void  JoltC_SoftBodyMotionProperties_SetVertexPosition(JoltC_SoftBodyMotionProperties* motionProperties, uint32_t index, JoltC_Vec3 position);
JOLTC_API void  JoltC_SoftBodyMotionProperties_GetVertexVelocity(const JoltC_SoftBodyMotionProperties* motionProperties, uint32_t index, JoltC_Vec3* outVelocity);
JOLTC_API void  JoltC_SoftBodyMotionProperties_SetVertexVelocity(JoltC_SoftBodyMotionProperties* motionProperties, uint32_t index, JoltC_Vec3 velocity);
JOLTC_API uint32_t JoltC_SoftBodyMotionProperties_GetVertexVelocities(const JoltC_SoftBodyMotionProperties* motionProperties, JoltC_Vec3* outVelocities, uint32_t capacity);
JOLTC_API float JoltC_SoftBodyMotionProperties_GetVertexInvMass(const JoltC_SoftBodyMotionProperties* motionProperties, uint32_t index);
/* Zero pins the vertex where it is; restore the old inverse mass to release it. Call
 * CalculateMassAndInertia afterwards so the body's aggregate mass follows. */
JOLTC_API void  JoltC_SoftBodyMotionProperties_SetVertexInvMass(JoltC_SoftBodyMotionProperties* motionProperties, uint32_t index, float invMass);
JOLTC_API void  JoltC_SoftBodyMotionProperties_CalculateMassAndInertia(JoltC_SoftBodyMotionProperties* motionProperties);

/* -------------------------------------------------------------------------- */
/*  Runtime configuration                                                     */
/* -------------------------------------------------------------------------- */
JOLTC_API uint32_t   JoltC_SoftBodyMotionProperties_GetNumIterations(const JoltC_SoftBodyMotionProperties* motionProperties);
JOLTC_API void       JoltC_SoftBodyMotionProperties_SetNumIterations(JoltC_SoftBodyMotionProperties* motionProperties, uint32_t numIterations);
JOLTC_API JoltC_Bool JoltC_SoftBodyMotionProperties_GetFacesDoubleSided(const JoltC_SoftBodyMotionProperties* motionProperties);

/* -------------------------------------------------------------------------- */
/*  Skinning at runtime                                                       */
/* -------------------------------------------------------------------------- */
/* Skins the vertices to the given joint matrices (one per joint index used by the inverse bind
 * matrices, in the space of the given center of mass transform). hardSkinAll teleports every
 * vertex to its skinned position, for the first frame after the body appears. */
JOLTC_API void JoltC_SoftBodyMotionProperties_SkinVertices(
    JoltC_SoftBodyMotionProperties* motionProperties,
    const JoltC_Mat44*              centerOfMassTransform,
    const JoltC_Mat44*              jointMatrices,
    uint32_t                        numJoints,
    JoltC_Bool                      hardSkinAll,
    JoltC_TempAllocator*            allocator);
JOLTC_API JoltC_Bool JoltC_SoftBodyMotionProperties_GetEnableSkinConstraints(const JoltC_SoftBodyMotionProperties* motionProperties);
JOLTC_API void       JoltC_SoftBodyMotionProperties_SetEnableSkinConstraints(JoltC_SoftBodyMotionProperties* motionProperties, JoltC_Bool enable);
JOLTC_API float      JoltC_SoftBodyMotionProperties_GetSkinnedMaxDistanceMultiplier(const JoltC_SoftBodyMotionProperties* motionProperties);
JOLTC_API void       JoltC_SoftBodyMotionProperties_SetSkinnedMaxDistanceMultiplier(JoltC_SoftBodyMotionProperties* motionProperties, float multiplier);

/* -------------------------------------------------------------------------- */
/*  Stepping a soft body by hand                                              */
/* -------------------------------------------------------------------------- */
/* Advances one soft body outside PhysicsSystem_Update, for bodies that live outside the broad
 * phase or need their own cadence. The body must be the one these motion properties belong to. */
JOLTC_API void JoltC_SoftBodyMotionProperties_CustomUpdate(
    JoltC_SoftBodyMotionProperties* motionProperties,
    float                           deltaTime,
    JoltC_Body*                     body,
    JoltC_PhysicsSystem*            system);

/* -------------------------------------------------------------------------- */
/*  SoftBodyContactListener — collision callbacks for soft bodies             */
/* -------------------------------------------------------------------------- */
/* The exact twin of the vehicle step listener repair: the callbacks existed in Jolt all along, and
 * without the Set call on the system they were unreachable. The listener must outlive its
 * registration; unregister with a null listener before destroying it. */
JOLTC_API JoltC_SoftBodyContactListener* JoltC_SoftBodyContactListener_Create(
    JoltC_OnSoftBodyContactValidateFn onValidate,
    JoltC_OnSoftBodyContactAddedFn    onAdded,
    void*                             userData);
JOLTC_API void JoltC_SoftBodyContactListener_Destroy(JoltC_SoftBodyContactListener* listener);
JOLTC_API void JoltC_PhysicsSystem_SetSoftBodyContactListener(JoltC_PhysicsSystem* system, JoltC_SoftBodyContactListener* listener);

/* -------------------------------------------------------------------------- */
/*  SoftBodyManifold — reading contacts inside the contact added callback     */
/* -------------------------------------------------------------------------- */
/* The manifold borrows the soft body's internal arrays: every one of these is only valid for the
 * duration of the callback that handed the manifold over. */
JOLTC_API uint32_t   JoltC_SoftBodyManifold_GetVertexCount(const JoltC_SoftBodyManifold* manifold);
JOLTC_API JoltC_Bool JoltC_SoftBodyManifold_HasContact(const JoltC_SoftBodyManifold* manifold, uint32_t vertexIndex);
/* Local to the soft body's center of mass, like the vertex positions. */
JOLTC_API void       JoltC_SoftBodyManifold_GetLocalContactPoint(const JoltC_SoftBodyManifold* manifold, uint32_t vertexIndex, JoltC_Vec3* outPoint);
JOLTC_API void       JoltC_SoftBodyManifold_GetContactNormal(const JoltC_SoftBodyManifold* manifold, uint32_t vertexIndex, JoltC_Vec3* outNormal);
JOLTC_API JoltC_BodyID JoltC_SoftBodyManifold_GetContactBodyID(const JoltC_SoftBodyManifold* manifold, uint32_t vertexIndex);
JOLTC_API uint32_t   JoltC_SoftBodyManifold_GetNumSensorContacts(const JoltC_SoftBodyManifold* manifold);
JOLTC_API JoltC_BodyID JoltC_SoftBodyManifold_GetSensorContactBodyID(const JoltC_SoftBodyManifold* manifold, uint32_t index);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* JOLTC_SOFT_BODY_H */
