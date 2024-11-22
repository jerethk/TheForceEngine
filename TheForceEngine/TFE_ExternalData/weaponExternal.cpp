#include <cstring>
#include <vector>
#include <TFE_System/cJSON.h>
#include <TFE_System/system.h>
#include <TFE_FileSystem/filestream.h>
#include <TFE_DarkForces/projectile.h>
#include "weaponExternal.h"
#include "logicTables.h"

namespace TFE_ExternalData
{
	static ExternalProjectile s_externalProjectiles[TFE_DarkForces::PROJ_COUNT];
	static ExternalEffect s_externalEffects[HEFFECT_COUNT];

	static bool s_externalProjectilesFromMod = false;
	static bool s_externalEffectsFromMod = false;

	//////////////////////////////
	// Forward Declarations
	//////////////////////////////
	int getProjectileIndex(char* type);
	bool tryAssignProjectileProperty(cJSON* data, ExternalProjectile& projectile);
	int getEffectIndex(char* type);
	bool tryAssignEffectProperty(cJSON* data, ExternalEffect& effect);


	ExternalProjectile* getExternalProjectiles()
	{
		return s_externalProjectiles;
	}

	ExternalEffect* getExternalEffects()
	{
		return s_externalEffects;
	}

	void clearExternalProjectiles()
	{
		s_externalProjectilesFromMod = false;
		for (int i = 0; i < TFE_DarkForces::PROJ_COUNT; i++)
		{
			s_externalProjectiles[i].type = nullptr;
		}
	}

	void clearExternalEffects()
	{
		s_externalEffectsFromMod = false;
		for (int i = 0; i < HEFFECT_COUNT; i++)
		{
			s_externalEffects[i].type = nullptr;
		}
	}

	void loadExternalProjectiles()
	{
		const char* programDir = TFE_Paths::getPath(PATH_PROGRAM);
		char extDataFile[TFE_MAX_PATH];
		sprintf(extDataFile, "%sExternalData/DarkForces/projectiles.json", programDir);

		TFE_System::logWrite(LOG_MSG, "EXTERNAL_DATA", "Loading projectile data");
		FileStream file;
		if (!file.open(extDataFile, FileStream::MODE_READ)) { return; }

		const size_t size = file.getSize();
		char* data = (char*)malloc(size + 1);
		if (!data || size == 0)
		{
			TFE_System::logWrite(LOG_ERROR, "EXTERNAL_DATA", "Projectiles.json is %u bytes in size and cannot be read.", size);
			return;
		}
		file.readBuffer(data, (u32)size);
		data[size] = 0;
		file.close();

		parseExternalProjectiles(data, false);
		free(data);
	}

	void parseExternalProjectiles(char* data, bool fromMod)
	{
		// If projectiles have already been loaded from a mod, don't replace them
		if (s_externalProjectilesFromMod)
		{
			return;
		}
		
		cJSON* root = cJSON_Parse(data);
		if (root)
		{
			cJSON* section = root->child;
			if (section && cJSON_IsArray(section) && strcasecmp(section->string, "projectiles") == 0)
			{
				cJSON* projectile = section->child;
				while (projectile)
				{
					cJSON* projectileType = projectile->child;

					// get the projectile type
					if (projectileType && cJSON_IsString(projectileType) && strcasecmp(projectileType->string, "type") == 0)
					{
						// For now stick with the hardcoded DF list
						int index = getProjectileIndex(projectileType->valuestring);

						if (index >= 0)
						{
							ExternalProjectile extProjectile = {};
							extProjectile.type = projectileType->valuestring;

							cJSON* projectileData = projectileType->next;
							if (projectileData && cJSON_IsObject(projectileData))
							{
								cJSON* dataItem = projectileData->child;

								// iterate through the data and assign properties
								while (dataItem)
								{
									tryAssignProjectileProperty(dataItem, extProjectile);
									dataItem = dataItem->next;
								}
							}

							s_externalProjectiles[index] = extProjectile;
						}
					}

					projectile = projectile->next;
				}
			}

			s_externalProjectilesFromMod = fromMod && validateExternalProjectiles();
		}
		else
		{
			const char* error = cJSON_GetErrorPtr();
			if (error)
			{
				TFE_System::logWrite(LOG_ERROR, "EXTERNAL_DATA", "Failed to parse json before\n%s", error);
			}
			else
			{
				TFE_System::logWrite(LOG_ERROR, "EXTERNAL_DATA", "Failed to parse json");
			}
		}
	}

	void loadExternalEffects()
	{
		const char* programDir = TFE_Paths::getPath(PATH_PROGRAM);
		char extDataFile[TFE_MAX_PATH];
		sprintf(extDataFile, "%sExternalData/DarkForces/effects.json", programDir);

		TFE_System::logWrite(LOG_MSG, "EXTERNAL_DATA", "Loading effects data");
		FileStream file;
		if (!file.open(extDataFile, FileStream::MODE_READ)) { return; }

		const size_t size = file.getSize();
		char* data = (char*)malloc(size + 1);
		if (!data || size == 0)
		{
			TFE_System::logWrite(LOG_ERROR, "EXTERNAL_DATA", "Effects.json is %u bytes in size and cannot be read.", size);
			return;
		}
		file.readBuffer(data, (u32)size);
		data[size] = 0;
		file.close();

		parseExternalEffects(data, false);
		free(data);
	}

