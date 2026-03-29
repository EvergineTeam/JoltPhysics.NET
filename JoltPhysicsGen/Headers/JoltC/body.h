/* JoltC - C bindings for JoltPhysics
 * SPDX-License-Identifier: MIT
 *
 * Body management through BodyInterface.
 */

#ifndef JOLTC_BODY_H
#define JOLTC_BODY_H

#include <JoltC/common.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*  BodyCreationSettings — opaque-handle API                                  */
/* ========================================================================== */
JOLTC_API JoltC_BodyCreationSettings* JoltC_BodyCreationSettings_Create(void);
JOLTC_API JoltC_BodyCreationSettings* JoltC_BodyCreationSettings_Create2(const JoltC_ShapeSettings* shapeSettings, JoltC_RVec3 position, JoltC_Quat rotation, JoltC_MotionType motionType, JoltC_ObjectLayer objectLayer);
JOLTC_API JoltC_BodyCreationSettings* JoltC_BodyCreationSettings_Create3(const JoltC_Shape* shape, JoltC_RVec3 position, JoltC_Quat rotation, JoltC_MotionType motionType, JoltC_ObjectLayer objectLayer);
JOLTC_API void JoltC_BodyCreationSettings_Destroy(JoltC_BodyCreationSettings* s);

/* Position / Rotation */
JOLTC_API void           JoltC_BodyCreationSettings_GetPosition(const JoltC_BodyCreationSettings* s, JoltC_RVec3* result);
JOLTC_API void           JoltC_BodyCreationSettings_SetPosition(JoltC_BodyCreationSettings* s, const JoltC_RVec3* value);
JOLTC_API void           JoltC_BodyCreationSettings_GetRotation(const JoltC_BodyCreationSettings* s, JoltC_Quat* result);
JOLTC_API void           JoltC_BodyCreationSettings_SetRotation(JoltC_BodyCreationSettings* s, const JoltC_Quat* value);

/* Velocities */
JOLTC_API void           JoltC_BodyCreationSettings_GetLinearVelocity(const JoltC_BodyCreationSettings* s, JoltC_Vec3* result);
JOLTC_API void           JoltC_BodyCreationSettings_SetLinearVelocity(JoltC_BodyCreationSettings* s, const JoltC_Vec3* value);
JOLTC_API void           JoltC_BodyCreationSettings_GetAngularVelocity(const JoltC_BodyCreationSettings* s, JoltC_Vec3* result);
JOLTC_API void           JoltC_BodyCreationSettings_SetAngularVelocity(JoltC_BodyCreationSettings* s, const JoltC_Vec3* value);

/* User data */
JOLTC_API uint64_t       JoltC_BodyCreationSettings_GetUserData(const JoltC_BodyCreationSettings* s);
JOLTC_API void           JoltC_BodyCreationSettings_SetUserData(JoltC_BodyCreationSettings* s, uint64_t value);

/* Layer / Group */
JOLTC_API JoltC_ObjectLayer JoltC_BodyCreationSettings_GetObjectLayer(const JoltC_BodyCreationSettings* s);
JOLTC_API void           JoltC_BodyCreationSettings_SetObjectLayer(JoltC_BodyCreationSettings* s, JoltC_ObjectLayer value);
JOLTC_API void           JoltC_BodyCreationSettings_GetCollisionGroup(const JoltC_BodyCreationSettings* s, JoltC_CollisionGroup* result);
JOLTC_API void           JoltC_BodyCreationSettings_SetCollisionGroup(JoltC_BodyCreationSettings* s, const JoltC_CollisionGroup* value);

/* Motion / DOFs */
JOLTC_API JoltC_MotionType JoltC_BodyCreationSettings_GetMotionType(const JoltC_BodyCreationSettings* s);
JOLTC_API void           JoltC_BodyCreationSettings_SetMotionType(JoltC_BodyCreationSettings* s, JoltC_MotionType value);
JOLTC_API JoltC_AllowedDOFs JoltC_BodyCreationSettings_GetAllowedDOFs(const JoltC_BodyCreationSettings* s);
JOLTC_API void           JoltC_BodyCreationSettings_SetAllowedDOFs(JoltC_BodyCreationSettings* s, JoltC_AllowedDOFs value);

