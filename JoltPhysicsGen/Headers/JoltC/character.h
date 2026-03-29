/* JoltC - C bindings for JoltPhysics
 * SPDX-License-Identifier: MIT
 *
 * Character (virtual) controller.
 */

#ifndef JOLTC_CHARACTER_H
#define JOLTC_CHARACTER_H

#include <JoltC/common.h>

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */
/*  CharacterVirtual settings                                                 */
/* -------------------------------------------------------------------------- */
typedef struct JoltC_CharacterVirtualSettings {
    /* CharacterBaseSettings members */
    JoltC_Vec3          up;
    float               maxSlopeAngle;
    JoltC_Bool          enhancedInternalEdgeRemoval;
    const JoltC_Shape*  shape;
    /* CharacterVirtualSettings members */
    float               mass;
    float               maxStrength;
    JoltC_Vec3          shapeOffset;
    JoltC_BackFaceMode  backFaceMode;
    float               predictiveContactDistance;
    uint32_t            maxCollisionIterations;
    uint32_t            maxConstraintIterations;
    float               minTimeRemaining;
    float               collisionTolerance;
    float               characterPadding;
    uint32_t            maxNumHits;
    float               hitReductionCosMaxAngle;
    float               penetrationRecoverySpeed;
    const JoltC_Shape*  innerBodyShape;
    JoltC_ObjectLayer   innerBodyLayer;
} JoltC_CharacterVirtualSettings;

JOLTC_API void JoltC_CharacterVirtualSettings_SetDefault(JoltC_CharacterVirtualSettings* settings);

/* -------------------------------------------------------------------------- */
/*  CharacterContactListener callbacks                                        */
/* -------------------------------------------------------------------------- */
typedef void (*JoltC_OnCharacterContactValidateFn)(void* userData, JoltC_BodyID bodyID2, JoltC_Bool* outAccept);
typedef void (*JoltC_OnCharacterContactAddedFn)(void* userData, JoltC_BodyID bodyID2, JoltC_RVec3 contactPosition, JoltC_Vec3 contactNormal, JoltC_Bool* outCanPushCharacter, JoltC_Bool* outCanReceiveImpulses);
typedef void (*JoltC_OnCharacterContactPersistedFn)(void* userData, JoltC_BodyID bodyID2, JoltC_RVec3 contactPosition, JoltC_Vec3 contactNormal, JoltC_Bool* outCanPushCharacter, JoltC_Bool* outCanReceiveImpulses);

JOLTC_API JoltC_CharacterContactListener* JoltC_CharacterContactListener_Create(
    JoltC_OnCharacterContactValidateFn   onValidate,
    JoltC_OnCharacterContactAddedFn      onAdded,
    JoltC_OnCharacterContactPersistedFn  onPersisted,
    void*                                userData);
JOLTC_API void JoltC_CharacterContactListener_Destroy(JoltC_CharacterContactListener* listener);

/* -------------------------------------------------------------------------- */
/*  CharacterVirtual create / destroy                                         */
/* -------------------------------------------------------------------------- */
JOLTC_API JoltC_CharacterVirtual* JoltC_CharacterVirtual_Create(
    const JoltC_CharacterVirtualSettings* settings,
    JoltC_RVec3                           position,
    JoltC_Quat                            rotation,
    uint64_t                              userData,
    JoltC_PhysicsSystem*                  system);
JOLTC_API void JoltC_CharacterVirtual_Destroy(JoltC_CharacterVirtual* character);

