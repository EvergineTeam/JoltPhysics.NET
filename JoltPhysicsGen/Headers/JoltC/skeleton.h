/* JoltC - C bindings for JoltPhysics
 * SPDX-License-Identifier: MIT
 *
 * Skeleton, SkeletonPose, SkeletalAnimation, SkeletonMapper, Ragdoll.
 */

#ifndef JOLTC_SKELETON_H
#define JOLTC_SKELETON_H

#include <JoltC/common.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*  Skeleton                                                                  */
/* ========================================================================== */
JOLTC_API JoltC_Skeleton* JoltC_Skeleton_Create(void);
JOLTC_API void     JoltC_Skeleton_Destroy(JoltC_Skeleton* skeleton);
JOLTC_API uint32_t JoltC_Skeleton_AddJoint(JoltC_Skeleton* skeleton, const char* name);
JOLTC_API uint32_t JoltC_Skeleton_AddJoint2(JoltC_Skeleton* skeleton, const char* name, int parentIndex);
JOLTC_API uint32_t JoltC_Skeleton_AddJoint3(JoltC_Skeleton* skeleton, const char* name, const char* parentName);
JOLTC_API int      JoltC_Skeleton_GetJointCount(const JoltC_Skeleton* skeleton);
JOLTC_API void     JoltC_Skeleton_GetJoint(const JoltC_Skeleton* skeleton, int index, JoltC_SkeletonJoint* joint);
JOLTC_API int      JoltC_Skeleton_GetJointIndex(const JoltC_Skeleton* skeleton, const char* name);
JOLTC_API void     JoltC_Skeleton_CalculateParentJointIndices(JoltC_Skeleton* skeleton);
JOLTC_API JoltC_Bool JoltC_Skeleton_AreJointsCorrectlyOrdered(const JoltC_Skeleton* skeleton);

/* ========================================================================== */
/*  SkeletonPose                                                              */
/* ========================================================================== */
JOLTC_API JoltC_SkeletonPose* JoltC_SkeletonPose_Create(void);
JOLTC_API void     JoltC_SkeletonPose_Destroy(JoltC_SkeletonPose* pose);
JOLTC_API void     JoltC_SkeletonPose_SetSkeleton(JoltC_SkeletonPose* pose, const JoltC_Skeleton* skeleton);
JOLTC_API const JoltC_Skeleton* JoltC_SkeletonPose_GetSkeleton(const JoltC_SkeletonPose* pose);
JOLTC_API void     JoltC_SkeletonPose_SetRootOffset(JoltC_SkeletonPose* pose, JoltC_RVec3 offset);
JOLTC_API JoltC_RVec3 JoltC_SkeletonPose_GetRootOffset(const JoltC_SkeletonPose* pose);
JOLTC_API int      JoltC_SkeletonPose_GetJointCount(const JoltC_SkeletonPose* pose);
JOLTC_API void     JoltC_SkeletonPose_GetJointState(const JoltC_SkeletonPose* pose, int index, JoltC_Vec3* outTranslation, JoltC_Quat* outRotation);
JOLTC_API void     JoltC_SkeletonPose_SetJointState(JoltC_SkeletonPose* pose, int index, JoltC_Vec3 translation, JoltC_Quat rotation);
JOLTC_API void     JoltC_SkeletonPose_GetJointMatrix(const JoltC_SkeletonPose* pose, int index, JoltC_Mat44* result);
JOLTC_API void     JoltC_SkeletonPose_SetJointMatrix(JoltC_SkeletonPose* pose, int index, const JoltC_Mat44* matrix);
JOLTC_API void     JoltC_SkeletonPose_GetJointMatrices(const JoltC_SkeletonPose* pose, JoltC_Mat44* outMatrices, int count);
JOLTC_API void     JoltC_SkeletonPose_SetJointMatrices(JoltC_SkeletonPose* pose, const JoltC_Mat44* matrices, int count);
JOLTC_API void     JoltC_SkeletonPose_CalculateJointMatrices(JoltC_SkeletonPose* pose);
JOLTC_API void     JoltC_SkeletonPose_CalculateJointStates(JoltC_SkeletonPose* pose);
JOLTC_API void     JoltC_SkeletonPose_CalculateLocalSpaceJointMatrices(const JoltC_SkeletonPose* pose, JoltC_Mat44* outMatrices);

