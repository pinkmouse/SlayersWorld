/*
SQLyog Community v12.3.2 (32 bit)
MySQL - 5.7.17-log : Database - characters
*********************************************************************
*/

/*!40101 SET NAMES utf8 */;

/*!40101 SET SQL_MODE=''*/;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;
CREATE DATABASE /*!32312 IF NOT EXISTS*/`characters` /*!40100 DEFAULT CHARACTER SET utf8 */;

USE `characters`;

/*Table structure for table `account_key_binds` */

DROP TABLE IF EXISTS `account_key_binds`;

CREATE TABLE `account_key_binds` (
  `accountID` int(11) NOT NULL,
  `typeID` int(11) NOT NULL,
  `key` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Table structure for table `characters` */

DROP TABLE IF EXISTS `characters`;

CREATE TABLE `characters` (
  `accountID` int(10) unsigned DEFAULT '0',
  `characterID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `name` tinytext NOT NULL,
  `skinID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `level` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `health` tinyint(3) unsigned NOT NULL DEFAULT '100',
  `mana` tinyint(3) unsigned NOT NULL DEFAULT '100',
  `alignment` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `mapID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `posX` int(10) unsigned NOT NULL DEFAULT '0',
  `posY` int(10) unsigned NOT NULL DEFAULT '0',
  `orientation` smallint(5) unsigned NOT NULL DEFAULT '0',
  `xp` int(10) unsigned NOT NULL DEFAULT '0',
  `creationDate` datetime DEFAULT NULL,
  `lastConnection` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`characterID`)
) ENGINE=InnoDB AUTO_INCREMENT=26 DEFAULT CHARSET=utf8;

/*Table structure for table `characters_point` */

DROP TABLE IF EXISTS `characters_point`;

CREATE TABLE `characters_point` (
  `characterID` int(10) unsigned NOT NULL,
  `free_point` int(10) unsigned DEFAULT '0',
  `force` int(10) unsigned DEFAULT '0',
  `stamina` int(10) unsigned DEFAULT '0',
  `dexterity` int(10) unsigned DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Table structure for table `characters_respawn` */

DROP TABLE IF EXISTS `characters_respawn`;

CREATE TABLE `characters_respawn` (
  `characterID` int(11) DEFAULT '300',
  `posX` int(11) DEFAULT '300',
  `posY` int(11) DEFAULT '300',
  `mapID` smallint(6) DEFAULT '0',
  `orientation` tinyint(4) DEFAULT '0',
  UNIQUE KEY `characterID` (`characterID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Table structure for table `characters_spell_binds` */

DROP TABLE IF EXISTS `characters_spell_binds`;

CREATE TABLE `characters_spell_binds` (
  `characterID` int(10) unsigned NOT NULL,
  `bindID` int(10) unsigned NOT NULL,
  `spellID` int(10) unsigned NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Table structure for table `characters_spells` */

DROP TABLE IF EXISTS `characters_spells`;

CREATE TABLE `characters_spells` (
  `characterID` int(10) unsigned NOT NULL,
  `spellID` int(10) unsigned NOT NULL,
  `cooldown` int(10) unsigned NOT NULL DEFAULT '0',
  `key` int(11) NOT NULL DEFAULT '-1'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Table structure for table `connection_logs` */

DROP TABLE IF EXISTS `connection_logs`;

CREATE TABLE `connection_logs` (
  `accountID` int(10) unsigned DEFAULT NULL,
  `connectionDate` timestamp NULL DEFAULT CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Table structure for table `login` */

DROP TABLE IF EXISTS `login`;

CREATE TABLE `login` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `login` varchar(50) NOT NULL,
  `password` varchar(50) NOT NULL,
  `characterName` tinytext NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=26 DEFAULT CHARSET=utf8;

/*Table structure for table `login_access` */

DROP TABLE IF EXISTS `login_access`;

CREATE TABLE `login_access` (
  `accountID` int(10) unsigned NOT NULL,
  `accessType` tinyint(3) unsigned NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
