/* JoltC - C bindings for JoltPhysics
 * SPDX-License-Identifier: MIT
 *
 * Vehicle system: constraints, wheels, controllers, engine, transmission.
 */

#ifndef JOLTC_VEHICLE_H
#define JOLTC_VEHICLE_H

#include <JoltC/common.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*  LinearCurve                                                               */
/* ========================================================================== */
JOLTC_API JoltC_LinearCurve* JoltC_LinearCurve_Create(void);
JOLTC_API void               JoltC_LinearCurve_Destroy(JoltC_LinearCurve* curve);
JOLTC_API void               JoltC_LinearCurve_Clear(JoltC_LinearCurve* curve);
JOLTC_API void               JoltC_LinearCurve_Reserve(JoltC_LinearCurve* curve, uint32_t numPoints);
JOLTC_API void               JoltC_LinearCurve_AddPoint(JoltC_LinearCurve* curve, float x, float y);
JOLTC_API void               JoltC_LinearCurve_Sort(JoltC_LinearCurve* curve);
JOLTC_API float              JoltC_LinearCurve_GetMinX(const JoltC_LinearCurve* curve);
JOLTC_API float              JoltC_LinearCurve_GetMaxX(const JoltC_LinearCurve* curve);
JOLTC_API float              JoltC_LinearCurve_GetValue(const JoltC_LinearCurve* curve, float x);
JOLTC_API uint32_t           JoltC_LinearCurve_GetPointCount(const JoltC_LinearCurve* curve);
JOLTC_API void               JoltC_LinearCurve_GetPoint(const JoltC_LinearCurve* curve, uint32_t index, JoltC_Point* result);
JOLTC_API void               JoltC_LinearCurve_GetPoints(const JoltC_LinearCurve* curve, JoltC_Point* points, uint32_t* count);

/* ========================================================================== */
/*  WheelSettings (base)                                                      */
/* ========================================================================== */
JOLTC_API JoltC_WheelSettings* JoltC_WheelSettings_Create(void);
JOLTC_API void       JoltC_WheelSettings_Destroy(JoltC_WheelSettings* s);
JOLTC_API JoltC_Vec3 JoltC_WheelSettings_GetPosition(const JoltC_WheelSettings* s);
JOLTC_API void       JoltC_WheelSettings_SetPosition(JoltC_WheelSettings* s, JoltC_Vec3 value);
JOLTC_API JoltC_Vec3 JoltC_WheelSettings_GetSuspensionForcePoint(const JoltC_WheelSettings* s);
JOLTC_API void       JoltC_WheelSettings_SetSuspensionForcePoint(JoltC_WheelSettings* s, JoltC_Vec3 value);
JOLTC_API JoltC_Vec3 JoltC_WheelSettings_GetSuspensionDirection(const JoltC_WheelSettings* s);
JOLTC_API void       JoltC_WheelSettings_SetSuspensionDirection(JoltC_WheelSettings* s, JoltC_Vec3 value);
JOLTC_API JoltC_Vec3 JoltC_WheelSettings_GetSteeringAxis(const JoltC_WheelSettings* s);
JOLTC_API void       JoltC_WheelSettings_SetSteeringAxis(JoltC_WheelSettings* s, JoltC_Vec3 value);
JOLTC_API JoltC_Vec3 JoltC_WheelSettings_GetWheelUp(const JoltC_WheelSettings* s);
JOLTC_API void       JoltC_WheelSettings_SetWheelUp(JoltC_WheelSettings* s, JoltC_Vec3 value);
JOLTC_API JoltC_Vec3 JoltC_WheelSettings_GetWheelForward(const JoltC_WheelSettings* s);
JOLTC_API void       JoltC_WheelSettings_SetWheelForward(JoltC_WheelSettings* s, JoltC_Vec3 value);
JOLTC_API float      JoltC_WheelSettings_GetSuspensionMinLength(const JoltC_WheelSettings* s);
JOLTC_API void       JoltC_WheelSettings_SetSuspensionMinLength(JoltC_WheelSettings* s, float value);
JOLTC_API float      JoltC_WheelSettings_GetSuspensionMaxLength(const JoltC_WheelSettings* s);
JOLTC_API void       JoltC_WheelSettings_SetSuspensionMaxLength(JoltC_WheelSettings* s, float value);
JOLTC_API float      JoltC_WheelSettings_GetSuspensionPreloadLength(const JoltC_WheelSettings* s);
JOLTC_API void       JoltC_WheelSettings_SetSuspensionPreloadLength(JoltC_WheelSettings* s, float value);
JOLTC_API JoltC_SpringSettings JoltC_WheelSettings_GetSuspensionSpring(const JoltC_WheelSettings* s);
JOLTC_API void       JoltC_WheelSettings_SetSuspensionSpring(JoltC_WheelSettings* s, JoltC_SpringSettings value);
JOLTC_API float      JoltC_WheelSettings_GetRadius(const JoltC_WheelSettings* s);
JOLTC_API void       JoltC_WheelSettings_SetRadius(JoltC_WheelSettings* s, float value);
JOLTC_API float      JoltC_WheelSettings_GetWidth(const JoltC_WheelSettings* s);
JOLTC_API void       JoltC_WheelSettings_SetWidth(JoltC_WheelSettings* s, float value);
JOLTC_API JoltC_Bool JoltC_WheelSettings_GetEnableSuspensionForcePoint(const JoltC_WheelSettings* s);
JOLTC_API void       JoltC_WheelSettings_SetEnableSuspensionForcePoint(JoltC_WheelSettings* s, JoltC_Bool value);

