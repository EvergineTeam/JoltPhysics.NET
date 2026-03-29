/* JoltC - C bindings for JoltPhysics
 * SPDX-License-Identifier: MIT
 */

#ifndef JOLTC_COMMON_H
#define JOLTC_COMMON_H

#include <stdint.h>
#include <stddef.h>

/* -------------------------------------------------------------------------- */
/*  Export macro                                                              */
/* -------------------------------------------------------------------------- */
#ifdef _WIN32
#  ifdef JOLTC_EXPORTS
#    define JOLTC_API __declspec(dllexport)
#  else
#    define JOLTC_API __declspec(dllimport)
#  endif
#else
#  define JOLTC_API __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */
/*  Boolean                                                                   */
/* -------------------------------------------------------------------------- */
typedef int JoltC_Bool;
#define JOLTC_TRUE  1
#define JOLTC_FALSE 0

/* -------------------------------------------------------------------------- */
/*  Error handling                                                            */
/* -------------------------------------------------------------------------- */
JOLTC_API const char* JoltC_GetLastError(void);
JOLTC_API void        JoltC_ClearLastError(void);

/* -------------------------------------------------------------------------- */
/*  Init / Shutdown / Trace / Assert                                          */
/* -------------------------------------------------------------------------- */
JOLTC_API int  JoltC_Init(void);
JOLTC_API void JoltC_Shutdown(void);
JOLTC_API void JoltC_SetTraceHandler(void (*handler)(const char* message));
JOLTC_API void JoltC_SetAssertFailureHandler(int (*handler)(const char* expression, const char* message, const char* file, unsigned int line));

/* -------------------------------------------------------------------------- */
/*  Opaque handles                                                            */
/* -------------------------------------------------------------------------- */
typedef struct JoltC_PhysicsSystem         JoltC_PhysicsSystem;
typedef struct JoltC_BodyInterface         JoltC_BodyInterface;
typedef struct JoltC_TempAllocator         JoltC_TempAllocator;
typedef struct JoltC_JobSystem             JoltC_JobSystem;
typedef struct JoltC_Shape                 JoltC_Shape;
typedef struct JoltC_ShapeSettings         JoltC_ShapeSettings;
typedef struct JoltC_BroadPhaseLayerInterface       JoltC_BroadPhaseLayerInterface;
typedef struct JoltC_ObjectVsBroadPhaseLayerFilter   JoltC_ObjectVsBroadPhaseLayerFilter;
typedef struct JoltC_ObjectLayerPairFilter  JoltC_ObjectLayerPairFilter;
typedef struct JoltC_ContactListener       JoltC_ContactListener;
typedef struct JoltC_BodyActivationListener JoltC_BodyActivationListener;
typedef struct JoltC_Constraint            JoltC_Constraint;
typedef struct JoltC_CharacterVirtual      JoltC_CharacterVirtual;
typedef struct JoltC_CharacterContactListener JoltC_CharacterContactListener;
typedef struct JoltC_NarrowPhaseQuery      JoltC_NarrowPhaseQuery;
typedef struct JoltC_Body                    JoltC_Body;
typedef struct JoltC_MotionProperties        JoltC_MotionProperties;
typedef struct JoltC_BroadPhaseLayerFilter   JoltC_BroadPhaseLayerFilter;
typedef struct JoltC_ObjectLayerFilter       JoltC_ObjectLayerFilter;
typedef struct JoltC_BodyFilter              JoltC_BodyFilter;
typedef struct JoltC_ShapeFilter             JoltC_ShapeFilter;
typedef struct JoltC_SimShapeFilter          JoltC_SimShapeFilter;
typedef struct JoltC_PhysicsStepListener     JoltC_PhysicsStepListener;
typedef struct JoltC_PhysicsMaterial         JoltC_PhysicsMaterial;
typedef struct JoltC_GroupFilter             JoltC_GroupFilter;
typedef struct JoltC_BroadPhaseQuery         JoltC_BroadPhaseQuery;
typedef struct JoltC_ContactManifold         JoltC_ContactManifold;
typedef struct JoltC_HeightFieldShapeSettings JoltC_HeightFieldShapeSettings;
typedef struct JoltC_MeshShapeSettings       JoltC_MeshShapeSettings;
typedef struct JoltC_ConvexHullShapeSettings JoltC_ConvexHullShapeSettings;
typedef struct JoltC_Character               JoltC_Character;
typedef struct JoltC_CharacterBase           JoltC_CharacterBase;
typedef struct JoltC_CharacterVsCharacterCollision JoltC_CharacterVsCharacterCollision;

