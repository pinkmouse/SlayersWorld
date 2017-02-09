/*
SQLyog Community v12.3.2 (32 bit)
MySQL - 5.7.17-log : Database - world
*********************************************************************
*/

/*!40101 SET NAMES utf8 */;

/*!40101 SET SQL_MODE=''*/;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;
CREATE DATABASE /*!32312 IF NOT EXISTS*/`world` /*!40100 DEFAULT CHARACTER SET utf8 */;

USE `world`;

/*Table structure for table `creature` */

DROP TABLE IF EXISTS `creature`;

CREATE TABLE `creature` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `mapID` smallint(6) NOT NULL DEFAULT '0',
  `entry` int(11) NOT NULL DEFAULT '0',
  `posX` int(11) NOT NULL DEFAULT '0',
  `posY` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=57 DEFAULT CHARSET=utf8;

/*Table structure for table `creature_template` */

DROP TABLE IF EXISTS `creature_template`;

CREATE TABLE `creature_template` (
  `entry` int(11) NOT NULL AUTO_INCREMENT,
  `skinID` tinyint(4) NOT NULL DEFAULT '0',
  `name` varchar(50) DEFAULT '',
  `level` tinyint(4) NOT NULL DEFAULT '0',
  `force` tinyint(4) NOT NULL DEFAULT '0',
  `stamina` tinyint(4) NOT NULL DEFAULT '0',
  `dexterity` tinyint(4) NOT NULL DEFAULT '0',
  `xp` tinyint(4) NOT NULL DEFAULT '0',
  `state` tinyint(4) NOT NULL DEFAULT '0',
  `maxRay` mediumint(9) unsigned NOT NULL DEFAULT '5',
  `maxVision` mediumint(9) unsigned NOT NULL DEFAULT '3',
  `movingTimeMin` float NOT NULL DEFAULT '1',
  `movingTimeMax` float NOT NULL DEFAULT '2',
  `stopTimeMin` float NOT NULL DEFAULT '1',
  `stopTimeMax` float NOT NULL DEFAULT '2',
  `respawnTime` int(11) NOT NULL DEFAULT '5000',
  `rank` tinyint(4) NOT NULL DEFAULT '0',
  `aiType` tinyint(4) NOT NULL DEFAULT '0',
  `faction` tinyint(4) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`entry`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8;

/*Table structure for table `gossip` */

DROP TABLE IF EXISTS `gossip`;

CREATE TABLE `gossip` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `typeUnit` tinyint(4) unsigned NOT NULL,
  `unitEntry` int(11) unsigned NOT NULL,
  `type` tinyint(4) unsigned NOT NULL,
  `data1` int(11) DEFAULT NULL,
  `msg` varchar(50) DEFAULT NULL,
  KEY `id` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;

/*Table structure for table `level_xp` */

DROP TABLE IF EXISTS `level_xp`;

CREATE TABLE `level_xp` (
  `level` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `xp` mediumint(8) unsigned NOT NULL DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Table structure for table `objectif_quest_template` */

DROP TABLE IF EXISTS `objectif_quest_template`;

CREATE TABLE `objectif_quest_template` (
  `questID` int(10) unsigned NOT NULL,
  `id` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `typeID` tinyint(3) unsigned NOT NULL,
  `data0` int(11) NOT NULL DEFAULT '-1',
  `data1` int(11) NOT NULL DEFAULT '-1',
  `data2` int(11) NOT NULL DEFAULT '-1',
  `data3` int(11) NOT NULL DEFAULT '-1'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Table structure for table `quest_template` */

DROP TABLE IF EXISTS `quest_template`;

CREATE TABLE `quest_template` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `repetitionType` tinyint(3) unsigned NOT NULL DEFAULT '0',
  KEY `id` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;

/*Table structure for table `spell_effect` */

DROP TABLE IF EXISTS `spell_effect`;

CREATE TABLE `spell_effect` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `effectType` tinyint(4) DEFAULT NULL,
  `target` tinyint(3) unsigned NOT NULL,
  `basepoint1` int(11) NOT NULL DEFAULT '0',
  `basepoint2` int(11) NOT NULL DEFAULT '0',
  `basepoint3` int(11) NOT NULL DEFAULT '0',
  `basepoint4` int(11) NOT NULL DEFAULT '0',
  `radiusMin` float DEFAULT '0',
  `radiusMax` float DEFAULT '0',
  KEY `id` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;

/*Table structure for table `spell_template` */

DROP TABLE IF EXISTS `spell_template`;

CREATE TABLE `spell_template` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `level` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `visualID` int(10) NOT NULL DEFAULT '-1',
  `visualIDTarget` int(11) NOT NULL DEFAULT '-1',
  `castTime` int(10) unsigned NOT NULL DEFAULT '0',
  `cooldown` int(10) unsigned NOT NULL DEFAULT '0',
  `speed` float unsigned NOT NULL DEFAULT '0',
  `resourceType` tinyint(4) NOT NULL DEFAULT '-1',
  `resourceNb` int(11) NOT NULL DEFAULT '0',
  `effect1` int(11) NOT NULL DEFAULT '-1',
  `effect2` int(11) NOT NULL DEFAULT '-1',
  `effect3` int(11) NOT NULL DEFAULT '-1',
  `effect4` int(11) NOT NULL DEFAULT '-1',
  `name` text,
  KEY `id` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