/* ========================================================================== */
/*  WheelSettingsWV (wheeled vehicle)                                         */
/* ========================================================================== */
JOLTC_API JoltC_WheelSettingsWV* JoltC_WheelSettingsWV_Create(void);
JOLTC_API float                  JoltC_WheelSettingsWV_GetInertia(const JoltC_WheelSettingsWV* s);
JOLTC_API void                   JoltC_WheelSettingsWV_SetInertia(JoltC_WheelSettingsWV* s, float value);
JOLTC_API float                  JoltC_WheelSettingsWV_GetAngularDamping(const JoltC_WheelSettingsWV* s);
JOLTC_API void                   JoltC_WheelSettingsWV_SetAngularDamping(JoltC_WheelSettingsWV* s, float value);
JOLTC_API float                  JoltC_WheelSettingsWV_GetMaxSteerAngle(const JoltC_WheelSettingsWV* s);
JOLTC_API void                   JoltC_WheelSettingsWV_SetMaxSteerAngle(JoltC_WheelSettingsWV* s, float value);
JOLTC_API const JoltC_LinearCurve* JoltC_WheelSettingsWV_GetLongitudinalFriction(const JoltC_WheelSettingsWV* s);
JOLTC_API void                   JoltC_WheelSettingsWV_SetLongitudinalFriction(JoltC_WheelSettingsWV* s, const JoltC_LinearCurve* curve);
JOLTC_API const JoltC_LinearCurve* JoltC_WheelSettingsWV_GetLateralFriction(const JoltC_WheelSettingsWV* s);
JOLTC_API void                   JoltC_WheelSettingsWV_SetLateralFriction(JoltC_WheelSettingsWV* s, const JoltC_LinearCurve* curve);
JOLTC_API float                  JoltC_WheelSettingsWV_GetMaxBrakeTorque(const JoltC_WheelSettingsWV* s);
JOLTC_API void                   JoltC_WheelSettingsWV_SetMaxBrakeTorque(JoltC_WheelSettingsWV* s, float value);
JOLTC_API float                  JoltC_WheelSettingsWV_GetMaxHandBrakeTorque(const JoltC_WheelSettingsWV* s);
JOLTC_API void                   JoltC_WheelSettingsWV_SetMaxHandBrakeTorque(JoltC_WheelSettingsWV* s, float value);

/* ========================================================================== */
/*  WheelSettingsTV (tracked vehicle)                                         */
/* ========================================================================== */
JOLTC_API JoltC_WheelSettingsTV* JoltC_WheelSettingsTV_Create(void);
JOLTC_API float                  JoltC_WheelSettingsTV_GetLongitudinalFriction(const JoltC_WheelSettingsTV* s);
JOLTC_API void                   JoltC_WheelSettingsTV_SetLongitudinalFriction(JoltC_WheelSettingsTV* s, float value);
JOLTC_API float                  JoltC_WheelSettingsTV_GetLateralFriction(const JoltC_WheelSettingsTV* s);
JOLTC_API void                   JoltC_WheelSettingsTV_SetLateralFriction(JoltC_WheelSettingsTV* s, float value);

