/* JoltC - C bindings for JoltPhysics
 * SPDX-License-Identifier: MIT
 *
 * Math utility functions (Vec3, Quat, Mat44, MassProperties, RayCast helpers).
 */

#ifndef JOLTC_MATH_H
#define JOLTC_MATH_H

#include <JoltC/common.h>

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */
/*  MassProperties (defined in common.h)                                      */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*  Math helpers                                                              */
/* -------------------------------------------------------------------------- */
JOLTC_API float JoltC_Math_Sin(float value);
JOLTC_API float JoltC_Math_Cos(float value);

/* -------------------------------------------------------------------------- */
/*  Vec3                                                                      */
/* -------------------------------------------------------------------------- */
JOLTC_API void  JoltC_Vec3_AxisX(JoltC_Vec3* result);
JOLTC_API void  JoltC_Vec3_AxisY(JoltC_Vec3* result);
JOLTC_API void  JoltC_Vec3_AxisZ(JoltC_Vec3* result);
JOLTC_API int   JoltC_Vec3_IsClose(const JoltC_Vec3* v1, const JoltC_Vec3* v2, float maxDistSq);
JOLTC_API int   JoltC_Vec3_IsNearZero(const JoltC_Vec3* v, float maxDistSq);
JOLTC_API int   JoltC_Vec3_IsNormalized(const JoltC_Vec3* v, float tolerance);
JOLTC_API int   JoltC_Vec3_IsNaN(const JoltC_Vec3* v);

JOLTC_API void  JoltC_Vec3_Negate(const JoltC_Vec3* v, JoltC_Vec3* result);
JOLTC_API void  JoltC_Vec3_Normalized(const JoltC_Vec3* v, JoltC_Vec3* result);
JOLTC_API void  JoltC_Vec3_Cross(const JoltC_Vec3* v1, const JoltC_Vec3* v2, JoltC_Vec3* result);
JOLTC_API void  JoltC_Vec3_Abs(const JoltC_Vec3* v, JoltC_Vec3* result);

JOLTC_API float JoltC_Vec3_Length(const JoltC_Vec3* v);
JOLTC_API float JoltC_Vec3_LengthSquared(const JoltC_Vec3* v);

JOLTC_API void  JoltC_Vec3_DotProduct(const JoltC_Vec3* v1, const JoltC_Vec3* v2, float* result);
JOLTC_API void  JoltC_Vec3_Normalize(const JoltC_Vec3* v, JoltC_Vec3* result);

JOLTC_API void  JoltC_Vec3_Add(const JoltC_Vec3* v1, const JoltC_Vec3* v2, JoltC_Vec3* result);
JOLTC_API void  JoltC_Vec3_Subtract(const JoltC_Vec3* v1, const JoltC_Vec3* v2, JoltC_Vec3* result);
JOLTC_API void  JoltC_Vec3_Multiply(const JoltC_Vec3* v1, const JoltC_Vec3* v2, JoltC_Vec3* result);
JOLTC_API void  JoltC_Vec3_MultiplyScalar(const JoltC_Vec3* v, float scalar, JoltC_Vec3* result);

JOLTC_API void  JoltC_Vec3_Divide(const JoltC_Vec3* v1, const JoltC_Vec3* v2, JoltC_Vec3* result);
JOLTC_API void  JoltC_Vec3_DivideScalar(const JoltC_Vec3* v, float scalar, JoltC_Vec3* result);

JOLTC_API void  JoltC_Vec3_MultiplyMatrix(const JoltC_Vec3* v, const JoltC_Mat44* mat, JoltC_Vec3* result);

