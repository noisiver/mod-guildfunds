#ifndef MOD_GUILDFUNDS
#define MOD_GUILDFUNDS

#include "ScriptMgr.h"

class GuildFunds : public LootScript, PlayerScript, WorldScript
{
public:
    GuildFunds();

    // LootScript
    void OnLootMoney(Player* /*player*/, uint32 /*gold*/) override;

    // PlayerScript
    void OnPlayerCompleteQuest(Player* /*player*/, Quest const* /*quest*/) override;

    // WorldScript
    void OnAfterConfigLoad(bool /*reload*/) override;

private:
    bool ShowInfo;
    uint32 LootMultiplier;
    uint32 QuestMultiplier;

    void SendFundsInformation(Player* /*player*/, uint32 /*money*/);
};

#endif