/* ========================================================================== */
/*  Wheel (runtime, base)                                                     */
/* ========================================================================== */
JOLTC_API const JoltC_WheelSettings* JoltC_Wheel_GetSettings(const JoltC_Wheel* w);
JOLTC_API float      JoltC_Wheel_GetAngularVelocity(const JoltC_Wheel* w);
JOLTC_API void       JoltC_Wheel_SetAngularVelocity(JoltC_Wheel* w, float value);
JOLTC_API float      JoltC_Wheel_GetRotationAngle(const JoltC_Wheel* w);
JOLTC_API void       JoltC_Wheel_SetRotationAngle(JoltC_Wheel* w, float value);
JOLTC_API float      JoltC_Wheel_GetSteerAngle(const JoltC_Wheel* w);
JOLTC_API void       JoltC_Wheel_SetSteerAngle(JoltC_Wheel* w, float value);
JOLTC_API JoltC_Bool JoltC_Wheel_HasContact(const JoltC_Wheel* w);
JOLTC_API JoltC_BodyID    JoltC_Wheel_GetContactBodyID(const JoltC_Wheel* w);
JOLTC_API JoltC_SubShapeID JoltC_Wheel_GetContactSubShapeID(const JoltC_Wheel* w);
JOLTC_API JoltC_RVec3 JoltC_Wheel_GetContactPosition(const JoltC_Wheel* w);
JOLTC_API JoltC_Vec3  JoltC_Wheel_GetContactPointVelocity(const JoltC_Wheel* w);
JOLTC_API JoltC_Vec3  JoltC_Wheel_GetContactNormal(const JoltC_Wheel* w);
JOLTC_API JoltC_Vec3  JoltC_Wheel_GetContactLongitudinal(const JoltC_Wheel* w);
JOLTC_API JoltC_Vec3  JoltC_Wheel_GetContactLateral(const JoltC_Wheel* w);
JOLTC_API float       JoltC_Wheel_GetSuspensionLength(const JoltC_Wheel* w);
JOLTC_API float       JoltC_Wheel_GetSuspensionLambda(const JoltC_Wheel* w);
JOLTC_API float       JoltC_Wheel_GetLongitudinalLambda(const JoltC_Wheel* w);
JOLTC_API float       JoltC_Wheel_GetLateralLambda(const JoltC_Wheel* w);
JOLTC_API JoltC_Bool  JoltC_Wheel_HasHitHardPoint(const JoltC_Wheel* w);

/* ========================================================================== */
/*  WheelWV / WheelTV                                                         */
/* ========================================================================== */
JOLTC_API const JoltC_WheelSettingsWV* JoltC_WheelWV_GetSettings(const JoltC_WheelWV* w);
JOLTC_API void  JoltC_WheelWV_ApplyTorque(JoltC_WheelWV* w, float torque, float deltaTime);

JOLTC_API const JoltC_WheelSettingsTV* JoltC_WheelTV_GetSettings(const JoltC_WheelTV* w);

/* ========================================================================== */
/*  VehicleConstraintSettings                                                 */
/* ========================================================================== */
JOLTC_API void JoltC_VehicleConstraintSettings_Init(JoltC_VehicleConstraintSettings* settings);

/* ========================================================================== */
/*  VehicleConstraint                                                         */
/* ========================================================================== */
JOLTC_API JoltC_VehicleConstraint* JoltC_VehicleConstraint_Create(JoltC_Body* body, const JoltC_VehicleConstraintSettings* settings);
JOLTC_API JoltC_PhysicsStepListener* JoltC_VehicleConstraint_AsPhysicsStepListener(JoltC_VehicleConstraint* vc);
JOLTC_API void       JoltC_VehicleConstraint_SetMaxPitchRollAngle(JoltC_VehicleConstraint* vc, float angle);
JOLTC_API void       JoltC_VehicleConstraint_SetVehicleCollisionTester(JoltC_VehicleConstraint* vc, const JoltC_VehicleCollisionTester* tester);
JOLTC_API void       JoltC_VehicleConstraint_OverrideGravity(JoltC_VehicleConstraint* vc, JoltC_Vec3 gravity);
JOLTC_API JoltC_Bool JoltC_VehicleConstraint_IsGravityOverridden(const JoltC_VehicleConstraint* vc);
JOLTC_API JoltC_Vec3 JoltC_VehicleConstraint_GetGravityOverride(const JoltC_VehicleConstraint* vc);
JOLTC_API void       JoltC_VehicleConstraint_ResetGravityOverride(JoltC_VehicleConstraint* vc);
JOLTC_API JoltC_Vec3 JoltC_VehicleConstraint_GetLocalForward(const JoltC_VehicleConstraint* vc);
JOLTC_API JoltC_Vec3 JoltC_VehicleConstraint_GetLocalUp(const JoltC_VehicleConstraint* vc);
JOLTC_API JoltC_Vec3 JoltC_VehicleConstraint_GetWorldUp(const JoltC_VehicleConstraint* vc);
JOLTC_API const JoltC_Body* JoltC_VehicleConstraint_GetVehicleBody(const JoltC_VehicleConstraint* vc);
JOLTC_API JoltC_VehicleController* JoltC_VehicleConstraint_GetController(JoltC_VehicleConstraint* vc);
JOLTC_API uint32_t   JoltC_VehicleConstraint_GetWheelsCount(JoltC_VehicleConstraint* vc);
JOLTC_API JoltC_Wheel* JoltC_VehicleConstraint_GetWheel(JoltC_VehicleConstraint* vc, uint32_t index);
JOLTC_API void       JoltC_VehicleConstraint_GetWheelLocalTransform(JoltC_VehicleConstraint* vc, uint32_t wheelIndex, JoltC_Vec3 wheelRight, JoltC_Vec3 wheelUp, JoltC_Mat44* result);
JOLTC_API void       JoltC_VehicleConstraint_GetWheelWorldTransform(JoltC_VehicleConstraint* vc, uint32_t wheelIndex, JoltC_Vec3 wheelRight, JoltC_Vec3 wheelUp, JoltC_Mat44* result);
JOLTC_API void       JoltC_VehicleConstraint_GetWheelLocalBasis(const JoltC_VehicleConstraint* constraint, uint32_t wheelIndex, JoltC_Vec3* outUp, JoltC_Vec3* outForward);

