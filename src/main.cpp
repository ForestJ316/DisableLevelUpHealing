#include "Hooks.h"
#include "Settings.h"

namespace
{
	void MessageHandler(F4SE::MessagingInterface::Message* a_msg)
	{
		switch (a_msg->type)
		{
			case F4SE::MessagingInterface::kGameDataReady:
				Settings::ReadIniSettings();
				break;
			default:
				break;
		}
	}

	F4SE_PLUGIN_LOAD(const F4SE::LoadInterface* a_f4se)
	{
		F4SE::Init(a_f4se);
		F4SE::AllocTrampoline(96);
		
		if (!F4SE::GetMessagingInterface()->RegisterListener(MessageHandler)) {
			return false;
		}

		Hooks::Hook();

		return true;
	}
}
