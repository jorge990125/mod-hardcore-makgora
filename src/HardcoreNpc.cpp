#include "ScriptMgr.h"
#include "Player.h"
#include "Config.h"
#include "Chat.h"
#include "GossipDef.h"
#include "ScriptedGossip.h"
#include "ScriptedCreature.h"
#include "WorldSessionMgr.h"

class npc_hardcore_ranking : public CreatureScript
{
public:
    npc_hardcore_ranking() : CreatureScript("npc_hardcore_ranking") { }

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        player->PlayerTalkClass->ClearMenus();

        // Consultar los NPCs que han matado jugadores
        QueryResult result = CharacterDatabase.Query(
            "SELECT npc_id, kill_count FROM npc_hardcore_ranking WHERE kill_count > 0 ORDER BY kill_count DESC LIMIT 20");

        if (!result)
        {
            // Si no hay registros, mostrar mensaje
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, "No hay registros de NPCs asesinos.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
        }
        else
        {
            // Si hay registros, mostrar la lista
            std::ostringstream menuText;
            menuText << "|TInterface\\Icons\\Ability_DualWield:20|t |cffff0000Ranking de NPCs Asesinos:|r\n\n";

            do
            {
                Field* fields = result->Fetch();
                uint32 npcId = fields[0].Get<uint32>();
                uint32 killCount = fields[1].Get<uint32>();

                // Obtener el nombre del NPC desde creature_template
                QueryResult npcNameResult = WorldDatabase.Query("SELECT name FROM creature_template WHERE entry = {}", npcId);
                std::string npcName = npcNameResult ? npcNameResult->Fetch()->Get<std::string>() : "Desconocido";

                // Formato del texto: "NPC Nombre - X muertes"
                menuText << "|cff0000ff" << npcName << "|r - " << killCount << " muertes\n";

            } while (result->NextRow());

            // Agregar texto al menú de Gossip
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, menuText.str(), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
        }

        // Mostrar el menú de Gossip (esto muestra las opciones)
        SendGossipMenuFor(player, 1, creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* /*creature*/, uint32 /*sender*/, uint32 /*action*/) override
    {
        player->PlayerTalkClass->ClearMenus();
        return true;
    }
};

class npc_hardcore_warning : public CreatureScript
{
public:
    npc_hardcore_warning() : CreatureScript("npc_hardcore_warning") { }

    struct npc_hardcore_warningAI : ScriptedAI
    {
        npc_hardcore_warningAI(Creature* creature) : ScriptedAI(creature) { }

        uint32 uiAdATimer;
        uint32 uiAdBTimer;
        uint32 uiAdCTimer;
        uint32 uiAdDTimer;
        uint32 uiDanceTimer; // Temporizador para la animación de baile

        void Reset()
        {
            uiAdATimer = 1000;
            uiAdBTimer = 23000;
            uiAdCTimer = 11000;
            uiAdDTimer = 23000;
            uiDanceTimer = 15000; // El baile comienza después de 15 segundos
        }

