cmake_minimum_required(VERSION 3.10)
project(masterThesis)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)


# wykrycie vcpkg
if(DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
endif()

# wczytanie konfigu dla bibliotek (wymaga ustawienia zmiennej środowiskowej)
include("${CMAKE_TOOLCHAIN_FILE}")

# używane biblioteki
find_package(Eigen3 3.4 REQUIRED NO_MODULE)
find_package(SQLite3 REQUIRED)

find_package(Qt6Widgets REQUIRED)
find_package(Qt6Core REQUIRED)
find_package(Qt6Gui REQUIRED)
find_package(Qt6UiTools REQUIRED)

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
    gui.h gui.cpp gui.ui
    addpointdialog.h addpointdialog.cpp addpointdialog.ui
    addlinedialog.h addlinedialog.cpp addlinedialog.ui
    addboundariesdialog.h addboundariesdialog.cpp addboundariesdialog.ui
    deleteobjectdialog.h deleteobjectdialog.cpp deleteobjectdialog.ui
    resources.qrc
    editobjectdialog.h editobjectdialog.cpp editobjectdialog.ui
)

# linkowanie bibliotek
target_link_libraries(masterThesis Eigen3::Eigen SQLite::SQLite3 Qt6::Widgets Qt6::Core Qt6::Gui Qt6::UiTools)
