/* JoltC - C bindings for JoltPhysics
 * SPDX-License-Identifier: MIT
 *
 * Physics system, body interface, job system, and temp allocator.
 */

#ifndef JOLTC_PHYSICS_SYSTEM_H
#define JOLTC_PHYSICS_SYSTEM_H

#include <JoltC/common.h>

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */
/*  TempAllocator                                                             */
/* -------------------------------------------------------------------------- */
JOLTC_API JoltC_TempAllocator* JoltC_TempAllocator_Create(uint32_t size);
JOLTC_API void                 JoltC_TempAllocator_Destroy(JoltC_TempAllocator* allocator);

/* -------------------------------------------------------------------------- */
/*  JobSystemThreadPool                                                       */
/* -------------------------------------------------------------------------- */
JOLTC_API JoltC_JobSystem* JoltC_JobSystemThreadPool_Create(uint32_t maxJobs, uint32_t maxBarriers, int numThreads);
JOLTC_API void             JoltC_JobSystem_Destroy(JoltC_JobSystem* jobSystem);

/* -------------------------------------------------------------------------- */
/*  BroadPhaseLayerInterface (callback-based)                                 */
/* -------------------------------------------------------------------------- */
JOLTC_API JoltC_BroadPhaseLayerInterface* JoltC_BroadPhaseLayerInterface_Create(
    JoltC_GetNumBroadPhaseLayersFn getNumLayers,
    JoltC_GetBroadPhaseLayerFn     getBroadPhaseLayer,
    void*                          userData);
JOLTC_API void JoltC_BroadPhaseLayerInterface_Destroy(JoltC_BroadPhaseLayerInterface* iface);

/* -------------------------------------------------------------------------- */
/*  ObjectVsBroadPhaseLayerFilter (callback-based)                            */
/* -------------------------------------------------------------------------- */
JOLTC_API JoltC_ObjectVsBroadPhaseLayerFilter* JoltC_ObjectVsBroadPhaseLayerFilter_Create(
    JoltC_ObjectVsBroadPhaseLayerFilterFn filterFn,
    void*                                 userData);
JOLTC_API void JoltC_ObjectVsBroadPhaseLayerFilter_Destroy(JoltC_ObjectVsBroadPhaseLayerFilter* filter);

/* -------------------------------------------------------------------------- */
/*  ObjectLayerPairFilter (callback-based)                                    */
/* -------------------------------------------------------------------------- */
JOLTC_API JoltC_ObjectLayerPairFilter* JoltC_ObjectLayerPairFilter_Create(
    JoltC_ObjectLayerPairFilterFn filterFn,
    void*                         userData);
JOLTC_API void JoltC_ObjectLayerPairFilter_Destroy(JoltC_ObjectLayerPairFilter* filter);

/* -------------------------------------------------------------------------- */
/*  ContactListener (callback-based)                                          */
/* -------------------------------------------------------------------------- */
JOLTC_API JoltC_ContactListener* JoltC_ContactListener_Create(
    JoltC_OnContactValidateFn  onValidate,
    JoltC_OnContactAddedFn     onAdded,
    JoltC_OnContactPersistedFn onPersisted,
    JoltC_OnContactRemovedFn   onRemoved,
    void*                      userData);
JOLTC_API void JoltC_ContactListener_Destroy(JoltC_ContactListener* listener);

/* -------------------------------------------------------------------------- */
/*  BodyActivationListener (callback-based)                                   */
/* -------------------------------------------------------------------------- */
JOLTC_API JoltC_BodyActivationListener* JoltC_BodyActivationListener_Create(
    JoltC_OnBodyActivatedFn   onActivated,
    JoltC_OnBodyDeactivatedFn onDeactivated,
    void*                     userData);
JOLTC_API void JoltC_BodyActivationListener_Destroy(JoltC_BodyActivationListener* listener);

/* -------------------------------------------------------------------------- */
/*  PhysicsSystem                                                             */
/* -------------------------------------------------------------------------- */
JOLTC_API JoltC_PhysicsSystem* JoltC_PhysicsSystem_Create(void);
JOLTC_API void JoltC_PhysicsSystem_Destroy(JoltC_PhysicsSystem* system);

