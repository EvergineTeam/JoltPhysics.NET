/* JoltC - C bindings for JoltPhysics
 * SPDX-License-Identifier: MIT
 *
 * Constraint creation and management.
 */

#ifndef JOLTC_CONSTRAINT_H
#define JOLTC_CONSTRAINT_H

#include <JoltC/common.h>

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */
/*  Constraint base — ref-counted                                             */
/* -------------------------------------------------------------------------- */
JOLTC_API void                    JoltC_Constraint_AddRef(JoltC_Constraint* constraint);
JOLTC_API void                    JoltC_Constraint_Release(JoltC_Constraint* constraint);
JOLTC_API void                    JoltC_Constraint_Destroy(JoltC_Constraint* constraint);
JOLTC_API JoltC_ConstraintType    JoltC_Constraint_GetType(const JoltC_Constraint* constraint);
JOLTC_API JoltC_ConstraintSubType JoltC_Constraint_GetSubType(const JoltC_Constraint* constraint);
JOLTC_API void                    JoltC_Constraint_SetEnabled(JoltC_Constraint* constraint, JoltC_Bool enabled);
JOLTC_API JoltC_Bool              JoltC_Constraint_GetEnabled(const JoltC_Constraint* constraint);
JOLTC_API JoltC_Bool              JoltC_Constraint_IsActive(const JoltC_Constraint* constraint);
JOLTC_API void                    JoltC_Constraint_SetUserData(JoltC_Constraint* constraint, uint64_t userData);
JOLTC_API uint64_t                JoltC_Constraint_GetUserData(const JoltC_Constraint* constraint);
JOLTC_API uint32_t                JoltC_Constraint_GetConstraintPriority(const JoltC_Constraint* constraint);
JOLTC_API void                    JoltC_Constraint_SetConstraintPriority(JoltC_Constraint* constraint, uint32_t priority);
JOLTC_API uint32_t                JoltC_Constraint_GetNumVelocityStepsOverride(const JoltC_Constraint* constraint);
JOLTC_API void                    JoltC_Constraint_SetNumVelocityStepsOverride(JoltC_Constraint* constraint, uint32_t steps);
JOLTC_API uint32_t                JoltC_Constraint_GetNumPositionStepsOverride(const JoltC_Constraint* constraint);
JOLTC_API void                    JoltC_Constraint_SetNumPositionStepsOverride(JoltC_Constraint* constraint, uint32_t steps);
JOLTC_API void                    JoltC_Constraint_NotifyShapeChanged(JoltC_Constraint* constraint, JoltC_BodyID bodyID, JoltC_Vec3 deltaCOM);
JOLTC_API void                    JoltC_Constraint_ResetWarmStart(JoltC_Constraint* constraint);
JOLTC_API void                    JoltC_Constraint_SetupVelocityConstraint(JoltC_Constraint* constraint, float deltaTime);
JOLTC_API void                    JoltC_Constraint_WarmStartVelocityConstraint(JoltC_Constraint* constraint, float warmStartImpulseRatio);
JOLTC_API JoltC_Bool              JoltC_Constraint_SolveVelocityConstraint(JoltC_Constraint* constraint, float deltaTime);
JOLTC_API JoltC_Bool              JoltC_Constraint_SolvePositionConstraint(JoltC_Constraint* constraint, float deltaTime, float baumgarte);

/* TwoBodyConstraint */
JOLTC_API JoltC_Body*             JoltC_TwoBodyConstraint_GetBody1(JoltC_Constraint* constraint);
JOLTC_API JoltC_Body*             JoltC_TwoBodyConstraint_GetBody2(JoltC_Constraint* constraint);
JOLTC_API void                    JoltC_TwoBodyConstraint_GetConstraintToBody1Matrix(const JoltC_Constraint* constraint, JoltC_Mat44* result);
JOLTC_API void                    JoltC_TwoBodyConstraint_GetConstraintToBody2Matrix(const JoltC_Constraint* constraint, JoltC_Mat44* result);

/* -------------------------------------------------------------------------- */
/*  PhysicsSystem constraint management                                       */
/* -------------------------------------------------------------------------- */
JOLTC_API void JoltC_PhysicsSystem_AddConstraint(JoltC_PhysicsSystem* system, JoltC_Constraint* constraint);
JOLTC_API void JoltC_PhysicsSystem_RemoveConstraint(JoltC_PhysicsSystem* system, JoltC_Constraint* constraint);