/* Bool flags */
JOLTC_API JoltC_Bool     JoltC_BodyCreationSettings_GetAllowDynamicOrKinematic(const JoltC_BodyCreationSettings* s);
JOLTC_API void           JoltC_BodyCreationSettings_SetAllowDynamicOrKinematic(JoltC_BodyCreationSettings* s, JoltC_Bool value);
JOLTC_API JoltC_Bool     JoltC_BodyCreationSettings_GetIsSensor(const JoltC_BodyCreationSettings* s);
JOLTC_API void           JoltC_BodyCreationSettings_SetIsSensor(JoltC_BodyCreationSettings* s, JoltC_Bool value);
JOLTC_API JoltC_Bool     JoltC_BodyCreationSettings_GetCollideKinematicVsNonDynamic(const JoltC_BodyCreationSettings* s);
JOLTC_API void           JoltC_BodyCreationSettings_SetCollideKinematicVsNonDynamic(JoltC_BodyCreationSettings* s, JoltC_Bool value);
JOLTC_API JoltC_Bool     JoltC_BodyCreationSettings_GetUseManifoldReduction(const JoltC_BodyCreationSettings* s);
JOLTC_API void           JoltC_BodyCreationSettings_SetUseManifoldReduction(JoltC_BodyCreationSettings* s, JoltC_Bool value);
JOLTC_API JoltC_Bool     JoltC_BodyCreationSettings_GetApplyGyroscopicForce(const JoltC_BodyCreationSettings* s);
JOLTC_API void           JoltC_BodyCreationSettings_SetApplyGyroscopicForce(JoltC_BodyCreationSettings* s, JoltC_Bool value);

/* Motion quality / edge removal / sleeping */
JOLTC_API JoltC_MotionQuality JoltC_BodyCreationSettings_GetMotionQuality(const JoltC_BodyCreationSettings* s);
JOLTC_API void           JoltC_BodyCreationSettings_SetMotionQuality(JoltC_BodyCreationSettings* s, JoltC_MotionQuality value);
JOLTC_API JoltC_Bool     JoltC_BodyCreationSettings_GetEnhancedInternalEdgeRemoval(const JoltC_BodyCreationSettings* s);
JOLTC_API void           JoltC_BodyCreationSettings_SetEnhancedInternalEdgeRemoval(JoltC_BodyCreationSettings* s, JoltC_Bool value);
JOLTC_API JoltC_Bool     JoltC_BodyCreationSettings_GetAllowSleeping(const JoltC_BodyCreationSettings* s);
JOLTC_API void           JoltC_BodyCreationSettings_SetAllowSleeping(JoltC_BodyCreationSettings* s, JoltC_Bool value);

/* Float properties */
JOLTC_API float          JoltC_BodyCreationSettings_GetFriction(const JoltC_BodyCreationSettings* s);
JOLTC_API void           JoltC_BodyCreationSettings_SetFriction(JoltC_BodyCreationSettings* s, float value);
JOLTC_API float          JoltC_BodyCreationSettings_GetRestitution(const JoltC_BodyCreationSettings* s);
JOLTC_API void           JoltC_BodyCreationSettings_SetRestitution(JoltC_BodyCreationSettings* s, float value);
JOLTC_API float          JoltC_BodyCreationSettings_GetLinearDamping(const JoltC_BodyCreationSettings* s);
JOLTC_API void           JoltC_BodyCreationSettings_SetLinearDamping(JoltC_BodyCreationSettings* s, float value);
JOLTC_API float          JoltC_BodyCreationSettings_GetAngularDamping(const JoltC_BodyCreationSettings* s);
JOLTC_API void           JoltC_BodyCreationSettings_SetAngularDamping(JoltC_BodyCreationSettings* s, float value);
JOLTC_API float          JoltC_BodyCreationSettings_GetMaxLinearVelocity(const JoltC_BodyCreationSettings* s);
JOLTC_API void           JoltC_BodyCreationSettings_SetMaxLinearVelocity(JoltC_BodyCreationSettings* s, float value);
JOLTC_API float          JoltC_BodyCreationSettings_GetMaxAngularVelocity(const JoltC_BodyCreationSettings* s);
JOLTC_API void           JoltC_BodyCreationSettings_SetMaxAngularVelocity(JoltC_BodyCreationSettings* s, float value);
JOLTC_API float          JoltC_BodyCreationSettings_GetGravityFactor(const JoltC_BodyCreationSettings* s);
JOLTC_API void           JoltC_BodyCreationSettings_SetGravityFactor(JoltC_BodyCreationSettings* s, float value);

