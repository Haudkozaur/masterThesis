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

# dodajemy foldery DataBaseManagers i sqlite do ścieżek include
include_directories(${CMAKE_SOURCE_DIR}/DataBaseManagers ${CMAKE_SOURCE_DIR}/sqlite)

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
    DataBaseManagers/DataBaseStarter.h
    DataBaseManagers/DataBaseStarter.cpp
    DataBaseManagers/DataBaseModelObjectsManager.cpp
    DataBaseManagers/DataBaseModelObjectsManager.h
    DataBaseManagers/DataBasePointsManager.cpp
    DataBaseManagers/DataBasePointsManager.h
    DataBaseManagers/DataBaseLinesManager.cpp
    DataBaseManagers/DataBaseLinesManager.h
    DataBaseManagers/TableTypes.h
    DataBaseManagers/DateBaseCrossSectionsManager.cpp
    DataBaseManagers/DataBaseCrossSectionsManager.h
    DataBaseManagers/DataBaseMaterialsManager.cpp
    DataBaseManagers/DataBaseMaterialsManager.h
    DataBaseManagers/DataBaseSupportsManager.cpp
    DataBaseManagers/DataBaseSupportsManager.h
    DataBaseManagers/DataBaseNodalLoadsManager.cpp
    DataBaseManagers/DataBaseNodalLoadsManager.h
    DataBaseManagers/toStringWithPrecision.h
    DataBaseManagers/DataBaseLineLoadsManager.cpp
    DataBaseManagers/DataBaseLineLoadsManager.h
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
    addmaterialdialog.h addmaterialdialog.cpp addmaterialdialog.ui
    addcrosssectiondialog.h addcrosssectiondialog.cpp addcrosssectiondialog.ui
    setpropertiesdialog.h setpropertiesdialog.cpp setpropertiesdialog.ui
    addpointappliedforce.h addpointappliedforce.cpp addpointappliedforce.ui
    addlineloaddialog.h addlineloaddialog.cpp addlineloaddialog.ui
)

# linkowanie bibliotek
target_link_libraries(masterThesis Eigen3::Eigen SQLite::SQLite3 Qt6::Widgets Qt6::Core Qt6::Gui Qt6::UiTools)
