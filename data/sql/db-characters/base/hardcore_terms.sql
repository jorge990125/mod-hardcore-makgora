/*
Navicat MySQL Data Transfer

Source Server         : wow
Source Server Version : 80036
Source Host           : 127.0.0.1:3306
Source Database       : hardcore_characters

Target Server Type    : MYSQL
Target Server Version : 80036
File Encoding         : 65001

Date: 2025-02-17 22:18:26
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for hardcore_terms
-- ----------------------------
DROP TABLE IF EXISTS `hardcore_terms`;
CREATE TABLE `hardcore_terms` (
  `player_guid` int unsigned NOT NULL,
  `accepted` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`player_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
