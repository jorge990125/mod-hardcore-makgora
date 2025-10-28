#include "ScriptMgr.h"
#include "Player.h"
#include "Config.h"
#include "Chat.h"
#include "GossipDef.h"
#include "ScriptedGossip.h"
#include "ScriptedCreature.h"
#include "WorldSessionMgr.h"

class HardcoreMode_Announcer : public PlayerScript
{
public:
    HardcoreMode_Announcer() : PlayerScript("HardcoreMode_Announcer") {}

    void OnPlayerLogin(Player* player)
    {
        if (sConfigMgr->GetOption<bool>("HardcoreMode.AnnounceEnable", true))
        {
            ChatHandler(player->GetSession()).SendSysMessage("Modulo de Servidor |cff4CFF00HardCore-Mak'Gora.");
        }
    }
};

enum ardcoreModeAcoreString
{
    ACORE_STRING_DEATH = 36000,
    ACORE_STRING_CLASSIC,
    ACORE_STRING_TBC,
    ACORE_STRING_WOTLK,
    ACORE_STRING_TITLE,
    ACORE_STRING_ERROR_TITLE,
    ACORE_STRING_LEAVE_GROUP,
    ACORE_STRING_GHOST,
    ACORE_STRING_PLAYER_KILLED_HARDCORE,
    ACORE_STRING_KILLED_HARDCORE,
    ACORE_STRING_NOT_REVIVE,
    ACORE_STRING_PLAYER_DEATH_HARDCORE,
    ACORE_STRING_NOT_INVITED_GROUP,
    ACORE_STRING_NOT_GROUP,
    ACORE_STRING_MAIL,
    ACORE_STRING_AUCTIONHOUSE,
    ACORE_STRING_GUILD_BANK,
    ACORE_STRING_GUILD_NOT_HERO
};

class HardcoreMode : public PlayerScript
{
public:
    explicit HardcoreMode() : PlayerScript("mod-hardcore") {}

    void OnPlayerLogin(Player* player) override
    {
        if (getHardcoreEnabled(player))
        {
            if (player->isDead())
            {
                ChatHandler(player->GetSession()).PSendSysMessage(ACORE_STRING_DEATH);
            }
        }
    }

