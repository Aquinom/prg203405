cmake_minimum_required(VERSION 3.19)
project(regicide LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(regicide
    WIN32 MACOSX_BUNDLE
    main.cpp
    card.cpp
    card.h
    deck.cpp
    deck.h
    player.cpp
    player.h
    enemy.cpp
    enemy.h
    gamestate.cpp
    gamestate.h
    gamewindow.cpp
    gamewindow.h
)

target_link_libraries(regicide
    PRIVATE
        Qt::Core
        Qt::Widgets
)

include(GNUInstallDirs)

install(TARGETS regicide
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET regicide
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
