

-- Export de la structure de la base pour world
CREATE DATABASE IF NOT EXISTS `world` /*!40100 DEFAULT CHARACTER SET utf8 */;
USE `world`;

-- Export de la structure de la table world. creature
CREATE TABLE IF NOT EXISTS `creature` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `mapID` smallint(6) NOT NULL DEFAULT '0',
  `entry` int(11) NOT NULL DEFAULT '0',
  `posX` int(11) NOT NULL DEFAULT '0',
  `posY` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=9 DEFAULT CHARSET=utf8;

-- Export de données de la table world.creature : ~8 rows (environ)
/*!40000 ALTER TABLE `creature` DISABLE KEYS */;
INSERT INTO `creature` (`id`, `mapID`, `entry`, `posX`, `posY`) VALUES
	(1, 0, 1, 100, 100),
	(2, 0, 2, 120, 100),
	(3, 0, 1, 60, 60),
	(4, 0, 1, 200, 200),
	(5, 0, 2, 200, 100),
	(6, 0, 2, 160, 60),
	(7, 0, 2, 30, 250),
	(8, 0, 1, 350, 100);
/*!40000 ALTER TABLE `creature` ENABLE KEYS */;

-- Export de la structure de la table world. creature_template
CREATE TABLE IF NOT EXISTS `creature_template` (
  `entry` int(11) NOT NULL AUTO_INCREMENT,
  `skinID` tinyint(4) NOT NULL DEFAULT '0',
  `name` varchar(50) DEFAULT '',
  `level` tinyint(4) NOT NULL DEFAULT '0',
  `force` tinyint(4) NOT NULL DEFAULT '0',
  `stamina` tinyint(4) NOT NULL DEFAULT '0',
  `dexterity` tinyint(4) NOT NULL DEFAULT '0',
  `xp` tinyint(4) NOT NULL DEFAULT '0',
  `state` tinyint(4) NOT NULL DEFAULT '0',
  `maxRay` mediumint(9) NOT NULL DEFAULT '5',
  `respawnTime` int(11) NOT NULL DEFAULT '5000',
  `rank` tinyint(4) NOT NULL DEFAULT '0',
  `aiType` tinyint(4) NOT NULL DEFAULT '0',
  PRIMARY KEY (`entry`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;

-- Export de données de la table world.creature_template : ~2 rows (environ)
/*!40000 ALTER TABLE `creature_template` DISABLE KEYS */;
INSERT INTO `creature_template` (`entry`, `skinID`, `name`, `level`, `force`, `stamina`, `dexterity`, `xp`, `state`, `maxRay`, `respawnTime`, `rank`, `aiType`) VALUES
	(1, 1, 'blob', 0, 0, 0, 0, 20, 0, 5, 5000, 0, 0),
	(2, 0, 'chevalier', 0, 0, 0, 0, 10, 0, 5, 5000, 0, 1);
/*!40000 ALTER TABLE `creature_template` ENABLE KEYS */;

-- Export de la structure de la table world. level_xp
CREATE TABLE IF NOT EXISTS `level_xp` (
  `level` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `xp` mediumint(8) unsigned NOT NULL DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Export de données de la table world.level_xp : ~2 rows (environ)
/*!40000 ALTER TABLE `level_xp` DISABLE KEYS */;
INSERT INTO `level_xp` (`level`, `xp`) VALUES
	(1, 50),
	(2, 60),
	(3, 70);
/*!40000 ALTER TABLE `level_xp` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
