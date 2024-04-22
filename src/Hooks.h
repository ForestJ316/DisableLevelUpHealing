#pragma once

// Offsets
typedef void(_fastcall* tBeginLevelUp)(RE::PlayerCharacter* a_player);
static REL::Relocation<tBeginLevelUp> BeginLevelUp{ REL::ID(647274) };

typedef void(_fastcall* tFinishLevelUp)(RE::PlayerCharacter* a_player);
static REL::Relocation<tFinishLevelUp> FinishLevelUp{ REL::ID(194884) };

// Hooks
class Hooks
{
public:
	static void Hook()
	{
		_UpdateLevelBegin = F4SE::GetTrampoline().write_call<5>(REL::ID(181041).address() + 0x53, UpdateLevelBegin);
		_UpdateLevelFinish = F4SE::GetTrampoline().write_call<5>(REL::ID(181041).address() + 0x8D, UpdateLevelFinish);
	}

	static void UpdatePlayerLevel()
	{
		oldPlayerLevel = RE::PlayerCharacter::GetSingleton()->GetLevel();
	}
	
private:
	static inline float oldMaxHealth = 0.f;
	static inline float oldMaxAP = 0.f;
	static inline int oldPlayerLevel = 0;

	static void UpdateLevelBegin(RE::PlayerCharacter* a_player)
	{
		auto avSingleton = RE::ActorValue::GetSingleton();
		oldMaxHealth = a_player->GetBaseActorValue(*avSingleton->health);
		oldMaxAP = a_player->GetBaseActorValue(*avSingleton->actionPoints);

		BeginLevelUp(a_player);
	}
	static inline REL::Relocation<decltype(UpdateLevelBegin)> _UpdateLevelBegin;

	static void UpdateLevelFinish(RE::PlayerCharacter* a_player)
	{
		auto avSingleton = RE::ActorValue::GetSingleton();

		auto currentHealth = a_player->GetActorValue(*avSingleton->health);
		auto currentAP = a_player->GetActorValue(*avSingleton->actionPoints);
		auto newMaxHealth = a_player->GetBaseActorValue(*avSingleton->health);
		auto newMaxAP = a_player->GetBaseActorValue(*avSingleton->actionPoints);

		FinishLevelUp(a_player);
		
		// We want to revert this back to old values
		auto updatedCurrentHealth = a_player->GetActorValue(*avSingleton->health);
		auto updatedCurrentAP = a_player->GetActorValue(*avSingleton->actionPoints);
		
		auto HealthDiff = updatedCurrentHealth - currentHealth;
		auto APDiff = updatedCurrentAP - currentAP;

		auto currentPlayerLevel = a_player->GetLevel();
		// Player leveled multiple levels instantly
		if (currentPlayerLevel - oldPlayerLevel > 1 && newMaxHealth - oldMaxHealth > 0)
		{
			oldMaxHealth = newMaxHealth - ((newMaxHealth - oldMaxHealth) * (currentPlayerLevel - oldPlayerLevel));
		}
		oldPlayerLevel = currentPlayerLevel;

		// Check for level-up on max health
		if (HealthDiff == 0 && oldMaxHealth < newMaxHealth)
		{
			HealthDiff += newMaxHealth - oldMaxHealth;
		}
		// Reduce values back to previous ones if they increased (account for base hp increase)
		if (HealthDiff > 0)
		{
			a_player->ModActorValue(RE::ACTOR_VALUE_MODIFIER::Damage, *avSingleton->health, -HealthDiff + (newMaxHealth - oldMaxHealth));
			// Update max health on HUD, beware soft-lock
			if (currentHealth + (newMaxHealth - oldMaxHealth) > 1)
			{
				a_player->ModActorValue(RE::ACTOR_VALUE_MODIFIER::Damage, *avSingleton->health, -1);
				a_player->ModActorValue(RE::ACTOR_VALUE_MODIFIER::Damage, *avSingleton->health, 1);
			}
		}
		if (APDiff > 0)
		{
			a_player->ModActorValue(RE::ACTOR_VALUE_MODIFIER::Damage, *avSingleton->actionPoints, -APDiff + (newMaxAP - oldMaxAP));
		}
	}
	static inline REL::Relocation<decltype(UpdateLevelFinish)> _UpdateLevelFinish;
};