/* Vehicle system opaque handles */
typedef struct JoltC_LinearCurve                         JoltC_LinearCurve;
typedef struct JoltC_WheelSettings                       JoltC_WheelSettings;
typedef struct JoltC_WheelSettingsWV                     JoltC_WheelSettingsWV;
typedef struct JoltC_WheelSettingsTV                     JoltC_WheelSettingsTV;
typedef struct JoltC_Wheel                               JoltC_Wheel;
typedef struct JoltC_WheelWV                             JoltC_WheelWV;
typedef struct JoltC_WheelTV                             JoltC_WheelTV;
typedef struct JoltC_VehicleConstraint                   JoltC_VehicleConstraint;
typedef struct JoltC_VehicleController                   JoltC_VehicleController;
typedef struct JoltC_WheeledVehicleController            JoltC_WheeledVehicleController;
typedef struct JoltC_MotorcycleController                JoltC_MotorcycleController;
typedef struct JoltC_TrackedVehicleController            JoltC_TrackedVehicleController;
typedef struct JoltC_VehicleControllerSettings           JoltC_VehicleControllerSettings;
typedef struct JoltC_WheeledVehicleControllerSettings    JoltC_WheeledVehicleControllerSettings;
typedef struct JoltC_MotorcycleControllerSettings        JoltC_MotorcycleControllerSettings;
typedef struct JoltC_TrackedVehicleControllerSettings    JoltC_TrackedVehicleControllerSettings;
typedef struct JoltC_VehicleEngine                       JoltC_VehicleEngine;
typedef struct JoltC_VehicleTransmission                 JoltC_VehicleTransmission;
typedef struct JoltC_VehicleTransmissionSettings         JoltC_VehicleTransmissionSettings;
typedef struct JoltC_VehicleCollisionTester              JoltC_VehicleCollisionTester;
typedef struct JoltC_VehicleTrack                        JoltC_VehicleTrack;

/* Skeleton / Ragdoll opaque handles */
typedef struct JoltC_Skeleton                            JoltC_Skeleton;
typedef struct JoltC_SkeletonPose                        JoltC_SkeletonPose;
typedef struct JoltC_SkeletalAnimation                   JoltC_SkeletalAnimation;
typedef struct JoltC_SkeletonMapper                      JoltC_SkeletonMapper;
typedef struct JoltC_RagdollSettings                     JoltC_RagdollSettings;
typedef struct JoltC_Ragdoll                             JoltC_Ragdoll;

/* Constraint settings opaque handles */
typedef struct JoltC_TwoBodyConstraintSettings            JoltC_TwoBodyConstraintSettings;

/* Soft body opaque handles */
typedef struct JoltC_SoftBodyCreationSettings             JoltC_SoftBodyCreationSettings;

/* Collision estimation result (opaque, contains internal arrays) */
typedef struct JoltC_CollisionEstimationResult            JoltC_CollisionEstimationResult;

/* -------------------------------------------------------------------------- */
/*  BodyID — pass-by-value (uint32)                                           */
/* -------------------------------------------------------------------------- */
typedef uint32_t JoltC_BodyID;
#define JOLTC_BODY_ID_INVALID 0xFFFFFFFFU

/* -------------------------------------------------------------------------- */
/*  Object / Broadphase layer — pass by value                                 */
/* -------------------------------------------------------------------------- */
typedef uint16_t JoltC_ObjectLayer;
#define JOLTC_OBJECT_LAYER_INVALID 0xFFFFU

typedef uint8_t  JoltC_BroadPhaseLayer;

/* -------------------------------------------------------------------------- */
/*  SubShapeID — pass by value (uint32)                                       */
/* -------------------------------------------------------------------------- */
typedef uint32_t JoltC_SubShapeID;

/* -------------------------------------------------------------------------- */
/*  Collision Group IDs                                                       */
/* -------------------------------------------------------------------------- */
typedef uint32_t JoltC_CollisionGroupID;
typedef uint32_t JoltC_CollisionSubGroupID;
#define JOLTC_INVALID_COLLISION_GROUP_ID  (~0U)
#define JOLTC_INVALID_COLLISION_SUBGROUP_ID (~0U)

typedef struct JoltC_CollisionGroup {
    const JoltC_GroupFilter*   groupFilter;
    JoltC_CollisionGroupID     groupID;
    JoltC_CollisionSubGroupID  subGroupID;
} JoltC_CollisionGroup;

/* -------------------------------------------------------------------------- */
/*  SubShapeIDPair                                                            */
/* -------------------------------------------------------------------------- */
typedef struct JoltC_SubShapeIDPair {
    JoltC_BodyID     body1ID;
    JoltC_SubShapeID subShapeID1;
    JoltC_BodyID     body2ID;
    JoltC_SubShapeID subShapeID2;
} JoltC_SubShapeIDPair;