/* ========================================================================== */
/*  SkeletalAnimation                                                         */
/* ========================================================================== */
JOLTC_API JoltC_SkeletalAnimation* JoltC_SkeletalAnimation_Create(void);
JOLTC_API void     JoltC_SkeletalAnimation_Destroy(JoltC_SkeletalAnimation* anim);
JOLTC_API float    JoltC_SkeletalAnimation_GetDuration(const JoltC_SkeletalAnimation* anim);
JOLTC_API JoltC_Bool JoltC_SkeletalAnimation_IsLooping(const JoltC_SkeletalAnimation* anim);
JOLTC_API void     JoltC_SkeletalAnimation_SetIsLooping(JoltC_SkeletalAnimation* anim, JoltC_Bool looping);
JOLTC_API void     JoltC_SkeletalAnimation_ScaleJoints(JoltC_SkeletalAnimation* anim, float scale);
JOLTC_API void     JoltC_SkeletalAnimation_Sample(const JoltC_SkeletalAnimation* anim, float time, JoltC_SkeletonPose* pose);
JOLTC_API int      JoltC_SkeletalAnimation_GetAnimatedJointCount(const JoltC_SkeletalAnimation* anim);
JOLTC_API void     JoltC_SkeletalAnimation_AddAnimatedJoint(JoltC_SkeletalAnimation* anim, const char* jointName);
JOLTC_API void     JoltC_SkeletalAnimation_AddKeyframe(JoltC_SkeletalAnimation* anim, int jointIndex, float time, JoltC_Vec3 translation, JoltC_Quat rotation);

/* ========================================================================== */
/*  SkeletonMapper                                                            */
/* ========================================================================== */
JOLTC_API JoltC_SkeletonMapper* JoltC_SkeletonMapper_Create(void);
JOLTC_API void     JoltC_SkeletonMapper_Destroy(JoltC_SkeletonMapper* mapper);
JOLTC_API void     JoltC_SkeletonMapper_Initialize(JoltC_SkeletonMapper* mapper, const JoltC_Skeleton* sk1, const JoltC_Mat44* neutralPose1, const JoltC_Skeleton* sk2, const JoltC_Mat44* neutralPose2);
JOLTC_API void     JoltC_SkeletonMapper_LockAllTranslations(JoltC_SkeletonMapper* mapper, const JoltC_Skeleton* sk2, const JoltC_Mat44* neutralPose2);
JOLTC_API void     JoltC_SkeletonMapper_LockTranslations(JoltC_SkeletonMapper* mapper, const JoltC_Skeleton* sk2, const JoltC_Bool* lockedTranslations, const JoltC_Mat44* neutralPose2);
JOLTC_API void     JoltC_SkeletonMapper_Map(const JoltC_SkeletonMapper* mapper, const JoltC_Mat44* pose1ModelSpace, const JoltC_Mat44* pose2LocalSpace, JoltC_Mat44* outPose2ModelSpace);
JOLTC_API void     JoltC_SkeletonMapper_MapReverse(const JoltC_SkeletonMapper* mapper, const JoltC_Mat44* pose2ModelSpace, JoltC_Mat44* outPose1ModelSpace);
JOLTC_API int      JoltC_SkeletonMapper_GetMappedJointIndex(const JoltC_SkeletonMapper* mapper, int joint1Index);
JOLTC_API JoltC_Bool JoltC_SkeletonMapper_IsJointTranslationLocked(const JoltC_SkeletonMapper* mapper, int joint2Index);

/* ========================================================================== */
/*  RagdollSettings                                                           */
/* ========================================================================== */
JOLTC_API JoltC_RagdollSettings* JoltC_RagdollSettings_Create(void);
JOLTC_API void     JoltC_RagdollSettings_Destroy(JoltC_RagdollSettings* settings);
JOLTC_API const JoltC_Skeleton* JoltC_RagdollSettings_GetSkeleton(const JoltC_RagdollSettings* s);
JOLTC_API void     JoltC_RagdollSettings_SetSkeleton(JoltC_RagdollSettings* s, JoltC_Skeleton* skeleton);
JOLTC_API JoltC_Bool JoltC_RagdollSettings_Stabilize(JoltC_RagdollSettings* s);
JOLTC_API void     JoltC_RagdollSettings_DisableParentChildCollisions(JoltC_RagdollSettings* s, const JoltC_Mat44* jointMatrices, float minSeparationDistance);
JOLTC_API void     JoltC_RagdollSettings_CalculateBodyIndexToConstraintIndex(JoltC_RagdollSettings* s);
JOLTC_API int      JoltC_RagdollSettings_GetConstraintIndexForBodyIndex(JoltC_RagdollSettings* s, int bodyIndex);
JOLTC_API void     JoltC_RagdollSettings_CalculateConstraintIndexToBodyIdxPair(JoltC_RagdollSettings* s);
JOLTC_API void     JoltC_RagdollSettings_ResizeParts(JoltC_RagdollSettings* s, int count);
JOLTC_API int      JoltC_RagdollSettings_GetPartCount(const JoltC_RagdollSettings* s);
JOLTC_API void     JoltC_RagdollSettings_SetPartShape(JoltC_RagdollSettings* s, int partIndex, const JoltC_Shape* shape);
JOLTC_API void     JoltC_RagdollSettings_SetPartPosition(JoltC_RagdollSettings* s, int partIndex, JoltC_RVec3 position);
JOLTC_API void     JoltC_RagdollSettings_SetPartRotation(JoltC_RagdollSettings* s, int partIndex, JoltC_Quat rotation);
JOLTC_API void     JoltC_RagdollSettings_SetPartMotionType(JoltC_RagdollSettings* s, int partIndex, JoltC_MotionType motionType);
JOLTC_API void     JoltC_RagdollSettings_SetPartObjectLayer(JoltC_RagdollSettings* s, int partIndex, JoltC_ObjectLayer layer);
JOLTC_API void     JoltC_RagdollSettings_SetPartMassProperties(JoltC_RagdollSettings* s, int partIndex, float mass);
JOLTC_API void     JoltC_RagdollSettings_SetPartToParent(JoltC_RagdollSettings* settings, int partIndex, int parentIndex, const JoltC_TwoBodyConstraintSettings* constraintSettings);
JOLTC_API JoltC_Ragdoll* JoltC_RagdollSettings_CreateRagdoll(JoltC_RagdollSettings* s, JoltC_PhysicsSystem* system, JoltC_CollisionGroupID collisionGroup, uint64_t userData);

