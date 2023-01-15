#include "mod_guildfunds.h"

GuildFunds::GuildFunds() : LootScript("GuildFundsLootScript"), PlayerScript("GuildFundsPlayerScript"), WorldScript("GuildFundsWorldScript") {}

void Addmod_guildfundsScripts()
{
    new GuildFunds();
}