/* -------------------------------------------------------------------------- */
/*  Blittable math types                                                      */
/* -------------------------------------------------------------------------- */
typedef struct JoltC_Vec2 {
    float x, y;
} JoltC_Vec2;

typedef struct JoltC_Vec3 {
    float x, y, z;
} JoltC_Vec3;

typedef struct JoltC_Vec4 {
    float x, y, z, w;
} JoltC_Vec4;

typedef struct JoltC_Quat {
    float x, y, z, w;
} JoltC_Quat;

typedef struct JoltC_RVec3 {
#ifdef JOLTC_DOUBLE_PRECISION
    double x, y, z;
#else
    float x, y, z;
#endif
} JoltC_RVec3;

typedef struct JoltC_Mat44 {
    float m[16]; /* column-major */
} JoltC_Mat44;

typedef struct JoltC_AABox {
    JoltC_Vec3 min;
    JoltC_Vec3 max;
} JoltC_AABox;

typedef struct JoltC_Triangle {
    JoltC_Vec3 v1, v2, v3;
    uint32_t   materialIndex;
} JoltC_Triangle;

typedef struct JoltC_IndexedTriangle {
    uint32_t i1, i2, i3;
    uint32_t materialIndex;
    uint32_t userData;
} JoltC_IndexedTriangle;

/* -------------------------------------------------------------------------- */
/*  Enums                                                                     */
/* -------------------------------------------------------------------------- */
typedef enum JoltC_MotionType {
    JOLTC_MOTION_TYPE_STATIC    = 0,
    JOLTC_MOTION_TYPE_KINEMATIC = 1,
    JOLTC_MOTION_TYPE_DYNAMIC   = 2
} JoltC_MotionType;

typedef enum JoltC_MotionQuality {
    JOLTC_MOTION_QUALITY_DISCRETE    = 0,
    JOLTC_MOTION_QUALITY_LINEAR_CAST = 1
} JoltC_MotionQuality;

typedef enum JoltC_Activation {
    JOLTC_ACTIVATION_ACTIVATE      = 0,
    JOLTC_ACTIVATION_DONT_ACTIVATE = 1
} JoltC_Activation;

typedef enum JoltC_BodyType {
    JOLTC_BODY_TYPE_RIGID = 0,
    JOLTC_BODY_TYPE_SOFT  = 1
} JoltC_BodyType;

typedef enum JoltC_ShapeType {
    JOLTC_SHAPE_TYPE_CONVEX      = 0,
    JOLTC_SHAPE_TYPE_COMPOUND    = 1,
    JOLTC_SHAPE_TYPE_DECORATED   = 2,
    JOLTC_SHAPE_TYPE_MESH        = 3,
    JOLTC_SHAPE_TYPE_HEIGHT_FIELD = 4,
    JOLTC_SHAPE_TYPE_SOFT_BODY   = 5,
    JOLTC_SHAPE_TYPE_PLANE       = 9,
    JOLTC_SHAPE_TYPE_EMPTY       = 10
} JoltC_ShapeType;

typedef enum JoltC_ShapeSubType {
    JOLTC_SHAPE_SUB_TYPE_SPHERE               = 0,
    JOLTC_SHAPE_SUB_TYPE_BOX                  = 1,
    JOLTC_SHAPE_SUB_TYPE_TRIANGLE             = 2,
    JOLTC_SHAPE_SUB_TYPE_CAPSULE              = 3,
    JOLTC_SHAPE_SUB_TYPE_TAPERED_CAPSULE      = 4,
    JOLTC_SHAPE_SUB_TYPE_CYLINDER             = 5,
    JOLTC_SHAPE_SUB_TYPE_CONVEX_HULL          = 6,
    JOLTC_SHAPE_SUB_TYPE_STATIC_COMPOUND      = 7,
    JOLTC_SHAPE_SUB_TYPE_MUTABLE_COMPOUND     = 8,
    JOLTC_SHAPE_SUB_TYPE_ROTATED_TRANSLATED   = 9,
    JOLTC_SHAPE_SUB_TYPE_SCALED               = 10,
    JOLTC_SHAPE_SUB_TYPE_OFFSET_CENTER_OF_MASS = 11,
    JOLTC_SHAPE_SUB_TYPE_MESH                 = 12,
    JOLTC_SHAPE_SUB_TYPE_HEIGHT_FIELD         = 13,
    JOLTC_SHAPE_SUB_TYPE_SOFT_BODY            = 14
} JoltC_ShapeSubType;

typedef enum JoltC_ValidateResult {
    JOLTC_VALIDATE_RESULT_ACCEPT_ALL  = 0,
    JOLTC_VALIDATE_RESULT_ACCEPT      = 1,
    JOLTC_VALIDATE_RESULT_REJECT      = 2,
    JOLTC_VALIDATE_RESULT_REJECT_ALL  = 3
} JoltC_ValidateResult;