/* Solver overrides */
JOLTC_API uint32_t       JoltC_BodyCreationSettings_GetNumVelocityStepsOverride(const JoltC_BodyCreationSettings* s);
JOLTC_API void           JoltC_BodyCreationSettings_SetNumVelocityStepsOverride(JoltC_BodyCreationSettings* s, uint32_t value);
JOLTC_API uint32_t       JoltC_BodyCreationSettings_GetNumPositionStepsOverride(const JoltC_BodyCreationSettings* s);
JOLTC_API void           JoltC_BodyCreationSettings_SetNumPositionStepsOverride(JoltC_BodyCreationSettings* s, uint32_t value);

/* Mass properties */
JOLTC_API JoltC_OverrideMassProperties JoltC_BodyCreationSettings_GetOverrideMassProperties(const JoltC_BodyCreationSettings* s);
JOLTC_API void           JoltC_BodyCreationSettings_SetOverrideMassProperties(JoltC_BodyCreationSettings* s, JoltC_OverrideMassProperties value);
JOLTC_API float          JoltC_BodyCreationSettings_GetInertiaMultiplier(const JoltC_BodyCreationSettings* s);
JOLTC_API void           JoltC_BodyCreationSettings_SetInertiaMultiplier(JoltC_BodyCreationSettings* s, float value);
JOLTC_API void           JoltC_BodyCreationSettings_GetMassPropertiesOverride(const JoltC_BodyCreationSettings* s, JoltC_MassProperties* result);
JOLTC_API void           JoltC_BodyCreationSettings_SetMassPropertiesOverride(JoltC_BodyCreationSettings* s, const JoltC_MassProperties* value);

/* -------------------------------------------------------------------------- */
/*  Create / Destroy                                                          */
/* -------------------------------------------------------------------------- */
JOLTC_API JoltC_BodyID JoltC_BodyInterface_CreateBody(JoltC_BodyInterface* iface, const JoltC_BodyCreationSettings* settings);
JOLTC_API JoltC_BodyID JoltC_BodyInterface_CreateAndAddBody(JoltC_BodyInterface* iface, const JoltC_BodyCreationSettings* settings, JoltC_Activation activation);
JOLTC_API void         JoltC_BodyInterface_DestroyBody(JoltC_BodyInterface* iface, JoltC_BodyID bodyID);

/* -------------------------------------------------------------------------- */
/*  Add / Remove                                                              */
/* -------------------------------------------------------------------------- */
JOLTC_API void JoltC_BodyInterface_AddBody(JoltC_BodyInterface* iface, JoltC_BodyID bodyID, JoltC_Activation activation);
JOLTC_API void JoltC_BodyInterface_RemoveBody(JoltC_BodyInterface* iface, JoltC_BodyID bodyID);
JOLTC_API JoltC_Bool JoltC_BodyInterface_IsAdded(const JoltC_BodyInterface* iface, JoltC_BodyID bodyID);

/* -------------------------------------------------------------------------- */
/*  Activation                                                                */
/* -------------------------------------------------------------------------- */
JOLTC_API void       JoltC_BodyInterface_ActivateBody(JoltC_BodyInterface* iface, JoltC_BodyID bodyID);
JOLTC_API void       JoltC_BodyInterface_DeactivateBody(JoltC_BodyInterface* iface, JoltC_BodyID bodyID);
JOLTC_API JoltC_Bool JoltC_BodyInterface_IsActive(const JoltC_BodyInterface* iface, JoltC_BodyID bodyID);

/* -------------------------------------------------------------------------- */
/*  Position / Rotation                                                       */
/* -------------------------------------------------------------------------- */
JOLTC_API void        JoltC_BodyInterface_SetPosition(JoltC_BodyInterface* iface, JoltC_BodyID bodyID, JoltC_RVec3 position, JoltC_Activation activation);
JOLTC_API JoltC_RVec3 JoltC_BodyInterface_GetPosition(const JoltC_BodyInterface* iface, JoltC_BodyID bodyID);
JOLTC_API JoltC_RVec3 JoltC_BodyInterface_GetCenterOfMassPosition(const JoltC_BodyInterface* iface, JoltC_BodyID bodyID);

JOLTC_API void       JoltC_BodyInterface_SetRotation(JoltC_BodyInterface* iface, JoltC_BodyID bodyID, JoltC_Quat rotation, JoltC_Activation activation);
JOLTC_API JoltC_Quat JoltC_BodyInterface_GetRotation(const JoltC_BodyInterface* iface, JoltC_BodyID bodyID);