        void UpdateAI(const uint32 diff)
        {
            if (uiAdATimer <= diff)
            {
                me->Say("¡Arr, qué tenemos aquí! Otro alma valiente o un incauto buscando suerte en los mares... ¿Qué te trae a mis dominios, forastero?", LANG_UNIVERSAL, NULL);
                me->HandleEmoteCommand(EMOTE_ONESHOT_EXCLAMATION);
                uiAdATimer = 122000;
            }
            else
                uiAdATimer -= diff;

            if (uiAdBTimer <= diff)
            {
                me->Say("¿Mi historia? ¡Haría falta una botella de ron para contarla completa! Pero si insistes... Hace muchos años surqué los mares en busca de tesoros perdidos y batallas que recordarían los océanos por siglos. Pero eso, amigo, es una historia para otro momento. El oro de hoy es lo que importa.", LANG_UNIVERSAL, NULL);
                me->HandleEmoteCommand(EMOTE_ONESHOT_EXCLAMATION);
                uiAdBTimer = 122000;
            }
            else
                uiAdBTimer -= diff;

            if (uiAdCTimer <= diff)
            {
                me->Say("¡Mi barco! El viento imparable, una bestia de los mares. ¡Nadie se atreve a desafiarla en estas aguas! Si alguna vez deseas unirte a mi tripulación, piensa bien antes de hacerlo... ser pirata no es solo riquezas, también hay que saber pelear, navegar y... sobrevivir.", LANG_UNIVERSAL, NULL);
                me->HandleEmoteCommand(EMOTE_ONESHOT_EXCLAMATION);
                uiAdCTimer = 122000;
            }
            else
                uiAdCTimer -= diff;

            if (uiAdDTimer <= diff)
            {
                me->Say("Yo ho ho y una botella de ron, ¡seremos los más fuertes de la región!...¡Malditos sean los cobardes que no se atreven a tomar el desafío! ¡Si no tienes agallas, mejor vete al infierno!", LANG_UNIVERSAL, NULL);
                me->HandleEmoteCommand(EMOTE_ONESHOT_EXCLAMATION);
                uiAdDTimer = 122000;
            }
            else
                uiAdDTimer -= diff;

            // Animación de baile
            if (uiDanceTimer <= diff)
            {
                me->HandleEmoteCommand(EMOTE_ONESHOT_DANCE);  // El NPC comienza a bailar
                uiDanceTimer = 30000;  // El baile dura 30 segundos, luego se repite
            }
            else
                uiDanceTimer -= diff;

            // Animación de baile
            if (uiDanceTimer <= diff)
            {
                me->HandleEmoteCommand(TEXT_EMOTE_WHISTLE);  // El NPC comienza a bailar
                uiDanceTimer = 122000;  // El baile dura 30 segundos, luego se repite
            }
            else
                uiDanceTimer -= diff;
        }
    };
    