/* -------------------------------------------------------------------------- */
/*  PointConstraint                                                           */
/* -------------------------------------------------------------------------- */
typedef struct JoltC_PointConstraintSettings {
    JoltC_ConstraintSpace space;
    JoltC_RVec3           point1;
    JoltC_RVec3           point2;
} JoltC_PointConstraintSettings;

JOLTC_API JoltC_Constraint* JoltC_PointConstraint_Create(
    JoltC_PhysicsSystem*                  system,
    JoltC_BodyID                          body1,
    JoltC_BodyID                          body2,
    const JoltC_PointConstraintSettings*  settings);

JOLTC_API void      JoltC_PointConstraint_SetPoint1(JoltC_Constraint* c, JoltC_ConstraintSpace space, JoltC_RVec3 point);
JOLTC_API void      JoltC_PointConstraint_SetPoint2(JoltC_Constraint* c, JoltC_ConstraintSpace space, JoltC_RVec3 point);
JOLTC_API JoltC_Vec3 JoltC_PointConstraint_GetLocalSpacePoint1(const JoltC_Constraint* c);
JOLTC_API JoltC_Vec3 JoltC_PointConstraint_GetLocalSpacePoint2(const JoltC_Constraint* c);
JOLTC_API JoltC_Vec3 JoltC_PointConstraint_GetTotalLambdaPosition(const JoltC_Constraint* c);
JOLTC_API void       JoltC_PointConstraintSettings_Init(JoltC_PointConstraintSettings* settings);
JOLTC_API JoltC_Constraint* JoltC_PointConstraint_GetSettings(const JoltC_Constraint* c);

/* -------------------------------------------------------------------------- */
/*  FixedConstraint                                                           */
/* -------------------------------------------------------------------------- */
typedef struct JoltC_FixedConstraintSettings {
    JoltC_ConstraintSpace space;
    JoltC_Bool            autoDetectPoint;
    JoltC_RVec3           point1;
    JoltC_Vec3            axisX1;
    JoltC_Vec3            axisY1;
    JoltC_RVec3           point2;
    JoltC_Vec3            axisX2;
    JoltC_Vec3            axisY2;
} JoltC_FixedConstraintSettings;

JOLTC_API JoltC_Constraint* JoltC_FixedConstraint_Create(
    JoltC_PhysicsSystem*                  system,
    JoltC_BodyID                          body1,
    JoltC_BodyID                          body2,
    const JoltC_FixedConstraintSettings*  settings);

JOLTC_API void JoltC_FixedConstraintSettings_Init(JoltC_FixedConstraintSettings* settings);
JOLTC_API JoltC_Vec3 JoltC_FixedConstraint_GetTotalLambdaPosition(const JoltC_Constraint* c);
JOLTC_API JoltC_Vec3 JoltC_FixedConstraint_GetTotalLambdaRotation(const JoltC_Constraint* c);
JOLTC_API JoltC_Constraint* JoltC_FixedConstraint_GetSettings(const JoltC_Constraint* c);

/* -------------------------------------------------------------------------- */
/*  DistanceConstraint                                                        */
/* -------------------------------------------------------------------------- */
typedef struct JoltC_DistanceConstraintSettings {
    JoltC_ConstraintSpace space;
    JoltC_RVec3           point1;
    JoltC_RVec3           point2;
    float                 minDistance;
    float                 maxDistance;
    JoltC_SpringSettings  limitsSpringSettings;
} JoltC_DistanceConstraintSettings;

JOLTC_API JoltC_Constraint* JoltC_DistanceConstraint_Create(
    JoltC_PhysicsSystem*                     system,
    JoltC_BodyID                             body1,
    JoltC_BodyID                             body2,
    const JoltC_DistanceConstraintSettings*  settings);

JOLTC_API void  JoltC_DistanceConstraint_SetDistance(JoltC_Constraint* c, float minDist, float maxDist);
JOLTC_API float JoltC_DistanceConstraint_GetMinDistance(const JoltC_Constraint* c);
JOLTC_API float JoltC_DistanceConstraint_GetMaxDistance(const JoltC_Constraint* c);
JOLTC_API JoltC_SpringSettings JoltC_DistanceConstraint_GetLimitsSpringSettings(const JoltC_Constraint* c);
JOLTC_API void  JoltC_DistanceConstraint_SetLimitsSpringSettings(JoltC_Constraint* c, JoltC_SpringSettings settings);
JOLTC_API JoltC_Vec3 JoltC_DistanceConstraint_GetTotalLambdaPosition(const JoltC_Constraint* c);
JOLTC_API void  JoltC_DistanceConstraintSettings_Init(JoltC_DistanceConstraintSettings* settings);
JOLTC_API JoltC_Constraint* JoltC_DistanceConstraint_GetSettings(const JoltC_Constraint* c);