JOLTC_API void JoltC_BodyInterface_SetPositionAndRotation(JoltC_BodyInterface* iface, JoltC_BodyID bodyID, JoltC_RVec3 position, JoltC_Quat rotation, JoltC_Activation activation);
JOLTC_API void JoltC_BodyInterface_GetPositionAndRotation(const JoltC_BodyInterface* iface, JoltC_BodyID bodyID, JoltC_RVec3* outPosition, JoltC_Quat* outRotation);

/* -------------------------------------------------------------------------- */
/*  Velocity                                                                  */
/* -------------------------------------------------------------------------- */
JOLTC_API void       JoltC_BodyInterface_SetLinearVelocity(JoltC_BodyInterface* iface, JoltC_BodyID bodyID, JoltC_Vec3 velocity);
JOLTC_API JoltC_Vec3 JoltC_BodyInterface_GetLinearVelocity(const JoltC_BodyInterface* iface, JoltC_BodyID bodyID);
JOLTC_API void       JoltC_BodyInterface_AddLinearVelocity(JoltC_BodyInterface* iface, JoltC_BodyID bodyID, JoltC_Vec3 velocity);

JOLTC_API void       JoltC_BodyInterface_SetAngularVelocity(JoltC_BodyInterface* iface, JoltC_BodyID bodyID, JoltC_Vec3 angularVelocity);
JOLTC_API JoltC_Vec3 JoltC_BodyInterface_GetAngularVelocity(const JoltC_BodyInterface* iface, JoltC_BodyID bodyID);

JOLTC_API void JoltC_BodyInterface_SetLinearAndAngularVelocity(JoltC_BodyInterface* iface, JoltC_BodyID bodyID, JoltC_Vec3 linearVelocity, JoltC_Vec3 angularVelocity);
JOLTC_API void JoltC_BodyInterface_MoveKinematic(JoltC_BodyInterface* iface, JoltC_BodyID bodyID, JoltC_RVec3 targetPosition, JoltC_Quat targetRotation, float deltaTime);

/* -------------------------------------------------------------------------- */
/*  Force / Torque / Impulse                                                  */
/* -------------------------------------------------------------------------- */
JOLTC_API void JoltC_BodyInterface_AddForce(JoltC_BodyInterface* iface, JoltC_BodyID bodyID, JoltC_Vec3 force);
JOLTC_API void JoltC_BodyInterface_AddForceAtPosition(JoltC_BodyInterface* iface, JoltC_BodyID bodyID, JoltC_Vec3 force, JoltC_RVec3 point);
JOLTC_API void JoltC_BodyInterface_AddTorque(JoltC_BodyInterface* iface, JoltC_BodyID bodyID, JoltC_Vec3 torque);
JOLTC_API void JoltC_BodyInterface_AddImpulse(JoltC_BodyInterface* iface, JoltC_BodyID bodyID, JoltC_Vec3 impulse);
JOLTC_API void JoltC_BodyInterface_AddImpulseAtPosition(JoltC_BodyInterface* iface, JoltC_BodyID bodyID, JoltC_Vec3 impulse, JoltC_RVec3 point);
JOLTC_API void JoltC_BodyInterface_AddAngularImpulse(JoltC_BodyInterface* iface, JoltC_BodyID bodyID, JoltC_Vec3 angularImpulse);

/* -------------------------------------------------------------------------- */
/*  Properties                                                                */
/* -------------------------------------------------------------------------- */
JOLTC_API void           JoltC_BodyInterface_SetMotionType(JoltC_BodyInterface* iface, JoltC_BodyID bodyID, JoltC_MotionType motionType, JoltC_Activation activation);
JOLTC_API JoltC_MotionType JoltC_BodyInterface_GetMotionType(const JoltC_BodyInterface* iface, JoltC_BodyID bodyID);
JOLTC_API JoltC_BodyType   JoltC_BodyInterface_GetBodyType(const JoltC_BodyInterface* iface, JoltC_BodyID bodyID);

JOLTC_API void  JoltC_BodyInterface_SetFriction(JoltC_BodyInterface* iface, JoltC_BodyID bodyID, float friction);
JOLTC_API float JoltC_BodyInterface_GetFriction(const JoltC_BodyInterface* iface, JoltC_BodyID bodyID);

