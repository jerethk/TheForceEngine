#include "gs_player.h"
#include "assert.h"
#include <TFE_DarkForces/player.h>
#include <TFE_DarkForces/mission.h>
#include <TFE_DarkForces/pickup.h>
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

		if (s_playerInfo.health <= 0)
		{
			killPlayer();
		}
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

	void GS_Player::addToHealth(s32 value)
	{
		s_playerInfo.health += value;

		if (s_playerInfo.health <= 0)
		{
			killPlayer();
		}
	}

	void GS_Player::addToShields(s32 value)
	{
		s_playerInfo.shields += value;
	}

	bool GS_Player::hasRedKey()
	{
		return s_playerInfo.itemRedKey == JTRUE;
	}

	bool GS_Player::hasBlueKey()
	{
		return s_playerInfo.itemBlueKey == JTRUE;
	}

	bool GS_Player::hasYellowKey()
	{
		return s_playerInfo.itemYellowKey == JTRUE;
	}

	bool GS_Player::hasGoggles()
	{
		return s_playerInfo.itemGoggles == JTRUE;
	}

	bool GS_Player::hasCleats()
	{
		return s_playerInfo.itemCleats == JTRUE;
	}

	bool GS_Player::hasMask()
	{
		return s_playerInfo.itemMask == JTRUE;
	}

	void GS_Player::giveRedKey()
	{
		s_playerInfo.itemRedKey = JTRUE;
	}

	void GS_Player::giveBlueKey()
	{
		s_playerInfo.itemBlueKey = JTRUE;
	}

	void GS_Player::giveYellowKey()
	{
		s_playerInfo.itemYellowKey = JTRUE;
	}

	void GS_Player::giveGoggles()
	{
		s_playerInfo.itemGoggles = JTRUE;
	}

	void GS_Player::giveCleats()
	{
		s_playerInfo.itemCleats = JTRUE;
	}

	void GS_Player::giveMask()
	{
		s_playerInfo.itemMask = JTRUE;
	}

	void GS_Player::removeRedKey()
	{
		s_playerInfo.itemRedKey = JFALSE;
	}

	void GS_Player::removeBlueKey()
	{
		s_playerInfo.itemBlueKey = JFALSE;
	}

	void GS_Player::removeYellowKey()
	{
		s_playerInfo.itemYellowKey = JFALSE;
	}

	void GS_Player::removeGoggles()
	{
		if (s_nightVisionActive)
		{
			disableNightVision();
		}
		
		s_playerInfo.itemGoggles = JFALSE;
	}

	void GS_Player::removeCleats()
	{
		if (s_wearingCleats)
		{
			disableCleats();
		}
		
		s_playerInfo.itemCleats = JFALSE;
	}

	void GS_Player::removeMask()
	{
		if (s_wearingGasmask)
		{
			disableMask();
		}
		
		s_playerInfo.itemMask = JFALSE;
	}

	bool GS_Player::hasPistol()
	{
		return s_playerInfo.itemPistol == JTRUE;
	}

	void GS_Player::givePistol()
	{
		s_playerInfo.itemPistol = JTRUE;
	}

	bool GS_Player::hasRifle() 
	{
		return s_playerInfo.itemRifle == JTRUE;
	}

	void GS_Player::giveRifle()
	{
		s_playerInfo.itemRifle = JTRUE;
	}

	bool GS_Player::hasAutogun()
	{
		return s_playerInfo.itemAutogun == JTRUE;
	}

	void GS_Player::giveAutogun()
	{
		s_playerInfo.itemAutogun = JTRUE;
	}

	bool GS_Player::hasFusion()
	{
		return s_playerInfo.itemFusion == JTRUE;
	}

	void GS_Player::giveFusion()
	{
		s_playerInfo.itemFusion = JTRUE;
	}

	bool GS_Player::hasMortar()
	{
		return s_playerInfo.itemMortar == JTRUE;
	}

	void GS_Player::giveMortar()
	{
		s_playerInfo.itemMortar = JTRUE;
	}

	bool GS_Player::hasConcussion()
	{
		return s_playerInfo.itemConcussion == JTRUE;
	}

	void GS_Player::giveConcussion()
	{
		s_playerInfo.itemConcussion = JTRUE;
	}

	bool GS_Player::hasCannon()
	{
		return s_playerInfo.itemCannon == JTRUE;
	}

	void GS_Player::giveCannon()
	{
		s_playerInfo.itemCannon = JTRUE;
	}

	void GS_Player::removePistol()
	{
		s_playerInfo.itemPistol = JFALSE;
		// TODO - to implement this (and other weapons) properly, we have to make the weapon disappear from the screen if the player is currently holding it
	}

	void GS_Player::killPlayer()
	{
		sound_play(s_playerDeathSoundSource);
		if (s_gasSectorTask)
		{
			task_free(s_gasSectorTask);
		}
		s_playerInfo.health = 0;
		s_gasSectorTask = nullptr;
		s_playerDying = JTRUE;
		s_reviveTick = s_curTick + 436;
	}

	bool GS_Player::scriptRegister(ScriptAPI api)
	{
		ScriptClassBegin("Player", "player", api);
		{
			ScriptObjMethod("void kill()", killPlayer);
			
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

			ScriptObjMethod("void addToHealth(int)", addToHealth);
			ScriptObjMethod("void addToShields(int)", addToShields);

			// Items
			ScriptObjMethod("bool hasRedKey()", hasRedKey);
			ScriptObjMethod("bool hasBlueKey()", hasBlueKey);
			ScriptObjMethod("bool hasYellowKey()", hasYellowKey);
			ScriptObjMethod("bool hasGoggles()", hasGoggles);
			ScriptObjMethod("bool hasCleats()", hasCleats);
			ScriptObjMethod("bool hasMask()", hasMask);
			
			ScriptObjMethod("void giveRedKey()", giveRedKey);
			ScriptObjMethod("void giveBlueKey()", giveBlueKey);
			ScriptObjMethod("void giveYellowKey()", giveYellowKey);
			ScriptObjMethod("void giveGoggles()", giveGoggles);
			ScriptObjMethod("void giveCleats()", giveCleats);
			ScriptObjMethod("void giveMask()", giveMask);

			ScriptObjMethod("void removeRedKey()", removeRedKey);
			ScriptObjMethod("void removeBlueKey()", removeBlueKey);
			ScriptObjMethod("void removeYellowKey()", removeYellowKey);
			ScriptObjMethod("void removeGoggles()", removeGoggles);
			ScriptObjMethod("void removeCleats()", removeCleats);
			ScriptObjMethod("void removeMask()", removeMask);

			// Weapons
			ScriptObjMethod("bool hasPistol()", hasPistol);
			ScriptObjMethod("void givePistol()", givePistol);
			ScriptObjMethod("bool hasRifle()", hasRifle);
			ScriptObjMethod("void giveRifle()", giveRifle);
			ScriptObjMethod("bool hasAutogun()", hasAutogun);
			ScriptObjMethod("void giveAutogun()", giveAutogun);
			ScriptObjMethod("bool hasFusion()", hasFusion);
			ScriptObjMethod("void giveFusion()", giveFusion);
			ScriptObjMethod("bool hasMortar()", hasMortar);
			ScriptObjMethod("void giveMortar()", giveMortar);
			ScriptObjMethod("bool hasConcussion()", hasConcussion);
			ScriptObjMethod("void giveConcussion()", giveConcussion);
			ScriptObjMethod("bool hasCannon()", hasCannon);
			ScriptObjMethod("void giveCannon()", giveCannon);

			//ScriptObjMethod("void removePistol()", removePistol);
		}
		ScriptClassEnd();
	}
}