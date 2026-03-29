/* JoltC - C bindings for JoltPhysics
 * SPDX-License-Identifier: MIT
 *
 * Direct Body access functions (used via body locks or from contact callbacks).
 * Also: BodyLockInterface, MotionProperties.
 */

#ifndef JOLTC_BODY_ACCESS_H
#define JOLTC_BODY_ACCESS_H

#include <JoltC/common.h>
#include <JoltC/math.h>

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */
/*  Opaque handles for direct-access types                                    */
/* -------------------------------------------------------------------------- */
typedef struct JoltC_Body              JoltC_Body;
typedef struct JoltC_MotionProperties  JoltC_MotionProperties;
typedef struct JoltC_BodyLockInterface JoltC_BodyLockInterface;
typedef struct JoltC_BodyLockRead      JoltC_BodyLockRead;
typedef struct JoltC_BodyLockWrite     JoltC_BodyLockWrite;
typedef struct JoltC_BodyLockMultiRead  JoltC_BodyLockMultiRead;
typedef struct JoltC_BodyLockMultiWrite JoltC_BodyLockMultiWrite;

/* -------------------------------------------------------------------------- */
/*  SubShapeID (passed by value)                                              */
/* -------------------------------------------------------------------------- */
typedef uint32_t JoltC_SubShapeID;

/* -------------------------------------------------------------------------- */
/*  BodyLockInterface                                                         */
/* -------------------------------------------------------------------------- */
JOLTC_API const JoltC_BodyLockInterface* JoltC_PhysicsSystem_GetBodyLockInterface(const JoltC_PhysicsSystem* system);
JOLTC_API const JoltC_BodyLockInterface* JoltC_PhysicsSystem_GetBodyLockInterfaceNoLock(const JoltC_PhysicsSystem* system);

/* -------------------------------------------------------------------------- */
/*  BodyLock Read/Write                                                       */
/* -------------------------------------------------------------------------- */
JOLTC_API JoltC_BodyLockRead*  JoltC_BodyLockRead_Create(const JoltC_BodyLockInterface* lockInterface, JoltC_BodyID bodyID);
JOLTC_API const JoltC_Body*    JoltC_BodyLockRead_GetBody(const JoltC_BodyLockRead* lock);
JOLTC_API void                 JoltC_BodyLockRead_Destroy(JoltC_BodyLockRead* lock);

JOLTC_API JoltC_BodyLockWrite* JoltC_BodyLockWrite_Create(const JoltC_BodyLockInterface* lockInterface, JoltC_BodyID bodyID);
JOLTC_API JoltC_Body*          JoltC_BodyLockWrite_GetBody(const JoltC_BodyLockWrite* lock);
JOLTC_API void                 JoltC_BodyLockWrite_Destroy(JoltC_BodyLockWrite* lock);

/* -------------------------------------------------------------------------- */
/*  BodyLockInterface — Lock / Unlock (alternative naming)                    */
/* -------------------------------------------------------------------------- */
JOLTC_API JoltC_BodyLockRead*  JoltC_BodyLockInterface_LockRead(const JoltC_BodyLockInterface* lockInterface, uint32_t bodyId);
JOLTC_API void                 JoltC_BodyLockInterface_UnlockRead(const JoltC_BodyLockInterface* lockInterface, JoltC_BodyLockRead* lock);
JOLTC_API JoltC_BodyLockWrite* JoltC_BodyLockInterface_LockWrite(const JoltC_BodyLockInterface* lockInterface, uint32_t bodyId);
JOLTC_API void                 JoltC_BodyLockInterface_UnlockWrite(const JoltC_BodyLockInterface* lockInterface, JoltC_BodyLockWrite* lock);

/* -------------------------------------------------------------------------- */
/*  BodyLockMulti Read / Write                                                */
/* -------------------------------------------------------------------------- */
JOLTC_API JoltC_BodyLockMultiRead*  JoltC_BodyLockInterface_LockMultiRead(const JoltC_BodyLockInterface* lockInterface, const uint32_t* bodyIds, int count);
JOLTC_API JoltC_BodyLockMultiWrite* JoltC_BodyLockInterface_LockMultiWrite(const JoltC_BodyLockInterface* lockInterface, const uint32_t* bodyIds, int count);
JOLTC_API void                      JoltC_BodyLockMultiRead_Destroy(JoltC_BodyLockMultiRead* lock);
JOLTC_API void                      JoltC_BodyLockMultiWrite_Destroy(JoltC_BodyLockMultiWrite* lock);
JOLTC_API JoltC_Body*               JoltC_BodyLockMultiWrite_GetBody(JoltC_BodyLockMultiWrite* lock, int index);

