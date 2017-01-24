cmake_minimum_required(VERSION 2.6)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

project(SWClient)

set(myproject_VERSION_MAJOR 1)
set(myproject_VERSION_MINOR 0)

include_directories("${PROJECT_BINARY_DIR}")

set(EXECUTABLE_NAME "Client")

if( WIN32 )
  set(RC client/resource.rc)
endif()

add_executable(${EXECUTABLE_NAME}
              client/client.cpp
              client/Define.hpp
              client/Global.hpp

              client/World/ClockHandler.cpp
              client/World/ClockHandler.hpp
              client/World/ConfigHandler.cpp
              client/World/ConfigHandler.hpp
              client/World/PacketDefine.hpp
              client/World/PacketHandler.cpp
              client/World/PacketHandler.hpp
              client/World/World.cpp
              client/World/World.hpp
              client/World/WorldPacket.cpp
              client/World/WorldPacket.hpp
              client/World/Events/Events.cpp
              client/World/Events/Events.hpp

              client/System/WorldPosition.cpp
              client/System/WorldPosition.hpp

              client/System/Resource/Resource.cpp
              client/System/Resource/Resource.hpp

              client/Map/Case.cpp
              client/Map/Case.hpp
              client/Map/Map.cpp
              client/Map/Map.hpp
              client/Map/MapManager.cpp
              client/Map/MapManager.hpp

              client/Socket/Socket.cpp
              client/Socket/Socket.hpp

              client/Graphics/Interface/InterfaceManager.cpp
              client/Graphics/Interface/InterfaceManager.hpp
              client/Graphics/Interface/WritingField.cpp
              client/Graphics/Interface/WritingField.hpp
              client/Graphics/Interface/HistoryField.cpp
              client/Graphics/Interface/HistoryField.hpp
              client/Graphics/Graphics.cpp
              client/Graphics/Graphics.hpp
              client/Graphics/Skin.cpp
              client/Graphics/Skin.hpp
              client/Graphics/SkinsManager.cpp
              client/Graphics/SkinsManager.hpp
              client/Graphics/SkinSprite.cpp
              client/Graphics/SkinSprite.hpp
              client/Graphics/TileSet.cpp
              client/Graphics/TileSet.hpp
              client/Graphics/TileSprite.cpp
              client/Graphics/TileSprite.hpp
              client/Graphics/Window.cpp
              client/Graphics/Window.hpp

              client/Entities/Creature.cpp
              client/Entities/Creature.hpp
              client/Entities/MovementHandler.cpp
              client/Entities/MovementHandler.hpp
              client/Entities/Player.cpp
              client/Entities/Player.hpp
              client/Entities/Unit.cpp
              client/Entities/Unit.hpp
              client/Entities/WorldObject.cpp
              client/Entities/WorldObject.hpp
              ${RC})

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

find_package(SFML 2 REQUIRED system window graphics network)

include_directories(${SFML_INCLUDE_DIR})
target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES})

# Install target
install(TARGETS ${EXECUTABLE_NAME} DESTINATION bin)
