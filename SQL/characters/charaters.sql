CREATE TABLE `characters` (
	`accountID` INT(10) UNSIGNED NULL DEFAULT '0',
	`characterID` INT(10) UNSIGNED NOT NULL AUTO_INCREMENT,
	`name` TINYTEXT NOT NULL,
	`level` TINYINT(3) UNSIGNED NOT NULL DEFAULT '0',
	`mapID` SMALLINT(5) UNSIGNED NOT NULL DEFAULT '0',
	`posX` INT(10) UNSIGNED NOT NULL DEFAULT '0',
	`posY` INT(10) UNSIGNED NOT NULL DEFAULT '0',
	PRIMARY KEY (`characterID`)
)
COLLATE='utf8_general_ci'
ENGINE=InnoDB
;