	void parseExternalEffects(char* data, bool fromMod)
	{
		// If effects have already been loaded from a mod, don't replace them
		if (s_externalEffectsFromMod)
		{
			return;
		}

		cJSON* root = cJSON_Parse(data);
		if (root)
		{
			cJSON* section = root->child;
			if (section && cJSON_IsArray(section) && strcasecmp(section->string, "effects") == 0)
			{
				cJSON* effect = section->child;
				while (effect)
				{
					cJSON* effectType = effect->child;

					// get the effect type
					if (effectType && cJSON_IsString(effectType) && strcasecmp(effectType->string, "type") == 0)
					{
						// For now stick with the hardcoded DF list
						int index = getEffectIndex(effectType->valuestring);

						if (index >= 0)
						{
							ExternalEffect extEffect = {};
							extEffect.type = effectType->valuestring;

							cJSON* effectData = effectType->next;
							if (effectData && cJSON_IsObject(effectData))
							{
								cJSON* dataItem = effectData->child;

								// iterate through the data and assign properties
								while (dataItem)
								{
									tryAssignEffectProperty(dataItem, extEffect);
									dataItem = dataItem->next;
								}
							}

							s_externalEffects[index] = extEffect;
						}
					}

					effect = effect->next;
				}
			}

			s_externalEffectsFromMod = fromMod && validateExternalEffects();
		}
		else
		{
			const char* error = cJSON_GetErrorPtr();
			if (error)
			{
				TFE_System::logWrite(LOG_ERROR, "EXTERNAL_DATA", "Failed to parse json before\n%s", error);
			}
			else
			{
				TFE_System::logWrite(LOG_ERROR, "EXTERNAL_DATA", "Failed to parse json");
			}
		}
	}

	bool validateExternalProjectiles()
	{
		// If the type field is null, we can assume the projectile's data hasn't loaded properly
		for (int i = 0; i < TFE_DarkForces::PROJ_COUNT; i++)
		{
			if (!s_externalProjectiles[i].type)
			{
				return false;
			}
		}

		return true;
	}

	bool validateExternalEffects()
	{
		// If the type field is null, we can assume the effect's data hasn't loaded properly
		for (int i = 0; i < HEFFECT_COUNT; i++)
		{
			if (!s_externalEffects[i].type)
			{
				return false;
			}
		}

		return true;
	}

	int getProjectileIndex(char* type)
	{
		for (int i = 0; i < TFE_DarkForces::PROJ_COUNT; i++)
		{
			if (strcasecmp(type, TFE_ExternalData::df_projectileTable[i]) == 0)
			{
				return i;
			}
		}

		return -1;
	}

	int getEffectIndex(char* type)
	{
		for (int i = 0; i < HEFFECT_COUNT; i++)
		{
			if (strcasecmp(type, TFE_ExternalData::df_effectTable[i]) == 0)
			{
				return i;
			}
		}

		return -1;
	}