/* ========================================================================== */
/*  VehicleCollisionTester                                                    */
/* ========================================================================== */
JOLTC_API void              JoltC_VehicleCollisionTester_Destroy(JoltC_VehicleCollisionTester* tester);
JOLTC_API JoltC_ObjectLayer JoltC_VehicleCollisionTester_GetObjectLayer(const JoltC_VehicleCollisionTester* tester);
JOLTC_API void              JoltC_VehicleCollisionTester_SetObjectLayer(JoltC_VehicleCollisionTester* tester, JoltC_ObjectLayer layer);
JOLTC_API JoltC_VehicleCollisionTester* JoltC_VehicleCollisionTesterRay_Create(JoltC_ObjectLayer layer, JoltC_Vec3 up, float maxSlopeAngle);
JOLTC_API JoltC_VehicleCollisionTester* JoltC_VehicleCollisionTesterCastSphere_Create(JoltC_ObjectLayer layer, float radius, JoltC_Vec3 up, float maxSlopeAngle);
JOLTC_API JoltC_VehicleCollisionTester* JoltC_VehicleCollisionTesterCastCylinder_Create(JoltC_ObjectLayer layer, float convexRadiusFraction);

/* ========================================================================== */
/*  VehicleControllerSettings                                                 */
/* ========================================================================== */
JOLTC_API void JoltC_VehicleControllerSettings_Destroy(JoltC_VehicleControllerSettings* settings);

/* ========================================================================== */
/*  WheeledVehicleControllerSettings                                          */
/* ========================================================================== */
JOLTC_API JoltC_WheeledVehicleControllerSettings* JoltC_WheeledVehicleControllerSettings_Create(void);
JOLTC_API void  JoltC_WheeledVehicleControllerSettings_GetEngine(const JoltC_WheeledVehicleControllerSettings* s, JoltC_VehicleEngineSettings* result);
JOLTC_API void  JoltC_WheeledVehicleControllerSettings_SetEngine(JoltC_WheeledVehicleControllerSettings* s, const JoltC_VehicleEngineSettings* value);
JOLTC_API const JoltC_VehicleTransmissionSettings* JoltC_WheeledVehicleControllerSettings_GetTransmission(const JoltC_WheeledVehicleControllerSettings* s);
JOLTC_API void  JoltC_WheeledVehicleControllerSettings_SetTransmission(JoltC_WheeledVehicleControllerSettings* s, const JoltC_VehicleTransmissionSettings* value);
JOLTC_API uint32_t JoltC_WheeledVehicleControllerSettings_GetDifferentialsCount(const JoltC_WheeledVehicleControllerSettings* s);
JOLTC_API void  JoltC_WheeledVehicleControllerSettings_SetDifferentialsCount(JoltC_WheeledVehicleControllerSettings* s, uint32_t count);
JOLTC_API void  JoltC_WheeledVehicleControllerSettings_GetDifferential(const JoltC_WheeledVehicleControllerSettings* s, uint32_t index, JoltC_VehicleDifferentialSettings* result);
JOLTC_API void  JoltC_WheeledVehicleControllerSettings_SetDifferential(JoltC_WheeledVehicleControllerSettings* s, uint32_t index, const JoltC_VehicleDifferentialSettings* value);
JOLTC_API void  JoltC_WheeledVehicleControllerSettings_SetDifferentials(JoltC_WheeledVehicleControllerSettings* s, const JoltC_VehicleDifferentialSettings* values, uint32_t count);
JOLTC_API float JoltC_WheeledVehicleControllerSettings_GetDifferentialLimitedSlipRatio(const JoltC_WheeledVehicleControllerSettings* s);
JOLTC_API void  JoltC_WheeledVehicleControllerSettings_SetDifferentialLimitedSlipRatio(JoltC_WheeledVehicleControllerSettings* s, float value);