/* -------------------------------------------------------------------------- */
/*  Quat                                                                      */
/* -------------------------------------------------------------------------- */
JOLTC_API void  JoltC_Quat_FromTo(const JoltC_Vec3* from, const JoltC_Vec3* to, JoltC_Quat* result);
JOLTC_API void  JoltC_Quat_GetAxisAngle(const JoltC_Quat* quat, JoltC_Vec3* outAxis, float* outAngle);
JOLTC_API void  JoltC_Quat_GetEulerAngles(const JoltC_Quat* quat, JoltC_Vec3* result);
JOLTC_API void  JoltC_Quat_RotateAxisX(const JoltC_Quat* quat, JoltC_Vec3* result);
JOLTC_API void  JoltC_Quat_RotateAxisY(const JoltC_Quat* quat, JoltC_Vec3* result);
JOLTC_API void  JoltC_Quat_RotateAxisZ(const JoltC_Quat* quat, JoltC_Vec3* result);
JOLTC_API void  JoltC_Quat_Inversed(const JoltC_Quat* quat, JoltC_Quat* result);
JOLTC_API float JoltC_Quat_GetRotationAngle(const JoltC_Quat* quat, const JoltC_Vec3* axis);
JOLTC_API void  JoltC_Quat_FromEulerAngles(const JoltC_Vec3* angles, JoltC_Quat* result);

JOLTC_API void  JoltC_Quat_Add(const JoltC_Quat* q1, const JoltC_Quat* q2, JoltC_Quat* result);
JOLTC_API void  JoltC_Quat_Subtract(const JoltC_Quat* q1, const JoltC_Quat* q2, JoltC_Quat* result);
JOLTC_API void  JoltC_Quat_Multiply(const JoltC_Quat* q1, const JoltC_Quat* q2, JoltC_Quat* result);
JOLTC_API void  JoltC_Quat_MultiplyScalar(const JoltC_Quat* q, float scalar, JoltC_Quat* result);
JOLTC_API void  JoltC_Quat_DivideScalar(const JoltC_Quat* q, float scalar, JoltC_Quat* result);
JOLTC_API void  JoltC_Quat_Dot(const JoltC_Quat* q1, const JoltC_Quat* q2, float* result);

JOLTC_API void  JoltC_Quat_Conjugated(const JoltC_Quat* quat, JoltC_Quat* result);
JOLTC_API void  JoltC_Quat_GetTwist(const JoltC_Quat* quat, const JoltC_Vec3* axis, JoltC_Quat* result);
JOLTC_API void  JoltC_Quat_GetSwingTwist(const JoltC_Quat* quat, JoltC_Quat* outSwing, JoltC_Quat* outTwist);
JOLTC_API void  JoltC_Quat_Lerp(const JoltC_Quat* from, const JoltC_Quat* to, float fraction, JoltC_Quat* result);
JOLTC_API void  JoltC_Quat_Slerp(const JoltC_Quat* from, const JoltC_Quat* to, float fraction, JoltC_Quat* result);
JOLTC_API void  JoltC_Quat_Rotate(const JoltC_Quat* quat, const JoltC_Vec3* vec, JoltC_Vec3* result);
JOLTC_API void  JoltC_Quat_InverseRotate(const JoltC_Quat* quat, const JoltC_Vec3* vec, JoltC_Vec3* result);
JOLTC_API void  JoltC_Quat_GetPerpendicular(const JoltC_Quat* quat, JoltC_Quat* result);

/* -------------------------------------------------------------------------- */
/*  Mat44                                                                     */
/* -------------------------------------------------------------------------- */
JOLTC_API void  JoltC_Mat44_Zero(JoltC_Mat44* result);
JOLTC_API void  JoltC_Mat44_Identity(JoltC_Mat44* result);
JOLTC_API void  JoltC_Mat44_Rotation(JoltC_Mat44* result, const JoltC_Quat* rotation);
JOLTC_API void  JoltC_Mat44_Rotation2(JoltC_Mat44* result, const JoltC_Vec3* axis, float angle);
JOLTC_API void  JoltC_Mat44_Translation(JoltC_Mat44* result, const JoltC_Vec3* translation);
JOLTC_API void  JoltC_Mat44_RotationTranslation(JoltC_Mat44* result, const JoltC_Quat* rotation, const JoltC_Vec3* translation);
JOLTC_API void  JoltC_Mat44_InverseRotationTranslation(JoltC_Mat44* result, const JoltC_Quat* rotation, const JoltC_Vec3* translation);
JOLTC_API void  JoltC_Mat44_Scale(JoltC_Mat44* result, const JoltC_Vec3* scale);
JOLTC_API void  JoltC_Mat44_Transposed(const JoltC_Mat44* m, JoltC_Mat44* result);
JOLTC_API void  JoltC_Mat44_Inversed(const JoltC_Mat44* m, JoltC_Mat44* result);

