DROP TABLE IF EXISTS `player_makgora_victories`;
CREATE TABLE `player_makgora_victories` (
  `player_id` INT(11) NOT NULL,
  `victories` INT(11) DEFAULT 0,
  PRIMARY KEY (`player_id`)
);
