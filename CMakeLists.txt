cmake_minimum_required(VERSION 3.15)
project(COMP345)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(.)

add_executable(COMP345 GameEngine/GameEngineRunner.cpp GameEngine/GameEngine.cpp Map/Map.cpp Map/MapLoader/MapLoader.cpp Player/Player.cpp Cards/Cards.cpp Dice/Dice.cpp)