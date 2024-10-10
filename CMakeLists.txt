cmake_minimum_required(VERSION 3.29)
set(TARGET_NAME sing-tray)
project(${TARGET_NAME})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable(${TARGET_NAME} main.cpp
        Extension/DottedIcon.cpp
        Extension/DottedIcon.h
        Sys/SystemCtl.cpp
        Sys/SystemCtl.h
        Sys/AutoRun.cpp
        Sys/AutoRun.h
        Extension/RouteManager.cpp
        Extension/RouteManager.h)

configure_file("./Resources/Icon.png" Icon.png COPYONLY)

find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)
target_link_libraries(${TARGET_NAME} Qt::Core Qt::Gui Qt::Widgets)

set(Drivers_Os_Compile_Options
        -flto
        -fdata-sections
        -ffunction-sections
        $<$<COMPILE_LANGUAGE:CXX>: -Wno-pmf-conversions>
)

set(Drivers_Os_Link_Options
        ${Drivers_Os_Compile_Options}
        -Wl,--gc-sections
)

target_compile_options(${TARGET_NAME} PRIVATE ${Drivers_Os_Compile_Options})
target_link_options(${TARGET_NAME} PRIVATE ${Drivers_Os_Link_Options})

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(${TARGET_NAME} PRIVATE -O0 -g)
    target_link_options(${TARGET_NAME} PRIVATE -O0 -g)
    target_compile_definitions(${TARGET_NAME} PRIVATE DEBUG)
else ()
    target_compile_options(${TARGET_NAME} PRIVATE -Os)
    target_link_options(${TARGET_NAME} PRIVATE -Os -Wl,--strip-all)
endif ()