typedef enum JoltC_OverrideMassProperties {
    JOLTC_OVERRIDE_MASS_CALC_MASS_AND_INERTIA = 0,
    JOLTC_OVERRIDE_MASS_CALC_INERTIA          = 1,
    JOLTC_OVERRIDE_MASS_PROVIDED              = 2
} JoltC_OverrideMassProperties;

typedef enum JoltC_AllowedDOFs {
    JOLTC_ALLOWED_DOFS_ALL            = 0b111111,
    JOLTC_ALLOWED_DOFS_TRANSLATION_X  = 0b000001,
    JOLTC_ALLOWED_DOFS_TRANSLATION_Y  = 0b000010,
    JOLTC_ALLOWED_DOFS_TRANSLATION_Z  = 0b000100,
    JOLTC_ALLOWED_DOFS_ROTATION_X     = 0b001000,
    JOLTC_ALLOWED_DOFS_ROTATION_Y     = 0b010000,
    JOLTC_ALLOWED_DOFS_ROTATION_Z     = 0b100000,
    JOLTC_ALLOWED_DOFS_PLANE_2D       = 0b100011
} JoltC_AllowedDOFs;

typedef enum JoltC_GroundState {
    JOLTC_GROUND_STATE_ON_GROUND       = 0,
    JOLTC_GROUND_STATE_ON_STEEP_GROUND = 1,
    JOLTC_GROUND_STATE_NOT_SUPPORTED   = 2,
    JOLTC_GROUND_STATE_IN_AIR          = 3
} JoltC_GroundState;

typedef enum JoltC_ConstraintSpace {
    JOLTC_CONSTRAINT_SPACE_LOCAL_TO_BODY_COM = 0,
    JOLTC_CONSTRAINT_SPACE_WORLD_SPACE       = 1
} JoltC_ConstraintSpace;

typedef enum JoltC_MotorState {
    JOLTC_MOTOR_STATE_OFF      = 0,
    JOLTC_MOTOR_STATE_VELOCITY = 1,
    JOLTC_MOTOR_STATE_POSITION = 2
} JoltC_MotorState;

typedef enum JoltC_SpringMode {
    JOLTC_SPRING_MODE_FREQUENCY_AND_DAMPING = 0,
    JOLTC_SPRING_MODE_STIFFNESS_AND_DAMPING = 1
} JoltC_SpringMode;

typedef enum JoltC_ConstraintType {
    JOLTC_CONSTRAINT_TYPE_CONSTRAINT   = 0,
    JOLTC_CONSTRAINT_TYPE_TWO_BODY     = 1
} JoltC_ConstraintType;

typedef enum JoltC_ConstraintSubType {
    JOLTC_CONSTRAINT_SUB_TYPE_FIXED            = 0,
    JOLTC_CONSTRAINT_SUB_TYPE_POINT            = 1,
    JOLTC_CONSTRAINT_SUB_TYPE_HINGE            = 2,
    JOLTC_CONSTRAINT_SUB_TYPE_SLIDER           = 3,
    JOLTC_CONSTRAINT_SUB_TYPE_DISTANCE         = 4,
    JOLTC_CONSTRAINT_SUB_TYPE_CONE             = 5,
    JOLTC_CONSTRAINT_SUB_TYPE_SWING_TWIST      = 6,
    JOLTC_CONSTRAINT_SUB_TYPE_SIX_DOF          = 7,
    JOLTC_CONSTRAINT_SUB_TYPE_PATH             = 8,
    JOLTC_CONSTRAINT_SUB_TYPE_VEHICLE          = 9,
    JOLTC_CONSTRAINT_SUB_TYPE_RACK_AND_PINION  = 10,
    JOLTC_CONSTRAINT_SUB_TYPE_GEAR             = 11,
    JOLTC_CONSTRAINT_SUB_TYPE_PULLEY           = 12
} JoltC_ConstraintSubType;

typedef enum JoltC_SwingType {
    JOLTC_SWING_TYPE_CONE    = 0,
    JOLTC_SWING_TYPE_PYRAMID = 1
} JoltC_SwingType;

typedef enum JoltC_SixDOFConstraintAxis {
    JOLTC_SIX_DOF_TRANSLATION_X = 0,
    JOLTC_SIX_DOF_TRANSLATION_Y = 1,
    JOLTC_SIX_DOF_TRANSLATION_Z = 2,
    JOLTC_SIX_DOF_ROTATION_X    = 3,
    JOLTC_SIX_DOF_ROTATION_Y    = 4,
    JOLTC_SIX_DOF_ROTATION_Z    = 5,
    JOLTC_SIX_DOF_NUM           = 6
} JoltC_SixDOFConstraintAxis;