/* -------------------------------------------------------------------------- */
/*  Body — read functions                                                     */
/* -------------------------------------------------------------------------- */
JOLTC_API JoltC_BodyID       JoltC_Body_GetID(const JoltC_Body* body);
JOLTC_API JoltC_BodyType     JoltC_Body_GetBodyType(const JoltC_Body* body);
JOLTC_API int                JoltC_Body_IsRigidBody(const JoltC_Body* body);
JOLTC_API int                JoltC_Body_IsSoftBody(const JoltC_Body* body);
JOLTC_API int                JoltC_Body_IsActive(const JoltC_Body* body);
JOLTC_API int                JoltC_Body_IsStatic(const JoltC_Body* body);
JOLTC_API int                JoltC_Body_IsKinematic(const JoltC_Body* body);
JOLTC_API int                JoltC_Body_IsDynamic(const JoltC_Body* body);
JOLTC_API int                JoltC_Body_CanBeKinematicOrDynamic(const JoltC_Body* body);

JOLTC_API int                JoltC_Body_IsSensor(const JoltC_Body* body);
JOLTC_API void               JoltC_Body_SetIsSensor(JoltC_Body* body, int value);

JOLTC_API int                JoltC_Body_GetCollideKinematicVsNonDynamic(const JoltC_Body* body);
JOLTC_API void               JoltC_Body_SetCollideKinematicVsNonDynamic(JoltC_Body* body, int value);

JOLTC_API int                JoltC_Body_GetUseManifoldReduction(const JoltC_Body* body);
JOLTC_API void               JoltC_Body_SetUseManifoldReduction(JoltC_Body* body, int value);
JOLTC_API int                JoltC_Body_GetUseManifoldReductionWithBody(const JoltC_Body* body, const JoltC_Body* other);

JOLTC_API int                JoltC_Body_GetApplyGyroscopicForce(const JoltC_Body* body);
JOLTC_API void               JoltC_Body_SetApplyGyroscopicForce(JoltC_Body* body, int value);

JOLTC_API int                JoltC_Body_GetEnhancedInternalEdgeRemoval(const JoltC_Body* body);
JOLTC_API void               JoltC_Body_SetEnhancedInternalEdgeRemoval(JoltC_Body* body, int value);
JOLTC_API int                JoltC_Body_GetEnhancedInternalEdgeRemovalWithBody(const JoltC_Body* body, const JoltC_Body* other);

JOLTC_API JoltC_MotionType   JoltC_Body_GetMotionType(const JoltC_Body* body);
JOLTC_API void               JoltC_Body_SetMotionType(JoltC_Body* body, JoltC_MotionType motionType);

JOLTC_API JoltC_BroadPhaseLayer JoltC_Body_GetBroadPhaseLayer(const JoltC_Body* body);
JOLTC_API JoltC_ObjectLayer  JoltC_Body_GetObjectLayer(const JoltC_Body* body);

JOLTC_API int                JoltC_Body_GetAllowSleeping(const JoltC_Body* body);
JOLTC_API void               JoltC_Body_SetAllowSleeping(JoltC_Body* body, int allowSleeping);
JOLTC_API void               JoltC_Body_ResetSleepTimer(JoltC_Body* body);

JOLTC_API float              JoltC_Body_GetFriction(const JoltC_Body* body);
JOLTC_API void               JoltC_Body_SetFriction(JoltC_Body* body, float friction);
JOLTC_API float              JoltC_Body_GetRestitution(const JoltC_Body* body);
JOLTC_API void               JoltC_Body_SetRestitution(JoltC_Body* body, float restitution);

JOLTC_API void               JoltC_Body_GetLinearVelocity(const JoltC_Body* body, JoltC_Vec3* result);
JOLTC_API void               JoltC_Body_SetLinearVelocity(JoltC_Body* body, const JoltC_Vec3* velocity);
JOLTC_API void               JoltC_Body_SetLinearVelocityClamped(JoltC_Body* body, const JoltC_Vec3* velocity);
JOLTC_API void               JoltC_Body_GetAngularVelocity(const JoltC_Body* body, JoltC_Vec3* result);
JOLTC_API void               JoltC_Body_SetAngularVelocity(JoltC_Body* body, const JoltC_Vec3* velocity);
JOLTC_API void               JoltC_Body_SetAngularVelocityClamped(JoltC_Body* body, const JoltC_Vec3* velocity);

JOLTC_API void               JoltC_Body_GetPointVelocityCOM(const JoltC_Body* body, const JoltC_Vec3* pointRelativeToCOM, JoltC_Vec3* result);
JOLTC_API void               JoltC_Body_GetPointVelocity(const JoltC_Body* body, const JoltC_RVec3* point, JoltC_Vec3* result);

JOLTC_API void               JoltC_Body_AddForce(JoltC_Body* body, const JoltC_Vec3* force);
JOLTC_API void               JoltC_Body_AddForceAtPosition(JoltC_Body* body, const JoltC_Vec3* force, const JoltC_RVec3* position);
JOLTC_API void               JoltC_Body_AddTorque(JoltC_Body* body, const JoltC_Vec3* torque);
JOLTC_API void               JoltC_Body_GetAccumulatedForce(const JoltC_Body* body, JoltC_Vec3* result);
JOLTC_API void               JoltC_Body_GetAccumulatedTorque(const JoltC_Body* body, JoltC_Vec3* result);
JOLTC_API void               JoltC_Body_ResetForce(JoltC_Body* body);
JOLTC_API void               JoltC_Body_ResetTorque(JoltC_Body* body);
JOLTC_API void               JoltC_Body_ResetMotion(JoltC_Body* body);

