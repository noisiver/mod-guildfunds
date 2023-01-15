#include "Guild.h"
#include "Player.h"

#include "mod_guildfunds.h"

void GuildFunds::OnLootMoney(Player* player, uint32 gold)
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

        if (LootMultiplier < 1)
            return;

        uint32 money = (gold / membersInRange) * LootMultiplier / 100;

        if (money < 1)
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

                        if (ShowInfo)
                            SendFundsInformation(member, money);
                    }
                }
            }
        }
    }
    else
    {
        if (Guild* guild = player->GetGuild())
        {
            uint32 money = gold * LootMultiplier / 100;

            if (money < 1 || LootMultiplier < 1)
                return;

            guild->HandleMemberDepositMoney(player->GetSession(), money);
            player->ModifyMoney(money);

            if (ShowInfo)
                SendFundsInformation(player, money);
        }
    }
}
