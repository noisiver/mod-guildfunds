#include "Config.h"

#include "mod_guildfunds.h"

void GuildFunds::OnAfterConfigLoad(bool /*reload*/)
{
    ShowInfo = sConfigMgr->GetOption<bool>("GuildFunds.ShowInfo", 1);
    LootMultiplier = sConfigMgr->GetOption<uint32>("GuildFunds.Looted", 10);
    QuestMultiplier = sConfigMgr->GetOption<uint32>("GuildFunds.Quests", 3);
}
