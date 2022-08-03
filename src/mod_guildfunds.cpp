#include "Chat.h"
#include "Config.h"
#include "Guild.h"
#include "Player.h"
#include "ScriptMgr.h"

bool gfShowInfo;
uint32 gfLootMultiplier;
uint32 gfQuestMultiplier;

class GuildFundsConfig : WorldScript
{
public:
    GuildFundsConfig() : WorldScript("GuildFundsConfig") {}

    void OnAfterConfigLoad(bool /*reload*/) override
    {
        gfShowInfo = sConfigMgr->GetOption<bool>("GuildFunds.ShowInfo", 1);
        gfLootMultiplier = sConfigMgr->GetOption<uint32>("GuildFunds.Looted", 10);
        gfQuestMultiplier = sConfigMgr->GetOption<uint32>("GuildFunds.Quests", 3);
    }
};

class GuildFundsLoot : LootScript
{
public:
    GuildFundsLoot() : LootScript("GuildFundsLoot") {}

    void OnLootMoney(Player* player, uint32 gold) override
    {
        if (Group* group = player->GetGroup())
        {
            uint32 membersInRange = 0;
            for (GroupReference* groupRef = group->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
            {
                if (Player* member = groupRef->GetSource())
                {
                    if (member->IsAtLootRewardDistance(player))
                        membersInRange++;
                }
            }

            uint32 money = (((gold / membersInRange) / 100) * gfLootMultiplier);

            if (money < 1 || gfLootMultiplier < 1)
                return;

            for (GroupReference* groupRef = group->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
            {
                if (Player* member = groupRef->GetSource())
                {
                    if (member->IsAtLootRewardDistance(player))
                    {
                        if (Guild* guild = member->GetGuild())
                        {
                            guild->HandleMemberDepositMoney(member->GetSession(), money);
                            member->ModifyMoney(money);

                            if (gfShowInfo)
                                PrintGuildFundsInformation(member, money);
                        }
                    }
                }
            }
        }
        else
        {
            if (Guild* guild = player->GetGuild())
            {
                uint32 money = ((gold / 100) * gfLootMultiplier);

                if (money < 1 || gfLootMultiplier < 1)
                    return;

                guild->HandleMemberDepositMoney(player->GetSession(), money);
                player->ModifyMoney(money);

                if (gfShowInfo)
                    PrintGuildFundsInformation(player, money);
            }
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
            uint32 money = ((quest->GetRewOrReqMoney(playerLevel) / 100) * gfQuestMultiplier);

            if (money < 1 || gfQuestMultiplier < 1)
                return;

            guild->HandleMemberDepositMoney(player->GetSession(), money);
            player->ModifyMoney(money);

            if (gfShowInfo)
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
