#include "Settings.h"

void Settings::ReadIniSettings()
{
	constexpr auto ini_path = L"Data/F4SE/Plugins/DisableLevelUpHealing.ini";

	const auto ReadSettingsIni = [&](std::filesystem::path path) {
		CSimpleIniA ini;
		ini.SetUnicode();
		ini.LoadFile(path.string().c_str());
		
		ReadBoolSetting(ini, "General", "Disable Level-Up Health", bDisableLevelUpHealth);
		ReadBoolSetting(ini, "General", "Disable Level-Up AP", bDisableLevelUpAP);
	};

	ReadSettingsIni(ini_path);
}

void Settings::ReadBoolSetting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, bool& a_setting)
{
	const char* bFound = nullptr;
	bFound = a_ini.GetValue(a_sectionName, a_settingName);
	if (bFound) {
		a_setting = a_ini.GetBoolValue(a_sectionName, a_settingName);
	}
}