typedef enum JoltC_BackFaceMode {
    JOLTC_BACK_FACE_IGNORE   = 0,
    JOLTC_BACK_FACE_COLLIDE  = 1
} JoltC_BackFaceMode;

typedef enum JoltC_TransmissionMode {
    JOLTC_TRANSMISSION_MODE_AUTO   = 0,
    JOLTC_TRANSMISSION_MODE_MANUAL = 1
} JoltC_TransmissionMode;

typedef enum JoltC_TrackSide {
    JOLTC_TRACK_SIDE_LEFT  = 0,
    JOLTC_TRACK_SIDE_RIGHT = 1
} JoltC_TrackSide;

/* -------------------------------------------------------------------------- */
/*  Vehicle blittable types                                                   */
/* -------------------------------------------------------------------------- */
typedef struct JoltC_Point {
    float x;
    float y;
} JoltC_Point;

typedef struct JoltC_VehicleAntiRollBar {
    int   leftWheel;
    int   rightWheel;
    float stiffness;
} JoltC_VehicleAntiRollBar;

typedef struct JoltC_VehicleEngineSettings {
    float                    maxTorque;
    float                    minRPM;
    float                    maxRPM;
    const JoltC_LinearCurve* normalizedTorque;
    float                    inertia;
    float                    angularDamping;
} JoltC_VehicleEngineSettings;

typedef struct JoltC_VehicleDifferentialSettings {
    int   leftWheel;
    int   rightWheel;
    float differentialRatio;
    float leftRightSplit;
    float limitedSlipRatio;
    float engineTorqueRatio;
} JoltC_VehicleDifferentialSettings;

typedef struct JoltC_VehicleConstraintSettings {
    JoltC_Vec3                             up;
    JoltC_Vec3                             forward;
    float                                  maxPitchRollAngle;
    uint32_t                               wheelsCount;
    JoltC_WheelSettings**                  wheels;
    uint32_t                               antiRollBarsCount;
    const JoltC_VehicleAntiRollBar*        antiRollBars;
    JoltC_VehicleControllerSettings*       controller;
} JoltC_VehicleConstraintSettings;

typedef struct JoltC_VehicleTrackSettings {
    uint32_t        drivenWheel;
    const uint32_t* wheels;
    uint32_t        wheelsCount;
    float           inertia;
    float           angularDamping;
    float           maxBrakeTorque;
    float           differentialRatio;
} JoltC_VehicleTrackSettings;

typedef struct JoltC_SkeletonJoint {
    const char* name;
    const char* parentName;
    int         parentJointIndex;
} JoltC_SkeletonJoint;

typedef void (*JoltC_TireMaxImpulseCallback)(
    void*    userData,
    uint32_t wheelIndex,
    float*   outLongitudinalImpulse,
    float*   outLateralImpulse,
    float    suspensionImpulse,
    float    longitudinalFriction,
    float    lateralFriction,
    float    longitudinalSlip,
    float    lateralSlip,
    float    deltaTime);

/* -------------------------------------------------------------------------- */
/*  Spring / Motor settings — blittable structs                               */
/* -------------------------------------------------------------------------- */
typedef struct JoltC_SpringSettings {
    JoltC_SpringMode mode;
    float            frequencyOrStiffness;
    float            damping;
} JoltC_SpringSettings;

typedef struct JoltC_MotorSettings {
    JoltC_SpringSettings springSettings;
    float                minForceLimit;
    float                maxForceLimit;
    float                minTorqueLimit;
    float                maxTorqueLimit;
} JoltC_MotorSettings;

/* -------------------------------------------------------------------------- */
/*  RayCast result — blittable struct                                         */
/* -------------------------------------------------------------------------- */
typedef struct JoltC_RayCastResult {
    JoltC_BodyID bodyID;
    float        fraction;
    uint32_t     subShapeID2;
} JoltC_RayCastResult;

typedef struct JoltC_RayCastSettings {
    JoltC_BackFaceMode backFaceModeTriangles;
    JoltC_BackFaceMode backFaceModeConvex;
    JoltC_Bool         treatConvexAsSolid;
} JoltC_RayCastSettings;

/* -------------------------------------------------------------------------- */
/*  CollidePoint result                                                       */
/* -------------------------------------------------------------------------- */
typedef struct JoltC_CollidePointResult {
    JoltC_BodyID     bodyID;
    JoltC_SubShapeID subShapeID2;
} JoltC_CollidePointResult;

