#pragma once

class Settings
{
public:
	static void ReadIniSettings();

	static void ReadBoolSetting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, bool& a_setting);

	// General
	static inline bool bDisableLevelUpHealth = false;
	static inline bool bDisableLevelUpAP = false;
	static inline bool bDisableSleepHealing = false;
};
