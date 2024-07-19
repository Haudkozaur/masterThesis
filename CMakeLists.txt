cmake_minimum_required(VERSION 3.10)
project(masterThesis)

set(CMAKE_CXX_STANDARD 14)

# Dodaj VCPKG do CMake
if(DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
endif()

# Wczytaj konfigurację z VCPKG
include("${CMAKE_TOOLCHAIN_FILE}")

# Znajdź pakiety Eigen3 i SQLite3
find_package(Eigen3 3.4 REQUIRED NO_MODULE)
find_package(SQLite3 REQUIRED)

add_executable(masterThesis
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
    toStringWithPrecision.h
    DataBaseLineLoadsManager.cpp
    DataBaseLineLoadsManager.h
    CrossSectionsAssistant.cpp
    CrossSectionsAssistant.h
    vcpkg.json
)

# Linkowanie bibliotek Eigen3 i SQLite3
target_link_libraries(masterThesis Eigen3::Eigen SQLite::SQLite3)