/* -------------------------------------------------------------------------- */
/*  CharacterVirtual methods                                                  */
/* -------------------------------------------------------------------------- */
JOLTC_API void       JoltC_CharacterVirtual_SetListener(JoltC_CharacterVirtual* c, JoltC_CharacterContactListener* listener);
JOLTC_API JoltC_Vec3 JoltC_CharacterVirtual_GetLinearVelocity(const JoltC_CharacterVirtual* c);
JOLTC_API void       JoltC_CharacterVirtual_SetLinearVelocity(JoltC_CharacterVirtual* c, JoltC_Vec3 velocity);
JOLTC_API JoltC_RVec3 JoltC_CharacterVirtual_GetPosition(const JoltC_CharacterVirtual* c);
JOLTC_API void       JoltC_CharacterVirtual_SetPosition(JoltC_CharacterVirtual* c, JoltC_RVec3 position);
JOLTC_API JoltC_Quat JoltC_CharacterVirtual_GetRotation(const JoltC_CharacterVirtual* c);
JOLTC_API void       JoltC_CharacterVirtual_SetRotation(JoltC_CharacterVirtual* c, JoltC_Quat rotation);
JOLTC_API JoltC_RVec3 JoltC_CharacterVirtual_GetCenterOfMassPosition(const JoltC_CharacterVirtual* c);

JOLTC_API float JoltC_CharacterVirtual_GetMass(const JoltC_CharacterVirtual* c);
JOLTC_API void  JoltC_CharacterVirtual_SetMass(JoltC_CharacterVirtual* c, float mass);
JOLTC_API float JoltC_CharacterVirtual_GetMaxStrength(const JoltC_CharacterVirtual* c);
JOLTC_API void  JoltC_CharacterVirtual_SetMaxStrength(JoltC_CharacterVirtual* c, float strength);
JOLTC_API float JoltC_CharacterVirtual_GetPenetrationRecoverySpeed(const JoltC_CharacterVirtual* c);
JOLTC_API void  JoltC_CharacterVirtual_SetPenetrationRecoverySpeed(JoltC_CharacterVirtual* c, float speed);
JOLTC_API float JoltC_CharacterVirtual_GetCharacterPadding(const JoltC_CharacterVirtual* c);
JOLTC_API uint32_t JoltC_CharacterVirtual_GetMaxNumHits(const JoltC_CharacterVirtual* c);
JOLTC_API void  JoltC_CharacterVirtual_SetMaxNumHits(JoltC_CharacterVirtual* c, uint32_t maxHits);

JOLTC_API uint64_t JoltC_CharacterVirtual_GetUserData(const JoltC_CharacterVirtual* c);
JOLTC_API void     JoltC_CharacterVirtual_SetUserData(JoltC_CharacterVirtual* c, uint64_t userData);
JOLTC_API JoltC_BodyID JoltC_CharacterVirtual_GetInnerBodyID(const JoltC_CharacterVirtual* c);

/* -------------------------------------------------------------------------- */
/*  CharacterBase ground state                                                */
/* -------------------------------------------------------------------------- */
JOLTC_API JoltC_GroundState JoltC_CharacterVirtual_GetGroundState(const JoltC_CharacterVirtual* c);
JOLTC_API JoltC_Bool        JoltC_CharacterVirtual_IsSupported(const JoltC_CharacterVirtual* c);
JOLTC_API JoltC_RVec3       JoltC_CharacterVirtual_GetGroundPosition(const JoltC_CharacterVirtual* c);
JOLTC_API JoltC_Vec3        JoltC_CharacterVirtual_GetGroundNormal(const JoltC_CharacterVirtual* c);
JOLTC_API JoltC_Vec3        JoltC_CharacterVirtual_GetGroundVelocity(const JoltC_CharacterVirtual* c);
JOLTC_API JoltC_BodyID      JoltC_CharacterVirtual_GetGroundBodyID(const JoltC_CharacterVirtual* c);

JOLTC_API JoltC_Vec3 JoltC_CharacterVirtual_GetUp(const JoltC_CharacterVirtual* c);
JOLTC_API void       JoltC_CharacterVirtual_SetUp(JoltC_CharacterVirtual* c, JoltC_Vec3 up);
JOLTC_API void       JoltC_CharacterVirtual_SetMaxSlopeAngle(JoltC_CharacterVirtual* c, float maxSlopeAngle);

JOLTC_API const JoltC_Shape* JoltC_CharacterVirtual_GetShape(const JoltC_CharacterVirtual* c);
JOLTC_API JoltC_Bool JoltC_CharacterVirtual_SetShape(JoltC_CharacterVirtual* c, const JoltC_Shape* shape, float maxPenetrationDepth,
    JoltC_TempAllocator* allocator);

