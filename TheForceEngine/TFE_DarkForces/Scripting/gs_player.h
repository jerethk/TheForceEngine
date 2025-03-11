#pragma once
#include <TFE_System/system.h>
#include <TFE_System/types.h>
#include <TFE_ForceScript/scriptInterface.h>
#include <string>

namespace TFE_DarkForces
{
	class GS_Player : public ScriptAPIClass
	{
	public:
		bool scriptRegister(ScriptAPI api) override;

		s32 getBatteryPercent();
		void setHealth(s32 value);
		void setShields(s32 value);
		void setBattery(s32 value);
		void setAmmoEnergy(s32 value);
		void setAmmoPower(s32 value);
		void setAmmoDetonator(s32 value);
		void setAmmoShell(s32 value);
		void setAmmoMine(s32 value);
		void setAmmoMissile(s32 value);
		void setAmmoPlasma(s32 value);

		void addToHealth(s32 value);
		void addToShields(s32 value);

		bool hasRedKey();
		bool hasBlueKey();
		bool hasYellowKey();
		bool hasGoggles();
		bool hasCleats();
		bool hasMask();
		
		void giveRedKey();
		void giveBlueKey();
		void giveYellowKey();
		void giveGoggles();
		void giveCleats();
		void giveMask();

		void removeRedKey();
		void removeBlueKey();
		void removeYellowKey();
		void removeGoggles();
		void removeCleats();
		void removeMask();

		bool hasPistol();
		void givePistol();
		bool hasRifle();
		void giveRifle();
		bool hasAutogun();
		void giveAutogun();
		bool hasFusion();
		void giveFusion();
		bool hasMortar();
		void giveMortar();
		bool hasConcussion();
		void giveConcussion();
		bool hasCannon();
		void giveCannon();

		void removePistol();
		void removeRifle();
		void removeAutogun();
		void removeFusion();
		void removeMortar();
		void removeConcussion();
		void removeCannon();

		void killPlayer();
	};
}