# CMakeList.txt : CMake project for TrucoGame, include source and define
# project specific logic here.
#

cmake_minimum_required(VERSION 3.14)

project(TrucoGame)

include(FetchContent)

# C++ version ===========================================================================

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# External libs =========================================================================

set(FETCHCONTENT_QUIET FALSE)
set(FETCHCONTENT_UPDATES_DISCONNECTED ON)

#Dear SFML
FetchContent_Declare(
    SFML
    URL ${SPASS_REPO_URL_PREFIX}/SFML/SFML/archive/69ea0cd863aed1d4092b970b676924a716ff718b.zip
)
FetchContent_MakeAvailable(SFML)

#JSON for modern C++
FetchContent_Declare(
    nlohmann
    URL ${SPASS_REPO_URL_PREFIX}/onavratil-monetplus/json/archive/e7b3b40b5a95bc74b9a7f662830a27c49ffc01b4.zip    
)
FetchContent_MakeAvailable(nlohmann)

# Include subdirectories
add_subdirectory(src)
add_subdirectory(include)