/* -------------------------------------------------------------------------- */
/*  CharacterVirtual simulation                                               */
/* -------------------------------------------------------------------------- */
JOLTC_API void JoltC_CharacterVirtual_Update(
    JoltC_CharacterVirtual* c,
    float                   deltaTime,
    JoltC_Vec3              gravity,
    JoltC_TempAllocator*    allocator);

typedef struct JoltC_ExtendedUpdateSettings {
    JoltC_Vec3 stickToFloorStepDown;
    JoltC_Vec3 walkStairsStepUp;
    float      walkStairsMinStepForward;
    float      walkStairsStepForwardTest;
    float      walkStairsCosAngleForwardContact;
    JoltC_Vec3 walkStairsStepDownExtra;
} JoltC_ExtendedUpdateSettings;

JOLTC_API void JoltC_ExtendedUpdateSettings_SetDefault(JoltC_ExtendedUpdateSettings* settings);

JOLTC_API void JoltC_CharacterVirtual_ExtendedUpdate(
    JoltC_CharacterVirtual*               c,
    float                                 deltaTime,
    JoltC_Vec3                            gravity,
    const JoltC_ExtendedUpdateSettings*   settings,
    JoltC_TempAllocator*                  allocator);

JOLTC_API void JoltC_CharacterVirtual_RefreshContacts(
    JoltC_CharacterVirtual* c,
    JoltC_TempAllocator*    allocator);

/* -------------------------------------------------------------------------- */
/*  Additional CharacterBase/CharacterVirtual accessors                       */
/* -------------------------------------------------------------------------- */
JOLTC_API float     JoltC_CharacterVirtual_GetCosMaxSlopeAngle(const JoltC_CharacterVirtual* c);
JOLTC_API JoltC_Bool JoltC_CharacterVirtual_IsSlopeTooSteep(const JoltC_CharacterVirtual* c, JoltC_Vec3 normal);
JOLTC_API JoltC_SubShapeID JoltC_CharacterVirtual_GetGroundSubShapeID(const JoltC_CharacterVirtual* c);
JOLTC_API uint64_t  JoltC_CharacterVirtual_GetGroundUserData(const JoltC_CharacterVirtual* c);
JOLTC_API JoltC_Mat44 JoltC_CharacterVirtual_GetWorldTransform(const JoltC_CharacterVirtual* c);
JOLTC_API JoltC_Mat44 JoltC_CharacterVirtual_GetCenterOfMassTransform(const JoltC_CharacterVirtual* c);
JOLTC_API JoltC_Bool JoltC_CharacterVirtual_GetEnhancedInternalEdgeRemoval(const JoltC_CharacterVirtual* c);
JOLTC_API void       JoltC_CharacterVirtual_SetEnhancedInternalEdgeRemoval(JoltC_CharacterVirtual* c, JoltC_Bool value);
JOLTC_API float      JoltC_CharacterVirtual_GetHitReductionCosMaxAngle(const JoltC_CharacterVirtual* c);
JOLTC_API void       JoltC_CharacterVirtual_SetHitReductionCosMaxAngle(JoltC_CharacterVirtual* c, float value);
JOLTC_API JoltC_Bool JoltC_CharacterVirtual_GetMaxHitsExceeded(const JoltC_CharacterVirtual* c);
JOLTC_API JoltC_Vec3 JoltC_CharacterVirtual_GetShapeOffset(const JoltC_CharacterVirtual* c);
JOLTC_API void       JoltC_CharacterVirtual_SetShapeOffset(JoltC_CharacterVirtual* c, JoltC_Vec3 value);
JOLTC_API JoltC_Vec3 JoltC_CharacterVirtual_CancelVelocityTowardsSteepSlopes(const JoltC_CharacterVirtual* c, JoltC_Vec3 desiredVelocity);
JOLTC_API void       JoltC_CharacterVirtual_UpdateGroundVelocity(JoltC_CharacterVirtual* c);
JOLTC_API JoltC_Bool JoltC_CharacterVirtual_HasCollidedWithBody(const JoltC_CharacterVirtual* c, JoltC_BodyID bodyID);
JOLTC_API uint32_t   JoltC_CharacterVirtual_GetNumActiveContacts(const JoltC_CharacterVirtual* c);
JOLTC_API void       JoltC_CharacterVirtual_SetInnerBodyShape(JoltC_CharacterVirtual* c, const JoltC_Shape* shape);

