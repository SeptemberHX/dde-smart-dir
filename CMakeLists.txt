cmake_minimum_required(VERSION 3.11)
set(CMAKE_CXX_STANDARD 17)

set(PLUGIN_NAME "dde-smart-dir")

project(${PLUGIN_NAME})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(GLOB SRCS "SmartDirPlugin.h" "SmartDirPlugin.cpp"
        "SmartDirPluginWidget.h" "SmartDirPluginWidget.cpp"
        "SmartDirSettings.h" "SmartDirSettings.cpp"
        "SmartDirUtils.h" "SmartDirUtils.cpp"
        "widgets/smartdirsettingwidget.h" "widgets/smartdirsettingwidget.cpp" "widgets/smartdirsettingwidget.ui"
        "widgets/SpecialLabel.h" "widgets/SpecialLabel.cpp"
        "widgets/SmartDirItemWidget.h" "widgets/SmartDirItemWidget.cpp"
        "widgets/smartdirwidget.h" "widgets/smartdirwidget.cpp" "resource.qrc"
        "DirsWatcher.h" "DirsWatcher.cpp")

find_package(Qt5Widgets REQUIRED)
find_package(DtkWidget REQUIRED)
find_package(Qt5LinguistTools REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(DdeDockInterface REQUIRED dde-dock)

add_definitions("${QT_DEFINITIONS} -DQT_PLUGIN")


add_library(${PLUGIN_NAME} SHARED ${SRCS} ${UIS_HDRS} ${QM_FILES})

#add_executable(${PLUGIN_NAME} ${SRCS})

target_include_directories(${PLUGIN_NAME} PUBLIC
        ${Qt5Widgets_INCLUDE_DIRS}
        ${DtkWidget_INCLUDE_DIRS}
        ${DdeDockInterface_INCLUDE_DIRS}
        )

target_compile_definitions(${PLUGIN_NAME} PUBLIC
        QT_DEBUG_NO_OUTPUT
        )

target_link_libraries(${PLUGIN_NAME} PRIVATE
        ${Qt5Widgets_LIBRARIES}
        ${DtkWidget_LIBRARIES}
        ${DdeDockInterface_LIBRARIES}
        )

INSTALL(TARGETS ${PLUGIN_NAME} DESTINATION /usr/lib/dde-dock/plugins/)

SET(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_NAME dde-dock-smartdir-plugin)
set(CPACK_DEBIAN_PACKAGE_VERSION 0.4.0-rc1)
set(CPACK_DEBIAN_PACKAGE_RELEASE 1)
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "SeptemberHX<september_hx@outlook.com>")
set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "Smart Directory inspired by smart folder on macOs")
set(CPACK_DEBIAN_FILE_NAME "${CPACK_DEBIAN_PACKAGE_NAME}-${CPACK_DEBIAN_PACKAGE_VERSION}-${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}.deb")

include(CPack)