/* -------------------------------------------------------------------------- */
/*  HingeConstraint                                                           */
/* -------------------------------------------------------------------------- */
typedef struct JoltC_HingeConstraintSettings {
    JoltC_ConstraintSpace space;
    JoltC_RVec3           point1;
    JoltC_Vec3            hingeAxis1;
    JoltC_Vec3            normalAxis1;
    JoltC_RVec3           point2;
    JoltC_Vec3            hingeAxis2;
    JoltC_Vec3            normalAxis2;
    float                 limitsMin;
    float                 limitsMax;
    JoltC_SpringSettings  limitsSpringSettings;
    float                 maxFrictionTorque;
    JoltC_MotorSettings   motorSettings;
} JoltC_HingeConstraintSettings;

JOLTC_API JoltC_Constraint* JoltC_HingeConstraint_Create(
    JoltC_PhysicsSystem*                   system,
    JoltC_BodyID                           body1,
    JoltC_BodyID                           body2,
    const JoltC_HingeConstraintSettings*   settings);

JOLTC_API float JoltC_HingeConstraint_GetCurrentAngle(const JoltC_Constraint* c);
JOLTC_API void  JoltC_HingeConstraint_SetMotorState(JoltC_Constraint* c, JoltC_MotorState state);
JOLTC_API JoltC_MotorState JoltC_HingeConstraint_GetMotorState(const JoltC_Constraint* c);
JOLTC_API void  JoltC_HingeConstraint_SetTargetAngularVelocity(JoltC_Constraint* c, float velocity);
JOLTC_API float JoltC_HingeConstraint_GetTargetAngularVelocity(const JoltC_Constraint* c);
JOLTC_API void  JoltC_HingeConstraint_SetTargetAngle(JoltC_Constraint* c, float angle);
JOLTC_API float JoltC_HingeConstraint_GetTargetAngle(const JoltC_Constraint* c);
JOLTC_API void  JoltC_HingeConstraint_SetLimits(JoltC_Constraint* c, float min, float max);
JOLTC_API float JoltC_HingeConstraint_GetLimitsMin(const JoltC_Constraint* c);
JOLTC_API float JoltC_HingeConstraint_GetLimitsMax(const JoltC_Constraint* c);
JOLTC_API JoltC_Bool JoltC_HingeConstraint_HasLimits(const JoltC_Constraint* c);
JOLTC_API void  JoltC_HingeConstraint_SetMaxFrictionTorque(JoltC_Constraint* c, float torque);
JOLTC_API float JoltC_HingeConstraint_GetMaxFrictionTorque(const JoltC_Constraint* c);
JOLTC_API JoltC_SpringSettings JoltC_HingeConstraint_GetLimitsSpringSettings(const JoltC_Constraint* c);
JOLTC_API void  JoltC_HingeConstraint_SetLimitsSpringSettings(JoltC_Constraint* c, JoltC_SpringSettings settings);
JOLTC_API JoltC_MotorSettings JoltC_HingeConstraint_GetMotorSettings(const JoltC_Constraint* c);
JOLTC_API void  JoltC_HingeConstraint_SetMotorSettings(JoltC_Constraint* c, JoltC_MotorSettings settings);
JOLTC_API JoltC_Vec3  JoltC_HingeConstraint_GetLocalSpacePoint1(const JoltC_Constraint* c);
JOLTC_API JoltC_Vec3  JoltC_HingeConstraint_GetLocalSpacePoint2(const JoltC_Constraint* c);
JOLTC_API JoltC_Vec3  JoltC_HingeConstraint_GetLocalSpaceHingeAxis1(const JoltC_Constraint* c);
JOLTC_API JoltC_Vec3  JoltC_HingeConstraint_GetLocalSpaceHingeAxis2(const JoltC_Constraint* c);
JOLTC_API JoltC_Vec3  JoltC_HingeConstraint_GetLocalSpaceNormalAxis1(const JoltC_Constraint* c);
JOLTC_API JoltC_Vec3  JoltC_HingeConstraint_GetLocalSpaceNormalAxis2(const JoltC_Constraint* c);
JOLTC_API JoltC_Vec3  JoltC_HingeConstraint_GetTotalLambdaPosition(const JoltC_Constraint* c);
JOLTC_API JoltC_Vec2  JoltC_HingeConstraint_GetTotalLambdaRotation(const JoltC_Constraint* c);
JOLTC_API float       JoltC_HingeConstraint_GetTotalLambdaRotationLimits(const JoltC_Constraint* c);
JOLTC_API float       JoltC_HingeConstraint_GetTotalLambdaMotor(const JoltC_Constraint* c);
JOLTC_API void        JoltC_HingeConstraintSettings_Init(JoltC_HingeConstraintSettings* settings);
JOLTC_API JoltC_Constraint* JoltC_HingeConstraint_GetSettings(const JoltC_Constraint* c);