/* -------------------------------------------------------------------------- */
/*  Non-virtual Character (kinematic body-based)                              */
/* -------------------------------------------------------------------------- */
typedef struct JoltC_Character JoltC_Character;

typedef struct JoltC_CharacterSettings {
    /* CharacterBaseSettings members */
    JoltC_Vec3          up;
    float               maxSlopeAngle;
    JoltC_Bool          enhancedInternalEdgeRemoval;
    const JoltC_Shape*  shape;
    /* CharacterSettings members */
    JoltC_ObjectLayer   layer;
    float               mass;
    float               friction;
    float               gravityFactor;
} JoltC_CharacterSettings;

JOLTC_API void JoltC_CharacterSettings_SetDefault(JoltC_CharacterSettings* settings);

JOLTC_API JoltC_Character* JoltC_Character_Create(
    const JoltC_CharacterSettings* settings,
    JoltC_RVec3                    position,
    JoltC_Quat                     rotation,
    uint64_t                       userData,
    JoltC_PhysicsSystem*           system);
JOLTC_API void JoltC_Character_Destroy(JoltC_Character* character);

JOLTC_API void JoltC_Character_AddToPhysicsSystem(JoltC_Character* c, JoltC_Activation activation, JoltC_Bool lockBodies);
JOLTC_API void JoltC_Character_RemoveFromPhysicsSystem(JoltC_Character* c, JoltC_Bool lockBodies);
JOLTC_API void JoltC_Character_Activate(JoltC_Character* c, JoltC_Bool lockBodies);
JOLTC_API void JoltC_Character_PostSimulation(JoltC_Character* c, float maxSeparationDistance, JoltC_Bool lockBodies);

JOLTC_API JoltC_Vec3  JoltC_Character_GetLinearVelocity(const JoltC_Character* c, JoltC_Bool lockBodies);
JOLTC_API void        JoltC_Character_SetLinearVelocity(JoltC_Character* c, JoltC_Vec3 velocity, JoltC_Bool lockBodies);
JOLTC_API void        JoltC_Character_AddLinearVelocity(JoltC_Character* c, JoltC_Vec3 velocity, JoltC_Bool lockBodies);
JOLTC_API void        JoltC_Character_AddImpulse(JoltC_Character* c, JoltC_Vec3 impulse, JoltC_Bool lockBodies);
JOLTC_API JoltC_BodyID JoltC_Character_GetBodyID(const JoltC_Character* c);

JOLTC_API JoltC_RVec3 JoltC_Character_GetPosition(const JoltC_Character* c, JoltC_Bool lockBodies);
JOLTC_API void        JoltC_Character_SetPosition(JoltC_Character* c, JoltC_RVec3 position, JoltC_Activation activation, JoltC_Bool lockBodies);
JOLTC_API JoltC_Quat  JoltC_Character_GetRotation(const JoltC_Character* c, JoltC_Bool lockBodies);
JOLTC_API void        JoltC_Character_SetRotation(JoltC_Character* c, JoltC_Quat rotation, JoltC_Activation activation, JoltC_Bool lockBodies);
JOLTC_API JoltC_RVec3 JoltC_Character_GetCenterOfMassPosition(const JoltC_Character* c, JoltC_Bool lockBodies);
JOLTC_API JoltC_ObjectLayer JoltC_Character_GetLayer(const JoltC_Character* c);
JOLTC_API void        JoltC_Character_SetLayer(JoltC_Character* c, JoltC_ObjectLayer layer, JoltC_Bool lockBodies);
JOLTC_API void        JoltC_Character_SetShape(JoltC_Character* c, const JoltC_Shape* shape, float maxPenetrationDepth, JoltC_Bool lockBodies);

