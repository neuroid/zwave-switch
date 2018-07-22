cmake_minimum_required(VERSION 3.1)
project(zwave-switch)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Werror -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include
                    lib/argh)

set(OPENZWAVE_PREFIX /usr
    CACHE STRING "Search for OpenZWave in OPENZWAVE_PREFIX/include and OPENZWAVE_PREFIX/lib")
find_path(OPENZWAVE_INCLUDE_DIR openzwave/Manager.h
          PATHS ${OPENZWAVE_PREFIX}/include)
find_library(OPENZWAVE_LIBRARY openzwave
             PATHS ${OPENZWAVE_PREFIX}/lib)
include_directories(${OPENZWAVE_INCLUDE_DIR}
                    # Annoyingly, the library is not using relative imports
                    ${OPENZWAVE_INCLUDE_DIR}/openzwave)

add_executable(zwave-switch
               src/switch.cpp
               src/add.cpp
               src/config.cpp
               src/handler.cpp
               src/on.cpp
               src/remove.cpp
               src/remove_failed.cpp)
target_link_libraries(zwave-switch
                      ${OPENZWAVE_LIBRARY})
