cmake_minimum_required(VERSION 3.23)
# CMAKE_TOOLCHAIN_FILE(C:/Users/joelh/Documents/CppStuff/vcpkg/scripts/buildsystems/vcpkg.cmake)
project(mrmp_implementation)

set(CMAKE_CXX_STANDARD 14)

add_executable(mrmp_implementation main.cpp RandomGenerator.cpp RandomGenerator.h WorkspaceGenarator.cpp WorkspaceGenarator.h WSFreeSpaceGenerator.cpp WSFreeSpaceGenerator.h mytypedefs.h constants.h)

include(C:/Users/joelh/Documents/CppStuff/vcpkg/scripts/buildsystems/vcpkg.cmake)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

#find_package(Qt5 REQUIRED)

# Link with CGAL
find_package(CGAL REQUIRED)
find_package(CGAL COMPONENTS Qt5)
find_package(PkgConfig REQUIRED)
find_package(Qt5 REQUIRED
        Core
        PrintSupport
        Gui
        Widgets
        )

pkg_check_modules(gmp REQUIRED IMPORTED_TARGET gmp)

target_link_libraries(mrmp_implementation PkgConfig::gmp)
target_link_libraries(mrmp_implementation CGAL::CGAL)
target_link_libraries(mrmp_implementation Qt5::Widgets)
#target_link_libraries(mrmp_implementation CGAL::CGAL_Qt5)
target_link_libraries(mrmp_implementation CGAL::CGAL_Basic_viewer)