/* -------------------------------------------------------------------------- */
/*  SliderConstraint                                                          */
/* -------------------------------------------------------------------------- */
typedef struct JoltC_SliderConstraintSettings {
    JoltC_ConstraintSpace space;
    JoltC_Bool            autoDetectPoint;
    JoltC_RVec3           point1;
    JoltC_Vec3            sliderAxis1;
    JoltC_Vec3            normalAxis1;
    JoltC_RVec3           point2;
    JoltC_Vec3            sliderAxis2;
    JoltC_Vec3            normalAxis2;
    float                 limitsMin;
    float                 limitsMax;
    JoltC_SpringSettings  limitsSpringSettings;
    float                 maxFrictionForce;
    JoltC_MotorSettings   motorSettings;
} JoltC_SliderConstraintSettings;

JOLTC_API JoltC_Constraint* JoltC_SliderConstraint_Create(
    JoltC_PhysicsSystem*                    system,
    JoltC_BodyID                            body1,
    JoltC_BodyID                            body2,
    const JoltC_SliderConstraintSettings*   settings);

JOLTC_API float JoltC_SliderConstraint_GetCurrentPosition(const JoltC_Constraint* c);
JOLTC_API void  JoltC_SliderConstraint_SetMotorState(JoltC_Constraint* c, JoltC_MotorState state);
JOLTC_API JoltC_MotorState JoltC_SliderConstraint_GetMotorState(const JoltC_Constraint* c);
JOLTC_API void  JoltC_SliderConstraint_SetTargetVelocity(JoltC_Constraint* c, float velocity);
JOLTC_API float JoltC_SliderConstraint_GetTargetVelocity(const JoltC_Constraint* c);
JOLTC_API void  JoltC_SliderConstraint_SetTargetPosition(JoltC_Constraint* c, float position);
JOLTC_API float JoltC_SliderConstraint_GetTargetPosition(const JoltC_Constraint* c);
JOLTC_API void  JoltC_SliderConstraint_SetLimits(JoltC_Constraint* c, float min, float max);
JOLTC_API float JoltC_SliderConstraint_GetLimitsMin(const JoltC_Constraint* c);
JOLTC_API float JoltC_SliderConstraint_GetLimitsMax(const JoltC_Constraint* c);
JOLTC_API JoltC_Bool JoltC_SliderConstraint_HasLimits(const JoltC_Constraint* c);
JOLTC_API void  JoltC_SliderConstraint_SetMaxFrictionForce(JoltC_Constraint* c, float force);
JOLTC_API float JoltC_SliderConstraint_GetMaxFrictionForce(const JoltC_Constraint* c);
JOLTC_API JoltC_SpringSettings JoltC_SliderConstraint_GetLimitsSpringSettings(const JoltC_Constraint* c);
JOLTC_API void  JoltC_SliderConstraint_SetLimitsSpringSettings(JoltC_Constraint* c, JoltC_SpringSettings settings);
JOLTC_API JoltC_MotorSettings JoltC_SliderConstraint_GetMotorSettings(const JoltC_Constraint* c);
JOLTC_API void  JoltC_SliderConstraint_SetMotorSettings(JoltC_Constraint* c, JoltC_MotorSettings settings);
JOLTC_API JoltC_Vec3  JoltC_SliderConstraint_GetTotalLambdaPosition(const JoltC_Constraint* c);
JOLTC_API float       JoltC_SliderConstraint_GetTotalLambdaPositionLimits(const JoltC_Constraint* c);
JOLTC_API JoltC_Vec3  JoltC_SliderConstraint_GetTotalLambdaRotation(const JoltC_Constraint* c);
JOLTC_API float       JoltC_SliderConstraint_GetTotalLambdaMotor(const JoltC_Constraint* c);
JOLTC_API void        JoltC_SliderConstraintSettings_Init(JoltC_SliderConstraintSettings* settings);
JOLTC_API void        JoltC_SliderConstraintSettings_SetSliderAxis(JoltC_SliderConstraintSettings* settings, JoltC_Vec3 axis);
JOLTC_API JoltC_Constraint* JoltC_SliderConstraint_GetSettings(const JoltC_Constraint* c);

