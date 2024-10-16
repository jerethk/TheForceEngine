#include "weaponExternal.h"
#include <vector>
#include <TFE_DarkForces/projectile.h>
#include <TFE_System/system.h>
#include <TFE_System/cJSON.h>
#include <TFE_Settings/logicTables.h>

namespace TFE_ExternalData
{
	ExternalProjectile s_externalProjectiles[TFE_DarkForces::PROJ_COUNT];

	ExternalEffect s_externalEffects[HEFFECT_COUNT];

	//////////////////////////////
	// Forward Declarations
	//////////////////////////////

	void parseExternalProjectiles(char* data);
	int getProjectileIndex(char* type);
	bool tryAssignProjectileProperty(cJSON* data, ExternalProjectile& projectile);
	void parseExternalEffects(char* data);
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

	void loadExternalProjectiles()
	{
		const char* programDir = TFE_Paths::getPath(PATH_PROGRAM);
		char extDataFile[TFE_MAX_PATH];
		sprintf(extDataFile, "%sExternalData\\projectiles.json", programDir);

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

		parseExternalProjectiles(data);
		free(data);
	}

	void parseExternalProjectiles(char* data)
	{
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

						if (index >= 0 && !s_externalProjectiles[index].type)
						{
							ExternalProjectile projectile = {};
							projectile.type = projectileType->valuestring;

							cJSON* projectileData = projectileType->next;
							if (projectileData && cJSON_IsObject(projectileData))
							{
								cJSON* dataItem = projectileData->child;

								// iterate through the data and assign properties
								while (dataItem)
								{
									tryAssignProjectileProperty(dataItem, projectile);
									dataItem = dataItem->next;
								}
							}

							s_externalProjectiles[index] = projectile;
						}
					}

					projectile = projectile->next;
				}
			}
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
		sprintf(extDataFile, "%sExternalData\\effects.json", programDir);

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

		parseExternalEffects(data);
		free(data);
	}

	void parseExternalEffects(char* data)
	{
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

						if (index >= 0 && !s_externalEffects[index].type)
						{
							ExternalEffect effect = {};
							effect.type = effectType->valuestring;

							cJSON* effectData = effectType->next;
							if (effectData && cJSON_IsObject(effectData))
							{
								cJSON* dataItem = effectData->child;

								// iterate through the data and assign properties
								while (dataItem)
								{
									tryAssignEffectProperty(dataItem, effect);
									dataItem = dataItem->next;
								}
							}

							s_externalEffects[index] = effect;
						}
					}

					effect = effect->next;
				}
			}
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
			if (strcasecmp(type, TFE_Settings::df_projectileTable[i]) == 0)
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
			if (strcasecmp(type, TFE_Settings::df_effectTable[i]) == 0)
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
			projectile.falloffAmount = data->valueint;
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
			projectile.force = data->valueint;
			return true;
		}

		if (cJSON_IsNumber(data) && strcasecmp(data->string, "speed") == 0)
		{
			projectile.speed = data->valueint;
			return true;
		}

		if (cJSON_IsNumber(data) && strcasecmp(data->string, "horzBounciness") == 0)
		{
			projectile.horzBounciness = data->valueint;
			return true;
		}

		if (cJSON_IsNumber(data) && strcasecmp(data->string, "vertBounciness") == 0)
		{
			projectile.vertBounciness = data->valueint;
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
			projectile.duration = data->valueint;
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
			for (int i = 0; i <= 17; i++)
			{
				if (strcasecmp(data->valuestring, TFE_Settings::df_effectTable[i]) == 0)
				{
					projectile.reflectEffectId = i;
					return true;
				}
			}

			return false;
		}

		if (cJSON_IsString(data) && strcasecmp(data->string, "hitEffect") == 0)
		{
			for (int i = 0; i <= 17; i++)
			{
				if (strcasecmp(data->valuestring, TFE_Settings::df_effectTable[i]) == 0)
				{
					projectile.hitEffectId = i;
					return true;
				}
			}

			return false;
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
	}
}