JOLTC_API void JoltC_PhysicsSystem_Init(
    JoltC_PhysicsSystem*                   system,
    uint32_t                               maxBodies,
    uint32_t                               numBodyMutexes,
    uint32_t                               maxBodyPairs,
    uint32_t                               maxContactConstraints,
    JoltC_BroadPhaseLayerInterface*        broadPhaseLayerInterface,
    JoltC_ObjectVsBroadPhaseLayerFilter*   objectVsBroadPhaseLayerFilter,
    JoltC_ObjectLayerPairFilter*           objectLayerPairFilter);

JOLTC_API void JoltC_PhysicsSystem_OptimizeBroadPhase(JoltC_PhysicsSystem* system);

JOLTC_API uint32_t JoltC_PhysicsSystem_Update(
    JoltC_PhysicsSystem* system,
    float                deltaTime,
    int                  collisionSteps,
    JoltC_TempAllocator* tempAllocator,
    JoltC_JobSystem*     jobSystem);

JOLTC_API void JoltC_PhysicsSystem_SetGravity(JoltC_PhysicsSystem* system, JoltC_Vec3 gravity);
JOLTC_API JoltC_Vec3 JoltC_PhysicsSystem_GetGravity(const JoltC_PhysicsSystem* system);

JOLTC_API void JoltC_PhysicsSystem_SetContactListener(JoltC_PhysicsSystem* system, JoltC_ContactListener* listener);
JOLTC_API void JoltC_PhysicsSystem_SetBodyActivationListener(JoltC_PhysicsSystem* system, JoltC_BodyActivationListener* listener);

JOLTC_API uint32_t JoltC_PhysicsSystem_GetNumBodies(const JoltC_PhysicsSystem* system);
JOLTC_API uint32_t JoltC_PhysicsSystem_GetNumActiveBodies(const JoltC_PhysicsSystem* system, JoltC_BodyType bodyType);
JOLTC_API uint32_t JoltC_PhysicsSystem_GetMaxBodies(const JoltC_PhysicsSystem* system);

/* -------------------------------------------------------------------------- */
/*  BodyInterface — obtained from PhysicsSystem, not owned by caller          */
/* -------------------------------------------------------------------------- */
JOLTC_API JoltC_BodyInterface* JoltC_PhysicsSystem_GetBodyInterface(JoltC_PhysicsSystem* system);
JOLTC_API JoltC_BodyInterface* JoltC_PhysicsSystem_GetBodyInterfaceNoLock(JoltC_PhysicsSystem* system);

/* -------------------------------------------------------------------------- */
/*  Additional PhysicsSystem queries                                          */
/* -------------------------------------------------------------------------- */
JOLTC_API int      JoltC_PhysicsSystem_WereBodiesInContact(const JoltC_PhysicsSystem* system, JoltC_BodyID body1, JoltC_BodyID body2);
JOLTC_API uint32_t JoltC_PhysicsSystem_GetNumConstraints(const JoltC_PhysicsSystem* system);
JOLTC_API void     JoltC_PhysicsSystem_GetBodies(const JoltC_PhysicsSystem* system, JoltC_BodyID* outIDs, uint32_t maxCount);

/* -------------------------------------------------------------------------- */
/*  ObjectLayerPairFilterTable (built-in table implementation)                */
/* -------------------------------------------------------------------------- */
JOLTC_API JoltC_ObjectLayerPairFilter* JoltC_ObjectLayerPairFilterTable_Create(uint32_t numObjectLayers);
JOLTC_API void JoltC_ObjectLayerPairFilterTable_DisableCollision(JoltC_ObjectLayerPairFilter* filter, JoltC_ObjectLayer layer1, JoltC_ObjectLayer layer2);
JOLTC_API void JoltC_ObjectLayerPairFilterTable_EnableCollision(JoltC_ObjectLayerPairFilter* filter, JoltC_ObjectLayer layer1, JoltC_ObjectLayer layer2);
JOLTC_API JoltC_Bool JoltC_ObjectLayerPairFilterTable_ShouldCollide(const JoltC_ObjectLayerPairFilter* filter, JoltC_ObjectLayer layer1, JoltC_ObjectLayer layer2);

/* -------------------------------------------------------------------------- */
/*  ObjectLayerPairFilterMask (built-in mask implementation)                  */
/* -------------------------------------------------------------------------- */
JOLTC_API JoltC_ObjectLayerPairFilter* JoltC_ObjectLayerPairFilterMask_Create(void);
JOLTC_API JoltC_ObjectLayer JoltC_ObjectLayerPairFilterMask_GetObjectLayer(uint32_t group, uint32_t mask);
JOLTC_API uint32_t JoltC_ObjectLayerPairFilterMask_GetGroup(JoltC_ObjectLayer layer);
JOLTC_API uint32_t JoltC_ObjectLayerPairFilterMask_GetMask(JoltC_ObjectLayer layer);

