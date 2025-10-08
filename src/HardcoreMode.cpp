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
                ChatHandler(player->GetSession()).PSendSysMessage("Moriste durante una sesión en modo hardcore, así que ahora eres un fantasma permanente.Intenta asustar a los vivos.");
            }
        }
    }

    void OnPlayerLevelChanged(Player* player, uint8 /*oldlevel*/) override
    {
        if (getRewardEnabled())
        {
            if (player->GetLevel() == 60)
            {
                constexpr uint32 TITLE_ID = 112; // ID real del título
                constexpr uint32 SPELL_ID = 44940; // ID del hechizo a lanzar

                for (uint32 itemId : {57578, 39656 })
                {
                    player->AddItem(itemId, 1);
                }

                // Lanzar el hechizo sin activar el GCD
                player->CastSpell(player, SPELL_ID, true);

                if (CharTitlesEntry const* titleEntry = sCharTitlesStore.LookupEntry(TITLE_ID))
                {
                    // Anuncio de agradecimiento
                    if (!player->HasTitle(titleEntry))
                    {
                        player->SetTitle(titleEntry);
                        ChatHandler(player->GetSession()).PSendSysMessage("|cff00ff00¡Felicidades, |cffffd700{} |cff00ff00!Has completado la segunda fase del desafio Hardcore y has recibido un título especial. Relajaos, tomad un cerveza y disfrutad del contenido de TBC.|r", player->GetName().c_str());
                    }
                    else
                    {
                        ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000Ya tienes este título.|r");
                    }
                }
                else
                {
                    ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000Error: No se encontró el título con ID {}.|r", TITLE_ID);
                }
            }

            if (player->GetLevel() == 70)
            {
                constexpr uint32 TITLE_ID = 86; // ID real del título
                constexpr uint32 SPELL_ID = 44940; // ID del hechizo a lanzar

                for (uint32 itemId : {57578, 43516 })
                {
                    player->AddItem(itemId, 1);
                }

                // Lanzar el hechizo sin activar el GCD
                player->CastSpell(player, SPELL_ID, true);

                if (CharTitlesEntry const* titleEntry = sCharTitlesStore.LookupEntry(TITLE_ID))
                {
                    // Anuncio de agradecimiento
                    if (!player->HasTitle(titleEntry))
                    {
                        player->SetTitle(titleEntry);

                        ChatHandler(player->GetSession()).PSendSysMessage("|cff00ff00¡Felicidades, |cffffd700{} |cff00ff00!Has completado el desafio Hardcore y has recibido un título especial. Relajaos, tomad un cerveza y disfrutad del contenido de Wotlk.|r", player->GetName().c_str());
                    }
                    else
                    {
                        ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000Ya tienes este título.|r");
                    }
                }
                else
                {
                    ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000Error: No se encontró el título con ID {}.|r", TITLE_ID);
                }
            }

            // Verificar si el jugador alcanzó el nivel 80
            if (player->GetLevel() == 80)
            {
                constexpr uint32 TITLE_ID = 114; // ID real del título
                constexpr uint32 SPELL_ID = 44940; // ID del hechizo a lanzar

                for (uint32 itemId : {57578, 19160 })
                {
                    player->AddItem(itemId, 1);
                }

                // Lanzar el hechizo sin activar el GCD
                player->CastSpell(player, SPELL_ID, true);

                if (CharTitlesEntry const* titleEntry = sCharTitlesStore.LookupEntry(TITLE_ID))
                {
                    // Anuncio de agradecimiento
                    if (!player->HasTitle(titleEntry))
                    {
                        player->SetTitle(titleEntry);
                        ChatHandler(player->GetSession()).PSendSysMessage("|cff00ff00¡Felicidades, |cffffd700{} |cff00ff00!Has completado el desafio Hardcore y has recibido un título especial. Relajaos, tomad un cerveza y disfrutad del contenido de Wotlk.|r", player->GetName().c_str());
                    }
                    else
                    {
                        ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000Ya tienes este título.|r");
                    }
                }
                else
                {
                    ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000Error: No se encontró el título con ID {}.|r", TITLE_ID);
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
                ChatHandler(player->GetSession()).PSendSysMessage("Saliendo del grupo... Ya no puedes formar parte de los vivos.");
                group->RemoveMember(player->GetGUID());
            }
        }
    }

    void OnPlayerReleasedGhost(Player* player) override
    {
        if (getHardcoreEnabled(player))
        {
            ChatHandler(player->GetSession()).PSendSysMessage("Serás un fantasma para siempre...");
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
            ChatHandler(killed->GetSession()).PSendSysMessage("Un jugador te mató... en modo hardcore.");

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
            ChatHandler(killed->GetSession()).PSendSysMessage("Moriste durante una sesión en modo hardcore... Problemas de habilidad.");

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
        if (getHardcoreEnabled(player))
        {
            ChatHandler(player->GetSession()).PSendSysMessage("No puedes ser revivido. Mejora tus habilidades.");
            player->KillPlayer();
            player->GetSession()->KickPlayer("El jugador murió durante una sesión en modo hardcore.");
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
            ChatHandler(player->GetSession()).PSendSysMessage("No puedes invitar jugadores a un grupo mientras estés muerto.");
            return false;
        }
        return true;
    }

    bool OnPlayerCanGroupAccept(Player* player, Group* /*group*/) override
    {
        if (getHardcoreEnabled(player) && player->isDead())
        {
            ChatHandler(player->GetSession()).PSendSysMessage("No puedes ser parte de un grupo.");
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
                case SMSG_PRE_RESURRECT: // No idea
                case CMSG_HEARTH_AND_RESURRECT: // No idea
                case SMSG_RESURRECT_REQUEST: // No idea, maybe never executed here because you can't send resurrect to anybody while dead
                case CMSG_RECLAIM_CORPSE: // Reviving going to corpse
                case CMSG_RESURRECT_RESPONSE: // Someone reviving your corpse
                case CMSG_SPIRIT_HEALER_ACTIVATE: // Spirit talking
                case SMSG_SPIRIT_HEALER_CONFIRM: // Spirit reviving, keep both spirit packets
                    ChatHandler(player->GetSession()).PSendSysMessage("No puedes ser revivido. Viaja como un fantasma o crea un nuevo héroe.");
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
