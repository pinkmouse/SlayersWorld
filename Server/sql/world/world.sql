/*
SQLyog Ultimate v11.11 (64 bit)
MySQL - 5.5.52-0+deb7u1 : Database - world
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

/*Table structure for table `characters` */

DROP TABLE IF EXISTS `characters`;

CREATE TABLE `characters` (
  `accountID` int(10) unsigned DEFAULT '0',
  `characterID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `name` tinytext NOT NULL,
  `skinID` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `level` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `health` tinyint(3) unsigned NOT NULL DEFAULT '100',
  `alignment` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `mapID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `posX` int(10) unsigned NOT NULL DEFAULT '0',
  `posY` int(10) unsigned NOT NULL DEFAULT '0',
  `orientation` smallint(5) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`characterID`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8;

/*Data for the table `characters` */

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

/*Data for the table `characters_respawn` */

/*Table structure for table `creature` */

DROP TABLE IF EXISTS `creature`;

CREATE TABLE `creature` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `mapID` smallint(6) NOT NULL DEFAULT '0',
  `entry` int(11) NOT NULL DEFAULT '0',
  `posX` int(11) NOT NULL DEFAULT '0',
  `posY` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=9 DEFAULT CHARSET=utf8;

/*Data for the table `creature` */

insert  into `creature`(`id`,`mapID`,`entry`,`posX`,`posY`) values (1,0,1,100,200),(2,0,2,120,200),(3,0,1,160,160),(4,0,1,200,200),(5,0,2,200,300),(6,0,2,160,260),(7,0,2,130,250),(8,0,1,350,300);

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
  `maxRay` mediumint(9) NOT NULL DEFAULT '5',
  `respawnTime` int(11) NOT NULL DEFAULT '5000',
  `rank` tinyint(4) NOT NULL DEFAULT '0',
  `aiType` tinyint(4) NOT NULL DEFAULT '0',
  PRIMARY KEY (`entry`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;

/*Data for the table `creature_template` */

insert  into `creature_template`(`entry`,`skinID`,`name`,`level`,`force`,`stamina`,`dexterity`,`xp`,`state`,`maxRay`,`respawnTime`,`rank`,`aiType`) values (1,1,'blob',0,0,0,0,20,0,5,5000,0,0),(2,66,'chevalier',0,0,0,0,10,0,5,5000,0,1);

/*Table structure for table `level_xp` */

DROP TABLE IF EXISTS `level_xp`;

CREATE TABLE `level_xp` (
  `level` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `xp` mediumint(8) unsigned NOT NULL DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `level_xp` */

insert  into `level_xp`(`level`,`xp`) values (1,50),(2,60),(3,70);

/*Table structure for table `login` */

DROP TABLE IF EXISTS `login`;

CREATE TABLE `login` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `login` varchar(50) NOT NULL,
  `password` varchar(50) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8;

/*Data for the table `login` */

insert  into `login`(`id`,`login`,`password`) values (4,'test2@test','098f6bcd4621d373cade4e832627b4f6');

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
