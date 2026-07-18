cmake_minimum_required(VERSION 3.19)
project(IndExingengine2 LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(IndExingengine2
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    BPlusTree.h
    HomeScreen.h
    CreateDBScreen.h
    InsertScreen.h
    ViewDBScreen.h
    DBOperationsScreen.h
    HomeScreen.cpp
    CreateDBScreen.cpp
    InsertScreen.cpp
    ViewDBScreen.cpp
    DBOperationsScreen.cpp



)

target_link_libraries(IndExingengine2
    PRIVATE
        Qt::Core
        Qt::Widgets
)

install(TARGETS IndExingengine2
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET IndExingengine2
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