JOLTC_API void               JoltC_Body_GetInverseInertia(const JoltC_Body* body, JoltC_Mat44* result);

JOLTC_API void               JoltC_Body_AddImpulse(JoltC_Body* body, const JoltC_Vec3* impulse);
JOLTC_API void               JoltC_Body_AddImpulseAtPosition(JoltC_Body* body, const JoltC_Vec3* impulse, const JoltC_RVec3* position);
JOLTC_API void               JoltC_Body_AddAngularImpulse(JoltC_Body* body, const JoltC_Vec3* angularImpulse);
JOLTC_API void               JoltC_Body_MoveKinematic(JoltC_Body* body, const JoltC_RVec3* targetPosition, const JoltC_Quat* targetRotation, float deltaTime);

JOLTC_API int                JoltC_Body_ApplyBuoyancyImpulse(JoltC_Body* body, const JoltC_RVec3* surfacePosition, const JoltC_Vec3* surfaceNormal, float buoyancy, float linearDrag, float angularDrag, const JoltC_Vec3* fluidVelocity, const JoltC_Vec3* gravity, float deltaTime);

JOLTC_API int                JoltC_Body_IsInBroadPhase(const JoltC_Body* body);
JOLTC_API int                JoltC_Body_IsCollisionCacheInvalid(const JoltC_Body* body);

JOLTC_API const JoltC_Shape* JoltC_Body_GetShape(const JoltC_Body* body);

JOLTC_API void               JoltC_Body_GetPosition(const JoltC_Body* body, JoltC_RVec3* result);
JOLTC_API void               JoltC_Body_GetRotation(const JoltC_Body* body, JoltC_Quat* result);
JOLTC_API void               JoltC_Body_GetWorldTransform(const JoltC_Body* body, JoltC_Mat44* result);
JOLTC_API void               JoltC_Body_GetCenterOfMassPosition(const JoltC_Body* body, JoltC_RVec3* result);
JOLTC_API void               JoltC_Body_GetCenterOfMassTransform(const JoltC_Body* body, JoltC_Mat44* result);
JOLTC_API void               JoltC_Body_GetInverseCenterOfMassTransform(const JoltC_Body* body, JoltC_Mat44* result);
JOLTC_API void               JoltC_Body_GetWorldSpaceBounds(const JoltC_Body* body, JoltC_AABox* result);
JOLTC_API void               JoltC_Body_GetWorldSpaceSurfaceNormal(const JoltC_Body* body, JoltC_SubShapeID subShapeID, const JoltC_RVec3* position, JoltC_Vec3* result);

JOLTC_API JoltC_MotionProperties* JoltC_Body_GetMotionProperties(JoltC_Body* body);
JOLTC_API JoltC_MotionProperties* JoltC_Body_GetMotionPropertiesUnchecked(JoltC_Body* body);

JOLTC_API void               JoltC_Body_SetUserData(JoltC_Body* body, uint64_t userData);
JOLTC_API uint64_t           JoltC_Body_GetUserData(const JoltC_Body* body);

/* -------------------------------------------------------------------------- */
/*  MotionProperties                                                          */
/* -------------------------------------------------------------------------- */
JOLTC_API JoltC_AllowedDOFs  JoltC_MotionProperties_GetAllowedDOFs(const JoltC_MotionProperties* properties);
JOLTC_API void               JoltC_MotionProperties_SetLinearDamping(JoltC_MotionProperties* properties, float damping);
JOLTC_API float              JoltC_MotionProperties_GetLinearDamping(const JoltC_MotionProperties* properties);
JOLTC_API void               JoltC_MotionProperties_SetAngularDamping(JoltC_MotionProperties* properties, float damping);
JOLTC_API float              JoltC_MotionProperties_GetAngularDamping(const JoltC_MotionProperties* properties);
JOLTC_API float              JoltC_MotionProperties_GetInverseMassUnchecked(const JoltC_MotionProperties* properties);
JOLTC_API void               JoltC_MotionProperties_SetInverseMass(JoltC_MotionProperties* properties, float inverseMass);
JOLTC_API void               JoltC_MotionProperties_GetInverseInertiaDiagonal(const JoltC_MotionProperties* properties, JoltC_Vec3* result);
JOLTC_API void               JoltC_MotionProperties_GetInertiaRotation(const JoltC_MotionProperties* properties, JoltC_Quat* result);
JOLTC_API void               JoltC_MotionProperties_SetInverseInertia(JoltC_MotionProperties* properties, const JoltC_Vec3* diagonal, const JoltC_Quat* rotation);
JOLTC_API void               JoltC_MotionProperties_SetMassProperties(JoltC_MotionProperties* properties, JoltC_AllowedDOFs allowedDOFs, const JoltC_MassProperties* massProperties);
JOLTC_API void               JoltC_MotionProperties_ScaleToMass(JoltC_MotionProperties* properties, float mass);

#ifdef __cplusplus
}
#endif

#endif /* JOLTC_BODY_ACCESS_H */