/* ========================================================================== */
/*  MotorcycleControllerSettings                                              */
/* ========================================================================== */
JOLTC_API JoltC_MotorcycleControllerSettings* JoltC_MotorcycleControllerSettings_Create(void);
JOLTC_API float JoltC_MotorcycleControllerSettings_GetMaxLeanAngle(const JoltC_MotorcycleControllerSettings* s);
JOLTC_API void  JoltC_MotorcycleControllerSettings_SetMaxLeanAngle(JoltC_MotorcycleControllerSettings* s, float value);
JOLTC_API float JoltC_MotorcycleControllerSettings_GetLeanSpringConstant(const JoltC_MotorcycleControllerSettings* s);
JOLTC_API void  JoltC_MotorcycleControllerSettings_SetLeanSpringConstant(JoltC_MotorcycleControllerSettings* s, float value);
JOLTC_API float JoltC_MotorcycleControllerSettings_GetLeanSpringDamping(const JoltC_MotorcycleControllerSettings* s);
JOLTC_API void  JoltC_MotorcycleControllerSettings_SetLeanSpringDamping(JoltC_MotorcycleControllerSettings* s, float value);
JOLTC_API float JoltC_MotorcycleControllerSettings_GetLeanSpringIntegrationCoefficient(const JoltC_MotorcycleControllerSettings* s);
JOLTC_API void  JoltC_MotorcycleControllerSettings_SetLeanSpringIntegrationCoefficient(JoltC_MotorcycleControllerSettings* s, float value);
JOLTC_API float JoltC_MotorcycleControllerSettings_GetLeanSpringIntegrationCoefficientDecay(const JoltC_MotorcycleControllerSettings* s);
JOLTC_API void  JoltC_MotorcycleControllerSettings_SetLeanSpringIntegrationCoefficientDecay(JoltC_MotorcycleControllerSettings* s, float value);
JOLTC_API float JoltC_MotorcycleControllerSettings_GetLeanSmoothingFactor(const JoltC_MotorcycleControllerSettings* s);
JOLTC_API void  JoltC_MotorcycleControllerSettings_SetLeanSmoothingFactor(JoltC_MotorcycleControllerSettings* s, float value);

/* ========================================================================== */
/*  TrackedVehicleControllerSettings                                          */
/* ========================================================================== */
JOLTC_API JoltC_TrackedVehicleControllerSettings* JoltC_TrackedVehicleControllerSettings_Create(void);
JOLTC_API void  JoltC_TrackedVehicleControllerSettings_GetEngine(const JoltC_TrackedVehicleControllerSettings* s, JoltC_VehicleEngineSettings* result);
JOLTC_API void  JoltC_TrackedVehicleControllerSettings_SetEngine(JoltC_TrackedVehicleControllerSettings* s, const JoltC_VehicleEngineSettings* value);
JOLTC_API const JoltC_VehicleTransmissionSettings* JoltC_TrackedVehicleControllerSettings_GetTransmission(const JoltC_TrackedVehicleControllerSettings* s);
JOLTC_API void  JoltC_TrackedVehicleControllerSettings_SetTransmission(JoltC_TrackedVehicleControllerSettings* s, const JoltC_VehicleTransmissionSettings* value);

