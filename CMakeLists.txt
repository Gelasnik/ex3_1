cmake_minimum_required(VERSION 3.15)
project(ex3)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(ex3
        tests/UniqueArrayTest.cpp
        custom_test.cpp
        MtmParkingLot.cpp
        ParkingLot.h
        ParkingLotPrinter.cpp
        ParkingLotPrinter.h
        ParkingLotTypes.h
        ParkingSpot.cpp
        ParkingSpot.h
        Time.cpp
        Time.h
        UniqueArray.h
        UniqueArrayImp.h
        ParkingLot.cpp)
