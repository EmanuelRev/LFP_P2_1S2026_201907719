cmake_minimum_required(VERSION 3.19)
project(Proyecto_2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(Proyecto_2
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    token.h token.cpp
    errormanager.h errormanager.cpp
    lexicalanalyzer.h lexicalanalyzer.cpp
    syntaxanalyzer.h syntaxanalyzer.cpp
    reportgenerator.h reportgenerator.cpp
)

target_link_libraries(Proyecto_2
    PRIVATE
        Qt::Core
        Qt::Widgets
)

install(TARGETS Proyecto_2
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET Proyecto_2
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