/* -------------------------------------------------------------------------- */
/*  ConeConstraint                                                            */
/* -------------------------------------------------------------------------- */
typedef struct JoltC_ConeConstraintSettings {
    JoltC_ConstraintSpace space;
    JoltC_RVec3           point1;
    JoltC_Vec3            twistAxis1;
    JoltC_RVec3           point2;
    JoltC_Vec3            twistAxis2;
    float                 halfConeAngle;
} JoltC_ConeConstraintSettings;

JOLTC_API JoltC_Constraint* JoltC_ConeConstraint_Create(
    JoltC_PhysicsSystem*                  system,
    JoltC_BodyID                          body1,
    JoltC_BodyID                          body2,
    const JoltC_ConeConstraintSettings*   settings);

JOLTC_API void  JoltC_ConeConstraint_SetHalfConeAngle(JoltC_Constraint* c, float angle);
JOLTC_API float JoltC_ConeConstraint_GetCosHalfConeAngle(const JoltC_Constraint* c);
JOLTC_API JoltC_Vec3 JoltC_ConeConstraint_GetTotalLambdaPosition(const JoltC_Constraint* c);
JOLTC_API float      JoltC_ConeConstraint_GetTotalLambdaRotation(const JoltC_Constraint* c);
JOLTC_API void       JoltC_ConeConstraintSettings_Init(JoltC_ConeConstraintSettings* settings);
JOLTC_API JoltC_Constraint* JoltC_ConeConstraint_GetSettings(const JoltC_Constraint* c);

/* -------------------------------------------------------------------------- */
/*  SwingTwistConstraint                                                      */
/* -------------------------------------------------------------------------- */
typedef struct JoltC_SwingTwistConstraintSettings {
    JoltC_ConstraintSpace space;
    JoltC_RVec3           position1;
    JoltC_Vec3            twistAxis1;
    JoltC_Vec3            planeAxis1;
    JoltC_RVec3           position2;
    JoltC_Vec3            twistAxis2;
    JoltC_Vec3            planeAxis2;
    JoltC_SwingType       swingType;
    float                 normalHalfConeAngle;
    float                 planeHalfConeAngle;
    float                 twistMinAngle;
    float                 twistMaxAngle;
    float                 maxFrictionTorque;
    JoltC_MotorSettings   swingMotorSettings;
    JoltC_MotorSettings   twistMotorSettings;
} JoltC_SwingTwistConstraintSettings;

JOLTC_API JoltC_Constraint* JoltC_SwingTwistConstraint_Create(
    JoltC_PhysicsSystem*                        system,
    JoltC_BodyID                                body1,
    JoltC_BodyID                                body2,
    const JoltC_SwingTwistConstraintSettings*   settings);

JOLTC_API void  JoltC_SwingTwistConstraint_SetNormalHalfConeAngle(JoltC_Constraint* c, float angle);
JOLTC_API float JoltC_SwingTwistConstraint_GetNormalHalfConeAngle(const JoltC_Constraint* c);
JOLTC_API void  JoltC_SwingTwistConstraint_SetPlaneHalfConeAngle(JoltC_Constraint* c, float angle);
JOLTC_API float JoltC_SwingTwistConstraint_GetPlaneHalfConeAngle(const JoltC_Constraint* c);
JOLTC_API void  JoltC_SwingTwistConstraint_SetTwistMinAngle(JoltC_Constraint* c, float angle);
JOLTC_API float JoltC_SwingTwistConstraint_GetTwistMinAngle(const JoltC_Constraint* c);
JOLTC_API void  JoltC_SwingTwistConstraint_SetTwistMaxAngle(JoltC_Constraint* c, float angle);
JOLTC_API float JoltC_SwingTwistConstraint_GetTwistMaxAngle(const JoltC_Constraint* c);

