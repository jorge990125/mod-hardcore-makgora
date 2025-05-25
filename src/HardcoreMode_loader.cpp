/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */

// From SC
void AddSC_mod_harcore();
void AddSC_mod_harcore_npc();
void AddSC_makgora();

// Add all
// cf. the naming convention https://github.com/azerothcore/azerothcore-wotlk/blob/master/doc/changelog/master.md#how-to-upgrade-4
// additionally replace all '-' in the module folder name with '_' here
void Addmod_hardcoreScripts()
{
    AddSC_mod_harcore();
    AddSC_mod_harcore_npc();
    AddSC_makgora();
}