/* ========================================================================== */
/*  WheeledVehicleController (runtime)                                        */
/* ========================================================================== */
JOLTC_API void  JoltC_WheeledVehicleController_SetDriverInput(JoltC_WheeledVehicleController* c, float forward, float right, float brake, float handBrake);
JOLTC_API void  JoltC_WheeledVehicleController_SetForwardInput(JoltC_WheeledVehicleController* c, float value);
JOLTC_API float JoltC_WheeledVehicleController_GetForwardInput(const JoltC_WheeledVehicleController* c);
JOLTC_API void  JoltC_WheeledVehicleController_SetRightInput(JoltC_WheeledVehicleController* c, float value);
JOLTC_API float JoltC_WheeledVehicleController_GetRightInput(const JoltC_WheeledVehicleController* c);
JOLTC_API void  JoltC_WheeledVehicleController_SetBrakeInput(JoltC_WheeledVehicleController* c, float value);
JOLTC_API float JoltC_WheeledVehicleController_GetBrakeInput(const JoltC_WheeledVehicleController* c);
JOLTC_API void  JoltC_WheeledVehicleController_SetHandBrakeInput(JoltC_WheeledVehicleController* c, float value);
JOLTC_API float JoltC_WheeledVehicleController_GetHandBrakeInput(const JoltC_WheeledVehicleController* c);
JOLTC_API float JoltC_WheeledVehicleController_GetWheelSpeedAtClutch(const JoltC_WheeledVehicleController* c);
JOLTC_API void  JoltC_WheeledVehicleController_SetTireMaxImpulseCallback(JoltC_WheeledVehicleController* c, JoltC_TireMaxImpulseCallback cb, void* userData);
JOLTC_API const JoltC_VehicleEngine* JoltC_WheeledVehicleController_GetEngine(const JoltC_WheeledVehicleController* c);
JOLTC_API const JoltC_VehicleTransmission* JoltC_WheeledVehicleController_GetTransmission(const JoltC_WheeledVehicleController* c);

/* ========================================================================== */
/*  MotorcycleController (runtime)                                            */
/* ========================================================================== */
JOLTC_API float     JoltC_MotorcycleController_GetWheelBase(const JoltC_MotorcycleController* c);
JOLTC_API JoltC_Bool JoltC_MotorcycleController_IsLeanControllerEnabled(const JoltC_MotorcycleController* c);
JOLTC_API void      JoltC_MotorcycleController_EnableLeanController(JoltC_MotorcycleController* c, JoltC_Bool value);
JOLTC_API JoltC_Bool JoltC_MotorcycleController_IsLeanSteeringLimitEnabled(const JoltC_MotorcycleController* c);
JOLTC_API void      JoltC_MotorcycleController_EnableLeanSteeringLimit(JoltC_MotorcycleController* c, JoltC_Bool value);
JOLTC_API float     JoltC_MotorcycleController_GetLeanSpringConstant(const JoltC_MotorcycleController* c);
JOLTC_API void      JoltC_MotorcycleController_SetLeanSpringConstant(JoltC_MotorcycleController* c, float value);
JOLTC_API float     JoltC_MotorcycleController_GetLeanSpringDamping(const JoltC_MotorcycleController* c);
JOLTC_API void      JoltC_MotorcycleController_SetLeanSpringDamping(JoltC_MotorcycleController* c, float value);
JOLTC_API float     JoltC_MotorcycleController_GetLeanSpringIntegrationCoefficient(const JoltC_MotorcycleController* c);
JOLTC_API void      JoltC_MotorcycleController_SetLeanSpringIntegrationCoefficient(JoltC_MotorcycleController* c, float value);
JOLTC_API float     JoltC_MotorcycleController_GetLeanSpringIntegrationCoefficientDecay(const JoltC_MotorcycleController* c);
JOLTC_API void      JoltC_MotorcycleController_SetLeanSpringIntegrationCoefficientDecay(JoltC_MotorcycleController* c, float value);
JOLTC_API float     JoltC_MotorcycleController_GetLeanSmoothingFactor(const JoltC_MotorcycleController* c);
JOLTC_API void      JoltC_MotorcycleController_SetLeanSmoothingFactor(JoltC_MotorcycleController* c, float value);

/* ========================================================================== */
/*  TrackedVehicleController (runtime)                                        */
/* ========================================================================== */
JOLTC_API const JoltC_VehicleTrack* JoltC_TrackedVehicleController_GetTrack(const JoltC_TrackedVehicleController* c, JoltC_TrackSide side);
JOLTC_API void  JoltC_TrackedVehicleController_SetDriverInput(JoltC_TrackedVehicleController* c, float forward, float leftRatio, float rightRatio, float brake);
JOLTC_API float JoltC_TrackedVehicleController_GetForwardInput(const JoltC_TrackedVehicleController* c);
JOLTC_API void  JoltC_TrackedVehicleController_SetForwardInput(JoltC_TrackedVehicleController* c, float value);
JOLTC_API float JoltC_TrackedVehicleController_GetLeftRatio(const JoltC_TrackedVehicleController* c);
JOLTC_API void  JoltC_TrackedVehicleController_SetLeftRatio(JoltC_TrackedVehicleController* c, float value);
JOLTC_API float JoltC_TrackedVehicleController_GetRightRatio(const JoltC_TrackedVehicleController* c);
JOLTC_API void  JoltC_TrackedVehicleController_SetRightRatio(JoltC_TrackedVehicleController* c, float value);
JOLTC_API float JoltC_TrackedVehicleController_GetBrakeInput(const JoltC_TrackedVehicleController* c);
JOLTC_API void  JoltC_TrackedVehicleController_SetBrakeInput(JoltC_TrackedVehicleController* c, float value);
JOLTC_API const JoltC_VehicleEngine* JoltC_TrackedVehicleController_GetEngine(const JoltC_TrackedVehicleController* c);
JOLTC_API const JoltC_VehicleTransmission* JoltC_TrackedVehicleController_GetTransmission(const JoltC_TrackedVehicleController* c);

