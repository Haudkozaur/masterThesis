cmake_minimum_required(VERSION 3.27)
project(masterThesis)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(masterThesis
        Header.h
        masterThesis.cpp
        MatrixAggregator.cpp
        MatrixAggregator.h
        matrixHandler.cpp
        matrixHandler.h
        RodsHandler.cpp
        RodsHandler.h
        sqlite/sqlite3.h
        sqlite/sqlite3.c
        DataBaseStarter.h
        DataBaseStarter.cpp
        DataBaseModelObjectsManager.cpp
        DataBaseModelObjectsManager.h
        DataBasePointsManager.cpp
        DataBasePointsManager.h
        DataBaseLinesManager.cpp
        DataBaseLinesManager.h
        TableTypes.h
        DateBaseCrossSectionsManager.cpp
        DataBaseCrossSectionsManager.h
        DataBaseMaterialsManager.cpp
        DataBaseMaterialsManager.h
        DataBaseSupportsManager.cpp
        DataBaseSupportsManager.h
        DataBaseNodalLoadsManager.cpp
        DataBaseNodalLoadsManager.h
)
