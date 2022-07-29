cmake_minimum_required(VERSION 3.12)

project(yoloRGBD VERSION 0.3 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/iconwin/app.rc")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_VIDEO")
find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Widgets  Quick REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Widgets  Quick REQUIRED)
find_package(OpenCV REQUIRED)
find_package(InferenceEngine REQUIRED)
find_package(ngraph REQUIRED)
find_package(OpenMP REQUIRED)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${OpenCV_INCLUDE_DIRS}
    ${InferenceEngine_INCLUDE_DIR}
)

set(PROJECT_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/include/utility.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/statusmonitor.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/threadcontroller.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/imageprocess.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ocvimageprovider.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/cpuinfer.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/statusmonitor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/src/utility.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/threadcontroller.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/imageprocess.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ocvimageprovider.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cpuinfer.cpp
        qml.qrc
)



if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(yoloRGBD
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET yoloRGBD APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(yoloRGBD SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(yoloRGBD
          ${PROJECT_SOURCES}
          ${APP_ICON_RESOURCE_WINDOWS}
        )
    endif()
endif()

target_compile_definitions(yoloRGBD
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(yoloRGBD
  PRIVATE
      Qt${QT_VERSION_MAJOR}::Core
      Qt${QT_VERSION_MAJOR}::Quick
      Qt${QT_VERSION_MAJOR}::Widgets
      ${OpenCV_LIBS}
      ${InferenceEngine_LIBRARIES}
      ${NGRAPH_LIBRARIES}
      ${OpenCV_LIBS}
      OpenMP::OpenMP_CXX)

set_target_properties(yoloRGBD PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_import_qml_plugins(yoloRGBD)
    qt_finalize_executable(yoloRGBD)
endif()