JOLTC_API void  JoltC_Mat44_Add(const JoltC_Mat44* m1, const JoltC_Mat44* m2, JoltC_Mat44* result);
JOLTC_API void  JoltC_Mat44_Subtract(const JoltC_Mat44* m1, const JoltC_Mat44* m2, JoltC_Mat44* result);
JOLTC_API void  JoltC_Mat44_Multiply(const JoltC_Mat44* m1, const JoltC_Mat44* m2, JoltC_Mat44* result);
JOLTC_API void  JoltC_Mat44_MultiplyScalar(const JoltC_Mat44* m, float scalar, JoltC_Mat44* result);

JOLTC_API void  JoltC_Mat44_GetAxisX(const JoltC_Mat44* m, JoltC_Vec3* result);
JOLTC_API void  JoltC_Mat44_GetAxisY(const JoltC_Mat44* m, JoltC_Vec3* result);
JOLTC_API void  JoltC_Mat44_GetAxisZ(const JoltC_Mat44* m, JoltC_Vec3* result);
JOLTC_API void  JoltC_Mat44_GetTranslation(const JoltC_Mat44* m, JoltC_Vec3* result);
JOLTC_API void  JoltC_Mat44_GetQuaternion(const JoltC_Mat44* m, JoltC_Quat* result);

/* -------------------------------------------------------------------------- */
/*  RMat44 (same storage as Mat44; uses RVec3 for translation)                */
/* -------------------------------------------------------------------------- */
JOLTC_API void  JoltC_RMat44_Identity(JoltC_Mat44* result);
JOLTC_API void  JoltC_RMat44_Zero(JoltC_Mat44* result);
JOLTC_API void  JoltC_RMat44_Rotation(JoltC_Mat44* result, const JoltC_Quat* rotation);
JOLTC_API void  JoltC_RMat44_RotationTranslation(JoltC_Mat44* result, const JoltC_Quat* rotation, const JoltC_RVec3* translation);
JOLTC_API void  JoltC_RMat44_Translation(JoltC_Mat44* result, const JoltC_RVec3* translation);
JOLTC_API void  JoltC_RMat44_Scale(JoltC_Mat44* result, const JoltC_Vec3* scale);
JOLTC_API void  JoltC_RMat44_Inversed(const JoltC_Mat44* m, JoltC_Mat44* result);
JOLTC_API void  JoltC_RMat44_InversedRotationTranslation(const JoltC_Mat44* m, JoltC_Mat44* result);

