/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */

// From SC
void AddSC_mod_hardcore();
void AddSC_mod_hardcore_npc();
void AddSC_makgora();

// Add all
// cf. the naming convention https://github.com/azerothcore/azerothcore-wotlk/blob/master/doc/changelog/master.md#how-to-upgrade-4
// additionally replace all '-' in the module folder name with '_' here
void Addmod_hardcore_makgoraScripts()
{
    AddSC_mod_hardcore();
    AddSC_mod_hardcore_npc();
    AddSC_makgora();
}
