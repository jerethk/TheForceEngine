#include "scriptElev.h"
#include <TFE_ForceScript/ScriptAPI-Shared/scriptMath.h>
#include <TFE_Jedi/InfSystem/infState.h>
#include <TFE_Jedi/InfSystem/infTypesInternal.h>
#include <TFE_Jedi/InfSystem/infSystem.h>
#include <TFE_Jedi/Memory/allocator.h>
#include <TFE_System/system.h>
#include <angelscript.h>

using namespace TFE_Jedi;

namespace TFE_DarkForces
{
	bool ScriptElev::isScriptElevValid(ScriptElev* elev)
	{
		return elev->m_id >= 0 && elev->m_id < allocator_getCount(s_infSerState.infElevators);
	}
	// TODO- Replace by updateFlags and enum?
	bool getElevMaster(ScriptElev* elev)
	{
		if (!ScriptElev::isScriptElevValid(elev)) { return false; }
		InfElevator* data = (InfElevator*)allocator_getByIndex(s_infSerState.infElevators, elev->m_id);
		return (data->updateFlags & ELEV_MASTER_ON) != 0;
	}

	float getElevSpeed(ScriptElev* elev)
	{
		if (!ScriptElev::isScriptElevValid(elev)) { return 0; }

		InfElevator* data = (InfElevator*)allocator_getByIndex(s_infSerState.infElevators, elev->m_id);
		return data->type == IELEV_ROTATE_WALL ? angleToFloat(data->speed >> FRAC_BITS_16) : fixed16ToFloat(data->speed);
	}

	void setElevSpeed(float value, ScriptElev* elev)
	{
		if (!ScriptElev::isScriptElevValid(elev)) { return; }

		InfElevator* data = (InfElevator*)allocator_getByIndex(s_infSerState.infElevators, elev->m_id);
		data->speed = data->type == IELEV_ROTATE_WALL ? intToFixed16(floatToAngle(value)) : floatToFixed16(value);
	}

	void ScriptElev::registerType()
	{
		s32 res = 0;
		asIScriptEngine* engine = (asIScriptEngine*)TFE_ForceScript::getEngine();

		ScriptValueType("Elevator");

		// Variables
		ScriptMemberVariable("int id", m_id);
		// Functions
		ScriptObjFunc("bool isValid()", isScriptElevValid);
		// Properties
		ScriptPropertyGetFunc("bool get_master()", getElevMaster);

		ScriptPropertyGetFunc("float get_speed()", getElevSpeed);
		ScriptPropertySetFunc("void set_speed(float)", setElevSpeed);
	}
}
