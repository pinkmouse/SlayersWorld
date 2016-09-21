
-- Export de la structure de la table world. creature
CREATE TABLE IF NOT EXISTS `creature` (
  `id` smallint(6) NOT NULL AUTO_INCREMENT,
  `entry` int(11) NOT NULL DEFAULT '0',
  `posX` int(11) NOT NULL DEFAULT '0',
  `posY` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;

-- Les données exportées n'étaient pas sélectionnées.
-- Export de la structure de la table world. creature_template
CREATE TABLE IF NOT EXISTS `creature_template` (
  `entry` int(11) NOT NULL AUTO_INCREMENT,
  `skinID` tinyint(4) DEFAULT '0',
  `name` varchar(50) DEFAULT '',
  `level` tinyint(4) DEFAULT '0',
  `force` tinyint(4) DEFAULT '0',
  `stamina` tinyint(4) DEFAULT '0',
  `dexterity` tinyint(4) DEFAULT '0',
  `xp` tinyint(4) DEFAULT '0',
  `state` tinyint(4) DEFAULT '0',
  PRIMARY KEY (`entry`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;

-- Les données exportées n'étaient pas sélectionnées.
/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
