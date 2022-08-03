#include "Chat.h"
#include "Config.h"
#include "Guild.h"
#include "Player.h"
#include "ScriptMgr.h"

uint32 lootFunds;
uint32 questFunds;

class GuildFundsConfig : WorldScript
{
public:
    GuildFundsConfig() : WorldScript("GuildFundsConfig") {}

    void OnAfterConfigLoad(bool /*reload*/) override
    {
        lootFunds = sConfigMgr->GetOption<uint32>("GuildFunds.Looted", 10);
        questFunds = sConfigMgr->GetOption<uint32>("GuildFunds.Quests", 3);
    }
};

class GuildFundsLoot : LootScript
{
public:
    GuildFundsLoot() : LootScript("GuildFundsLoot") {}

    void OnLootMoney(Player* player, uint32 gold) override
    {
        if (Guild* guild = player->GetGuild())
        {
            uint32 money = ((gold / 100) * questFunds);

            if (money < 0 || lootFunds < 1)
                return;

            guild->HandleMemberDepositMoney(player->GetSession(), money);
            PrintGuildFundsInformation(player, money);
        }
    }

private:
    void PrintGuildFundsInformation(Player* player, uint32 money)
    {
        uint32 gold = money / GOLD;
        uint32 silver = (money % GOLD) / SILVER;
        uint32 copper = (money % GOLD) % SILVER;
        std::string info;

        if (money < SILVER)
            info = Acore::StringFormat("%i copper was deposited to the guild bank.", copper);
        else if (money < GOLD)
            info = Acore::StringFormat("%i silver, %i copper was deposited to the guild bank.", silver, copper);
        else
            info = Acore::StringFormat("%i gold, %i silver, %i copper was deposited to the guild bank.", gold, silver, copper);

        ChatHandler(player->GetSession()).SendSysMessage(info);
    }
};

class GuildFundsQuest : public PlayerScript
{
public:
    GuildFundsQuest() : PlayerScript("GuildFundsQuest") {}

    void OnPlayerCompleteQuest(Player* player, Quest const* quest) override
    {
        if (Guild* guild = player->GetGuild())
        {
            uint32 playerLevel = player->getLevel();
            uint32 money = ((quest->GetRewOrReqMoney(playerLevel) / 100) * questFunds);

            if (money < 0 || questFunds < 1)
                return;

            guild->HandleMemberDepositMoney(player->GetSession(), money);
            PrintGuildFundsInformation(player, money);
        }
    }

private:
    void PrintGuildFundsInformation(Player* player, uint32 money)
    {
        uint32 gold = money / GOLD;
        uint32 silver = (money % GOLD) / SILVER;
        uint32 copper = (money % GOLD) % SILVER;
        std::string info;

        if (money < SILVER)
            info = Acore::StringFormat("%i copper was deposited to the guild bank.", copper);
        else if (money < GOLD)
            info = Acore::StringFormat("%i silver, %i copper was deposited to the guild bank.", silver, copper);
        else
            info = Acore::StringFormat("%i gold, %i silver, %i copper was deposited to the guild bank.", gold, silver, copper);

        ChatHandler(player->GetSession()).SendSysMessage(info);
    }
};

void AddGuildFundsScripts()
{
    new GuildFundsConfig();
    new GuildFundsLoot();
    new GuildFundsQuest();
}
