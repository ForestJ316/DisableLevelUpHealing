#pragma once
#include "Settings.h"

// Hooks
class Hooks
{
public:
	static void Hook()
	{
		_UpdateLevelFinish = REL::GetTrampoline().write_call<5>(REL::ID(2233291).address() + 0x25B, UpdateLevelFinish);
		_RestoreAV = REL::Relocation<std::uintptr_t>(RE::VTABLE::PlayerCharacter[7]).write_vfunc(0x08, RestoreAV);
	}
	
private:
	static inline bool bIsLevelUp = false;

	static void UpdateLevelFinish(RE::PlayerCharacter* a_player)
	{
		bIsLevelUp = true;
		_UpdateLevelFinish(a_player);
		bIsLevelUp = false;
		// Update max health on Pipboy HUD, beware soft-lock
		if (auto a_actorValue = RE::ActorValue::GetSingleton(); Settings::bDisableLevelUpHealth && a_player->GetActorValue(*a_actorValue->health) > 0.1f) {
			a_player->ModActorValue(RE::ACTOR_VALUE_MODIFIER::kDamage, *a_actorValue->health, -0.1f);
			a_player->ModActorValue(RE::ACTOR_VALUE_MODIFIER::kDamage, *a_actorValue->health, 0.1f);
		}
		
	}
	static inline REL::Relocation<decltype(UpdateLevelFinish)> _UpdateLevelFinish;

	static void RestoreAV(RE::PlayerCharacter* a_player, const RE::ActorValueInfo& a_info, float a_amount)
	{
		if (bIsLevelUp == true) {
			// If it's Health and healing on level-up is disabled
			if (a_info.formID == RE::ActorValue::GetSingleton()->health->formID && Settings::bDisableLevelUpHealth) {
				// Do nothing
			}
			// If it's AP and AP regen on level-up is disabled
			else if (a_info.formID == RE::ActorValue::GetSingleton()->actionPoints->formID && Settings::bDisableLevelUpAP) {
				// Do nothing
			}
			// Otherwise just restore as normal
			else {
				_RestoreAV(a_player, a_info, a_amount);
			}
		}
		else {
			_RestoreAV(a_player, a_info, a_amount);
		}
	}
	static inline REL::Relocation<decltype(RestoreAV)> _RestoreAV;
};
