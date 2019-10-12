cmake_minimum_required(VERSION 3.15)
project(COMP345)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(COMP345
        Map/Map.cpp
        Map/Map.h
        Map/MapDriver.cpp Cards/Cards.cpp Cards/Cards.h Cards/CardsDriver.cpp Map/MapLoader/MapLoaderDriver.cpp Map/MapLoader/MapLoader.h Map/MapLoader/MapLoader.cpp Dice/Dice.cpp Dice/Dice.h Dice/DiceDriver.cpp Player/Player.h Player/Player.cpp Player/Player_driver.cpp)