/* ========================================================================== */
/*  VehicleEngine (runtime)                                                   */
/* ========================================================================== */
JOLTC_API void  JoltC_VehicleEngine_ClampRPM(JoltC_VehicleEngine* e);
JOLTC_API float JoltC_VehicleEngine_GetCurrentRPM(const JoltC_VehicleEngine* e);
JOLTC_API void  JoltC_VehicleEngine_SetCurrentRPM(JoltC_VehicleEngine* e, float rpm);
JOLTC_API float JoltC_VehicleEngine_GetAngularVelocity(const JoltC_VehicleEngine* e);
JOLTC_API float JoltC_VehicleEngine_GetTorque(const JoltC_VehicleEngine* e, float acceleration);
JOLTC_API void  JoltC_VehicleEngine_ApplyTorque(JoltC_VehicleEngine* e, float torque, float deltaTime);
JOLTC_API void  JoltC_VehicleEngine_ApplyDamping(JoltC_VehicleEngine* e, float deltaTime);
JOLTC_API JoltC_Bool JoltC_VehicleEngine_AllowSleep(const JoltC_VehicleEngine* e);

/* ========================================================================== */
/*  VehicleTransmissionSettings (opaque — has internal arrays)                */
/* ========================================================================== */
JOLTC_API JoltC_VehicleTransmissionSettings* JoltC_VehicleTransmissionSettings_Create(void);
JOLTC_API void  JoltC_VehicleTransmissionSettings_Destroy(JoltC_VehicleTransmissionSettings* s);
JOLTC_API JoltC_TransmissionMode JoltC_VehicleTransmissionSettings_GetMode(const JoltC_VehicleTransmissionSettings* s);
JOLTC_API void  JoltC_VehicleTransmissionSettings_SetMode(JoltC_VehicleTransmissionSettings* s, JoltC_TransmissionMode mode);
JOLTC_API uint32_t JoltC_VehicleTransmissionSettings_GetGearRatioCount(const JoltC_VehicleTransmissionSettings* s);
JOLTC_API float JoltC_VehicleTransmissionSettings_GetGearRatio(const JoltC_VehicleTransmissionSettings* s, uint32_t index);
JOLTC_API void  JoltC_VehicleTransmissionSettings_SetGearRatios(JoltC_VehicleTransmissionSettings* s, const float* values, uint32_t count);
JOLTC_API uint32_t JoltC_VehicleTransmissionSettings_GetReverseGearRatioCount(const JoltC_VehicleTransmissionSettings* s);
JOLTC_API float JoltC_VehicleTransmissionSettings_GetReverseGearRatio(const JoltC_VehicleTransmissionSettings* s, uint32_t index);
JOLTC_API void  JoltC_VehicleTransmissionSettings_SetReverseGearRatios(JoltC_VehicleTransmissionSettings* s, const float* values, uint32_t count);
JOLTC_API float JoltC_VehicleTransmissionSettings_GetSwitchTime(const JoltC_VehicleTransmissionSettings* s);
JOLTC_API void  JoltC_VehicleTransmissionSettings_SetSwitchTime(JoltC_VehicleTransmissionSettings* s, float value);
JOLTC_API float JoltC_VehicleTransmissionSettings_GetClutchReleaseTime(const JoltC_VehicleTransmissionSettings* s);
JOLTC_API void  JoltC_VehicleTransmissionSettings_SetClutchReleaseTime(JoltC_VehicleTransmissionSettings* s, float value);
JOLTC_API float JoltC_VehicleTransmissionSettings_GetSwitchLatency(const JoltC_VehicleTransmissionSettings* s);
JOLTC_API void  JoltC_VehicleTransmissionSettings_SetSwitchLatency(JoltC_VehicleTransmissionSettings* s, float value);
JOLTC_API float JoltC_VehicleTransmissionSettings_GetShiftUpRPM(const JoltC_VehicleTransmissionSettings* s);
JOLTC_API void  JoltC_VehicleTransmissionSettings_SetShiftUpRPM(JoltC_VehicleTransmissionSettings* s, float value);
JOLTC_API float JoltC_VehicleTransmissionSettings_GetShiftDownRPM(const JoltC_VehicleTransmissionSettings* s);
JOLTC_API void  JoltC_VehicleTransmissionSettings_SetShiftDownRPM(JoltC_VehicleTransmissionSettings* s, float value);
JOLTC_API float JoltC_VehicleTransmissionSettings_GetClutchStrength(const JoltC_VehicleTransmissionSettings* s);
JOLTC_API void  JoltC_VehicleTransmissionSettings_SetClutchStrength(JoltC_VehicleTransmissionSettings* s, float value);
JOLTC_API void  JoltC_VehicleTransmissionSettings_SetGearRatio(JoltC_VehicleTransmissionSettings* s, int gearIndex, float ratio);
JOLTC_API void  JoltC_VehicleTransmissionSettings_SetReverseGearRatio(JoltC_VehicleTransmissionSettings* s, int gearIndex, float ratio);

