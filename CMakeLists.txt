cmake_minimum_required(VERSION 3.15)
project(COMP345)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(COMP345
        Map.cpp
        Map.h
        MapDriver.cpp Cards/Cards.cpp Cards/Cards.h Cards/CardsDriver.cpp MapLoaderDriver.cpp MapLoader.h MapLoader.cpp Dice/Dice.cpp Dice/Dice.h Dice/DiceDriver.cpp Player.h Player.cpp Player_driver.cpp)