/* -------------------------------------------------------------------------- */
/*  CollideShape result                                                       */
/* -------------------------------------------------------------------------- */
typedef struct JoltC_CollideShapeResult {
    JoltC_Vec3       contactPointOn1;
    JoltC_Vec3       contactPointOn2;
    JoltC_Vec3       penetrationAxis;
    float            penetrationDepth;
    JoltC_SubShapeID subShapeID1;
    JoltC_SubShapeID subShapeID2;
    JoltC_BodyID     bodyID2;
} JoltC_CollideShapeResult;

/* -------------------------------------------------------------------------- */
/*  ShapeCast result                                                          */
/* -------------------------------------------------------------------------- */
typedef struct JoltC_ShapeCastResult {
    JoltC_Vec3       contactPointOn1;
    JoltC_Vec3       contactPointOn2;
    JoltC_Vec3       penetrationAxis;
    float            penetrationDepth;
    JoltC_SubShapeID subShapeID1;
    JoltC_SubShapeID subShapeID2;
    JoltC_BodyID     bodyID2;
    float            fraction;
    JoltC_Bool       isBackFaceHit;
} JoltC_ShapeCastResult;

/* -------------------------------------------------------------------------- */
/*  BroadPhaseCast result                                                     */
/* -------------------------------------------------------------------------- */
typedef struct JoltC_BroadPhaseCastResult {
    JoltC_BodyID bodyID;
    float        fraction;
} JoltC_BroadPhaseCastResult;

/* -------------------------------------------------------------------------- */
/*  CollideShape / ShapeCast settings (blittable)                             */
/* -------------------------------------------------------------------------- */
typedef struct JoltC_CollideShapeSettings {
    JoltC_BackFaceMode backFaceModeTriangles;
    JoltC_BackFaceMode backFaceModeConvex;
    float              maxSeparationDistance;
    float              collisionTolerance;
} JoltC_CollideShapeSettings;

typedef struct JoltC_ShapeCastSettings {
    JoltC_BackFaceMode backFaceModeTriangles;
    JoltC_BackFaceMode backFaceModeConvex;
    JoltC_Bool         useShrunkenShapeAndConvexRadius;
    JoltC_Bool         returnDeepestPoint;
    float              collisionTolerance;
    float              penetrationTolerance;
} JoltC_ShapeCastSettings;

/* -------------------------------------------------------------------------- */
/*  ContactSettings — enriched contact listener data (blittable)              */
/* -------------------------------------------------------------------------- */
typedef struct JoltC_ContactSettings {
    float       combinedFriction;
    float       combinedRestitution;
    float       invMassScale1;
    float       invInertiaScale1;
    float       invMassScale2;
    float       invInertiaScale2;
    JoltC_Bool  isSensor;
    JoltC_Vec3  relativeLinearSurfaceVelocity;
    JoltC_Vec3  relativeAngularSurfaceVelocity;
} JoltC_ContactSettings;

/* -------------------------------------------------------------------------- */
/*  MassProperties — blittable                                                */
/* -------------------------------------------------------------------------- */
typedef struct JoltC_MassProperties {
    float        mass;
    JoltC_Mat44  inertia;
} JoltC_MassProperties;

/* -------------------------------------------------------------------------- */
/*  Body creation settings — blittable struct                                 */
/* -------------------------------------------------------------------------- */
typedef struct JoltC_BodyCreationSettings {
    JoltC_RVec3                 position;
    JoltC_Quat                  rotation;
    JoltC_Vec3                  linearVelocity;
    JoltC_Vec3                  angularVelocity;
    uint64_t                    userData;
    JoltC_ObjectLayer           objectLayer;
    JoltC_CollisionGroup        collisionGroup;
    JoltC_MotionType            motionType;
    JoltC_AllowedDOFs           allowedDOFs;
    JoltC_Bool                  allowDynamicOrKinematic;
    JoltC_Bool                  isSensor;
    JoltC_Bool                  collideKinematicVsNonDynamic;
    JoltC_Bool                  useManifoldReduction;
    JoltC_Bool                  applyGyroscopicForce;
    JoltC_MotionQuality         motionQuality;
    JoltC_Bool                  enhancedInternalEdgeRemoval;
    JoltC_Bool                  allowSleeping;
    float                       friction;
    float                       restitution;
    float                       linearDamping;
    float                       angularDamping;
    float                       maxLinearVelocity;
    float                       maxAngularVelocity;
    float                       gravityFactor;
    uint32_t                    numVelocityStepsOverride;
    uint32_t                    numPositionStepsOverride;
    JoltC_OverrideMassProperties overrideMassProperties;
    float                       inertiaMultiplier;
    JoltC_MassProperties        massPropertiesOverride;
    /* Shape handle — must be set before passing to CreateBody */
    const JoltC_Shape*          shape;
} JoltC_BodyCreationSettings;