JOLTC_API void   JoltC_SwingTwistConstraint_SetSwingMotorState(JoltC_Constraint* c, JoltC_MotorState state);
JOLTC_API JoltC_MotorState JoltC_SwingTwistConstraint_GetSwingMotorState(const JoltC_Constraint* c);
JOLTC_API void   JoltC_SwingTwistConstraint_SetTwistMotorState(JoltC_Constraint* c, JoltC_MotorState state);
JOLTC_API JoltC_MotorState JoltC_SwingTwistConstraint_GetTwistMotorState(const JoltC_Constraint* c);
JOLTC_API void   JoltC_SwingTwistConstraint_SetTargetAngularVelocityCS(JoltC_Constraint* c, JoltC_Vec3 velocity);
JOLTC_API JoltC_Vec3 JoltC_SwingTwistConstraint_GetTargetAngularVelocityCS(const JoltC_Constraint* c);
JOLTC_API void   JoltC_SwingTwistConstraint_SetTargetOrientationCS(JoltC_Constraint* c, JoltC_Quat orientation);
JOLTC_API JoltC_Quat JoltC_SwingTwistConstraint_GetTargetOrientationCS(const JoltC_Constraint* c);
JOLTC_API void   JoltC_SwingTwistConstraint_SetMaxFrictionTorque(JoltC_Constraint* c, float torque);
JOLTC_API float  JoltC_SwingTwistConstraint_GetMaxFrictionTorque(const JoltC_Constraint* c);
JOLTC_API JoltC_Vec3  JoltC_SwingTwistConstraint_GetTotalLambdaPosition(const JoltC_Constraint* c);
JOLTC_API float       JoltC_SwingTwistConstraint_GetTotalLambdaTwist(const JoltC_Constraint* c);
JOLTC_API float       JoltC_SwingTwistConstraint_GetTotalLambdaSwingY(const JoltC_Constraint* c);
JOLTC_API float       JoltC_SwingTwistConstraint_GetTotalLambdaSwingZ(const JoltC_Constraint* c);
JOLTC_API float       JoltC_SwingTwistConstraint_GetTotalLambdaMotor(const JoltC_Constraint* c);
JOLTC_API void        JoltC_SwingTwistConstraintSettings_Init(JoltC_SwingTwistConstraintSettings* settings);
JOLTC_API JoltC_Constraint* JoltC_SwingTwistConstraint_GetSettings(const JoltC_Constraint* c);

/* -------------------------------------------------------------------------- */
/*  SixDOFConstraint                                                          */
/* -------------------------------------------------------------------------- */
typedef struct JoltC_SixDOFConstraintSettings {
    JoltC_ConstraintSpace space;
    JoltC_RVec3           position1;
    JoltC_Vec3            axisX1;
    JoltC_Vec3            axisY1;
    JoltC_RVec3           position2;
    JoltC_Vec3            axisX2;
    JoltC_Vec3            axisY2;
    float                 maxFriction[6];
    JoltC_SwingType       swingType;
    float                 limitMin[6];
    float                 limitMax[6];
    JoltC_SpringSettings  limitsSpringSettings[3]; /* Translation axes only */
    JoltC_MotorSettings   motorSettings[6];
} JoltC_SixDOFConstraintSettings;

JOLTC_API JoltC_Constraint* JoltC_SixDOFConstraint_Create(
    JoltC_PhysicsSystem*                     system,
    JoltC_BodyID                             body1,
    JoltC_BodyID                             body2,
    const JoltC_SixDOFConstraintSettings*    settings);

