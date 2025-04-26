#pragma once
//////////////////////////////////////////////////////////////////////
// Dark Forces Player -
// Player information and object data.
//////////////////////////////////////////////////////////////////////

#include <TFE_System/types.h>
#include <TFE_DarkForces/Actor/actorModule.h>
#include "logic.h"

namespace TFE_DarkForces
{
	struct PlayerLogic
	{
		Logic logic;

		vec2_fixed dir;
		vec3_fixed move;
		fixed16_16 stepHeight;

		LogicAnimation anim;
	};
}  // namespace TFE_DarkForces