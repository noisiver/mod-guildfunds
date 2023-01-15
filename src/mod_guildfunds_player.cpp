#include "Chat.h"
#include "Guild.h"
#include "Player.h"

#include "mod_guildfunds.h"

void GuildFunds::OnPlayerCompleteQuest(Player* player, Quest const* quest)
{
    if (Guild* guild = player->GetGuild())
    {
        if (QuestMultiplier < 1)
            return;

        uint32 playerLevel = player->GetLevel();

        if (quest->GetRewOrReqMoney(playerLevel) < 1)
            return;

        uint32 money = quest->GetRewOrReqMoney(playerLevel) * QuestMultiplier / 100;

        if (money < 1)
            return;

        guild->HandleMemberDepositMoney(player->GetSession(), money);
        player->ModifyMoney(money);

        if (ShowInfo)
            SendFundsInformation(player, money);
    }
}

void GuildFunds::SendFundsInformation(Player* player, uint32 money)
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