/* -------------------------------------------------------------------------- */
/*  Mat4 aliases (shortened names forwarding to Mat44)                         */
/* -------------------------------------------------------------------------- */
JOLTC_API JoltC_Mat44 JoltC_Mat4_Identity(void);
JOLTC_API JoltC_Mat44 JoltC_Mat4_Zero(void);
JOLTC_API JoltC_Mat44 JoltC_Mat4_Rotation(JoltC_Quat rotation);
JOLTC_API JoltC_Mat44 JoltC_Mat4_Rotation2(JoltC_Vec3 axis, float angle);
JOLTC_API JoltC_Mat44 JoltC_Mat4_RotationTranslation(JoltC_Quat rotation, JoltC_Vec3 translation);
JOLTC_API JoltC_Mat44 JoltC_Mat4_Translation(JoltC_Vec3 translation);
JOLTC_API JoltC_Mat44 JoltC_Mat4_Scale(JoltC_Vec3 scale);
JOLTC_API JoltC_Mat44 JoltC_Mat4_Inversed(JoltC_Mat44 mat);
JOLTC_API JoltC_Mat44 JoltC_Mat4_InverseRotationTranslation(JoltC_Mat44 mat);
JOLTC_API JoltC_Mat44 JoltC_Mat4_Transposed(JoltC_Mat44 mat);
JOLTC_API JoltC_Mat44 JoltC_Mat4_Multiply(JoltC_Mat44 a, JoltC_Mat44 b);
JOLTC_API JoltC_Mat44 JoltC_Mat4_MultiplyScalar(JoltC_Mat44 mat, float scalar);
JOLTC_API JoltC_Mat44 JoltC_Mat4_Add(JoltC_Mat44 a, JoltC_Mat44 b);
JOLTC_API JoltC_Mat44 JoltC_Mat4_Subtract(JoltC_Mat44 a, JoltC_Mat44 b);
JOLTC_API JoltC_Vec3  JoltC_Mat4_GetAxisX(JoltC_Mat44 mat);
JOLTC_API JoltC_Vec3  JoltC_Mat4_GetAxisY(JoltC_Mat44 mat);
JOLTC_API JoltC_Vec3  JoltC_Mat4_GetAxisZ(JoltC_Mat44 mat);
JOLTC_API JoltC_Vec3  JoltC_Mat4_GetTranslation(JoltC_Mat44 mat);
JOLTC_API JoltC_Quat  JoltC_Mat4_GetQuaternion(JoltC_Mat44 mat);

/* -------------------------------------------------------------------------- */
/*  RMat4 aliases (shortened names forwarding to RMat44)                       */
/* -------------------------------------------------------------------------- */
JOLTC_API JoltC_Mat44 JoltC_RMat4_Identity(void);
JOLTC_API JoltC_Mat44 JoltC_RMat4_Zero(void);
JOLTC_API JoltC_Mat44 JoltC_RMat4_Rotation(JoltC_Quat rotation);
JOLTC_API JoltC_Mat44 JoltC_RMat4_RotationTranslation(JoltC_Quat rotation, JoltC_RVec3 translation);
JOLTC_API JoltC_Mat44 JoltC_RMat4_Translation(JoltC_RVec3 translation);
JOLTC_API JoltC_Mat44 JoltC_RMat4_Scale(JoltC_Vec3 scale);
JOLTC_API JoltC_Mat44 JoltC_RMat4_Inversed(JoltC_Mat44 mat);
JOLTC_API JoltC_Mat44 JoltC_RMat4_InverseRotationTranslation(JoltC_Mat44 mat);

/* -------------------------------------------------------------------------- */
/*  RayCast helpers                                                           */
/* -------------------------------------------------------------------------- */
JOLTC_API void  JoltC_RayCast_GetPointOnRay(const JoltC_Vec3* origin, const JoltC_Vec3* direction, float fraction, JoltC_Vec3* result);
JOLTC_API void  JoltC_RRayCast_GetPointOnRay(const JoltC_RVec3* origin, const JoltC_Vec3* direction, float fraction, JoltC_RVec3* result);

/* -------------------------------------------------------------------------- */
/*  MassProperties helpers                                                    */
/* -------------------------------------------------------------------------- */
JOLTC_API void  JoltC_MassProperties_DecomposePrincipalMomentsOfInertia(JoltC_MassProperties* properties, JoltC_Mat44* rotation, JoltC_Vec3* diagonal);
JOLTC_API void  JoltC_MassProperties_ScaleToMass(JoltC_MassProperties* properties, float mass);
JOLTC_API void  JoltC_MassProperties_GetEquivalentSolidBoxSize(float mass, const JoltC_Vec3* inertiaDiagonal, JoltC_Vec3* result);

#ifdef __cplusplus
}
#endif

#endif /* JOLTC_MATH_H */