    bool OnGossipHello(Player* player, Creature* creature) override
    {
        player->PlayerTalkClass->ClearMenus();

        // Comprobar si el jugador ya aceptó los términos
        QueryResult result = CharacterDatabase.Query("SELECT accepted FROM hardcore_terms WHERE player_guid = {}", player->GetGUID().GetCounter());
        if (result)
        {
            // Si ya aceptó, cerrar menú y evitar que lo vuelva a ver
            CloseGossipMenuFor(player);
            return true;
        }

        // Mostrar mensaje de advertencia sobre los reinos Hardcore
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, "|cFFFF0000¡Términos y condiciones!|r", GOSSIP_SENDER_MAIN, 1);
        AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "Te damos la bienvenida a Pixie Hardcore.", GOSSIP_SENDER_MAIN, 1);
        AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "Los personajes que mueren en el reinos Hardcore no pueden resucitar en |cFFFF0000NINGÚN|r caso.", GOSSIP_SENDER_MAIN, 1);
        AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "Asistencia al cliente no puede restaurar personajes Hardcore muertos.", GOSSIP_SENDER_MAIN, 1);
        AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "Si accedes a jugar en estos reinos, aceptas que la muerte de tu personaje será permanente.", GOSSIP_SENDER_MAIN, 1);
        AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "Esto incluye desconexiones, problemas de lag, caídas de los servidores, errores de juego o cualquier otro motivo.", GOSSIP_SENDER_MAIN, 1);
        AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "Las muertes en actividades JcJ consensuadas (como Duelo a muerte o el JcJ voluntario) forman parte del juego.", GOSSIP_SENDER_MAIN, 1);
        AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "Debes mostrar una actitud positiva y no participar en situaciones de acoso, odio o abuso.", GOSSIP_SENDER_MAIN, 1);
        AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "Acostúmbrate a un juego peligroso, pero es un juego y vienes a pasártelo bien. Pórtate de una forma amistosa.", GOSSIP_SENDER_MAIN, 1);
        AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "Pixie se reserva el derecho a tomar medidas estrictas frente a comportamientos deliberadamente negativos en estos reinos.", GOSSIP_SENDER_MAIN, 1);
        AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, "Cualquier jugador que cause la muerte de un personaje de forma consciente y deliberada fuera de un contexto de JcJ consensuado podrá ver intervenidas sus cuentas.", GOSSIP_SENDER_MAIN, 1);

        // Agregar opciones de aceptación y rechazo
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, "[ Estoy de acuerdo ]", GOSSIP_SENDER_MAIN, 2);
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, "[ Rechazar ]", GOSSIP_SENDER_MAIN, 3);

        // Mostrar el menú de Gossip
        SendGossipMenuFor(player, 1, creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) override
    {
        player->PlayerTalkClass->ClearMenus();

        if (action == 2) // "Estoy de acuerdo"
        {
            uint32 agreeSpellId = 44940; // Reemplazar con el ID del hechizo de acuerdo
            player->CastSpell(player, agreeSpellId, true);
            CloseGossipMenuFor(player);

            // Listado de mensajes de acuerdo, seleccionamos uno aleatorio
            std::vector<std::string> dialogs = {
                "¡Hoy es un gran día para morir!",
                "¡Prepárate para un desafío que pondrá a prueba tu resistencia!",
                "¡Hazlo por el honor de los caídos!",
                "¡La muerte es solo el comienzo de una gran aventura!",
                "¡No hay vuelta atrás, aventurero! ¡Este es tu destino!"
                "'Modo Hardcore... Solo los más fuertes sobreviven, marinero. Si caes, será el final de tu historia. ¡No te arrepientas ahora!'",
                "'Este es el verdadero reto, marinero. En el modo Hardcore, la muerte es definitiva. ¿Estás listo para desafiar la suerte?'",
                "'¡Solo los valientes se atreven a jugar en este modo! La muerte te espera a la vuelta de cada esquina... ¡cuidado!'",
                "'¡Nada de revivir aquí! Si caes, caes para siempre. Solo los más fuertes se quedan. ¿Lo eres tú?'"
            };

            // Selecciona un mensaje aleatorio
            srand(time(NULL)); // Inicializa la semilla aleatoria
            int randomIndex = rand() % dialogs.size(); // Obtiene un índice aleatorio
            creature->Say(dialogs[randomIndex].c_str(), LANG_UNIVERSAL, NULL);

            // Dar items
            for (uint32 itemId : {38186})
            {
                if (player->HasItemCount(itemId, 1, true))
                    continue;

                player->AddItem(itemId, 1);
            }

            // Guardar en la base de datos que el jugador aceptó los términos
            CharacterDatabase.Execute("INSERT INTO hardcore_terms (player_guid, accepted) VALUES ({}, 1) ON DUPLICATE KEY UPDATE accepted=1", player->GetGUID().GetCounter());
        }
        else if (action == 3) // "Rechazar"
        {
            player->GetSession()->KickPlayer();
            CloseGossipMenuFor(player);

            // Listado de mensajes de rechazo, seleccionamos uno aleatorio
            std::vector<std::string> rejectDialogs = {
                "¡No todos están preparados para enfrentar la muerte!",
                "Me temo que no todos tienen lo necesario para ser parte de este reto...",
                "Lamentablemente, no eres apto para enfrentar este desafío.",
                "Aún no estás listo para el desafío, pero tal vez algún día...",
                "¡Te has perdido de una gran aventura, aventurero!"
            };

            // Selecciona un mensaje aleatorio
            srand(time(NULL)); // Inicializa la semilla aleatoria
            int randomIndex = rand() % rejectDialogs.size(); // Obtiene un índice aleatorio
            creature->Say(rejectDialogs[randomIndex].c_str(), LANG_UNIVERSAL, NULL);
        }
        return true;
    }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_hardcore_warningAI(creature);
    }
};

void AddSC_mod_harcore_npc()
{
    new npc_hardcore_ranking();
    new npc_hardcore_warning();
}