/* ========================================================================== */
/*  VehicleTransmission (runtime)                                             */
/* ========================================================================== */
JOLTC_API void  JoltC_VehicleTransmission_Set(JoltC_VehicleTransmission* t, int currentGear, float clutchFriction);
JOLTC_API void  JoltC_VehicleTransmission_Update(JoltC_VehicleTransmission* t, float deltaTime, float currentRPM, float forwardInput, JoltC_Bool canShiftUp);
JOLTC_API int   JoltC_VehicleTransmission_GetCurrentGear(const JoltC_VehicleTransmission* t);
JOLTC_API float JoltC_VehicleTransmission_GetClutchFriction(const JoltC_VehicleTransmission* t);
JOLTC_API JoltC_Bool JoltC_VehicleTransmission_IsSwitchingGear(const JoltC_VehicleTransmission* t);
JOLTC_API float JoltC_VehicleTransmission_GetCurrentRatio(const JoltC_VehicleTransmission* t);
JOLTC_API JoltC_Bool JoltC_VehicleTransmission_AllowSleep(const JoltC_VehicleTransmission* t);

/* ========================================================================== */
/*  VehicleTrack (runtime)                                                    */
/* ========================================================================== */
JOLTC_API float    JoltC_VehicleTrack_GetAngularVelocity(const JoltC_VehicleTrack* t);
JOLTC_API void     JoltC_VehicleTrack_SetAngularVelocity(JoltC_VehicleTrack* t, float velocity);
JOLTC_API uint32_t JoltC_VehicleTrack_GetDrivenWheel(const JoltC_VehicleTrack* t);
JOLTC_API float    JoltC_VehicleTrack_GetInertia(const JoltC_VehicleTrack* t);
JOLTC_API float    JoltC_VehicleTrack_GetAngularDamping(const JoltC_VehicleTrack* t);
JOLTC_API float    JoltC_VehicleTrack_GetMaxBrakeTorque(const JoltC_VehicleTrack* t);
JOLTC_API float    JoltC_VehicleTrack_GetDifferentialRatio(const JoltC_VehicleTrack* t);

/* ========================================================================== */
/*  Init helpers                                                              */
/* ========================================================================== */
JOLTC_API void JoltC_VehicleEngineSettings_Init(JoltC_VehicleEngineSettings* settings);
JOLTC_API void JoltC_VehicleDifferentialSettings_Init(JoltC_VehicleDifferentialSettings* settings);
JOLTC_API void JoltC_VehicleAntiRollBar_Init(JoltC_VehicleAntiRollBar* bar);
JOLTC_API void JoltC_VehicleTrackSettings_Init(JoltC_VehicleTrackSettings* settings);

/* ========================================================================== */
/*  Wheel / WheelWV / WheelTV — Create / Destroy                             */
/* ========================================================================== */
JOLTC_API JoltC_Wheel* JoltC_Wheel_Create(const JoltC_WheelSettings* settings);
JOLTC_API void         JoltC_Wheel_Destroy(JoltC_Wheel* wheel);
JOLTC_API JoltC_Wheel* JoltC_WheelWV_Create(const JoltC_WheelSettings* settings);
JOLTC_API JoltC_Wheel* JoltC_WheelTV_Create(const JoltC_WheelSettings* settings);

#ifdef __cplusplus
}
#endif

#endif /* JOLTC_VEHICLE_H */
