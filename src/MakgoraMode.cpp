#include "ScriptMgr.h"
#include "Player.h"
#include "Config.h"
#include "Chat.h"
#include "WorldSessionMgr.h"

using namespace Acore::ChatCommands;
static std::unordered_map<Player*, bool> makgoraPlayers; 

class MakgoraMode : public PlayerScript
{
public:
    explicit MakgoraMode() : PlayerScript("mod-hardcore") {}

    void OnPlayerDuelStart(Player* challenger, Player* opponent) override
    {
        if (!challenger || !opponent)
            return;

        // Verificar si el sistema Mak'Gora está habilitado
        if (!sConfigMgr->GetOption<bool>("Makgora.Enable", false))
            return;  // Si no está habilitado, no se permite el duelo Mak'Gora

        // Verificar si ambos están en un duelo Mak'Gora
        if (makgoraPlayers[challenger] && makgoraPlayers[opponent])
        {
            std::ostringstream startMessage;
            startMessage << "|cffff0000[Mak'Gora] |cffffd700" << challenger->GetName()
                << "|r desafió a |cffffd700" << opponent->GetName()
                << "|r a un duelo a muerte. ¡Que la fuerza los acompañe!";

            sWorldSessionMgr->SendServerMessage(SERVER_MSG_STRING, startMessage.str().c_str());

            // Notificar a los jugadores
            ChatHandler(challenger->GetSession()).PSendSysMessage("¡Has desafiado a {} a un Mak'Gora! ¡No hay segundas oportunidades!", opponent->GetName().c_str());
            ChatHandler(opponent->GetSession()).PSendSysMessage("¡{} te ha desafiado a un Mak'Gora! ¡Solo uno quedará en pie!", challenger->GetName().c_str());
        }
    }

    void OnPlayerDuelEnd(Player* winner, Player* loser, DuelCompleteType type) override
    {
        if (!winner || !loser)
            return;

        // Verificar si el sistema Mak'Gora está habilitado
        if (!sConfigMgr->GetOption<bool>("Makgora.Enable", false))
            return;  // Si no está habilitado, no se aplica la lógica del Mak'Gora

        // Verificar si el duelo fue un Mak'Gora
        if (makgoraPlayers[winner] && makgoraPlayers[loser])
        {
            if (type == DUEL_WON || type == DUEL_FLED)
            {
                std::ostringstream deathMessage;
                deathMessage << "|cffff0000[Mak'Gora] |cffffd700" << loser->GetName()
                    << "|r ha sido derrotado por |cffffd700" << winner->GetName()
                    << "|r. ¡Gloria al vencedor!";

                std::ostringstream deathhardcore;
                deathhardcore << "|cffff0000[Hardcore] ¡El jugador |cffffd700" << loser->GetName()
                    << "|cffff0000 ha sido asesinado en nivel " << static_cast<int>(loser->GetLevel())
                    << "|cffff0000 por el jugador |cffffd700"
                    << winner->GetName() << "|cffff0000 de nivel " << static_cast<int>(winner->GetLevel()) << "|cffff0000!Que descanse en paz...|r";

                std::ostringstream deathplayer;
                deathplayer << "|TInterface\\Icons\\Ability_DualWield:60|t\n"  // Icono arriba con salto de línea
                    << "|cffff0000¡El jugador |cffffd700"
                    << loser->GetName() << "|cffff0000 ha sido asesinado"
                    << "|cffff0000 por el jugador |cffffd700" << winner->GetName()
                    << "|cffff0000, en nivel " << static_cast<int>(loser->GetLevel())
                    << "|cffff0000!";

                WorldPacket data(SMSG_NOTIFICATION, (deathplayer.str().size() + 1));
                data << deathplayer.str();
                sWorldSessionMgr->SendGlobalMessage(&data);

                sWorldSessionMgr->SendServerMessage(SERVER_MSG_STRING, deathMessage.str().c_str());
                sWorldSessionMgr->SendServerMessage(SERVER_MSG_STRING, deathhardcore.str().c_str());

                // El perdedor muere permanentemente
                loser->KillPlayer();
                loser->SetHealth(0);  // Asegura que el jugador esté muerto completamente
                loser->RemoveAllAuras();  // Elimina cualquier aura que esté afectando al jugador

                loser->GetSession()->SendAreaTriggerMessage("¡Has sido derrotado en Mak'Gora! Puedes liberar tu espíritu.");

                // Registrar victoria en la base de datos
                QueryResult result = CharacterDatabase.Query("SELECT victories FROM player_makgora_victories WHERE player_id = {}", winner->GetGUID().GetCounter());
                uint32 victoryCount = 0;
                if (result)
                {
                    Field* fields = result->Fetch();
                    victoryCount = fields[0].Get<uint32>();
                    victoryCount++;
                    CharacterDatabase.Execute("UPDATE player_makgora_victories SET victories = {} WHERE player_id = {}", victoryCount, winner->GetGUID().GetCounter());
                }
                else
                {
                    // Si el jugador no tiene entradas en la tabla, insertamos una nueva fila
                    CharacterDatabase.Execute("INSERT INTO player_makgora_victories (player_id, victories) VALUES ({}, 1)", winner->GetGUID().GetCounter());
                    victoryCount = 1;
                }

                // Anunciar la victoria
                ChatHandler(winner->GetSession()).PSendSysMessage("¡Victoria {} en Mak'Gora!", victoryCount);
            }
        }

        // Limpiar estado de Mak'Gora
        makgoraPlayers.erase(winner);
        makgoraPlayers.erase(loser);
    }
};

class MakgoraCommand : public CommandScript
{
public:
    MakgoraCommand() : CommandScript("MakgoraCommand") {}

    ChatCommandTable GetCommands() const override
    {
        static ChatCommandTable makgoraCommandTable =
        {
            // Usar .makgora y luego mostrar un mensaje con
            { "makgora", HandleMakgoraCommand, SEC_PLAYER, Console::No },
        };

        return makgoraCommandTable;
    }

    static bool HandleMakgoraCommand(ChatHandler* handler, const char* /*args*/)
    {
        // Verificar si el sistema Mak'Gora está habilitado
        if (!sConfigMgr->GetOption<bool>("Makgora.Enable", false))
        {
            handler->SendSysMessage("El sistema de Mak'Gora está deshabilitado en este momento.");
            return false;
        }

        Player* challenger = handler->GetSession()->GetPlayer();
        Player* opponent = handler->getSelectedPlayer();

        if (!opponent)
        {
            handler->SendSysMessage("Debes seleccionar un oponente para Mak'Gora.");
            return false;
        }

        if (challenger == opponent)
        {
            handler->SendSysMessage("No puedes desafiarte a ti mismo.");
            return false;
        }

        if (challenger->duel || opponent->duel)
        {
            handler->SendSysMessage("Uno de los jugadores ya está en un duelo.");
            return false;
        }

        // Marcar a los jugadores como Mak'Gora
        makgoraPlayers[challenger] = true;
        makgoraPlayers[opponent] = true;

        // Iniciar el duelo con el hechizo de duelo (ID: 7266)
        challenger->CastSpell(opponent, 7266, true);

        // Mensajes
        handler->PSendSysMessage("¡Has desafiado a {} a un Mak'Gora!", opponent->GetName().c_str());
        if (opponent->GetSession())
        {
            ChatHandler(opponent->GetSession()).PSendSysMessage("¡{} te ha desafiado a un Mak'Gora!", challenger->GetName().c_str());
        }

        return true;
    }
};

void AddSC_makgora()
{
    new MakgoraMode();
    new MakgoraCommand();
}
