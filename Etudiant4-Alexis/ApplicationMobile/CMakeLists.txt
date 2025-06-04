cmake_minimum_required(VERSION 3.16)

project(Application_Drone VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appApplication_Drone
    main.cpp
)

qt_add_qml_module(appApplication_Drone
    URI Application_Drone
    VERSION 1.0
    QML_FILES
        Main.qml
        RESOURCES
        RESOURCES ressource.qrc
        QML_FILES
        QML_FILES
        QML_FILES
        QML_FILES
        QML_FILES
        QML_FILES
        QML_FILES
        QML_FILES
        QML_FILES
        QML_FILES
)


# Spécifier l'icône pour Windows
if (WIN32)
    set(APP_ICON_RESOURCE ${CMAKE_SOURCE_DIR}/images/my_icon.ico)
    set_property(TARGET appApplication_Drone PROPERTY RESOURCE ${APP_ICON_RESOURCE})
endif()

target_link_libraries(appApplication_Drone
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS appApplication_Drone
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