/* Helper to fill defaults matching JoltPhysics defaults */
JOLTC_API void JoltC_BodyCreationSettings_SetDefault(JoltC_BodyCreationSettings* settings);

/* -------------------------------------------------------------------------- */
/*  PhysicsSettings — blittable struct matching JPH::PhysicsSettings           */
/* -------------------------------------------------------------------------- */
typedef struct JoltC_PhysicsSettings {
    int         maxInFlightBodyPairs;
    int         stepListenersBatchSize;
    int         stepListenerBatchesPerJob;
    float       baumgarte;
    float       speculativeContactDistance;
    float       penetrationSlop;
    float       linearCastThreshold;
    float       linearCastMaxPenetration;
    float       manifoldTolerance;
    float       maxPenetrationDistance;
    float       bodyPairCacheMaxDeltaPositionSq;
    float       bodyPairCacheCosMaxDeltaRotationDiv2;
    float       contactNormalCosMaxDeltaRotation;
    float       contactPointPreserveLambdaMaxDistSq;
    uint32_t    numVelocitySteps;
    uint32_t    numPositionSteps;
    float       minVelocityForRestitution;
    float       timeBeforeSleep;
    float       pointVelocitySleepThreshold;
    JoltC_Bool  deterministicSimulation;
    JoltC_Bool  constraintWarmStart;
    JoltC_Bool  useBodyPairContactCache;
    JoltC_Bool  useManifoldReduction;
    JoltC_Bool  useLargeIslandSplitter;
    JoltC_Bool  allowSleeping;
    JoltC_Bool  checkActiveEdges;
} JoltC_PhysicsSettings;

/* -------------------------------------------------------------------------- */
/*  Callback typedefs                                                         */
/* -------------------------------------------------------------------------- */

/* BroadPhaseLayerInterface callbacks */
typedef uint32_t (*JoltC_GetNumBroadPhaseLayersFn)(void* userData);
typedef JoltC_BroadPhaseLayer (*JoltC_GetBroadPhaseLayerFn)(void* userData, JoltC_ObjectLayer layer);

/* ObjectVsBroadPhaseLayerFilter callback */
typedef JoltC_Bool (*JoltC_ObjectVsBroadPhaseLayerFilterFn)(void* userData, JoltC_ObjectLayer layer1, JoltC_BroadPhaseLayer layer2);

/* ObjectLayerPairFilter callback */
typedef JoltC_Bool (*JoltC_ObjectLayerPairFilterFn)(void* userData, JoltC_ObjectLayer layer1, JoltC_ObjectLayer layer2);

/* Contact listener callbacks */
typedef JoltC_ValidateResult (*JoltC_OnContactValidateFn)(void* userData, JoltC_BodyID body1, JoltC_BodyID body2);
typedef void (*JoltC_OnContactAddedFn)(void* userData, JoltC_BodyID body1, JoltC_BodyID body2);
typedef void (*JoltC_OnContactPersistedFn)(void* userData, JoltC_BodyID body1, JoltC_BodyID body2);
typedef void (*JoltC_OnContactRemovedFn)(void* userData, JoltC_BodyID body1, JoltC_BodyID body2);

/* Body activation listener callbacks */
typedef void (*JoltC_OnBodyActivatedFn)(void* userData, JoltC_BodyID bodyID, uint64_t bodyUserData);
typedef void (*JoltC_OnBodyDeactivatedFn)(void* userData, JoltC_BodyID bodyID, uint64_t bodyUserData);

/* -------------------------------------------------------------------------- */
/*  Filter callback typedefs                                                  */
/* -------------------------------------------------------------------------- */
typedef JoltC_Bool (*JoltC_BroadPhaseLayerFilterFn)(void* userData, JoltC_BroadPhaseLayer layer);
typedef JoltC_Bool (*JoltC_ObjectLayerFilterFn)(void* userData, JoltC_ObjectLayer layer);
typedef JoltC_Bool (*JoltC_BodyFilterFn)(void* userData, JoltC_BodyID bodyID);
typedef JoltC_Bool (*JoltC_BodyFilterLockedFn)(void* userData, const JoltC_Body* body);
typedef JoltC_Bool (*JoltC_ShapeFilterFn)(void* userData, const JoltC_Shape* shape2, JoltC_SubShapeID subShapeID2);
typedef JoltC_Bool (*JoltC_ShapeFilter2Fn)(void* userData, const JoltC_Shape* shape1, JoltC_SubShapeID subShapeID1,
                                           const JoltC_Shape* shape2, JoltC_SubShapeID subShapeID2);