JOLTC_API void  JoltC_SixDOFConstraint_SetTranslationLimits(JoltC_Constraint* c, JoltC_Vec3 limitMin, JoltC_Vec3 limitMax);
JOLTC_API void  JoltC_SixDOFConstraint_SetRotationLimits(JoltC_Constraint* c, JoltC_Vec3 limitMin, JoltC_Vec3 limitMax);
JOLTC_API float JoltC_SixDOFConstraint_GetLimitsMin(const JoltC_Constraint* c, JoltC_SixDOFConstraintAxis axis);
JOLTC_API float JoltC_SixDOFConstraint_GetLimitsMax(const JoltC_Constraint* c, JoltC_SixDOFConstraintAxis axis);
JOLTC_API void  JoltC_SixDOFConstraint_SetMotorState(JoltC_Constraint* c, JoltC_SixDOFConstraintAxis axis, JoltC_MotorState state);
JOLTC_API JoltC_MotorState JoltC_SixDOFConstraint_GetMotorState(const JoltC_Constraint* c, JoltC_SixDOFConstraintAxis axis);
JOLTC_API void       JoltC_SixDOFConstraint_SetTargetVelocityCS(JoltC_Constraint* c, JoltC_Vec3 velocity);
JOLTC_API JoltC_Vec3 JoltC_SixDOFConstraint_GetTargetVelocityCS(const JoltC_Constraint* c);
JOLTC_API void       JoltC_SixDOFConstraint_SetTargetAngularVelocityCS(JoltC_Constraint* c, JoltC_Vec3 velocity);
JOLTC_API JoltC_Vec3 JoltC_SixDOFConstraint_GetTargetAngularVelocityCS(const JoltC_Constraint* c);
JOLTC_API void       JoltC_SixDOFConstraint_SetTargetPositionCS(JoltC_Constraint* c, JoltC_Vec3 position);
JOLTC_API JoltC_Vec3 JoltC_SixDOFConstraint_GetTargetPositionCS(const JoltC_Constraint* c);
JOLTC_API void       JoltC_SixDOFConstraint_SetTargetOrientationCS(JoltC_Constraint* c, JoltC_Quat orientation);
JOLTC_API JoltC_Quat JoltC_SixDOFConstraint_GetTargetOrientationCS(const JoltC_Constraint* c);
JOLTC_API void  JoltC_SixDOFConstraint_SetMaxFriction(JoltC_Constraint* c, JoltC_SixDOFConstraintAxis axis, float friction);
JOLTC_API float JoltC_SixDOFConstraint_GetMaxFriction(const JoltC_Constraint* c, JoltC_SixDOFConstraintAxis axis);
JOLTC_API JoltC_Vec3 JoltC_SixDOFConstraint_GetTranslationLimitsMin(const JoltC_Constraint* c);
JOLTC_API JoltC_Vec3 JoltC_SixDOFConstraint_GetTranslationLimitsMax(const JoltC_Constraint* c);
JOLTC_API JoltC_Vec3 JoltC_SixDOFConstraint_GetRotationLimitsMin(const JoltC_Constraint* c);
JOLTC_API JoltC_Vec3 JoltC_SixDOFConstraint_GetRotationLimitsMax(const JoltC_Constraint* c);
JOLTC_API JoltC_Bool JoltC_SixDOFConstraint_IsFreeAxis(const JoltC_Constraint* c, JoltC_SixDOFConstraintAxis axis);
JOLTC_API JoltC_Bool JoltC_SixDOFConstraint_IsFixedAxis(const JoltC_Constraint* c, JoltC_SixDOFConstraintAxis axis);
JOLTC_API JoltC_SpringSettings JoltC_SixDOFConstraint_GetLimitsSpringSettings(const JoltC_Constraint* c, JoltC_SixDOFConstraintAxis axis);
JOLTC_API void  JoltC_SixDOFConstraint_SetLimitsSpringSettings(JoltC_Constraint* c, JoltC_SixDOFConstraintAxis axis, JoltC_SpringSettings settings);
JOLTC_API JoltC_MotorSettings JoltC_SixDOFConstraint_GetMotorSettings(const JoltC_Constraint* c, JoltC_SixDOFConstraintAxis axis);
JOLTC_API JoltC_Quat JoltC_SixDOFConstraint_GetRotationInConstraintSpace(const JoltC_Constraint* c);
JOLTC_API void       JoltC_SixDOFConstraint_SetTargetOrientationBS(JoltC_Constraint* c, JoltC_Quat orientation);
JOLTC_API JoltC_Vec3 JoltC_SixDOFConstraint_GetTotalLambdaPosition(const JoltC_Constraint* c);
JOLTC_API JoltC_Vec3 JoltC_SixDOFConstraint_GetTotalLambdaRotation(const JoltC_Constraint* c);
JOLTC_API JoltC_Vec3 JoltC_SixDOFConstraint_GetTotalLambdaMotorTranslation(const JoltC_Constraint* c);
JOLTC_API JoltC_Vec3 JoltC_SixDOFConstraint_GetTotalLambdaMotorRotation(const JoltC_Constraint* c);
JOLTC_API void       JoltC_SixDOFConstraintSettings_Init(JoltC_SixDOFConstraintSettings* settings);
JOLTC_API JoltC_Bool JoltC_SixDOFConstraintSettings_IsFreeAxis(const JoltC_SixDOFConstraintSettings* s, JoltC_SixDOFConstraintAxis axis);
JOLTC_API JoltC_Bool JoltC_SixDOFConstraintSettings_IsFixedAxis(const JoltC_SixDOFConstraintSettings* s, JoltC_SixDOFConstraintAxis axis);
JOLTC_API void       JoltC_SixDOFConstraintSettings_MakeFreeAxis(JoltC_SixDOFConstraintSettings* s, JoltC_SixDOFConstraintAxis axis);
JOLTC_API void       JoltC_SixDOFConstraintSettings_MakeFixedAxis(JoltC_SixDOFConstraintSettings* s, JoltC_SixDOFConstraintAxis axis);
JOLTC_API void       JoltC_SixDOFConstraintSettings_SetLimitedAxis(JoltC_SixDOFConstraintSettings* s, JoltC_SixDOFConstraintAxis axis, float min, float max);
JOLTC_API JoltC_Constraint* JoltC_SixDOFConstraint_GetSettings(const JoltC_Constraint* c);