/* -------------------------------------------------------------------------- */
/*  BroadPhaseLayerInterfaceTable (built-in table implementation)             */
/* -------------------------------------------------------------------------- */
JOLTC_API JoltC_BroadPhaseLayerInterface* JoltC_BroadPhaseLayerInterfaceTable_Create(uint32_t numObjectLayers, uint32_t numBroadPhaseLayers);
JOLTC_API void JoltC_BroadPhaseLayerInterfaceTable_MapObjectToBroadPhaseLayer(JoltC_BroadPhaseLayerInterface* iface, JoltC_ObjectLayer objectLayer, JoltC_BroadPhaseLayer broadPhaseLayer);

/* -------------------------------------------------------------------------- */
/*  BroadPhaseLayerInterfaceMask (built-in mask implementation)               */
/* -------------------------------------------------------------------------- */
JOLTC_API JoltC_BroadPhaseLayerInterface* JoltC_BroadPhaseLayerInterfaceMask_Create(uint32_t numBroadPhaseLayers);
JOLTC_API void JoltC_BroadPhaseLayerInterfaceMask_ConfigureLayer(JoltC_BroadPhaseLayerInterface* iface, JoltC_BroadPhaseLayer broadPhaseLayer, uint32_t groupsToInclude, uint32_t groupsToExclude);

/* -------------------------------------------------------------------------- */
/*  GroupFilter / GroupFilterTable                                            */
/* -------------------------------------------------------------------------- */
JOLTC_API void JoltC_GroupFilter_Destroy(JoltC_GroupFilter* filter);
JOLTC_API JoltC_Bool JoltC_GroupFilter_CanCollide(const JoltC_GroupFilter* filter, const JoltC_CollisionGroup* group1, const JoltC_CollisionGroup* group2);

JOLTC_API JoltC_GroupFilter* JoltC_GroupFilterTable_Create(uint32_t numSubGroups);
JOLTC_API void JoltC_GroupFilterTable_DisableCollision(JoltC_GroupFilter* filter, JoltC_CollisionSubGroupID subGroup1, JoltC_CollisionSubGroupID subGroup2);
JOLTC_API void JoltC_GroupFilterTable_EnableCollision(JoltC_GroupFilter* filter, JoltC_CollisionSubGroupID subGroup1, JoltC_CollisionSubGroupID subGroup2);
JOLTC_API JoltC_Bool JoltC_GroupFilterTable_IsCollisionEnabled(const JoltC_GroupFilter* filter, JoltC_CollisionSubGroupID subGroup1, JoltC_CollisionSubGroupID subGroup2);

/* -------------------------------------------------------------------------- */
/*  PhysicsMaterial                                                           */
/* -------------------------------------------------------------------------- */
JOLTC_API JoltC_PhysicsMaterial* JoltC_PhysicsMaterial_Create(const char* name, uint32_t color);
JOLTC_API void JoltC_PhysicsMaterial_Destroy(JoltC_PhysicsMaterial* material);
JOLTC_API const char* JoltC_PhysicsMaterial_GetDebugName(const JoltC_PhysicsMaterial* material);
JOLTC_API uint32_t JoltC_PhysicsMaterial_GetDebugColor(const JoltC_PhysicsMaterial* material);

/* -------------------------------------------------------------------------- */
/*  PhysicsStepListener                                                       */
/* -------------------------------------------------------------------------- */
JOLTC_API JoltC_PhysicsStepListener* JoltC_PhysicsStepListener_Create(JoltC_OnPhysicsStepFn fn, void* userData);
JOLTC_API void JoltC_PhysicsStepListener_Destroy(JoltC_PhysicsStepListener* listener);
JOLTC_API void JoltC_PhysicsSystem_AddStepListener(JoltC_PhysicsSystem* system, JoltC_PhysicsStepListener* listener);
JOLTC_API void JoltC_PhysicsSystem_RemoveStepListener(JoltC_PhysicsSystem* system, JoltC_PhysicsStepListener* listener);

