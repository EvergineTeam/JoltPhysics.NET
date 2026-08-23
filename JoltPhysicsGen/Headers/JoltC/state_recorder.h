/* JoltC - C bindings for JoltPhysics
 * SPDX-License-Identifier: MIT
 *
 * Determinism and state: the in-memory StateRecorder, whole-system save and restore, and the
 * per-object SaveState family. The intended loop is: save, keep simulating, rewind, restore,
 * re-simulate the same steps and land on bit-identical state -- same binary, same machine.
 */

#ifndef JOLTC_STATE_RECORDER_H
#define JOLTC_STATE_RECORDER_H

#include <JoltC/common.h>

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */
/*  StateRecorderImpl - an in-memory stream                                   */
/* -------------------------------------------------------------------------- */
JOLTC_API JoltC_StateRecorder* JoltC_StateRecorderImpl_Create(void);
JOLTC_API void JoltC_StateRecorderImpl_Destroy(JoltC_StateRecorder* recorder);

/* Back to the start for reading: the save/restore loop is Save, Rewind, Restore. */
JOLTC_API void JoltC_StateRecorderImpl_Rewind(JoltC_StateRecorder* recorder);

/* Empties the stream so the recorder can hold a fresh snapshot. */
JOLTC_API void JoltC_StateRecorderImpl_Clear(JoltC_StateRecorder* recorder);

/* The snapshot as bytes, for shipping across a network or storing. CopyData writes at most
 * capacity bytes and returns how many the snapshot holds in total; SetData replaces the stream
 * contents with the given bytes, ready to Rewind and restore. */
JOLTC_API uint64_t JoltC_StateRecorderImpl_GetDataSize(const JoltC_StateRecorder* recorder);
JOLTC_API uint64_t JoltC_StateRecorderImpl_CopyData(const JoltC_StateRecorder* recorder, uint8_t* buffer, uint64_t capacity);
JOLTC_API void JoltC_StateRecorderImpl_SetData(JoltC_StateRecorder* recorder, const uint8_t* data, uint64_t size);

JOLTC_API JoltC_Bool JoltC_StateRecorder_IsEOF(const JoltC_StateRecorder* recorder);

/* In validating mode a restore checks the incoming data against the current state, which is how
 * Jolt's own determinism tests catch the exact place two simulations diverge. */
JOLTC_API void JoltC_StateRecorder_SetValidating(JoltC_StateRecorder* recorder, JoltC_Bool validating);
JOLTC_API JoltC_Bool JoltC_StateRecorder_IsValidating(const JoltC_StateRecorder* recorder);

/* -------------------------------------------------------------------------- */
/*  Whole-system state                                                        */
/* -------------------------------------------------------------------------- */
/* stateFlags is a JoltC_StateRecorderState combination; ALL is the replay/rollback snapshot. */
JOLTC_API void JoltC_PhysicsSystem_SaveState(const JoltC_PhysicsSystem* system, JoltC_StateRecorder* recorder, uint32_t stateFlags);
JOLTC_API JoltC_Bool JoltC_PhysicsSystem_RestoreState(JoltC_PhysicsSystem* system, JoltC_StateRecorder* recorder);

/* One body's worth of state, for syncing a handful of bodies instead of the world. */
JOLTC_API void JoltC_PhysicsSystem_SaveBodyState(const JoltC_PhysicsSystem* system, const JoltC_Body* body, JoltC_StateRecorder* recorder);
JOLTC_API void JoltC_PhysicsSystem_RestoreBodyState(JoltC_PhysicsSystem* system, JoltC_Body* body, JoltC_StateRecorder* recorder);

/* -------------------------------------------------------------------------- */
/*  Per-object state, for things the system snapshot does not include         */
/* -------------------------------------------------------------------------- */
JOLTC_API void JoltC_Constraint_SaveState(const JoltC_Constraint* constraint, JoltC_StateRecorder* recorder);
JOLTC_API void JoltC_Constraint_RestoreState(JoltC_Constraint* constraint, JoltC_StateRecorder* recorder);

/* A CharacterVirtual is not a body, so the system snapshot never sees it: saving it is always the
 * caller's job, and these are the calls to do it with. */
JOLTC_API void JoltC_CharacterVirtual_SaveState(const JoltC_CharacterVirtual* character, JoltC_StateRecorder* recorder);
JOLTC_API void JoltC_CharacterVirtual_RestoreState(JoltC_CharacterVirtual* character, JoltC_StateRecorder* recorder);
JOLTC_API void JoltC_Character_SaveState(const JoltC_Character* character, JoltC_StateRecorder* recorder);
JOLTC_API void JoltC_Character_RestoreState(JoltC_Character* character, JoltC_StateRecorder* recorder);

JOLTC_API void JoltC_SoftBodyMotionProperties_SaveState(const JoltC_SoftBodyMotionProperties* motionProperties, JoltC_StateRecorder* recorder);
JOLTC_API void JoltC_SoftBodyMotionProperties_RestoreState(JoltC_SoftBodyMotionProperties* motionProperties, JoltC_StateRecorder* recorder);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* JOLTC_STATE_RECORDER_H */