/* CharacterBase accessors on Character */
JOLTC_API JoltC_GroundState JoltC_Character_GetGroundState(const JoltC_Character* c);
JOLTC_API JoltC_Bool        JoltC_Character_IsSupported(const JoltC_Character* c);
JOLTC_API JoltC_RVec3       JoltC_Character_GetGroundPosition(const JoltC_Character* c);
JOLTC_API JoltC_Vec3        JoltC_Character_GetGroundNormal(const JoltC_Character* c);
JOLTC_API JoltC_Vec3        JoltC_Character_GetGroundVelocity(const JoltC_Character* c);
JOLTC_API JoltC_BodyID      JoltC_Character_GetGroundBodyID(const JoltC_Character* c);
JOLTC_API JoltC_Vec3        JoltC_Character_GetUp(const JoltC_Character* c);
JOLTC_API void              JoltC_Character_SetUp(JoltC_Character* c, JoltC_Vec3 up);
JOLTC_API void              JoltC_Character_SetMaxSlopeAngle(JoltC_Character* c, float maxSlopeAngle);

/* -------------------------------------------------------------------------- */
/*  Additional Character methods                                              */
/* -------------------------------------------------------------------------- */
JOLTC_API void       JoltC_Character_GetPositionAndRotation(const JoltC_Character* character, JoltC_RVec3* outPosition, JoltC_Quat* outRotation);
JOLTC_API JoltC_Mat44 JoltC_Character_GetWorldTransform(const JoltC_Character* character);
JOLTC_API void       JoltC_Character_SetLinearAndAngularVelocity(JoltC_Character* character, JoltC_Vec3 linearVelocity, JoltC_Vec3 angularVelocity);
JOLTC_API void       JoltC_Character_SetPositionAndRotation(JoltC_Character* character, JoltC_RVec3 position, JoltC_Quat rotation, int activation);

/* -------------------------------------------------------------------------- */
/*  CharacterBase (polymorphic base — obtain via AsBase converters)           */
/* -------------------------------------------------------------------------- */
JOLTC_API JoltC_CharacterBase* JoltC_Character_AsBase(JoltC_Character* character);
JOLTC_API JoltC_CharacterBase* JoltC_CharacterVirtual_AsBase(JoltC_CharacterVirtual* character);
JOLTC_API void      JoltC_CharacterBase_Destroy(JoltC_CharacterBase* character);
JOLTC_API float     JoltC_CharacterBase_GetCosMaxSlopeAngle(const JoltC_CharacterBase* character);
JOLTC_API uint32_t  JoltC_CharacterBase_GetGroundBodyId(const JoltC_CharacterBase* character);
JOLTC_API JoltC_Vec3  JoltC_CharacterBase_GetGroundNormal(const JoltC_CharacterBase* character);
JOLTC_API JoltC_RVec3 JoltC_CharacterBase_GetGroundPosition(const JoltC_CharacterBase* character);
JOLTC_API int       JoltC_CharacterBase_GetGroundState(const JoltC_CharacterBase* character);
JOLTC_API uint32_t  JoltC_CharacterBase_GetGroundSubShapeId(const JoltC_CharacterBase* character);
JOLTC_API uint64_t  JoltC_CharacterBase_GetGroundUserData(const JoltC_CharacterBase* character);
JOLTC_API JoltC_Vec3  JoltC_CharacterBase_GetGroundVelocity(const JoltC_CharacterBase* character);
JOLTC_API JoltC_Vec3  JoltC_CharacterBase_GetUp(const JoltC_CharacterBase* character);
JOLTC_API int       JoltC_CharacterBase_IsSlopeTooSteep(const JoltC_CharacterBase* character, JoltC_Vec3 normal);
JOLTC_API int       JoltC_CharacterBase_IsSupported(const JoltC_CharacterBase* character);
JOLTC_API void      JoltC_CharacterBase_SetMaxSlopeAngle(JoltC_CharacterBase* character, float maxSlopeAngle);
JOLTC_API void      JoltC_CharacterBase_SetUp(JoltC_CharacterBase* character, JoltC_Vec3 up);