/* ========================================================================== */
/*  Ragdoll                                                                   */
/* ========================================================================== */
JOLTC_API void     JoltC_Ragdoll_Destroy(JoltC_Ragdoll* ragdoll);
JOLTC_API void     JoltC_Ragdoll_AddToPhysicsSystem(JoltC_Ragdoll* ragdoll, JoltC_Activation activation, JoltC_Bool lockBodies);
JOLTC_API void     JoltC_Ragdoll_RemoveFromPhysicsSystem(JoltC_Ragdoll* ragdoll, JoltC_Bool lockBodies);
JOLTC_API void     JoltC_Ragdoll_Activate(JoltC_Ragdoll* ragdoll, JoltC_Bool lockBodies);
JOLTC_API JoltC_Bool JoltC_Ragdoll_IsActive(const JoltC_Ragdoll* ragdoll, JoltC_Bool lockBodies);
JOLTC_API void     JoltC_Ragdoll_ResetWarmStart(JoltC_Ragdoll* ragdoll);
JOLTC_API void     JoltC_Ragdoll_SetPose(JoltC_Ragdoll* ragdoll, const JoltC_SkeletonPose* pose, JoltC_Bool lockBodies);
JOLTC_API void     JoltC_Ragdoll_SetPose2(JoltC_Ragdoll* ragdoll, JoltC_RVec3 rootOffset, const JoltC_Mat44* jointMatrices, JoltC_Bool lockBodies);
JOLTC_API void     JoltC_Ragdoll_GetPose(const JoltC_Ragdoll* ragdoll, JoltC_SkeletonPose* outPose, JoltC_Bool lockBodies);
JOLTC_API void     JoltC_Ragdoll_GetPose2(const JoltC_Ragdoll* ragdoll, JoltC_RVec3* outRootOffset, JoltC_Mat44* outJointMatrices, JoltC_Bool lockBodies);
JOLTC_API void     JoltC_Ragdoll_DriveToPoseUsingMotors(JoltC_Ragdoll* ragdoll, const JoltC_SkeletonPose* pose);
JOLTC_API void     JoltC_Ragdoll_DriveToPoseUsingKinematics(JoltC_Ragdoll* ragdoll, const JoltC_SkeletonPose* pose, float deltaTime, JoltC_Bool lockBodies);
JOLTC_API void     JoltC_Ragdoll_GetRootTransform(const JoltC_Ragdoll* ragdoll, JoltC_RVec3* outPosition, JoltC_Quat* outRotation, JoltC_Bool lockBodies);
JOLTC_API int      JoltC_Ragdoll_GetBodyCount(const JoltC_Ragdoll* ragdoll);
JOLTC_API JoltC_BodyID JoltC_Ragdoll_GetBodyID(const JoltC_Ragdoll* ragdoll, int bodyIndex);
JOLTC_API int      JoltC_Ragdoll_GetConstraintCount(const JoltC_Ragdoll* ragdoll);
JOLTC_API JoltC_Constraint* JoltC_Ragdoll_GetConstraint(JoltC_Ragdoll* ragdoll, int constraintIndex);
JOLTC_API const JoltC_RagdollSettings* JoltC_Ragdoll_GetRagdollSettings(const JoltC_Ragdoll* ragdoll);

#ifdef __cplusplus
}
#endif

#endif /* JOLTC_SKELETON_H */
