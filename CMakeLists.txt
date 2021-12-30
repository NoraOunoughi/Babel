project(babel)
cmake_minimum_required(VERSION 3.17)
add_definitions("-fPIC")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_compile_options(-fdiagnostics-color=always)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_compile_options(-fcolor-diagnostics)
endif()

# [Conan]
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# [RPath]
set(CMAKE_INSTALL_RPATH "$ORIGIN/../lib")
set(CMAKE_BUILD_WITH_INSTALL_RPATH ON)

# [C++ Standard]
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR}/ ${CMAKE_PREFIX_PATH})
message(WARNING ${CMAKE_BINARY_DIR})
message(WARNING ${CMAKE_PREFIX_PATH})
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

# [Qt]
#include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
#set(CMAKE_PREFIX_PATH ${CONAN_QT_ROOT})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5OpenGL CONFIG REQUIRED)
find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5Gui CONFIG REQUIRED)
find_package(Qt5Network CONFIG REQUIRED)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

# [Include directories]
include_directories(build)
include_directories(${CONAN_INCLUDE_DIRS})

# [Generate Executable]
add_executable(babel_client
    client/client.cpp
    client/BabelClient/BabelClient.cpp
    client/BabelClient/call.cpp
    client/BabelClient/user.cpp
    client/call/StreamHandler.cpp
    client/call/StreamEncoder.cpp
    client/call/Exception.cpp
    client/udp/ReceiverUdp.cpp
    client/udp/SenderUdp.cpp
    client/tcp/TCP_client.cpp
    client/tcp/ClientSession.cpp
    clientServer/Packet.cpp
    client/Qt_interface/mainwindow.cpp
    client/Qt_interface/mainwindow.h
    client/Qt_interface/mainwindow.ui
)

add_executable(babel_server
    server/server.cpp
    server/BabelServer/BabelServer.cpp
    server/BabelServer/BabelUser.cpp
    server/Network/TCP_server.cpp
    server/Network/Network.cpp
    server/Network/session.cpp
    clientServer/Packet.cpp
)

# [RPath]
#SET_TARGET_PROPERTIES(babel PROPERTIES INSTALL_RPATH "$ORIGIN/../lib")
#SET_TARGET_PROPERTIES(gui PROPERTIES INSTALL_RPATH "$ORIGIN/../lib")
#set(CMAKE_EXE_LINKER_FLAGS "-Wl,--disable-new-dtags")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath -Wl,$ORIGIN/../lib")

# if(ANDROID)
#     add_library(babel SHARED
#         client/Qt_interface/main.cpp
#         client/Qt_interface/mainwindow.cpp
#         client/Qt_interface/mainwindow.h
#         client/Qt_interface/mainwindow.ui
#     )
# else()
#     add_executable(babel
#         client/Qt_interface/main.cpp
#         client/Qt_interface/mainwindow.cpp
#         client/Qt_interface/mainwindow.h
#         client/Qt_interface/mainwindow.ui
#     )
# endif()

# target_link_libraries(babel PRIVATE Qt5::Widgets Qt5::OpenGL Qt5::Widgets Qt5::Core Qt5::Network)

# [Link libraries]
target_link_libraries(babel_server ${CONAN_LIBS})
target_link_libraries(babel_client ${CONAN_LIBS})