JOLTC_API void  JoltC_BodyInterface_SetRestitution(JoltC_BodyInterface* iface, JoltC_BodyID bodyID, float restitution);
JOLTC_API float JoltC_BodyInterface_GetRestitution(const JoltC_BodyInterface* iface, JoltC_BodyID bodyID);

JOLTC_API void  JoltC_BodyInterface_SetGravityFactor(JoltC_BodyInterface* iface, JoltC_BodyID bodyID, float gravityFactor);
JOLTC_API float JoltC_BodyInterface_GetGravityFactor(const JoltC_BodyInterface* iface, JoltC_BodyID bodyID);

JOLTC_API void     JoltC_BodyInterface_SetObjectLayer(JoltC_BodyInterface* iface, JoltC_BodyID bodyID, JoltC_ObjectLayer layer);
JOLTC_API JoltC_ObjectLayer JoltC_BodyInterface_GetObjectLayer(const JoltC_BodyInterface* iface, JoltC_BodyID bodyID);

JOLTC_API void     JoltC_BodyInterface_SetUserData(JoltC_BodyInterface* iface, JoltC_BodyID bodyID, uint64_t userData);
JOLTC_API uint64_t JoltC_BodyInterface_GetUserData(const JoltC_BodyInterface* iface, JoltC_BodyID bodyID);

/* -------------------------------------------------------------------------- */
/*  Shape                                                                     */
/* -------------------------------------------------------------------------- */
JOLTC_API void JoltC_BodyInterface_SetShape(JoltC_BodyInterface* iface, JoltC_BodyID bodyID, const JoltC_Shape* shape, JoltC_Bool updateMassProperties, JoltC_Activation activation);
JOLTC_API void JoltC_BodyInterface_NotifyShapeChanged(JoltC_BodyInterface* iface, JoltC_BodyID bodyID, JoltC_Vec3 previousCenterOfMass, JoltC_Bool updateMassProperties, JoltC_Activation activation);
JOLTC_API const JoltC_Shape* JoltC_BodyInterface_GetShape(const JoltC_BodyInterface* iface, JoltC_BodyID bodyID);

/* -------------------------------------------------------------------------- */
/*  Point velocity / Transform                                                */
/* -------------------------------------------------------------------------- */
JOLTC_API JoltC_Vec3 JoltC_BodyInterface_GetPointVelocity(const JoltC_BodyInterface* iface, JoltC_BodyID bodyID, JoltC_RVec3 point);
JOLTC_API JoltC_Mat44 JoltC_BodyInterface_GetWorldTransform(const JoltC_BodyInterface* iface, JoltC_BodyID bodyID);
JOLTC_API JoltC_Mat44 JoltC_BodyInterface_GetCenterOfMassTransform(const JoltC_BodyInterface* iface, JoltC_BodyID bodyID);

/* -------------------------------------------------------------------------- */
/*  Inverse mass / inertia                                                    */
/* -------------------------------------------------------------------------- */
JOLTC_API float JoltC_BodyInterface_GetInverseMass(const JoltC_BodyInterface* iface, JoltC_BodyID bodyID);
JOLTC_API void  JoltC_BodyInterface_SetInverseMass(JoltC_BodyInterface* iface, JoltC_BodyID bodyID, float inverseMass);

/* -------------------------------------------------------------------------- */
/*  Additional body interface methods                                         */
/* -------------------------------------------------------------------------- */
JOLTC_API void  JoltC_BodyInterface_SetMotionQuality(JoltC_BodyInterface* iface, JoltC_BodyID bodyID, JoltC_MotionQuality quality);
JOLTC_API JoltC_MotionQuality JoltC_BodyInterface_GetMotionQuality(const JoltC_BodyInterface* iface, JoltC_BodyID bodyID);

JOLTC_API void  JoltC_BodyInterface_SetLinearDamping(JoltC_BodyInterface* iface, JoltC_BodyID bodyID, float damping);
JOLTC_API float JoltC_BodyInterface_GetLinearDamping(const JoltC_BodyInterface* iface, JoltC_BodyID bodyID);
JOLTC_API void  JoltC_BodyInterface_SetAngularDamping(JoltC_BodyInterface* iface, JoltC_BodyID bodyID, float damping);
JOLTC_API float JoltC_BodyInterface_GetAngularDamping(const JoltC_BodyInterface* iface, JoltC_BodyID bodyID);

