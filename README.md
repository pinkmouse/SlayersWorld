# SlayersWorld
> Inspired by [slayersonline](http://http://www.slayersonline.net/)

### Introduction

Slayers World is a project developed to provide a new engine for players looking to find the thrills of an old school MMORPG game.
After a year of development the project has taken a different path and Slayers World is no longer maintained, that's why today the project is available in open source with MIT License.

Slayers World has the basic features of a MMORPG, with management of items, quests, classes, spells, monsters, groups, ... and even a battleground.

Showcase :

https://github.com/pinkmouse/SlayersWorld/blob/master/_medias/BGvideo.mp4
![alt text](https://github.com/pinkmouse/SlayersWorld/blob/master/_medias/Capture3.png "Features")
![alt text](https://github.com/pinkmouse/SlayersWorld/blob/master/_medias/tease.gif "Features")

The goal is to provide people who want to learn about the subject a base on which they can learn and practice.

For the technical part, the whole game was developed in C++ (it is easily possible to adapt the code in another object-oriented language), the graphical and network engine is based on the [SFML library](https://www.sfml-dev.org/index-fr.php).

The project is split into two parts :

### Server


  - Make a characters DB and execute the sql file
  - Make a world DB and execute the sql file
  - Insert new account in login
  - Write `server.conf` and place it side to server.exe

  `server.conf`
  ```sh
  Port = 1234
  LoginDebug = 0
  charactersDB = 127.0.0.1;root;root;characters;3306;
  worldDB = 127.0.0.1;root;root;world;3306;
  ```

  - Add map folder side to server.exe
  - Add libmysql.dll side to server.exe
  - Create account on Database

    ```INSERT INTO `login` (login, password) VALUES ('test@test', MD5('test'));```

  - Create a player
  
    ```INSERT INTO `characters` (accountID, name, health, alignment) VALUES (1, 'toto', 100, 0);```
  
### Client

  - Add config file side to client.exe
  
      `config.wtf`
    ```
    IPServer = 127.0.0.1
    ```
    
  - Add datas folder side to client.exe (map, font, img)
  - TIPS : Write a little launcher(windows) because client.exe need param

    `launcher.bat`
    ```sh
    start "" "D:\Projects\Build\Client\client.exe" test@test.fr test
    ```

### Issues

  - Add entry on CMAKE(for windows only) 'SFML_ROOT' with root directory
  
**We roxx Yeah!**
