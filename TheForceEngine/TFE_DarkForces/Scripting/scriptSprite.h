#pragma once

#include <TFE_System/system.h>
#include <TFE_System/types.h>

namespace TFE_DarkForces
{
	class ScriptSprite
	{
	public:
		ScriptSprite() : m_id(-1) {};
		ScriptSprite(s32 id) : m_id(id) {};
		void registerType();

	public:
		s32 m_id;
	};

	extern bool isSpriteValid(ScriptSprite* sprite);
}
