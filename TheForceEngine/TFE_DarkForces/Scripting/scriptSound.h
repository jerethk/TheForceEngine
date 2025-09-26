#pragma once

#include <TFE_System/system.h>
#include <TFE_System/types.h>

namespace TFE_DarkForces
{
	class ScriptSound
	{
	public:
		ScriptSound() : m_id(-1) {};
		ScriptSound(s32 id) : m_id(id) {};
		void registerType();

	public:
		s32 m_id;
	};

	extern bool isSoundValid(ScriptSound* sound);
}
