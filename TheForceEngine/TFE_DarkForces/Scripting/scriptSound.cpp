#include "scriptSound.h"
#include "assert.h"
#include <angelscript.h>
#include <TFE_ForceScript/forceScript.h>
#include <TFE_ForceScript/scriptAPI.h>
#include <TFE_Jedi/Memory/allocator.h>
#include <TFE_DarkForces/sound.h>

namespace TFE_DarkForces
{
	bool isSoundValid(ScriptSound* sound)
	{
		return sound->m_id >= 0 && sound->m_id < sound_getCount();
	}

	void ScriptSound::registerType()
	{
		s32 res = 0;
		asIScriptEngine* engine = (asIScriptEngine*)TFE_ForceScript::getEngine();

		ScriptValueType("Sound");
		// Variables
		ScriptMemberVariable("int id", m_id);
		
		// Functions
		ScriptObjFunc("bool isValid()", isSoundValid);
	}
}
