#include "gs_player.h"
#include "assert.h"
#include <TFE_DarkForces/player.h>
#include <TFE_System/system.h>
#include <angelscript.h>

namespace TFE_DarkForces
{
	s32 GS_Player::getBatteryPercent()
	{
		return (s32)(100.0 * s_batteryPower / FIXED(2) );
	}

	void GS_Player::setHealth(s32 value)
	{
		s_playerInfo.health = value;
	}

	void GS_Player::setShields(s32 value)
	{
		s_playerInfo.shields = value;
	}

	void GS_Player::setBattery(s32 value)
	{
		s_batteryPower = (fixed16_16)((value / 100.0) * FIXED(2));
	}

	void GS_Player::setAmmoEnergy(s32 value)
	{
		s_playerInfo.ammoEnergy = value;
	}

	void GS_Player::setAmmoPower(s32 value)
	{
		s_playerInfo.ammoPower = value;
	}

	void GS_Player::setAmmoDetonator(s32 value)
	{
		s_playerInfo.ammoDetonator = value;
	}

	void GS_Player::setAmmoShell(s32 value)
	{
		s_playerInfo.ammoShell = value;
	}

	void GS_Player::setAmmoMine(s32 value)
	{
		s_playerInfo.ammoMine = value;
	}

	void GS_Player::setAmmoMissile(s32 value)
	{
		s_playerInfo.ammoMissile = value;
	}

	void GS_Player::setAmmoPlasma(s32 value)
	{
		s_playerInfo.ammoPlasma = value;
	}


	bool GS_Player::scriptRegister(ScriptAPI api)
	{
		ScriptClassBegin("Player", "player", api);
		{
			// Health / ammo getters
			ScriptLambdaPropertyGet("int get_health()", s32, { return s_playerInfo.health; });
			ScriptLambdaPropertyGet("int get_shields()", s32, { return s_playerInfo.shields; });
			ScriptLambdaPropertyGet("int get_ammoEnergy()", s32, { return s_playerInfo.ammoEnergy; });
			ScriptLambdaPropertyGet("int get_ammoPower()", s32, { return s_playerInfo.ammoPower; });
			ScriptLambdaPropertyGet("int get_ammoDetonator()", s32, { return s_playerInfo.ammoDetonator; });
			ScriptLambdaPropertyGet("int get_ammoShell()", s32, { return s_playerInfo.ammoShell; });
			ScriptLambdaPropertyGet("int get_ammoMine()", s32, { return s_playerInfo.ammoMine; });
			ScriptLambdaPropertyGet("int get_ammoPlasma()", s32, { return s_playerInfo.ammoPlasma; });
			ScriptLambdaPropertyGet("int get_ammoMissile()", s32, { return s_playerInfo.ammoMissile; });
			ScriptObjMethod("int get_battery()", getBatteryPercent);	// Get battery as a percentage

			// Health / ammo setters
			ScriptPropertySet("void set_health(int)", setHealth);
			ScriptPropertySet("void set_shields(int)", setShields);
			ScriptPropertySet("void set_battery(int)", setBattery);
			ScriptPropertySet("void set_ammoEnergy(int)", setAmmoEnergy);
			ScriptPropertySet("void set_ammoPower(int)", setAmmoPower);
			ScriptPropertySet("void set_ammoDetonator(int)", setAmmoDetonator);
			ScriptPropertySet("void set_ammoShell(int)", setAmmoShell);
			ScriptPropertySet("void set_ammoMine(int)", setAmmoMine);
			ScriptPropertySet("void set_ammoMissile(int)", setAmmoMissile);
			ScriptPropertySet("void set_ammoPlasma(int)", setAmmoPlasma);
		}
		ScriptClassEnd();
	}
}