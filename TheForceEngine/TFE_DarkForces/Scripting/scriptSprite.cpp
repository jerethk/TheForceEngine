#include "scriptSprite.h"
#include "assert.h"
#include <angelscript.h>
#include <TFE_ForceScript/forceScript.h>
#include <TFE_ForceScript/scriptAPI.h>
#include <TFE_Jedi/Memory/allocator.h>
#include <TFE_Asset/spriteAsset_Jedi.h>

namespace TFE_DarkForces
{
	bool isSpriteValid(ScriptSprite* sprite)
	{
		return sprite->m_id >= 0 && sprite->m_id < TFE_Sprite_Jedi::getWaxCount(POOL_LEVEL);
	}

	void ScriptSprite::registerType()
	{
		s32 res = 0;
		asIScriptEngine* engine = (asIScriptEngine*)TFE_ForceScript::getEngine();

		ScriptValueType("Sprite");
		// Variables
		ScriptMemberVariable("int id", m_id);
		
		// Functions
		ScriptObjFunc("bool isValid()", isSpriteValid);
	}
}
