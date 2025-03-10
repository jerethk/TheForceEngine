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
	};
}