typedef JoltC_Bool (*JoltC_SimShapeFilterFn)(void* userData,
    const JoltC_Body* body1, const JoltC_Shape* shape1, JoltC_SubShapeID subShapeID1,
    const JoltC_Body* body2, const JoltC_Shape* shape2, JoltC_SubShapeID subShapeID2);

/* -------------------------------------------------------------------------- */
/*  Collector & result callback typedefs                                      */
/* -------------------------------------------------------------------------- */
typedef void (*JoltC_CollideShapeResultFn)(void* userData, const JoltC_CollideShapeResult* result);
typedef void (*JoltC_CastShapeResultFn)(void* userData, const JoltC_ShapeCastResult* result);
typedef void (*JoltC_BroadPhaseCastResultFn)(void* userData, const JoltC_BroadPhaseCastResult* result);
typedef void (*JoltC_CollideShapeBodyResultFn)(void* userData, JoltC_BodyID bodyID);

/* -------------------------------------------------------------------------- */
/*  Enhanced contact listener callbacks (with Body/ContactManifold/Settings)  */
/* -------------------------------------------------------------------------- */
typedef JoltC_ValidateResult (*JoltC_OnContactValidateEnhancedFn)(void* userData,
    const JoltC_Body* body1, const JoltC_Body* body2,
    JoltC_RVec3 baseOffset, const JoltC_CollideShapeResult* collisionResult);
typedef void (*JoltC_OnContactAddedEnhancedFn)(void* userData,
    const JoltC_Body* body1, const JoltC_Body* body2,
    const JoltC_ContactManifold* manifold, JoltC_ContactSettings* settings);
typedef void (*JoltC_OnContactPersistedEnhancedFn)(void* userData,
    const JoltC_Body* body1, const JoltC_Body* body2,
    const JoltC_ContactManifold* manifold, JoltC_ContactSettings* settings);
typedef void (*JoltC_OnContactRemovedEnhancedFn)(void* userData,
    const JoltC_SubShapeIDPair* subShapePair);

/* -------------------------------------------------------------------------- */
/*  Step listener callback                                                    */
/* -------------------------------------------------------------------------- */
typedef void (*JoltC_OnPhysicsStepFn)(void* userData, float deltaTime, JoltC_Bool isFirstStep, JoltC_Bool isLastStep);

/* -------------------------------------------------------------------------- */
/*  Procs structs — function-pointer tables for SetProcs updates              */
/* -------------------------------------------------------------------------- */
typedef struct JoltC_BodyActivationListener_Procs {
    JoltC_OnBodyActivatedFn   onBodyActivated;
    JoltC_OnBodyDeactivatedFn onBodyDeactivated;
} JoltC_BodyActivationListener_Procs;

typedef struct JoltC_BodyFilter_Procs {
    JoltC_BodyFilterFn       shouldCollide;
    JoltC_BodyFilterLockedFn shouldCollideLocked;
} JoltC_BodyFilter_Procs;

typedef struct JoltC_BroadPhaseLayerFilter_Procs {
    JoltC_BroadPhaseLayerFilterFn shouldCollide;
} JoltC_BroadPhaseLayerFilter_Procs;

typedef struct JoltC_ContactListener_Procs {
    JoltC_OnContactValidateFn  onContactValidate;
    JoltC_OnContactAddedFn     onContactAdded;
    JoltC_OnContactPersistedFn onContactPersisted;
    JoltC_OnContactRemovedFn   onContactRemoved;
} JoltC_ContactListener_Procs;

typedef struct JoltC_ObjectLayerFilter_Procs {
    JoltC_ObjectLayerFilterFn shouldCollide;
} JoltC_ObjectLayerFilter_Procs;

typedef struct JoltC_PhysicsStepListener_Procs {
    JoltC_OnPhysicsStepFn onStep;
} JoltC_PhysicsStepListener_Procs;

typedef struct JoltC_ShapeFilter_Procs {
    JoltC_ShapeFilterFn  shouldCollide;
    JoltC_ShapeFilter2Fn shouldCollide2;
} JoltC_ShapeFilter_Procs;

typedef struct JoltC_SimShapeFilter_Procs {
    JoltC_SimShapeFilterFn shouldCollide;
} JoltC_SimShapeFilter_Procs;

/* -------------------------------------------------------------------------- */
/*  Init / Shutdown                                                           */
/* -------------------------------------------------------------------------- */
JOLTC_API void JoltC_RegisterDefaultAllocator(void);
JOLTC_API void JoltC_CreateFactory(void);
JOLTC_API void JoltC_DestroyFactory(void);
JOLTC_API void JoltC_RegisterTypes(void);
JOLTC_API void JoltC_UnregisterTypes(void);

#ifdef __cplusplus
}
#endif

#endif /* JOLTC_COMMON_H */