/* -------------------------------------------------------------------------- */
/*  SimShapeFilter on PhysicsSystem                                           */
/* -------------------------------------------------------------------------- */
JOLTC_API void JoltC_PhysicsSystem_SetSimShapeFilter(JoltC_PhysicsSystem* system, const JoltC_SimShapeFilter* filter);

/* -------------------------------------------------------------------------- */
/*  Enhanced ContactListener (with Body/ContactManifold/ContactSettings)      */
/* -------------------------------------------------------------------------- */
JOLTC_API JoltC_ContactListener* JoltC_ContactListener_CreateEnhanced(
    JoltC_OnContactValidateEnhancedFn  onValidate,
    JoltC_OnContactAddedEnhancedFn     onAdded,
    JoltC_OnContactPersistedEnhancedFn onPersisted,
    JoltC_OnContactRemovedEnhancedFn   onRemoved,
    void*                              userData);

/* -------------------------------------------------------------------------- */
/*  PhysicsSystem — constraints                                               */
/* -------------------------------------------------------------------------- */
JOLTC_API void JoltC_PhysicsSystem_AddConstraints(JoltC_PhysicsSystem* system, JoltC_Constraint** constraints, int count);
JOLTC_API void JoltC_PhysicsSystem_RemoveConstraints(JoltC_PhysicsSystem* system, JoltC_Constraint** constraints, int count);
JOLTC_API int  JoltC_PhysicsSystem_GetConstraints(const JoltC_PhysicsSystem* system, JoltC_Constraint** outConstraints, int maxCount);

/* -------------------------------------------------------------------------- */
/*  PhysicsSystem — settings                                                  */
/* -------------------------------------------------------------------------- */
JOLTC_API void JoltC_PhysicsSystem_GetPhysicsSettings(const JoltC_PhysicsSystem* system, JoltC_PhysicsSettings* outSettings);
JOLTC_API void JoltC_PhysicsSystem_SetPhysicsSettings(JoltC_PhysicsSystem* system, const JoltC_PhysicsSettings* settings);

/* -------------------------------------------------------------------------- */
/*  PhysicsSystem — activate bodies in AABB                                   */
/* -------------------------------------------------------------------------- */
JOLTC_API void JoltC_PhysicsSystem_ActivateBodiesInAABox(JoltC_PhysicsSystem* system, JoltC_Vec3 min, JoltC_Vec3 max);

/* -------------------------------------------------------------------------- */
/*  Callback SetProcs — update function pointers on existing objects          */
/* -------------------------------------------------------------------------- */
JOLTC_API void JoltC_BodyActivationListener_SetProcs(JoltC_BodyActivationListener* listener, JoltC_BodyActivationListener_Procs procs, void* userData);
JOLTC_API void JoltC_ContactListener_SetProcs(JoltC_ContactListener* listener, JoltC_ContactListener_Procs procs, void* userData);
JOLTC_API void JoltC_PhysicsStepListener_SetProcs(JoltC_PhysicsStepListener* listener, JoltC_PhysicsStepListener_Procs procs, void* userData);

/* -------------------------------------------------------------------------- */
/*  ObjectVsBroadPhaseLayerFilterTable (built-in table implementation)        */
/* -------------------------------------------------------------------------- */
JOLTC_API JoltC_ObjectVsBroadPhaseLayerFilter* JoltC_ObjectVsBroadPhaseLayerFilterTable_Create(
    const JoltC_BroadPhaseLayerInterface* bpInterface,
    uint32_t                              numBroadPhaseLayers,
    const JoltC_ObjectLayerPairFilter*    objectFilter,
    uint32_t                              numObjectLayers);

/* -------------------------------------------------------------------------- */
/*  ObjectVsBroadPhaseLayerFilterMask (built-in mask implementation)          */
/* -------------------------------------------------------------------------- */
JOLTC_API JoltC_ObjectVsBroadPhaseLayerFilter* JoltC_ObjectVsBroadPhaseLayerFilterMask_Create(
    const JoltC_BroadPhaseLayerInterface* bpInterface);

/* -------------------------------------------------------------------------- */
/*  SoftBodyCreationSettings                                                  */
/* -------------------------------------------------------------------------- */
JOLTC_API JoltC_SoftBodyCreationSettings* JoltC_SoftBodyCreationSettings_Create(void);
JOLTC_API void JoltC_SoftBodyCreationSettings_Destroy(JoltC_SoftBodyCreationSettings* settings);

#ifdef __cplusplus
}
#endif

#endif /* JOLTC_PHYSICS_SYSTEM_H */
