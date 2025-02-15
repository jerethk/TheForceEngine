#pragma once
//////////////////////////////////////////////////////////////////////
// Dark Forces
// Core Actor/AI functionality.
// -----------------------------
// AI in Dark Forces is handled by a central "dispatcher" with a
// a number of "modules" attached that handle specific functionality.
// This is basically a primitive ECS-style system.
// 
// Each module has function pointers to handle updates, message, damage,
// and destruction. During the AI update, the dispatch will loop over
// each module and call its update function. There is also some communication
// between modules to keep redundant data up to date.
// 
// Dispatcher (ActorDispatcher in TFE):
//   + ActorModule modules[]
//   + MovementModule moveMod  -- this could have been added to the list
//                                but every AI actor has it.
// 
// For example, a Stormtrooper is:
//  Dispatcher
//    + Damage Module -
//        Holds HP, drop item, hurt and die sounds. This is the
//        getting hurt and dying functionality.
//    + Attack Module -
//        Handles attacking functionality, and includes the
//        projectile type, melee range, etc.
//    + Thinker Module -
//        Handles overall state and sets targets for movement.
//    + Movement Module -
//        Handles the actual movement, collision detection and
//        response, basic animation.
//////////////////////////////////////////////////////////////////////
#include "actorModule.h"
#include "../sound.h"
#include <TFE_System/types.h>
#include <TFE_DarkForces/logic.h>
#include <TFE_Jedi/Collision/collision.h>

namespace TFE_DarkForces
{
	enum ActorStateVersion : u32
	{
		ActorState_InitVersion = 1,
		ActorState_Teams = 2,
		ActorState_CurVersion = ActorState_Teams,
	};
	
	void actorDispatch_serialize(Logic*& logic, SecObject* obj, Stream* stream);

	// Helper Functions
	void actor_serializeObject(Stream* stream, SecObject*& obj, s32 saveVersion);
	void actor_serializeWall(Stream* stream, RWall*& wall);
	void actor_serializeCollisionInfo(Stream* stream, CollisionInfo* colInfo);
	void actor_serializeTarget(Stream* stream, ActorTarget* target);
	void actor_serializeMovementModule(Stream* stream, ActorModule*& mod, ActorDispatch* dispatch);
	void actor_serializeAttackModule(Stream* stream, ActorModule*& mod, ActorDispatch* dispatch);
	void actor_serializeDamageModule(Stream* stream, ActorModule*& mod, ActorDispatch* dispatch);
	void actor_serializeThinkerModule(Stream* stream, ActorModule*& mod, ActorDispatch* dispatch);
	void actor_serializeFlyerModule(Stream* stream, ActorModule*& mod, ActorDispatch* dispatch);
	void actor_serializeFlyerRemoteModule(Stream* stream, ActorModule*& mod, ActorDispatch* dispatch);

	void actor_serializeTiming(Stream* stream, ActorTiming* timing);
	void actor_serializeLogicAnim(Stream* stream, LogicAnimation* anim);
	void actor_serializeMovementModuleBase(Stream* stream, MovementModule* moveMod);
}  // namespace TFE_DarkForces