	bool tryAssignProjectileProperty(cJSON* data, ExternalProjectile &projectile)
	{
		if (!data)
		{
			return false;
		}

		if (cJSON_IsString(data) && strcasecmp(data->string, "assetType") == 0)
		{
			projectile.assetType = data->valuestring;
			return true;
		}

		if (cJSON_IsString(data) && strcasecmp(data->string, "asset") == 0)
		{
			projectile.asset = data->valuestring;
			return true;
		}

		if (cJSON_IsBool(data) && strcasecmp(data->string, "fullBright") == 0)
		{
			projectile.fullBright = cJSON_IsTrue(data);
			return true;
		}

		if (cJSON_IsBool(data) && strcasecmp(data->string, "zeroWidth") == 0)
		{
			projectile.zeroWidth = cJSON_IsTrue(data);
			return true;
		}

		if (cJSON_IsBool(data) && strcasecmp(data->string, "autoAim") == 0)
		{
			projectile.autoAim = cJSON_IsTrue(data);
			return true;
		}

		if (cJSON_IsBool(data) && strcasecmp(data->string, "movable") == 0)
		{
			projectile.movable = cJSON_IsTrue(data);
			return true;
		}

		if (cJSON_IsString(data) && strcasecmp(data->string, "updateFunc") == 0)
		{
			projectile.updateFunc = data->valuestring;
			return true;
		}
		
		if (cJSON_IsNumber(data) && strcasecmp(data->string, "damage") == 0)
		{
			projectile.damage = data->valueint;
			return true;
		}

		if (cJSON_IsNumber(data) && strcasecmp(data->string, "falloffAmount") == 0)
		{
			projectile.falloffAmount = u32(data->valuedouble * ONE_16);
			return true;
		}

		if (cJSON_IsNumber(data) && strcasecmp(data->string, "nextFalloffTick") == 0)
		{
			projectile.nextFalloffTick = data->valueint;
			return true;
		}

		if (cJSON_IsNumber(data) && strcasecmp(data->string, "damageFalloffDelta") == 0)
		{
			projectile.damageFalloffDelta = data->valueint;
			return true;
		}

		if (cJSON_IsNumber(data) && strcasecmp(data->string, "minDamage") == 0)
		{
			projectile.minDamage = data->valueint;
			return true;
		}

		if (cJSON_IsNumber(data) && strcasecmp(data->string, "force") == 0)
		{
			projectile.force = u32(data->valuedouble * ONE_16);
			return true;
		}

		if (cJSON_IsNumber(data) && strcasecmp(data->string, "speed") == 0)
		{
			projectile.speed = data->valueint;
			return true;
		}

		if (cJSON_IsNumber(data) && strcasecmp(data->string, "horzBounciness") == 0)
		{
			projectile.horzBounciness = u32(data->valuedouble * ONE_16);
			return true;
		}

		if (cJSON_IsNumber(data) && strcasecmp(data->string, "vertBounciness") == 0)
		{
			projectile.vertBounciness = u32(data->valuedouble * ONE_16);
			return true;
		}

		if (cJSON_IsNumber(data) && strcasecmp(data->string, "bounceCount") == 0)
		{
			projectile.bounceCount = data->valueint;
			return true;
		}

		if (cJSON_IsNumber(data) && strcasecmp(data->string, "reflectVariation") == 0)
		{
			projectile.reflectVariation = data->valueint;
			return true;
		}

		if (cJSON_IsNumber(data) && strcasecmp(data->string, "duration") == 0)
		{
			projectile.duration = u32(data->valuedouble * 145.65);	// 145.65 ticks per second
			return true;
		}

		if (cJSON_IsNumber(data) && strcasecmp(data->string, "angularSpeed") == 0)
		{
			projectile.homingAngularSpeed = floatToAngle(data->valueint);
			return true;
		}

		if (cJSON_IsString(data) && strcasecmp(data->string, "flightSound") == 0)
		{
			projectile.flightSound = data->valuestring;
			return true;
		}

		if (cJSON_IsString(data) && strcasecmp(data->string, "reflectSound") == 0)
		{
			projectile.reflectSound = data->valuestring;
			return true;
		}

		if (cJSON_IsString(data) && strcasecmp(data->string, "cameraPassSound") == 0)
		{
			projectile.cameraPassSound = data->valuestring;
			return true;
		}

		if (cJSON_IsString(data) && strcasecmp(data->string, "reflectEffect") == 0)
		{
			for (int i = 0; i < HEFFECT_COUNT; i++)
			{
				if (strcasecmp(data->valuestring, TFE_ExternalData::df_effectTable[i]) == 0)
				{
					projectile.reflectEffectId = i;
					return true;
				}
			}

			return false;
		}

		if (cJSON_IsString(data) && strcasecmp(data->string, "hitEffect") == 0)
		{
			for (int i = 0; i < HEFFECT_COUNT; i++)
			{
				if (strcasecmp(data->valuestring, TFE_ExternalData::df_effectTable[i]) == 0)
				{
					projectile.hitEffectId = i;
					return true;
				}
			}

			return false;
		}

		if (cJSON_IsBool(data) && strcasecmp(data->string, "explodeOnTimeout") == 0)
		{
			projectile.explodeOnTimeout = cJSON_IsTrue(data);
			return true;
		}

		return false;
	}

	bool tryAssignEffectProperty(cJSON* data, ExternalEffect& effect)
	{
		if (!data)
		{
			return false;
		}

		if (cJSON_IsString(data) && strcasecmp(data->string, "wax") == 0)
		{
			effect.wax = data->valuestring;
			return true;
		}

		if (cJSON_IsNumber(data) && strcasecmp(data->string, "force") == 0)
		{
			effect.force = data->valueint;
			return true;
		}

		if (cJSON_IsNumber(data) && strcasecmp(data->string, "damage") == 0)
		{
			effect.damage = data->valueint;
			return true;
		}
		
		if (cJSON_IsNumber(data) && strcasecmp(data->string, "explosiveRange") == 0)
		{
			effect.explosiveRange = data->valueint;
			return true;
		}

		if (cJSON_IsNumber(data) && strcasecmp(data->string, "wakeupRange") == 0)
		{
			effect.wakeupRange = data->valueint;
			return true;
		}

		if (cJSON_IsString(data) && strcasecmp(data->string, "soundEffect") == 0)
		{
			effect.soundEffect = data->valuestring;
			return true;
		}

		if (cJSON_IsNumber(data) && strcasecmp(data->string, "soundPriority") == 0)
		{
			effect.soundPriority = data->valueint;
			return true;
		}

		return false;
	}
}