JOLTC_API void  JoltC_BodyInterface_SetMaxLinearVelocity(JoltC_BodyInterface* iface, JoltC_BodyID bodyID, float maxVelocity);
JOLTC_API float JoltC_BodyInterface_GetMaxLinearVelocity(const JoltC_BodyInterface* iface, JoltC_BodyID bodyID);
JOLTC_API void  JoltC_BodyInterface_SetMaxAngularVelocity(JoltC_BodyInterface* iface, JoltC_BodyID bodyID, float maxVelocity);
JOLTC_API float JoltC_BodyInterface_GetMaxAngularVelocity(const JoltC_BodyInterface* iface, JoltC_BodyID bodyID);

JOLTC_API JoltC_Vec3 JoltC_BodyInterface_GetWorldSpaceSurfaceNormal(const JoltC_BodyInterface* iface, JoltC_BodyID bodyID, uint32_t subShapeID, JoltC_RVec3 position);

/* -------------------------------------------------------------------------- */
/*  Extended BodyInterface methods                                            */
/* -------------------------------------------------------------------------- */

/* Forward decl for Body handle */
typedef struct JoltC_Body JoltC_Body;

JOLTC_API JoltC_Body* JoltC_BodyInterface_CreateBodyDirect(JoltC_BodyInterface* iface, const JoltC_BodyCreationSettings* settings);
JOLTC_API JoltC_Body* JoltC_BodyInterface_CreateBodyWithID(JoltC_BodyInterface* iface, JoltC_BodyID bodyID, const JoltC_BodyCreationSettings* settings);
JOLTC_API JoltC_Body* JoltC_BodyInterface_CreateBodyWithoutID(JoltC_BodyInterface* iface, const JoltC_BodyCreationSettings* settings);
JOLTC_API void        JoltC_BodyInterface_DestroyBodyWithoutID(JoltC_BodyInterface* iface, JoltC_Body* body);
JOLTC_API int         JoltC_BodyInterface_AssignBodyID(JoltC_BodyInterface* iface, JoltC_Body* body);
JOLTC_API int         JoltC_BodyInterface_AssignBodyIDWithID(JoltC_BodyInterface* iface, JoltC_Body* body, JoltC_BodyID desiredID);
JOLTC_API JoltC_Body* JoltC_BodyInterface_UnassignBodyID(JoltC_BodyInterface* iface, JoltC_BodyID bodyID);

JOLTC_API void JoltC_BodyInterface_RemoveAndDestroyBody(JoltC_BodyInterface* iface, JoltC_BodyID bodyID);

JOLTC_API void JoltC_BodyInterface_SetPositionAndRotationWhenChanged(JoltC_BodyInterface* iface, JoltC_BodyID bodyID, JoltC_RVec3 position, JoltC_Quat rotation, JoltC_Activation activation);
JOLTC_API void JoltC_BodyInterface_SetPositionRotationAndVelocity(JoltC_BodyInterface* iface, JoltC_BodyID bodyID, JoltC_RVec3 position, JoltC_Quat rotation, JoltC_Vec3 linearVelocity, JoltC_Vec3 angularVelocity);

JOLTC_API void JoltC_BodyInterface_GetLinearAndAngularVelocity(const JoltC_BodyInterface* iface, JoltC_BodyID bodyID, JoltC_Vec3* outLinear, JoltC_Vec3* outAngular);
JOLTC_API void JoltC_BodyInterface_AddLinearAndAngularVelocity(JoltC_BodyInterface* iface, JoltC_BodyID bodyID, JoltC_Vec3 linearVelocity, JoltC_Vec3 angularVelocity);

JOLTC_API void JoltC_BodyInterface_AddForceAndTorque(JoltC_BodyInterface* iface, JoltC_BodyID bodyID, JoltC_Vec3 force, JoltC_Vec3 torque);

JOLTC_API void JoltC_BodyInterface_GetInverseInertia(const JoltC_BodyInterface* iface, JoltC_BodyID bodyID, JoltC_Mat44* result);

JOLTC_API void JoltC_BodyInterface_ActivateBodies(JoltC_BodyInterface* iface, const JoltC_BodyID* bodyIDs, uint32_t count);
JOLTC_API void JoltC_BodyInterface_DeactivateBodies(JoltC_BodyInterface* iface, const JoltC_BodyID* bodyIDs, uint32_t count);
JOLTC_API void JoltC_BodyInterface_ResetSleepTimer(JoltC_BodyInterface* iface, JoltC_BodyID bodyID);

