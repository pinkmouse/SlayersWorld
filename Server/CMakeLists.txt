cmake_minimum_required(VERSION 2.6)

project(SWServer)

set(myproject_VERSION_MAJOR 1)
set(myproject_VERSION_MINOR 0)

include_directories("${PROJECT_BINARY_DIR}")

set(EXECUTABLE_NAME "SWServer")
add_executable(${EXECUTABLE_NAME}
              server/server.cpp
              server/Define.hpp
              server/Global.hpp

              server/World/ClockHandler.cpp
              server/World/ClockHandler.hpp
              server/World/ConfigHandler.cpp
              server/World/ConfigHandler.hpp
              server/World/UnitManager.cpp
              server/World/UnitManager.hpp
              server/World/PacketDefine.hpp
              server/World/PacketHandler.cpp
              server/World/PacketHandler.hpp
              server/World/SqlManager.cpp
              server/World/SqlManager.hpp
              server/World/World.cpp
              server/World/World.hpp
              server/World/WorldPacket.cpp
              server/World/WorldPacket.hpp
              server/World/WorldSocket.cpp
              server/World/WorldSocket.hpp

	      server/System/WebHook.hpp
	      server/System/SWExit.hpp
	      server/System/SWType.hpp
              server/System/WorldPosition.cpp
              server/System/WorldPosition.hpp
	      server/System/Commands.cpp

	      server/System/Quest/Quest.cpp
              server/System/Quest/Quest.hpp
  	      server/System/Quest/QuestManager.cpp
              server/System/Quest/QuestManager.hpp

	      server/System/Item/Item.cpp
              server/System/Item/Item.hpp
  	      server/System/Item/ItemManager.cpp
              server/System/Item/ItemManager.hpp

	      server/System/Reward/Reward.cpp
              server/System/Reward/Reward.hpp
  	      server/System/Reward/RewardManager.cpp
              server/System/Reward/RewardManager.hpp

	      server/System/Required/Required.cpp
              server/System/Required/Required.hpp
	      server/System/Required/RequiredManager.cpp
              server/System/Required/RequiredManager.hpp
	      server/System/Required/SubRequired.cpp
              server/System/Required/SubRequired.hpp
	      server/System/Required/SubRequiredQuest.cpp
              server/System/Required/SubRequiredQuest.hpp
	      server/System/Required/SubRequiredLevel.cpp
              server/System/Required/SubRequiredLevel.hpp

	      server/System/Resource/Resource.cpp
              server/System/Resource/Resource.hpp
	      server/System/Resource/ResourceHealth.cpp
              server/System/Resource/ResourceHealth.hpp
	      server/System/Resource/ResourceMana.cpp
              server/System/Resource/ResourceMana.hpp
	      server/System/Resource/ResourceAlignment.cpp
              server/System/Resource/ResourceAlignment.hpp

	      server/System/Spell/SpellTemplate.cpp
              server/System/Spell/SpellTemplate.hpp
	      server/System/Spell/SpellManager.cpp
              server/System/Spell/SpellManager.hpp
	      server/System/Spell/Spell.cpp
              server/System/Spell/Spell.hpp
	      server/System/Spell/Aura.cpp
              server/System/Spell/Aura.hpp
	      server/System/Spell/AuraEffect.cpp
              server/System/Spell/AuraEffect.hpp

	      server/System/Instance/BattleGround/BGCapturePoint.cpp
              server/System/Instance/BattleGround/BGCapturePoint.hpp


              server/Map/Case.cpp
              server/Map/Case.hpp
              server/Map/Map.cpp
	      server/Map/PathFinding.cpp
              server/Map/Map.hpp
              server/Map/MapTemplate.cpp
              server/Map/MapTemplate.hpp
              server/Map/MapManager.cpp
              server/Map/MapManager.hpp
              server/Map/MapNetwork.cpp
              server/Map/Square.cpp
              server/Map/Square.hpp

              server/Entities/Creature.cpp
	      server/Entities/CreatureAI.cpp
              server/Entities/Creature.hpp
              server/Entities/AnimationUnit.cpp
              server/Entities/AnimationUnit.hpp
              server/Entities/MovementHandler.cpp
              server/Entities/MovementHandler.hpp
              server/Entities/MovementHandlerCreature.cpp
              server/Entities/MovementHandlerCreature.hpp
              server/Entities/Player.cpp
              server/Entities/Player.hpp
              server/Entities/Unit.cpp
              server/Entities/Unit.hpp
              server/Entities/GroupManager.cpp
              server/Entities/GroupManager.hpp
              server/Entities/WorldObject.cpp
              server/Entities/WorldObject.hpp
              server/Entities/DynamicObject.cpp
              server/Entities/DynamicObject.hpp
              server/Entities/DynamicObjectManager.cpp
              server/Entities/DynamicObjectManager.hpp
              server/Entities/GameObject.cpp
              server/Entities/GameObject.hpp
              server/Entities/Areatrigger.cpp
              server/Entities/Areatrigger.hpp
              server/Entities/LevelManager.cpp
              server/Entities/LevelManager.hpp
              )

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

find_package(SFML 2 REQUIRED system window graphics network audio)
find_package(MySQL REQUIRED)
find_package(CURL REQUIRED)

include_directories(${SFML_INCLUDE_DIR}
                    ${MYSQL_INCLUDE_DIR}
		    ${CURL_INCLUDE_DIR})
target_link_libraries(${EXECUTABLE_NAME} ${MYSQL_LIBRARY} ${SFML_LIBRARIES} ${CURL_LIBRARIES})
add_definitions(-D_CRT_SECURE_NO_WARNINGS)

if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-std=gnu++0x)
endif()

# Install target
install(TARGETS ${EXECUTABLE_NAME} DESTINATION bin)