    void OnPlayerLevelChanged(Player* player, uint8 /*oldlevel*/) override
    {
        if (getRewardEnabled())
        {
            if (player->GetLevel() == 60)
            {
                uint32 RewardTitleLevel60 = sConfigMgr->GetOption<uint32>("Reward.TitleLevel60", 112); //ID titulo				
                uint32 item1level60 = sConfigMgr->GetOption<uint32>("Reward.Item1.Level60", 57578); //Itiem 1
                uint32 item2level60 = sConfigMgr->GetOption<uint32>("Reward.Item2.Level60", 39656); //Itiem 2

                constexpr uint32 SPELL_ID = 44940; // ID del hechizo a lanzar
				
                for (uint32 itemId : { item1level60, item2level60 })
                {
                    player->AddItem(itemId, 1);
                }
                // Lanzar el hechizo sin activar el GCD
                player->CastSpell(player, SPELL_ID, true);

                if (CharTitlesEntry const* titleEntry = sCharTitlesStore.LookupEntry(RewardTitleLevel60))
                {
                    // Anuncio de agradecimiento
                    if (!player->HasTitle(titleEntry))
                    {
                        player->SetTitle(titleEntry);
                        ChatHandler(player->GetSession()).PSendSysMessage(ACORE_STRING_CLASSIC, player->GetName().c_str());
                    }
                    else
                    {
                        ChatHandler(player->GetSession()).PSendSysMessage(ACORE_STRING_TITLE);
                    }
                }
                else
                {
                    ChatHandler(player->GetSession()).PSendSysMessage(ACORE_STRING_ERROR_TITLE, RewardTitleLevel60);
                }
            }

            if (player->GetLevel() == 70)
            {
                uint32 RewardTitleLevel70 = sConfigMgr->GetOption<uint32>("Reward.TitleLevel70", 86); //ID titulo
                uint32 item1level70 = sConfigMgr->GetOption<uint32>("Reward.Item1.Level70", 57578); //Itiem 1
                uint32 item2level70 = sConfigMgr->GetOption<uint32>("Reward.Item2.Level70", 39656); //Itiem 2

                constexpr uint32 SPELL_ID = 44940; // ID del hechizo a lanzar
				
                for (uint32 itemId : { item1level70, item2level70 })
                {
                    player->AddItem(itemId, 1);
                }
                // Lanzar el hechizo sin activar el GCD
                player->CastSpell(player, SPELL_ID, true);

                if (CharTitlesEntry const* titleEntry = sCharTitlesStore.LookupEntry(RewardTitleLevel70))
                {
                    // Anuncio de agradecimiento
                    if (!player->HasTitle(titleEntry))
                    {
                        player->SetTitle(titleEntry);
                        ChatHandler(player->GetSession()).PSendSysMessage(ACORE_STRING_TBC, player->GetName().c_str());
                    }
                    else
                    {
                        ChatHandler(player->GetSession()).PSendSysMessage(ACORE_STRING_TITLE);
                    }
                }
                else
                {
                    ChatHandler(player->GetSession()).PSendSysMessage(ACORE_STRING_ERROR_TITLE, RewardTitleLevel70);
                }
            }

            // Verificar si el jugador alcanzó el nivel 80
            if (player->GetLevel() == 80)
            {
                uint32 RewardTitleLevel80 = sConfigMgr->GetOption<uint32>("Reward.TitleLevel80", 114);  //ID titulo
                uint32 item1level80 = sConfigMgr->GetOption<uint32>("Reward.Item1.Level80", 57578); //Itiem 1
                uint32 item2level80 = sConfigMgr->GetOption<uint32>("Reward.Item2.Level80", 39656); //Itiem 2

                constexpr uint32 SPELL_ID = 44940; // ID del hechizo a lanzar
				
                for (uint32 itemId : { item1level80, item2level80 })
                {
                    player->AddItem(itemId, 1);
                }
                // Lanzar el hechizo sin activar el GCD
                player->CastSpell(player, SPELL_ID, true);

                if (CharTitlesEntry const* titleEntry = sCharTitlesStore.LookupEntry(RewardTitleLevel80))
                {
                    // Anuncio de agradecimiento
                    if (!player->HasTitle(titleEntry))
                    {
                        player->SetTitle(titleEntry);
                        ChatHandler(player->GetSession()).PSendSysMessage(ACORE_STRING_WOTLK, player->GetName().c_str());
                    }
                    else
                    {
                        ChatHandler(player->GetSession()).PSendSysMessage(ACORE_STRING_TITLE);
                    }
                }
                else
                {
                    ChatHandler(player->GetSession()).PSendSysMessage(ACORE_STRING_ERROR_TITLE, RewardTitleLevel80);
                }
            }
        }
    }

    void OnPlayerJustDied(Player* player) override
    {
        if (getHardcoreEnabled(player))
        {
            Group* group = player->GetGroup();
            if (group)
            {
                ChatHandler(player->GetSession()).PSendSysMessage(ACORE_STRING_LEAVE_GROUP);
                group->RemoveMember(player->GetGUID());
            }
        }
    }

    void OnPlayerReleasedGhost(Player* player) override
    {
        if (getHardcoreEnabled(player))
        {
            ChatHandler(player->GetSession()).PSendSysMessage(ACORE_STRING_GHOST);
            return;
        }
    }

    void OnPlayerPVPKill(Player* killer, Player* killed) override
    {
        if (getHardcoreEnabled(killed))
        {
            std::ostringstream deathMessage;
            deathMessage << "|cffff0000[Hardcore] ¡El jugador |cffffd700" << killed->GetName()
                << "|cffff0000 ha sido asesinado en nivel " << static_cast<int>(killed->GetLevel())
                << "|cffff0000 por el jugador |cffffd700" << killer->GetName()
                << "|cffff0000 de nivel " << static_cast<int>(killer->GetLevel()) << "|cffff0000!Que descanse en paz...|r";

            std::ostringstream deathplayer;
            deathplayer << "|TInterface\\Icons\\Ability_DualWield:60|t\n"  // Icono arriba con salto de línea
                << "|cffff0000¡El jugador |cffffd700"
                << killed->GetName() << "|cffff0000 ha sido asesinado"
                << "|cffff0000 por el jugador |cffffd700" << killer->GetName()
                << "|cffff0000, en nivel " << static_cast<int>(killed->GetLevel())
                << "|cffff0000!";

            // Mensaje para el jugador muerto
            ChatHandler(killed->GetSession()).PSendSysMessage(ACORE_STRING_PLAYER_KILLED_HARDCORE);

            WorldPacket data(SMSG_NOTIFICATION, (deathplayer.str().size() + 1));
            data << deathplayer.str();
            sWorldSessionMgr->SendGlobalMessage(&data);

            // Mensaje global en el servidor
            sWorldSessionMgr->SendServerMessage(SERVER_MSG_STRING, deathMessage.str().c_str());
        }
    }