/* -------------------------------------------------------------------------- */
/*  CharacterSettings / CharacterVirtualSettings Init aliases                 */
/* -------------------------------------------------------------------------- */
JOLTC_API void JoltC_CharacterSettings_Init(JoltC_CharacterSettings* settings);
JOLTC_API void JoltC_CharacterVirtualSettings_Init(JoltC_CharacterVirtualSettings* settings);

/* -------------------------------------------------------------------------- */
/*  Additional CharacterVirtual methods                                       */
/* -------------------------------------------------------------------------- */
JOLTC_API int       JoltC_CharacterVirtual_CanWalkStairs(const JoltC_CharacterVirtual* character, JoltC_Vec3 linearVelocity);
JOLTC_API void      JoltC_CharacterVirtual_FinishTrackingContactChanges(JoltC_CharacterVirtual* character);
JOLTC_API void      JoltC_CharacterVirtual_GetActiveContact(const JoltC_CharacterVirtual* character, uint32_t index,
                        JoltC_Vec3* outContactNormal, JoltC_Vec3* outContactVelocity,
                        uint32_t* outBodyId2, uint32_t* outSubShapeId2);
JOLTC_API uint32_t  JoltC_CharacterVirtual_GetID(const JoltC_CharacterVirtual* character);
JOLTC_API int       JoltC_CharacterVirtual_HasCollidedWith(const JoltC_CharacterVirtual* character, uint32_t bodyId);
JOLTC_API int       JoltC_CharacterVirtual_HasCollidedWithCharacter(const JoltC_CharacterVirtual* character, uint32_t otherCharacterId);
JOLTC_API void      JoltC_CharacterVirtual_SetCharacterVsCharacterCollision(JoltC_CharacterVirtual* character, JoltC_CharacterVsCharacterCollision* collision);
JOLTC_API void      JoltC_CharacterVirtual_StartTrackingContactChanges(JoltC_CharacterVirtual* character);
JOLTC_API int       JoltC_CharacterVirtual_StickToFloor(JoltC_CharacterVirtual* character, JoltC_Vec3 stepDown, JoltC_TempAllocator* allocator);
JOLTC_API int       JoltC_CharacterVirtual_WalkStairs(JoltC_CharacterVirtual* character, float deltaTime,
                        JoltC_Vec3 stepUp, JoltC_Vec3 stepForward, JoltC_Vec3 stepForwardTest,
                        JoltC_Vec3 stepDownExtra, JoltC_TempAllocator* allocator);

/* -------------------------------------------------------------------------- */
/*  CharacterContactListener Procs (alternative to Create)                    */
/* -------------------------------------------------------------------------- */
typedef struct JoltC_CharacterContactListener_Procs {
    JoltC_OnCharacterContactValidateFn   onValidate;
    JoltC_OnCharacterContactAddedFn      onAdded;
    JoltC_OnCharacterContactPersistedFn  onPersisted;
} JoltC_CharacterContactListener_Procs;

JOLTC_API void JoltC_CharacterContactListener_SetProcs(
    JoltC_CharacterContactListener* listener,
    JoltC_CharacterContactListener_Procs procs,
    void* userData);

/* -------------------------------------------------------------------------- */
/*  CharacterVsCharacterCollision                                             */
/* -------------------------------------------------------------------------- */
JOLTC_API JoltC_CharacterVsCharacterCollision* JoltC_CharacterVsCharacterCollision_Create(void);
JOLTC_API JoltC_CharacterVsCharacterCollision* JoltC_CharacterVsCharacterCollision_CreateSimple(void);
JOLTC_API void JoltC_CharacterVsCharacterCollision_Destroy(JoltC_CharacterVsCharacterCollision* collision);
JOLTC_API void JoltC_CharacterVsCharacterCollisionSimple_AddCharacter(JoltC_CharacterVsCharacterCollision* collision, JoltC_CharacterVirtual* character);
JOLTC_API void JoltC_CharacterVsCharacterCollisionSimple_RemoveCharacter(JoltC_CharacterVsCharacterCollision* collision, JoltC_CharacterVirtual* character);

#ifdef __cplusplus
}
#endif

#endif /* JOLTC_CHARACTER_H */
