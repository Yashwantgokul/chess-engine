cmake_minimum_required(VERSION 3.19)
project(chessengine LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Multimedia Concurrent LinguistTools)

qt_standard_project_setup()

qt_add_executable(chessengine
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    board.cpp board.h
    movegen.cpp movegen.h
)

qt_add_translations(
    TARGETS chessengine
    TS_FILES chessengine_en_IN.ts
)

target_link_libraries(chessengine
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Multimedia
        Qt::Concurrent
)

install(TARGETS chessengine
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET chessengine
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