    void OnPlayerKilledByCreature(Creature* killer, Player* killed) override
    {
        if (getHardcoreEnabled(killed))
        {
            std::ostringstream deathMessage;
            deathMessage << "|cffff0000[Hardcore] ¡El jugador |cffffd700" << killed->GetName()
                << "|cffff0000 ha sido asesinado en nivel " << static_cast<int>(killed->GetLevel())
                << "|cffff0000 por la criatura |cffffd700"
                << killer->GetName() << "|cffff0000 de nivel " << static_cast<int>(killer->GetLevel()) << "|cffff0000!Que descanse en paz...|r";

            std::ostringstream deathplayer;
            deathplayer << "|TInterface\\Icons\\Ability_DualWield:60|t\n"  // Icono arriba con salto de línea
                << "|cffff0000¡El jugador |cffffd700"
                << killed->GetName() << "|cffff0000 ha sido asesinado"
                << "|cffff0000 por la criatura |cffffd700" << killer->GetName()
                << "|cffff0000, en nivel " << static_cast<int>(killed->GetLevel())
                << "|cffff0000!";

            // Mensaje para el jugador muerto
            ChatHandler(killed->GetSession()).PSendSysMessage(ACORE_STRING_KILLED_HARDCORE);

            WorldPacket data(SMSG_NOTIFICATION, (deathplayer.str().size() + 1));
            data << deathplayer.str();
            sWorldSessionMgr->SendGlobalMessage(&data);

            sWorldSessionMgr->SendServerMessage(SERVER_MSG_STRING, deathMessage.str().c_str());

            if (!killer || !killed)
                return;

            uint32 npcId = killer->GetEntry();

            // Guardar el registro de la muerte en la base de datos
            CharacterDatabase.Execute(
                "INSERT INTO npc_hardcore_ranking (npc_id, kill_count) VALUES ({}, 1) "
                "ON DUPLICATE KEY UPDATE kill_count = kill_count + 1", npcId);
        }
    }

    void OnPlayerResurrect(Player* player, float /*restore_percent*/, bool /*applySickness*/) override
    { // We keep this function just to prevent some exploits for reviving

        // En la arena o en el campo de batalla, la exepcion debe concederse antes de la condicion getHardcoreEnabled.
        if (player->IsGameMaster() || player->GetMap()->IsBattlegroundOrArena())
            return;

        if (getHardcoreEnabled(player))
        {
            ChatHandler(player->GetSession()).PSendSysMessage(ACORE_STRING_NOT_REVIVE);
            player->KillPlayer();
            player->GetSession()->KickPlayer(ACORE_STRING_PLAYER_DEATH_HARDCORE);
            return;
        }
    }

    bool OnPlayerCanUseChat(Player* player, uint32 /*type*/, uint32 /*language*/, std::string& /*msg*/) override
    {
        if (getHardcoreEnabled(player) && player->isDead())
        {
            return false;
        }
        return true;
    }

    bool OnPlayerCanUseChat(Player* player, uint32 /*type*/, uint32 /*language*/, std::string& /*msg*/, Player* /*receiver*/) override
    {
        if (getHardcoreEnabled(player) && player->isDead())
        {
            return false;
        }
        return true;
    }

    bool OnPlayerCanUseChat(Player* player, uint32 /*type*/, uint32 /*language*/, std::string& /*msg*/, Group* /*group*/) override
    {
        if (getHardcoreEnabled(player) && player->isDead())
        {
            return false;
        }
        return true;
    }

    bool OnPlayerCanUseChat(Player* player, uint32 /*type*/, uint32 /*language*/, std::string& /*msg*/, Guild* /*guild*/) override
    {
        if (getHardcoreEnabled(player) && player->isDead())
        {
            return false;
        }
        return true;
    }

    bool OnPlayerCanUseChat(Player* player, uint32 /*type*/, uint32 /*language*/, std::string& /*msg*/, Channel* /*channel*/) override
    {
        if (getHardcoreEnabled(player) && player->isDead())
        {
            return false;
        }
        return true;
    }

    bool OnPlayerCanGroupInvite(Player* player, std::string& /*membername*/) override
    {
        if (getHardcoreEnabled(player) && player->isDead())
        {
            ChatHandler(player->GetSession()).PSendSysMessage(ACORE_STRING_NOT_INVITED_GROUP);
            return false;
        }
        return true;
    }