JOLTC_API void JoltC_BodyInterface_SetUseManifoldReduction(JoltC_BodyInterface* iface, JoltC_BodyID bodyID, int value);
JOLTC_API int  JoltC_BodyInterface_GetUseManifoldReduction(const JoltC_BodyInterface* iface, JoltC_BodyID bodyID);

JOLTC_API void JoltC_BodyInterface_SetIsSensor(JoltC_BodyInterface* iface, JoltC_BodyID bodyID, int value);
JOLTC_API int  JoltC_BodyInterface_IsSensor(const JoltC_BodyInterface* iface, JoltC_BodyID bodyID);

JOLTC_API void JoltC_BodyInterface_InvalidateContactCache(JoltC_BodyInterface* iface, JoltC_BodyID bodyID);

JOLTC_API int  JoltC_BodyInterface_ApplyBuoyancyImpulse(JoltC_BodyInterface* iface, JoltC_BodyID bodyID, JoltC_RVec3 surfacePosition, JoltC_Vec3 surfaceNormal, float buoyancy, float linearDrag, float angularDrag, JoltC_Vec3 fluidVelocity, JoltC_Vec3 gravity, float deltaTime);

/* -------------------------------------------------------------------------- */
/*  BodyInterface — collision group                                           */
/* -------------------------------------------------------------------------- */
JOLTC_API void JoltC_BodyInterface_GetCollisionGroup(const JoltC_BodyInterface* iface, JoltC_BodyID bodyID, JoltC_CollisionGroup* outGroup);
JOLTC_API void JoltC_BodyInterface_SetCollisionGroup(JoltC_BodyInterface* iface, JoltC_BodyID bodyID, const JoltC_CollisionGroup* group);

/* -------------------------------------------------------------------------- */
/*  BodyInterface — additional methods (batch 2)                              */
/* -------------------------------------------------------------------------- */
JOLTC_API void JoltC_BodyInterface_ActivateBodiesInAABox(JoltC_BodyInterface* iface, JoltC_Vec3 min, JoltC_Vec3 max);
JOLTC_API void JoltC_BodyInterface_AddForce2(JoltC_BodyInterface* iface, JoltC_BodyID bodyID, JoltC_Vec3 force, JoltC_RVec3 point);
JOLTC_API void JoltC_BodyInterface_AddImpulse2(JoltC_BodyInterface* iface, JoltC_BodyID bodyID, JoltC_Vec3 impulse, JoltC_RVec3 point);
JOLTC_API int  JoltC_BodyInterface_AssignBodyID2(JoltC_BodyInterface* iface, JoltC_Body* body, JoltC_BodyID desiredID);

/* -------------------------------------------------------------------------- */
/*  Body — direct access (requires body lock or known-safe context)           */
/* -------------------------------------------------------------------------- */
JOLTC_API void             JoltC_Body_GetCollisionGroup(const JoltC_Body* body, JoltC_CollisionGroup* outGroup);
JOLTC_API void             JoltC_Body_SetCollisionGroup(JoltC_Body* body, const JoltC_CollisionGroup* group);
JOLTC_API const JoltC_Body* JoltC_Body_GetFixedToWorldBody(void);

/* -------------------------------------------------------------------------- */
/*  BodyInterface — SoftBody creation                                         */
/* -------------------------------------------------------------------------- */
JOLTC_API JoltC_Body*   JoltC_BodyInterface_CreateSoftBody(JoltC_BodyInterface* bi, const JoltC_SoftBodyCreationSettings* settings);
JOLTC_API JoltC_Body*   JoltC_BodyInterface_CreateSoftBodyWithID(JoltC_BodyInterface* bi, const JoltC_SoftBodyCreationSettings* settings, uint32_t bodyId);
JOLTC_API JoltC_Body*   JoltC_BodyInterface_CreateSoftBodyWithoutID(JoltC_BodyInterface* bi, const JoltC_SoftBodyCreationSettings* settings);
JOLTC_API JoltC_BodyID  JoltC_BodyInterface_CreateAndAddSoftBody(JoltC_BodyInterface* bi, const JoltC_SoftBodyCreationSettings* settings, JoltC_Activation activation);

#ifdef __cplusplus
}
#endif

#endif /* JOLTC_BODY_H */
