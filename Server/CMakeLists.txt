cmake_minimum_required(VERSION 2.6)

project(SWserver)

set(myproject_VERSION_MAJOR 1)
set(myproject_VERSION_MINOR 0)

include_directories("${PROJECT_BINARY_DIR}")

set(EXECUTABLE_NAME "SWserver")
add_executable(${EXECUTABLE_NAME}
              server/server.cpp
              server/Define.hpp
              server/Global.hpp

              server/World/ClockHandler.cpp
              server/World/ClockHandler.hpp
              server/World/ConfigHandler.cpp
              server/World/ConfigHandler.hpp
              server/World/CreatureManager.cpp
              server/World/CreatureManager.hpp
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

              server/System/WorldPosition.cpp
              server/System/WorldPosition.hpp
	      server/System/Commands.cpp

              server/Map/Case.cpp
              server/Map/Case.hpp
              server/Map/Map.cpp
              server/Map/Map.hpp
              server/Map/MapManager.cpp
              server/Map/MapManager.hpp
              server/Map/MapNetwork.cpp
              server/Map/Square.cpp
              server/Map/Square.hpp

              server/Entities/Creature.cpp
              server/Entities/Creature.hpp
              server/Entities/MovementHandler.cpp
              server/Entities/MovementHandler.hpp
              server/Entities/Player.cpp
              server/Entities/Player.hpp
              server/Entities/Unit.cpp
              server/Entities/Unit.hpp
              server/Entities/WorldObject.cpp
              server/Entities/WorldObject.hpp
              server/Entities/LevelManager.cpp
              server/Entities/LevelManager.hpp
              )

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

find_package(SFML 2 REQUIRED system window graphics network audio)
find_package(MySQL REQUIRED)

include_directories(${SFML_INCLUDE_DIR}
                    ${MYSQL_INCLUDE_DIR})
target_link_libraries(${EXECUTABLE_NAME} ${MYSQL_LIBRARY} ${SFML_LIBRARIES})

if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-std=gnu++0x)
endif()

# Install target
install(TARGETS ${EXECUTABLE_NAME} DESTINATION bin)