    bool OnPlayerCanGroupAccept(Player* player, Group* /*group*/) override
    {
        if (getHardcoreEnabled(player) && player->isDead())
        {
            ChatHandler(player->GetSession()).PSendSysMessage(ACORE_STRING_NOT_GROUP);
            return false;
        }
        return true;
    }

private:
    bool getHardcoreEnabled(Player* player)
    {
        if (!player->IsGameMaster() && sConfigMgr->GetOption<bool>("ModHardcore.Enable", false))
        {
            if (player->GetLevel() >= sConfigMgr->GetOption<int>("ModHardcoreMinLevel.Enable", 1) && player->GetLevel() <= sConfigMgr->GetOption<int>("ModHardcoreMaxLevel.Enable", 79))
            {
                return true;
            }
        }
        return false;
    }

    bool getRewardEnabled()
    {
        if (!sConfigMgr->GetOption<bool>("Reward.Enable", false))
        {
            return false;
        }
        return true;
    }
};

class HardModeServerScript : ServerScript
{
public:
    HardModeServerScript() : ServerScript("mod-hardcore") {}

    bool CanPacketReceive(WorldSession* session, WorldPacket& packet) override
    {
        if (!sConfigMgr->GetOption<bool>("ModHardcore.Enable", false))
        {
            return true;
        }

        if (!session)
        {
            return true;
        }

        auto player = session->GetPlayer();
        if (!player || !player->isDead() || player->IsGameMaster())
        {
            return true;
        }

        if (player->GetLevel() >= sConfigMgr->GetOption<int>("ModHardcoreMinLevel.Enable", 1) && player->GetLevel() <= sConfigMgr->GetOption<int>("ModHardcoreMaxLevel.Enable", 79))
        {
            auto opCode = packet.GetOpcode();
            switch (opCode)
            {
                // mail
                case CMSG_GET_MAIL_LIST:
                case CMSG_SEND_MAIL:
                case CMSG_MAIL_CREATE_TEXT_ITEM:
                case CMSG_MAIL_DELETE:
                case CMSG_MAIL_TAKE_ITEM:
                case CMSG_MAIL_TAKE_MONEY:
                case CMSG_MAIL_RETURN_TO_SENDER:
                    ChatHandler(player->GetSession()).PSendSysMessage(ACORE_STRING_MAIL);
                    return false;

                // ah
                case CMSG_AUCTION_LIST_ITEMS:
                case CMSG_AUCTION_LIST_BIDDER_ITEMS:
                case CMSG_AUCTION_LIST_OWNER_ITEMS:
                case CMSG_AUCTION_PLACE_BID:
                case CMSG_AUCTION_REMOVE_ITEM:
                case CMSG_AUCTION_SELL_ITEM:
                    ChatHandler(player->GetSession()).PSendSysMessage(ACORE_STRING_AUCTIONHOUSE);
                    return false;

                // guild
                case CMSG_GUILD_BANKER_ACTIVATE:
                case CMSG_GUILD_BANK_QUERY_TAB:
                case CMSG_GUILD_BANK_DEPOSIT_MONEY:
                case CMSG_GUILD_BANK_WITHDRAW_MONEY:
                case CMSG_GUILD_BANK_BUY_TAB:
                case CMSG_GUILD_BANK_UPDATE_TAB:
                    ChatHandler(player->GetSession()).PSendSysMessage(ACORE_STRING_GUILD_BANK);
                    return false;
                case SMSG_PRE_RESURRECT: // No idea
                case CMSG_HEARTH_AND_RESURRECT: // No idea
                case SMSG_RESURRECT_REQUEST: // No idea, maybe never executed here because you can't send resurrect to anybody while dead
                case CMSG_RECLAIM_CORPSE: // Reviving going to corpse
                case CMSG_RESURRECT_RESPONSE: // Someone reviving your corpse
                case CMSG_SPIRIT_HEALER_ACTIVATE: // Spirit talking
                case SMSG_SPIRIT_HEALER_CONFIRM: // Spirit reviving, keep both spirit packets
                    ChatHandler(player->GetSession()).PSendSysMessage(ACORE_STRING_GUILD_NOT_HERO);
                    return false;
                case CMSG_GM_RESURRECT:
                    if (!sConfigMgr->GetOption<bool>("ModHardcoreGMCanResurrect.Enable", false))
                    {
                        return false;
                    }
                    break;
            }
        }
        return true;
    }
};


void AddSC_mod_harcore()
{
    new HardcoreMode_Announcer();
    new HardcoreMode();
    new HardModeServerScript();
}
