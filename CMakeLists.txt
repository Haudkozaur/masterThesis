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
    DataBaseManagers/CrossSectionsAssistant.cpp
    DataBaseManagers/CrossSectionsAssistant.h
    vcpkg.json
    GUI/gui.h
    GUI/gui.cpp
    GUI/gui.ui
    GUI/addpointdialog.h
    GUI/addpointdialog.cpp
    GUI/addpointdialog.ui
    GUI/addlinedialog.h
    GUI/addlinedialog.cpp
    GUI/addlinedialog.ui
    GUI/addboundariesdialog.h
    GUI/addboundariesdialog.cpp
    GUI/addboundariesdialog.ui
    GUI/deleteobjectdialog.h
    GUI/deleteobjectdialog.cpp
    GUI/deleteobjectdialog.ui
    resources.qrc
    GUI/editobjectdialog.h
    GUI/editobjectdialog.cpp
    GUI/editobjectdialog.ui
    GUI/addmaterialdialog.h
    GUI/addmaterialdialog.cpp
    GUI/addmaterialdialog.ui
    GUI/addcrosssectiondialog.h
    GUI/addcrosssectiondialog.cpp
    GUI/addcrosssectiondialog.ui
    GUI/setpropertiesdialog.h
    GUI/setpropertiesdialog.cpp
    GUI/setpropertiesdialog.ui
    GUI/addpointappliedforce.h
    GUI/addpointappliedforce.cpp
    GUI/addpointappliedforce.ui
    GUI/addlineloaddialog.h
    GUI/addlineloaddialog.cpp
    GUI/addlineloaddialog.ui
    GUI/basedialog.h
    GUI/basedialog.cpp
    DataBaseManagers/DataBaseManagers.h
    DataBaseManagers/databasesolverpreparer.h DataBaseManagers/databasesolverpreparer.cpp
    Solver/solver.h Solver/solver.cpp
    Solver/member.h Solver/member.cpp
    Solver/nodeload.h Solver/nodeload.cpp
    Solver/nodesupport.h Solver/nodesupport.cpp
    Solver/matrixoperations.h Solver/matrixoperations.cpp
    Solver/node.h Solver/node.cpp
    Solver/UniformLoad.h Solver/UniformLoad.cpp
    Solver/MemberSupportConditions.h Solver/membersupportconditions.cpp
    DataBaseManagers/DataBaseMeshManager.h DataBaseManagers/DataBaseMeshManager.cpp
    GUI/ViewDialog.h GUI/ViewDialog.cpp GUI/ViewDialog.ui
    DataBaseManagers/DataBaseResultsManager.h DataBaseManagers/DataBaseResultsManager.cpp
    GUI/StartWindow.h GUI/StartWindow.cpp GUI/StartWindow.ui
    SlabGUI/SlabGUI.h SlabGUI/SlabGUI.cpp SlabGUI/SlabGUI.ui
    SlabGUI/AddSurfaceDialog.h SlabGUI/AddSurfaceDialog.cpp SlabGUI/AddSurfaceDialog.ui
    DataBaseManagers/DataBaseSurfacesManager.h DataBaseManagers/DataBaseSurfacesManager.cpp
    DataBaseManagers/DataBaseCircularLinesManager.h DataBaseManagers/DataBaseCircularLinesManager.cpp
    SlabGUI/AddSlabSupportsDialog.h SlabGUI/AddSlabSupportsDialog.cpp SlabGUI/AddSlabSupportsDialog.ui
    DataBaseManagers/DataBaseLineSupportsManager.h DataBaseManagers/DataBaseLineSupportsManager.cpp





)

# linkowanie bibliotek
target_link_libraries(masterThesis Eigen3::Eigen SQLite::SQLite3 Qt6::Widgets Qt6::Core Qt6::Gui Qt6::UiTools)
