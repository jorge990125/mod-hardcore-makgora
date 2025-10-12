# Hardcore Mod
Modulo para emulador 3.3.5a Azerothcore.
Intento de replicar un comportamiento blizzlike de la modalidad hardcore de vanilla, la cual tendra opciones de de niveles establecidos y recompenzas por niveles.

# Configuración
Activar hardcore
ModHardcore.Enable
-  Description: Enable Hardcore Mod.
-  Default: 0 - Disabled 1 - Enabled

# Rango de niveles 
ModHardcoreMinLevel.Enable and ModHardcoreMaxLevel.Enable
-  Description: Disable Hardcore Level at Min-Max range.
-  Default: ModHardcoreMinLevel.Enable = 10 ModHardcoreMaxLevel.Enable = 79

Ejemplo
- Establece un rango de nivel 10 a 79 para habilitar el modo extremo, de modo que en los niveles 1-9 y 80 el modo extremo esté deshabilitado.
- Establece un rango de nivel 1 a 80 para habilitar el modo extremo en todos los niveles.

# Recomepzas
Reward.Enable
-  Description:   Enable Reward for level 60,70,80.
-  Default: 0 - Disabled 1 - Enabled

Las recompenzas estaran disponibles en los niveles 60,70, 80. Las cuales seran recompenzadas por dos item y un titulo.

Reward.TitleLevel60
 -   Description: Title ID to assign when the player reaches level 60.
-    Default:     0 - Disabled (no title granted)

Reward.TitleLevel60 = 112 (id titulo)

Reward.Item1.Level60 / Reward.Item2.Level60
-    Description: Item IDs to grant when the player reaches level 60.
-    Default:     0 - Disabled (no item granted)

Reward.Item1.Level60 = 57578 (id item de recompenza 1)
Reward.Item2.Level60 = 39656 (id item de recompenza 2)

# Duelos Mak´Gora
Sistema de duelos usados a traves del comando ".makgora " la cual realizara duelos a muerte. Cualquier duelo realizado por este sistema ocasionara muerte permanente.

Makgora.Enable
-    Description:   Enable Makgora system duel.
-    Default: 0 - Disabled 1 - Enabled 

Makgora.Enable = 1

# Función todavia no verificada su funcionamiento
ModHardcoreGMCanResurrect.Enable
-  Description: Enable GM resurrection.
-  Default: 0 - Disabled 1 - Enabled

# Autor 
Rexol -  * [jorge990125](https://github.com/jorge990125): (Creador del modulo):

# Instalación
Follow official guide: https://www.azerothcore.org/wiki/installation

# Modulo usado como base del usuario * [Zindokar](https://github.com/Zindokar)
https://github.com/Zindokar/mod-hardcore
