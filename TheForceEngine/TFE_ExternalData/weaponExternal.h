#pragma once
#include <TFE_System/types.h>

///////////////////////////////////////////
// TFE Externalised Projectile data
///////////////////////////////////////////

namespace TFE_ExternalData
{
	struct ExternalProjectile
	{
		const char* type = nullptr;
		const char* updateFunc = "";
		u32 damage;
		u32 falloffAmount;
		u32 nextFalloffTick;
		u32 damageFalloffDelta;
		u32 minDamage;
		u32 force;
		u32 speed;
		u32 horzBounciness;
		u32 vertBounciness;
		u32 bounceCount;
		u32 reflectVariation;
		u32 duration;
		const char* flightSound = "";
		const char* reflectSound = "";
		const char* cameraPassSound = "";
		u32 reflectEffectId = -1;
		u32 hitEffectId = -1;
	};

	struct ExternalEffect
	{
		const char* type = nullptr;
		const char* wax = "";
		s32 force;
		s32 damage;
		s32 explosiveRange;
		s32 wakeupRange;
		const char* soundEffect = "";
		s32 soundPriority;
	};

	void loadExternalProjectiles();
	bool validateExternalProjectiles();
	ExternalProjectile* getExternalProjectiles();
	void loadExternalEffects();
	bool validateExternalEffects();
	ExternalEffect* getExternalEffects();
}