DROP TABLE IF EXISTS `npc_hardcore_ranking`;
CREATE TABLE IF NOT EXISTS npc_hardcore_ranking (
    npc_id INT UNSIGNED NOT NULL,
    kill_count INT UNSIGNED NOT NULL DEFAULT 0,
    PRIMARY KEY (npc_id)
);