/* -------------------------------------------------------------------------- */
/*  PulleyConstraint                                                          */
/* -------------------------------------------------------------------------- */
typedef struct JoltC_PulleyConstraintSettings {
    JoltC_ConstraintSpace space;
    JoltC_RVec3           bodyPoint1;
    JoltC_RVec3           fixedPoint1;
    JoltC_RVec3           bodyPoint2;
    JoltC_RVec3           fixedPoint2;
    float                 ratio;
    float                 minLength;
    float                 maxLength;
} JoltC_PulleyConstraintSettings;

JOLTC_API JoltC_Constraint* JoltC_PulleyConstraint_Create(
    JoltC_PhysicsSystem*                     system,
    JoltC_BodyID                             body1,
    JoltC_BodyID                             body2,
    const JoltC_PulleyConstraintSettings*    settings);

/* -------------------------------------------------------------------------- */
/*  GearConstraint                                                            */
/* -------------------------------------------------------------------------- */
typedef struct JoltC_GearConstraintSettings {
    JoltC_ConstraintSpace space;
    JoltC_Vec3            hingeAxis1;
    JoltC_Vec3            hingeAxis2;
    float                 ratio;
} JoltC_GearConstraintSettings;

JOLTC_API JoltC_Constraint* JoltC_GearConstraint_Create(
    JoltC_PhysicsSystem*                   system,
    JoltC_BodyID                           body1,
    JoltC_BodyID                           body2,
    const JoltC_GearConstraintSettings*    settings);

JOLTC_API void  JoltC_GearConstraint_SetConstraints(JoltC_Constraint* c, const JoltC_Constraint* gear1, const JoltC_Constraint* gear2);
JOLTC_API float JoltC_GearConstraint_GetTotalLambda(const JoltC_Constraint* c);
JOLTC_API void  JoltC_GearConstraintSettings_Init(JoltC_GearConstraintSettings* settings);
JOLTC_API JoltC_Constraint* JoltC_GearConstraint_GetSettings(const JoltC_Constraint* c);

/* -------------------------------------------------------------------------- */
/*  RackAndPinionConstraint                                                   */
/* -------------------------------------------------------------------------- */
typedef struct JoltC_RackAndPinionConstraintSettings {
    JoltC_ConstraintSpace space;
    JoltC_Vec3            hingeAxis;
    JoltC_Vec3            sliderAxis;
    float                 ratio;
} JoltC_RackAndPinionConstraintSettings;

JOLTC_API JoltC_Constraint* JoltC_RackAndPinionConstraint_Create(
    JoltC_PhysicsSystem*                          system,
    JoltC_BodyID                                  body1,
    JoltC_BodyID                                  body2,
    const JoltC_RackAndPinionConstraintSettings*  settings);

JOLTC_API void JoltC_RackAndPinionConstraint_SetConstraints(JoltC_Constraint* c, const JoltC_Constraint* pinion, const JoltC_Constraint* rack);
JOLTC_API float JoltC_RackAndPinionConstraint_GetTotalLambda(const JoltC_Constraint* c);

#ifdef __cplusplus
}
#endif

#endif /* JOLTC_CONSTRAINT_H */
