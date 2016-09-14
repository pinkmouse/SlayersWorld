# SlayersWorld
> Inspired by [slayersonline](http://http://www.slayersonline.net/)

### Server

  - Make a characters DB and execute the sql file
  - Insert new account in login
  - Write `server.conf` and place it side to server.exe

`server.conf`
```sh
Port = 1234
LoginDebug = 0
charactersDB = 127.0.0.1;root;root;characters;3306;
```

  - Add map folder side to server.exe
  - Add libmysql.dll side to server.exe
  - Create account on Database

```INSERT INTO `login` (login, password) VALUES ('test@test', MD5('test'));```

  - Create a player
  
```INSERT INTO `characters` (accountID, name, health, alignment) VALUES (1, 'toto', 100, 0);```
  
### Client

  - Add datas folder side to client.exe (map, font, img)
  - Write a little launcher(windows) because client.exe need param

`launcher.bat`
```sh
start "" "D:\Projects\Build\Client\client.exe" test@test.fr test
```

**We works Yeah!**
