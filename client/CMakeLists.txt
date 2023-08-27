cmake_minimum_required(VERSION 3.10)

# Project name
project(multiplayer-room) 

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)

#Indique que la compilation doit échouer si la norme spécifiée (C++23 dans cet exemple) n'est pas supportée par le compilateur.
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories, CMake will inform the compiler to also look in this directory when resolving #include directives.
include_directories(lib/SFML/include)
include_directories(lib/ENet/include)
include_directories(src/utils)
include_directories(src/game/core)
include_directories(src/game/entities)
include_directories(src/networking)


# Link directories (where the .a or .lib files are located)
link_directories(lib/SFML/lib)
link_directories(lib/ENet/lib)

# Source files 
set(SOURCE_FILES
    src/main.cpp
    src/utils/config.hpp
    src/utils/config.cpp

    src/game/core/Game.hpp
    src/game/core/Game.cpp
    src/game/entities/Player.hpp
    src/game/entities/Player.cpp

    src/networking/NetworkClient.cpp
    src/networking/NetworkClient.hpp
)

##############

# Spécifiez où les binaires doivent être créés
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)


# Set compiler options for Debug build
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # Indique quels sont les fichiers à compiler
    add_executable(debug ${SOURCE_FILES})     # <- this is the target

    target_compile_options(debug PRIVATE -Wall -Wextra -g)
    # -g: Cela permet au débogueur de montrer des valeurs de variable, de naviguer dans le code source.

    # Relie les différentes libraries à l'executable
    target_link_libraries(debug ${CMAKE_SOURCE_DIR}/lib/enet/lib/enet64.lib Ws2_32 Winmm sfml-system sfml-window sfml-graphics)
endif()

# Set compiler options for Release build
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    # Indique quels sont les fichiers à compiler
    add_executable(release ${SOURCE_FILES})     # <- this is the target

    target_compile_options(release PRIVATE -O3)
    # -03 : optimise le code pour que le programme soit plus rapide
    # Les options ou cibles spécifiées sont utilisées uniquement pour la cible sur laquelle vous appelez la commande. Elles ne sont pas propagées aux cibles qui sont liées à celle-ci.

    # Relie les différentes libraries à l'executable
    target_link_libraries(release enet sfml-system sfml-window sfml-graphics)
endif()



