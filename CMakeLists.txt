cmake_minimum_required(VERSION 3.9)
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
        "widgets/smartdirwidget.h" "widgets/smartdirwidget.cpp" "resource.qrc")

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
